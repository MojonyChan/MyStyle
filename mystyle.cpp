#include "mystyle.h"

#include <QGraphicsEffect>
#include <QDebug>
#include <qdrawutil.h>

namespace  {
void drawShadow(QPainter *painter, const QStyleOption *option, int borderW)
{
    // 绘制边框
    QLinearGradient leftG(option->rect.x() + borderW, option->rect.y() + borderW, option->rect.x(), option->rect.y()+borderW);
    QLinearGradient topG(option->rect.x() + borderW, option->rect.y() + borderW, option->rect.x()+borderW, option->rect.y());
    QLinearGradient rightG(option->rect.width()-borderW, option->rect.y() + borderW, option->rect.width(), option->rect.y() + borderW);
    QLinearGradient bottomG(option->rect.x() + borderW, option->rect.height()-borderW, option->rect.x()+borderW, option->rect.height());
    QRadialGradient leftTopG(option->rect.x() + borderW, option->rect.y() + borderW, borderW);
    QRadialGradient leftBottomG(option->rect.x() + borderW, option->rect.height()-borderW, borderW);
    QRadialGradient rightTopG(option->rect.width()-borderW, option->rect.y() + borderW, borderW);
    QRadialGradient rightBottomG(option->rect.width()-borderW, option->rect.height()-borderW, borderW);

    auto colorSetting = [](QGradient &g){
        g.setColorAt(0, QColor(100, 100, 100, 100));
        g.setColorAt(1, QColor(255, 255, 255, 0));
    };

    colorSetting(leftG);
    colorSetting(rightG);
    colorSetting(topG);
    colorSetting(bottomG);
    colorSetting(leftTopG);
    colorSetting(leftBottomG);
    colorSetting(rightTopG);
    colorSetting(rightBottomG);

    painter->setPen(Qt::NoPen);

    painter->setBrush(QColor(100, 100, 100, 100));
    painter->drawRect(option->rect - QMargins(borderW, borderW, borderW, borderW));

    painter->setBrush(leftG);
    painter->drawRect(option->rect.x(), option->rect.y() + borderW, borderW, option->rect.height()-2*borderW);
    painter->setBrush(rightG);
    painter->drawRect(option->rect.width()-borderW, option->rect.y() + borderW, borderW, option->rect.height()-2*borderW);
    painter->setBrush(topG);
    painter->drawRect(option->rect.x()+borderW, option->rect.y(), option->rect.width()-2*borderW, borderW);
    painter->setBrush(bottomG);
    painter->drawRect(option->rect.x()+borderW, option->rect.height()-borderW, option->rect.width()-2*borderW, borderW);

    painter->save();

    // 小矩形区域和面板边框区域总和
    painter->setClipRegion(QRegion(option->rect.x(), option->rect.y(), borderW, borderW));
    painter->setBrush(leftTopG);
    painter->drawEllipse(QPoint(option->rect.x()+borderW, option->rect.y()+borderW), borderW, borderW);
    painter->restore();

    painter->save();
    painter->setBrush(leftBottomG);
    painter->setClipRegion(QRegion(option->rect.x(), option->rect.height()-borderW, borderW, borderW));
    painter->drawEllipse(QPoint(option->rect.x()+borderW, option->rect.height()-borderW), borderW, borderW);
    painter->restore();

    painter->save();
    painter->setBrush(rightTopG);
    painter->setClipRegion(QRegion(option->rect.width()-borderW, option->rect.y(), borderW, borderW));
    painter->drawEllipse(QPoint(option->rect.width()-borderW, option->rect.y()+borderW), borderW, borderW);
    painter->restore();

    painter->save();
    painter->setBrush(rightBottomG);
    painter->setClipRegion(QRegion(option->rect.width()-borderW, option->rect.height()-borderW, borderW, borderW));
    painter->drawEllipse(QPoint(option->rect.width()-borderW, option->rect.height()-borderW), borderW, borderW);
    painter->restore();
}

static QWindow *qt_getWindow(const QWidget *widget)
{
    return widget ? widget->window()->windowHandle() : nullptr;
}
}

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
    case PE_PanelMenu: {
        int borderW = pixelMetric(PM_MenuPanelWidth);

        QLinearGradient linearG(QPoint(option->rect.x(), option->rect.y()), QPoint(option->rect.x() + option->rect.width(), option->rect.y() + option->rect.height()));
        linearG.setColorAt(0, QColor(255,182,193, 180));
        linearG.setColorAt(0.5, QColor(100,149,237, 180));
        linearG.setColorAt(1, QColor(255,222,173, 180));

        painter->setPen(Qt::NoPen);
        painter->setBrush(linearG);
        painter->drawRoundedRect(option->rect - QMargins(borderW, borderW, borderW, borderW), 5, 5);
        return;
    }

    case PE_FrameMenu:
        // == PE_Frame 不能省略，否则实际效果出现一个边框
        return;

    case PE_PanelButtonCommand: {
        const QStyleOptionDelayButton *delayButton = qstyleoption_cast<const QStyleOptionDelayButton *>(option);
        if (delayButton) {
            // 这里画主面板
            painter->setPen(Qt::NoPen);
            if (delayButton->state & QStyle::State_MouseOver)
                painter->setBrush(QColor("#D3D3D3").dark(110));
            else
                painter->setBrush(QColor("#D3D3D3"));
            painter->drawRoundedRect(delayButton->rect, 5, 5);
        }
        return;
    }

    case PE_PanelLineEdit: {
        // LineEdit的面板，可以提供状态选择
        const QStyleOptionFrame *lineEdit = qstyleoption_cast<const QStyleOptionFrame *>(option);
        QLinearGradient linearG(QPoint(option->rect.x(), option->rect.y()), QPoint(option->rect.x() + option->rect.width(), option->rect.y() + option->rect.height()));

        linearG.setColorAt(0, QColor(255, 182, 193, 180));
        linearG.setColorAt(0.5, QColor(100, 149, 237, 180));
        linearG.setColorAt(1, QColor(255, 222, 173, 180));
        painter->setPen(Qt::NoPen);
        painter->setBrush(linearG);
        if (!(lineEdit->state & QStyle::State_HasFocus))
            painter->drawRoundedRect(option->rect-QMargins(lineEdit->lineWidth, lineEdit->lineWidth,
                                                           lineEdit->lineWidth, lineEdit->lineWidth),
                                     lineEdit->lineWidth, lineEdit->lineWidth);
        else
            painter->fillRect(option->rect-QMargins(lineEdit->lineWidth, lineEdit->lineWidth,
                            lineEdit->lineWidth, lineEdit->lineWidth), linearG);
        proxy()->drawPrimitive(PE_FrameLineEdit, lineEdit, painter, widget);

        return;
    }
    case PE_FrameLineEdit: {
        // LineEdit的边框，可以设置状态对边框的显示
        const QStyleOptionDelayButton *delayButton = qstyleoption_cast<const QStyleOptionDelayButton *>(option);
        if (delayButton)
            return;

        const QStyleOptionFrame *lineEdit = qstyleoption_cast<const QStyleOptionFrame *>(option);
        bool focus = lineEdit->state & QStyle::State_HasFocus;

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setClipRegion(QRegion(option->rect).subtracted(option->rect-QMargins(lineEdit->lineWidth, lineEdit->lineWidth,
                                                   lineEdit->lineWidth, lineEdit->lineWidth)));

        if (focus) {
            painter->setBrush(Qt::darkCyan);
        } else {
            painter->setBrush(Qt::transparent);

        }
        painter->drawRoundedRect(lineEdit->rect, lineEdit->lineWidth, lineEdit->lineWidth);
        painter->restore();
        return;
    }

    default:
        break;
    }

//    qDebug() << element << widget->metaObject()->className() << __func__;
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

    case CE_MenuItem: {
        const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
        bool hover = menuItem->state & State_Selected && menuItem->state & State_Enabled;
        QColor textC(QColor(50, 50, 50));
        QColor backgroundC(Qt::transparent);
        int iw = pixelMetric(PM_SmallIconSize);
        if (hover) {
            textC = Qt::white;
            backgroundC = QColor(100, 149, 237);
        }

        if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
            p->setPen(textC);
            p->setBrush(Qt::NoBrush);
            p->drawLine(menuItem->rect.x(), menuItem->rect.center().y(), menuItem->rect.x()+menuItem->rect.width(), menuItem->rect.center().y());
        }

        p->save();
        p->setPen(Qt::NoPen);
        p->setBrush(backgroundC);
        p->drawRoundedRect(menuItem->rect, 5, 5);
        p->restore();

        p->save();
        p->setPen(textC);
        p->setBrush(Qt::NoBrush);
        p->drawText(menuItem->rect - QMargins(iw+8, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, menuItem->text);
        p->restore();

        if (!menuItem->icon.isNull()) {
            QPixmap iconP = menuItem->icon.pixmap(iw, iw);
            p->save();
            QPainterPath path;

            path.addEllipse(QRect(menuItem->rect.x()+2, menuItem->rect.y()+(menuItem->rect.height()-iw)/2, iw, iw));
            p->setClipPath(path);
            p->drawPixmap(menuItem->rect.x()+2, menuItem->rect.y()+(menuItem->rect.height()-iw)/2, iconP);
            p->restore();
        }
        return;
    }

    case CE_PushButton: {
        const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(opt);
        if (!button || !(button->features & QStyleOptionDelayButton::DelayBytton))
            return QCommonStyle::drawControl(element, opt, p, widget);

        const QStyleOptionDelayButton *delayButton = qstyleoption_cast<const QStyleOptionDelayButton *>(opt);

        if (delayButton) {
            proxy()->drawControl(CE_PushButtonBevel, delayButton, p, widget);

            QStyleOptionDelayButton subopt = *delayButton;
            proxy()->drawControl(CE_PushButtonLabel, &subopt, p, widget);
        }

        return;
    }

    case CE_PushButtonBevel: {
        if (const QStyleOptionDelayButton *button = qstyleoption_cast<const QStyleOptionDelayButton *>(opt)) {
            QStyleOptionDelayButton b(*button);  // 如果这里自定义的StyleOption未定义拷贝构造，就会出现数据错乱
            // no PM_ButtonShiftHorizontal and PM_ButtonShiftVertical for fusion style
            b.state &= ~(State_On | State_Sunken);
            QCommonStyle::drawControl(element, &b, p, widget);
        }
        return;
    }

    case CE_PushButtonLabel: {
        const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(opt);
        if (!button || !(button->features & QStyleOptionDelayButton::DelayBytton))
            return QCommonStyle::drawControl(element, opt, p, widget);

        const QStyleOptionDelayButton *delayButton = qstyleoption_cast<const QStyleOptionDelayButton *>(opt);
        if (delayButton) {
            if (delayButton->progress >= 0) {
                QLinearGradient linear(delayButton->rect.x(), delayButton->rect.y(),
                                       delayButton->rect.x()+delayButton->rect.width(), delayButton->rect.y()+delayButton->rect.height());
                linear.setColorAt(0, QColor(255,182,193));
                linear.setColorAt(0.5, QColor(100,149,237));
                linear.setColorAt(1, QColor(255,222,173));

                p->setBrush(linear);
                p->drawRoundedRect(QRect(delayButton->rect.x(), delayButton->rect.y(),
                                    int(delayButton->rect.width()*delayButton->progress), delayButton->rect.height()), 5, 5);
            }
        }  // end if

        QRegion leftTextRect = QRect(delayButton->rect.x(), delayButton->rect.y(), int(delayButton->rect.width()*delayButton->progress), delayButton->rect.height());
        QRegion rightTextRect = QRegion(delayButton->rect).subtracted(leftTextRect);

        if (!delayButton->text.isEmpty() && delayButton->icon.isNull()) {
            // 切割区域形成渐变

            if (!leftTextRect.isNull()) {
                // 左侧被覆盖区域白色字体
                p->save();
                p->setClipRegion(leftTextRect);
                p->setPen(Qt::white);
                p->drawText(opt->rect, Qt::AlignCenter, delayButton->text);
                p->restore();
            }

            p->save();
            p->setClipRegion(rightTextRect);
            p->setPen(Qt::black);
            p->drawText(opt->rect, Qt::AlignCenter, delayButton->text);
            p->restore();
        } else if (!delayButton->icon.isNull()) {
            // 只有一张图片的时候，只显示图片
            QIcon::Mode mode = button->state & State_Enabled ? QIcon::Normal : QIcon::Disabled;
            if (mode == QIcon::Normal && button->state & State_HasFocus)
                mode = QIcon::Active;
            QIcon::State state = QIcon::Off;
            if (button->state & State_On)
                state = QIcon::On;

            QPixmap pixmap = delayButton->icon.pixmap(qt_getWindow(widget), delayButton->iconSize, mode, state);

            if (delayButton->text.isEmpty()) {
                p->drawPixmap(QPoint(((delayButton->rect.width()-delayButton->rect.x()) - pixmap.width())/2, ((delayButton->rect.height()-delayButton->rect.y()) - pixmap.height())/2), pixmap);
            } else {
                // 既有图标又有文字
                int iconSpacing = 4;

                // 画图标
                p->drawPixmap(iconSpacing, ((delayButton->rect.height()-delayButton->rect.y()) - pixmap.height())/2, pixmap);

                // 画文字
                if (!leftTextRect.isNull()) {
                    // 左侧被覆盖区域白色字体
                    p->save();
                    p->setClipRegion(leftTextRect);
                    p->setPen(Qt::white);
                    p->drawText(opt->rect-QMargins(2*iconSpacing+pixmap.width(), 0, 0, 0), Qt::AlignVCenter, delayButton->text);
                    p->restore();
                }

                p->save();
                p->setClipRegion(rightTextRect);
                p->setPen(Qt::black);
                p->drawText(opt->rect-QMargins(2*iconSpacing+pixmap.width(), 0, 0, 0), Qt::AlignVCenter, delayButton->text);
                p->restore();
            }

        }  // end else if
        return;
    }  // end CE_PushButtonLabel

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
    case PM_MenuPanelWidth:
        return 6;
    case PM_SmallIconSize:
        return 16;

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
        r = subElementRect(QStyle::SE_ProgressBarContents, opt, widget);
        break;

    case SE_PushButtonContents:
//        if (const QStyleOptionDelayButton *btn = qstyleoption_cast<const QStyleOptionDelayButton *>(opt)) {
//            r = opt->rect;
//        }

        break;
    default:
        break;
    }

    return r;
}

QSize MyStyle::sizeFromContents(QStyle::ContentsType ct, const QStyleOption *opt, const QSize &csz, const QWidget *widget) const
{
    QSize newSize = QCommonStyle::sizeFromContents(ct, opt, csz, widget);

    switch (ct) {
    case CT_MenuItem: {
        if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(opt)) {
            int w = newSize.width();
            int maxpmw = menuItem->maxIconWidth;
            int tabSpacing = 20;
            int iconw = pixelMetric(PM_SmallIconSize);

            if (menuItem->text.contains(QLatin1Char('\t')))
                w += tabSpacing;
            else if (menuItem->menuItemType == QStyleOptionMenuItem::DefaultItem) {
                QFontMetrics fm(menuItem->font);
                QFont fontBold = menuItem->font;
                fontBold.setBold(true);
                QFontMetrics fmBold(fontBold);
                w += fmBold.horizontalAdvance(menuItem->text) - fm.horizontalAdvance(menuItem->text);
            }
            w += maxpmw + iconw + 6;
            newSize.setWidth(w);
            if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
                if (!menuItem->text.isEmpty()) {
                    newSize.setHeight(menuItem->fontMetrics.height());
                }
            }
            newSize.setWidth(newSize.width());
        }

        return newSize;
    }
    default:
        break;

    }

    return newSize;
}

void MyStyle::polish(QWidget *widget)
{
    if (widget->inherits("QMenu")) {
        widget->setAttribute(Qt::WA_TranslucentBackground);
        widget->setWindowFlags(widget->windowFlags() | Qt::NoDropShadowWindowHint | Qt::FramelessWindowHint);
        widget->setMouseTracking(true);

        QGraphicsDropShadowEffect *dropEffect = new QGraphicsDropShadowEffect;
        dropEffect->setOffset(0);
        dropEffect->setBlurRadius(2 * pixelMetric(QStyle::PM_MenuPanelWidth));

        widget->setGraphicsEffect(dropEffect);

    } else if (widget->inherits("DelayButton")) {
        widget->setAttribute(Qt::WA_OpaquePaintEvent, false);

    }

}
