#ifndef ADD_PLOT_WINDOW_H
#define ADD_PLOT_WINDOW_H

#include <memory>
#include <QDialog>
#include "qwidget.h"

namespace Ui {
class Add_Plot_Window;
}

namespace logpb {
class Session;
}

class Basic_Plot;
class Msg_Field_Selection;

class Add_Plot_Window : public QDialog {
    Q_OBJECT

public:
    explicit Add_Plot_Window(QWidget* parent, logpb::Session* session);
    ~Add_Plot_Window();

    // static int create_and_add_plot(QWidget* parent, logpb::Session* session);
    static std::unique_ptr<Basic_Plot> create_and_add_plot(
        QWidget* parent, logpb::Session* session);

    void make_connections();

private slots:
    void add_plot_to_session();

private:
    Ui::Add_Plot_Window* ui;
    QWidget* parent;
    logpb::Session* session;
    std::unique_ptr<Basic_Plot> created_plot;

    std::vector<Msg_Field_Selection*> fields;
};

#endif  // ADD_PLOT_WINDOW_H
