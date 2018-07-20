#ifndef _DRIVER_AD

#define _DRIVER_AD
  #include "..\DataStruct.h"
#endif

//==============================================================================
//  ��������   : InitAdc
//  ��������   : CPU�������г�ʼ�������������������衢������ģ��ĳ�ʼ������ʼ�������������жϣ������ѭ��
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void InitAdc(void)
{
    REFCTL0 &= ~REFMSTR;                               //�ڲ��ο���ѹ��Ч
    ADC12CTL0  &= ~(ADC12ENC);                         //����ENCΪ0���Ӷ������޸�ADC12�Ĵ�����ֵ   
    ADC12MCTL0 = (ADC12INCH_0 + ADC12SREF_1);
    ADC12CTL0  = (ADC12REFON  + ADC12REF2_5V + ADC12SHT02);
    ADC12CTL1  |= (ADC12SHP + ADC12CONSEQ0 + ADC12SSEL1 + ADC12CSTARTADD_0); 
}
void EnAd0Sample(void)
{
    ADC12CTL0 |= (ADC12ON + ADC12REFON);
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= (ADC12SC);                              //��ADʹ��AD����
}
void DisAd0Sample(void)
{
    ADC12CTL0 &= ~ADC12ENC;                              //REFON ADC12ON��ENC��λ���޸�
    ADC12CTL0 &= (~( ADC12ON + ADC12SC + ADC12REFON));   //�ر�ADģ���ֹ����\
    
}

//********************************************************
//      Date:
//     Mender  : KYO
//     Function: �򿪵��̫���ܵ��ݲ���
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

    ADC12CTL0 &= ~ADC12ENC;                   //��ʼ�޸�ADC�Ĵ���

    ADC12CTL0  = (ADC12REFON  + ADC12REF2_5V + ADC12SHT0_2 + ADC12ON + ADC12MSC );     ////2.5V��׼�򿪣���ADC12��
    //ADC12CTL0  = (ADC12SHT0_2 + ADC12ON + ADC12MSC );     ////2.5V��׼�򿪣���ADC12�� 
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
//     Function: �رյ��̫���ܵ��ݲ���
//
//********************************************************
void Close_BatSunC_Para()
{
 CLOSE_SUN_SAMPLE;
 CLOSE_BAT_SAMPLE;
 CLOSE_C_SAMPLE;

 ADC12CTL0 &= ~ADC12ENC;                   //�ر�ADC12
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
            sSampleCount = 0;//����زɼ�ʹ�ܣ�����ͨ����AD�ɼ���������0
				g_OpenBatFlag = TRUE;//�Ѿ��ɼ�﮵�ص�ѹ
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
    case 6: //0ͨ��
        break;
    case 8: //1ͨ��
        break;
    case 10://2ͨ��
        break;
    default:
        break;
    }
}


