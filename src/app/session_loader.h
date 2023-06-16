#pragma once

#include <string>

namespace logpb {
class Session;

int save_session(const std::string& file_path, const logpb::Session& session);
}
