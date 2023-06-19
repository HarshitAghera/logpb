#pragma once

#include <string>
#include <string_view>

using namespace std::string_view_literals;

namespace logpb {
class Session;

class Session_Serializer {
    static constexpr auto MSG_DEF_TABLE = "message_definitions"sv;
    static constexpr auto DEVICE_CONN_TABLE = "device_connections"sv;
    static constexpr auto DEVICE_CONN_TYPE = "type"sv;
    static constexpr auto LOGGER_TABLE = "loggers"sv;
    static constexpr auto PLOTTER_TABLE = "plotters"sv;
    static constexpr auto CURVE_TABLE = "curves"sv;
    static constexpr auto PLOT_TABLE = "plots"sv;
    static constexpr auto DEF_FILE = "def_files"sv;
    static constexpr auto FILE_CONNECTION = "file_connection"sv;
    static constexpr auto COM_CONNECTION = "com_connection"sv;
    static constexpr auto CSV_LOGGER = "csv_loggers"sv;
    static constexpr auto NUMERIC_PLOTTER = "numeric_plotters"sv;
    static constexpr auto PLOT_CURVE = "2d_curves"sv;
    static constexpr auto BASIC_PLOT = "basic_plots"sv;

public:
    int serialize(const Session& session);
    int deserialize(const std::string_view file_path, Session& session);
};
}
