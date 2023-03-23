#ifndef BASIC_PLOT_H
#define BASIC_PLOT_H

#include <QDockWidget>

namespace Ui {
class Basic_Plot;
}

class QwtPlot;

class Basic_Plot : public QDockWidget
{
    Q_OBJECT

public:
    void configure_plot(QwtPlot* plot);
    explicit Basic_Plot(QWidget *parent = nullptr);
    ~Basic_Plot();


private:
    Ui::Basic_Plot *ui;
    std::vector<float> y_data{10, 20, 30, 40, 50, 70};
    std::vector<float> x_data{1, 2, 3, 4, 5, 6};
};

#endif // BASIC_PLOT_H
