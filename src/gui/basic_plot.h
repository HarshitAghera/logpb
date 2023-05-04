#ifndef BASIC_PLOT_H
#define BASIC_PLOT_H

#include <memory>

class QWidget;

class QwtPlot;
class QwtPlotCurve;

class Curve;
struct Plot_Info;

class Plot_Curve {
public:
    Plot_Curve(const Curve* cd);
    int update();
    QwtPlotCurve* get_curve() { return curve; };

private:
    QwtPlotCurve* curve;
    const Curve* curve_data;
};

class Plot_Curve_Factory {
public:
    Plot_Curve_Factory(QwtPlot* p);
    Plot_Curve_Factory() = default;
    Plot_Curve create(const Curve* curve);
    void set_plot(QwtPlot* p) { plot = p; };

private:
    QwtPlot* plot;
    int curves;
};

class Basic_Plot_Impl;

class Basic_Plot {
public:
    explicit Basic_Plot(QWidget* parent, const Plot_Info* info);
    ~Basic_Plot();
    Basic_Plot(Basic_Plot&&);
    Basic_Plot& operator=(Basic_Plot&&);

    int update_and_redraw();
    QWidget* get_plot_widget();

private:
    std::unique_ptr<Basic_Plot_Impl> pimpl;
    // Basic_Plot_Impl* pimpl;
};

#endif  // BASIC_PLOT_H
