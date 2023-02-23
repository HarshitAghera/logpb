#ifndef CONNECT_WINDOW_H
#define CONNECT_WINDOW_H

#include <QDialog>

namespace Ui {
class Connect_Window;
}

class Connect_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Connect_Window(QWidget *parent = nullptr);
    ~Connect_Window();

private:
    Ui::Connect_Window *ui;
};

#endif // CONNECT_WINDOW_H
