#pragma once

namespace google {
namespace protobuf {
namespace io {
class ZeroCopyInputStream;
}
}  // namespace protobuf
}  // namespace google

using namespace google::protobuf::io;

namespace logpb {
class Device_Connection {
public:
    Device_Connection();
    virtual ~Device_Connection();
    virtual const char* name() = 0;
    virtual ZeroCopyInputStream* get_stream() = 0;
};
}  // namespace logpb
