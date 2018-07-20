/*------------------------------------------------------------------------
 Module:      ParaCfg.cpp
 Author:      linxueqin
 Project:     ��վƽ̨
 State:       V1.0
 Creation Date:   2014-4-29
 Description:   ���в�������Լ������ң�ţ�ң�⣬ң�ز����ĳ�ʼ�������漰��λ
------------------------------------------------------------------------*/


#ifndef _PARA_CFG

#define _PARA_CFG
  #include "DataStruct.h"
#endif

//==============================================================================
//  ��������   : InitPara
//  ��������   : ע����õ��Ĳ������п�����ʼ��������EEPROM��ȡ�����粻��ʧ������
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void InitcfgPara(void)
{
    unsigned int i;
    g_pVersion = &(g_gSmsPhone[VERSION]);//�汾�ſ�д
    g_pCompany = &(g_gSmsPhone[COMPNAME]);
    g_pDevId = &(g_gSmsPhone[DEVID+1]);
    g_ucParaChang = 0; //�������CheckCfgPara����ǰ�ߣ�101�������������־  BIT0Ϊ 101������BIT1Ϊң����BIT2Ϊң��ϵ����BIT3Ϊң��ƫ������BIT4Ϊң�ŵ�� BIT5Ϊ���в�����
    
    gRecorder_flag.LIST_flag = OFF;
    for(i = 0; i < RMT_WORD_NUM; i++)
    {
        g_gRmtInfo[i] = 0;
    }
    for(i = 0; i < RMT_MEAS_NUM; i++)
    {
      g_gRmtMeas[i] = 0;//INITYC;
    }
    //g_gRTCtype = 0xA8;
    CheckCfgPara();//��ȡ���в������������������λ��Ĭ��ֵ
    InitTsYxInfo();
    InitTsYcInfo();
    section_current =1;
    #ifdef ODU
    for(BYTE bLine = 0;bLine < 12;bLine++)
    {
      if((g_gRunPara[RP_YXYCMODE] & BIT[bLine]) && (g_gRunPara[RP_YXYCMODE] & BIT[bLine+1]))
      {
        g_LineFlag[bLine]=1;
        g_LineFlag[bLine+1]=2;
        bLine++;
      }
      else
         g_LineFlag[bLine]=0;
    }
    #endif
    g_TimeRF003_flag = OFF;
    g_step_earth =0;
  gRecorder_flagall = OFF;
    g_gRmtLockLB =0;
    g_ucRefYxYcTs = 0;//ң��ң�����ݱ仯��־ BIT0Ϊң�� BIT1Ϊң��
    g_ZsqPara = 0;   //zx
    g_debug_rate = 0; //101
    g_rtc_time =0;
  g_off_gps =0;
    g_GPS_READ_NUM=0;

  
  g_OutputInfo.Interrupt_flag = OFF;
  g_CrystalError.Crystal_start_flag =0;
  g_CrystalError.Crystal_end_flag =OFF;
  g_CrystalError.Compen_cry_total =0;
  g_CrystalError.Compen_cry_num=0;
  g_CrystalError.diff_value_num=0;
  g_RF003_timeInfo.RF003_num =0;
  g_sRtcManager.m_gRealTimerFlag = OFF;
        for(BYTE i =0;i<3;i++)  
  g_RF003_timeInfo.RF003_mini_num[i] =0;//RF003�ķ��Ӽ���
        g_RF003_timeInfo.LUBO_time_flag =OFF;
    g_RF003_timeInfo.RF003_time_flag=OFF;
  memset((unsigned char *)&g_CrystalError,0,sizeof(g_CrystalError));
  //num =0;
  for(i=0;i<3;i++)
  {
    g_phase_loss[i] = OFF;
    gloss_phase_flag[i] = OFF;
    g_detephase_flag= OFF;
  }
        g_RF_pps =0;
        g_RF_Reset =OFF;  
  for(i=0;i<RI_FTINFO_NUM;i++)
    g_gRmtLock10s[i] =10;//���ݽӿ���������ø�λʱ��10SΪ��λ
  #ifdef LOCALYX
  for(i =0;i<EXT_DIN_NUM;i++) 
    g_gDIMcLock10s[i] =10;
  #endif
}
void InitTsYxInfo(void)
{
    int i = 0, j = 0;
    unsigned int RIIndex = 0;
    int sYxCount = 0;
    int dYxCount = 0;
    //����ң�ŵ��������Ҫ�ϴ���ң�Ÿ���
    for(i = 0; i < IEC_YX_NUM; i++)
    {
        if(g_ucYXAddr[i] == 0)
            break;
        if(((g_ucYXAddr[i] - 1) >= DYX_ADDR) &&(g_ucYXAddr[i]!=0xffff)&&(g_ucYXAddr[i]!=0xfffe))
          j++;
    }
    g_ucYxTransNum = i;
    g_ucDYxTransNum = j;
    g_ucSYxTrsStartId = 0xffff;
    g_ucDYxTrsStartId = 0xffff;
    g_open_oducall=0x55;
    for(i = 0; i < g_ucYxTransNum; i++)
    {
      RIIndex = (g_ucYXAddr[i] - 1);
      if(RIIndex < DYX_ADDR || RIIndex ==0xfffe || RIIndex ==0xfffd)
      {//����
        if(g_ucSYxTrsStartId == 0xffff) 
          g_ucSYxTrsStartId = i;
        BYTE bRIVal = 0;
        if(RIIndex < DYX_ADDR)     bRIVal  = GetRIStatus(RIIndex);
        else if(RIIndex ==0xfffe)  bRIVal = 0;
        else if(RIIndex ==0xfffd)  bRIVal = 1;
        SetRIStatus(sYxCount++,bRIVal,1);
      }
      else
      {//˫��
        SetRIStatus(dYxCount++,GetRIStatus(RIIndex),2);
        if(g_ucDYxTrsStartId == 0xffff) 
          g_ucDYxTrsStartId = i;
      }
    }
    if(g_ucSYxTrsStartId == 0xffff)
      g_ucSYxTrsStartId = 0;
    if(g_ucDYxTrsStartId == 0xffff)
      g_ucDYxTrsStartId = 0;
        
}
void InitTsYcInfo(void)
{
    int i = 0;
  unsigned int RIIndex = 0;
    //����ң����������Ҫ�ϴ���ң�����
    for(i = 0; i < IEC_YC_NUM; i++)
    {
        if(g_ucYCAddr[i] == 0)
            break;
    }
    g_ucYcTransNum = i;
    for(i = 0; i < g_ucYcTransNum; i++)   //������Ҫ�ϴ���ң��ֵ�����վ���������л���
    {
      RIIndex = g_ucYCAddr[i];
      if(RIIndex < 255)
          g_unYcTrans[i] = g_gRmtMeas[g_ucYCAddr[i] - 1];//(((unsigned long)g_gRmtMeas[g_ucYCAddr[i] - 1] * g_unYCCoe[i]) >> 10 )+ g_unYCOffset[i];
      else
        g_unYcTrans[i] = 0;
  }
}

//==============================================================================
//  ��������   : SaveCfgPara
//  ��������   : �����й����У�����g_gChangFlag�еĲ������ı�־�ж�ĳ�����ò��������仯�������ò������浽��Ӧ�Ĵ洢���ͱ�������
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void SaveCfgPara(void)  //�����й����У����ĳ�����ò��������仯�������ò������浽��Ӧ�Ĵ洢����
{
    RefreshPara();
 
    //���в���
    if((g_ucParaChang & BIT0) == BIT0)
    {
        g_ucParaChang &= NBIT0;
      
        g_LogType |= BITD;
        g_gRunPara[RP_CRC] = CrcCount((unsigned int *)g_gRunPara, RP_CRC);      //����CRC
        CAT_SpiWriteWords(EEPADD_RP, RUN_PARA_NUM, g_gRunPara); //���浽EEPROM��

        if(g_gRunPara[Terminal_Type] == AERIAL_TERMINAL)
        {
          //����ʵʱģʽ��׼ʵʱģʽ�����GPRS ODU��ģʽ�ڳ����м���
          if(g_gRunPara[RP_POWER_MODE] == QUASI_REALTIME)
            CloseGPRS();
          else if(g_gRunPara[RP_POWER_MODE] == REALTIME)
          {
             OpenGPRS();
             if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
                 g_EnSmsSend = ON;
             g_sTimer[TM_GPRSPWOERDOWN].m_TmCount=0; //���׼ʵʱģʽ��GPRS��ʱ���� 
          }
        }
        
        //һң�豸 �ر�ң���ϴ�
        if(g_gRunPara[RP_DEVICE_TYPE] == 0)  
        {
            g_SendYc = 0;
        }
        SetLbNum();
    if(g_rtc_time == ON)
    {
      g_rtc_time = OFF;
      InitRtc();
      
    }
    if(g_off_gps == ON)//ֻ��GPS�����÷����仯ʱ���Ż����¶�������
    {
      g_off_gps = OFF;
      if(g_gRunPara[RP_CFG_KEYH]& (BIT[RP_OFF_GPS-16]))
      {
        CLOSE_GPS_POWER;
        g_sTimer[TM_GPS_TIME].m_TmCount =0;
        g_sTimer[TM_OPENGPS_TIME].m_TmCount =0;
        g_CrystalError.Compen_cry_total =0;
        g_CrystalError.Crystal_start_flag =0;
        //g_gRunPara[RP_CFG_KEYH] &= ~(BIT[RP_set_TIME-16]);//�ر�GPS��ͬʱ��RTC
      }
      else
      {
        
        //InitGps();
                                OPEN_GPS_POWER;
                          OpenTimer(TM_GPS_TIME);
                          OpenTimer(TM_OPENGPS_TIME);
        g_CrystalError.Compen_cry_total =0;
        g_CrystalError.Crystal_start_flag =0;
        //g_gRunPara[RP_CFG_KEYH] |= (BIT[RP_set_TIME-16]);
        
      }
      
    }
        if(g_debug_rate == ON)
        {
          //CloseUart3();
          UCA3IFG &= ~UCTXIFG;                    //�رշ����ж�  
          g_debug_rate = OFF;
          
          Uart3Init();
          if(g_gRunPara[RP_CFG_KEY]& BIT[RP_DEBUG_RATE])//���ڲ�����ת�ⲿ����ʱ��Ҫ������Ч
              while(1);
               //app();//Ӧ�ó������      
          
        }
        //���¶�ʱ������ֵ
        unsigned int TmId = 0;
        for(TmId = 0;TmId < TIMER_NUM;TmId++)
        {
          if(g_sTimer[TmId].m_TmCount > 0)
          {//��ʱ��������
            RefreshTmCount(TmId);
          }
        } 
    }

    
    //IEC101��������
    if((g_ucParaChang & BIT1) == BIT1)
    {
        g_ucParaChang &= NBIT1;
        g_ucPara101[IECP_CRC] = AddChar(g_ucPara101, IECP_CRC);      //����CS
        CAT_SpiWriteBytes(EEPADD_IECPARA , IEC_PARA_NUM, g_ucPara101);     //���浽EEPROM��
        CAT_SpiWriteBytes(EEPADDBK_IECPARA, IEC_PARA_NUM, g_ucPara101);    //���浽EEPROM�ı�������
    }
    //ң�ŵ�ַ���
    if((g_ucParaChang & BIT2) == BIT2)
    {
        g_ucParaChang &= NBIT2;
        g_ucYXAddr[IEC_YX_NUM] = AddChar((unsigned char *)g_ucYXAddr, IEC_YX_NUM*2);      //����CS
        #define RDYXMUN_MAX 100 //���ζ�ȡe2������
        for(int i = 0;i<50;i++)
        {
          if((IEC_YX_NUM+1 - i*RDYXMUN_MAX) <= RDYXMUN_MAX)
          {
            CAT_SpiWriteWords(EEPADD_IECYXADDR+RDYXMUN_MAX*i*2, IEC_YX_NUM+1 - i*RDYXMUN_MAX, &g_ucYXAddr[RDYXMUN_MAX*i]);
            break;
          }
          else
          {
            CAT_SpiWriteWords(EEPADD_IECYXADDR+RDYXMUN_MAX*i*2, RDYXMUN_MAX, &g_ucYXAddr[RDYXMUN_MAX*i]);
          }
          WatchDog();
        }
        
        //CAT_SpiWriteBytes(EEPADD_IECYXADDR , IEC_YX_NUM*2 + 1, (unsigned char *)g_ucYXAddr);     //���浽EEPROM��
        //CAT_SpiWriteBytes(EEPADDBK_IECYXADDR, IEC_YX_NUM*2 + 1, (unsigned char *)g_ucYXAddr);    //���浽EEPROM�ı�������
        
   }
    //ң�����
    if((g_ucParaChang & BIT3) == BIT3)
    {
        g_ucParaChang &= NBIT3;             
        g_ucYCAddr[IEC_YC_NUM] = AddChar(g_ucYCAddr, IEC_YC_NUM);      //����CS
        CAT_SpiWriteBytes(EEPADD_IECYCADDR , IEC_YC_NUM + 1, g_ucYCAddr);     //���浽EEPROM��
        CAT_SpiWriteBytes(EEPADDBK_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr);    //���浽EEPROM�ı�������
      
    }
    //ң�ص�ַ���
    if((g_ucParaChang & BIT4) == BIT4)
    {
        g_ucParaChang &= NBIT4;
        g_ucYKPa[IEC_YK_NUM] = AddChar(g_ucYKPa, IEC_YK_NUM);      //����CS
        CAT_SpiWriteBytes(EEPADD_IECYKADDR , IEC_YK_NUM + 1, g_ucYKPa);     //���浽EEPROM��
        CAT_SpiWriteBytes(EEPADDBK_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa);    //���浽EEPROM�ı�������
    }
    //�绰�����
    if((g_ucParaChang & BIT5) == BIT5)
    {
        g_ucParaChang &= NBIT5;
        g_gSmsPhone[PHONE_CS] = AddChar(g_gSmsPhone, PHONE_CS);      //����CS
        CAT_SpiWriteBytes(EEPADD_PHONE , PHONE_PA_NUM, g_gSmsPhone);     //���浽EEPROM��
        CAT_SpiWriteBytes(EEPADDBK_PHONE, PHONE_PA_NUM, g_gSmsPhone);    //���浽EEPROM�ı�������
    }

}
//==============================================================================
//  ��������   : CheckCfgPara
//  ��������   : ��EEPROM�ж�ȡ����������У���ж����ݵ���Ч�ԣ����ݴ�����101��������ң�ţ�����101������ΪĬ��ֵ
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void CheckCfgPara(void)
{
    //unsigned char i;
    
    //��EEPROM�ж�ȡ���в������ж���Ч�ԣ���Ч��ֵ��g_gRunPara����Ч���ʼ��g_gRunParaΪĬ��ֵ
    CAT_SpiReadWords(EEPADD_RP, RUN_PARA_NUM, g_gRunPara);  //������ǰ������ֵ
    if (g_gRunPara[RP_CRC] != CrcCount((unsigned int *)g_gRunPara, RP_CRC)) //CRCУ�����
    {
      RstRunPara();
      g_ucParaChang |= BIT0;   //���ñ��溯��
    }
    g_gRunParaFltLkTBk = g_gRunPara[RP_FLTLK_T];
    g_gRunParaZsqStTBk = g_gRunPara[RP_ZSQCST_T];
    g_gRunParaYxRsdTTBk = g_gRunPara[RP_YXRSD_T];
    g_gShortFltLkTBk = g_gRunPara[RP_SHORTFAULT_RESETTIME];
    //101��ϵͳ����
    CAT_SpiReadBytes(EEPADD_IECPARA, IEC_PARA_NUM, g_ucPara101);  //����
    if (g_ucPara101[IECP_CRC] != AddChar(g_ucPara101, IECP_CRC))       //CRCУ��
    {
        CAT_SpiReadBytes(EEPADDBK_IECPARA, IEC_PARA_NUM, g_ucPara101);    //����
        if (g_ucPara101[IECP_CRC] == AddChar(g_ucPara101, IECP_CRC))       //CRCУ��
        {
            CAT_SpiWriteBytes(EEPADD_IECPARA, IEC_PARA_NUM, g_ucPara101); //���������ǹ�ȥ
        }
        else
        {
            RstIEC101Para();  //IEC para initial
            g_ucParaChang |= BIT1;   //���ñ��溯��
        }
    }
    //101��ң�ŵ��
      #define RDYXMUN_MAX 100 //���ζ�ȡe2������
      for(int i = 0;i<50;i++)
      {
        if((IEC_YX_NUM+1 - i*RDYXMUN_MAX) <= RDYXMUN_MAX)
        {
          CAT_SpiReadWords(EEPADD_IECYXADDR+RDYXMUN_MAX*i*2, IEC_YX_NUM+1 - i*RDYXMUN_MAX, &g_ucYXAddr[RDYXMUN_MAX*i]);
          break;
        }
        else
        {
          CAT_SpiReadWords(EEPADD_IECYXADDR+RDYXMUN_MAX*i*2, RDYXMUN_MAX, &g_ucYXAddr[RDYXMUN_MAX*i]);
        }
        WatchDog();
      }
    if (g_ucYXAddr[IEC_YX_NUM] != AddChar((unsigned char *)g_ucYXAddr, IEC_YX_NUM*2))       //CRCУ��
    {
       RstIEC101YxAddr();  //ң�ŵ���ʼ��
       g_ucParaChang |= BIT2;   //���ñ��溯��
    }
    
    //101��ң����
    CAT_SpiReadBytes(EEPADD_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr);  //����
    if (g_ucYCAddr[IEC_YC_NUM] != AddChar(g_ucYCAddr, IEC_YC_NUM))       //CRCУ��
    {
        CAT_SpiReadBytes(EEPADDBK_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr);    //����
        if (g_ucYCAddr[IEC_YC_NUM] == AddChar(g_ucYCAddr, IEC_YC_NUM))       //CRCУ��
        {
            CAT_SpiWriteBytes(EEPADD_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr); //���������ǹ�ȥ
        }
        else
        {
            RstIEC101YcAddr();  //ң�����ʼ��
            g_ucParaChang |= BIT3;   //���ñ��溯��
        }
    }
    //101��ң�ص��
    CAT_SpiReadBytes(EEPADD_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa);  //����
    if (g_ucYKPa[IEC_YK_NUM] != AddChar(g_ucYKPa, IEC_YK_NUM))       //CRCУ��
    {
        CAT_SpiReadBytes(EEPADDBK_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa);    //����
        if (g_ucYKPa[IEC_YK_NUM] == AddChar(g_ucYKPa, IEC_YK_NUM))       //CRCУ��
        {
            CAT_SpiWriteBytes(EEPADD_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa); //���������ǹ�ȥ
        }
        else
        {
            RstIEC101YkAddr();  //ң�ص���ʼ��
            g_ucParaChang |= BIT4;   //���ñ��溯��
        }
    }
    //�绰�����
    CAT_SpiReadBytes(EEPADD_PHONE, PHONE_PA_NUM, g_gSmsPhone);  //����
    if (g_gSmsPhone[PHONE_CS] != AddChar(g_gSmsPhone, PHONE_CS))       //CRCУ��
    {
        CAT_SpiReadBytes(EEPADDBK_PHONE, PHONE_PA_NUM, g_gSmsPhone);    //����
        if (g_gSmsPhone[PHONE_CS] == AddChar(g_gSmsPhone, PHONE_CS))       //CRCУ��
        {
            CAT_SpiWriteBytes(EEPADD_PHONE, PHONE_PA_NUM, g_gSmsPhone); //���������ǹ�ȥ
        }
        else
        {
            RstSmsPhoneInfo();  //�绰������ʼ��
            g_ucParaChang |= BIT5;   //���ñ��溯��
        }
    }
}

//==============================================================================
//  ��������   : RstRunPara
//  ��������   : ��λ�ڴ��е����в���ΪĬ��ֵ
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void RstRunPara(void)
{
    g_gRunPara[RP_CFG_KEY] = 0xAAC9;//1000 1100 
    g_gRunPara[RP_CFG_KEYH] = 0x0401;//1000 1100 
    g_gRunPara[YX_MODEl]=0x00;   // 0 ��ʵʱ
    g_gRunPara[YX_MODEh]=0x00; 
    g_gRunPara[YX_DSl]=0xF000;  //shuang
    g_gRunPara[YX_DSh]=0x00FF;  //shuang
    g_gRunPara[RP_YXYCMODE]=0;
    g_gRunPara[Terminal_Type] = 0;
    g_gRunPara[RP_POWER_MODE] = 0;//�ϵ�׼ʵʱģʽ
    g_gRunPara[RP_GPRS_COMP] = 1;//Ĭ��ӳ�ͨ
    g_gRunPara[RP_DEVICE_TYPE] = 1;
    g_gRunPara[RP_ZSQ_TYPE] = 0;//Ĭ��ѡ��RF001��
    g_gRunPara[RP_COMM_ADDR] = 1;//ͨ��վ��ַ      վ��ַĬ��Ϊ1
    g_gRunPara[RP_FLTLK_T] = 60;
    g_gRunPara[RP_ZSQCST_T] = 600;
    g_gRunPara[RP_ADCOL_T] = 1; 
    g_gRunPara[RP_BEAT_T] = 0;//Ĭ�ϲ���������
    g_gRunPara[RP_SENDYC_T] = 900;  
    g_gRunPara[RP_RFBAND] = 6;
    g_gRunPara[RP_SYX_INFADDR] = 0x01;
    g_gRunPara[RP_DYX_INFADDR] = DYX_ADDR;
    g_gRunPara[RP_YXRSD_T] = 600;//10���� 
    g_gRunPara[RP_YCLIMIT] = 120;//ң����ֵ ����
    g_gRunPara[YX_delay] = 20;
    g_gRunPara[RP_SAVELOAD_T] = 900;//15m
    g_gRunPara[RP_TELIMIT] = 64;//�¶���ֵ
    g_gRunPara[RP_ADBAT_T]=1440;
    g_gRunPara[RP_valt_T]=7200;
    g_gRunPara[RP_YCCAP] = 50;//ң����ֵ ���
    g_gRunPara[RP_STATUSRP_TIME] = 9;      //״̬�㱨����ʱ��
    g_gRunPara[RP_STATUSRP_INTERVAL] = 1;  //״̬�㱨���߼�����죩
    g_gRunPara[RP_CLOSEGPRSPWR_T] = 180;//180;
    g_gRunPara[RP_SNTP_T]=60;
    g_gRunPara[RP_ALARM_BAT] = 11000;//��ص�ѹ������ֵĬ��=590
    g_gRunPara[RP_ABSOLUTE_CURREN] = 90;//�����仯
    g_gRunPara[RP_ABSOLUTE_VOLTAGE]= 180;//��ѹ�仯10%
    g_gRunPara[RP_COEFFI_VOLTAGE] =5773;
    g_gRunPara[RP_diff_NUM] =5;
    g_gRunPara[RP_electric_Vari]= 10;
    g_gRunPara[RP_VOLTAGE_RMS]= 20;
  g_gRunPara[RP_LUBOGPRS_T] =30;
    g_gRunPara[RP_CURRENT_CY]=200;
  g_gRunPara[RP_CURRENT_CA]=1000;
  g_gRunPara[RP_CURRENT_CB]=1000;
  g_gRunPara[RP_CURRENT_CC]=1000;
  g_gRunPara[RP_VOLTAGE_V0]=1000;
  g_gRunPara[RP_LBSTORAGE_TIME]=RP_LBSTORAGE_INVALID;
  g_gRunPara[RP_LUBO_NUM]= RP_LUBO_INVALID;
  g_gRunPara[RP_LUBO_CYCLE]= 0;
  g_gRunPara[RP_CALL_TIME]= 10;
  g_gRunPara[RP_GPS_T]= 15;//min
  g_gRunPara[RP_OPENGPS_T]= 300;//s
  g_gRunPara[RP_OPENGPS_NUM] =3;
  g_gRunPara[RP_SHORTFAULT_RESETTIME]=6;
  g_gRunPara[RP_LUBO_RESETTIME] =600;//10����
  g_gRunPara[RP_SET_KEY]=0;
  g_gRunPara[RP_YXRSD_NUM]=20;
    //g_gRunPara[RP_RECORDER_TOLNUM] = 20;  
}
void RstIEC101Para(void )
{
      g_ucPara101[IECP_LINKADDR_NUM] = 2;//2;
      g_ucPara101[IECP_TRANSRSN_NUM] = 2;//2;
      g_ucPara101[IECP_COMNADDR_NUM] = 2;
      g_ucPara101[IECP_INFO_NUM] = 2;
      g_ucPara101[IECP_COM_MODE] = 1;//0;
      g_ucPara101[IECP_AUT_MODE] = 0;
      
      g_ucPara101[IECP_OBJ_COM0] = OBJRF;
      g_ucPara101[IECP_OBJ_COM1] = OBJGPRS;
      g_ucPara101[IECP_OBJ_COM2] = OBJGPS;//OBJEX;
      g_ucPara101[IECP_OBJ_COM3] = OBJDBG;
      g_ucPara101[IECP_YC_TYPE] = 0;//Ĭ����9����һ��ֵ


}

void RstIEC101YxAddr(void)  //ң�ŵ��ĳ�ʼ��
{
    unsigned int i,j=0;
    
      memset(g_ucYXAddr,0,sizeof(g_ucYXAddr));
      for(i = RI_1A_SHORT; i <= RI_3O_SHORT;i++)
          g_ucYXAddr[j++] = i+1;
      for(i = RI_1A_GND; i <= RI_3O_GND;i++)
         g_ucYXAddr[j++] = i+1;
      for(i = RI_1A_LVD; i <= RI_3O_LVD;i++)
         g_ucYXAddr[j++] = i+1;
      for(i = RI_1A_CABT; i <= RI_3O_CABT;i++)
         g_ucYXAddr[j++] = i+1;
      for(i = RI_ZD_LVD; i <= RI_IN_DAERR;i++)
          g_ucYXAddr[j++] = i+1;
      for(i = RI_ZD_YX1; i <= RI_ZD_YX12;i++)
          g_ucYXAddr[j++] = i+1;
       for(i = RI_ZD_DYX1; i <= RI_ZD_DYX6;i++)
          g_ucYXAddr[j++] = i+1;
       

        
//      for(i = 0; i < IEC_YX_NUM; i++)
//      {
//          g_ucYXAddr[i] = i+1;
//      }
}

void RstIEC101YcAddr(void)  //ң����ĳ�ʼ��
{
    unsigned char i;
    for(i = 0; i < IEC_YC_NUM; i++)
    {
        g_ucYCAddr[i] = i+1;
    }
}

void RstIEC101YcSiqu(void)  //ң�������ĳ�ʼ��
{
//      unsigned char i;
//      for(i = 0; i < IEC_YC_NUM; i++)
//      {
//          g_unYCSiqu[i] = 10;
//      }
}

void RstIEC101YkAddr(void)  //ң�ص��ĳ�ʼ��
{
    unsigned char i;
    for(i = 0; i < IEC_YK_NUM; i++)
    {
        g_ucYKPa[i] = 0;
    }
    g_ucYKPa[OUT_RMT1] = OUT_RMT1_ORDER;
    g_ucYKPa[OUT_RMT2] = OUT_RMT2_ORDER;
    g_ucYKPa[OUT_CLEAR_SOE] = OUT_CLEAR_SOE_ORDER;
    g_ucYKPa[OUT_GPRS_PW] = OUT_GPRS_PW_ORDER;
    g_ucYKPa[OUT_BAT_ACT] = OUT_BAT_ACT_ORDER;
    g_ucYKPa[OUT_CLEAR_LOAD] = OUT_CLEAR_LOAD_ORDER;
    g_ucYKPa[OUT_ODU_RESET] = OUT_ODU_RESET_ORDER;
    g_ucYKPa[OUT_ODU2_RESET] = OUT_ODU2_RESET_ORDER;
    g_ucYKPa[OUT_CLEAR_COMTRADE]= OUT_COMTRAD_ORDER;
  g_ucYKPa[OUT_GPS_PW]= OUT_GPS_PW_ORDER;
  g_ucYKPa[OUT_CALL_YC]= OUT_CALL_FI_YC;
  g_ucYKPa[OUT_TEST_MODE] = OUT_CALL_AUTO_TEST_MODE;
  g_ucYKPa[OUT_SET_MODE] = OUT_CALL_SET_MODE;
 g_ucYKPa[OUT_RST_ALL1] = OUT_LINE1_FI;
  g_ucYKPa[OUT_RST_ALL2] = OUT_LINE2_FI;
  g_ucYKPa[OUT_RST_ALL3] = OUT_LINE3_FI;
}
void RstSmsPhoneInfo(void)  //�绰�����ĳ�ʼ��
{
    char chDefPhone[] = "+86 13811005566";
    BYTE i;
    for(i = 0; i < PHONE_PA_NUM;i++)
      g_gSmsPhone[i] = 0;
      
    g_gSmsPhone[PHONE_ENCTL] = 1;//Ĭ�ϵ�һ���绰��������
    for(i = 0; i < PHONE_NUM;i++)
    {
      g_gSmsPhone[PN_START+i*PHONE_CHARNUM] = (PHONE_CHARNUM-1);
      memcpy(&(g_gSmsPhone[PN_START+i*PHONE_CHARNUM+1]),chDefPhone,(PHONE_CHARNUM-1));
    }
//      g_gSmsPhone[PHONE1_CHARLEN] = PHONE_CHARNUM;
//      g_gSmsPhone[PHONE2_CHARLEN] = PHONE_CHARNUM;
//      g_gSmsPhone[PHONE3_CHARLEN] = PHONE_CHARNUM;
//      memcpy(&(g_gSmsPhone[PHONE1]),chDefPhone,PHONE_CHARNUM);
//      memcpy(&(g_gSmsPhone[PHONE2]),chDefPhone,PHONE_CHARNUM);
//      memcpy(&(g_gSmsPhone[PHONE3]),chDefPhone,PHONE_CHARNUM);
    g_pVersion[0] = strlen((char*)VerNum); //���汾�Ÿ�ֵ
    for(BYTE i = 0; i < g_pVersion[0]; i++)
    {
        g_pVersion[1+i] = VerNum[i];
    }
    g_pCompany[0] = 2;
    g_pCompany[1] = 'K';
    g_pCompany[2] = 'R';
	  g_pDevId[0] = 24;
    memcpy(&g_pDevId[1],"JYL0123456789012345678900",24);
}

//����¼��ң��ֵ
void SetLbNum()
{
        if(g_gRunPara[RP_LUBO_NUM] < RP_LUBO_INVALID && (g_gRunPara[RP_LUBO_NUM] < g_Recorder_Total_COUNT))//g_gRunPara[RP_LUBO_NUM] > 0 && (
        {
           g_gRmtMeas[RM_LUBO_NUM] = g_gRunPara[RP_LUBO_NUM];
           g_ucRefYxYcTs |= BIT1;
        }
        else
        {
            g_gRmtMeas[RM_LUBO_NUM] = g_Recorder_Total_COUNT;
            g_ucRefYxYcTs |= BIT1;
        }
        if(g_gRunPara[RP_LUBO_NUM] < RP_LUBO_INVALID)
        {
           g_gRmtMeas[RM_ALLLUBO_NUM] = g_gRunPara[RP_LUBO_NUM];
           g_ucRefYxYcTs |= BIT1;
        }
        else
        {
            g_gRmtMeas[RM_ALLLUBO_NUM] = 64;
            g_ucRefYxYcTs |= BIT1;
        }
}


