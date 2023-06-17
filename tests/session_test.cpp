#include <gtest/gtest.h>

#include <session.h>
#include <session_loader.h>

TEST(Session_Load_Save, template_functionality) {
    std::string file_path{"msg_defs/session.toml"};

    logpb::Session session;
    logpb::Session_Serializer session_serializer;

    session.add_msg_def("hello");
    session.add_msg_def("hi");

    session_serializer.serialize(session);
}
