#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY PCin(14)

void KEY_Init(void);          //������ʼ��
int KEY_Scan(void);          //����ɨ��

#endif  
