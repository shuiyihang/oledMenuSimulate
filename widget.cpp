#include "widget.h"
#include "ui_widget.h"
#include "QPainter"

#include "userPage.h"

#include "config.h"
#include "QDebug"



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

    mainMenu.configSetInit(&mainMenu.operat_config);

    view_Timer.setInterval(FRESH_INTERVAL);
}

void Widget::ViewStart()
{
    view_Timer.start();
    connect(&view_Timer, &QTimer::timeout, [=](){


        if(mainMenu.menuHandle.need_refresh || mainMenu.menuHandle.cur_type & LEAF_DYN_ASSERT){
            mainMenu.menuHandle.need_refresh = 0;

            update();
        }

    });
}


void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QFont font("Microsoft YaHei",10, 75);
    QPen pen = painter.pen();

    /**设置画笔**/
    pen.setColor(QColor("#00fffc"));//QColor("#99ffff")
    pen.setWidth(2);

    /**设置背景色**/
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,QColor("#0b0b0b"));
    this->setPalette(palette);


    /**设置字体**/
    font.setPixelSize(LIST_FONT_SIZE);
    painter.setFont(font);
    painter.setPen(pen);



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
    qDebug()<<QString("%1").arg(event->key());
    switch (event->key()) {
    case Qt::Key_Up:
        if(__get_node_type(mainMenu.menuHandle.cur_type) == NON_LEAF_SIGN)
        {
            if(mainMenu.menuHandle.edit_mode){//处于可编辑模式
                mainMenu.updata_Binding_param(&mainMenu.menuHandle,1);
            }else{
                mainMenu.chooseCursorUp(&mainMenu.menuHandle);
                mainMenu.menuHandle.need_refresh = 1;
            }
        }

        qDebug()<< "Up";
        break;
    case Qt::Key_Down:
        if(__get_node_type(mainMenu.menuHandle.cur_type) == NON_LEAF_SIGN)
        {
            if(mainMenu.menuHandle.edit_mode){//处于可编辑模式
                mainMenu.updata_Binding_param(&mainMenu.menuHandle,0);
            }else{
                mainMenu.chooseCursorDown(&mainMenu.menuHandle);
                mainMenu.menuHandle.need_refresh = 1;
            }
        }
        qDebug()<<"Down";
        break;
    case Qt::Key_Left:
        if(mainMenu.menuHandle.edit_mode){
            mainMenu.menuHandle.edit_mode = 0;
            mainMenu.menuHandle.need_refresh = 1;
        }else{
            mainMenu.enterExit_to_newPage(&mainMenu.menuHandle,RETURN_PAGE);
        }

        qDebug()<<"left";
        break;
    case Qt::Key_Right:
        mainMenu.enterExit_to_newPage(&mainMenu.menuHandle,ENTER_PAGE);
        qDebug()<<"right";
        break;
    case Qt::Key_Return:

        if(mainMenu.menuHandle.cur_type != NON_LEAF_EDIT){
            mainMenu.select_verify_deal(&mainMenu.menuHandle);
        }else{
            //进入编辑模式
            mainMenu.menuHandle.edit_mode = 1;
            mainMenu.menuHandle.need_refresh = 1;
        }
        qDebug()<<"enter";
        break;

    }

}


