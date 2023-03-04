#pragma once

#include <base/msg_type_gen.h>

namespace logpb {
class Session {
public:
    const Message_Def_Gen& get_msg_defs() { return msg_defs; };

    File_Error_Collector add_msg_def(std::string file);

    int parse_msgs();

private:
    Message_Def_Gen msg_defs;

    std::vector<std::string> def_files;
};
}  // namespace logpb
