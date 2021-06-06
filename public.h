
/**
 * 队列和栈部分公用内容
 * 这一部分是linux内核里一种高级用法
*/




#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"


typedef   unsigned char     u8_t;
typedef   unsigned short    u16_t;
typedef   unsigned int      u32_t;
typedef   signed char       s8_t;
typedef   signed short      s16_t;
typedef   signed int        s32_t;



#define CONTAINER_OF(ptr,type,member) ({\
(type*)((char*)ptr-(size_t)(&((type*)0)->member));\
})


#define list_entry(ptr, type, member)   \
    CONTAINER_OF(ptr, type, member)

//加括号是因为传进来的参数是一个取地址，防止出错
//双向链表使用
#define list_for_each_entry(pos, head, member)      \
    for(pos = list_entry((head)->next, typeof(*pos), member); \
        &pos->member != (head);                                   \
        pos = list_entry(pos->member.next, typeof(*pos), member))

//单链表遍历使用
#define single_list_for_each_entry(pos, head, member)      \
    for(pos = list_entry((head)->next, typeof(*pos), member); \
        &pos->member != (NULL);                                   \
        pos = list_entry(pos->member.next, typeof(*pos), member))

#ifdef __cplusplus
}
#endif

#endif
