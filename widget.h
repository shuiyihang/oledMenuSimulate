#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QTimer"
#include "QPaintEvent"
#include "QKeyEvent"

#include "managermenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    /**初始化定时器*/
    void ViewInit();

    /**启动定时器*/
    void ViewStart();


    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);



    ManagerMenu mainMenu;


    QTimer view_Timer;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
