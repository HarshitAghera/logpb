#include "add_plot_window.h"
#include "ui_add_plot_window.h"

Add_Plot_Window::Add_Plot_Window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Plot_Window)
{
    ui->setupUi(this);
}

Add_Plot_Window::~Add_Plot_Window()
{
    delete ui;
}
