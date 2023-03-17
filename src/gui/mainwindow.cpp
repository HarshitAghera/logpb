#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "load_msg_defs.h"
#include "connect_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    make_connections();
}

void MainWindow::make_connections() {
    connect(ui->actionLoad_definitions, &QAction::triggered, this,
            &MainWindow::load_msg_defs);
    connect(ui->actionConnect, &QAction::triggered, this,
            &MainWindow::create_connection);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::load_msg_defs() {
    // open a sub-window
    Load_Msg_Defs_Window connect_window{this, &session};
    connect_window.exec();
}

void MainWindow::create_connection() {
    session.set_connection(Connect_Window::create_connection(this));
}
