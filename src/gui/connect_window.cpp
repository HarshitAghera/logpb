#include "connect_window.h"
#include "ui_connect_window.h"
#include <QFileDialog>
#include <base/device_connection_impl.h>

#include <memory>

Connect_Window::Connect_Window(QWidget* parent)
    : QDialog(parent), ui(new Ui::Connect_Window) {
    ui->setupUi(this);

    make_qt_ss_connections();
}

Connect_Window::~Connect_Window() { delete ui; }

void Connect_Window::make_qt_ss_connections() {
    // clang-format off
    connect(ui->bb_decision, &QDialogButtonBox::accepted,
            this, &Connect_Window::accept);
    connect(ui->bb_decision, &QDialogButtonBox::rejected,
            this, &Connect_Window::reject);
    connect(ui->pb_browse_defs, &QAbstractButton::clicked,
            this, &Connect_Window::create_file_connection);
    // clang-format on
}

void Connect_Window::create_file_connection() {
    QString file_name = QFileDialog::getOpenFileName(
        this, "Select Message Definitions", "", "All files (*)");

    ui->id_defs->setPlainText(file_name);

    connection = std::make_unique<logpb::File_Connection>(
        logpb::File_Connection::create_from_path(file_name.toStdString()));
}

std::unique_ptr<logpb::Device_Connection> Connect_Window::create_connection(
    QWidget* parent) {
    Connect_Window con_win{parent};
    con_win.exec();

    return std::move(con_win.connection);
}
