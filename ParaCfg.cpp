/*------------------------------------------------------------------------
 Module:      ParaCfg.cpp
 Author:      linxueqin
 Project:     子站平台
 State:       V1.0
 Creation Date:   2014-4-29
 Description:   运行参数，规约参数，遥信，遥测，遥控参数的初始化，保存及复位
------------------------------------------------------------------------*/


#ifndef _PARA_CFG

#define _PARA_CFG
  #include "DataStruct.h"
#endif

//==============================================================================
//  函数名称   : InitPara
//  功能描述   : 注意对用到的参数进行开机初始化，并从EEPROM中取出掉电不丢失的数据
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void InitcfgPara(void)
{
    unsigned int i;
    g_pVersion = &(g_gSmsPhone[VERSION]);//版本号可写
    g_pCompany = &(g_gSmsPhone[COMPNAME]);
    g_pDevId = &(g_gSmsPhone[DEVID+1]);
    g_ucParaChang = 0; //必须放在CheckCfgPara函数前边，101参数发生变更标志  BIT0为 101参数，BIT1为遥测点表，BIT2为遥测系数，BIT3为遥测偏移量，BIT4为遥信点表 BIT5为运行参数表
    
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
    CheckCfgPara();//读取运行参数，如果参数错误，则复位成默认值
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
    g_ucRefYxYcTs = 0;//遥信遥测数据变化标志 BIT0为遥信 BIT1为遥测
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
  g_RF003_timeInfo.RF003_mini_num[i] =0;//RF003的分钟计数
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
    g_gRmtLock10s[i] =10;//根据接口软件的设置复位时以10S为单位
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
    //根据遥信点表计算出需要上传的遥信个数
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
      {//单点
        if(g_ucSYxTrsStartId == 0xffff) 
          g_ucSYxTrsStartId = i;
        BYTE bRIVal = 0;
        if(RIIndex < DYX_ADDR)     bRIVal  = GetRIStatus(RIIndex);
        else if(RIIndex ==0xfffe)  bRIVal = 0;
        else if(RIIndex ==0xfffd)  bRIVal = 1;
        SetRIStatus(sYxCount++,bRIVal,1);
      }
      else
      {//双点
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
    //根据遥测点表计算出需要上传的遥测个数
    for(i = 0; i < IEC_YC_NUM; i++)
    {
        if(g_ucYCAddr[i] == 0)
            break;
    }
    g_ucYcTransNum = i;
    for(i = 0; i < g_ucYcTransNum; i++)   //计算需要上传的遥测值，按照具体情况进行换算
    {
      RIIndex = g_ucYCAddr[i];
      if(RIIndex < 255)
          g_unYcTrans[i] = g_gRmtMeas[g_ucYCAddr[i] - 1];//(((unsigned long)g_gRmtMeas[g_ucYCAddr[i] - 1] * g_unYCCoe[i]) >> 10 )+ g_unYCOffset[i];
      else
        g_unYcTrans[i] = 0;
  }
}

//==============================================================================
//  函数名称   : SaveCfgPara
//  功能描述   : 在运行过程中，根据g_gChangFlag中的参数更改标志判断某各配置参数发生变化，把配置参数保存到对应的存储器和备份区中
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void SaveCfgPara(void)  //在运行过程中，如果某各配置参数发生变化，把配置参数保存到对应的存储器中
{
    RefreshPara();
 
    //运行参数
    if((g_ucParaChang & BIT0) == BIT0)
    {
        g_ucParaChang &= NBIT0;
      
        g_LogType |= BITD;
        g_gRunPara[RP_CRC] = CrcCount((unsigned int *)g_gRunPara, RP_CRC);      //计算CRC
        CAT_SpiWriteWords(EEPADD_RP, RUN_PARA_NUM, g_gRunPara); //保存到EEPROM中

        if(g_gRunPara[Terminal_Type] == AERIAL_TERMINAL)
        {
          //根据实时模式和准实时模式开或关GPRS ODU的模式在程序中计算
          if(g_gRunPara[RP_POWER_MODE] == QUASI_REALTIME)
            CloseGPRS();
          else if(g_gRunPara[RP_POWER_MODE] == REALTIME)
          {
             OpenGPRS();
             if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
                 g_EnSmsSend = ON;
             g_sTimer[TM_GPRSPWOERDOWN].m_TmCount=0; //清空准实时模式下GPRS定时过程 
          }
        }
        
        //一遥设备 关闭遥测上传
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
    if(g_off_gps == ON)//只有GPS的配置发生变化时，才会重新对其配置
    {
      g_off_gps = OFF;
      if(g_gRunPara[RP_CFG_KEYH]& (BIT[RP_OFF_GPS-16]))
      {
        CLOSE_GPS_POWER;
        g_sTimer[TM_GPS_TIME].m_TmCount =0;
        g_sTimer[TM_OPENGPS_TIME].m_TmCount =0;
        g_CrystalError.Compen_cry_total =0;
        g_CrystalError.Crystal_start_flag =0;
        //g_gRunPara[RP_CFG_KEYH] &= ~(BIT[RP_set_TIME-16]);//关闭GPS的同时打开RTC
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
          UCA3IFG &= ~UCTXIFG;                    //关闭发送中断  
          g_debug_rate = OFF;
          
          Uart3Init();
          if(g_gRunPara[RP_CFG_KEY]& BIT[RP_DEBUG_RATE])//由内部晶振转外部晶振时需要重启生效
              while(1);
               //app();//应用程序入口      
          
        }
        //更新定时器计数值
        unsigned int TmId = 0;
        for(TmId = 0;TmId < TIMER_NUM;TmId++)
        {
          if(g_sTimer[TmId].m_TmCount > 0)
          {//定时器已启用
            RefreshTmCount(TmId);
          }
        } 
    }

    
    //IEC101参数设置
    if((g_ucParaChang & BIT1) == BIT1)
    {
        g_ucParaChang &= NBIT1;
        g_ucPara101[IECP_CRC] = AddChar(g_ucPara101, IECP_CRC);      //计算CS
        CAT_SpiWriteBytes(EEPADD_IECPARA , IEC_PARA_NUM, g_ucPara101);     //保存到EEPROM中
        CAT_SpiWriteBytes(EEPADDBK_IECPARA, IEC_PARA_NUM, g_ucPara101);    //保存到EEPROM的备份区中
    }
    //遥信地址点表
    if((g_ucParaChang & BIT2) == BIT2)
    {
        g_ucParaChang &= NBIT2;
        g_ucYXAddr[IEC_YX_NUM] = AddChar((unsigned char *)g_ucYXAddr, IEC_YX_NUM*2);      //计算CS
        #define RDYXMUN_MAX 100 //单次读取e2最大个数
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
        
        //CAT_SpiWriteBytes(EEPADD_IECYXADDR , IEC_YX_NUM*2 + 1, (unsigned char *)g_ucYXAddr);     //保存到EEPROM中
        //CAT_SpiWriteBytes(EEPADDBK_IECYXADDR, IEC_YX_NUM*2 + 1, (unsigned char *)g_ucYXAddr);    //保存到EEPROM的备份区中
        
   }
    //遥测参数
    if((g_ucParaChang & BIT3) == BIT3)
    {
        g_ucParaChang &= NBIT3;             
        g_ucYCAddr[IEC_YC_NUM] = AddChar(g_ucYCAddr, IEC_YC_NUM);      //计算CS
        CAT_SpiWriteBytes(EEPADD_IECYCADDR , IEC_YC_NUM + 1, g_ucYCAddr);     //保存到EEPROM中
        CAT_SpiWriteBytes(EEPADDBK_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr);    //保存到EEPROM的备份区中
      
    }
    //遥控地址点表
    if((g_ucParaChang & BIT4) == BIT4)
    {
        g_ucParaChang &= NBIT4;
        g_ucYKPa[IEC_YK_NUM] = AddChar(g_ucYKPa, IEC_YK_NUM);      //计算CS
        CAT_SpiWriteBytes(EEPADD_IECYKADDR , IEC_YK_NUM + 1, g_ucYKPa);     //保存到EEPROM中
        CAT_SpiWriteBytes(EEPADDBK_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa);    //保存到EEPROM的备份区中
    }
    //电话号码表
    if((g_ucParaChang & BIT5) == BIT5)
    {
        g_ucParaChang &= NBIT5;
        g_gSmsPhone[PHONE_CS] = AddChar(g_gSmsPhone, PHONE_CS);      //计算CS
        CAT_SpiWriteBytes(EEPADD_PHONE , PHONE_PA_NUM, g_gSmsPhone);     //保存到EEPROM中
        CAT_SpiWriteBytes(EEPADDBK_PHONE, PHONE_PA_NUM, g_gSmsPhone);    //保存到EEPROM的备份区中
    }

}
//==============================================================================
//  函数名称   : CheckCfgPara
//  功能描述   : 从EEPROM中读取参数，并做校验判断数据的有效性，数据错误则报101参数错误遥信，并把101参数置为默认值
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void CheckCfgPara(void)
{
    //unsigned char i;
    
    //从EEPROM中读取运行参数，判断有效性，有效则赋值给g_gRunPara，无效则初始化g_gRunPara为默认值
    CAT_SpiReadWords(EEPADD_RP, RUN_PARA_NUM, g_gRunPara);  //读出当前保护定值
    if (g_gRunPara[RP_CRC] != CrcCount((unsigned int *)g_gRunPara, RP_CRC)) //CRC校验错误
    {
      RstRunPara();
      g_ucParaChang |= BIT0;   //调用保存函数
    }
    g_gRunParaFltLkTBk = g_gRunPara[RP_FLTLK_T];
    g_gRunParaZsqStTBk = g_gRunPara[RP_ZSQCST_T];
    g_gRunParaYxRsdTTBk = g_gRunPara[RP_YXRSD_T];
    g_gShortFltLkTBk = g_gRunPara[RP_SHORTFAULT_RESETTIME];
    //101的系统参数
    CAT_SpiReadBytes(EEPADD_IECPARA, IEC_PARA_NUM, g_ucPara101);  //读出
    if (g_ucPara101[IECP_CRC] != AddChar(g_ucPara101, IECP_CRC))       //CRC校验
    {
        CAT_SpiReadBytes(EEPADDBK_IECPARA, IEC_PARA_NUM, g_ucPara101);    //读出
        if (g_ucPara101[IECP_CRC] == AddChar(g_ucPara101, IECP_CRC))       //CRC校验
        {
            CAT_SpiWriteBytes(EEPADD_IECPARA, IEC_PARA_NUM, g_ucPara101); //备份区覆盖过去
        }
        else
        {
            RstIEC101Para();  //IEC para initial
            g_ucParaChang |= BIT1;   //调用保存函数
        }
    }
    //101的遥信点表
      #define RDYXMUN_MAX 100 //单次读取e2最大个数
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
    if (g_ucYXAddr[IEC_YX_NUM] != AddChar((unsigned char *)g_ucYXAddr, IEC_YX_NUM*2))       //CRC校验
    {
       RstIEC101YxAddr();  //遥信点表初始化
       g_ucParaChang |= BIT2;   //调用保存函数
    }
    
    //101的遥测点表
    CAT_SpiReadBytes(EEPADD_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr);  //读出
    if (g_ucYCAddr[IEC_YC_NUM] != AddChar(g_ucYCAddr, IEC_YC_NUM))       //CRC校验
    {
        CAT_SpiReadBytes(EEPADDBK_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr);    //读出
        if (g_ucYCAddr[IEC_YC_NUM] == AddChar(g_ucYCAddr, IEC_YC_NUM))       //CRC校验
        {
            CAT_SpiWriteBytes(EEPADD_IECYCADDR, IEC_YC_NUM + 1, g_ucYCAddr); //备份区覆盖过去
        }
        else
        {
            RstIEC101YcAddr();  //遥测点表初始化
            g_ucParaChang |= BIT3;   //调用保存函数
        }
    }
    //101的遥控点表
    CAT_SpiReadBytes(EEPADD_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa);  //读出
    if (g_ucYKPa[IEC_YK_NUM] != AddChar(g_ucYKPa, IEC_YK_NUM))       //CRC校验
    {
        CAT_SpiReadBytes(EEPADDBK_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa);    //读出
        if (g_ucYKPa[IEC_YK_NUM] == AddChar(g_ucYKPa, IEC_YK_NUM))       //CRC校验
        {
            CAT_SpiWriteBytes(EEPADD_IECYKADDR, IEC_YK_NUM + 1, g_ucYKPa); //备份区覆盖过去
        }
        else
        {
            RstIEC101YkAddr();  //遥控点表初始化
            g_ucParaChang |= BIT4;   //调用保存函数
        }
    }
    //电话号码表
    CAT_SpiReadBytes(EEPADD_PHONE, PHONE_PA_NUM, g_gSmsPhone);  //读出
    if (g_gSmsPhone[PHONE_CS] != AddChar(g_gSmsPhone, PHONE_CS))       //CRC校验
    {
        CAT_SpiReadBytes(EEPADDBK_PHONE, PHONE_PA_NUM, g_gSmsPhone);    //读出
        if (g_gSmsPhone[PHONE_CS] == AddChar(g_gSmsPhone, PHONE_CS))       //CRC校验
        {
            CAT_SpiWriteBytes(EEPADD_PHONE, PHONE_PA_NUM, g_gSmsPhone); //备份区覆盖过去
        }
        else
        {
            RstSmsPhoneInfo();  //电话号码表初始化
            g_ucParaChang |= BIT5;   //调用保存函数
        }
    }
}

//==============================================================================
//  函数名称   : RstRunPara
//  功能描述   : 复位内存中的运行参数为默认值
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void RstRunPara(void)
{
    g_gRunPara[RP_CFG_KEY] = 0xAAC9;//1000 1100 
    g_gRunPara[RP_CFG_KEYH] = 0x0401;//1000 1100 
    g_gRunPara[YX_MODEl]=0x00;   // 0 ：实时
    g_gRunPara[YX_MODEh]=0x00; 
    g_gRunPara[YX_DSl]=0xF000;  //shuang
    g_gRunPara[YX_DSh]=0x00FF;  //shuang
    g_gRunPara[RP_YXYCMODE]=0;
    g_gRunPara[Terminal_Type] = 0;
    g_gRunPara[RP_POWER_MODE] = 0;//上电准实时模式
    g_gRunPara[RP_GPRS_COMP] = 1;//默认映瀚通
    g_gRunPara[RP_DEVICE_TYPE] = 1;
    g_gRunPara[RP_ZSQ_TYPE] = 0;//默认选中RF001型
    g_gRunPara[RP_COMM_ADDR] = 1;//通信站地址      站地址默认为1
    g_gRunPara[RP_FLTLK_T] = 60;
    g_gRunPara[RP_ZSQCST_T] = 600;
    g_gRunPara[RP_ADCOL_T] = 1; 
    g_gRunPara[RP_BEAT_T] = 0;//默认不传输心跳
    g_gRunPara[RP_SENDYC_T] = 900;  
    g_gRunPara[RP_RFBAND] = 6;
    g_gRunPara[RP_SYX_INFADDR] = 0x01;
    g_gRunPara[RP_DYX_INFADDR] = DYX_ADDR;
    g_gRunPara[RP_YXRSD_T] = 600;//10分钟 
    g_gRunPara[RP_YCLIMIT] = 120;//遥测阈值 绝对
    g_gRunPara[YX_delay] = 20;
    g_gRunPara[RP_SAVELOAD_T] = 900;//15m
    g_gRunPara[RP_TELIMIT] = 64;//温度阈值
    g_gRunPara[RP_ADBAT_T]=1440;
    g_gRunPara[RP_valt_T]=7200;
    g_gRunPara[RP_YCCAP] = 50;//遥测阈值 相对
    g_gRunPara[RP_STATUSRP_TIME] = 9;      //状态汇报上线时间
    g_gRunPara[RP_STATUSRP_INTERVAL] = 1;  //状态汇报上线间隔（天）
    g_gRunPara[RP_CLOSEGPRSPWR_T] = 180;//180;
    g_gRunPara[RP_SNTP_T]=60;
    g_gRunPara[RP_ALARM_BAT] = 11000;//电池电压报警阀值默认=590
    g_gRunPara[RP_ABSOLUTE_CURREN] = 90;//电流变化
    g_gRunPara[RP_ABSOLUTE_VOLTAGE]= 180;//电压变化10%
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
  g_gRunPara[RP_LUBO_RESETTIME] =600;//10分钟
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
      g_ucPara101[IECP_YC_TYPE] = 0;//默认用9，归一化值


}

void RstIEC101YxAddr(void)  //遥信点表的初始化
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

void RstIEC101YcAddr(void)  //遥测点表的初始化
{
    unsigned char i;
    for(i = 0; i < IEC_YC_NUM; i++)
    {
        g_ucYCAddr[i] = i+1;
    }
}

void RstIEC101YcSiqu(void)  //遥测死区的初始化
{
//      unsigned char i;
//      for(i = 0; i < IEC_YC_NUM; i++)
//      {
//          g_unYCSiqu[i] = 10;
//      }
}

void RstIEC101YkAddr(void)  //遥控点表的初始化
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
void RstSmsPhoneInfo(void)  //电话号码表的初始化
{
    char chDefPhone[] = "+86 13811005566";
    BYTE i;
    for(i = 0; i < PHONE_PA_NUM;i++)
      g_gSmsPhone[i] = 0;
      
    g_gSmsPhone[PHONE_ENCTL] = 1;//默认第一个电话号码启用
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
    g_pVersion[0] = strlen((char*)VerNum); //给版本号赋值
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

//更新录波遥测值
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


