#include "mystyle.h"
#include <QDebug>

void MyStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
//    qDebug() << element << widget->metaObject()->className() << __func__;
    switch (element) {
    case PE_IndicatorProgressChunk: {
        QLinearGradient linear;

        linear.setStart(0, 0);
        linear.setFinalStop(widget->width(), widget->height());
        linear.setColorAt(0, QColor(255,182,193));
        linear.setColorAt(0.5, QColor(100,149,237));
        linear.setColorAt(1, QColor(255,222,173));

        painter->setPen(Qt::NoPen);
        painter->setBrush(linear);

        painter->drawRoundedRect(option->rect, 8, 8);

        return;
    }
    default:
        break;
    }

    return QCommonStyle::drawPrimitive(element, option, painter, widget);
}

void MyStyle::drawControl(QStyle::ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *widget) const
{
//    qDebug() << element << widget->metaObject()->className() << __func__;
    p->setRenderHint(QPainter::Antialiasing);

    switch (element) {
    case CE_ProgressBarContents: {
        const QStyleOptionProgressBar *pb = qstyleoption_cast<const QStyleOptionProgressBar *>(opt);
        const bool vertial = pb->orientation == Qt::Vertical;
        QRect rect = pb->rect;
        int minimum = pb->minimum;
        int maximum = pb->maximum;
        int progress = pb->progress;


        QStyleOptionProgressBar pbBits = *pb;

        if (vertial) {
            pbBits.rect = QRect(rect.x(), rect.height() - int(rect.height() * double(progress) / (maximum-minimum)), rect.width(), int(rect.height() * double(progress) / (maximum-minimum)));
        } else {
            pbBits.rect = QRect(rect.x(), rect.y(), int(rect.width() * double(progress) / (maximum-minimum)), rect.height());
        }

        p->setBrush(QColor("#D3D3D3"));
        p->drawRoundedRect(rect, 8, 8);
        proxy()->drawPrimitive(PE_IndicatorProgressChunk, &pbBits, p, widget);

        return;
    }

    case CE_ProgressBar: {
        // 源码中，利用subElementRect返回的子元素大小当作参数使用这里列出的其他宏重复调用本身，我这里不做实现
        break;
    }

    case CE_ProgressBarGroove: {
        // 从源码分析 这里宽度只有1
        p->setPen(Qt::transparent);
        p->setBrush(Qt::NoBrush);
        p->drawRect(opt->rect);
        break;
    }

    case CE_ProgressBarLabel: {
        const QStyleOptionProgressBar *pBarOpt = qstyleoption_cast<const QStyleOptionProgressBar *>(opt);
        QString text = QString("已完成").append(QString::number(double(pBarOpt->progress) / (pBarOpt->maximum-pBarOpt->minimum) * 100)).append("%");
        QFont font = p->font();
        bool vertical = pBarOpt->orientation == Qt::Vertical;

        font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
        p->setFont(font);
        /* 字体矩形渐变色 */
        double mid = (double(pBarOpt->progress) / (pBarOpt->maximum-pBarOpt->minimum) > 0) ? double(pBarOpt->progress) / (pBarOpt->maximum-pBarOpt->minimum) : 0.001;
        mid = mid >= 1 ? 0.999 : mid;

        if (!vertical) {
            QLinearGradient textGradient(QPointF(pBarOpt->rect.left(), pBarOpt->rect.top()), QPointF(pBarOpt->rect.width(), pBarOpt->rect.top()));

            textGradient.setColorAt(0, Qt::white);
            textGradient.setColorAt(mid, Qt::white);
            textGradient.setColorAt(mid + 0.001, Qt::darkGray);
            textGradient.setColorAt(1, Qt::darkGray);

            p->setPen(QPen(QBrush(textGradient), 1));
            p->drawText(pBarOpt->rect, Qt::AlignCenter | Qt::TextSingleLine, text);
        } else {
            QLinearGradient textGradient(QPointF(pBarOpt->rect.left(), pBarOpt->rect.height()), QPointF(pBarOpt->rect.left(), pBarOpt->rect.top()));

            textGradient.setColorAt(0, Qt::white);

            textGradient.setColorAt(mid, Qt::white);
            textGradient.setColorAt(mid + 0.001, Qt::darkGray);
            textGradient.setColorAt(1, Qt::darkGray);

            p->setPen(QPen(QBrush(textGradient), 1));
            p->drawText(QRectF((pBarOpt->rect.width()-QFontMetricsF(p->font()).width("字"))/2, pBarOpt->rect.top(), QFontMetricsF(p->font()).width("字"), pBarOpt->rect.height()), Qt::AlignCenter | Qt::TextWrapAnywhere, text);
        }

        return;
    }

    default:
        break;
    }
    return QCommonStyle::drawControl(element, opt, p, widget);
}

void MyStyle::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const
{
    p->setRenderHint(QPainter::Antialiasing);
    switch (cc) {
    case CC_Slider: {
        const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(opt);

        int sliderValue = slider->sliderPosition;
        int sliderPositon = sliderPositionFromValue(slider->minimum, slider->maximum, sliderValue, opt->rect.width()-pixelMetric(PM_SliderLength, slider, widget));
        int sliderH = pixelMetric(PM_SliderThickness, opt, widget);
        QRect sliderAreaRect = QRect(QPoint(opt->rect.left(), opt->rect.bottom()/2 - sliderH/2), QSize(sliderPositon, sliderH));
        QRect handleRect = proxy()->subControlRect(CC_Slider, opt, SC_SliderHandle, widget);

        QLinearGradient areaLinear;
        areaLinear.setStart(sliderAreaRect.x(), sliderAreaRect.y());
        areaLinear.setFinalStop(opt->rect.width(), sliderAreaRect.y());
        areaLinear.setColorAt(0, QColor(255,182,193));
        areaLinear.setColorAt(0.5, QColor(100,149,237));
        areaLinear.setColorAt(1, QColor(255,222,173));

        QLinearGradient handleLinear;
        handleLinear.setStart(handleRect.x(), handleRect.y());
        handleLinear.setFinalStop(handleRect.x(), handleRect.y() + handleRect.height());
        handleLinear.setColorAt(0, QColor(0,0,0));
        handleLinear.setColorAt(0.5, QColor(100,100,100));
        handleLinear.setColorAt(1, QColor(50,50,50));

        p->setBrush(Qt::gray);
        p->setPen(Qt::NoPen);
        p->drawRoundedRect(proxy()->subControlRect(CC_Slider, opt, SC_SliderGroove, widget), 4, 4);

        p->setBrush(areaLinear);
        p->drawRoundedRect(sliderAreaRect, 4, 4);

        p->setBrush(handleLinear);
        p->drawEllipse(handleRect);
        return;
    }
    default:
        break;

    }
    QCommonStyle::drawComplexControl(cc, opt, p, widget);
}

QPixmap MyStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const
{
    return QCommonStyle::generatedIconPixmap(iconMode, pixmap, opt);
}

int MyStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    switch (m) {
    case PM_SliderControlThickness:
        return 10;
    case PM_SliderLength:
        return 10;
    case PM_SliderThickness:
        return 6;

    default:
        break;
    }
    return QCommonStyle::pixelMetric(m, opt, widget);
}

int MyStyle::styleHint(QStyle::StyleHint sh, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *hret) const
{
    return  QCommonStyle::styleHint(sh, opt, widget, hret);  // SH_Widget_ShareActivation 0 查看源码默认没有使用这个宏，默认返回为0
}

QRect MyStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    switch (cc) {
    case CC_Slider: {
        const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(opt);
        int hwidth = proxy()->pixelMetric(PM_SliderLength, opt, widget);  // 滑块宽度
        int hheight = proxy()->pixelMetric(PM_SliderControlThickness, opt, widget);  // 滑块高度
        int sheight = proxy()->pixelMetric(PM_SliderThickness, opt, widget);  // 滑块区域高度

        switch (sc) {
        case SC_SliderHandle: {
            // 返回滑块部分的矩形坐标，这个地方不是为了画图使用而是为了碰撞检测，hitTestComplexControl函数的使用

            int sliderPositon = sliderPositionFromValue(slider->minimum, slider->maximum, slider->sliderValue, opt->rect.width()-hwidth);

            return QRect(slider->rect.x() + sliderPositon,
                         opt->rect.center().y()-hheight/2, hwidth, hheight);
            }
        case SC_SliderGroove: {
            // 返回已经整个滑块的矩形坐标
            return QRect(slider->rect.x(), opt->rect.center().y()-sheight/2, slider->rect.width(), sheight);
        }
        default:
            break;

        }
        break;
    }
    default:
        break;

    }
    return QCommonStyle::subControlRect(cc, opt, sc, widget);
}

QRect MyStyle::subElementRect(QStyle::SubElement sr, const QStyleOption *opt, const QWidget *widget) const
{
//    qDebug() << sr << widget->metaObject()->className() << __func__;
    QRect r = QCommonStyle::subElementRect(sr, opt, widget);
    switch (sr) {
    case SE_ProgressBarContents: {
        r = widget->rect();
        break;
    }

    case SE_ProgressBarLabel:
        r = subElementRect(QStyle::SE_ProgressBarContents, opt, widget);

        break;

    case SE_ProgressBarGroove:
        r = subElementRect(QStyle::SE_PushButtonContents, opt, widget);
        break;

    default:
        break;
    }

    return r;
}

QSize MyStyle::sizeFromContents(QStyle::ContentsType ct, const QStyleOption *opt, const QSize &csz, const QWidget *widget) const
{
    return QCommonStyle::sizeFromContents(ct, opt, csz, widget);
}
