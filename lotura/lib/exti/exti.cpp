#include "exti.h"

void IRAM_ATTR flow1() // CH1 유량센서 인터럽트
{
  flowFrequency1++;
}

void IRAM_ATTR flow2() // CH2 유량센서 인터럽트
{
  flowFrequency2++;
}