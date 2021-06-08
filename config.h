#ifndef CONFIG_H
#define CONFIG_H

/**窗口大小*/
#define WIN_WIDTH   384
#define WIN_HEIGHT  189


#define SCROL_WIDTH  26
#define SCROL_HEIGHT 159

/**刷新周期*/
#define FRESH_INTERVAL  20

/**标题*/
#define WIN_TITLE   "菜单模拟"
#define TITLE_Y     30
/**显示菜单项开始的x位置**/
#define TEXT_START_X    20

#define TEXT_START_Y    45
/**显示字体的大小**/
#define LIST_FONT_SIZE  20
/**每一行之间的间隔**/
#define FONT_GAP    35

#define SELECT_WIDTH    30

#define OLED_BCKCOLOR   #01000e
#define OLED_FONTCOLOR  #99ffff

#define LOGO_SET      ":/image/setting.png"
#define LOGO_BLE      ":/image/bluetooth.png"
#define LOGO_TIM      ":/image/timer.png"
#define LOGO_BAT      ":/image/battery.png"

#define LOGO_ARROW_UP   ":/image/arrow_up.png"
#define LOGO_ARROW_DOWN ":/image/arrow_down.png"

#define LOGO_PID        ":/image/pid.png"

#define RING_POS_X      40
#define RING_POS_Y      50


#endif // CONFIG_H
