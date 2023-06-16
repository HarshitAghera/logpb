#include "session.h"

#include <msg_type_gen.h>
#include <device_connection.h>
#include <logger.h>

namespace logpb {
Session::Session() : msg_defs{new Message_Def_Gen} {}

Session::~Session() = default;

void Session::reset_msg_defs() {
    msg_defs = std::make_unique<Message_Def_Gen>();
}

void Session::set_connection(std::unique_ptr<Device_Connection> con) {
    connection = std::move(con);
}

void Session::refresh_msg_defs() { msg_defs->refresh_registery(); };

File_Error_Collector Session::add_msg_def(std::string file) {
    File_Error_Collector error = msg_defs->import_def_file(file);
    def_files.push_back(file);

    return error;
}

int Session::add_csv_logger(const std::string& msg_name,
                            std::unique_ptr<CSV_Logger> logger) {
    get_msg_defs().register_logger(msg_name, logger.get());

    csv_loggers.push_back(std::move(logger));

    return 0;
}

QWidget* Session::add_numeric_plotter(Plot_Info& plot, QWidget* parent) {
    for (auto& curve : plot.curves) {
        auto add_plotter = [&](const std::string& field) -> int {
            const int* plot_index = plotter_registery.find(field);

            if (plot_index) {
                return *plot_index;
            } else {
                plotters.push_back(std::make_unique<Plotter>());
                msg_defs->register_plotter(field, plotters.back().get());

                return plotters.size() - 1;
            }
        };

        curve_serializers.push_back(Curve_Serializer{
            add_plotter(curve.x_field), add_plotter(curve.y_field)});

        curves.push_back(std::make_unique<Curve>(
            curve_serializers.back().deserialize(plotters)));
        curve.curve = curves.back().get();
    }

    plots.push_back(Basic_Plot{parent, &plot});

    return plots.back().get_plot_widget();
}

int Session::update_and_redraw_plots() {
    for (auto& plot : plots) {
        plot.update_and_redraw();
    }

    return 0;
}
}  // namespace logpb
