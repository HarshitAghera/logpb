#include "basic_plot.h"
#include "qnamespace.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "ui_basic_plot.h"

#include <QDockWidget>

#include <QwtPlotZoomer>
#include <QwtPlotPanner>
#include <QwtPlotGrid>
#include <QwtPlotCurve>
#include <QwtPlotCanvas>
#include <QwtPlotIntervalCurve>
#include <QwtLegend>
#include <QwtIntervalSymbol>
#include <QwtSymbol>
#include <QwtText>
#include <QwtScaleDraw>
#include <QwtPlotRenderer>

#include <base/plotter.h>

namespace Ui {
class Basic_Plot;
}

namespace {
void set_interaction_policy(QwtPlotCanvas* canvas) {
    QwtPlotZoomer* zoomer = new QwtPlotZoomer(canvas);
    zoomer->setRubberBandPen(QColor(Qt::black));
    zoomer->setTrackerPen(QColor(Qt::black));
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton,
                            Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);

    QwtPlotPanner* panner = new QwtPlotPanner(canvas);
    panner->setMouseButton(Qt::MiddleButton);
}
}  // namespace

class Basic_Plot_Impl : public QDockWidget {
    Q_OBJECT

public:
    explicit Basic_Plot_Impl(QWidget* parent, const Plot_Info* info);
    ~Basic_Plot_Impl();

    int update_and_redraw();

private:
    void configure_plot();

    Ui::Basic_Plot* ui;
    QwtPlot* plot;
    Plot_Curve_Factory curve_factory;
    const Plot_Info* plot_info;
    std::vector<Plot_Curve> curves;
    std::vector<float> y_data{10, 20, 30, 40, 50, 70};
    std::vector<float> x_data{1, 2, 3, 4, 5, 6};
};

Basic_Plot::Basic_Plot(QWidget* parent, const Plot_Info* info)
    : pimpl{std::make_unique<Basic_Plot_Impl>(parent, info)} {}

Basic_Plot::~Basic_Plot() = default;
Basic_Plot::Basic_Plot(Basic_Plot&&) = default;
Basic_Plot& Basic_Plot::operator=(Basic_Plot&&) = default;

int Basic_Plot::update_and_redraw() { return pimpl->update_and_redraw(); }

Plot_Curve::Plot_Curve(const Curve* cd)
    : curve{new QwtPlotCurve{}}, curve_data{cd} {}

int Plot_Curve::update() {
    auto series = curve_data->get_series();
    curve->setRawSamples(series.x_data, series.y_data, series.size);

    return 0;
}

Plot_Curve_Factory::Plot_Curve_Factory(QwtPlot* p) : plot{p} {}

Plot_Curve Plot_Curve_Factory::create(const Curve* c) {
    Plot_Curve plot_curve{c};

    QwtPlotCurve* curve = plot_curve.get_curve();

    curve->attach(plot);
    curve->setPen(QColor{Qt::blue}, 2, Qt::PenStyle::SolidLine);

    auto symbol = new QwtSymbol{QwtSymbol::Style::Hexagon, QColor{Qt::black},
                                QColor{Qt::red}, QSize{20, 20}};
    curve->setSymbol(symbol);

    curve->setStyle(QwtPlotCurve::CurveStyle::Lines);

    plot_curve.update();

    ++curves;

    return plot_curve;
}

Basic_Plot_Impl::Basic_Plot_Impl(QWidget* parent, const Plot_Info* info)
    : QDockWidget(parent), ui(new Ui::Basic_Plot), plot_info{info} {
    ui->setupUi(this);
    configure_plot();
}

void Basic_Plot_Impl::configure_plot() {
    plot = new QwtPlot(ui->dockWidgetContents);

    curve_factory.set_plot(plot);

    plot->setObjectName(QString::fromUtf8("plot"));
    plot->setGeometry(QRect(30, 130, 551, 321));
    plot->setAutoReplot(false);

    std::string title;
    if (plot_info->curves.size() == 1) {
        title = plot_info->curves[0].x_field + " vs " +
                plot_info->curves[0].y_field;

        plot->setAxisTitle(QwtAxis::XBottom, QString::fromStdString(
                                                 plot_info->curves[0].x_field));
        plot->setAxisTitle(QwtAxis::YLeft, QString::fromStdString(
                                               plot_info->curves[0].y_field));
    } else if (plot_info->curves.size() == 2) {
        title = plot_info->curves[0].y_field + " vs " +
                plot_info->curves[0].x_field + "\n" +
                plot_info->curves[1].y_field + " vs " +
                plot_info->curves[1].x_field;

        plot->setAxisTitle(QwtAxis::XBottom, QString::fromStdString(
                                                 plot_info->curves[0].x_field));
        plot->setAxisTitle(QwtAxis::YLeft, QString::fromStdString(
                                               plot_info->curves[0].y_field));
        plot->setAxisTitle(QwtAxis::YRight, QString::fromStdString(
                                                plot_info->curves[1].y_field));
    } else {
        return;
    }
    plot->setTitle(QString::fromStdString(title));

    // canvas
    QwtPlotCanvas* canvas = new QwtPlotCanvas();
    canvas->setPalette(Qt::darkGray);
    canvas->setBorderRadius(10);
    plot->setCanvas(canvas);
    set_interaction_policy(canvas);

    // grid
    QwtPlotGrid* grid = new QwtPlotGrid{};
    grid->attach(plot);

    for (auto& curve_info : plot_info->curves) {
        curves.push_back(curve_factory.create(curve_info.curve));
    }

    // legend
    plot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

int Basic_Plot_Impl::update_and_redraw() {
    for (auto& curve : curves) {
        curve.update();
    }

    plot->replot();

    return 0;
}

Basic_Plot_Impl::~Basic_Plot_Impl() { delete ui; }

#include "basic_plot.moc"
