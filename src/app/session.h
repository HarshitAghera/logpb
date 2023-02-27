#pragma once

#include <msg_type_gen.h>

namespace logpb {
class Session {
public:
    Session();

    int add_msg_defs(std::vector<std::string> files);
    int parse_msgs();

private:
    Message_Def_Gen msg_defs;

    std::vector<std::string> def_files;
};
}  // namespace logpb
