#include "progressring.h"

#include "QPainter"

#include "QDebug"
ProgressRing::ProgressRing(QWidget *parent) : QWidget(parent)
{

}

void ProgressRing::setValue(int value)
{
    this->currentValue = value;
}
void ProgressRing::setRange(int minValue, int maxValue)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
}

void ProgressRing::setStartAngle(int startAngle)
{
    this->startAngle = startAngle;
}
void ProgressRing::setClockWise(bool clockWise)
{
    this->clockWise = clockWise;
}

void ProgressRing::setBgColor(const QColor &bgColor)
{
    this->bgColor = bgColor;
}
void ProgressRing::setTextColor(const QColor &textColor)
{
    this->textColor = textColor;
}
void ProgressRing::setRingColor(const QColor &ringColor)
{
    this->ringColor = ringColor;
}

void ProgressRing::setRingBgColor(const QColor &ringBgColor)
{
    this->ringBgColor = ringBgColor;
}

void ProgressRing::setCircleColor(const QColor &circleColor)
{
    this->circleColor = circleColor;
}

void ProgressRing::setRingPadding(int ringPadding)
{
    this->ringPadding = ringPadding;
}

void ProgressRing::setRingWidth(int ringWidth)
{
    this->ringWidth = ringWidth;
}


void ProgressRing::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    qDebug()<<QString("width:%1,height:%2").arg(width).arg(height);
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制背景
    drawBg(&painter);
    //绘制进度
    drawRing(&painter);

    //绘制间隔,重新绘制一个圆遮住,产生间距效果
    if (ringPadding > 0) {
        drawPadding(&painter);
    }

    //绘制中间圆
    drawCircle(&painter);
    //绘制当前值
    drawValue(&painter);
}

void ProgressRing::drawBg(QPainter *painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    //这里有个技巧,如果没有间距则设置成圆环的背景色
    painter->setBrush(ringPadding == 0 ? ringBgColor : bgColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void ProgressRing::drawRing(QPainter *painter)
{
    int radius = 99 - ringPadding;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(ringColor);

    QRectF rect(-radius, -radius, radius * 2, radius * 2);

    //计算总范围角度,当前值范围角度,剩余值范围角度
    double angleAll = 360.0;
    double angleCurrent = angleAll * ((currentValue - minValue) / (maxValue - minValue));
    double angleOther = angleAll - angleCurrent;

    qDebug()<<QString("angleCurrent:%1,ringPadding:%2").arg(angleCurrent).arg(ringPadding);

    //如果逆时针
    if (!clockWise) {
        angleCurrent = -angleCurrent;
        angleOther = -angleOther;
    }

    //动态设置当前进度颜色
    QColor color = ringColor;
//    if (alarmMode == 1) {
//        if (currentValue >= ringValue3) {
//            color = ringColor3;
//        } else if (currentValue >= ringValue2) {
//            color = ringColor2;
//        } else {
//            color = ringColor1;
//        }
//    } else if (alarmMode == 2) {
//        if (currentValue <= ringValue1) {
//            color = ringColor1;
//        } else if (currentValue <= ringValue2) {
//            color = ringColor2;
//        } else {
//            color = ringColor3;
//        }
//    }

    //绘制当前值饼圆
    painter->setBrush(color);
    painter->drawPie(rect, (startAngle - angleCurrent) * 16, angleCurrent * 16);

    //绘制剩余值饼圆
    painter->setBrush(ringBgColor);
    painter->drawPie(rect, (startAngle - angleCurrent - angleOther) * 16, angleOther * 16);

    painter->restore();
}

void ProgressRing::drawPadding(QPainter *painter)
{
    int radius = 99 - ringWidth - ringPadding;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void ProgressRing::drawCircle(QPainter *painter)
{
    //文字的区域要减去进度的宽度及间距
    int radius = 99 - ringWidth - (ringPadding * 2);
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(circleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void ProgressRing::drawValue(QPainter *painter)
{
    //文字的区域要减去进度的宽度及间距
    int radius = 99 - ringWidth - (ringPadding * 2);
    painter->save();
    painter->setPen(textColor);

    QFont font;
    int fontSize = radius - (showPercent ? 5 : 6);
    qDebug()<<QString("fontsize:%1,radius:%2,showPercent:%d").arg(fontSize).arg(radius).arg(showPercent);
    font.setPixelSize(fontSize);
    painter->setFont(font);

    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue;
    if (showPercent) {
        double percent = (currentValue * 100) / (maxValue - minValue);
        strValue = QString("%1%").arg(percent, 0, 'f', precision);
    } else {
        strValue = QString("%1").arg(currentValue, 0, 'f', precision);
    }

    painter->drawText(textRect, Qt::AlignCenter, strValue);

    painter->restore();
}
