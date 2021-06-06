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
    //����׼������,���÷����,ƽ������������,�ȱ�������
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //���Ʊ���
    drawBg(&painter);
    //���ƽ���
    drawRing(&painter);

    //���Ƽ��,���»���һ��Բ��ס,�������Ч��
    if (ringPadding > 0) {
        drawPadding(&painter);
    }

    //�����м�Բ
    drawCircle(&painter);
    //���Ƶ�ǰֵ
    drawValue(&painter);
}

void ProgressRing::drawBg(QPainter *painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    //�����и�����,���û�м�������ó�Բ���ı���ɫ
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

    //�����ܷ�Χ�Ƕ�,��ǰֵ��Χ�Ƕ�,ʣ��ֵ��Χ�Ƕ�
    double angleAll = 360.0;
    double angleCurrent = angleAll * ((currentValue - minValue) / (maxValue - minValue));
    double angleOther = angleAll - angleCurrent;

    qDebug()<<QString("angleCurrent:%1,ringPadding:%2").arg(angleCurrent).arg(ringPadding);

    //�����ʱ��
    if (!clockWise) {
        angleCurrent = -angleCurrent;
        angleOther = -angleOther;
    }

    //��̬���õ�ǰ������ɫ
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

    //���Ƶ�ǰֵ��Բ
    painter->setBrush(color);
    painter->drawPie(rect, (startAngle - angleCurrent) * 16, angleCurrent * 16);

    //����ʣ��ֵ��Բ
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
    //���ֵ�����Ҫ��ȥ���ȵĿ�ȼ����
    int radius = 99 - ringWidth - (ringPadding * 2);
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(circleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void ProgressRing::drawValue(QPainter *painter)
{
    //���ֵ�����Ҫ��ȥ���ȵĿ�ȼ����
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
