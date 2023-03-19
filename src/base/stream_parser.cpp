#include "stream_parser.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <msg_defs/msg_defs.pb.h>

#include <fmt/core.h>

namespace logpb {
Stream_Parser::Stream_Parser(std::unique_ptr<ZeroCopyInputStream> is,
                             const Message_Def_Gen* defs)
    : own_zcis{std::move(is)},
      zcis{own_zcis.get()},
      cis{zcis},
      msg_defs{defs} {}

Stream_Parser::Stream_Parser(ZeroCopyInputStream* is,
                             const Message_Def_Gen* defs)
    : zcis{is}, cis{zcis}, msg_defs{defs} {}

Stream_Parser Stream_Parser::create_from_file(const int file_descriptor,
                                              const Message_Def_Gen* defs) {
    std::unique_ptr<FileInputStream> fis{new FileInputStream{file_descriptor}};
    return Stream_Parser{std::move(fis), defs};
}

int Stream_Parser::parse() {
    bool clean_eof{false};

    bool parse_result{true};

    while (parse_result) {
        parse_result = parse_msg(&clean_eof);
    }

    return 0;
}

int Stream_Parser::parse_msg(bool* clean_eof) {
    if (clean_eof != nullptr) *clean_eof = false;
    int start = cis.CurrentPosition();

    // Read the size.
    uint32_t size;
    if (!cis.ReadVarint32(&size)) {
        if (clean_eof != nullptr) *clean_eof = cis.CurrentPosition() == start;
        return false;
    }

    start = cis.CurrentPosition();

    // Read the msgid.
    uint32_t msgid;
    if (!cis.ReadVarint32(&msgid)) {
        if (clean_eof != nullptr) *clean_eof = cis.CurrentPosition() == start;
        return false;
    }
    // Get the position after any size bytes have been read (and only the
    // message itself remains).
    int position_after_size = cis.CurrentPosition();

    // Tell the stream not to read beyond that size.
    io::CodedInputStream::Limit limit = cis.PushLimit(static_cast<int>(size));

    std::unique_ptr<Message> msg{msg_defs->parse_message(msgid, cis)};
    // fmt::print("{}\n", msg->DebugString());

    // Parse the message.
    if (!msg) return false;
    if (!cis.ConsumedEntireMessage()) return false;
    if (cis.CurrentPosition() - position_after_size != static_cast<int>(size))
        return false;

    // Release the limit.
    cis.PopLimit(limit);

    return true;
}
}  // namespace logpb
