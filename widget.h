#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
