#pragma once

#include "device_connection.h"
#include <string>
#include <string_view>
#include <memory>

namespace google {
namespace protobuf {
namespace io {
class FileInputStream;
}
}  // namespace protobuf
}  // namespace google

using namespace google::protobuf::io;

namespace logpb {
class File_Connection : public Device_Connection {
public:
    ~File_Connection() override;
    File_Connection(File_Connection&&);
    File_Connection& operator=(File_Connection&&);

    static File_Connection create_from_path(const std::string_view file_name);

    const char* name() override { return "File Connection"; };
    ZeroCopyInputStream* get_stream() override;

private:
    File_Connection(const int fd);

private:
    std::unique_ptr<FileInputStream> file_stream;
};
}  // namespace logpb
