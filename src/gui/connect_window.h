#ifndef CONNECT_WINDOW_H
#define CONNECT_WINDOW_H

#include <QDialog>

namespace Ui {
class Connect_Window;
}

namespace logpb {
class Device_Connection;
}

class Connect_Window : public QDialog
{
    Q_OBJECT

private:
    explicit Connect_Window(QWidget *parent = nullptr);
    ~Connect_Window();

public:
    static std::unique_ptr<logpb::Device_Connection> create_connection(QWidget* parent);

    // private slots:
private:
    void create_file_connection();

private:
    void make_qt_ss_connections();

private:
    Ui::Connect_Window *ui;
    std::unique_ptr<logpb::Device_Connection> connection;
};

#endif // CONNECT_WINDOW_H
