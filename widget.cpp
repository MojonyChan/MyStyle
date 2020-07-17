#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QStyleFactory>
#include <QStyleHints>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMinimumSize(500, 300);


    QProgressBar *pBar = new QProgressBar(this);
    QSlider *slider = new QSlider(this);
    QLabel *textLabel = new QLabel(this);

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

    pBar->move(50, 50);
    slider->move(250, 150);
    textLabel->move(250, 120);

    connect(slider, &QSlider::valueChanged, pBar, &QProgressBar::setValue);
}

Widget::~Widget()
{
    delete ui;
}
