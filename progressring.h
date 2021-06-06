#ifndef PROGRESSRING_H
#define PROGRESSRING_H

#include <QWidget>

class ProgressRing : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressRing(QWidget *parent = nullptr);


    //���÷�Χֵ
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    //���������Сֵ
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);

    //����Ŀ��ֵ
    void setValue(double value);
    void setValue(int value);

    //���þ�ȷ��
    void setPrecision(int precision);

    //����˳ʱ����ʱ��ת
    void setClockWise(bool clockWise);
    //������ʾ�ٷֱ�
    void setShowPercent(bool showPercent);
    //���������Զ�����
    void setAlarmMode(int alarmMode);
    //������ʼ�Ƕ�
    void setStartAngle(int startAngle);
    //����Բ�����
    void setRingPadding(int ringPadding);
    //����Բ�����
    void setRingWidth(int ringWidth);

    //�����Ƿ����ö�����ʾ
    void setAnimation(bool animation);
    //���ö�����ʾ�Ĳ���
    void setAnimationStep(double animationStep);

    //���ñ�����ɫ
    void setBgColor(const QColor &bgColor);
    //�����ı���ɫ
    void setTextColor(const QColor &textColor);
    //����Բ��������ɫ
    void setRingColor(const QColor &ringColor);
    //����Բ��������ɫ
    void setRingBgColor(const QColor &ringBgColor);
    //��������Բ��ɫ
    void setCircleColor(const QColor &circleColor);

    //��������ֵ
    void setRingValue1(int ringValue1);
    void setRingValue2(int ringValue2);
    void setRingValue3(int ringValue3);

    //����������ɫ
    void setRingColor1(const QColor &ringColor1);
    void setRingColor2(const QColor &ringColor2);
    void setRingColor3(const QColor &ringColor3);


private:
    double minValue;                //��Сֵ
    double maxValue;                //���ֵ
    double value;                   //Ŀ��ֵ
    int precision;                  //��ȷ��,С�����λ

    bool clockWise;                 //˳ʱ����ʱ��
    bool showPercent;               //��ʾ�ٷֱ�
    int alarmMode;                  //���䱨��ģʽ,����Ϊ��ͬ����ɫ
    int startAngle;                 //��ʼ�Ƕ�
    int ringPadding;                //Բ�����
    int ringWidth;                  //Բ�����

    bool animation;                 //�Ƿ����ö�����ʾ
    double animationStep;           //������ʾʱ����

    QColor bgColor;                 //������ɫ
    QColor textColor;               //������ɫ
    QColor ringColor;               //Բ����ɫ
    QColor ringBgColor;             //Բ�����ȱ���
    QColor circleColor;             //����Բ��ɫ

    int ringValue1;                 //����ֵ1
    int ringValue2;                 //����ֵ2
    int ringValue3;                 //����ֵ3

    QColor ringColor1;              //������ɫ1
    QColor ringColor2;              //������ɫ2
    QColor ringColor3;              //������ɫ3

    bool reverse;                   //�Ƿ�������
    double currentValue;            //��ǰֵ
    QTimer *timer;                  //��ʱ�����ƶ���

    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawRing(QPainter *painter);
    void drawPadding(QPainter *painter);
    void drawCircle(QPainter *painter);
    void drawValue(QPainter *painter);
signals:

};

#endif // PROGRESSRING_H
