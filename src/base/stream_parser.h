#pragma once

#include "msg_type_gen.h"
#include <memory>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream.h>

using namespace google::protobuf::io;

namespace logpb {
class Stream_Parser {
public:
    static Stream_Parser create_from_file(const int file_descriptor,
                                          const Message_Def_Gen* defs);

    int parse();

    int parse_msg(bool* clean_eof);

private:
    Stream_Parser(std::unique_ptr<ZeroCopyInputStream> is,
                  const Message_Def_Gen* msg_defs);

    std::unique_ptr<ZeroCopyInputStream> zcis;
    CodedInputStream cis;

    const Message_Def_Gen* msg_defs;
    // ZeroCopyCodedInputStream
};
}  // namespace logpb
