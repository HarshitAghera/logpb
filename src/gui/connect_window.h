#ifndef CONNECT_WINDOW_H
#define CONNECT_WINDOW_H

#include <QDialog>

namespace logpb {
class Session;
}

namespace Ui {
class Connect_Window;
}

class Connect_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Connect_Window(QWidget *parent, logpb::Session *s);
    ~Connect_Window();

private slots:
    void load_msg_defs();

private:
    void make_connections();

private:
    Ui::Connect_Window *ui;

    logpb::Session *session;
};

#endif // CONNECT_WINDOW_H
