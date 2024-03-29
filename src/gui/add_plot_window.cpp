#include "add_plot_window.h"
#include "ui_add_plot_window.h"
#include "msg_field_selection.h"
#include <session.h>
#include <plotter.h>

Add_Plot_Window::Add_Plot_Window(QWidget* parent, logpb::Session* session,
                                 Plot_Widget_Factory* pwf)
    : QDialog(parent),
      ui(new Ui::Add_Plot_Window),
      parent{parent},
      session{session},
      pwf{pwf} {
    ui->setupUi(this);

    auto* fr_x =
        new Msg_Field_Selection(ui->gb_x_axis, &session->get_msg_defs());
    fr_x->setObjectName(QString::fromUtf8("fr_x"));
    fr_x->setGeometry(QRect(10, 90, 705, 115));
    // fr_x->setFrameShape(QFrame::StyledPanel);
    // fr_x->setFrameShadow(QFrame::Raised);

    auto* fr_y =
        new Msg_Field_Selection(ui->gb_y_axis, &session->get_msg_defs());
    fr_y->setObjectName(QString::fromUtf8("fr_y"));
    fr_y->setGeometry(QRect(10, 30, 705, 115));
    // fr_y->setFrameShape(QFrame::StyledPanel);
    // fr_y->setFrameShadow(QFrame::Raised);

    fields.push_back(fr_x);
    fields.push_back(fr_y);

    make_connections();
}

Add_Plot_Window::~Add_Plot_Window() { delete ui; }

void Add_Plot_Window::create_and_add_plot(QWidget* parent,
                                          logpb::Session* session,
                                          Plot_Widget_Factory* pwf) {
    Add_Plot_Window plot_window(parent, session, pwf);
    plot_window.exec();
}

void Add_Plot_Window::add_plot_to_session() {
    Plot_Curve_Fields plot_info{
        .curves{{fields[0]->get_field(), fields[1]->get_field()}}};

    session->add_numeric_plotter(plot_info, pwf);

    accept();
}

void Add_Plot_Window::make_connections() {
    // clang-format off
    connect(ui->bb_decision, &QDialogButtonBox::accepted,
            this, &Add_Plot_Window::add_plot_to_session);
    connect(ui->bb_decision, &QDialogButtonBox::rejected,
            this, &Add_Plot_Window::reject);
    // clang-format on
}
