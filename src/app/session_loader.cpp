#include "session_loader.h"

#include "session.h"
#include <device_connection.h>
#include <logger.h>
#include <toml++/toml.h>

namespace logpb {

int Session_Serializer::serialize(const Session& session) {
    toml::table table;

    //---------------- message definitions ------------------//
    toml::table def_table;

    toml::array def_files;
    def_files.insert(def_files.begin(), session.def_files.begin(),
                     session.def_files.end());

    def_table.emplace(DEF_FILE, std::move(def_files));
    table.emplace(MSG_DEF_TABLE, std::move(def_table));
    //------------------------------------------------------//

    //---------------- device connections ------------------//
    toml::table con_table;

    if (session.connection.get()) {
        std::string_view con_type{session.connection->name()};
        con_table.emplace(DEVICE_CONN_TYPE, con_type);
        if (con_type == FILE_CONNECTION) {
            toml::array file_connection{session.file_connection_file_path};
            con_table.emplace(FILE_CONNECTION, std::move(file_connection));
        } else if (con_type == COM_CONNECTION) {
            toml::array com_connection;
            con_table.emplace(COM_CONNECTION, std::move(com_connection));
        }
    }

    table.emplace(DEVICE_CONN_TABLE, std::move(con_table));
    //------------------------------------------------------//

    //---------------------- loggers -----------------------//
    toml::table loggers;

    toml::array csv_loggers;
    for (auto& cl : session.csv_logger_infos) {
        toml::array logger{cl.msg_name, cl.file_path, cl.del, cl.lb};

        csv_loggers.push_back(std::move(logger));
    }

    loggers.emplace(CSV_LOGGER, std::move(csv_loggers));
    table.emplace(LOGGER_TABLE, std::move(loggers));
    //------------------------------------------------------//

    // ----------------------- plotters --------------------//
    toml::table plotters;

    toml::array numeric_plotters;
    for (auto& msg_name : session.plotter_registery.get_key_list()) {
        numeric_plotters.push_back(msg_name);
    }

    plotters.emplace(NUMERIC_PLOTTER, std::move(numeric_plotters));
    table.emplace(PLOTTER_TABLE, std::move(plotters));
    //------------------------------------------------------//

    // ------------------------ curves ---------------------//
    toml::table curves;

    toml::array curves_2d;
    for (auto& curve : session.curve_serializers) {
        curves_2d.push_back(toml::array{curve.get_x(), curve.get_y()});
    }

    curves.emplace(PLOT_CURVE, std::move(curves_2d));
    table.emplace(CURVE_TABLE, std::move(curves));
    //------------------------------------------------------//

    // ------------------------ plots ---------------------//
    toml::table plots;

    toml::array plot_curves;
    for (auto& plot_info : session.plots) {
        toml::array curve;
        for (auto& c : plot_info.curves) {
            curve.push_back(c.curve_index);
        }
        plot_curves.push_back(std::move(curve));
    }

    plots.emplace(BASIC_PLOT, std::move(plot_curves));
    table.emplace(PLOT_TABLE, std::move(plots));
    //------------------------------------------------------//

    std::cout << table << '\n';
    return 0;
}

}  // namespace logpb
