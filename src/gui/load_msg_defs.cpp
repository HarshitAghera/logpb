#include "load_msg_defs.h"
#include "ui_load_msg_defs.h"
#include <QFileDialog>
#include "app/session.h"

Load_Msg_Defs_Window::Load_Msg_Defs_Window(QWidget* parent, logpb::Session* s)
    : QDialog(parent), ui(new Ui::Load_Msg_Defs_Window), session{s} {
    ui->setupUi(this);

    make_connections();
}

void Load_Msg_Defs_Window::make_connections() {
    connect(ui->pb_browse_defs, &QAbstractButton::clicked, this,
            &Load_Msg_Defs_Window::create_new_msg_defs);
}

Load_Msg_Defs_Window::~Load_Msg_Defs_Window() { delete ui; }

void Load_Msg_Defs_Window::create_new_msg_defs() {
    session->reset_msg_defs();
    add_msg_defs();
}

void Load_Msg_Defs_Window::add_msg_defs() {
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

    session->refresh_msg_defs();
}
