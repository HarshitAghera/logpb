#pragma once

#include <google/protobuf/message.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>

using namespace google::protobuf;
using namespace google::protobuf::io;

namespace logpb {
class Stream_Generator {
public:
    static Stream_Generator create_from_file(int fd);

    void gen_msgs();
    int serialize(const Message& message);

private:
    Stream_Generator(std::unique_ptr<ZeroCopyOutputStream> os);

    std::unique_ptr<ZeroCopyOutputStream> zcos;
    CodedOutputStream cos;
};
}  // namespace logpb
