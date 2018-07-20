
#ifndef _DELAY
#define _DELAY
  #include "DataStruct.h"

#endif
/***************************************************************************
���ж��ݵ���ʱ,ʹ���߿��Ը���ʱ��Ƶ�����е�����ʱ����
***************************************************************************/
void delay(unsigned long unDelayNum)
{
    unsigned long i;
    for(i = 0;i < unDelayNum;i++)
    {
       _NOP();
    }
}
//������ʱ
void delayms(unsigned int unMSNum)
{
    unsigned int iCtOld = 0;
    unsigned int iCtCur = 0;
    unsigned int iCtCount = 0;
    unsigned int unMs = unMSNum;
    iCtOld = g_sRtcManager.m_gRealTimer[RTC_MICROSEC];
    for(;;)
    {
        iCtCur = g_sRtcManager.m_gRealTimer[RTC_MICROSEC];
        if(iCtCur < iCtOld)
          iCtCount = 1000 + iCtCur - iCtOld ;
        else
          iCtCount = iCtCur - iCtOld;
        if(iCtCount >= unMs)
          break;
        else
        {
          unMs -= iCtCount;
          iCtOld = iCtCur;
        }
          
       WatchDog();
    }
}

void delay_1ms(INT16U dat)
{
    INT16U i,j;
    
    for(i = 0; i < dat; i++)
    {
     for(j = 0; j < 20; j++)       //2041  ���8M�ľ����������ʱ1����
       {_NOP();}                 
      WatchDog();
    }
}