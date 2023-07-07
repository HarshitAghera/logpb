#include "session_loader.h"

#include <fstream>

#include "session.h"
#include <plotter.h>
#include <device_connection.h>
#include <logger.h>

#define TOML_EXCEPTIONS 0
#include <toml++/toml.h>

namespace logpb {

int Session_Serializer::serialize(const std::string_view file_path,
                                  const Session& session) {
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

    std::ofstream file{std::string{file_path}, std::ios_base::out};
    file << table << '\n';

    std::cout << table << "\n\n\n";
    return 0;
}

int Session_Serializer::deserialize(const std::string_view file_path,
                                    Session& session,
                                    Plot_Widget_Factory* pwf) {
    toml::parse_result result = toml::parse_file(file_path);

    if (! result) {
        return -1;
    }

    toml::table table = std::move(result).table();

    std::cout << table << "\n\n\n";

    session.reset_msg_defs();
    //---------------- message definitions ------------------//
    auto def_files = table[MSG_DEF_TABLE][DEF_FILE].as_array();
    if (!def_files) {
        return -1;
    }

    def_files->for_each([&](auto& el) {
        auto fp = el.as_string();

        if (fp) {
            session.add_msg_def(std::string{fp->value_or(""sv)});
        }
    });
    //------------------------------------------------------//
    session.refresh_msg_defs();

    //---------------- device connections ------------------//
    auto device_conn_table = table[DEVICE_CONN_TABLE].as_table();
    if (!device_conn_table) {
        return -1;
    }

    auto con_type = (*device_conn_table)[DEVICE_CONN_TYPE].as_string();
    if (!con_type) {
        return -1;
    }

    auto type = con_type->value_or(""sv);

    if (type == FILE_CONNECTION) {
        auto file_name =
            (*device_conn_table)[FILE_CONNECTION][0].value_or(""sv);
        if (file_name.empty()) {
            return -1;
        }

        session.create_file_connection(file_name);
    } else if (type == COM_CONNECTION) {
    }
    //------------------------------------------------------//

    //---------------------- loggers -----------------------//
    auto logger_table = table[LOGGER_TABLE].as_table();
    if (!logger_table) {
        return -1;
    }

    auto csv_loggers = (*logger_table)[CSV_LOGGER].as_array();

    if (!csv_loggers) {
        return -1;
    }

    csv_loggers->for_each([&](auto& cl) {
        auto logger = cl.as_array();

        if (logger) {
            session.add_csv_logger((*logger)[0].value_or(std::string{}),
                                   (*logger)[1].value_or(""sv),
                                   (*logger)[2].value_or(""sv),
                                   (*logger)[3].value_or(""sv));
        }
    });
    // ----------------------- plotters --------------------//
    auto plotter_table = table[PLOTTER_TABLE].as_table();
    if (!plotter_table) {
        return -1;
    }

    auto numeric_plotters = (*plotter_table)[NUMERIC_PLOTTER].as_array();
    if (!numeric_plotters) {
        return -1;
    }

    // int index{};

    numeric_plotters->for_each([&](auto& elem) {
        // auto np = elem.as_string()->value_or(std::string{});

        // session.plotters.push_back(std::make_unique<Plotter>());
        // session.plotter_registery.insert({np, index});
        // session.msg_defs->register_plotter(np,
        // session.plotters.back().get());

        // ++index;
    });
    //------------------------------------------------------//

    // ------------------------ curves ---------------------//
    auto curve_table = table[CURVE_TABLE].as_table();
    if (!curve_table) {
        return -1;
    }

    auto plot_curves = (*curve_table)[PLOT_CURVE].as_array();
    if (!plot_curves) {
        return -1;
    }

    plot_curves->for_each([](auto& elem) {
        // auto curve = elem.as_array();
    });
    //------------------------------------------------------//

    // ------------------------ plots ---------------------//
    auto plot_table = table[PLOT_TABLE].as_table();
    if (!plot_table) {
        return -1;
    }

    auto basic_plots = (*plot_table)[BASIC_PLOT].as_array();
    if (!basic_plots) {
        return -1;
    }

    basic_plots->for_each([&](auto& bp) {
        auto plot = bp.as_array();

        Plot_Curve_Fields plot_info;

        plot->for_each([&](auto& elem) {
            auto curve_index = elem.as_integer()->value_or(0);

            int x_index =
                (*plot_curves)[curve_index].as_array()->at(0).value_or(0);
            int y_index =
                (*plot_curves)[curve_index].as_array()->at(1).value_or(0);

            plot_info.curves.push_back(
                {.x_field =
                     (*numeric_plotters)[x_index].value_or(std::string{}),
                 .y_field =
                     (*numeric_plotters)[y_index].value_or(std::string{})});
        });

        session.add_numeric_plotter(plot_info, pwf);
    });
    //------------------------------------------------------//

    return 0;
}
}  // namespace logpb
