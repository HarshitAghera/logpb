#include "parse_error.h"
#include <fmt/core.h>

namespace logpb {
void Parser_Error_Collector::AddError(const std::string& filename, int line,
                                      int column, const std::string& message) {
    std::string er = fmt::format("line {}:{} : {}", line, column, message);
    std::string msg = fmt::format("Parse Error for file {} : {}", filename, er);

    fmt::print("{}\n", msg);

    fec->add_error(std::move(er));
    errors.push_back(std::move(msg));
}

void Parser_Error_Collector::AddWarning(const std::string& filename, int line,
                                        int column,
                                        const std::string& message) {
    std::string wn = fmt::format("line {}:{} : {}", line, column, message);
    std::string msg =
        fmt::format("Parse Warning for file {} : {}", filename, wn);

    fmt::print("{}\n", msg);

    fec->add_warning(std::move(wn));
    warnings.push_back(std::move(msg));
}
}  // namespace logpb
