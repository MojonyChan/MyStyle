#include "mystyle.h"
#include <QDebug>

void MyStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{

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
    QCommonStyle::drawComplexControl(cc, opt, p, widget);
}

QPixmap MyStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const
{
    return QCommonStyle::generatedIconPixmap(iconMode, pixmap, opt);
}

int MyStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    return QCommonStyle::pixelMetric(m, opt, widget);
}

int MyStyle::styleHint(QStyle::StyleHint sh, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *hret) const
{
    return  QCommonStyle::styleHint(sh, opt, widget, hret);  // SH_Widget_ShareActivation 0 查看源码默认没有使用这个宏，默认返回为0
}

QRect MyStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    qDebug() << cc;
    return QCommonStyle::subControlRect(cc, opt, sc, widget);
}

QRect MyStyle::subElementRect(QStyle::SubElement sr, const QStyleOption *opt, const QWidget *widget) const
{
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
