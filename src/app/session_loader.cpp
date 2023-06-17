#include "session_loader.h"

#include "session.h"
#include <toml++/toml.h>

namespace logpb {

int Session_Serializer::serialize(const Session& session) {
    toml::table table;

    toml::array def_files;
    def_files.insert(def_files.begin(), session.def_files.begin(),
                     session.def_files.end());
    toml::table def_table;
    def_table.emplace(DEF_FILE, std::move(def_files));
    table.emplace(MSG_DEF_TABLE, std::move(def_table));

    std::cout << table << '\n';
    return 0;
}

}  // namespace logpb
