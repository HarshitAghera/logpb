#ifndef LOAD_MSG_DEFS_H
#define LOAD_MSG_DEFS_H

#include <QDialog>

namespace logpb {
class Session;
}

namespace Ui {
class Load_Msg_Defs_Window;
}

class Load_Msg_Defs_Window : public QDialog {
    Q_OBJECT

public:
    explicit Load_Msg_Defs_Window(QWidget *parent, logpb::Session *s);
    ~Load_Msg_Defs_Window();

private slots:
    void create_new_msg_defs();
    void add_msg_defs();

private:
    void make_connections();

private:
    Ui::Load_Msg_Defs_Window *ui;

    logpb::Session *session;
};

#endif  // LOAD_MSG_DEFS_H
