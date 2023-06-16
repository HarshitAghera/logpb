#pragma once

#include <parse_error.h>
#include <hash_map.h>
#include <plotter.h>
#include <memory>

class QWidget;

struct Plot_Info;
class CSV_Logger;

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

    File_Error_Collector add_msg_def(std::string file);

    int add_csv_logger(const std::string& msg_name,
                       std::unique_ptr<CSV_Logger> logger);

    void add_numeric_plotter(Plot_Info& plot, QWidget* parent);

    void reset_msg_defs();

    int parse_msgs();

    void refresh_msg_defs();

private:
    std::unique_ptr<Message_Def_Gen> msg_defs;
    std::unique_ptr<Device_Connection> connection;

    std::vector<std::unique_ptr<CSV_Logger>> csv_loggers;

    std::vector<std::unique_ptr<Plotter>> plotters;
    S_Container<std::string, const int> plotter_registery;

    std::vector<std::unique_ptr<Curve>> curves;
    std::vector<Curve_Serializer> curve_serializers;

    std::vector<Plot_Info> plots;

    std::vector<std::string> def_files;
};
}  // namespace logpb
