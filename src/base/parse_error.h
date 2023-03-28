#pragma once

#include <vector>
#include <string>

#include <google/protobuf/compiler/importer.h>

using namespace google::protobuf::compiler;

namespace logpb {
class File_Error_Collector {
public:
    void add_error(std::string msg) { errors.emplace_back(std::move(msg)); }

    void add_warning(std::string msg) { warnings.emplace_back(std::move(msg)); }

private:
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

class Parser_Error_Collector final : public MultiFileErrorCollector {
public:
    const std::vector<std::string>& get_errors() const { return errors; }
    const std::vector<std::string>& get_warnings() const { return warnings; }

    void set_file_collector(File_Error_Collector* ec) { fec = ec; }

private:
    void AddError(const std::string& filename, int line, int column,
                  const std::string& message) override;
    void AddWarning(const std::string& filename, int line, int column,
                    const std::string& message) override;

private:
    File_Error_Collector* fec;

    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};
}  // namespace logpb
