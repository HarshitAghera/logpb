#ifndef ADD_PLOT_WINDOW_H
#define ADD_PLOT_WINDOW_H

#include <QDialog>

namespace Ui {
class Add_Plot_Window;
}

class Add_Plot_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Plot_Window(QWidget *parent = nullptr);
    ~Add_Plot_Window();

private:
    Ui::Add_Plot_Window *ui;
};

#endif // ADD_PLOT_WINDOW_H
