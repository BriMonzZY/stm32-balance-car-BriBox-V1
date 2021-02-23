#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY PCin(14)

void KEY_Init(void);          //按键初始化
int KEY_Scan(void);          //按键扫描

#endif  
