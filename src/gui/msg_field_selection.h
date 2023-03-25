#ifndef MSG_FIELD_SELECTION_H
#define MSG_FIELD_SELECTION_H

#include <QFrame>
#include <string>

namespace Ui {
class Msg_Field_Selection;
}

namespace logpb {
class Message_Def_Gen;
}

class Msg_Field_Selection : public QFrame {
    Q_OBJECT

public:
    explicit Msg_Field_Selection(QWidget* parent,
                                 const logpb::Message_Def_Gen* msg_defs);
    ~Msg_Field_Selection();

    std::string get_field();

private:
    void make_connections();

    void refresh_message_list(int index);
    void refresh_field_list(int index);

private:
    Ui::Msg_Field_Selection *ui;
    const logpb::Message_Def_Gen* msg_defs;

    std::vector<std::string> pkgs;
    std::vector<std::string> msgs;
    std::vector<std::string> fields;
};

#endif // MSG_FIELD_SELECTION_H
