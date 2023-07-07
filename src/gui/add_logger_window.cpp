#include "add_logger_window.h"
#include "ui_add_logger_window.h"

#include <QFileDialog>
#include <session.h>
#include <msg_type_gen.h>
#include <logger.h>

Add_Logger_Window::Add_Logger_Window(QWidget* parent, logpb::Session* s)
    : QDialog(parent), ui(new Ui::Add_Logger_Window), session(s) {
    ui->setupUi(this);
    pkgs = session->get_msg_defs().get_package_list();

    for (const auto& pkg : pkgs) {
        ui->cb_package->addItem(QString::fromStdString(pkg));
    }

    refresh_message_list(0);

    make_connections();
}

Add_Logger_Window::~Add_Logger_Window() { delete ui; }

void Add_Logger_Window::make_connections() {
    // clang-format off
    connect(ui->bb_decision, &QDialogButtonBox::accepted,
            this, &Add_Logger_Window::add_logger_to_session);
    connect(ui->bb_decision, &QDialogButtonBox::rejected,
            this, &Add_Logger_Window::reject);
    connect(ui->pb_browse, &QPushButton::clicked,
            this, &Add_Logger_Window::set_file_path);
    connect(ui->cb_package, &QComboBox::currentIndexChanged,
            this, &Add_Logger_Window::set_file_path);
    // clang-format on
}

void Add_Logger_Window::set_file_path() {
    file_path = QFileDialog::getSaveFileName(
        this, "Save file as", "", "CSV files (*.csv);;All files (*.*)");

    ui->id_defs->setPlainText(file_path);
}

void Add_Logger_Window::add_logger_to_session() {
    const auto fp = file_path.toUtf8();
    const std::string_view path(fp.data(), fp.size());

    auto msg_name = pkgs[ui->cb_package->currentIndex()] + "." +
                    msgs[ui->cb_message->currentIndex()];

    session->add_csv_logger(msg_name, path, ",", "");
    accept();
}

void Add_Logger_Window::refresh_message_list(int index) {
    msgs = session->get_msg_defs().get_message_list({pkgs[index]});

    ui->cb_message->clear();

    for (const auto& msg : msgs) {
        ui->cb_message->addItem(QString::fromStdString(msg));
    }
}

int Add_Logger_Window::creat_and_add_logger(QWidget* parent,
                                            logpb::Session* session) {
    Add_Logger_Window logger_window{parent, session};
    logger_window.exec();

    return 0;
}
