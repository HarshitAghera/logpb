#include "basic_plot.h"
#include "qnamespace.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "ui_basic_plot.h"

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

Basic_Plot::Basic_Plot(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::Basic_Plot)
{
    ui->setupUi(this);
    configure_plot(ui->plot);
}

void Basic_Plot::configure_plot(QwtPlot* plot) {

    plot->setTitle( "Temperature of Friedberg/Germany" );

    plot->setAxisTitle( QwtAxis::XBottom, "2007" );
    // plot->setAxisScaleDiv( QwtAxis::XBottom, yearScaleDiv() );
    // plot->setAxisScaleDraw( QwtAxis::XBottom, new YearScaleDraw() );

    plot->setAxisTitle( QwtAxis::YLeft,
        QString( "Temperature [%1C]" ).arg( QChar( 0x00B0 ) ) );

    QwtPlotCanvas* canvas = new QwtPlotCanvas();
    canvas->setPalette( Qt::darkGray );
    canvas->setBorderRadius( 10 );

    plot->setCanvas( canvas );

    // grid
    QwtPlotGrid* grid = new QwtPlotGrid{};
    grid->attach( plot );

    plot->insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    const int numDays = 365;

    QVector< QPointF > averageData( numDays );
    QVector< QwtIntervalSample > rangeData( numDays );

    auto curve = new QwtPlotCurve{};
    curve->setPen(QColor{Qt::blue}, 2, Qt::PenStyle::SolidLine);

    auto symbol = new QwtSymbol{QwtSymbol::Style::Hexagon, QColor{Qt::black}, QColor{Qt::red}, QSize{20, 20}};
    curve->setSymbol(symbol);

    curve->setStyle(QwtPlotCurve::CurveStyle::Lines);

    curve->setRawSamples(x_data.data(), y_data.data(), x_data.size());

    curve->attach(plot);


    QwtPlotZoomer* zoomer = new QwtPlotZoomer( canvas );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,
        Qt::RightButton );


    QwtPlotPanner* panner = new QwtPlotPanner( canvas );
    panner->setMouseButton( Qt::MiddleButton );
}

Basic_Plot::~Basic_Plot()
{
    delete ui;
}
