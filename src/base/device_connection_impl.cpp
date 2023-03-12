#include "device_connection_impl.h"

#include <stdio.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace logpb {
File_Connection::File_Connection(const int fd)
    : file_stream{new FileInputStream{fd}} {
    file_stream->SetCloseOnDelete(true);
}

File_Connection::~File_Connection() = default;

File_Connection::File_Connection(File_Connection&&) = default;

File_Connection& File_Connection::operator=(File_Connection&&) = default;

ZeroCopyInputStream* File_Connection::get_stream() {
    return file_stream.get();
};

File_Connection File_Connection::create_from_path(
    const std::string_view file_name) {
    std::FILE* fd = fopen(file_name.data(), "rb");
    return File_Connection{fileno(fd)};
}
}  // namespace logpb
