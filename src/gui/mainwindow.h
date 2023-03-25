#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <app/session.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void load_msg_defs();
    void create_connection();
    void add_logger();
    void add_plot();

private:
    void make_connections();

private:
    Ui::MainWindow *ui;
    logpb::Session session;
};
#endif // MAINWINDOW_H
