#include "session.h"

namespace logpb {
Session::Session() : msg_defs{new Message_Def_Gen} {}

void Session::reset_msg_defs() {
    msg_defs = std::make_unique<Message_Def_Gen>();
}

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
}  // namespace logpb
