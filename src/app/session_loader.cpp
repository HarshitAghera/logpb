#include "session_loader.h"

#include "session.h"
#include <toml++/toml.h>

namespace logpb {

int save_session(const std::string& file_path, const logpb::Session& session) {
    auto session_template = toml::parse_file(file_path);

    std::cout << session_template << '\n';

    return 0;
}
}
