#include "delaybutton.h"
#include <QDebug>

bool DelayButton::hasPressed = false;

DelayButton::DelayButton(QWidget *parent) : QPushButton(parent)
{
    delayAnimation = new QPropertyAnimation(this, "delayProgress", this);

}

void DelayButton::setDelayTime(int time)
{
    mDelayTime = time;
}

double DelayButton::delayProgress() const
{
    return mProgress;
}

void DelayButton::enterEvent(QEvent *)
{
    enterWidget = true;
    update();

}

bool DelayButton::hitButton(const QPoint &pos) const
{
    bool hit = QPushButton::hitButton(pos);
    if (hit) {
        if (delayAnimation->state() == QPropertyAnimation::Stopped) {
            hasPressed = true;
            delayAnimation->setStartValue(0);
            delayAnimation->setEndValue(1);
            delayAnimation->setDuration(mDelayTime);
            delayAnimation->start();
        } else if (delayAnimation->state() == QPropertyAnimation::Running) {
            // 正在返回动画
            hasPressed = true;
            delayAnimation->pause();
            delayAnimation->setStartValue(mProgress);
            delayAnimation->setEndValue(1);
            delayAnimation->setDuration(int(mDelayTime*(1-mProgress)));
            delayAnimation->setCurrentTime(0);
            if (delayAnimation->state() == QPropertyAnimation::Paused)
                delayAnimation->resume();
            else if (delayAnimation->state() == QPropertyAnimation::Stopped)
                delayAnimation->start();
        }
    }

    return hit;
}

void DelayButton::mouseReleaseEvent(QMouseEvent *)
{
    if (hasPressed) {
        if (mProgress == 1.0) {
            // 延时按钮已经结束 不需要再返回  直接保存这个样式
            hasPressed = false;
            return;
        }

        if (delayAnimation->state() == QPropertyAnimation::Running)
            delayAnimation->pause();
        delayAnimation->setStartValue(mProgress);
        delayAnimation->setEndValue(0);
        delayAnimation->setDuration(500);
        delayAnimation->setCurrentTime(0);
        if (delayAnimation->state() == QPropertyAnimation::Paused)
            delayAnimation->resume();
        else if (delayAnimation->state() == QPropertyAnimation::Stopped)
            delayAnimation->start();
        hasPressed = false;
    }
}

void DelayButton::mouseMoveEvent(QMouseEvent *)
{
    return;
}

void DelayButton::leaveEvent(QEvent *)
{
    enterWidget = false;
    update();
}

void DelayButton::initStyleOption(QStyleOptionButton *style) const
{
    QPushButton::initStyleOption(style);
    QStyleOptionDelayButton *delayButton = qstyleoption_cast<QStyleOptionDelayButton *>(style);

    delayButton->initFrom(this);

    if (!delayButton)
        return;
    delayButton->delayTime = mDelayTime;
    delayButton->progress = mProgress;

    // 强制类型转换才能使用 |= 符号  使用Q_DECLARE_OPERATORS_FOR_FLAGS(QStyleOptionButton::ButtonFeatures) 会出现类型重定义
    delayButton->features |= QStyleOptionButton::ButtonFeature(QStyleOptionDelayButton::DelayBytton);

    if (enterWidget)
        delayButton->state |= QStyle::State_MouseOver;
    else
        delayButton->state &= (~QStyle::State_MouseOver);

    if (this->isFlat())
        delayButton->features |= QStyleOptionButton::Flat;
#if QT_CONFIG(menu)
    if (this->menu())
        delayButton->features |= QStyleOptionButton::HasMenu;
#endif
    if (autoDefault())
        delayButton->features |= QStyleOptionButton::AutoDefaultButton;
    if (this->isDefault())
        delayButton->features |= QStyleOptionButton::DefaultButton;
    if (this->isChecked())
        delayButton->state |= QStyle::State_On;
    if (!this->isFlat() && !this->isDown())
        delayButton->state |= QStyle::State_Raised;
    delayButton->text = this->text();
    delayButton->icon = this->icon();
    delayButton->iconSize = iconSize();
}

void DelayButton::setProgress(const double progress)
{
    this->mProgress = progress;
    this->update();
}

void DelayButton::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);

    QStyleOptionDelayButton bstyle;
    initStyleOption(&bstyle);
    p.drawControl(QStyle::CE_PushButton, bstyle);
}

