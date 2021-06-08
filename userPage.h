#ifndef __USERPAGE_H__
#define __USERPAGE_H__

#include "time.h"
#include "menuBase.h"
#include "QPainter"

//#ifdef __cplusplus
//extern "C" {
//#endif


typedef struct configSet
{
    u8_t bt_state;//蓝牙开关状态
    u8_t correct_state;//自动改正开关
    u8_t oneHandle_state;//单手状态
    u8_t glid_state;//是否允许滑动
    int  battery;//显示电量，用分钟数代替测试

    /*测试调参*/
    int p_pid;
    int i_pid;
    int d_pid;
}configSet_Typedef;






//#ifdef __cplusplus
//}
//#endif

#endif
