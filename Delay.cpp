
#ifndef _DELAY
#define _DELAY
  #include "DataStruct.h"

#endif
/***************************************************************************
进行短暂的延时,使用者可以根据时钟频率自行调节延时长短
***************************************************************************/
void delay(unsigned long unDelayNum)
{
    unsigned long i;
    for(i = 0;i < unDelayNum;i++)
    {
       _NOP();
    }
}
//毫秒延时
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
     for(j = 0; j < 20; j++)       //2041  针对8M的晶振，这个是延时1毫秒
       {_NOP();}                 
      WatchDog();
    }
}