#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "load_msg_defs.h"
#include "connect_window.h"
#include "add_logger_window.h"
#include <base/stream_parser.h>
#include "basic_plot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    make_connections();
}

void MainWindow::make_connections() {
    // clang-format off
    connect(ui->actionLoad_definitions, &QAction::triggered,
            this, &MainWindow::load_msg_defs);
    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::create_connection);
    connect(ui->actionAdd_Logger, &QAction::triggered,
            this, &MainWindow::add_logger);
    // clang-format on

    auto plot = new Basic_Plot{this};
    plot->show();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::load_msg_defs() {
    // open a sub-window
    Load_Msg_Defs_Window connect_window{this, &session};
    connect_window.exec();
}

void MainWindow::create_connection() {
    session.set_connection(Connect_Window::create_connection(this));

    logpb::Stream_Parser parser{session.get_connection().get_stream(),
                                &session.get_msg_defs()};
    parser.parse();
}

void MainWindow::add_logger() {
    int error = Add_Logger_Window::creat_and_add_logger(this, &session);
}
