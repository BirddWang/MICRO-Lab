#ifndef _CCP1_H
#define	_CCP1_H

#include <xc.h> // include processor files - each processor file is guarded.  

void CCP1_Compare_Initialize();
void CCP1_PWM_Initialize();
void CCP1_PWM_Set(unsigned int duty);
#endif	

