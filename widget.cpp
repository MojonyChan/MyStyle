#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QStyleFactory>
#include <QStyleHints>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    menu(new QMenu(this))
{
    ui->setupUi(this);
    this->setMinimumSize(500, 300);

    QProgressBar *pBar = new QProgressBar(this);
    QSlider *slider = new QSlider(this);
    QLabel *textLabel = new QLabel(this);
    DelayButton *button = new DelayButton(this);
//    QComboBox *comboBox = new QComboBox(this);
    QLineEdit *lineEdit = new QLineEdit(this);

    lineEdit->setFixedSize(400, 40);
    lineEdit->setClearButtonEnabled(true);
    pBar->setMinimumSize(400, 40);
    pBar->setValue(70);
    pBar->setRange(0, 100);
    pBar->setOrientation(Qt::Horizontal);

    slider->setMinimumSize(200, 30);
    slider->setRange(0,100);
    slider->setValue(70);
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    textLabel->setText("滑动滑条测试效果");
    button->setText("我是PushButton");
    button->setToolTip("我是PushButton");
    button->setIcon(style()->standardIcon(QStyle::SP_DesktopIcon));
//    comboBox->addItem("测试1");
//    comboBox->addItem("测试2");

    lineEdit->move(50, 190);
    button->move(80, 150);
    pBar->move(50, 50);
    slider->move(250, 150);
    textLabel->move(250, 120);
//    comboBox->move(250, 190);

    QPixmap icon(16, 16);
    icon.fill(Qt::red);
    menu->addAction(style()->standardIcon(QStyle::SP_TrashIcon) ,"测试1");
    menu->addAction("测试2");
    menu->addAction("测试3");
    menu->addSeparator();
    menu->addAction("测试12345");

    connect(slider, &QSlider::valueChanged, pBar, &QProgressBar::setValue);
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton) {
        rightButtonPressed = true;
    }

    return QWidget::mousePressEvent(e);
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    if (rightButtonPressed) {
        menu->exec(mapToParent(e->pos()));
        rightButtonPressed = false;
    }
}

void Widget::paintEvent(QPaintEvent *e)
{
//    QPainter p(this);

//    QRegion eo(50, 50, 3, 3);
//    QRegion ho(50, 50, 3, 3, QRegion::Ellipse);

//    p.setClipRegion(eo.subtracted(ho));
//    p.setBrush(Qt::darkCyan);
//    p.drawRect(0, 0, this->width(), this->height());
}

Widget::~Widget()
{
    delete ui;
}
