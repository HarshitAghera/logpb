#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionOpen_triggered() {}

void MainWindow::on_actionConnect_triggered() {
    // open a sub-window
    Connect_Window connect_window{this};
    connect_window.exec();
}
