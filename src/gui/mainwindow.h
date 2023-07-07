#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <session.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Basic_Plot;
class MainWindow;

class Numeric_Plot_Factory : public Plot_Widget_Factory {
public:
    Numeric_Plot_Factory(MainWindow* mw) : main_window{mw} {};

    void process_plot_info(const Plot_Info& plot_info) override;

private:
    MainWindow* main_window;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void add_plot_to_cs(std::unique_ptr<Basic_Plot> bp);

private slots:
    void load_msg_defs();
    void create_connection();
    void add_logger();
    void add_plot();
    void save_session();
    void load_session();

private:
    void make_connections();
    void update_and_redraw_basic_plots();

private:
    Ui::MainWindow *ui;
    logpb::Session session;

    Numeric_Plot_Factory plot_factory;
    std::vector<std::unique_ptr<Basic_Plot>> basic_plots;
};

#endif  // MAINWINDOW_H
