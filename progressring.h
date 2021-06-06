#ifndef PROGRESSRING_H
#define PROGRESSRING_H

#include <QWidget>

class ProgressRing : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressRing(QWidget *parent = nullptr);


    //设置范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    //设置最大最小值
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);

    //设置目标值
    void setValue(double value);
    void setValue(int value);

    //设置精确度
    void setPrecision(int precision);

    //设置顺时针逆时针转
    void setClockWise(bool clockWise);
    //设置显示百分比
    void setShowPercent(bool showPercent);
    //设置启动自动检验
    void setAlarmMode(int alarmMode);
    //设置起始角度
    void setStartAngle(int startAngle);
    //设置圆环间距
    void setRingPadding(int ringPadding);
    //设置圆环宽度
    void setRingWidth(int ringWidth);

    //设置是否启用动画显示
    void setAnimation(bool animation);
    //设置动画显示的步长
    void setAnimationStep(double animationStep);

    //设置背景颜色
    void setBgColor(const QColor &bgColor);
    //设置文本颜色
    void setTextColor(const QColor &textColor);
    //设置圆环进度颜色
    void setRingColor(const QColor &ringColor);
    //设置圆环背景颜色
    void setRingBgColor(const QColor &ringBgColor);
    //设置中心圆颜色
    void setCircleColor(const QColor &circleColor);

    //设置三种值
    void setRingValue1(int ringValue1);
    void setRingValue2(int ringValue2);
    void setRingValue3(int ringValue3);

    //设置三种颜色
    void setRingColor1(const QColor &ringColor1);
    void setRingColor2(const QColor &ringColor2);
    void setRingColor3(const QColor &ringColor3);


private:
    double minValue;                //最小值
    double maxValue;                //最大值
    double value;                   //目标值
    int precision;                  //精确度,小数点后几位

    bool clockWise;                 //顺时针逆时针
    bool showPercent;               //显示百分比
    int alarmMode;                  //警戒报警模式,进度为不同的颜色
    int startAngle;                 //起始角度
    int ringPadding;                //圆环间距
    int ringWidth;                  //圆环宽度

    bool animation;                 //是否启用动画显示
    double animationStep;           //动画显示时步长

    QColor bgColor;                 //背景颜色
    QColor textColor;               //文字颜色
    QColor ringColor;               //圆环颜色
    QColor ringBgColor;             //圆环进度背景
    QColor circleColor;             //中心圆颜色

    int ringValue1;                 //环形值1
    int ringValue2;                 //环形值2
    int ringValue3;                 //环形值3

    QColor ringColor1;              //环形颜色1
    QColor ringColor2;              //环形颜色2
    QColor ringColor3;              //环形颜色3

    bool reverse;                   //是否往回走
    double currentValue;            //当前值
    QTimer *timer;                  //定时器绘制动画

    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawRing(QPainter *painter);
    void drawPadding(QPainter *painter);
    void drawCircle(QPainter *painter);
    void drawValue(QPainter *painter);
signals:

};

#endif // PROGRESSRING_H
