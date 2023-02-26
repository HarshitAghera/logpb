#include "connect_window.h"
#include "ui_connect_window.h"
#include <QFileDialog>

Connect_Window::Connect_Window(QWidget *parent)
    : QDialog(parent), ui(new Ui::Connect_Window) {
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
}
