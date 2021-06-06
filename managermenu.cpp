#include "managermenu.h"
#include "config.h"
ManagerMenu::ManagerMenu()
{
    MenuInit();
    currentHandleInit();
}

ManagerMenu::~ManagerMenu()
{
    free_branch_auto(rootNode);
}



curHandle_Typedef ManagerMenu::menuHandle;
/**菜单里的全局配置*/
configSet_Typedef ManagerMenu::operat_config;






void ManagerMenu::MenuInit()
{
    rootNode = branchCreate(NON_LEAF,"设置",simulate_show_list_page);
    UniversalNode = branchCreate(NON_LEAF,"通用",simulate_show_list_page);
    KeyNode = branchCreate(NON_LEAF,"键盘",simulate_show_list_page);

    BluetoothNode = branchCreate(NON_LEAF,"蓝牙",simulate_show_option_icon);//增加蓝牙开关控制节点

    //添加,翻页测试使用
    WifiNode = leafCreate(LEAF_OPEN, "WIFI",test_turn_page,NULL);//静态显示的
    NotifyNode = leafCreate(LEAF_OPEN, "通知",test_turn_page,NULL);//静态显示的
    HotsportNode = leafCreate(LEAF_OPEN, "个人热点",test_turn_page,NULL);//静态显示的
    NoDisturbNode = leafCreate(LEAF_OPEN, "勿扰模式",test_turn_page,NULL);//静态显示的
    /////////////

    PhoneNode = leafCreate(LEAF_OPEN, "关于本机",test_turn_page,NULL);//静态显示的
    TimeNode = leafCreate(LEAF_OPEN,"时间",test_turn_page,NULL);//动态显示

    CorrectNode = branchCreate(NON_LEAF,"自动改正",simulate_show_option_icon);//增加开关控制节点
    oneHandleNode = branchCreate(NON_LEAF,"单手键盘",simulate_show_option_icon);//增加开关控制节点
    slideInputNode = branchCreate(NON_LEAF,"滑行键入",simulate_show_option_icon);

    tree_node_binding_oneTime(7, rootNode,BluetoothNode,WifiNode,UniversalNode,NotifyNode,HotsportNode,NoDisturbNode,TimeNode);

    tree_node_binding_oneTime(2, UniversalNode,PhoneNode,KeyNode);

    tree_node_binding_oneTime(3, KeyNode,CorrectNode,
                                oneHandleNode,slideInputNode);


//    BluetoothNode_1 = leafCreate(LEAF_CLOSE_MULTI_DISEN, "蓝牙",blueTooth_page_deal, &text_onoff);//增加蓝牙开关控制节点
//    CorrectNode_1 = leafCreate(LEAF_CLOSE_MULTI_DISEN, "自动改正",autoCorrct_page_deal, &text_onoff);
//    slideInputNode_1 = leafCreate(LEAF_CLOSE_MULTI_DISEN, "滑行键入",glide_page_deal,&text_onoff);
//    oneHandleNode_1 = leafCreate(LEAF_CLOSE_NOMULTI_EN, "左",oneHandle_page_deal, &sign_onoff);
//    oneHandleNode_2 = leafCreate(LEAF_CLOSE_NOMULTI_DISEN, "中",oneHandle_page_deal, &sign_onoff);
//    oneHandleNode_3 = leafCreate(LEAF_CLOSE_NOMULTI_DISEN, "右",oneHandle_page_deal, &sign_onoff);


}

void ManagerMenu::currentHandleInit()
{
    memset(&menuHandle,0,sizeof(curHandle_Typedef));
    menuHandle.cur_list_head = &rootNode->localPos;
    menuHandle.cur_type = rootNode->unitType;
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

MenuItem_Typedef *ManagerMenu::branchCreate(NODE_TYPE nodeType, const char *text, show_dir_page cb)
{
    MenuItem_Typedef* non_leaf = (MenuItem_Typedef*)malloc(sizeof(MenuItem_Typedef));
    if(non_leaf == NULL){
        return NULL;
    }
    memset(non_leaf,0,sizeof(MenuItem_Typedef));
    non_leaf->briefInfo = text;
    non_leaf->unitType = nodeType;
    non_leaf->showMenu = cb;

    return non_leaf;

}

MenuItem_Typedef *ManagerMenu::leafCreate(NODE_TYPE nodeType, const char *text, show_leaf_page cb, iconInfo_Typedef *argIcon)
{
    MenuItem_Typedef* leaf = (MenuItem_Typedef*)malloc(sizeof(MenuItem_Typedef));
    if(leaf == NULL){
        return NULL;
    }
    memset(leaf,0,sizeof(MenuItem_Typedef));
    leaf->endPageDeal = cb;
    leaf->icon = argIcon;
    leaf->unitType = nodeType;
    leaf->briefInfo = text;

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

void ManagerMenu::simulate_show_list_page(const MenuItem_Typedef *menu, QPainter &painter)
{
    const struct single_list_head *list_node = &menu->localPos;
    MenuItem_Typedef *temp;
    u8_t cnt = 0;
    u8_t labelNum = 0;
    painter.drawText(TEXT_START_X,TITLE_Y,menu->briefInfo);//TEXT_START_Y

    single_list_for_each_entry(temp,list_node,brother)
    {
        if(cnt >= menuHandle.startItem){
            if(labelNum == menuHandle.cursorPos){
                painter.drawText(TEXT_START_X,TEXT_START_Y+FONT_GAP*labelNum,QString("==>%1").arg(temp->briefInfo));
            }else{
                painter.drawText(TEXT_START_X,TEXT_START_Y+FONT_GAP*labelNum,temp->briefInfo);
            }
            labelNum++;
            if(labelNum >= PAGE_NUMS){
                break;
            }
        }
        cnt++;
    }

}

void ManagerMenu::simulate_show_option_icon(const MenuItem_Typedef *menu, QPainter &painter)
{
    const struct single_list_head *list_node = &menu->localPos;
    MenuItem_Typedef *temp;
    u8_t cnt = 0;
    painter.drawText(TEXT_START_X,0,menu->briefInfo);//TEXT_START_Y
    single_list_for_each_entry(temp,list_node,brother)
    {
        if(cnt == menuHandle.cur_choose)
            painter.drawText(TEXT_START_X,TEXT_START_Y+FONT_GAP*cnt,QString("==>:%1           %2").arg(temp->briefInfo).arg(temp->cur_icon));

        else
            painter.drawText(TEXT_START_X,TEXT_START_Y+FONT_GAP*cnt,QString("    %1           %2").arg(temp->briefInfo).arg(temp->cur_icon));

        cnt++;
    }

}

void ManagerMenu::test_turn_page(MenuItem_Typedef *leaf, QPainter &painter)
{
    painter.drawText(TEXT_START_X,0,QString("======%1======").arg(leaf->briefInfo));

    painter.drawText(TEXT_START_X,TEXT_START_Y,QString("[翻页测试:     .... ]"));

}



