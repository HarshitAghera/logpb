#pragma once

#include <parse_error.h>
#include <hash_map.h>
#include <plotter.h>
#include <memory>

class QWidget;

struct Plot_Info;
class CSV_Logger;
class CSV_Logger_Info;
class Session_Serializer;

namespace logpb {
class Message_Def_Gen;
class Device_Connection;

class Session {
public:
    Session();
    ~Session();

    const Message_Def_Gen& get_msg_defs() { return *msg_defs; };

    Device_Connection& get_connection() { return *connection; }
    void set_connection(std::unique_ptr<Device_Connection> con);

    Device_Connection& create_file_connection(const std::string_view file_name);

    File_Error_Collector add_msg_def(std::string file);

    int add_csv_logger(const std::string& msg_name, const std::string_view file,
                       const std::string_view del, const std::string_view lb);

    void add_numeric_plotter(Plot_Info& plot, QWidget* parent);

    void reset_msg_defs();

    int parse_msgs();

    void refresh_msg_defs();

    friend Session_Serializer;

private:
    std::unique_ptr<Message_Def_Gen> msg_defs;
    std::vector<std::string> def_files;

    std::unique_ptr<Device_Connection> connection;
    std::string file_connection_file_path;

    std::vector<std::unique_ptr<CSV_Logger>> csv_loggers;
    std::vector<CSV_Logger_Info> csv_logger_infos;

    std::vector<std::unique_ptr<Plotter>> plotters;
    S_Container<std::string, const int> plotter_registery;

    std::vector<std::unique_ptr<Curve>> curves;
    std::vector<Curve_Serializer> curve_serializers;

    std::vector<Plot_Info> plots;

};
}  // namespace logpb
