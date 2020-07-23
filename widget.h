#ifndef WIDGET_H
#define WIDGET_H

#include "mystyle.h"
#include "delaybutton.h"

#include <QWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    Ui::Widget *ui;
    bool rightButtonPressed = false;
    QMenu *menu;
};

#endif // WIDGET_H
