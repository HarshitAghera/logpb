#include "connect_window.h"
#include "ui_connect_window.h"
#include <QFileDialog>
#include "app/session.h"

Connect_Window::Connect_Window(QWidget* parent, logpb::Session* s)
    : QDialog(parent), ui(new Ui::Connect_Window), session{s} {
    ui->setupUi(this);

    make_connections();
}

void Connect_Window::make_connections() {
    connect(ui->pb_browse_defs, &QAbstractButton::clicked, this,
            &Connect_Window::load_msg_defs);
}

Connect_Window::~Connect_Window() { delete ui; }

void Connect_Window::load_msg_defs() {
    QStringList msg_defs =
        QFileDialog::getOpenFileNames(this, "Select Message Definitions", "",
                                      "Proto files (*.proto);;All files (*)");

    ui->id_defs->setPlainText(msg_defs.join("\n"));

    for (auto& def : msg_defs) {
        session->add_msg_def(def.toStdString());
    }

    auto& error = session->get_msg_defs().get_errors();

    QString message{"Errors :\n"};

    for (auto& e : error.get_errors()) {
        message.append(e.c_str());
        message.append('\n');
    }

    message.append("\n\nWarnings:\n");

    for (auto& w : error.get_warnings()) {
        message.append(w.c_str());
        message.append('\n');
    }

    ui->id_parse_errors->setPlainText(message);
}
