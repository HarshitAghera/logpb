#pragma once

#include <base/msg_type_gen.h>
#include <base/device_connection.h>

namespace logpb {
class Session {
public:
    Session();

    const Message_Def_Gen& get_msg_defs() { return *msg_defs; };

    Device_Connection& get_connection() { return *connection; }
    void set_connection(std::unique_ptr<Device_Connection> con) {
        connection = std::move(con);
    }

    File_Error_Collector add_msg_def(std::string file);

    int add_csv_logger(const std::string& msg_name,
                       std::unique_ptr<CSV_Logger> logger);

    void reset_msg_defs();

    int parse_msgs();

    void refresh_msg_defs() { msg_defs->refresh_registery(); };

private:
    std::unique_ptr<Message_Def_Gen> msg_defs;
    std::unique_ptr<Device_Connection> connection;
    std::vector<std::unique_ptr<CSV_Logger>> csv_loggers;

    std::vector<std::string> def_files;
};
}  // namespace logpb
