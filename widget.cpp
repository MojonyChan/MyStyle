#include "widget.h"
#include "ui_widget.h"
#include "mystyle.h"

#include <QDebug>
#include <QStyleFactory>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMinimumSize(500, 300);

    QProgressBar *pBar = new QProgressBar(this);
    QSlider *slider = new QSlider(this);
    QLabel *textLabel = new QLabel(this);

    pBar->setMinimumSize(40, 400);
    pBar->setValue(70);
    pBar->setRange(0, 100);
    pBar->setOrientation(Qt::Vertical);
    pBar->setStyle(new MyStyle);
    slider->setRange(0,100);
    slider->setOrientation(Qt::Horizontal);
    textLabel->setText("滑动滑条测试效果");

    pBar->move(50, 50);
    slider->move(250, 150);
    textLabel->move(250, 120);

    connect(slider, &QSlider::valueChanged, pBar, &QProgressBar::setValue);
}

Widget::~Widget()
{
    delete ui;
}
