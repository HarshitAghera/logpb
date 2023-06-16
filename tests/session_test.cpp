#include <gtest/gtest.h>

#include <session.h>
#include <session_loader.h>

TEST(Session_Load_Save, template_functionality) {
    std::string file_path{"resources/session.toml"};

    logpb::Session session;

    save_session(file_path, session);
}
