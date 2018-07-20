#ifndef _MAIN

#define _MAIN
#include "DataStruct.h"
#include "Main.h"

 CBJ101S mBJ101S0;
 CBJ101S mBJ101S1;
 //CPrtcOdu mPrtcOdu;
 //CPrtcOdu mPrtcOdu1;
 CPrtcGPS mPrtcGPS;//新增加的类
 CPrtcRF mPrtcRf;
#ifdef USE_GB104S
 CGB104S mGB104S;
#endif
WORD g_LEN;

void app(void)@"APPLICATION"
{
    WDTCTL = WDTPW + WDTHOLD;//禁止芯片自带的看门狗，采用外部看门狗
    InitSys();
    //strcpy(ComtrderCfg1,ComtrderCfg);
  //char *pdat_name;
  //pdat_name = "8,Un,N,,V,1.000000,0.000000,0.000000,-8192,8191,1,1,S\r\n";
   //g_len = strlen(pdat_name); 
    //InitCommObj();
    //ReplySuccess(); //重启成功，和远程升级有关

    //启动时取出重启次数加1并保存
    CAT_SpiReadWords(EEPADD_RSTARTNUM, 1, &g_RstartNum);
    if(g_RstartNum >= 0xfffe) g_RstartNum = 0;
    
    g_RstartNum++;
    CAT_SpiWriteWords(EEPADD_RSTARTNUM, 1, &g_RstartNum);
    CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &g_FRecorder_Current_COUNT);//先读出来防止断电后归0
     
    CAT_SpiReadWords(EEPADD_LUBONUM, 1, &(g_Recorder_Total_COUNT));//先读出来防止断电后归0
    if((g_FRecorder_Current_COUNT >= 0xfffe)||(g_Recorder_Total_COUNT>= 0xfffe))
    {
        g_FRecorder_Current_COUNT =0;
  CAT_SpiWriteWords(EEPADD_LUBOFLAG, 1, &g_FRecorder_Current_COUNT);
  g_Recorder_Total_COUNT =0;
        CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &(g_Recorder_Total_COUNT));
    }
    SetLbNum();
    g_LogType |= BIT0;//重启记录
    
    CAT_SpiReadBytes(EEPADD_VerNum, strlen((char *)VerNum), g_VerNum);
    if((g_VerNum[0] ==0xff)||(g_VerNum[1] ==0xff))//版本号前两个字母为SV
    {

       strcpy((char *)g_VerNum,(char *)VerNum);
       CAT_SpiWriteBytes(EEPADD_VerNum, strlen((char *)VerNum),g_VerNum);
    }
    else if(strcmp((char *)g_VerNum,(char *)VerNum))
    {
       strcpy((char *)g_VerNum,(char *)VerNum);
       CAT_SpiWriteBytes(EEPADD_VerNum, strlen((char *)VerNum), g_VerNum);
       g_LogType |= BITE;
    }
   #ifdef GETSOEFROMRAM
      char *Flash_ptr; 
      char qq;  
      Flash_ptr = (char *) (SOE_RECORD_ADD); 
      qq=*Flash_ptr;
      if(qq == 0x55)//内部flash的数据没有清空时，读取SOE的个数
        CAT_SpiReadWords(EEPADD_SOE_NUM, 1, &soe_num);
    #endif
    //gRecorder_cfg[0].CFG_Leng=0x55;
    //unsigned int gRecorder_cf=0x55;
    
    //Sector_Erase(FADDR_SOE_START);
    //Sst26vf064b_Page_WriteBytes(FADDR_SOE_START+0*RECORDER_INFO_LENGTH,(unsigned char *)&(gRecorder_cf),2);
    //Sst26vf064b_Read(FADDR_SOE_START+0*RECORDER_INFO_LENGTH,(unsigned char *)&gRecorder_cf,2);
    /*unsigned int avalue = 0;
  
    CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &avalue);//测试*/

    g_LEN = strlen(ComtrderCfg); 
    while(1)
    {
        WatchDog();
        HandleTimerEvent();
        CalcuRmtMeas();//整理AD采集数据 
        if(g_sRtcManager.m_FixYCFlag == YES)
    {
      SaveLoad(0);
      g_sRtcManager.m_FixYCFlag = NO;
    }
    else if(g_sRtcManager.m_FixYCFlag == ON)
    {
      SaveLoad(0x55);
      g_sRtcManager.m_FixYCFlag = NO;
    }
        if(g_sRtcManager.m_gRealTimerFlag ==TRUE)//主站发起对时命令后，等邋MS=1000时再写入RTC芯片的时间
        {
            WriteRTCTime();//将系统时间写入RTC芯片中
            g_sRtcManager.m_gRealTimerFlag = OFF;
        }
        if((g_sRtcManager.m_ucRtcSynFlag == YES) &&(g_sRtcManager.m_gRealTimerFlag == OFF))//每4分钟从时钟芯片读取一次时间，该标志在定时器中断中计数设标志
        {
            ReadRealTime(1);//从RTC芯片中读取系统时钟，并更新SysTime中的时间
            g_sRtcManager.m_ucRtcSynFlag = NO;
        }
    
        if(!(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISDDMODE]))
        {//在对点模式下不处理实际数据
            if(pRf != null) pRf->Run();
            if(pOdu != null) pOdu->Run();
            if(pOdu1 != null) pOdu1->Run();
            if(pGps != null) pGps->Run();
        }
        //BYTE byBuf1[20] = {0XAA,0XAA,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11};  //测试使用
        //CommSendData(byBuf1,2,g_CmIdDBG);
        //pRf->SaveLBFlash(byBuf1,0,0);
        //OpenBatSmsGprs();
        SetYxTrans();
        SetYcTrans(); 
        //SaveSoeData();
        #ifndef GETSOEFROMRAM
            CopySoeToFlash();
    #else
      SaveSoeData();
        #endif
        
        if(pDbg != null) pDbg->Run();
        if(pGprs != null) pGprs->Run();

        SaveCfgPara();
    Check_LogFile();
        
        if(g_gRunPara[RP_CFG_KEY]& BIT[RP_DEBUG_RATE])
        {
      
          if(CheckInLPM3() == TRUE)
          {
         LPM3;//进入低功耗
          }
        }
        
    }//end of while(1)
    

}

//==============================================================================
//  函数名称   : CheckInLPM3
//  功能描述   : 判断是否进入低功耗
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : TRUE = 可进入低功耗 FALSE = 还有事件未处理，不能进入低功耗
//  其他说明   : 
//  作者       ：
//==============================================================================
bool CheckInLPM3(void)
{
    int iCmId = 0;
    for(iCmId = CMID0;iCmId <= CMID3;iCmId++)
    {
      if(g_sRxBuff[iCmId].m_iRcvFlag == ON)
      {
        return FALSE;
      }
    }
    if(g_sSampleFinish == ON)
      return FALSE;
      
    if(g_ucRefYxYcTs != 0)
      return FALSE;

  return TRUE;
}

void ReadSoeInfoFromFlash()
{
  CAT_SpiReadWords(EEPADD_SOE_FLASHNUM, 3, (unsigned int *)(&g_InFlashInfo));
  
   if(g_InFlashInfo.m_InFlashTailNum == 0xffff)
    {
      g_InFlashInfo.m_InFlashTailNum = 0;
    g_InFlashInfo.m_unSSoeInFlashHead = 0;
    g_InFlashInfo.m_OverWriteFlag = 0;
    
    }
   CAT_SpiReadWords(EEPADD_SSOETAIL, 1, &g_unSSoeInFlashTail);
   if(g_unSSoeInFlashTail == 0xffff)
    {
          g_unSSoeInFlashTail = 0;
    
    }
  
    if(pDbg != null)
    {
      CAT_SpiReadWords(EEPADD_SSOEHEAD1, 1, &(pDbg->m_SSoeHeadPtr));
      if(pDbg->m_SSoeHeadPtr == 0xffff) 
          pDbg->m_SSoeHeadPtr = 0;
      //if(pDbg->m_SSoeHeadPtr != g_unSSoeInFlashTail)
      //  g_yxChangeflag |= ((CBJ101S *)pDbg)->m_SSoeBit;
         CAT_SpiReadWords(EEPADD_SFEIHEAD1, 1, &(pDbg->m_SFeiHeadPtr));
         if(pDbg->m_SFeiHeadPtr == 0xffff) 
         {
             pDbg->m_SFeiHeadPtr = 0;
         }
     }

    if(pGprs != null)
    {
      CAT_SpiReadWords(EEPADD_SSOEHEAD2, 1, &(pGprs->m_SSoeHeadPtr));
      if(pGprs->m_SSoeHeadPtr == 0xffff) 
          pGprs->m_SSoeHeadPtr = 0;
      //if(pGprs->m_SSoeHeadPtr != g_unSSoeInFlashTail)
       // g_yxChangeflag |= ((CBJ101S *)pGprs)->m_SSoeBit;
        CAT_SpiReadWords(EEPADD_SFEIHEAD2, 1, &(pGprs->m_SFeiHeadPtr));
        if(pGprs->m_SFeiHeadPtr == 0xffff) 
        {
            pGprs->m_SFeiHeadPtr = 0;
        } 
    }
    
    
}


//==============================================================================
//  函数名称   : InitCommObj
//  功能描述   : 初始化端口对象
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : 
//  其他说明   : 
//  作者       ：
//==============================================================================
void InitCommObj(void)
{
  //初始化通道，给各个串口分配对象==============================================
    //如果通道号配置错误，可以将DEBUG宏去掉，让程序处于正式运行状态，
    //此状态下调试串口默认被分配到com3
     g_CmIdRF   = CMIDINV;
     g_CmIdGPRS = CMIDINV;
     g_CmIdEX  =  CMIDINV; 
     g_CmIdDBG =  CMIDINV; 
     g_CmIdODU1 = CMIDINV;
     g_CmIdODU2 = CMIDINV;
     g_CmIdGps = CMIDINV;
     #ifdef DEBUG
      for(int i = CMID0;i <= CMID3;i++)
     #else
      for(int i = CMID0;i <= CMID2;i++)
     #endif
     {
      switch (g_ucPara101[IECP_OBJ_COM0 + i])
      {
        case OBJRF:
          g_CmIdRF   = i;
          break;
        case OBJGPRS:
          g_CmIdGPRS = i;
          break;
        case OBJDBG:
          g_CmIdDBG = i;
          break;
        case OBJEX:
          g_CmIdEX = i;
          break;
         case OBJODU1:
          g_CmIdODU1 = i;
          break;
         case OBJODU2:
          g_CmIdODU2 = i;
          break;
         case OBJGPS:
          g_CmIdGps = i;
          break;
        default:
          break;
      }
     }
     #ifndef DEBUG
      g_CmIdDBG = CMID3;//运行时调试通道默认配置到串口3,不认参数设置中的值
     #endif
      
  //========================================================================
     pDbg = null;
     pGprs = null;
     pRf = null;
     pOdu = null;
     pOdu1 = null;
     pGps  = null;
    int iCmId = 0;
    
    //调试口
    iCmId = g_CmIdDBG;
    if( iCmId != CMIDINV)
    {
      pDbg = &mBJ101S0;
      pDbg->Init(iCmId);
    }

    //GPRS口
    iCmId = g_CmIdGPRS;
    if(iCmId!= CMIDINV)
    {
      #ifdef USE_GB104S
      pGprs = &mGB104S;
      pGprs->Init(iCmId);
      #else
      pGprs = &mBJ101S1;
      pGprs->Init(iCmId);
      #endif
     }
    //GPS默认配置到扩展口
    iCmId = g_CmIdGps;
   
      if( iCmId != CMIDINV)
      {
        pGps = &mPrtcGPS;
        pGps->Init(iCmId);
      }
   
    
    //RF口和ODU0复用，扩展串口和ODU1复用
    if(g_gRunPara[Terminal_Type] == CABLE_TERMINAL_DODU)
    {//若是双ODU
      iCmId = g_CmIdODU1;
      /*if(iCmId != CMIDINV)
      {
        pOdu = &mPrtcOdu;
        pOdu->Init(iCmId);
      }*/
      iCmId = g_CmIdODU2;
      /*if(iCmId != CMIDINV)
      {
        pOdu1 = &mPrtcOdu1;
        pOdu1->Init(iCmId);
      }*/
    }
    else if(g_gRunPara[Terminal_Type] == CABLE_TERMINAL_ODU)
    {//单ODU
      iCmId = g_CmIdODU1;
      /*if(iCmId != CMIDINV)
      {
        pOdu = &mPrtcOdu;
        pOdu->Init(iCmId);
      }*/
    }
    else
    {//架空或者电缆RF
      iCmId = g_CmIdRF;
      if(iCmId != CMIDINV)
      {
        pRf= &mPrtcRf;
        pRf->Init(iCmId);
      }
    }

   //端口协议的4个参数进行备份
    for(int i = 0;i < 4;i++)
        g_unParaObjComBk[i] = g_ucPara101[IECP_OBJ_COM0 + i];

  #ifndef GETSOEFROMRAM
   ReadSoeInfoFromFlash();//读取保存到外部flash中的SOE信息
  #endif
    return;
}

//==============================================================================
//  函数名称   : InitSys
//  功能描述   : CPU开机所有初始化工作，包括各个外设、参数、模块的初始化，初始化结束后启动中断，进入大循环
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void InitSys(void)
{
    _DINT();  
    IoInit();//IO端口初始化
    InitClk();//系统时钟初始化
    LIGHT_RUN_DOWN(); 
  LIGHT_RUN2_DOWN();
    InitAdc(); 
    WIRE_LESS_RST_L(); //复位无线模块
    CLOSE_DI_POWER();//关闭DI采集电源
    CLOSE_BAT_POWER;
    WatchDog();
    
    
    ReadRealTime(0);    //读出全部RTC数据
    UCB0SPIInit();  //SPI总线初始化
    InitcfgPara();//系统参数初始化，包括系统用到的参数初始化和从存储器中读取出来的参数。
    InitRtc();//系统时钟初始化
    InitComm();//通信初始化,包括RF通信和GPRS通信、232通信。  //必须在参数初始化之后，因为有用到运行参数

    InitDataProc();
    
    //InitDin();//初始化DI
    DisAd0Sample();//禁止AD采集
    InitTimer();//定时器初始化
    
     OPEN_WIRE_POWER;   //打开无线模块电源
    OPEN_SUN_CHARGE;   //打开太阳能充电
    
    WIRE_LESS_RST_H(); //设置无线模块复位脚为高
    InitGPRS();
    ReadRealTime(2); //初始化时从RTC读取时间更新到实时时钟
    InitCommObj();
    
    //WDTCTL = WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2;//开启WDT
    _EINT(); 
    InitGps();
  if(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL)
  {
      g_gRunPara[RP_POWER_MODE] = QUASI_REALTIME;
      SetRIStatus(RI_REAL_QUASI,1); 
      g_ucRefYxYcTs |= BIT0;
      
  }
  #if 1
    BYTE byBuf1[2] = {0XAA,0XAA};  //测试使用
    CommSendData(byBuf1,2,g_CmIdDBG);
 #endif 
}



void WatchDog(void)
{
 FEED_WATCH_DOG();
}
void ExitGprsSet(void)
{
  GPRS_SETA_H();
  GPRS_ONOFF_IN();
  GPRS_RST_H();     
}
void HandleTimerEvent(void)
{
    //unsigned char num_flag =0;
    if(g_sTimer[TM_ADCOLT].m_TmFlag == ON)
    {//启动AD采集位
        g_sTimer[TM_ADCOLT].m_TmFlag = OFF;
        Open_BatSunC_Para(OPEN_SUNC);
        g_EnAdTmCount = g_OpenAdDelay;
    }
    if(g_sTimer[TM_ADBAT].m_TmFlag == ON)
    {//启动AD采集理电池
        g_sTimer[TM_ADBAT].m_TmFlag = OFF;
        Open_BatSunC_Para(OPEN_BATSUNC);//(OPEN_BAT);
        g_EnAdTmCount = g_OpenAdDelay;
        g_OpenBatFlag = ON;
    }
    if(g_EnAdTmFlag == ON)
    {//使能AD采集
        g_EnAdTmFlag = OFF; 
        if(g_OpenBatFlag == ON)
            OpenTimer(TM_ADBAT); 
            
        OpenTimer(TM_ADCOLT);
        ADC12CTL0 |= (ADC12SC);
    }
    if(g_CrystalError.Crystal_end_flag ==ON)
    {
    if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
    {
          /*BYTE bBuf[6] = {0xE6,g_ccr0,g_ccr0>>8,g_CrystalError.start_RealTime[2],g_CrystalError.start_RealTime[2]>>8,g_CrystalError.start_RealTime[1]};
      //memcpy(g_OutputInfo.g_buf,bBuf,6);
      MyPrintfD(bBuf,6);*/
        
      //MyPrintfD(g_OutputInfo.g_buf,6);
      BYTE bBuf1[13] = {0xE7,g_CrystalError.Crystal_end_num,g_CrystalError.Crystal_end_num>>8,g_CrystalError.Crystal_end_num>>16,g_CrystalError.Crystal_end_num>>24,g_CrystalError.Crystal_start_num,g_CrystalError.Crystal_start_num>>8,g_CrystalError.Crystal_start_num>>16,g_CrystalError.Crystal_start_num>>24,g_CrystalError.end_RealTime[2],g_CrystalError.end_RealTime[2]>>8,g_CrystalError.end_RealTime[1],g_CrystalError.end_RealTime[0]};
      //memcpy(g_OutputInfo.g_buf1,bBuf1,9);
      MyPrintfD(bBuf1,13);
      BYTE bBuf2[5] = {0xE8,g_CrystalError.start_RealTime[2],g_CrystalError.start_RealTime[2]>>8,g_CrystalError.start_RealTime[1],g_CrystalError.start_RealTime[0]};
      //memcpy(g_OutputInfo.g_buf2,bBuf2,5);
      MyPrintfD(bBuf2,5);
    }
    Computed_error_difference();
    }
    if(g_sTimer[TM_CLOSECHARGE].m_TmFlag == ON)
    {//关闭充电30分钟后打开充电
        g_sTimer[TM_CLOSECHARGE].m_TmFlag = OFF;
        OPEN_SUN_CHARGE;
    }
    if(g_sTimer[TM_GPRSPWOERDOWN].m_TmFlag == ON)
    {//打开GPRS5分钟后关闭电源
        g_sTimer[TM_GPRSPWOERDOWN].m_TmFlag = OFF;
        CloseGPRS(1);
    }
    if(g_sTimer[TM_SAVELOAD].m_TmFlag == ON)
    {//保存负荷数据
        g_sTimer[TM_SAVELOAD].m_TmFlag = OFF;
       // SaveLoad();
        OpenTimer(TM_SAVELOAD);
    }
    if(g_sTimer[TM_BATPWOERDOWN].m_TmFlag == ON)
    {
        g_sTimer[TM_BATPWOERDOWN].m_TmFlag = OFF;
        //CLOSE_GPRS_POWER;
        CLOSE_BAT_POWER; 
    }
  if(g_sTimer[TM_GPS_TIME].m_TmFlag == ON)
  {
    g_sTimer[TM_GPS_TIME].m_TmFlag = OFF;
    OPEN_GPS_POWER;
    OpenTimer(TM_GPS_TIME);//GPS间隔打开的时间
        OpenTimer(TM_OPENGPS_TIME);//gps打开后最长持续该时间后关闭gps
        
  }
  if(g_sTimer[TM_OPENGPS_TIME].m_TmFlag == ON)
  {
    g_sTimer[TM_OPENGPS_TIME].m_TmFlag = OFF;
    if(g_gpsPowerSt == 1)//本次打开GPS没有收到有效信息，收到有效信息后会提前关闭GPS电源
    {
      g_GPS_READ_NUM++;
      if(g_GPS_READ_NUM == g_gRunPara[RP_OPENGPS_NUM])
      {
        CreatNewSoe(RI_GPS_ERR,1);
        g_GPS_READ_NUM =0;
        OpenTimer(TM_GPS_TIME);//gps打开的时间间隔拉长至2小时
            OpenTimer(TM_OPENGPS_TIME);//gps打开后最长持续该时间后关闭gps
      }
    }
    CLOSE_GPS_POWER;
    (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.flag = FALSE;
  }
    if(g_sTimer[TM_SMSSENDALLOW].m_TmFlag == ON)
    {
        g_sTimer[TM_SMSSENDALLOW].m_TmFlag = OFF;
        g_EnSmsSend = ON;
    if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
    {
          BYTE bBuf[2] = {0xFB,0XFB};
          MyPrintfD(bBuf,2);
    }
       
   }
   if(g_sTimer[TM_WAITXINHAO].m_TmFlag == ON)
   {
       g_sTimer[TM_WAITXINHAO].m_TmFlag = OFF;
       if(g_unGroundBuffTail > 0)
       {
           for(int i = 0; i < g_unGroundBuffTail; i++)
           {
               CreatNewSoe(g_gGroundBuff[i],1);
               g_gRmtLock[g_gGroundBuff[i]]= g_gRunPara[RP_FLTLK_T];
           }
       }
       g_unGroundBuffTail = 0;
   }
   if(g_sTimer[TM_SENDSTATUSDELAY].m_TmFlag == ON)
   {
       g_sTimer[TM_SENDSTATUSDELAY].m_TmFlag = OFF;
       g_SendStatus = ON;
   }
   #ifdef ZDSENDADDR
   if(g_sTimer[TM_SENDADDR].m_TmFlag == ON)
   {
       BYTE byBuf[] = {0XFE,0XFE,0XFE,0XFE,0XFE,0x08,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
       sprintf((char *)(&(byBuf[9])),"%05d",g_gRunPara[RP_COMM_ADDR]);
       for(int i = 0; i < 9; i++)
           byBuf[14] += byBuf[5+i];
       CommSendData(byBuf,15,g_CmIdDBG);
       CommSendData(byBuf,15,g_CmIdGPRS);
       g_sTimer[TM_SENDADDR].m_TmFlag = OFF;
       OpenTimer(TM_SENDADDR); 
   }
   #endif
   if(g_sTimer[TM_SENDFAULTDELAY].m_TmFlag == ON)
   {
       g_sTimer[TM_SENDFAULTDELAY].m_TmFlag = OFF;
       g_SendFault = ON;
   }
   if(g_sTimer[TM_SHORT_FLAG].m_TmFlag == ON)
   {
      g_sTimer[TM_SHORT_FLAG].m_TmFlag =OFF;
    if(g_unShortBufftail > 0)
        {
          for(int i = 0; i < g_unShortBufftail; i++)
            {
              CreatNewSoe(g_gshortBuff[i],1);
                g_gRmtLock[g_gshortBuff[i]]= g_gRunPara[RP_FLTLK_T];
            }
        }
    g_unShortBufftail =0;
   }
   if(g_sTimer[TM_SENDSTATE].m_TmFlag == ON)
   {
       g_sTimer[TM_SENDSTATE].m_TmFlag = OFF;
       CreatNewSoe(RI_STATE_CYCLE,0);
   }
   
   if((g_sTimer[TM_SNTP_TIME].m_TmFlag == ON) && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SNTP_TIME]))
   {
        g_sTimer[TM_SNTP_TIME].m_TmFlag = OFF; 
        BYTE byBuf[] = {0x55,0XAA,0X55,0XAA,0X07,0X00,0X01,0X01,0XA8,0X8B};
        CommSendData(byBuf,10,g_CmIdGPRS);
        MyPrintfD(byBuf,10);//测试用
   }
   if((g_sTimer[TM_SNTP_TIME].m_TmCount == 0) && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SNTP_TIME]))
   {
       OpenTimer(TM_SNTP_TIME);//定时SNTP对时
   }
   if(g_sTimer[TM_GPRS_RW].m_TmFlag == ON)
   {
       g_sTimer[TM_GPRS_RW].m_TmFlag = OFF;
	   if(g_GprsPowerConsum == ON)
           {
		   if(pGprs != null) ((CPrtcSms *)pGprs)->Send_GPRS_Power (0x00,0x0F);//退出低功耗;	
	   }
           else if(g_GprsPowerConsum == OFF)
           {
	   		if(pGprs != null) ((CPrtcSms *)pGprs)->Send_GPRS_Power (0x4C,0x50);//退出低功耗
           }// g_IHDCmdRBit = 0;                    //超时以后把GPRS所有的读写命令标志位清除
      // g_IHDCmdWBit = 0;
   }
   if(g_sTimer[TM_SMS_RW].m_TmFlag == ON)
   {
       g_sTimer[TM_SMS_RW].m_TmFlag = OFF;
       g_MesReadFlag = OFF;
       g_SmsCmdCode = 0;
   }
   if(g_sTimer[TM_RECORDED].m_TmFlag == ON)
   {
      g_sTimer[TM_RECORDED].m_TmFlag = OFF;
      
        pRf->Recorder.Recorder_flag = ON;
    
    g_detephase_flag = OFF;
    for(BYTE i =0;i<3;i++)
    {
      gloss_phase_flag[i] = OFF;
      g_phase_loss[i] = OFF;
    }
    if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
    {
       BYTE byBuf[2] = {0x55,0X55};
      //CommSendData(byBuf,10,g_CmIdGPRS);
        MyPrintfD(byBuf,2);//测试用
    }
   }
   if(g_sTimer[TM_YKTRIGGER].m_TmFlag == ON)
   {
      g_sTimer[TM_YKTRIGGER].m_TmFlag = OFF;
      CLOSE_YK_TRIGGER ;
      
      /*CreatNewSoe(RI_LUBO_FLAG,1);
        //g_gRmtLock[RI_LUBO_FLAG] = g_gRunPara[RP_FLTLK_T];
      g_ucRefYxYcTs |= BIT1;
       
     g_gRmtLockLB = g_gRunPara[RP_FLTLK_T];  */ 
   }
   if(g_sTimer[TM_OPENGPRS_RF].m_TmFlag == ON)
   {
      g_sTimer[TM_OPENGPRS_RF].m_TmFlag = OFF;
      ((CBJ101S *)pGprs)->m_tra_FI = OFF ;
      ((CBJ101S *)pGprs)->RecFIUpdata(0xDD);
   }
   if(g_sTimer[TM_OPENDEG_RF].m_TmFlag == ON)
   {
      g_sTimer[TM_OPENDEG_RF].m_TmFlag = OFF;
      ((CBJ101S *)pDbg)->m_tra_FI = OFF ; 
      ((CBJ101S *)pDbg)->RecFIUpdata(0xDD);
   }
   
   if(g_OutputInfo.Interrupt_flag == ON)
   {
      g_OutputInfo.Interrupt_flag = OFF;
    //memcpy(g_OutputInfo.g_buf,bBuf,6)
    MyPrintfD(g_OutputInfo.g_buf,6);
    //MyPrintfD(g_OutputInfo.g_buf1,9);
    //MyPrintfD(g_OutputInfo.g_buf2,5);
   }
   if(g_OutputInfo.Interrupt_flag1 == ON)
   {
      g_OutputInfo.Interrupt_flag1 = OFF;
    //memcpy(g_OutputInfo.g_buf,bBuf,6)
    MyPrintfD(g_OutputInfo.g_buf1,24);
    //MyPrintfD(g_OutputInfo.g_buf1,9);
    //MyPrintfD(g_OutputInfo.g_buf2,5);
   }
   
}
//1s定时器中断与类对象中定时函数的接口
bool TimeOut(int flag)
{
      bool ret[5] = {0};
      bool RetAll = 0;
      if(pDbg != null)  ret[0] = pDbg->DoTimeOut(0);
      if(pGprs != null) ret[1] = pGprs->DoTimeOut(0);
      if(pRf != null)   ret[2] = pRf->DoTimeOut(0);
      if(pOdu != null)  ret[3] = pOdu->DoTimeOut(0);
      if(pOdu1 != null) ret[4] = pOdu1->DoTimeOut(0);
      
      RetAll = ret[0]||ret[1]||ret[2]||ret[3]||ret[4];
    return RetAll;
}

//==============================================================================
//  函数名称   : CheckRmtLock
//  功能描述   : 判断故障闭锁
//  输入参数   : 
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
//==============================================================================

  void CheckRmtLock(void)
  {
    int i = 0;
    int iTime = 0;
    //判断故障闭锁时间
    for(i = RI_1A_SHORT; i < RI_FTINFO_NUM ;i++)
    {
       if(g_gRmtLock[i] > 0)
       {
        if(g_gRunParaFltLkTBk != g_gRunPara[RP_FLTLK_T])
        {//故障闭锁的运行参数有变化
          if(i >= RI_1LVD && i <= RI_3LVD )//RI_12LVD低压告警闭锁值是其他故障的5倍
            iTime = g_gRunPara[RP_FLTLK_T]*5 - (g_gRunParaFltLkTBk*5 - g_gRmtLock[i]);
          else
            iTime = g_gRunPara[RP_FLTLK_T] - (g_gRunParaFltLkTBk - g_gRmtLock[i]);
          if(iTime < 0)  iTime = 1;
          g_gRmtLock[i] = iTime;
        }
    if(g_gShortFltLkTBk!= g_gRunPara[RP_SHORTFAULT_RESETTIME])//运行参数中短路故障复位时间备份，用于参数变化时使其立即生效)
    {
      iTime = g_gRunPara[RP_SHORTFAULT_RESETTIME] - (g_gShortFltLkTBk - g_gRmtLock[i]);
            if(iTime < 0)  iTime = 1;
            g_gRmtLock[i] = iTime;
    }
    if(g_gRmtLock10s[i] > 0)//以10S为单位
    {
      g_gRmtLock10s[i]--;
      if(g_gRmtLock10s[i] ==0)
      {
         g_gRmtLock[i]--;
         g_gRmtLock10s[i] =10;
      }
    }
        //g_gRmtLock[i]--;
        if(g_gRmtLock[i] == 0)
        {
            CreatNewSoe(i,0);
        }
       } 
    }
    if(g_gRmtLockLB > 0)//为了应付送检，录波标志位按时清0
    {
        g_gRmtLockLB--;
        if(g_gRmtLockLB == 0)
        {
            CreatNewSoe(RI_LUBO_FLAG,0);
        }
    }
    
    //判断脉冲遥信闭锁时间
    #ifdef LOCALYX
    for(i = 0; i < EXT_DIN_NUM;i++)
    {
       if(g_gDIMcLock[i] > 0)
       {
        if(g_gRunParaFltLkTBk != g_gRunPara[RP_FLTLK_T])
        {//故障闭锁的运行参数有变化
          iTime = g_gRunPara[RP_FLTLK_T] - (g_gRunParaFltLkTBk - g_gDIMcLock[i]);
          if(iTime < 0)  iTime = 1;
          g_gDIMcLock[i] = iTime;
        }
    if(g_gDIMcLock10s[i] > 0)
    {
      g_gDIMcLock10s[i]--;
      if(g_gDIMcLock10s[i] ==0)
      {
        g_gDIMcLock[i]--;
        g_gDIMcLock10s[i] =10;
        
      }
    }
        //g_gDIMcLock[i]--;
        if(g_gDIMcLock[i] == 0)
        {
            CreatNewSoe(RI_ZD_YX1 + i,0);
        }
       } 
    }
 #endif
    if(g_gRunParaFltLkTBk != g_gRunPara[RP_FLTLK_T]) 
      g_gRunParaFltLkTBk = g_gRunPara[RP_FLTLK_T];
   if(g_gShortFltLkTBk != g_gRunPara[RP_SHORTFAULT_RESETTIME]) 
      g_gShortFltLkTBk = g_gRunPara[RP_SHORTFAULT_RESETTIME];
    return;
 }
 //==============================================================================
//  函数名称   : RefreshPara
//  功能描述   :根据g_ucParaChang变化更新各对象内存中的参数，一个对象的通信参数变化时其他对象的内存区也需要更新
//  输入参数   : 
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
//==============================================================================
 void RefreshPara(void)
 {
    if(((g_ucParaChang & BIT0) == BIT0) //运行参数
    || ((g_ucParaChang & BIT2) == BIT2)//遥信地址点表
    || ((g_ucParaChang & BIT3) == BIT3))//遥测参数
    {
      
       if((g_ucParaChang & BIT2) == BIT2)
          InitTsYxInfo();
       if((g_ucParaChang & BIT3) == BIT3)
          InitTsYcInfo();
          
       if(pDbg != null)
           pDbg->SetEqpInfo();
       if(pGprs != null)
          pGprs->SetEqpInfo();
    }
    //IEC101参数设置
    if((g_ucParaChang & BIT1) == BIT1)
    {
        if(pDbg != null)
           pDbg->initpara();
       if(pGprs != null)
          pGprs->initpara();
          
       //若是端口重新分配，则需要重判断类对象的启动或停止
        BYTE bChange= 0;
       for(int i = 0;i < 4;i++)
       {
          if(g_unParaObjComBk[i] != g_ucPara101[IECP_OBJ_COM0 + i])
          {
            bChange = 1;
            break;
           }
       }
       if(bChange == 1)
          InitCommObj();
       
    }
    
    return;
 }

 void GPS_Config(BYTE Type)
 {
   char ph[50];
   unsigned char GPS_CRC=0;
   BYTE SendBuf[50];
   BYTE wWritePtr=0;
  BYTE j;
   switch  (Type)
   {
    case GPS_PCAS00://保存配置信息
       sprintf((char *)ph,"$PCAS00");
       for(j = 0; j < strlen(ph); j++)
        SendBuf[ wWritePtr++] = ph[j];
    break;
    case GPS_PCAS03://设置要求输出或停止输出的NMEA语句
       sprintf((char *)ph,"$PCAS03,0,0,0,0,1,0,0,0");//"$PCAS03,1,0,0,0,0,0,0,0");//
       for(j = 0; j < strlen(ph); j++)
       SendBuf[wWritePtr++] = ph[j];
     break;
     case GPS_PCAS04://配置工作系统GPS BDS GLONAS
       sprintf((char *)ph,"$PCAS04,3");//北斗和GPS双模
       for(j = 0; j < strlen(ph); j++)
       SendBuf[wWritePtr++] = ph[j];
     break;
     case GPS_CCRM0://开始或停止输出NMEA语句
       sprintf((char *)ph,"$CCRM0,GGA,3,1");//关闭全部语句
       for(j = 0; j < strlen(ph); j++)
       SendBuf[ wWritePtr++] = ph[j];
     break;
     default:
       break;
   }
     GPS_CRC = XorChar(&SendBuf[1], strlen(ph)-1);
     SendBuf[wWritePtr++]  = 0x2A;
     
     SendBuf[ wWritePtr++]= HexToChar(GPS_CRC >> 4);
     SendBuf[ wWritePtr++] = HexToChar(GPS_CRC & 0x0F);
     SendBuf[ wWritePtr++]= 0X0D;
     SendBuf[ wWritePtr++]= 0X0A;
     CommSendData((BYTE *)SendBuf,wWritePtr,g_CmIdGps);
           //CommSendData((BYTE *)SendBuf,wWritePtr,g_CmIdDBG);
     //CommWrite((char *)m_SendBuf.pBuf,m_SendBuf.wWritePtr);
     delayms(1000);
 }

 //==============================================================================
 //  函数名称 : InitGPS
 //  功能描述 : GPS配置初始化
 //  输入参数 : <无>
 //          
 //  输出参数 ：<无>
 //  返回值   : <无>
 //  其他说明 : 
 //  作者   ：
 //==============================================================================

 void InitGps(void)
 {
  
  if((g_gRunPara[RP_CFG_KEYH]& (BIT[RP_OFF_GPS-16])) ==0)
   {
        OPEN_GPS_POWER;
        delay_1ms(1000);//delayms(3000);//电源稳定后再配置GPS模块，否则容易配置不成功
    GPS_Config(GPS_PCAS04);
    GPS_Config(GPS_CCRM0);
    GPS_Config(GPS_PCAS03);
    GPS_Config(GPS_PCAS00);
    //if((g_gRunPara[RP_CFG_KEYH])&(BIT[RP_OFF_GPS-16]))
    //CLOSE_GPS_POWER;
        
   
        //OPEN_GPS_POWER;
        OpenTimer(TM_GPS_TIME);
        OpenTimer(TM_OPENGPS_TIME);//gps打开后持续该时间后关闭gps
    }
 }
//==============================================================================
//  函数名称   : InitGPRS
//  功能描述   : GPRS配置初始化
//  输入参数   : <无>
//                
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================

void InitGPRS(void)
{
  GPRS_SETA_L();
  GPRS_RST_L();
  delay_1ms(200);
  switch(g_gRunPara[RP_POWER_MODE])
  {
  case QUASI_REALTIME:
              CLOSE_GPRS_POWER;//CloseGPRS();//去掉这里的函数，目的是为了不让程序一启动就发调试报文
              g_EnSmsSend = OFF;
            break;
  case REALTIME:
            if(g_gRunPara[Terminal_Type]!=AERIAL_TERMINAL)
            {
              CLOSE_GPRS_POWER;//CloseGPRS();
              g_EnSmsSend = OFF;
            }
            else
            {
              OPEN_GPRS_POWER;//OpenGPRS();
              if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
                  g_EnSmsSend = ON;              
            }
            break;
  default:
            CLOSE_GPRS_POWER;//CloseGPRS();
            g_EnSmsSend = OFF;
            break;
  }
  delay_1ms(2000);
  GPRS_SETA_H();
  GPRS_ONOFF_IN();
  GPRS_RST_H();   
}
void Computed_error_difference()//通过邋錑PS与自己记得晶振个数计算需要补偿的晶振个数
{
    DWORD start_time,end_time,diff_time;
  DWORD GPS_crynum =0;//gps走过的晶振个数
  DWORD diff_crynum =0;//GPS与自己记得晶振走过的时间差
  DWORD cry_total =0;
    if(g_CrystalError.start_RealTime[0] > g_CrystalError.end_RealTime[0])//起始分钟大于结束分钟
      end_time = (g_CrystalError.end_RealTime[0]+60)*60 + g_CrystalError.end_RealTime[1];//每次秒脉冲来时对应的毫秒固定位999，计算差值时开始和结束都不用计算了  
  else
    end_time = g_CrystalError.end_RealTime[0]*60 + g_CrystalError.end_RealTime[1];
  start_time = g_CrystalError.start_RealTime[0]*60 + g_CrystalError.start_RealTime[1];
   
  if(end_time >= start_time)
    diff_time =end_time -start_time;
  else
    diff_time =0;
   GPS_crynum = diff_time << 15;// diff_time*32768;
  
  if(GPS_crynum > g_CrystalError.Crystal_end_num)
  {
    diff_crynum = GPS_crynum - g_CrystalError.Crystal_end_num;
    g_CrystalError.Compen_cry_flag = ON;
  }
  else
  {
    diff_crynum =  g_CrystalError.Crystal_end_num - GPS_crynum;
    g_CrystalError.Compen_cry_flag = OFF;
  }
  if(diff_crynum < 327680)//GPS与自己晶振的误差小于10S进行补偿，如果相差很大就不做补偿了
  {
    cry_total = (DWORD)((diff_time)*1000)/diff_crynum;//正常大于1s才会补偿
    if(cry_total >1000)//按1000计算得出1min约有60个晶振的误差，大约为2个ms.补偿结果正确，按这次计算的结果补偿，补偿计算错误，按上一次的补偿补，
      g_CrystalError.Compen_cry_total = cry_total;//(DWORD)((diff_time)*1000)/diff_crynum;
  }
  else
  {
    g_CrystalError.Compen_cry_total =0;
    g_CrystalError.Crystal_start_flag =0;
  }

  //if(g_CrystalError.Compen_cry_total <=1000)//补偿计算错误，不做补偿了，因为每1s的误差不会有
    //g_CrystalError.Compen_cry_total =0;
  BYTE bBuf2[9] = {0xE9,g_CrystalError.Compen_cry_total,g_CrystalError.Compen_cry_total>>8,g_CrystalError.Compen_cry_total>>16,g_CrystalError.Compen_cry_total>>24,diff_crynum,diff_crynum>>8,diff_crynum>>16,diff_crynum>>24};
      
  MyPrintfD(bBuf2,9);
    g_CrystalError.start_RealTime[0] = g_CrystalError.end_RealTime[0];
  g_CrystalError.start_RealTime[1] = g_CrystalError.end_RealTime[1];
  g_CrystalError.start_RealTime[2] =  g_CrystalError.end_RealTime[2];
    
  g_CrystalError.Crystal_end_flag = OFF;
}
void CloseGPRS(BYTE bDa,BYTE Close_Type)
{
  if((Close_Type==1)||(g_gRunPara[RP_POWER_MODE] != REALTIME))
  {
    CLOSE_GPRS_POWER; 
    CLOSE_BAT_POWER; //任何时候，只要GPRS关闭了，电池都该关闭
    g_EnSmsSend = OFF;
  }
  if(pGprs != null)
  {
      pGprs->m_initflag = 0;//7
      pGprs->m_linkflag=0;   /*断开链路*/
      pGprs->m_zdflag=0;
  }
  if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
  {
     BYTE bBuf[3] = {0xEC,0XEC,0XEC};
    if(bDa == 1) bBuf[2] = 0XE1;
    if(bDa == 2) bBuf[2] = 0XE2;
    if(Close_Type == 1)bBuf[2] = 0XE3;
     MyPrintfD(bBuf,3);
  }

}
void OpenGPRS(BYTE bDa)
{
  OPEN_GPRS_POWER;  
     if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
     {
    BYTE bBuf[3] = {0xEA,0XEA,0XEA};
    if(bDa == 1) bBuf[2] = 0XE1;
    else if(bDa == 2) bBuf[2] = 0XE2;
    else if(bDa == 3) bBuf[2] = 0XE3;
     MyPrintfD(bBuf,3);
     }
  
}
//判断GPRS通道是否有数据需要上传
//1 :有数据未上传
//0: 无数据上传
//提前关GPRS时需要此函数进行判断，发送提前关主板电源指令到ODU也需要此判断
BOOL CheckHaveDataToSd()
{
   if(pGprs == null)
    return 0;//GPRS通道都没配置，返回无数据要上传

  if((pGprs->m_WaitConfTpId == M_SP_NA) || (pGprs->m_WaitConfTpId == M_DP_NA)
    || (pGprs->m_WaitConfTpId == M_SP_TB) || (pGprs->m_WaitConfTpId == M_DP_TB)
    || g_ucRefYxYcTs || pGprs->m_SendYcFlag || pGprs->m_SendChgYcFlag
    || pGprs->m_reSendDelay || pGprs->m_resendflag
    || (g_yxChangeflag & 0x03c0))// 0011 1100 0000  SSMS,DSMS标识位不能加入，因为不一定发短信
     return 1;

  if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
  {
    if( g_SmsReSendWaitCount || g_sTimer[TM_WAITXINHAO].m_TmCount 
      || g_sTimer[TM_SMSSENDALLOW].m_TmCount || (g_EnSmsSend == ON))
        return 1;
   }

    
  return 0;
}

#endif
