#include "managermenu.h"
#include "config.h"
#include "QPixmap"

#include "QDebug"
ManagerMenu::ManagerMenu()
{
    MenuInit();
    currentHandleInit();
}

ManagerMenu::~ManagerMenu()
{
    free_branch_auto(UniversalNode);
}



curHandle_Typedef ManagerMenu::menuHandle;
/**菜单里的全局配置*/
configSet_Typedef ManagerMenu::operat_config;


//定义开关的风格
iconInfo_Typedef text_onoff = {
    .on_icon = "开",
    .off_icon = "关",
};

iconInfo_Typedef sign_onoff = {
    .on_icon = ":/image/on_oled.png",
    .off_icon = ":/image/off_oled.png",
};

/*秒表计时功能*/


void ManagerMenu::MenuInit()
{

    UniversalNode = branchCreate(NON_LEAF,"通用",simulate_main_logo_page,NULL);

    rootNode = branchCreate(NON_LEAF,"设置",simulate_show_list_page,LOGO_SET);
    BluetoothNode = branchCreate(NON_LEAF,"蓝牙",simulate_show_option_icon,LOGO_BLE);//增加蓝牙开关控制节点
    NotifyNode = leafCreate(LEAF_OPEN_REFRESH, "时间",show_dynamic_time_page,NULL,LOGO_TIM);//实时显示当前时间
    TimeNode = leafCreate(LEAF_OPEN_REFRESH,"电量",battery_ring_page,NULL,LOGO_BAT);//动态显示


    KeyNode = branchCreate(NON_LEAF,"键盘",simulate_show_list_page,NULL);

    //添加,翻页测试使用
    PidNode = branchCreate(NON_LEAF_EDIT, "调参",simulate_edit_param_task,LOGO_PID);//静态显示的

    P_param = leafCreate(LEAF_CLOSE_EDIT, "P:",test_turn_page,NULL,NULL);
    I_param = leafCreate(LEAF_CLOSE_EDIT, "I:",test_turn_page,NULL,NULL);
    D_param = leafCreate(LEAF_CLOSE_EDIT, "D:",test_turn_page,NULL,NULL);

    bindParamInit(P_param,&operat_config.p_pid);
    bindParamInit(I_param,&operat_config.i_pid);
    bindParamInit(D_param,&operat_config.d_pid);


    HotsportNode = leafCreate(LEAF_OPEN_REFRESH, "个人热点",test_turn_page,NULL,NULL);//静态显示的
    NoDisturbNode = leafCreate(LEAF_OPEN_REFRESH, "勿扰模式",test_turn_page,NULL,NULL);//静态显示的
    /////////////

    PhoneNode = leafCreate(LEAF_OPEN_REFRESH, "关于本机",test_turn_page,NULL,NULL);//静态显示的


    CorrectNode = branchCreate(NON_LEAF,"自动改正",simulate_show_option_icon,NULL);//增加开关控制节点
    oneHandleNode = branchCreate(NON_LEAF,"单手键盘",simulate_show_option_icon,NULL);//增加开关控制节点
    slideInputNode = branchCreate(NON_LEAF,"滑行键入",simulate_show_option_icon,NULL);




    BluetoothNode_1 = SpecialleafCreate(LEAF_CLOSE_OFF, "蓝牙",blueTooth_page_deal, &sign_onoff);//增加蓝牙开关控制节点
//    CorrectNode_1 = leafCreate(LEAF_CLOSE_MULTI_DISEN, "自动改正",autoCorrct_page_deal, NULL);
//    slideInputNode_1 = leafCreate(LEAF_CLOSE_MULTI_DISEN, "滑行键入",glide_page_deal,NULL);
//    oneHandleNode_1 = leafCreate(LEAF_CLOSE_NOMULTI_EN, "左",oneHandle_page_deal, NULL);
//    oneHandleNode_2 = leafCreate(LEAF_CLOSE_NOMULTI_DISEN, "中",oneHandle_page_deal, NULL);
//    oneHandleNode_3 = leafCreate(LEAF_CLOSE_NOMULTI_DISEN, "右",oneHandle_page_deal, NULL);

    tree_node_binding_oneTime(4, UniversalNode,rootNode,PidNode,BluetoothNode,TimeNode,NotifyNode);

    tree_node_binding_oneTime(5, rootNode,PhoneNode,KeyNode,HotsportNode,NoDisturbNode);
    tree_node_binding_oneTime(3, PidNode,P_param,I_param,D_param);

    tree_node_binding_oneTime(3, KeyNode,CorrectNode,
                                oneHandleNode,slideInputNode);

    tree_node_binding_oneTime(1,BluetoothNode,BluetoothNode_1);
}

void ManagerMenu::currentHandleInit()
{
    memset(&menuHandle,0,sizeof(curHandle_Typedef));
    menuHandle.cur_list_head = &UniversalNode->localPos;
    menuHandle.cur_type = UniversalNode->unitType;
    menuHandle.chosse_cnt = get_menu_choose_cnt(&menuHandle);
    menuHandle.need_refresh = 1;

    /**顺带初始化全局的配置*/
    memset(&operat_config,0,sizeof(configSet_Typedef));

}

void ManagerMenu::tree_node_binding_oneTime(u16_t cnt, MenuItem_Typedef *non_leaf,...)
{
    va_list argPtr;
   MenuItem_Typedef *temp;
   va_start(argPtr,non_leaf);
   while(cnt--)
   {
       temp = va_arg(argPtr,MenuItem_Typedef *);
       single_list_add_tail(&temp->brother,&non_leaf->localPos);
       temp->parentPtr = &non_leaf->localPos;
   }
   va_end(argPtr);

}

u8_t ManagerMenu::get_menu_choose_cnt(curHandle_Typedef *handle)
{
    u8_t cnt=0;
    struct single_list_head* temp = handle->cur_list_head->next;
    while(temp){
        cnt++;
        temp = temp->next;
    }
    return cnt;
}

u8_t ManagerMenu::get_uplist_from_curlisthead(curHandle_Typedef *handle)
{
    MenuItem_Typedef *pos;
    struct single_list_head *ptr = handle->cur_list_head;
    pos = list_entry(ptr,MenuItem_Typedef,localPos);

    if(pos->parentPtr == NULL){
        return False;
    }
    handle->cur_list_head = pos->parentPtr;
    handle->chosse_cnt = get_menu_choose_cnt(handle);//注意摆放的位置

    pos = list_entry(handle->cur_list_head,MenuItem_Typedef,localPos);
    handle->cur_choose = pos->selectNum;
    handle->cursorPos = pos->cursorPos;
    handle->startItem = pos->selectNum - pos->cursorPos;

    // printf("返回上一层现在的选择:%d,光标:%d,开始条目:%d\n",handle->cur_choose,handle->cursorPos,handle->startItem);
    return True;
}

void ManagerMenu::select_verify_deal(curHandle_Typedef *handle)
{
    MenuItem_Typedef *pos;
    u8_t cnt = 0;
    struct single_list_head *ptr = handle->cur_list_head;

    single_list_for_each_entry(pos,ptr,brother)
    {
        if(__get_node_type(pos->unitType) != CLOSE_LEAF_SIGN){
                return;//
        }
        if(cnt == handle->cur_choose){

            pos->updataConfig(pos);//改变配置参数

            if(pos->unitType&MULTI_LEAF_ASSERT)//如果支持多选,可以直接跳出去了
                break;
        }else{
            if(!(pos->unitType&MULTI_LEAF_ASSERT)){//不支持多选
                printf("%s\n",pos->briefInfo);
                pos->cur_icon = pos->icon->off_icon;
            }
        }
        cnt++;
    }
    handle->need_refresh = 1;
}

void ManagerMenu::enterExit_to_newPage(curHandle_Typedef *handle, u8_t mode)
{
    MenuItem_Typedef *pos,*save;
    u8_t cnt = 0;
    struct single_list_head *ptr = handle->cur_list_head;
    if(mode == ENTER_PAGE){

        save = list_entry(ptr,MenuItem_Typedef,localPos);
//        printf("%d\n",save->unitType);
        if(__get_node_type(save->unitType) == CLOSE_LEAF_SIGN){
//            printf("false non leaf\n");
            return;
        }
        if(ptr->next){//非空进入下一个页面
            single_list_for_each_entry(pos,ptr,brother)
            {
                if(cnt == handle->cur_choose && __get_node_type(pos->unitType) != CLOSE_LEAF_SIGN){

                    save->cursorPos = handle->cursorPos;//提前保存一下
                    save->selectNum = handle->cur_choose;

                    handle->cur_choose = 0;//
                    handle->cursorPos = 0;
                    handle->startItem = 0;
                    handle->cur_list_head = &pos->localPos;//重新初始list

                    handle->chosse_cnt = get_menu_choose_cnt(handle);//注意摆放的位置
                    break;
                }
                cnt++;
            }
            handle->need_refresh = 1;
        }
    }else{//返回上一级
        if(get_uplist_from_curlisthead(handle))
            handle->need_refresh = 1;
    }
}

MenuItem_Typedef *ManagerMenu::branchCreate(NODE_TYPE nodeType, const char *text, show_dir_page cb,const char* logo)
{
    MenuItem_Typedef* non_leaf = (MenuItem_Typedef*)malloc(sizeof(MenuItem_Typedef));
    if(non_leaf == NULL){
        return NULL;
    }
    memset(non_leaf,0,sizeof(MenuItem_Typedef));
    non_leaf->briefInfo = text;
    non_leaf->unitType = nodeType;
    non_leaf->showMenu = cb;
    non_leaf->logo = logo;

    return non_leaf;

}

MenuItem_Typedef *ManagerMenu::leafCreate(NODE_TYPE nodeType, const char *text, show_leaf_page cb, iconInfo_Typedef *argIcon, const char* logo)
{
    MenuItem_Typedef* leaf = (MenuItem_Typedef*)malloc(sizeof(MenuItem_Typedef));
    if(leaf == NULL){
        return NULL;
    }
    memset(leaf,0,sizeof(MenuItem_Typedef));
    leaf->icon = argIcon;
    leaf->unitType = nodeType;
    leaf->briefInfo = text;

    leaf->endPageDeal = cb;

    leaf->logo = logo;

    if(argIcon){
        if(nodeType&LEAF_INIT_STATE){
            leaf->cur_icon = argIcon->on_icon;
        }else{
            leaf->cur_icon = argIcon->off_icon;
        }

    }

    return leaf;

}

MenuItem_Typedef *ManagerMenu::SpecialleafCreate(NODE_TYPE nodeType, const char *text, modify_config cb, iconInfo_Typedef *argIcon)
{
    MenuItem_Typedef* leaf = (MenuItem_Typedef*)malloc(sizeof(MenuItem_Typedef));
    if(leaf == NULL){
        return NULL;
    }
    memset(leaf,0,sizeof(MenuItem_Typedef));
    leaf->icon = argIcon;
    leaf->unitType = nodeType;
    leaf->briefInfo = text;

    leaf->updataConfig = cb;

    if(argIcon){
        if(nodeType&LEAF_INIT_STATE){
            leaf->cur_icon = argIcon->on_icon;
        }else{
            leaf->cur_icon = argIcon->off_icon;
        }

    }

    return leaf;

}

void ManagerMenu::free_branch_auto(MenuItem_Typedef *non_lef)
{
    struct single_list_head *ptr = &non_lef->localPos;
    MenuItem_Typedef *temp;

    single_list_for_each_entry(temp,ptr,brother){
        if(__get_node_type(temp->unitType) == OPEN_LEAF_SIGN){
            free(temp);
        }else{
            free_branch_auto(temp);
        }
    }
    free(non_lef);
}

void ManagerMenu::chooseCursorUp(curHandle_Typedef *handle)
{
    if(handle->cur_choose>0){
        handle->cur_choose--;
    }

    handle->cursorPos--;
    if(handle->chosse_cnt <= PAGE_NUMS){
        handle->show_cnt = handle->chosse_cnt;
        if(handle->cursorPos < 0){
            handle->cursorPos = 0;//diff
        }
    }else{
        handle->show_cnt = PAGE_NUMS;
        if(handle->cursorPos < 0){
            handle->cursorPos = 0;
            if(handle->startItem > 0 ){
                handle->startItem--;
            }
        }
    }
}

void ManagerMenu::chooseCursorDown(curHandle_Typedef *handle)
{
    if(handle->cur_choose < handle->chosse_cnt-1){
        handle->cur_choose++;
    }

    handle->cursorPos++;
    if(handle->chosse_cnt <= PAGE_NUMS){
        handle->show_cnt = handle->chosse_cnt;
        if(handle->cursorPos >= handle->show_cnt){
            handle->cursorPos = handle->show_cnt-1;//diff
        }
    }else{
        handle->show_cnt = PAGE_NUMS;
        if(handle->cursorPos >= PAGE_NUMS){
            handle->cursorPos = PAGE_NUMS-1;
            if(handle->startItem < handle->chosse_cnt - PAGE_NUMS){
                handle->startItem++;
            }
        }
    }
}

void ManagerMenu::bindParamInit(MenuItem_Typedef *node, void *bindParam)
{
    node->param = bindParam;
}

void ManagerMenu::configSetInit(configSet_Typedef *cfg)
{
    cfg->p_pid = 15;
    cfg->i_pid = 10;
    cfg->d_pid = 30;
}

void ManagerMenu::updata_Binding_param(curHandle_Typedef *handle, u8_t rise)
{
    MenuItem_Typedef *pos;
    u8_t cnt = 0;
    struct single_list_head *ptr = handle->cur_list_head;
    single_list_for_each_entry(pos,ptr,brother)
    {

        if(cnt == handle->cur_choose){
            if(rise){
                (*(int *)pos->param)++;//参数应该转成什么类型用户是清楚的
            }else{
                if((*(int *)pos->param) > 0){
                    (*(int *)pos->param)--;
                }
            }
            break;
        }
        cnt++;
    }

    handle->need_refresh = 1;
}

void ManagerMenu::simulate_show_list_page(const MenuItem_Typedef *menu, QPainter &painter)
{
    const struct single_list_head *list_node = &menu->localPos;
    MenuItem_Typedef *temp;
    u8_t cnt = 0;
    u8_t labelNum = 0;
    /*画标题*/
    painter.save();
    painter.drawText(QRectF(0,3,WIN_WIDTH,TITLE_Y), Qt::AlignCenter, menu->briefInfo);
    painter.drawLine(0,35,WIN_WIDTH,35);




    //上下 各留9pixel
    /*画滚动条*/
    int start_x = WIN_WIDTH-SCROL_WIDTH;
    int start_y = TITLE_Y+9;
    painter.drawRect(start_x,start_y,SCROL_WIDTH,SCROL_HEIGHT-9);

    //画上下三角，20*20
    painter.drawPixmap(start_x+3,start_y+2,20,20,QPixmap(LOGO_ARROW_UP));
    painter.drawPixmap(start_x+3,TITLE_Y+SCROL_HEIGHT-22,20,20,QPixmap(LOGO_ARROW_DOWN));

    //画按钮边框
    painter.drawLine(start_x,start_y+24,start_x+SCROL_WIDTH,start_y+24);//
    painter.drawLine(start_x,TITLE_Y+SCROL_HEIGHT-24,start_x+SCROL_WIDTH,TITLE_Y+SCROL_HEIGHT-24);

    //减去标题的高度是
    int heightReal = SCROL_HEIGHT-18;//9pixel
    int barReal = heightReal - 48;//减去三角和框27

    int barLen;
    int yOffset;
    QColor color("#00fffc");

    if(PAGE_NUMS > menuHandle.chosse_cnt){
        barLen = barReal;
    }else{
        barLen = barReal*PAGE_NUMS/menuHandle.chosse_cnt;
    }

    int freeSpace = barReal - barLen;

    if(PAGE_NUMS > menuHandle.chosse_cnt){
        yOffset = 0;
    }else{
        yOffset = freeSpace*menuHandle.startItem/(menuHandle.chosse_cnt - PAGE_NUMS);
    }
//    qDebug()<<"barreal:"<<barReal<<"barLen:"<<barLen<<"yOffset:"<<yOffset<<"chosse_cnt"<<menuHandle.chosse_cnt<<"freespace:"<<freeSpace;
//    qDebug()<<"startItem:"<<menuHandle.startItem;

    painter.setBrush(color);
    painter.drawRect(start_x+4,start_y+30+yOffset,SCROL_WIDTH-8,barLen);


    //结束画滚动条


    painter.restore();
    /*画内容*/
    single_list_for_each_entry(temp,list_node,brother)
    {
        if(cnt >= menuHandle.startItem){
            if(labelNum == menuHandle.cursorPos){
                painter.drawPixmap(50,TEXT_START_Y+FONT_GAP*labelNum,30,30,QPixmap(":/image/select.png"));
            }
            painter.drawText(QRectF(0,TEXT_START_Y+FONT_GAP*labelNum,WIN_WIDTH,FONT_GAP), Qt::AlignCenter, temp->briefInfo);
            labelNum++;
            if(labelNum >= PAGE_NUMS){
                break;
            }
        }
        cnt++;
    }

}

void ManagerMenu::simulate_main_logo_page(const MenuItem_Typedef *menu, QPainter &painter)
{
    const struct single_list_head *list_node = &menu->localPos;
    MenuItem_Typedef *temp;
    u8_t labelNum = 0;
    /*画内容*/
    single_list_for_each_entry(temp,list_node,brother)
    {
        if(labelNum == menuHandle.cursorPos){
            /*显示对应的logo 和 文字 , 左右小箭头*/
            painter.drawPixmap((WIN_WIDTH-100)*0.5,(WIN_HEIGHT-100)*0.5,100,100,QPixmap(temp->logo));
            painter.drawText(QRectF((WIN_WIDTH-100)*0.5,(WIN_HEIGHT-100)*0.5+70,100,100), Qt::AlignCenter, temp->briefInfo);
            break;
        }

        labelNum++;
    }

}

void ManagerMenu::simulate_edit_param_task(const MenuItem_Typedef *menu, QPainter &painter)
{
    const struct single_list_head *list_node = &menu->localPos;
    MenuItem_Typedef *temp;
    u8_t cnt = 0;

    /*画标题*/
    painter.save();

    if(menuHandle.edit_mode){
        painter.drawText(QRectF(0,3,WIN_WIDTH,TITLE_Y), Qt::AlignCenter, QString(menu->briefInfo)+"(编辑模式)");
    }else{
        painter.drawText(QRectF(0,3,WIN_WIDTH,TITLE_Y), Qt::AlignCenter, menu->briefInfo);
    }

    painter.drawLine(0,35,WIN_WIDTH,35);


    single_list_for_each_entry(temp,list_node,brother)
    {
        if(cnt == menuHandle.cursorPos){
            painter.drawPixmap(50,TEXT_START_Y+FONT_GAP*cnt,30,30,QPixmap(":/image/select.png"));
        }
        painter.drawText(QRectF(0,TEXT_START_Y+FONT_GAP*cnt,WIN_WIDTH,FONT_GAP), Qt::AlignCenter, QString(temp->briefInfo)+QString("        %1").arg((*(int *)temp->param)));

        cnt++;
    }

}

void ManagerMenu::simulate_show_option_icon(const MenuItem_Typedef *menu, QPainter &painter)
{
    const struct single_list_head *list_node = &menu->localPos;
    MenuItem_Typedef *temp;
    u8_t cnt = 0;

    painter.save();
    painter.drawText(QRectF(0,5,WIN_WIDTH,TITLE_Y), Qt::AlignCenter, menu->briefInfo);
    painter.drawLine(0,35,WIN_WIDTH,35);
    painter.restore();
    single_list_for_each_entry(temp,list_node,brother)
    {
        if(cnt == menuHandle.cur_choose){
            painter.drawPixmap(50,TEXT_START_Y+FONT_GAP*cnt,30,30,QPixmap(":/image/select.png"));
            painter.drawPixmap(330,5+TEXT_START_Y+FONT_GAP*cnt,30,30,QPixmap(temp->cur_icon));
        }
        else{
            painter.drawPixmap(330,5+TEXT_START_Y+FONT_GAP*cnt,30,30,QPixmap(temp->cur_icon));
        }
        painter.drawText(QRectF(0,TEXT_START_Y+FONT_GAP*cnt,WIN_WIDTH,FONT_GAP), Qt::AlignCenter, temp->briefInfo);
        cnt++;
    }

}

void ManagerMenu::blueTooth_page_deal(MenuItem_Typedef *leaf)
{

    operat_config.bt_state = ~operat_config.bt_state;
    if(operat_config.bt_state){
        leaf->cur_icon = leaf->icon->on_icon;
    }else{
        leaf->cur_icon = leaf->icon->off_icon;
    }
}

void ManagerMenu::test_turn_page(MenuItem_Typedef *leaf, QPainter &painter)
{
    /*画标题*/
    painter.save();
    painter.drawText(QRectF(0,5,WIN_WIDTH,TITLE_Y), Qt::AlignCenter, leaf->briefInfo);
    painter.drawLine(0,35,WIN_WIDTH,35);
    painter.restore();

    painter.drawText(QRectF(0,TEXT_START_Y,WIN_WIDTH,FONT_GAP), Qt::AlignCenter, QString("[翻页测试:     .... ]"));

}

void ManagerMenu::show_dynamic_time_page(MenuItem_Typedef *leaf, QPainter &painter)
{
    painter.save();
    painter.drawText(QRectF(0,5,WIN_WIDTH,TITLE_Y), Qt::AlignCenter, leaf->briefInfo);
    painter.drawLine(0,35,WIN_WIDTH,35);

    QTime current = QTime::currentTime();
    QFont font = painter.font();

    font.setPixelSize(30);
    painter.setFont(font);
    painter.drawText(QRectF(0,2*TEXT_START_Y,WIN_WIDTH,FONT_GAP), Qt::AlignCenter, QString("[ %1:%2 ]").arg(current.hour()).arg(current.minute()));

    painter.restore();

}

void ManagerMenu::battery_ring_page(MenuItem_Typedef *leaf, QPainter &painter)
{
    /*显示电量圆环*/
    QTime current = QTime::currentTime();
    int radius = 50;//外圆半径

    int padsize = 35;//内圆半径

    int startAngle = 90;

    operat_config.battery = current.minute();//为了测试使用
    painter.save();


    QRectF rect(RING_POS_X, RING_POS_Y, radius * 2, radius * 2);

    double angleAll = 360.0;
    double angleCurrent = angleAll * operat_config.battery / 100;
    double angleOther = angleAll - angleCurrent;

    QColor color("#00fffc");
    QColor bgcolor("#0b0b0b");


    painter.setBrush(color);
    painter.drawPie(rect, (startAngle - angleCurrent) * 16, angleCurrent * 16);


    painter.setBrush(bgcolor);
    painter.drawPie(rect, (startAngle - angleCurrent - angleOther) * 16, angleOther * 16);

    painter.drawEllipse(QRectF(RING_POS_X+radius-padsize,RING_POS_Y+radius-padsize,padsize*2,padsize*2));







    QFont font;
    font.setPixelSize(25);
    painter.setFont(font);

    QRectF textRect(RING_POS_X+radius-padsize,RING_POS_Y+radius-padsize,padsize*2,padsize*2);
    QString strValue = QString("%1%").arg(operat_config.battery);

    painter.drawText(textRect, Qt::AlignCenter, strValue);
    painter.restore();

    qDebug()<<"ring display";

}



