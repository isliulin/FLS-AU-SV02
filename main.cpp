#ifndef _MAIN

#define _MAIN
#include "DataStruct.h"
#include "Main.h"

 CBJ101S mBJ101S0;
 CBJ101S mBJ101S1;
 //CPrtcOdu mPrtcOdu;
 //CPrtcOdu mPrtcOdu1;
 CPrtcGPS mPrtcGPS;//�����ӵ���
 CPrtcRF mPrtcRf;
#ifdef USE_GB104S
 CGB104S mGB104S;
#endif
WORD g_LEN;

void app(void)@"APPLICATION"
{
    WDTCTL = WDTPW + WDTHOLD;//��ֹоƬ�Դ��Ŀ��Ź��������ⲿ���Ź�
    InitSys();
    //strcpy(ComtrderCfg1,ComtrderCfg);
  //char *pdat_name;
  //pdat_name = "8,Un,N,,V,1.000000,0.000000,0.000000,-8192,8191,1,1,S\r\n";
   //g_len = strlen(pdat_name); 
    //InitCommObj();
    //ReplySuccess(); //�����ɹ�����Զ�������й�

    //����ʱȡ������������1������
    CAT_SpiReadWords(EEPADD_RSTARTNUM, 1, &g_RstartNum);
    if(g_RstartNum >= 0xfffe) g_RstartNum = 0;
    
    g_RstartNum++;
    CAT_SpiWriteWords(EEPADD_RSTARTNUM, 1, &g_RstartNum);
    CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &g_FRecorder_Current_COUNT);//�ȶ�������ֹ�ϵ���0
     
    CAT_SpiReadWords(EEPADD_LUBONUM, 1, &(g_Recorder_Total_COUNT));//�ȶ�������ֹ�ϵ���0
    if((g_FRecorder_Current_COUNT >= 0xfffe)||(g_Recorder_Total_COUNT>= 0xfffe))
    {
        g_FRecorder_Current_COUNT =0;
  CAT_SpiWriteWords(EEPADD_LUBOFLAG, 1, &g_FRecorder_Current_COUNT);
  g_Recorder_Total_COUNT =0;
        CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &(g_Recorder_Total_COUNT));
    }
    SetLbNum();
    g_LogType |= BIT0;//������¼
    
    CAT_SpiReadBytes(EEPADD_VerNum, strlen((char *)VerNum), g_VerNum);
    if((g_VerNum[0] ==0xff)||(g_VerNum[1] ==0xff))//�汾��ǰ������ĸΪSV
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
      if(qq == 0x55)//�ڲ�flash������û�����ʱ����ȡSOE�ĸ���
        CAT_SpiReadWords(EEPADD_SOE_NUM, 1, &soe_num);
    #endif
    //gRecorder_cfg[0].CFG_Leng=0x55;
    //unsigned int gRecorder_cf=0x55;
    
    //Sector_Erase(FADDR_SOE_START);
    //Sst26vf064b_Page_WriteBytes(FADDR_SOE_START+0*RECORDER_INFO_LENGTH,(unsigned char *)&(gRecorder_cf),2);
    //Sst26vf064b_Read(FADDR_SOE_START+0*RECORDER_INFO_LENGTH,(unsigned char *)&gRecorder_cf,2);
    /*unsigned int avalue = 0;
  
    CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &avalue);//����*/

    g_LEN = strlen(ComtrderCfg); 
    while(1)
    {
        WatchDog();
        HandleTimerEvent();
        CalcuRmtMeas();//����AD�ɼ����� 
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
        if(g_sRtcManager.m_gRealTimerFlag ==TRUE)//��վ�����ʱ����󣬵���MS=1000ʱ��д��RTCоƬ��ʱ��
        {
            WriteRTCTime();//��ϵͳʱ��д��RTCоƬ��
            g_sRtcManager.m_gRealTimerFlag = OFF;
        }
        if((g_sRtcManager.m_ucRtcSynFlag == YES) &&(g_sRtcManager.m_gRealTimerFlag == OFF))//ÿ4���Ӵ�ʱ��оƬ��ȡһ��ʱ�䣬�ñ�־�ڶ�ʱ���ж��м������־
        {
            ReadRealTime(1);//��RTCоƬ�ж�ȡϵͳʱ�ӣ�������SysTime�е�ʱ��
            g_sRtcManager.m_ucRtcSynFlag = NO;
        }
    
        if(!(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISDDMODE]))
        {//�ڶԵ�ģʽ�²�����ʵ������
            if(pRf != null) pRf->Run();
            if(pOdu != null) pOdu->Run();
            if(pOdu1 != null) pOdu1->Run();
            if(pGps != null) pGps->Run();
        }
        //BYTE byBuf1[20] = {0XAA,0XAA,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11};  //����ʹ��
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
         LPM3;//����͹���
          }
        }
        
    }//end of while(1)
    

}

//==============================================================================
//  ��������   : CheckInLPM3
//  ��������   : �ж��Ƿ����͹���
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : TRUE = �ɽ���͹��� FALSE = �����¼�δ�������ܽ���͹���
//  ����˵��   : 
//  ����       ��
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
//  ��������   : InitCommObj
//  ��������   : ��ʼ���˿ڶ���
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : 
//  ����˵��   : 
//  ����       ��
//==============================================================================
void InitCommObj(void)
{
  //��ʼ��ͨ�������������ڷ������==============================================
    //���ͨ�������ô��󣬿��Խ�DEBUG��ȥ�����ó�������ʽ����״̬��
    //��״̬�µ��Դ���Ĭ�ϱ����䵽com3
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
      g_CmIdDBG = CMID3;//����ʱ����ͨ��Ĭ�����õ�����3,���ϲ��������е�ֵ
     #endif
      
  //========================================================================
     pDbg = null;
     pGprs = null;
     pRf = null;
     pOdu = null;
     pOdu1 = null;
     pGps  = null;
    int iCmId = 0;
    
    //���Կ�
    iCmId = g_CmIdDBG;
    if( iCmId != CMIDINV)
    {
      pDbg = &mBJ101S0;
      pDbg->Init(iCmId);
    }

    //GPRS��
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
    //GPSĬ�����õ���չ��
    iCmId = g_CmIdGps;
   
      if( iCmId != CMIDINV)
      {
        pGps = &mPrtcGPS;
        pGps->Init(iCmId);
      }
   
    
    //RF�ں�ODU0���ã���չ���ں�ODU1����
    if(g_gRunPara[Terminal_Type] == CABLE_TERMINAL_DODU)
    {//����˫ODU
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
    {//��ODU
      iCmId = g_CmIdODU1;
      /*if(iCmId != CMIDINV)
      {
        pOdu = &mPrtcOdu;
        pOdu->Init(iCmId);
      }*/
    }
    else
    {//�ܿջ��ߵ���RF
      iCmId = g_CmIdRF;
      if(iCmId != CMIDINV)
      {
        pRf= &mPrtcRf;
        pRf->Init(iCmId);
      }
    }

   //�˿�Э���4���������б���
    for(int i = 0;i < 4;i++)
        g_unParaObjComBk[i] = g_ucPara101[IECP_OBJ_COM0 + i];

  #ifndef GETSOEFROMRAM
   ReadSoeInfoFromFlash();//��ȡ���浽�ⲿflash�е�SOE��Ϣ
  #endif
    return;
}

//==============================================================================
//  ��������   : InitSys
//  ��������   : CPU�������г�ʼ�������������������衢������ģ��ĳ�ʼ������ʼ�������������жϣ������ѭ��
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void InitSys(void)
{
    _DINT();  
    IoInit();//IO�˿ڳ�ʼ��
    InitClk();//ϵͳʱ�ӳ�ʼ��
    LIGHT_RUN_DOWN(); 
  LIGHT_RUN2_DOWN();
    InitAdc(); 
    WIRE_LESS_RST_L(); //��λ����ģ��
    CLOSE_DI_POWER();//�ر�DI�ɼ���Դ
    CLOSE_BAT_POWER;
    WatchDog();
    
    
    ReadRealTime(0);    //����ȫ��RTC����
    UCB0SPIInit();  //SPI���߳�ʼ��
    InitcfgPara();//ϵͳ������ʼ��������ϵͳ�õ��Ĳ�����ʼ���ʹӴ洢���ж�ȡ�����Ĳ�����
    InitRtc();//ϵͳʱ�ӳ�ʼ��
    InitComm();//ͨ�ų�ʼ��,����RFͨ�ź�GPRSͨ�š�232ͨ�š�  //�����ڲ�����ʼ��֮����Ϊ���õ����в���

    InitDataProc();
    
    //InitDin();//��ʼ��DI
    DisAd0Sample();//��ֹAD�ɼ�
    InitTimer();//��ʱ����ʼ��
    
     OPEN_WIRE_POWER;   //������ģ���Դ
    OPEN_SUN_CHARGE;   //��̫���ܳ��
    
    WIRE_LESS_RST_H(); //��������ģ�鸴λ��Ϊ��
    InitGPRS();
    ReadRealTime(2); //��ʼ��ʱ��RTC��ȡʱ����µ�ʵʱʱ��
    InitCommObj();
    
    //WDTCTL = WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2;//����WDT
    _EINT(); 
    InitGps();
  if(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL)
  {
      g_gRunPara[RP_POWER_MODE] = QUASI_REALTIME;
      SetRIStatus(RI_REAL_QUASI,1); 
      g_ucRefYxYcTs |= BIT0;
      
  }
  #if 1
    BYTE byBuf1[2] = {0XAA,0XAA};  //����ʹ��
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
    {//����AD�ɼ�λ
        g_sTimer[TM_ADCOLT].m_TmFlag = OFF;
        Open_BatSunC_Para(OPEN_SUNC);
        g_EnAdTmCount = g_OpenAdDelay;
    }
    if(g_sTimer[TM_ADBAT].m_TmFlag == ON)
    {//����AD�ɼ�����
        g_sTimer[TM_ADBAT].m_TmFlag = OFF;
        Open_BatSunC_Para(OPEN_BATSUNC);//(OPEN_BAT);
        g_EnAdTmCount = g_OpenAdDelay;
        g_OpenBatFlag = ON;
    }
    if(g_EnAdTmFlag == ON)
    {//ʹ��AD�ɼ�
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
    {//�رճ��30���Ӻ�򿪳��
        g_sTimer[TM_CLOSECHARGE].m_TmFlag = OFF;
        OPEN_SUN_CHARGE;
    }
    if(g_sTimer[TM_GPRSPWOERDOWN].m_TmFlag == ON)
    {//��GPRS5���Ӻ�رյ�Դ
        g_sTimer[TM_GPRSPWOERDOWN].m_TmFlag = OFF;
        CloseGPRS(1);
    }
    if(g_sTimer[TM_SAVELOAD].m_TmFlag == ON)
    {//���渺������
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
    OpenTimer(TM_GPS_TIME);//GPS����򿪵�ʱ��
        OpenTimer(TM_OPENGPS_TIME);//gps�򿪺��������ʱ���ر�gps
        
  }
  if(g_sTimer[TM_OPENGPS_TIME].m_TmFlag == ON)
  {
    g_sTimer[TM_OPENGPS_TIME].m_TmFlag = OFF;
    if(g_gpsPowerSt == 1)//���δ�GPSû���յ���Ч��Ϣ���յ���Ч��Ϣ�����ǰ�ر�GPS��Դ
    {
      g_GPS_READ_NUM++;
      if(g_GPS_READ_NUM == g_gRunPara[RP_OPENGPS_NUM])
      {
        CreatNewSoe(RI_GPS_ERR,1);
        g_GPS_READ_NUM =0;
        OpenTimer(TM_GPS_TIME);//gps�򿪵�ʱ����������2Сʱ
            OpenTimer(TM_OPENGPS_TIME);//gps�򿪺��������ʱ���ر�gps
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
        MyPrintfD(byBuf,10);//������
   }
   if((g_sTimer[TM_SNTP_TIME].m_TmCount == 0) && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SNTP_TIME]))
   {
       OpenTimer(TM_SNTP_TIME);//��ʱSNTP��ʱ
   }
   if(g_sTimer[TM_GPRS_RW].m_TmFlag == ON)
   {
       g_sTimer[TM_GPRS_RW].m_TmFlag = OFF;
	   if(g_GprsPowerConsum == ON)
           {
		   if(pGprs != null) ((CPrtcSms *)pGprs)->Send_GPRS_Power (0x00,0x0F);//�˳��͹���;	
	   }
           else if(g_GprsPowerConsum == OFF)
           {
	   		if(pGprs != null) ((CPrtcSms *)pGprs)->Send_GPRS_Power (0x4C,0x50);//�˳��͹���
           }// g_IHDCmdRBit = 0;                    //��ʱ�Ժ��GPRS���еĶ�д�����־λ���
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
        MyPrintfD(byBuf,2);//������
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
//1s��ʱ���ж���������ж�ʱ�����Ľӿ�
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
//  ��������   : CheckRmtLock
//  ��������   : �жϹ��ϱ���
//  �������   : 
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================

  void CheckRmtLock(void)
  {
    int i = 0;
    int iTime = 0;
    //�жϹ��ϱ���ʱ��
    for(i = RI_1A_SHORT; i < RI_FTINFO_NUM ;i++)
    {
       if(g_gRmtLock[i] > 0)
       {
        if(g_gRunParaFltLkTBk != g_gRunPara[RP_FLTLK_T])
        {//���ϱ��������в����б仯
          if(i >= RI_1LVD && i <= RI_3LVD )//RI_12LVD��ѹ�澯����ֵ���������ϵ�5��
            iTime = g_gRunPara[RP_FLTLK_T]*5 - (g_gRunParaFltLkTBk*5 - g_gRmtLock[i]);
          else
            iTime = g_gRunPara[RP_FLTLK_T] - (g_gRunParaFltLkTBk - g_gRmtLock[i]);
          if(iTime < 0)  iTime = 1;
          g_gRmtLock[i] = iTime;
        }
    if(g_gShortFltLkTBk!= g_gRunPara[RP_SHORTFAULT_RESETTIME])//���в����ж�·���ϸ�λʱ�䱸�ݣ����ڲ����仯ʱʹ��������Ч)
    {
      iTime = g_gRunPara[RP_SHORTFAULT_RESETTIME] - (g_gShortFltLkTBk - g_gRmtLock[i]);
            if(iTime < 0)  iTime = 1;
            g_gRmtLock[i] = iTime;
    }
    if(g_gRmtLock10s[i] > 0)//��10SΪ��λ
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
    if(g_gRmtLockLB > 0)//Ϊ��Ӧ���ͼ죬¼����־λ��ʱ��0
    {
        g_gRmtLockLB--;
        if(g_gRmtLockLB == 0)
        {
            CreatNewSoe(RI_LUBO_FLAG,0);
        }
    }
    
    //�ж�����ң�ű���ʱ��
    #ifdef LOCALYX
    for(i = 0; i < EXT_DIN_NUM;i++)
    {
       if(g_gDIMcLock[i] > 0)
       {
        if(g_gRunParaFltLkTBk != g_gRunPara[RP_FLTLK_T])
        {//���ϱ��������в����б仯
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
//  ��������   : RefreshPara
//  ��������   :����g_ucParaChang�仯���¸������ڴ��еĲ�����һ�������ͨ�Ų����仯ʱ����������ڴ���Ҳ��Ҫ����
//  �������   : 
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================
 void RefreshPara(void)
 {
    if(((g_ucParaChang & BIT0) == BIT0) //���в���
    || ((g_ucParaChang & BIT2) == BIT2)//ң�ŵ�ַ���
    || ((g_ucParaChang & BIT3) == BIT3))//ң�����
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
    //IEC101��������
    if((g_ucParaChang & BIT1) == BIT1)
    {
        if(pDbg != null)
           pDbg->initpara();
       if(pGprs != null)
          pGprs->initpara();
          
       //���Ƕ˿����·��䣬����Ҫ���ж�������������ֹͣ
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
    case GPS_PCAS00://����������Ϣ
       sprintf((char *)ph,"$PCAS00");
       for(j = 0; j < strlen(ph); j++)
        SendBuf[ wWritePtr++] = ph[j];
    break;
    case GPS_PCAS03://����Ҫ�������ֹͣ�����NMEA���
       sprintf((char *)ph,"$PCAS03,0,0,0,0,1,0,0,0");//"$PCAS03,1,0,0,0,0,0,0,0");//
       for(j = 0; j < strlen(ph); j++)
       SendBuf[wWritePtr++] = ph[j];
     break;
     case GPS_PCAS04://���ù���ϵͳGPS BDS GLONAS
       sprintf((char *)ph,"$PCAS04,3");//������GPS˫ģ
       for(j = 0; j < strlen(ph); j++)
       SendBuf[wWritePtr++] = ph[j];
     break;
     case GPS_CCRM0://��ʼ��ֹͣ���NMEA���
       sprintf((char *)ph,"$CCRM0,GGA,3,1");//�ر�ȫ�����
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
 //  �������� : InitGPS
 //  �������� : GPS���ó�ʼ��
 //  ������� : <��>
 //          
 //  ������� ��<��>
 //  ����ֵ   : <��>
 //  ����˵�� : 
 //  ����   ��
 //==============================================================================

 void InitGps(void)
 {
  
  if((g_gRunPara[RP_CFG_KEYH]& (BIT[RP_OFF_GPS-16])) ==0)
   {
        OPEN_GPS_POWER;
        delay_1ms(1000);//delayms(3000);//��Դ�ȶ���������GPSģ�飬�����������ò��ɹ�
    GPS_Config(GPS_PCAS04);
    GPS_Config(GPS_CCRM0);
    GPS_Config(GPS_PCAS03);
    GPS_Config(GPS_PCAS00);
    //if((g_gRunPara[RP_CFG_KEYH])&(BIT[RP_OFF_GPS-16]))
    //CLOSE_GPS_POWER;
        
   
        //OPEN_GPS_POWER;
        OpenTimer(TM_GPS_TIME);
        OpenTimer(TM_OPENGPS_TIME);//gps�򿪺������ʱ���ر�gps
    }
 }
//==============================================================================
//  ��������   : InitGPRS
//  ��������   : GPRS���ó�ʼ��
//  �������   : <��>
//                
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================

void InitGPRS(void)
{
  GPRS_SETA_L();
  GPRS_RST_L();
  delay_1ms(200);
  switch(g_gRunPara[RP_POWER_MODE])
  {
  case QUASI_REALTIME:
              CLOSE_GPRS_POWER;//CloseGPRS();//ȥ������ĺ�����Ŀ����Ϊ�˲��ó���һ�����ͷ����Ա���
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
void Computed_error_difference()//ͨ�����GPS���Լ��ǵþ������������Ҫ�����ľ������
{
    DWORD start_time,end_time,diff_time;
  DWORD GPS_crynum =0;//gps�߹��ľ������
  DWORD diff_crynum =0;//GPS���Լ��ǵþ����߹���ʱ���
  DWORD cry_total =0;
    if(g_CrystalError.start_RealTime[0] > g_CrystalError.end_RealTime[0])//��ʼ���Ӵ��ڽ�������
      end_time = (g_CrystalError.end_RealTime[0]+60)*60 + g_CrystalError.end_RealTime[1];//ÿ����������ʱ��Ӧ�ĺ���̶�λ999�������ֵʱ��ʼ�ͽ��������ü�����  
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
  if(diff_crynum < 327680)//GPS���Լ���������С��10S���в�����������ܴ�Ͳ���������
  {
    cry_total = (DWORD)((diff_time)*1000)/diff_crynum;//��������1s�ŻᲹ��
    if(cry_total >1000)//��1000����ó�1minԼ��60�����������ԼΪ2��ms.���������ȷ������μ���Ľ������������������󣬰���һ�εĲ�������
      g_CrystalError.Compen_cry_total = cry_total;//(DWORD)((diff_time)*1000)/diff_crynum;
  }
  else
  {
    g_CrystalError.Compen_cry_total =0;
    g_CrystalError.Crystal_start_flag =0;
  }

  //if(g_CrystalError.Compen_cry_total <=1000)//����������󣬲��������ˣ���Ϊÿ1s��������
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
    CLOSE_BAT_POWER; //�κ�ʱ��ֻҪGPRS�ر��ˣ���ض��ùر�
    g_EnSmsSend = OFF;
  }
  if(pGprs != null)
  {
      pGprs->m_initflag = 0;//7
      pGprs->m_linkflag=0;   /*�Ͽ���·*/
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
//�ж�GPRSͨ���Ƿ���������Ҫ�ϴ�
//1 :������δ�ϴ�
//0: �������ϴ�
//��ǰ��GPRSʱ��Ҫ�˺��������жϣ�������ǰ�������Դָ�ODUҲ��Ҫ���ж�
BOOL CheckHaveDataToSd()
{
   if(pGprs == null)
    return 0;//GPRSͨ����û���ã�����������Ҫ�ϴ�

  if((pGprs->m_WaitConfTpId == M_SP_NA) || (pGprs->m_WaitConfTpId == M_DP_NA)
    || (pGprs->m_WaitConfTpId == M_SP_TB) || (pGprs->m_WaitConfTpId == M_DP_TB)
    || g_ucRefYxYcTs || pGprs->m_SendYcFlag || pGprs->m_SendChgYcFlag
    || pGprs->m_reSendDelay || pGprs->m_resendflag
    || (g_yxChangeflag & 0x03c0))// 0011 1100 0000  SSMS,DSMS��ʶλ���ܼ��룬��Ϊ��һ��������
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
