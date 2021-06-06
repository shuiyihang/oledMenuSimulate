#include "widget.h"
#include "ui_widget.h"
#include "QPainter"

#include "userPage.h"

#include "config.h"
#include "QDebug"

//定义开关的风格
iconInfo_Typedef text_onoff = {
    .on_icon = "开",
    .off_icon = "关",
};

iconInfo_Typedef sign_onoff = {
    .on_icon = "√",
    .off_icon = " ",
};



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ViewInit();

    ViewStart();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::ViewInit()
{
    setFixedSize(WIN_WIDTH,WIN_HEIGHT);
    setWindowTitle(WIN_TITLE);

    view_Timer.setInterval(FRESH_INTERVAL);
}

void Widget::ViewStart()
{
    view_Timer.start();
    connect(&view_Timer, &QTimer::timeout, [=](){


        if(mainMenu.menuHandle.need_refresh){
            mainMenu.menuHandle.need_refresh = 0;

            update();
        }

    });
}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFont font = painter.font();
    font.setPixelSize(LIST_FONT_SIZE);
    painter.setFont(font);


    MenuItem_Typedef *pos;
    struct single_list_head *ptr = mainMenu.menuHandle.cur_list_head;

    pos = list_entry(ptr,MenuItem_Typedef,localPos);

    mainMenu.menuHandle.cur_type = pos->unitType;
    if(__get_node_type(pos->unitType) == OPEN_LEAF_SIGN){
        pos->endPageDeal(pos,painter);
    }else if(__get_node_type(pos->unitType) == NON_LEAF_SIGN){
        pos->showMenu(pos,painter);
    }



}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if(__get_node_type(mainMenu.menuHandle.cur_type) == NON_LEAF_SIGN)
        {
            mainMenu.chooseCursorUp(&mainMenu.menuHandle);
            mainMenu.menuHandle.need_refresh = 1;
        }

        qDebug()<< "Up";
        break;
    case Qt::Key_Down:
        if(__get_node_type(mainMenu.menuHandle.cur_type) == NON_LEAF_SIGN)
        {
            mainMenu.chooseCursorDown(&mainMenu.menuHandle);
            mainMenu.menuHandle.need_refresh = 1;

        }
        qDebug()<<"Down";
        break;
    case Qt::Key_Left:
        qDebug()<<"left";
        break;
    case Qt::Key_Right:
        qDebug()<<"right";
        break;
    }

}


