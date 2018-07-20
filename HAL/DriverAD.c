#ifndef _DRIVER_AD

#define _DRIVER_AD
  #include "..\DataStruct.h"
#endif

//==============================================================================
//  函数名称   : InitAdc
//  功能描述   : CPU开机所有初始化工作，包括各个外设、参数、模块的初始化，初始化结束后启动中断，进入大循环
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void InitAdc(void)
{
    REFCTL0 &= ~REFMSTR;                               //内部参考电压有效
    ADC12CTL0  &= ~(ADC12ENC);                         //设置ENC为0，从而可以修改ADC12寄存器的值   
    ADC12MCTL0 = (ADC12INCH_0 + ADC12SREF_1);
    ADC12CTL0  = (ADC12REFON  + ADC12REF2_5V + ADC12SHT02);
    ADC12CTL1  |= (ADC12SHP + ADC12CONSEQ0 + ADC12SSEL1 + ADC12CSTARTADD_0); 
}
void EnAd0Sample(void)
{
    ADC12CTL0 |= (ADC12ON + ADC12REFON);
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= (ADC12SC);                              //打开AD使能AD采样
}
void DisAd0Sample(void)
{
    ADC12CTL0 &= ~ADC12ENC;                              //REFON ADC12ON在ENC复位后修改
    ADC12CTL0 &= (~( ADC12ON + ADC12SC + ADC12REFON));   //关闭AD模块禁止采样\
    
}

//********************************************************
//      Date:
//     Mender  : KYO
//     Function: 打开电池太阳能电容采样
//
//********************************************************
void Open_BatSunC_Para(unsigned int BatNo)
{
	
    P6SEL  |=  BIT1 + BIT2 + BIT3;   //ReadCapVolt+ReadSunVolt+ReadBatVolt
    if(BatNo == OPEN_SUNC)
    {
        OPEN_C_SAMPLE;
    	OPEN_SUN_SAMPLE;   
    }
    else if(BatNo == OPEN_BAT)
    {
        OPEN_BAT_SAMPLE;
    }
    else if(BatNo == OPEN_BATSUNC)
    {
        OPEN_C_SAMPLE;
        OPEN_SUN_SAMPLE;
        OPEN_BAT_SAMPLE;
    }

    ADC12CTL0 &= ~ADC12ENC;                   //开始修改ADC寄存器

    ADC12CTL0  = (ADC12REFON  + ADC12REF2_5V + ADC12SHT0_2 + ADC12ON + ADC12MSC );     ////2.5V基准打开，打开ADC12，
    //ADC12CTL0  = (ADC12SHT0_2 + ADC12ON + ADC12MSC );     ////2.5V基准打开，打开ADC12， 
    ADC12CTL1  = (ADC12SHP + ADC12CONSEQ_3  + ADC12CSTARTADD_1);                       // Use sampling timer, repeat sequence
   
    ADC12MCTL1 = ADC12INCH_1 + ADC12SREF_1;  // ref+=AVcc, channel = A1,CapVolt
    ADC12MCTL2 = ADC12INCH_2 + ADC12SREF_1;  // ref+=AVcc, channel = A2,SunVolt
    ADC12MCTL3 = ADC12INCH_3 + ADC12SREF_1;  // ref+=AVcc, channel = A3, BatVolt
    ADC12MCTL4 = (ADC12INCH_4+ADC12EOS);//channel=A4 BatFVolt, and end
    
    
    ADC12IE   |= BIT3;                        // Enable ADC12IFG.3
    ADC12CTL0 |= ADC12ENC;
}

//********************************************************
//      Date:
//     Mender  : KYO
//     Function: 关闭电池太阳能电容采样
//
//********************************************************
void Close_BatSunC_Para()
{
 CLOSE_SUN_SAMPLE;
 CLOSE_BAT_SAMPLE;
 CLOSE_C_SAMPLE;

 ADC12CTL0 &= ~ADC12ENC;                   //关闭ADC12
 ADC12CTL0 &= ~ADC12REFON; 
 ADC12CTL0 &= ~ADC12ON; 
}
//**********************************************************
//  date     : 2012-07-05
//  mender   : KYO
//  function : sampling three channel voltage
//  State    : CapVolt--ADC12MCTL1
//             SunVolt--ADC12MCTL2
//             BatVolt--ADC12MCTL3
//           : 2012-07-09   add mulitle select
//**********************************************************
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    INT16U VectorId_AD;
    static unsigned int sSampleCount = 0;
    VectorId_AD = ADC12IV;
    switch(VectorId_AD)
    {
    case 12: 
        if(g_OpenBatFlag == ON)
        {
            sSampleCount = 0;//若电池采集使能，所有通道的AD采集次数都清0
				g_OpenBatFlag = TRUE;//已经采集锂电池电压
        }
        g_sSampleData[CHAN_CAP][sSampleCount]  = ADC12MEM1;
        g_sSampleData[CHAN_SUN][sSampleCount]  = ADC12MEM2;
        g_sSampleData[CHAN_BAT][sSampleCount]  = ADC12MEM3;
        g_sSampleData[CHAN_BATF][sSampleCount] = ADC12MEM4;
        
		sSampleCount++;
        if(sSampleCount == AC_BUFF_LEN)
        {
			sSampleCount  = 0;
            ADC12CTL0 &= ~ADC12ENC;
            g_sSampleFinish = ON;
            LPM3_EXIT; 
        }
          
	break;
    case 6: //0通道
        break;
    case 8: //1通道
        break;
    case 10://2通道
        break;
    default:
        break;
    }
}


