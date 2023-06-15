#pragma once

#include <string>

namespace logpb {
class Session;

int save_session(std::string& file_path, logpb::Session& session);

}
