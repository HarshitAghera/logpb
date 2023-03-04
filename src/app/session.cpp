#include "session.h"

namespace logpb {
File_Error_Collector Session::add_msg_def(std::string file) {
    File_Error_Collector error = msg_defs.import_def_file(file);
    def_files.push_back(file);

    return error;
}
}  // namespace logpb
