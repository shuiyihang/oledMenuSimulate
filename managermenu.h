#ifndef MANAGERMENU_H
#define MANAGERMENU_H

#include "menuBase.h"
#include "userPage.h"
#include "stdarg.h"
#include "progressring.h"
#include "QTime"

class ManagerMenu
{
public:
    ManagerMenu();
    ~ManagerMenu();

    void MenuInit();
    void currentHandleInit();


    /**菜单的核心管理api**/
    void tree_node_binding_oneTime(u16_t cnt, MenuItem_Typedef *non_leaf,...);
    u8_t get_menu_choose_cnt(curHandle_Typedef *handle);
    u8_t get_uplist_from_curlisthead(curHandle_Typedef *handle);
    void select_verify_deal(curHandle_Typedef *handle);
    void enterExit_to_newPage(curHandle_Typedef *handle, u8_t mode);
    MenuItem_Typedef* branchCreate(NODE_TYPE nodeType , const char *text, show_dir_page cb,const char* logo);
    MenuItem_Typedef* leafCreate(NODE_TYPE nodeType, const char *text, show_leaf_page cb , iconInfo_Typedef *argIcon, const char* logo);
    MenuItem_Typedef* SpecialleafCreate(NODE_TYPE nodeType, const char *text, modify_config cb, iconInfo_Typedef *argIcon);
    void free_branch_auto(MenuItem_Typedef* non_lef);
    void currentHandleInit(MenuItem_Typedef * root, curHandle_Typedef *handle);
    void chooseCursorUp(curHandle_Typedef *handle);
    void chooseCursorDown(curHandle_Typedef *handle);




    /**非核心，仅显示使用**/
    static void simulate_show_list_page(const MenuItem_Typedef *menu , QPainter &painter);
    static void simulate_main_logo_page(const MenuItem_Typedef *menu , QPainter &painter);//左右滚动带logo的主界面

    static void simulate_show_option_icon(const MenuItem_Typedef *menu, QPainter &painter);
    static void blueTooth_page_deal( MenuItem_Typedef *leaf);
    static void aboutPhone_page( MenuItem_Typedef *leaf);
    static void test_turn_page(MenuItem_Typedef *leaf , QPainter &painter);
    static void show_dynamic_time_page(MenuItem_Typedef *leaf , QPainter &painter);
    static void autoCorrct_page_deal( MenuItem_Typedef *leaf);
    static void oneHandle_page_deal( MenuItem_Typedef *leaf);
    static void glide_page_deal( MenuItem_Typedef *leaf);

    static void battery_ring_page(MenuItem_Typedef *leaf , QPainter &painter);



    /**管理当前*/
    static curHandle_Typedef menuHandle;
    /**菜单里的全局配置*/
    static configSet_Typedef operat_config;


    /**定义的树节点*/
    MenuItem_Typedef *rootNode;
    MenuItem_Typedef *UniversalNode;
    MenuItem_Typedef *BluetoothNode;
    MenuItem_Typedef *PhoneNode;
    MenuItem_Typedef *TimeNode;
    MenuItem_Typedef *KeyNode;
    MenuItem_Typedef *CorrectNode;
    MenuItem_Typedef *oneHandleNode;
    MenuItem_Typedef *slideInputNode;

    MenuItem_Typedef *BluetoothNode_1, *CorrectNode_1, *slideInputNode_1, *oneHandleNode_1, *oneHandleNode_2, *oneHandleNode_3;

    MenuItem_Typedef *NotifyNode, *HotsportNode, *NoDisturbNode, *WifiNode;


};

#endif // MANAGERMENU_H
