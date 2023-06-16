#include "mainwindow.h"
#include "qgraphicsscene.h"
#include "qwidget.h"
#include "ui_mainwindow.h"
#include "load_msg_defs.h"
#include "connect_window.h"
#include "add_logger_window.h"
#include "basic_plot.h"
#include <stream_parser.h>
#include <device_connection.h>
#include "add_plot_window.h"

#include <QGraphicsItem>
#include <QGraphicsProxyWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->cs_graphics_view->setRenderHint(QPainter::Antialiasing);
    ui->cs_graphics_view->setScene(new QGraphicsScene{this});

    make_connections();
}

void MainWindow::make_connections() {
    // clang-format off
    connect(ui->actionLoad_definitions, &QAction::triggered,
            this, &MainWindow::load_msg_defs);
    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::create_connection);
    connect(ui->actionAdd_Logger, &QAction::triggered,
            this, &MainWindow::add_logger);
    connect(ui->actionAdd_Plot, &QAction::triggered,
            this, &MainWindow::add_plot);
    // clang-format on

    // auto plot = new Basic_Plot{this};
    // plot->show();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::add_plot_to_cs(QWidget* plot) {
    QPoint initPos{10, 10};

    // Create the graphics item that will be used to move the widget around the
    // screen as well as be selectable (for example in case we want to delete a
    // widget that is in the scene) Depending on the position of the graphics
    // item relative to its widget proxy you can adjust the size and location of
    // both
    QGraphicsRectItem* proxyControl = ui->cs_graphics_view->scene()->addRect(
        initPos.x(), initPos.y(), plot->width(), 20, QPen(Qt::black),
        QBrush(Qt::darkGreen));

    // widget->width() works properly here because
    // of the resize(layout->sizeHint()) that we
    // have used inside it
    proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
    proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);

    // Create the proxy by adding the widget to the scene
    QGraphicsProxyWidget* const proxy =
        ui->cs_graphics_view->scene()->addWidget(plot);

    // In my case the rectangular graphics item is supposed to be above my
    // widget so the position of the widget is shifted along the Y axis based on
    // the height of the rectangle of that graphics item
    proxy->setPos(initPos.x(), initPos.y() + proxyControl->rect().height());
    proxy->setParentItem(proxyControl);
}
void MainWindow::load_msg_defs() {
    // open a sub-window
    Load_Msg_Defs_Window connect_window{this, &session};
    connect_window.exec();
}

void MainWindow::create_connection() {
    session.set_connection(Connect_Window::create_connection(this));

    logpb::Stream_Parser parser{session.get_connection().get_stream(),
                                &session.get_msg_defs()};
    parser.parse();

    update_and_redraw_basic_plots();
}

void MainWindow::update_and_redraw_basic_plots() {
    for (auto& plot : basic_plots) {
        plot->update_and_redraw();
    }
}

void MainWindow::add_logger() {
    int error = Add_Logger_Window::creat_and_add_logger(this, &session);
}

void MainWindow::add_plot() {
    auto plot = Add_Plot_Window::create_and_add_plot(this, &session);

    add_plot_to_cs(plot->get_plot_widget());

    basic_plots.push_back(std::move(plot));

    update();
}
