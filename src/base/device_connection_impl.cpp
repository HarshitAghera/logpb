#include "device_connection_impl.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <fcntl.h>
#ifdef _MSC_VER
#include <google/protobuf/io/io_win32.h>
#else
#define O_BINARY 0
#endif

#ifdef _WIN32
using google::protobuf::io::win32::open;
#endif

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
    return File_Connection{open(file_name.data(), O_RDONLY | O_BINARY)};
}
}  // namespace logpb
