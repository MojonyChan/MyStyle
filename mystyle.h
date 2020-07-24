#ifndef MYSTYLE_H
#define MYSTYLE_H

#include "delaybutton.h"

#include <QObject>
#include <QCommonStyle>
#include <QStyleOption>
#include <QPainter>
#include <QFontMetricsF>
#include <QPalette>
#include <QLinearGradient>
#include <QPaintEngine>
#include <QPixmapCache>
#include <QApplication>

class MyStyle : public QCommonStyle
{
    Q_OBJECT

public:
    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = nullptr) const override;

    void drawControl(QStyle::ControlElement element, const QStyleOption *opt,
                     QPainter *p, const QWidget *widget = nullptr) const override;

    void drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex *opt,
                            QPainter *p, const QWidget *widget = nullptr) const override;

    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap,
                                const QStyleOption *opt) const override;

    int pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt = nullptr,
                    const QWidget *widget = nullptr) const override;

    int styleHint(QStyle::StyleHint sh, const QStyleOption *opt = nullptr,
                   const QWidget *widget = nullptr, QStyleHintReturn *hret = nullptr) const override;

    QRect subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt,
                         QStyle::SubControl sc, const QWidget *widget = nullptr) const override;

    QRect subElementRect(QStyle::SubElement sr, const QStyleOption *opt,
                         const QWidget *widget = nullptr) const override;

    QSize sizeFromContents(QStyle::ContentsType ct, const QStyleOption *opt,
                           const QSize &csz, const QWidget *widget = nullptr) const override;

    void polish(QWidget *widget) override;

signals:

public slots:
};

#endif // MYSTYLE_H
