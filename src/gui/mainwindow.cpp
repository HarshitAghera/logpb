#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::load_msg_defs() {
    // open a sub-window
    Connect_Window connect_window{this};
    connect_window.exec();
}
