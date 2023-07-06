#include "session.h"

#include <msg_type_gen.h>
#include <device_connection.h>
#include <device_connection_impl.h>
#include <logger.h>

#include <toml++/toml.h>

namespace logpb {
Session::Session() : msg_defs{new Message_Def_Gen} {}

Session::~Session() = default;

void Session::reset_msg_defs() {
    msg_defs = std::make_unique<Message_Def_Gen>();
}

void Session::set_connection(std::unique_ptr<Device_Connection> con) {
    connection = std::move(con);
}

Device_Connection& Session::create_file_connection(
    const std::string_view file_name) {
    file_connection_file_path = file_name;
    connection = std::make_unique<logpb::File_Connection>(
        logpb::File_Connection::create_from_path(file_connection_file_path));

    return *connection;
}

void Session::refresh_msg_defs() { msg_defs->refresh_registery(); };

File_Error_Collector Session::add_msg_def(std::string file) {
    File_Error_Collector error = msg_defs->import_def_file(file);
    def_files.push_back(file);

    return error;
}

int Session::add_csv_logger(const std::string& msg_name,
                            const std::string_view file,
                            const std::string_view del,
                            const std::string_view lb) {
    csv_logger_infos.emplace_back(
        CSV_Logger_Info{.msg_name{msg_name},
                        .file_path = std::string{file},
                        .del = std::string{del},
                        .lb = std::string{lb}});

    csv_loggers.emplace_back(std::make_unique<CSV_Logger>(file, del, lb));

    get_msg_defs().register_logger(msg_name, csv_loggers.back().get());

    return 0;
}

void Session::add_numeric_plotter(Plot_Info& plot) {
    auto add_plotter = [&](const std::string& field) -> int {
        const int* plot_index = plotter_registery.find(field);

        if (plot_index) {
            return *plot_index;
        } else {
            plotters.push_back(std::make_unique<Plotter>());
            const int index = plotters.size() - 1;
            plotter_registery.insert({field, index});
            msg_defs->register_plotter(field, plotters.back().get());

            return index;
        }
    };

    for (auto& curve : plot.curves) {
        curve_serializers.push_back(Curve_Serializer{
            add_plotter(curve.x_field), add_plotter(curve.y_field)});

        curves.push_back(std::make_unique<Curve>(
            curve_serializers.back().deserialize(plotters)));
        curve.curve = curves.back().get();
        curve.curve_index = curves.size() - 1;
    }

    plots.push_back(plot);

    // return plots.back().get_plot_widget();
}

void Session::update_parser() {
    parser = std::make_unique<logpb::Stream_Parser>(
        get_connection().get_stream(), &get_msg_defs());
}

void Session::parse_data() { parser->parse(); }
}  // namespace logpb
