#ifndef DELAYBUTTON_H
#define DELAYBUTTON_H

#include <QPushButton>
#include <QStyleOptionButton>
#include <QStylePainter>
#include <QPropertyAnimation>
#include <QMenu>


class QStyleOptionDelayButton :public QStyleOptionButton
{
public:
    QStyleOptionDelayButton(){}
    QStyleOptionDelayButton(const QStyleOptionDelayButton &c) :QStyleOptionButton(c)
    {
        this->state = c.state;
        this->features = c.features;
        this->delayTime = c.delayTime;
        this->progress = c.progress;
        this->text = c.text;
        this->icon = c.icon;
        this->iconSize = c.iconSize;
    }

    enum ButtonFeature{
        DelayBytton = (CommandLinkButton << 1)
    };

    int delayTime;  // 毫秒
    qreal progress;

};

class DelayButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(double delayProgress READ delayProgress WRITE setProgress)

public:
    explicit DelayButton(QWidget *parent = nullptr);
    void setDelayTime(int time);
    void setColor(const QColor color);
    void color() const;
    double delayProgress() const;

protected:
    void paintEvent(QPaintEvent *) override;
    void leaveEvent(QEvent *) override;
    void enterEvent(QEvent *) override;
    bool hitButton(const QPoint &pos) const override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void initStyleOption(QStyleOptionButton *style) const;
    void setProgress(const double progress);

private:
    int mDelayTime = 3000;
    double mProgress = 0;
    static bool hasPressed;
    bool enterWidget = false;
    QPropertyAnimation *delayAnimation;
    QColor mColor;

signals:

public slots:
};

#endif // DELAYBUTTON_H
