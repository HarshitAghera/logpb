#include "stream_generator.h"

#include "msg_defs/msg_defs.pb.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/descriptor.pb.h>
// #include "fmt/core.h"

namespace logpb {
Stream_Generator::Stream_Generator(std::unique_ptr<ZeroCopyOutputStream> is)
    : zcos{std::move(is)}, cos{zcos.get()} {}

Stream_Generator Stream_Generator::create_from_file(int fd) {
    std::unique_ptr<FileOutputStream> fos{new FileOutputStream{fd}};
    fos->SetCloseOnDelete(true);
    return Stream_Generator{std::move(fos)};
}

int Stream_Generator::serialize(const Message& message) {
    size_t size = message.ByteSizeLong();
    if (size > INT_MAX) return false;

    uint32_t msg_id{
        message.GetDescriptor()->options().GetExtension(nanopb_msgopt).msgid()};

    cos.WriteVarint32(static_cast<uint32_t>(size));
    cos.WriteVarint32(msg_id);

    // Write the content.
    uint8_t* buffer =
        cos.GetDirectBufferForNBytesAndAdvance(static_cast<int>(size));
    if (buffer != nullptr) {
        // Optimization: The message fits in one buffer, so use the faster
        // direct-to-array serialization path.
        message.SerializeWithCachedSizesToArray(buffer);
    } else {
        // Slightly-slower path when the message is multiple buffers.
        message.SerializeWithCachedSizes(&cos);
        if (cos.HadError()) return false;
    }

    return true;
}

void Stream_Generator::gen_msgs() {
    logpb::Data data;

    for (int i{}; i < 100; ++i) {
        logpb::Pressure* pres = data.add_pressure();
        logpb::Force* force = data.add_force();

        pres->set_pressure(i);
        pres->set_temperature(i);

        force->set_force(i);
        force->add_temperature(i);
        force->add_temperature(2 * i);
    }

    serialize(data);

    for (int i{}; i < 100; ++i) {
        logpb::Pressure pres;
        pres.set_pressure(10 * i);
        pres.set_temperature(5 * i);

        serialize(pres);

        logpb::Force force;
        force.set_force(20 * i);
        force.add_temperature(10 * i);
        force.add_temperature(20 * i - 5);
        force.add_temperature(i);

        serialize(force);
    }
}
}  // namespace logpb
