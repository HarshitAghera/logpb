#include "connect_window.h"
#include "ui_connect_window.h"

Connect_Window::Connect_Window(QWidget *parent) :
                                                  QDialog(parent),
                                                  ui(new Ui::Connect_Window)
{
    ui->setupUi(this);
}

Connect_Window::~Connect_Window()
{
    delete ui;
}
