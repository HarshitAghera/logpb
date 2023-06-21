#include <gtest/gtest.h>

#include <session.h>
#include <session_loader.h>

TEST(Session_Load_Save, template_functionality) {
    std::string file_path{"session.toml"};

    logpb::Session session;
    logpb::Session_Serializer session_serializer;

    // session.add_msg_def("hello");
    // session.add_msg_def("hi");

    // session_serializer.serialize(file_path, session);
    session_serializer.deserialize(file_path, session);
}
