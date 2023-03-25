#include "msg_field_selection.h"
#include "ui_msg_field_selection.h"
#include <base/msg_type_gen.h>

Msg_Field_Selection::Msg_Field_Selection(QWidget* parent,
                                         const logpb::Message_Def_Gen* msg_defs)
    : QFrame(parent), ui(new Ui::Msg_Field_Selection), msg_defs{msg_defs} {
    ui->setupUi(this);

    pkgs = msg_defs->get_package_list();

    for (const auto& pkg : pkgs) {
        ui->cb_package->addItem(QString::fromStdString(pkg));
    }

    refresh_message_list(0);

    make_connections();
}

Msg_Field_Selection::~Msg_Field_Selection() { delete ui; }

void Msg_Field_Selection::make_connections() {
    // clang-format off
    connect(ui->cb_package, &QComboBox::currentIndexChanged,
            this, &Msg_Field_Selection::refresh_message_list);
    connect(ui->cb_message, &QComboBox::currentIndexChanged,
            this, &Msg_Field_Selection::refresh_field_list);
    // clang-format on
}

void Msg_Field_Selection::refresh_message_list(int index) {
    msgs = msg_defs->get_message_list({pkgs[index]});

    ui->cb_message->clear();

    for (const auto& msg : msgs) {
        ui->cb_message->addItem(QString::fromStdString(msg));
    }

    refresh_field_list(0);
}

void Msg_Field_Selection::refresh_field_list(int index) {
    fields = msg_defs->get_field_list(pkgs[ui->cb_package->currentIndex()],
                                      {msgs[ui->cb_message->currentIndex()]});

    ui->cb_field->clear();

    for (const auto& field : fields) {
        ui->cb_field->addItem(QString::fromStdString(field));
    }
}

std::string Msg_Field_Selection::get_field() {
    return pkgs[ui->cb_package->currentIndex()] + "." +
           msgs[ui->cb_message->currentIndex()] + "." +
           fields[ui->cb_field->currentIndex()];
}
