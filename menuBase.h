#ifndef __MENUBASE_H__
#define __MENUBASE_H__


#include "string.h"
#include "stdarg.h"
#include "stdio.h"




#include "Hi_single_list.h"
#include "QPainter"



#define True    1
#define False   0

#define PAGE_NUMS   4   //一页最多容纳 3 行

typedef struct MenuItem MenuItem_Typedef;




typedef void (*show_dir_page)(const MenuItem_Typedef *menu , QPainter &painter);

typedef void (*show_leaf_page)( MenuItem_Typedef *leaf , QPainter &painter);

typedef void (*modify_config)( MenuItem_Typedef *leaf);

/**
 * TODO 
 * 简化处理函数的类型
 * 
*/
typedef struct iconInfo
{
    const char *on_icon;
    const char *off_icon;
}iconInfo_Typedef;
typedef struct MenuItem
{
    u16_t unitType;          //该菜单节点的信息

    s8_t selectNum;         //选中的条目序号
    s8_t cursorPos;         //光标位置
    const char *briefInfo;  //子菜单标题信息
    const char *logo;
    const iconInfo_Typedef *icon;       //子菜单的图标信息
    const char *cur_icon;
    struct single_list_head  localPos;  //绑定子目录的头节点
    struct single_list_head  brother;
    struct single_list_head *parentPtr;
    union{
        show_dir_page   showMenu;
        show_leaf_page  endPageDeal;    //包括 显示静态/动态页面和全局配置修改后调用上一次刷新配置页面
        modify_config   updataConfig;
    };
    void *param;//外部地址修改参数
}MenuItem_Typedef;



#define LEAF_TYPE_BIT   (14)
#define LEAF_MUTLI_BIT  (13)
#define LEAF_STATE_BIT  (12)


#define LEAF_INIT_STATE     (1 << LEAF_STATE_BIT)
#define MULTI_LEAF_ASSERT   (1 << LEAF_MUTLI_BIT)
#define CAN_ENTER_ASSERT    (3 << LEAF_TYPE_BIT)

#define LEAF_DYN_ASSERT     (1)


/**
 * 位值代表
 * 0叶子/1非叶子  1展开/0不能展开    1多选/0单选  默认状态1开/0关
*/

enum UNIT_BIT_STATUS {
    UNIT_NONE = 0x0,
    UNIT_NON_LEAF = 0x8000,
    UNIT_UNFOLD = 0x4000,
    UNIT_MULTI_EN = 0x2000,
    UNIT_SWITCH_ON = 0x1000,
    UNIT_EDIT_EN = 0x0800,
    UNIT_REFRESH_EN = 0x0400,
};

typedef enum {
    NON_LEAF = UNIT_NON_LEAF,//非叶子节点

    NON_LEAF_EDIT = UNIT_NON_LEAF|UNIT_EDIT_EN,//非叶子节点可编辑页面

    LEAF_OPEN_STATIC = UNIT_UNFOLD ,//可以展开的叶子节点,静态显示

    LEAF_OPEN_REFRESH = UNIT_UNFOLD|UNIT_REFRESH_EN,//需要动态刷新的可以展开的叶子节点

    LEAF_OPEN_EDIT_REFRESH = UNIT_UNFOLD|UNIT_EDIT_EN|UNIT_REFRESH_EN,//可以进行编辑可以展开需要实时刷新的叶子节点

    LEAF_CLOSE_MULTI_OFF = UNIT_MULTI_EN,//不能展开的叶子节点, 支持多选, 默认状态是关

    LEAF_CLOSE_MULTI_ON = UNIT_MULTI_EN|UNIT_SWITCH_ON,//不能展开的叶子节点, 支持多选, 默认状态是开

    LEAF_CLOSE_OFF = UNIT_NONE,//不能展开的叶子节点, 不支持多选, 默认状态是关

    LEAF_CLOSE_ON   = UNIT_SWITCH_ON,//不能展开的叶子节点, 不支持多选, 默认状态是开

    LEAF_CLOSE_EDIT = UNIT_EDIT_EN|UNIT_REFRESH_EN,
}NODE_TYPE;

enum{
    CLOSE_LEAF_SIGN = 0,//也即原来的叶子节点,如果是这种节点,不要进入下一页
    OPEN_LEAF_SIGN = 1,
    NON_LEAF_SIGN = 2,
};

enum{
    ENTER_PAGE,
    RETURN_PAGE,
};

typedef struct curHandle
{
    u16_t cur_type;
    u8_t chosse_cnt;
    u8_t show_cnt;//显示的目数
    u8_t need_refresh;//是否需要刷屏

    s8_t cur_choose;
    s8_t startItem;//顶叶序号
    s8_t cursorPos;//光标位置

    u8_t edit_mode;
    
    struct single_list_head *cur_list_head;//指向菜单的头节点
}curHandle_Typedef;



static inline u8_t __get_node_type(u16_t multi)
{
    return (multi>>LEAF_TYPE_BIT)&3;
};





#endif
