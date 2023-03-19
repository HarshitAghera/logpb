#ifndef ADD_LOGGER_WINDOW_H
#define ADD_LOGGER_WINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class Add_Logger_Window;
}

namespace logpb {
class Session;
}

class Add_Logger_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Logger_Window(QWidget* parent, logpb::Session* session);
    ~Add_Logger_Window();

    int static creat_and_add_logger(QWidget* parent, logpb::Session* session);

    void make_connections();

private slots:
    void set_file_path();
    void add_logger_to_session();
    void refresh_message_list(int index);

private:
    Ui::Add_Logger_Window *ui;
    logpb::Session* session;

    QString file_path;

    std::vector<std::string> pkgs;
    std::vector<std::string> msgs;
};

#endif // ADD_LOGGER_WINDOW_H
