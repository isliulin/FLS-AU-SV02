#ifndef  RECORDER_WAVE_GLOBALS

#define  RECORDER_WAVE_GLOBALS
  #include "DataStruct.h"
  #include "Main.h"
#endif

/*extern char ComtrderCfg1[]=
"The Recording File of Indicator 108,1999\n\
8,8A,0D\n\
1,Ia,A,,A,1,0,0,0,65535,1,1,S\n\
2,Ib,B,,A,1,0,0,0,65535,1,1,S\n\
3,Ic,C,,A,1,0,0,0,65535,1,1,S\n\
4,I0,0,,A,1,0,0,0,65535,1,1,S\n\
5,Ua,A,,V,1,0,0,0,65535,1,1,S\n\
6,Ub,B,,V,1,0,0,0,65535,1,1,S\n\
7,Uc,C,,V,1,0,0,0,65535,1,1,S\n\
8,U0,0,,V,1,0,0,0,65535,1,1,S\n\
50\n\
1\n\
4000,1200\n\
10/03/15,12:20:27.672\n\
10/03/15,12:20:27.772\n\
ASCII\n\
1";*/

//static unsigned char ReadBack[256] = {0};//��ȡ�ļ��е���Ϣ

static unsigned int  wSendCFGNum = 0;////�Ѿ����͵������ļ����ֽ��������ƶ�֡������
static WORD  wSendDATNum = 0;////�Ѿ����͵������ļ����ֽ��������ƶ�֡������
CBJ101S *g_Cur101Obj = NULL;


unsigned char ChkluboSum(BYTE *buf, unsigned int len)
 {
    WORD checksum, i;

    checksum = 0;
    for (i = 0; i < len; i++)
    {
        checksum = checksum + *buf;
        buf ++;
    }
    return LOBYTE(checksum & 0xff);
 }
unsigned int Calculate_offset(BYTE * Type,WORD FileNum)//,unsigned int Offset_fault
{
  WORD ADDR_LEN;
  unsigned int offset_phase =0xFFFF;
        unsigned int Offset_fault;
  unsigned int DAT_FAULT;
  DAT_FAULT = DAT_FAULT_NUM;//�������ܲ�һ���ǹ��Ϻ�Ĳ�
  for(BYTE i =0;i<3;i++)//ABC����
  {
    if(Type[i] <4)//123��Ӧ�����������糡�½�����������,�ҵ��κ�һ������˳����Ͱ��������
    {
      if( i == 0 )
        ADDR_LEN = EEPADD_ALEN;
      else if(i ==1)
        ADDR_LEN = EEPADD_BLEN;
      else
        ADDR_LEN = EEPADD_CLEN;
      CAT_SpiReadWords(ADDR_LEN + FileNum*EELEN_LENG, 1, &offset_phase);
      break;
    }
    //else
      //offset_phase =0;
  
  }
    if(offset_phase < DAT_FAULT)//(offset_phase >0) &&
      Offset_fault = DAT_FAULT- offset_phase;
    else
      Offset_fault = 470;//��ʱû���ϵ������¼������ʱ�̶���5���벨
        return Offset_fault;
}

//***********************************************************************************
//*��������cfg_dat_length
//*���ܣ����ڡ�CFG�ļ��Ƕ�������ݣ���Ҫ�Զ����.CFG�ļ��޸���¼����ʼ��ʱ���¼��������ʱ��
//*����: ��������.CFG�ĳ���
//***********************************************************************************

void cfg_dat_length(RECORDER_CFG *pgRecorder_cfg,WORD FileNum)
{
  char ch[100]={0};
  char ch1[10]={0};
     char *pdat_type;
     unsigned int time_start,time_end,dat_start,offset_start,type_offset;
   unsigned int lubo_time=0;
   unsigned int  offset_Max =0;
   unsigned int  offset_time,offset_fault;
     unsigned int DAT_A,DAT_MAX; //DAT_FAULT,
     RECORDER_CFG recorder_cfg;
   BYTE type[3];
    {  //.CFG�ĳ���

    CAT_SpiWriteBytes(EEPADD_CFG, strlen(ComtrderCfg), (unsigned char*) ComtrderCfg);
    // unsigned char group;
      // CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, (unsigned int *)&group);
      for(int j =0;j<3;j++)//��CFG�ļ��м���ABC¼��������ԭ��0 û�յ�1 �糡����2 �½�3����ͻ��4ͬ��5ң��
      {
          long FADDR_RECORDER = FADDR_RECORDER_INFO +(long)FileNum*(long)FLINEADDR + (long)j*(long)FPHASEADDR;
          Sst26vf064b_Read(FADDR_RECORDER ,(unsigned char *)&recorder_cfg,sizeof(recorder_cfg)); //�������ﱣ��gRecorder_cfg��ֵ����Ϊ�����¼����һ�����ܴ�����        
          if(recorder_cfg.lubo_flag==0x55)
          {
            if(j ==0)
          type[0] = recorder_cfg.lubo_type;
        else if(j ==1)
          type[1]  = recorder_cfg.lubo_type;
        else 
          type[2]  = recorder_cfg.lubo_type;
      }
      else 
      {
        if(j ==0)
          type[0]  = 0;
        else if(j ==1)
          type[1]  =0;
        else 
          type[2]  = 0;
      }
            
      }
       DAT_A =  DAT_AD_NUM;
    //DAT_FAULT = DAT_FAULT_NUM;
       DAT_MAX = DAT_MAX_OFFSET;
      /*sprintf((char *)ch,"%02d/%02d/%04d,%02d:%02d:%02d.%03d\n",pgRecorder_cfg->comtrade_time[RTC_DATE],pgRecorder_cfg->comtrade_time[RTC_MONTH],
                                                                pgRecorder_cfg->comtrade_time[RTC_YEAR],pgRecorder_cfg->comtrade_time[RTC_HOUR],
                                                                pgRecorder_cfg->comtrade_time[RTC_MINUT],pgRecorder_cfg->comtrade_time[RTC_SEC],pgRecorder_cfg->comtrade_time[RTC_MICROSEC]);*/

      //gRecorder_cfg[file_current_num].CFG_Leng = strlen(ComtrderCfg1);//+strlen(ComtrderCfg2)+strlen(ch)*2;
     
    CAT_SpiReadWords(EEPADD_COMLEN + FileNum*EELEN_LENG, 1, &offset_Max);
    if(g_gRunPara[RP_LUBO_CYCLE] ==0)//(||(offset_Max ==0))
    {
        if(offset_Max < DAT_MAX)
        {
          offset_time = DAT_A - offset_Max;
        }
        else
        {
          offset_time = DAT_A ;
        }
    }
    else
    {
        if(offset_Max < DAT_MAX)
        {
          offset_time = g_gRunPara[RP_LUBO_CYCLE] *80; // - offset_Max;
        }
        else
        {
          offset_time = g_gRunPara[RP_LUBO_CYCLE] *80;//DAT_A ;
        }
    }
    offset_fault = Calculate_offset(type,FileNum);//����¼������ʱ�̵�ƫ����
    /*if(offset_Max < DAT_FAULT)
    {
    offset_fault = DAT_FAULT+ offset_Max;//DAT_FAULT- offset_Max;
    }
    else
    {
    offset_fault = DAT_FAULT;
    }*/
       sprintf((char *)ch1,"%04d\r\n",offset_time);

    type_offset = COMTRADE_START_STYPE;//�洢¼������
      offset_start =CRG_TIME_OFFSET;
      time_start = CRG_TIME_START;
      time_end = CRG_TIME_END;
      dat_start = DAT_TYPE;
    sprintf((char *)ch,"%1d%1d%1d",type[0],type[1],type[2]);
    //memcpy(&ComtrderCfg1[type_offset],ch,strlen(ch));
    

    
    CAT_SpiWriteBytes(EEPADD_CFG + CFG_COMP_OFFST, 2, (unsigned char*)&g_pCompany[1]);

    CAT_SpiWriteBytes(EEPADD_CFG+type_offset, strlen(ch), (unsigned char*) ch);
    
      
    sprintf((char *)ch,"%02d/%02d/%04d,%02d:%02d:%02d.%03d\r\n",pgRecorder_cfg->comtrade_time[RTC_DATE],pgRecorder_cfg->comtrade_time[RTC_MONTH],
                                   pgRecorder_cfg->comtrade_time[RTC_YEAR],pgRecorder_cfg->comtrade_time[RTC_HOUR],
                                   pgRecorder_cfg->comtrade_time[RTC_MINUT],pgRecorder_cfg->comtrade_time[RTC_SEC],pgRecorder_cfg->comtrade_time[RTC_MICROSEC]);
      //memcpy(&ComtrderCfg1[time_start],ch,strlen(ch));
    CAT_SpiWriteBytes(EEPADD_CFG+time_start, strlen(ch), (unsigned char*) ch);

    lubo_time = pgRecorder_cfg->comtrade_time[RTC_MICROSEC];

    lubo_time += (offset_fault>>2);
       if(lubo_time >= 1000)   //���ʱ
       {
         //lubo_time = 0;
        
        pgRecorder_cfg->comtrade_time[RTC_SEC]++;
        pgRecorder_cfg->comtrade_time[RTC_MICROSEC] = lubo_time -1000;

        if(pgRecorder_cfg->comtrade_time[RTC_SEC] >= 60)     //���Ӽ�ʱ
        {
            pgRecorder_cfg->comtrade_time[RTC_SEC] = 0;
            pgRecorder_cfg->comtrade_time[RTC_MINUT]++;
            
            if(pgRecorder_cfg->comtrade_time[RTC_MINUT] >= 60)
            {
               pgRecorder_cfg->comtrade_time[RTC_MINUT] = 0;
               pgRecorder_cfg->comtrade_time[RTC_HOUR]++;
          if( pgRecorder_cfg->comtrade_time[RTC_HOUR] >=24)
          {
            pgRecorder_cfg->comtrade_time[RTC_HOUR] =0;
          pgRecorder_cfg->comtrade_time[RTC_DATE]++;
          }
            }
          }
   }
   else
    pgRecorder_cfg->comtrade_time[RTC_MICROSEC]= lubo_time;
    sprintf((char *)ch,"%02d/%02d/%04d,%02d:%02d:%02d.%03d\r\n",pgRecorder_cfg->comtrade_time[RTC_DATE],pgRecorder_cfg->comtrade_time[RTC_MONTH],
                                   pgRecorder_cfg->comtrade_time[RTC_YEAR],pgRecorder_cfg->comtrade_time[RTC_HOUR],
                                   pgRecorder_cfg->comtrade_time[RTC_MINUT],pgRecorder_cfg->comtrade_time[RTC_SEC],pgRecorder_cfg->comtrade_time[RTC_MICROSEC]);
      //memcpy(&ComtrderCfg1[time_end],ch,strlen(ch));
    CAT_SpiWriteBytes(EEPADD_CFG+time_end, strlen(ch), (unsigned char*) ch);
    //memcpy(&ComtrderCfg1[offset_start],ch1,strlen(ch1));
    CAT_SpiWriteBytes(EEPADD_CFG+offset_start, strlen(ch1), (unsigned char*) ch1);
      if(!(g_gRunPara[RP_CFG_KEY] & BIT[RP_COMTRADE_TYPE]))
       pdat_type ="ASCII ";
       else
       pdat_type ="BINARY";
      //memcpy(&ComtrderCfg1[dat_start],pdat_type,strlen(pdat_type));
    CAT_SpiWriteBytes(EEPADD_CFG+dat_start, strlen(pdat_type), (unsigned char*) pdat_type);
      //strtemp = ComtrderCfg1;
    
    }
//    char chTmp[510] = {0};
//    for(unsigned int i =0;i< strlen(ComtrderCfg);i++)
//    {  
//      chTmp[i] = CAT_SpiReadByte(EEPADD_CFG+i);//ÿ���ֽ���У���
//    }
//    chTmp[509] = 0x99;
}

unsigned char* LogDirectory(unsigned char *pTxBuf, unsigned char leng)
{
  
  unsigned int VSQ=0x80;
  //char ch[5];
  unsigned int FLogInfo[FLOGINFONUM];    //��EEPROM�ж�ȡ������FLASH�б��渺�ɼ�¼�������Ϣ��������+�����洢��¼��λ��+����һ����¼��λ��+У��
  unsigned long FLogAddr;
  BYTE byLogDa[128] = {0};      
        
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = CATALOG_TYPE;
      CAT_SpiReadWords(EEPADD_LOGNUM, FLOGINFONUM, FLogInfo);
      if(FLogInfo[FLOG_CS] != (FLogInfo[FLOG_TOTALNUM] + FLogInfo[FLOG_NEW] + FLogInfo[FLOG_OLD]))//+FLoadInfo[FLOAD_DAY]) || FLoadInfo[FLOAD_TOTALNUM] > FLASH_LOAD_MAXNUM || FLoadInfo[FLOAD_NEW] > FLASH_LOAD_MAXNUM)//���FLASH��ַ���ڸ��ɼ�¼����������
          FLogInfo[FLOG_TOTALNUM] = 0; //��ո��ɼ�¼������
            wavelog_total = FLogInfo[FLOG_TOTALNUM];
    //if(wavelog_total > 0)
            wavelog_New = FLogInfo[FLOG_NEW];
      pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = 1| VSQ;
      
      //pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = (soe_num)| VSQ;
      pTxBuf += leng;
      gRecorder_flag.LIST_flag = OFF;
     if(wavelog_total==0)//���һ��=(wSendLISTNum > wave_total) ||
      {     
  
        for(int i=0;i<13;i++)
        {
          *pTxBuf++=0;
        }
        
      }
      else
      {
      //if(wSendLISTNum == wave_total) wSendLISTNum=0;
        *pTxBuf++ = 1;
              *pTxBuf++ = 0;
        //sprintf((char *)ch,"LOG");
        //*pTxBuf++ = strlen(ch);
        //for(BYTE j = 0; j < strlen(ch); j++)
       //*pTxBuf++  = ch[j];
        
        //Log_TOTAL_Leng = 9+wavelog_total * FLASH_PLOAD_LEN;//����ͷ9���ֽ�
        Log_TOTAL_Leng  = 15 + 31+ (long)wavelog_total * FLASH_PLOAD_LEN;
    *pTxBuf++ = Log_TOTAL_Leng;
        *pTxBuf++ = Log_TOTAL_Leng>>8;
        *pTxBuf++ = Log_TOTAL_Leng>>16;   
        /* if(((m_Recorder_cfg.lubo_total_num + 1)== g_FRecorder_Current_COUNT)||((m_Recorder_cfg.lubo_total_num+1 == FRECORDER_TOLNUM )&&(g_FRecorder_Current_COUNT==0)))
        m_Recorder_cfg.CFG_SOF = 0x20;  <0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ� 
        else
        m_Recorder_cfg.CFG_SOF = 0;
        if(wSendLISTNum >= wave_total-1)//SOF״̬
        {
        //m_Recorder_cfg.CFG_SOF = 0x20;  <0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ�  
        gRecorder_flag.LIST_flag = OFF;
        
        }    
       else
        {
        //m_Recorder_cfg.CFG_SOF = 0;
        gRecorder_flag.LIST_flag = TRUE;//ON;���޸�
        }*/
        FLogAddr = FADDR_LOG_START;// + ((unsigned long)wSendLISTNum * FLASH_DAYLOAD_LEN);
       Sst26vf064b_Read(FLogAddr,byLogDa, FLASH_PLOAD_LEN);
      *pTxBuf++ = 0x20;//m_Recorder_cfg.CFG_SOF;
      *pTxBuf++= 0;
        *pTxBuf++= byLogDa[10];
      *pTxBuf++= byLogDa[9];
      *pTxBuf++= byLogDa[8];
      *pTxBuf++= byLogDa[7];
      *pTxBuf++= byLogDa[6];
      
        *pTxBuf++= byLogDa[5];
      //else
        /*pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_YEAR];
      if(gRecorder_flag.LIST_flag == OFF)
      {
      wSendLISTNum = 0;
      }*/
     }
     return pTxBuf;
}
unsigned char* FixDirectory(unsigned char *pTxBuf, unsigned char leng)
{
      unsigned int VSQ=0x80;
      unsigned int FLoadInfo[FLOADINFONUM]; 
      unsigned long FLoadAddr;
      BYTE byLoadDa[128] = {0};
      BYTE LoadDay[31] = {0};
      BYTE wCurLoadDay;
      //char ch[6];
      BYTE YC_NUM =0;
      BYTE Fixpt_SOF;
      WORD i;
      BYTE YcIndex;
      //BYTE flag_lb;
      //RECORDER_CFG m_Recorder_cfg;
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = CATALOG_TYPE;
      CAT_SpiReadWords(EEPADD_LOADNUM, FLOADINFONUM, FLoadInfo);
      if((FLoadInfo[FLOAD_CS] != FLoadInfo[FLOAD_TOTALNUM] + FLoadInfo[FLOAD_NEW] + FLoadInfo[FLOAD_OLD]+FLoadInfo[FLOAD_DAY]) || FLoadInfo[FLOAD_TOTALNUM] > FLASH_LOAD_MAXNUM || FLoadInfo[FLOAD_NEW] > FLASH_LOAD_MAXNUM)//���FLASH��ַ���ڸ��ɼ�¼����������

          FLoadInfo[FLOAD_TOTALNUM] = 0; //��ո��ɼ�¼������
          if(FLoadInfo[FLOAD_TOTALNUM] >0)//������ϴ����ļ�����
            wavefix_total = FLoadInfo[FLOAD_DAY]+1;//FLoadInfo[FLOAD_TOTALNUM] / 96 +1;
      else
        wavefix_total = 0;
      
      if(wavefix_total- wSendLISTNum <18)
      pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = (wavefix_total-wSendLISTNum)| VSQ;
      else
        pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = 18| VSQ;
      pTxBuf += leng;
      gRecorder_flag.LIST_flag = OFF; 
      
      if(FLoadInfo[FLOAD_NEW] == 0)//����ļ���λ��
            wCurLoadDay = 31 -1;
          else
            wCurLoadDay = FLoadInfo[FLOAD_NEW]/96;//-1; 
     if((wSendLISTNum > wavefix_total) ||(wavefix_total==0))//���һ��=
      {     
        wSendLISTNum = 0;
        if(wavefix_total == 0)
        {
  
        for(int i=0;i<13;i++)
        {
          *pTxBuf++=0;
        }
        }
      }
      else
      {
      if(wSendLISTNum == wavefix_total) wSendLISTNum=0;
      for( i=0;(wSendLISTNum < wavefix_total)&&(i< 18);i++,wSendLISTNum++)
      {
      
       FLoadAddr = FADDR_LOAD_START + ((unsigned long)wSendLISTNum * FLASH_DAYLOAD_LEN);
       Sst26vf064b_Read(FLoadAddr,byLoadDa, FLASH_PLOAD_LEN);
       YC_NUM =0;
       CAT_SpiReadBytes(EEPADD_DAYNUM+wSendLISTNum,1,&LoadDay[wSendLISTNum]);
         *pTxBuf++ = wSendLISTNum;
              *pTxBuf++ = wSendLISTNum >> 8;
        //sprintf((char *)ch,"FIX");
          
             //for(BYTE j = 0; j < strlen(ch); j++)
          //*pTxBuf++  = ch[j];
      for(BYTE m = 0; m < g_ucYcTransNum; m++)   //������Ҫ�ϴ���ң��ֵ�����վ���������л���
             {
                if(g_ucYCAddr[m]!=255)  
          {
        YcIndex = g_ucYCAddr[m] - 1;
        if(YcIndex <RM_1A_LUBO )//RM_1A_CABLE_T(YcIndex >= RM_BAT_U) && ((RM_1A_LOAD_I + RM_LOADI_NUM))
                    {
          YC_NUM++;
        }

          }
             }
      if(g_ucPara101[IECP_YC_TYPE] == 2) //�̸�����  //�̸�����
     {
          //m_Fixpt_SECTION_Leng = 17+18+YC_NUM*10;
      Fixpt_SECTION_Leng = 24 + 43+28+YC_NUM*14;//��һ�б���ͷ24���ֽڵڶ��б���ͷ43
          //m_Fixpt_TOTAL_Leng =17+((DWORD)(18+YC_NUM*10)* LoadDay);//�ļ�ͷ17���ֽ�+��ͷ+YC�ĸ���
          Fixpt_TOTAL_Leng = 24+43+((DWORD)(28+YC_NUM*14)* (LoadDay[wSendLISTNum]));
    }
    else 
    {
      Fixpt_SECTION_Leng = 24 + 43+28+YC_NUM*11;//��һ�б���ͷ24���ֽڵڶ��б���ͷ43
          //m_Fixpt_TOTAL_Leng =17+((DWORD)(18+YC_NUM*10)* LoadDay);//�ļ�ͷ17���ֽ�+��ͷ+YC�ĸ���
          Fixpt_TOTAL_Leng = 24+43+((DWORD)(28+YC_NUM*11)* (LoadDay[wSendLISTNum]));
    }
         //Fixpt_SECTION_Leng = 17+18+YC_NUM*10;
         //Fixpt_TOTAL_Leng =17+((DWORD)(18+YC_NUM*10)* (LoadDay[wSendLISTNum]));//�ļ�ͷ17���ֽ�+��ͷ+YC�ĸ���
        //*pTxBuf++ = (LoadDay[wSendLISTNum] * FLASH_PLOAD_LEN) ;
        //*pTxBuf++ = m_Recorder_cfg.lubo_total_num >> 8;
       
      
        *pTxBuf++ = Fixpt_TOTAL_Leng;
        *pTxBuf++ = Fixpt_TOTAL_Leng>>8;
        *pTxBuf++ = Fixpt_TOTAL_Leng>>16;    
        if(wCurLoadDay == wSendLISTNum)//(((m_Recorder_cfg.lubo_total_num + 1)== g_FRecorder_Current_COUNT)||((m_Recorder_cfg.lubo_total_num+1 == FRECORDER_TOLNUM )&&(g_FRecorder_Current_COUNT==0)))
        Fixpt_SOF = 0x20; /*<0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ�*/ 
        else
        Fixpt_SOF = 0;
        if(wSendLISTNum >= wavefix_total-1)//SOF״̬
        {
        //m_Recorder_cfg.CFG_SOF = 0x20;  /*<0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ�*/  
        gRecorder_flag.LIST_flag = OFF;
        
        }    
        else
        {
        //m_Recorder_cfg.CFG_SOF = 0;
        gRecorder_flag.LIST_flag = TRUE;//ON;���޸�
        }
        
      *pTxBuf++ =Fixpt_SOF;
      
      *pTxBuf++= 0;//¼������ʱ��
      *pTxBuf++= byLoadDa[7];
      *pTxBuf++= byLoadDa[6];
      *pTxBuf++= byLoadDa[5];
      *pTxBuf++= byLoadDa[4];
      *pTxBuf++= byLoadDa[3];
      
        *pTxBuf++= byLoadDa[2];
      }
      if(gRecorder_flag.LIST_flag == OFF)
      {
      wSendLISTNum = 0;
      }
     }
     return pTxBuf;
}


//***********************************************************************************
//*��������FileDirectory
//*���ܣ���Ӧ��վ���ٻ�Ŀ¼������
//*����: ��������¼����Ŀ¼�ļ�
//***********************************************************************************

unsigned char * SoeDirectory(unsigned char *pTxBuf, unsigned char leng,unsigned int wave_total)//�ٻ�Ŀ¼
{
      unsigned int VSQ=0x80;
      //char ch[5];
      
      
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = CATALOG_TYPE;  
      //pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = (soe_num)| VSQ;
      pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = 1| VSQ;
      pTxBuf += leng;
      gRecorder_flag.LIST_flag = OFF;
    #ifndef GETSOEFROMRAM//���ⲿFLASHȡ����
       //soe_num= g_InFlashInfo.m_InFlashTailNum;
       unsigned int Flashheadbk  =g_InFlashInfo.m_unSSoeInFlashHead;
       soe_num =  ReadNumSoeFromFlash((WORD *)&Flashheadbk,g_unSSoeInFlashTail);  
    #endif
     if(soe_num==0)//���һ��=(wSendLISTNum > wave_total) ||
      {       

            for(int i=0;i<13;i++)
            {
              *pTxBuf++=0;
            }
          
      }
      else
      {
        //if(wSendLISTNum == wave_total) wSendLISTNum=0;
     
          //sprintf((char *)ch,"SOE");
          //*pTxBuf++ = strlen(ch);
          //for(BYTE j = 0; j < strlen(ch); j++)
     //*pTxBuf++  = ch[j];
      *pTxBuf++ = 1;
          *pTxBuf++ = 0;
       
       
          //TOTAL_Leng = 12+soe_num * SOE_RECORD_LEN;//����ͷ9���ֽ�
      TOTAL_Leng = 14 + 33+ soe_num * SOE_RECORD_LEN;
          *pTxBuf++ = TOTAL_Leng;
          *pTxBuf++ = TOTAL_Leng>>8;
          *pTxBuf++ = TOTAL_Leng>>16;       
          /* if(((m_Recorder_cfg.lubo_total_num + 1)== g_FRecorder_Current_COUNT)||((m_Recorder_cfg.lubo_total_num+1 == FRECORDER_TOLNUM )&&(g_FRecorder_Current_COUNT==0)))
            m_Recorder_cfg.CFG_SOF = 0x20;  <0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ� 
          else
            m_Recorder_cfg.CFG_SOF = 0;
          if(wSendLISTNum >= wave_total-1)//SOF״̬
          {
            //m_Recorder_cfg.CFG_SOF = 0x20;  <0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ�  
            gRecorder_flag.LIST_flag = OFF;
            
          }      
         else
          {
            //m_Recorder_cfg.CFG_SOF = 0;
            gRecorder_flag.LIST_flag = TRUE;//ON;���޸�
          }*/
          
        *pTxBuf++ = 0x20;//m_Recorder_cfg.CFG_SOF;
      
        *pTxBuf++= g_sRtcManager.m_gRealTimer[RTC_MICROSEC];//¼������ʱ��
        *pTxBuf++= g_sRtcManager.m_gRealTimer[RTC_SEC];
        *pTxBuf++= g_sRtcManager.m_gRealTimer[RTC_MINUT];
        *pTxBuf++= g_sRtcManager.m_gRealTimer[RTC_HOUR];
        *pTxBuf++= g_sRtcManager.m_gRealTimer[RTC_DATE]|(g_sRtcManager.m_gRealTimer[RTC_WEEK]<<5);
        *pTxBuf++= g_sRtcManager.m_gRealTimer[RTC_MONTH];
        //if(m_Recorder_cfg.comtrade_time[RTC_YEAR] >=2000)
          *pTxBuf++=g_sRtcManager.m_gRealTimer[RTC_YEAR]-2000;// m_Recorder_cfg.comtrade_time[RTC_YEAR]-2000;
        //else
          /*pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_YEAR];
      if(gRecorder_flag.LIST_flag == OFF)
      {
        wSendLISTNum = 0;
      }*/
     }
     return pTxBuf;
}
void LuBoNum(unsigned int wave_total)
{
  BYTE i ;
  if(g_gRunPara[RP_LUBO_NUM] < RP_LUBO_INVALID)
    {
        //CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &g_FRecorder_Current_COUNT);
      for(i =0;i<64;i++)
        lubonum_valid[i] = OFF;
      if(g_FRecorder_Current_COUNT == wave_total)
      {
        if(g_gRunPara[RP_LUBO_NUM] >= g_FRecorder_Current_COUNT)
        {
          for( i =0;i<wave_total;i++ )
            lubonum_valid[i] = ON;
        }
        else
        {
          for(i =(g_FRecorder_Current_COUNT -g_gRunPara[RP_LUBO_NUM]); i<wave_total;i++)
            lubonum_valid[i] = ON;
        }
          
      }
      else if(g_FRecorder_Current_COUNT < wave_total)
      {
        if(g_gRunPara[RP_LUBO_NUM] <= g_FRecorder_Current_COUNT)
        {
          for(i =(g_FRecorder_Current_COUNT -g_gRunPara[RP_LUBO_NUM]); i<g_FRecorder_Current_COUNT;i++)
            lubonum_valid[i] = ON;
        }
        else
        {
          for(i =0;i<g_FRecorder_Current_COUNT;i++)
            lubonum_valid[i] = ON;
          if((64-g_gRunPara[RP_LUBO_NUM]+g_FRecorder_Current_COUNT)>0)
          {
            for(i =(64-g_gRunPara[RP_LUBO_NUM]+g_FRecorder_Current_COUNT);i<64;i++)
              lubonum_valid[i] = ON;
          }
        }
      }
    }
}
void LuBoGetNum(unsigned int wave_total)//unsigned int ���������¼������Ч�����Ŀ¼�����µ��ж�
{
  RECORDER_CFG m_Recorder_cfg;
    BYTE num_lubo =0;
  for(int i=0;i< wave_total;i++)
    {
    for(int j=0;j<3;j++)
    {
        long FADDR_RECORDER =FADDR_RECORDER_INFO+ (long)i*(long)FLINEADDR +(long)j*(long)FPHASEADDR; 
        Sst26vf064b_Read(FADDR_RECORDER,(unsigned char *)&m_Recorder_cfg,sizeof(m_Recorder_cfg)); //(unsigned char *)�������ﱣ��gRecorder_cfg��ֵ����Ϊ�����¼����һ�����ܴ����� 
        //flag_lb++;
        if(m_Recorder_cfg.lubo_flag ==0x55)
        {
          //flag_lb =0;
            break;
        }
                }
      TimeAdd((WORD*)&m_Recorder_cfg.comtrade_time,0,0,0,0,g_gRunPara[RP_LBSTORAGE_TIME]);
      
      if((m_Recorder_cfg.comtrade_time[RTC_YEAR]>g_sRtcManager.m_gRealTimer[RTC_YEAR])||(m_Recorder_cfg.comtrade_time[RTC_YEAR]==g_sRtcManager.m_gRealTimer[RTC_YEAR] && m_Recorder_cfg.comtrade_time[RTC_MONTH] > g_sRtcManager.m_gRealTimer[RTC_MONTH])||
        (m_Recorder_cfg.comtrade_time[RTC_YEAR]==g_sRtcManager.m_gRealTimer[RTC_YEAR]&&m_Recorder_cfg.comtrade_time[RTC_MONTH]==g_sRtcManager.m_gRealTimer[RTC_MONTH]&&m_Recorder_cfg.comtrade_time[RTC_DATE]>=g_sRtcManager.m_gRealTimer[RTC_DATE]))
        {
          lubo_valid[i]= 0x55;
              num_lubo ++;
        }
    
    
  }
     
}
#if 0
void LuBoGetNum(unsigned int wave_total)//unsigned int ���������¼������Ч�����Ŀ¼�����µ��ж�
{
  RECORDER_CFG m_Recorder_cfg;
  BYTE num_lubo =0;
  //unsigned long start_addr;
        WORD total_day,day_num,n;
        DWORD a,b;
    
    
  BYTE month_day[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
  total_day =0;
  for(int i=0;i< wave_total;i++)
  {
      //flag_lb =0;
      for(int j=0;j<3;j++)
      {
            long FADDR_RECORDER =FADDR_RECORDER_INFO+ (long)i*(long)FLINEADDR +(long)j*(long)FPHASEADDR; 
            Sst26vf064b_Read(FADDR_RECORDER,(unsigned char *)&m_Recorder_cfg,sizeof(m_Recorder_cfg)); //(unsigned char *)�������ﱣ��gRecorder_cfg��ֵ����Ϊ�����¼����һ�����ܴ����� 
            //flag_lb++;
            if(m_Recorder_cfg.lubo_flag ==0x55)
            {
              //flag_lb =0;
                break;
            }

      
       }
      //if(g_sRtcManager.m_gRealTimer[RTC_YEAR] == m_Recorder_cfg.comtrade_time[RTC_YEAR])
      //{
      if((((m_Recorder_cfg.comtrade_time[RTC_YEAR]%4) ==0) &&(m_Recorder_cfg.comtrade_time[RTC_YEAR]%100)!=0)||(m_Recorder_cfg.comtrade_time[RTC_YEAR]%400 ==0))
        month_day[2]=29;//{0,31,29,31,30,31,30,31,31,30,31,30,31};
    //else
      //month_day[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    for (int m =0;m<13;m++)
      total_day += month_day[m];
    //unsigned int month =m_Recorder_cfg.comtrade_time[RTC_MONTH];
    if((m_Recorder_cfg.comtrade_time[RTC_DATE] == g_sRtcManager.m_gRealTimer[RTC_DATE])&&(m_Recorder_cfg.comtrade_time[RTC_YEAR] == g_sRtcManager.m_gRealTimer[RTC_YEAR]))
    {
      if(g_sRtcManager.m_gRealTimer[RTC_HOUR] >= m_Recorder_cfg.comtrade_time[RTC_HOUR])
      {
        a = g_sRtcManager.m_gRealTimer[RTC_HOUR]*60 + g_sRtcManager.m_gRealTimer[RTC_MINUT];
        b = m_Recorder_cfg.comtrade_time[RTC_HOUR]*60 + m_Recorder_cfg.comtrade_time[RTC_MINUT];

        if((a - b) < g_gRunPara[RP_LBSTORAGE_TIME])
        {
      lubo_valid[i]= 0x55;
      num_lubo ++;
        }
      }
      else
      {
    lubo_valid[i]= 0x55;
    num_lubo ++;
      }
  }
  else
  {
    if(m_Recorder_cfg.comtrade_time[RTC_MONTH] == g_sRtcManager.m_gRealTimer[RTC_MONTH])
    {
      if(g_sRtcManager.m_gRealTimer[RTC_DATE] >= m_Recorder_cfg.comtrade_time[RTC_DATE] )
      {
    a = g_sRtcManager.m_gRealTimer[RTC_DATE] *24*60+g_sRtcManager.m_gRealTimer[RTC_HOUR]*60 + g_sRtcManager.m_gRealTimer[RTC_MINUT];
    b = m_Recorder_cfg.comtrade_time[RTC_DATE] *24*60+m_Recorder_cfg.comtrade_time[RTC_HOUR]*60 + m_Recorder_cfg.comtrade_time[RTC_MINUT];
    if((a - b) < g_gRunPara[RP_LBSTORAGE_TIME])
    {
      lubo_valid[i]= 0x55;
      num_lubo ++;
    }
      }
      else
      {
    lubo_valid[i]= 0x55;
    num_lubo ++;
      }
    }
    else
    {
      if(m_Recorder_cfg.comtrade_time[RTC_YEAR] == g_sRtcManager.m_gRealTimer[RTC_YEAR])
      {
    if(g_sRtcManager.m_gRealTimer[RTC_MONTH] >= m_Recorder_cfg.comtrade_time[RTC_MONTH])
    {
      day_num = month_day[m_Recorder_cfg.comtrade_time[RTC_MONTH]] - m_Recorder_cfg.comtrade_time[RTC_DATE];

      for(n = m_Recorder_cfg.comtrade_time[RTC_MONTH]+1; n <g_sRtcManager.m_gRealTimer[RTC_MONTH];n++ )
      day_num += month_day[n];

      b = m_Recorder_cfg.comtrade_time[RTC_HOUR]*60 + m_Recorder_cfg.comtrade_time[RTC_MINUT];//m_Recorder_cfg.comtrade_time[RTC_MONTH] * month_day[RTC_MONTH] +m_Recorder_cfg.comtrade_time[RTC_DATE]) *24*60+
      a = (day_num+g_sRtcManager.m_gRealTimer[RTC_DATE]) *24*60+g_sRtcManager.m_gRealTimer[RTC_HOUR]*60 + g_sRtcManager.m_gRealTimer[RTC_MINUT];
      if((a - b) < g_gRunPara[RP_LBSTORAGE_TIME])
      {
        lubo_valid[i]= 0x55;
        num_lubo ++;
      }
    }
    else
    {
      lubo_valid[i]= 0x55;
      num_lubo ++;
    }
        }
        else if(m_Recorder_cfg.comtrade_time[RTC_YEAR] < g_sRtcManager.m_gRealTimer[RTC_YEAR])
        {
    if(g_sRtcManager.m_gRealTimer[RTC_YEAR] - m_Recorder_cfg.comtrade_time[RTC_YEAR] ==1)
    {
              
      day_num = month_day[m_Recorder_cfg.comtrade_time[RTC_MONTH]] - m_Recorder_cfg.comtrade_time[RTC_DATE];
              
      for(n = m_Recorder_cfg.comtrade_time[RTC_MONTH]+1; n <13;n++ )
      day_num += month_day[n];
              
      for(n = 1; n <g_sRtcManager.m_gRealTimer[RTC_MONTH];n++ )
      day_num += month_day[n];

      b = (m_Recorder_cfg.comtrade_time[RTC_HOUR]*60) + m_Recorder_cfg.comtrade_time[RTC_MINUT];//m_Recorder_cfg.comtrade_time[RTC_YEAR]*total_day+g_sRtcManager.m_gRealTimer[RTC_MONTH] * month_day[RTC_MONTH]+g_sRtcManager.m_gRealTimer[RTC_DATE]) *24*60+
      a = (day_num +g_sRtcManager.m_gRealTimer[RTC_DATE]) *24*60+g_sRtcManager.m_gRealTimer[RTC_HOUR]*60 + g_sRtcManager.m_gRealTimer[RTC_MINUT];
      if((a - b) < g_gRunPara[RP_LBSTORAGE_TIME])
      {
      lubo_valid[i]= 0x55;
      num_lubo ++;
      } 
     }
    }
    else if(m_Recorder_cfg.comtrade_time[RTC_YEAR] > g_sRtcManager.m_gRealTimer[RTC_YEAR])
    {
      lubo_valid[i]= 0x55;
      num_lubo ++;
    } 
        }
            
      }
      
      
      //}
      }
    //return num_lubo;
}
# endif
//************************************************************************
//*��������FileDirectory
//*���ܣ���Ӧ��վ���ٻ�Ŀ¼������
//*����: ��������¼����Ŀ¼�ļ�
//***********************************************************************************

unsigned char * FileDirectory(unsigned char *pTxBuf, unsigned char leng,unsigned int wave_total)//�ٻ�Ŀ¼
{
      unsigned int VSQ=0x80;
    unsigned char sc_num =0;
          unsigned char *pTxPos;
         long FADDR_RECORDER; 
      //BYTE flag_lb;
      RECORDER_CFG m_Recorder_cfg;
    BYTE count_num=0; 
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = CATALOG_TYPE;
      if((g_gRunPara[RP_LBSTORAGE_TIME] == RP_LBSTORAGE_INVALID)&&(g_gRunPara[RP_LUBO_NUM] == RP_LUBO_INVALID))
      {
        if(wave_total- wSendLISTNum <18)
          pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = (wave_total-wSendLISTNum)| VSQ;
        else
        pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = 18| VSQ;
      }
      else
        pTxPos = &pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]];//=  VSQ;
    
      pTxBuf += leng;
      gRecorder_flag.LIST_flag = OFF;
     if((wSendLISTNum > wave_total) ||(wave_total==0))//���һ��=
      {       
          wSendLISTNum = 0;
          if(wave_total == 0)
          {

            for(int i=0;i<13;i++)
            {
              *pTxBuf++=0;
            }
          }
      }
      else
      {
        if(wSendLISTNum == wave_total) wSendLISTNum=0;
        for(int i=0;(wSendLISTNum < wave_total)&&(i< 18);i++,wSendLISTNum++)
        {
      //flag_lb =0;
          for(int j=0;j<3;j++)
          {
      if(g_Recorder_Total_COUNT < FRECORDER_TOLNUM)//������С��64����ͷ��ʼȡֵ g_FRecorder_Current_COUNT;//flash���浽�ڼ�����
      {
        FADDR_RECORDER =FADDR_RECORDER_INFO+ (long)wSendLISTNum*(long)FLINEADDR +(long)j*(long)FPHASEADDR;
        count_num = wSendLISTNum;
      }
      else
      {
        
        FADDR_RECORDER =FADDR_RECORDER_INFO+ (long)BK_FRecorder_Current_COUNT*(long)FLINEADDR +(long)j*(long)FPHASEADDR; 
        count_num = BK_FRecorder_Current_COUNT;
      }
      Sst26vf064b_Read(FADDR_RECORDER,(unsigned char *)&m_Recorder_cfg,sizeof(m_Recorder_cfg)); //(unsigned char *)�������ﱣ��gRecorder_cfg��ֵ����Ϊ�����¼����һ�����ܴ����� 
            //flag_lb++;
            if(m_Recorder_cfg.lubo_flag ==0x55)
            {
              //flag_lb =0;
                break;
            }
          }
          BK_FRecorder_Current_COUNT++;
    if(BK_FRecorder_Current_COUNT >= FRECORDER_TOLNUM)
    BK_FRecorder_Current_COUNT =0;
          if(wSendLISTNum >= wave_total-1)//SOF״̬
            {
              //m_Recorder_cfg.CFG_SOF = 0x20;  /*<0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ�*/  
              gRecorder_flag.LIST_flag = OFF;
            
            }      
            else
            {
              //m_Recorder_cfg.CFG_SOF = 0;
              gRecorder_flag.LIST_flag = TRUE;//ON;���޸�
            }
      //if(flag_lb >=3) 
        //continue;
          if((((lubo_valid[count_num] ==0x55)&&(g_gRunPara[RP_LBSTORAGE_TIME] != RP_LBSTORAGE_INVALID))||(g_gRunPara[RP_LBSTORAGE_TIME] == RP_LBSTORAGE_INVALID)) &&(((lubonum_valid[count_num] ==ON)&&(g_gRunPara[RP_LUBO_NUM] < RP_LUBO_INVALID))||(g_gRunPara[RP_LUBO_NUM] == RP_LUBO_INVALID)))
          {
      sc_num ++;
      *pTxBuf++ = m_Recorder_cfg.lubo_total_num;
            *pTxBuf++ = m_Recorder_cfg.lubo_total_num >> 8;
            *pTxBuf++ = m_Recorder_cfg.TOTAL_Leng;
            *pTxBuf++ = m_Recorder_cfg.TOTAL_Leng>>8;
            *pTxBuf++ = m_Recorder_cfg.TOTAL_Leng>>16;       
            if(((m_Recorder_cfg.lubo_total_num + 1)== g_FRecorder_Current_COUNT)||((m_Recorder_cfg.lubo_total_num+1 == FRECORDER_TOLNUM )&&(g_FRecorder_Current_COUNT==0)))
              m_Recorder_cfg.CFG_SOF = 0x20;  /*<0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ�*/ 
            else
              m_Recorder_cfg.CFG_SOF = 0;
            /*if(wSendLISTNum >= wave_total-1)//SOF״̬
            {
              //m_Recorder_cfg.CFG_SOF = 0x20;  <0>:=���滹��Ŀ¼�ļ���<1>:=���Ŀ¼�ļ� 
              gRecorder_flag.LIST_flag = OFF;
            
            }      
            else
            {
              //m_Recorder_cfg.CFG_SOF = 0;
              gRecorder_flag.LIST_flag = TRUE;//ON;���޸�
            }*/
          
          *pTxBuf++ = m_Recorder_cfg.CFG_SOF;
      
          //*pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_MICROSEC];//¼������ʱ��
          //*pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_SEC];
      *pTxBuf++ = (m_Recorder_cfg.comtrade_time[RTC_MICROSEC]+m_Recorder_cfg.comtrade_time[RTC_SEC]*1000)&0XFF;
          *pTxBuf++ = (m_Recorder_cfg.comtrade_time[RTC_MICROSEC]+m_Recorder_cfg.comtrade_time[RTC_SEC]*1000)>>8;
          *pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_MINUT];
          *pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_HOUR];
          *pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_DATE]|(m_Recorder_cfg.comtrade_time[RTC_WEEK]<<5);
          *pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_MONTH];
          if(m_Recorder_cfg.comtrade_time[RTC_YEAR] >=2000)
            *pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_YEAR]-2000;
          else
            *pTxBuf++= m_Recorder_cfg.comtrade_time[RTC_YEAR];
         }
      }
      if(gRecorder_flag.LIST_flag == OFF)
      {
        wSendLISTNum = 0;
      }
     }
   if((g_gRunPara[RP_LBSTORAGE_TIME] != RP_LBSTORAGE_INVALID)||(g_gRunPara[RP_LUBO_NUM] < RP_LUBO_INVALID))
           *pTxPos = (sc_num+0x80);
   //pTxBuf[6+g_ucPara101[IECP_LINKADDR_NUM]] = (sc_num+0x80);
     return pTxBuf;
}

unsigned char *  ApprovalLogFile(unsigned char *pTxBuf,unsigned char leng,WORD wSecLenPtr)
{
    //WORD FileName;
    //BYTE Loadday;
    //BYTE log_sum=0;
    if(pTxBuf[leng + 3]==3)//�ڴ�����Ͽ�,AFQ
    {
    //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng],gRecorder_flag.pRXBuff[leng + 1]);//-1;
    
    if(section_current==1) //�Ͽɵ�һ�ں��ͽ�2׼������
    {
     // section_current++;//=2;
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//SECTIONPREPAR_TYPE;
      pTxBuf += leng;
      //log_recorded.log_Ptr  =0;
            //log_recorded.log_Curren_count =1;
      //log_recorded.log_count =0;
      
     
      //log_sum_section =0;
      //fixpt_recorded.fixpt_count_flag =OFF;  
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng];//�ļ���������ֽ�
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
      *pTxBuf++ = section_current;//��ǰ������
        *pTxBuf++ =1;//LSQ=1 ����ֹͣ����Ľڴ���      
        *pTxBuf++ = log_sum_section;
        //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
       /*if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
       {
        *pTxBuf++ = pgRecorder_cfg->CFG_Leng;
        *pTxBuf++ = pgRecorder_cfg->CFG_Leng>>8; 
         GLubocfg_Sum =0;
        
       }
       else//����.DAT�ڵĳ���
       {
        *pTxBuf++ = pgRecorder_cfg->DATA_Leng;
        *pTxBuf++ = pgRecorder_cfg->DATA_Leng>>8;
         GLubodat_Sum =0;
       }*/
      // if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
      }
     // *pTxBuf++ = 46 +FLASH_PLOAD_LEN;//9 +FLASH_PLOAD_LEN;
     // *pTxBuf++ = (46 +FLASH_PLOAD_LEN)>>8;  //(9 +FLASH_PLOAD_LEN)>>8;  //
    }
   else if(pTxBuf[leng + 3]==1)//�ļ�������Ͽ�AFQ
     {
      section_current=1;
      pTxBuf = NULL;
     }
    return pTxBuf;
     
}

unsigned char *  ApprovalFixFile(unsigned char *pTxBuf,unsigned char leng,WORD wSecLenPtr)
{
  WORD FileName;
  BYTE Loadday;
        BYTE fix_sum=0;
  if(pTxBuf[leng + 3]==3)//�ڴ�����Ͽ�,AFQ
        {
          FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng],gRecorder_flag.pRXBuff[leng + 1]);//-1;
     CAT_SpiReadBytes(EEPADD_DAYNUM + FileName ,1,&Loadday);
    if(section_current < Loadday) //�Ͽɵ�һ�ں��ͽ�2׼������
    {
      section_current++;//=2;
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= SECTIONPREPAR_TYPE;
      pTxBuf += leng;
      fixpt_recorded.fixpt_Ptr =0;
      fixpt_recorded.fixpt_count =0;
      //fixpt_recorded.fixpt_Curren_count =0;
      fixpt_recorded.fixpt_Curren_count =1;
    //fix_YC_NUM =0;
          fixpt_sum[section_current-1] =0;
      //fixpt_recorded.fixpt_count_flag =OFF;    
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng];//�ļ���������ֽ�
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
      *pTxBuf++ = section_current;//��ǰ������
      
     
          //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
       /*if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
       {
          *pTxBuf++ = pgRecorder_cfg->CFG_Leng;
          *pTxBuf++ = pgRecorder_cfg->CFG_Leng>>8; 
           GLubocfg_Sum =0;
          
       }
       else//����.DAT�ڵĳ���
       {
          *pTxBuf++ = pgRecorder_cfg->DATA_Leng;
          *pTxBuf++ = pgRecorder_cfg->DATA_Leng>>8;
           GLubodat_Sum =0;
       }*/
       if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
    {
      *pTxBuf++ = Fixpt_SECTION_Leng&0x00FF;
      *pTxBuf++ = (Fixpt_SECTION_Leng>>8)&0x00FF; 
    }
    else
    {
      *pTxBuf++ = (Fixpt_SECTION_Leng-67)&0x00FF;
      *pTxBuf++ = ((Fixpt_SECTION_Leng-67)>>8)&0x00FF;  
    }
      //*pTxBuf++ = (fix_YC_NUM *10)+20;
        //*pTxBuf++ =((fix_YC_NUM *10)+20)>>8;
      
          *pTxBuf++ = 0;//6;//SRQ=0,��һ��׼������
        
     }
     else if(section_current >= Loadday)//== 2�Ͽɵڶ��ں�������
     {
          pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
          pTxBuf += leng;
          
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng];//�ļ���������ֽ�
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
        *pTxBuf++ = section_current;//��ǰ������
        *pTxBuf++ =1;//LSQ=1 ����ֹͣ����Ľڴ���
          //pTxBuf[wSecLenPtr] = 1;//LSQ=3 ����ֹͣ����Ľڴ���
             //GLubocfg_Sum = ChkluboSum(ComtrderCfg1, strlen(ComtrderCfg1));
          for(int i =0;i<Loadday;i++)
            fix_sum += fixpt_sum[i];
           *pTxBuf++ = fix_sum;//�ڵ�У��λ
     }
   }
   else if(pTxBuf[leng + 3]==1)//�ļ�������Ͽ�AFQ
   {
      section_current=1;
      pTxBuf = NULL;
   }
  return pTxBuf;
}



unsigned char *  ApprovalSoeFile(unsigned char *pTxBuf,unsigned char leng,WORD wSecLenPtr)
{
  if(pTxBuf[leng + 3]==3)//�ڴ�����Ͽ�,AFQ
    {
      if(section_current==1) //�Ͽɵ�һ�ں��ͽ�2׼������
        {
    pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
            pTxBuf += leng;
          
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng];//�ļ���������ֽ�
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
          *pTxBuf++ = section_current;//��ǰ������
          *pTxBuf++ =1;//LSQ=1 ����ֹͣ����Ľڴ���
          //pTxBuf[wSecLenPtr] = 1;//LSQ=3 ����ֹͣ����Ľڴ���
              //GLubocfg_Sum = ChkluboSum(ComtrderCfg1, strlen(ComtrderCfg1));
          *pTxBuf++ = soe_sum;//GLubodat_Sum + GLubocfg_Sum;//�ڵ�У��λ
    }
  }
  else if(pTxBuf[leng + 3]==1)//�ļ�������Ͽ�AFQ
    {
          section_current=1;
          pTxBuf = NULL;
    }
         return pTxBuf;
}

//***********************************************************************************
//*��������ApprovalFile
//*���ܣ��õ���վ�ϿɵĽڴ���󣬷�����һ��׼�������ı���
//*����: �������ڵ�ʱ�������нڵ�У�飬������֤�ô�¼������ȷ��
//***********************************************************************************

unsigned char *  ApprovalFile(unsigned char *pTxBuf,unsigned char leng,RECORDER_CFG *pgRecorder_cfg,WORD wSecLenPtr)
{
  if(pTxBuf[leng + 3]==3)//�ڴ�����Ͽ�,AFQ
  {
    if(section_current==1) //�Ͽɵ�һ�ں��ͽ�2׼������
    {
      section_current=2;
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= SECTIONPREPAR_TYPE;
      pTxBuf += leng;
          
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng];//�ļ���������ֽ�
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
      *pTxBuf++ = section_current;//��ǰ������
          //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
       if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
       {
          *pTxBuf++ = pgRecorder_cfg->CFG_Leng;
          *pTxBuf++ = pgRecorder_cfg->CFG_Leng>>8; 
           GLubocfg_Sum =0;
          
       }
       else//����.DAT�ڵĳ���
       {
          *pTxBuf++ = pgRecorder_cfg->DATA_Leng;
          *pTxBuf++ = pgRecorder_cfg->DATA_Leng>>8;
           GLubodat_Sum =0;
       }
          *pTxBuf++ = 0;//6;//SRQ=0,��һ��׼������
        
     }
     else if(section_current == 2)//�Ͽɵڶ��ں�������
     {
          pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
          pTxBuf += leng;
          
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng];//�ļ���������ֽ�
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
        *pTxBuf++ = section_current;//��ǰ������
        *pTxBuf++ =1;//LSQ=1 ����ֹͣ����Ľڴ���
          //pTxBuf[wSecLenPtr] = 1;//LSQ=3 ����ֹͣ����Ľڴ���
              //GLubocfg_Sum = ChkluboSum(ComtrderCfg1, strlen(ComtrderCfg1));
         *pTxBuf++ = GLubodat_Sum + GLubocfg_Sum;//�ڵ�У��λ
     }
   }
   else if(pTxBuf[leng + 3]==1)//�ļ�������Ͽ�AFQ
   {
      section_current=1;
    g_gRmtLockLB =0;
    CreatNewSoe(RI_LUBO_FLAG,0);
      pTxBuf = NULL;
   }
  return pTxBuf;
}
//***********************************************************************************
//*��������FileDataCfg
//*���ܣ��ļ�1���䣬��Ӧ.cfg�ļ�
//*����: �����ļ����δ���
//***********************************************************************************

unsigned char *  FileDataCfg(unsigned char *pTxBuf, unsigned char leng,RECORDER_CFG *pgRecorder_cfg,int segment_leng)//WORD wSecLenPtr,
{
       //int segment_leng = SEGMENT_LENGTH;
       unsigned int j;
       
       unsigned char *pTxPos;
       unsigned char cfg_byte;
       wCfgTotalNum = (pgRecorder_cfg->CFG_Leng);//gRecorder_cfg[FileName] SEGMENT_LENGTH);//IEC_YX_NUM;
            //if( wCfgTotalNum >=1)
       gRecorder_flag.LIST_flag = OFF;
       //pTxBuf += leng;     
       if(wSendCFGNum >= wCfgTotalNum)//���һ��
       {
          pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
            pTxBuf += leng;
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
          *pTxBuf++ = 3;//pTxBuf[wSecLenPtr]LSQ=3 ����ֹͣ����Ľڴ���
          GLubocfg_Sum =0;
          for(unsigned int i =0;i< strlen(ComtrderCfg);i++)
          {
            
      cfg_byte = CAT_SpiReadByte(EEPADD_CFG+i);//ÿ���ֽ���У���
      GLubocfg_Sum = GLubocfg_Sum +cfg_byte;//ChkluboSum(&cfg_byte, 1);
      //GLubocfg_Sum = ChkluboSum((BYTE *)ComtrderCfg1, strlen(ComtrderCfg1));
          }
           
          *pTxBuf++ = GLubocfg_Sum ;
          wSendCFGNum=0;
              
        }
        else
        {
           pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= SEGMENT_TYPE;//���͵Ķ���Ϣ
           pTxBuf += leng;
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
           pTxPos = pTxBuf;
          *pTxBuf++ =0; //pTxBuf[leng+3]  = 0;
           
           for(j = 0;(j < segment_leng)&&(wSendCFGNum < wCfgTotalNum);wSendCFGNum++,j++)
           {
               //*pTxBuf++ = *strtemp++;
         *pTxBuf++  =  CAT_SpiReadByte(EEPADD_CFG+wSendCFGNum);
           }
          
           *pTxPos = pTxBuf - pTxPos-1;//LL
           //w_gfgId;//�γ���pTxBuf[wSecLenPtr]
           gRecorder_flag.LIST_flag = TRUE;//ON;
        }
             //wSendCFGNum += w_gfgId;
            //m_SendBuf.pBuf[wSecLenPtr] = m_SendBuf.wWritePtr - wSecLenPtr -1;
        return pTxBuf;    
}

unsigned char *LogData(unsigned char *pTxBuf,unsigned char leng,int segment_leng)
{
        
      char *pdat_name;
      //BYTE byCurSoeNum = 0;
      //unsigned int object_addr_length =2;
      //BYTE YC_Type =9;//��һ��ֵ
      //WORD second;
      //WORD Millisecond,i;
      char ch[60]={0};
      char ch_1[5]={0};
      //char ch_1[10];
      BYTE byLoadDa[128] = {0};
      //unsigned char type = M_SP_TB;
      pdat_name="ulog.msg,v1.0";//"AU";
      unsigned char *pTxPos;
      WORD n,i;
      WORD log_leng =0;
      unsigned char *pTxBuf1 = pTxBuf;
      //BYTE max_num;
      //BYTE Loadday;
      //BYTE YcIndex = 0;
      //BYTE infoaddrl,infoaddrh;
      //WORD wLoadMeas[60] = {0};
      //int offset =0;
      //BYTE g_YC_NUM =0;
      gRecorder_flag.LIST_flag = OFF;
      //WORD FileName;
        //char ch[250];
      //WORD FileName;
     // BYTE Current_Section;
      BYTE length =0;
      
      char chDevId[30]={0};
      memcpy(chDevId,g_pDevId,24);
      char *pDevID = chDevId;
      int wCurPos = 0;
      //Current_Section = gRecorder_flag.pRXBuff[leng + 2];
      unsigned long FLoadAddr;
      wCurPos = wavelog_New -1- log_recorded.log_Ptr;
      if(wCurPos < 0)
        wCurPos = FLASH_LOG_MAXNUM + wCurPos;
     
      FLoadAddr = FADDR_LOG_START + ((unsigned long)(wCurPos) *FLASH_PLOG_LEN);//Current_Section-1
      //CAT_SpiReadBytes(EEPADD_DAYNUM + FileName ,1,&Loadday);
      if((log_recorded.log_Ptr >= log_recorded.log_Curren_count ) &&(log_recorded.log_count_flag == ON))//���һ��Sample_num
      {
         pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
          //pTxBuf[wSecLenPtr] = 3;//LSQ=3 ����ֹͣ����Ľڴ���
              //GLubocfg_Sum = ChkluboSum(ComtrderCfg1, strlen(ComtrderCfg1));
          pTxBuf += leng;
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
          *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
          *pTxBuf++ = 3;//pTxBuf[wSecLenPtr]LSQ=3 ��ֹͣ����Ľڴ���
          *pTxBuf++ = log_sum_section;//fixpt_sum[Current_Section-1];
          log_sum_total +=log_sum_section;
          //soe_recorded.soe_count = 0;
           //return pTxBuf;
      }
      else
      {
          pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= SEGMENT_TYPE;//���͵Ķ���Ϣ
          pTxBuf += leng;
         *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
          
        pTxPos = pTxBuf;
        *pTxBuf++  = 0;//SEGMENT_LENGTH;
          
          //for(int i=0;(wSendDATNum < (DAT_NUM - offset_time_Max));wSendDATNum++)//ÿһ�е�ֵSample_num
        if(log_recorded.log_count_flag  == OFF)//(!soe_recorded.Soe_Area )&&(!soe_recorded.soe_count)&&(!soe_recorded.Soe_Ptr)
        {
          
          //sprintf((char *)ch,"%.*s,%02x%02x\r\n",sizeof(pdat_name),pdat_name,wavelog_total,(wavelog_total >>8));
       sprintf((char *)ch,"%s\r\n%s,%04d\r\n",pdat_name,pDevID,wavelog_total);
      log_sum_section = 0;
          for(n = 0; n < strlen(ch); n++)
          {
            *pTxBuf++ = ch[n];
            log_sum_section += ch[n];
          }
          //CAT_SpiReadBytes(EEPADD_DAYNUM+FileName,1,&(fixpt_recorded.fixpt_count));
          if(wavelog_total >0)
            log_recorded.log_Curren_count = wavelog_total;//1;//���н��������ݣ����Ӧ�Ķ�����Ϊ1�����淢��ʱ�ټ������Ķ���
          log_recorded.log_Ptr = 0;//wavelog_New;//0;
          log_recorded.log_count_flag = ON;
          //fix_YC_NUM =0;
        }//����δ��
        else if(log_recorded.log_count_flag == ON)
        {
          
          Sst26vf064b_Read(FLoadAddr,byLoadDa,FLASH_PLOAD_LEN);
          if((byLoadDa[0] ==0xFF) &&(byLoadDa[1] ==0xFF)&&(byLoadDa[2] ==0xFF))
            {
              Sst26vf064b_Read(FLoadAddr,byLoadDa,FLASH_PLOAD_LEN);
            }
          /*for (WORD i =0;i< FLASH_LOG_MAXNUM;i++)
          {
              
            Sst26vf064b_Read(FLoadAddr,byLoadDa,FLASH_PLOAD_LEN);
            if((byLoadDa[0] ==0xFF) &&(byLoadDa[1] ==0xFF)&&(byLoadDa[2] ==0xFF))
            {
              log_recorded.log_Ptr 
              FLoadAddr  = FADDR_LOG_START + ((unsigned long)(log_recorded.log_Ptr) *FLASH_PLOG_LEN);//Current_Section-1
            }
           }*/
            //memcpy(wLoadMeas,&byLoadDa[8],FLASH_PLOAD_LEN-8);
          
          if(log_recorded.log_Ptr < log_recorded.log_Curren_count)
          {
            
              
              //sprintf((char *)ch,"%02x%02x,%02d%02d%02d_%02d%02d%02d,",byLoadDa[4],byLoadDa[3],byLoadDa[5],byLoadDa[6],byLoadDa[7],byLoadDa[8],byLoadDa[9],byLoadDa[10]);
      sprintf((char *)ch,"%02d,%04d-%02d-%02d %02d:%02d:%02d.%03d,",MAKEWORD(byLoadDa[3],byLoadDa[4]),byLoadDa[5]+2000,byLoadDa[6],byLoadDa[7],byLoadDa[8],byLoadDa[9],byLoadDa[10],0);
              for(n = 0; n < strlen(ch); n++)
              {
            
                *pTxBuf++ = ch[n];
                log_sum_section+= ch[n];
                }
              //memcpy(pTxBuf,&byLoadDa[10],byLoadDa[1] -8);//leng
              //pTxBuf += (byLoadDa[1] -8);
              if((byLoadDa[1] >=9)&&(byLoadDa[1] < 255))
                length = byLoadDa[1]-9;
              else
                length = 0;
              for(n = 0; n < length; n++)
              {
            
                *pTxBuf++ =byLoadDa[11+n];
                log_sum_section+= byLoadDa[11+n];
                }
              log_leng = strlen(ch) + (byLoadDa[1] -9);
              sprintf((char *)ch_1,",%02d",byLoadDa[2]);
              for(n = 0; n < strlen(ch_1); n++)
              {
            
                *pTxBuf++ = ch_1[n];
                log_sum_section+= ch_1[n];
                }
              log_leng = strlen(ch) + length+ strlen(ch_1);//(byLoadDa[1] -9)
              if(log_leng <126)
              {
                for(i=log_leng; i< 126; i++)//n < strlen(ch),
                {
            
                *pTxBuf++ = 0x20;
                log_sum_section+= 0x20;
                
                }
              }
              sprintf((char *)ch_1,"\r\n");
              for(n = 0; n < strlen(ch_1); n++)
              {
            
                *pTxBuf++ = ch_1[n];
                log_sum_section+= ch_1[n];
                }
            log_recorded.log_Ptr++;
            
            pTxBuf[1] = pTxBuf - &(pTxBuf1[4]);//LL
    
            if(pTxBuf[1] == 0x2B)
            {
              pTxBuf[1] = 0x8F;
            }
          }
          
           
          if(log_recorded.log_Ptr ==0)
          pTxBuf = NULL;
        }
        if(pTxBuf != NULL)
        {
        *pTxPos = pTxBuf - pTxPos-1;//LL
            
        }
        gRecorder_flag.LIST_flag = TRUE;//ON; 
    }
    
            pTxBuf[1] = pTxBuf - &(pTxBuf1[4]);//LL
    
            if(pTxBuf[1] == 0x2B)
            {
              pTxBuf[1] = 0x8F;
            }
    return pTxBuf;
  }


unsigned char * FixptData(unsigned char *pTxBuf,unsigned char leng,int segment_leng)
{
      
    char *pdat_name;
    //BYTE byCurSoeNum = 0;
    unsigned int object_addr_length =2;
    //BYTE YC_Type =9;//��һ��ֵ
    //WORD second;
    //WORD Millisecond,i;
    char ch[202]={0};
    char ch_1[40]={0};
    BYTE byLoadDa[128] = {0};
    //unsigned char type = M_SP_TB;
    pdat_name ="fixpt";//"AU";
    unsigned char *pTxPos;
    WORD n,i;
    //BYTE max_num;
    //BYTE Loadday;
    BYTE YcIndex = 0;
    BYTE count;
    BYTE infoaddrl,infoaddrh;
    //WORD wLoadMeas[60] = {0};
    int offset =0;
    //BYTE g_YC_NUM =0;
    gRecorder_flag.LIST_flag = OFF;
    //WORD FileName;
      //char ch[250];
    WORD FileName;
    BYTE Current_Section;
    unsigned long FLoadAddrh;

    char chDevId[30]={0};
    memcpy(chDevId,g_pDevId,24);
    char *pDevID = chDevId;

    FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng],gRecorder_flag.pRXBuff[leng + 1]);//-1;
    Current_Section = gRecorder_flag.pRXBuff[leng + 2];
    unsigned long FLoadAddr = FADDR_LOAD_START + ((unsigned long)FileName * FLASH_DAYLOAD_LEN)+ ((unsigned long)(Current_Section-1) *FLASH_PLOAD_LEN);
    //CAT_SpiReadBytes(EEPADD_DAYNUM + FileName ,1,&Loadday);
    if((fixpt_recorded.fixpt_Ptr >= fixpt_recorded.fixpt_Curren_count ) &&(fixpt_recorded.fixpt_count_flag == ON))//���һ��Sample_num
    {
       pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
        //pTxBuf[wSecLenPtr] = 3;//LSQ=3 ����ֹͣ����Ľڴ���
            //GLubocfg_Sum = ChkluboSum(ComtrderCfg1, strlen(ComtrderCfg1));
        pTxBuf += leng;
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
        *pTxBuf++ = 3;//pTxBuf[wSecLenPtr]LSQ=3 ��ֹͣ����Ľڴ���
        *pTxBuf++ = fixpt_sum[Current_Section-1];
        //soe_recorded.soe_count = 0;
         //return pTxBuf;
    }
    else
    {
        pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= SEGMENT_TYPE;//���͵Ķ���Ϣ
        pTxBuf += leng;
       *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
        
      pTxPos = pTxBuf;
      *pTxBuf++  = 0;//SEGMENT_LENGTH;
        
        //for(int i=0;(wSendDATNum < (DAT_NUM - offset_time_Max));wSendDATNum++)//ÿһ�е�ֵSample_num
      if(fixpt_recorded.fixpt_count_flag == OFF)//(!soe_recorded.Soe_Area )&&(!soe_recorded.soe_count)&&(!soe_recorded.Soe_Ptr)
      {
        FLoadAddrh = FADDR_LOAD_START + ((unsigned long)FileName * FLASH_DAYLOAD_LEN);
        Sst26vf064b_Read(FLoadAddrh,byLoadDa,FLASH_PLOAD_LEN);
    CAT_SpiReadBytes(EEPADD_DAYNUM+FileName,1,&count);
        //sprintf((char *)ch,"%.*s,%02d%02d%02d,%02d,%02d\r\n",sizeof(pdat_name),pdat_name,byLoadDa[2],byLoadDa[3],byLoadDa[4],object_addr_length,YC_Type);
    sprintf((char *)ch,"%s%04d%02d%02d.msg,v1.0\r\n%s,%04d-%02d-%02d,%02d,%02d\r\n",pdat_name,byLoadDa[2]+2000,byLoadDa[3],byLoadDa[4],pDevID,byLoadDa[2]+2000,byLoadDa[3],byLoadDa[4],count,object_addr_length);

          fixpt_sum[Current_Section-1] =0;
          for(n = 0; n < strlen(ch); n++)
        {
            *pTxBuf++ = ch[n];
            fixpt_sum[Current_Section-1] += ch[n];
        }
       // CAT_SpiReadBytes(EEPADD_DAYNUM+FileName,1,&count);//fixpt_recorded.fixpt_count
        if(count >0)//fixpt_recorded.fixpt_count
          fixpt_recorded.fixpt_Curren_count =1;//���н��������ݣ����Ӧ�Ķ�����Ϊ1�����淢��ʱ�ټ������Ķ���
        fixpt_recorded.fixpt_Ptr =0;
          fixpt_recorded.fixpt_count_flag = ON;
        
      }//����δ��
      else if(fixpt_recorded.fixpt_count_flag == ON)
      {
        
        
                    Sst26vf064b_Read(FLoadAddr,byLoadDa,FLASH_PLOAD_LEN);
                //memcpy(wLoadMeas,&byLoadDa[8],FLASH_PLOAD_LEN-8);
            
                  for( i =fixpt_recorded.fixpt_count; i < g_ucYcTransNum; i++)   //������Ҫ�ϴ���ң��ֵ�����վ���������л���
                  {
                    if(g_ucYCAddr[i]!=255)
                {
                    YcIndex = g_ucYCAddr[i] - 1;
                      if(YcIndex <RM_1A_LUBO )//RM_1A_CABLE_T(YcIndex >= RM_BAT_U) && ((RM_1A_LOAD_I + RM_LOADI_NUM))
                      {      
                    infoaddrl=(i+ADDR_YC_LO)&0xff;
                infoaddrh=((i+ADDR_YC_LO)>>8)&0xff;
                //offset +=sprintf(ch + offset,"%02x%02x %02x%02x,",infoaddrl,infoaddrh,byLoadDa[8+YcIndex*2],byLoadDa[9+YcIndex*2]);//LOBYTE(wLoadMeas[YcIndex]), HIBYTE(wLoadMeas[YcIndex]));
                WORD YcValue = MAKEWORD(byLoadDa[8+YcIndex*2],byLoadDa[9+YcIndex*2]); 
         if(g_ucPara101[IECP_YC_TYPE] == 2) //�̸�����
                {
                    float YCValF = YcValue * GetYcCoef(YcIndex);              
                    offset +=sprintf(ch + offset,"%02x%02x,%08.2f,",infoaddrh,infoaddrl,YCValF);
                }
                else
                {
                    WORD YCValW = (WORD)(YcValue * GetYcCoef(YcIndex));             
                    offset +=sprintf(ch + offset,"%02x%02x,%05d,",infoaddrh,infoaddrl,YCValW);
                }
                      }
              
              if (strlen(ch) >= 180)//200
              {
                fixpt_recorded.fixpt_count ++;
                break;
              }
          
                }
            
            //else
              fixpt_recorded.fixpt_count ++;
              }
        if(fixpt_recorded.fixpt_count >= g_ucYcTransNum )//ÿһ�ں�ӻ���
        {
          ch[offset -1]='\r';
          ch[offset]='\n';
                 }              //ch[offset+1]='\0';
        if(fixpt_recorded.fixpt_Ptr ==0)
        {
          
          fix_YC_NUM =0;
          for( i = 0; i < g_ucYcTransNum; i++)   //������Ҫ�ϴ���ң��ֵ�����վ���������л���
              {
                  if(g_ucYCAddr[i]!=255)
                {
                    YcIndex = g_ucYCAddr[i] - 1;
                      if(YcIndex <RM_1A_LUBO )//RM_1A_CABLE_T(YcIndex >= RM_BAT_U) && ((RM_1A_LOAD_I + RM_LOADI_NUM))
                      
                        fix_YC_NUM++;   //m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(wLoadMeas[YcIndex]);
          
                }
              }
          //fixpt_recorded.fixpt_Curren_count = (fix_YC_NUM *10 + 1)/200 +1 ;//ÿ��ң��ռ���ֽ���Ϊ10 (0140 0000��),��1��ԭ�������һ��ң��㲻�ö��ţ��õ���/R/N//fixpt_recorded.fixpt_Curren_count= strlen(ch) / 200 +1;//�������ݶ���
          //sprintf((char *)ch_1,"%02d,%02d%02d%02d_%02d%02d%02d,",fix_YC_NUM,byLoadDa[2],byLoadDa[3],byLoadDa[4],byLoadDa[5],byLoadDa[6],byLoadDa[7]);
     if(g_ucPara101[IECP_YC_TYPE] == 2) //�̸�����
             fixpt_recorded.fixpt_Curren_count = (fix_YC_NUM *14 )/strlen(ch) +1 ;//
          else
              fixpt_recorded.fixpt_Curren_count = (fix_YC_NUM *11)/strlen(ch) +1 ;//180
         sprintf((char *)ch_1,"%02d,%04d-%02d-%02d %02d:%02d:%02d.%03d,",fix_YC_NUM,byLoadDa[2]+2000,byLoadDa[3],byLoadDa[4],byLoadDa[5],byLoadDa[6],byLoadDa[7],0); 
          for(n = 0; n < strlen(ch_1); n++)
          {
          
              *pTxBuf++ = ch_1[n];
              fixpt_sum[Current_Section-1] += ch_1[n];
          }
        }
        
        if(fixpt_recorded.fixpt_Ptr < fixpt_recorded.fixpt_Curren_count)
        {
          
            //if((strlen(ch) - (fixpt_recorded.fixpt_Ptr * 200)) > 200)
                //max_num = 200;
            //else
                //max_num = strlen(ch) - fixpt_recorded.fixpt_Ptr * 200;
            //for(n =(fixpt_recorded.fixpt_Ptr * 200),i=0; i< max_num; n++,i++)//n < strlen(ch),
            for(n =0;n<strlen(ch);n++)
            {
          
                *pTxBuf++ = ch[n];
                fixpt_sum[Current_Section-1] += ch[n];
              
            }
          fixpt_recorded.fixpt_Ptr++;
        }
          
         
        if(fixpt_recorded.fixpt_Ptr ==0)
        pTxBuf = NULL;
      }
      if(pTxBuf != NULL)
      {
      *pTxPos = pTxBuf - pTxPos-1;//LL
          
      }
      gRecorder_flag.LIST_flag = TRUE;//ON; 
  }
  return pTxBuf;
}



unsigned char * SoeData(unsigned char *pTxBuf,unsigned char leng,int segment_leng)
{
    
  char *pdat_name;
  //BYTE byCurSoeNum = 0;
  unsigned int object_addr_length =2;
  WORD second;
    WORD Millisecond,i;
    char ch[60]={0};
  #ifdef GETSOEFROMRAM
    BYTE bySoeDa[512];
  #else
    BYTE bySoeDa[255];
  #endif
    //unsigned char type = M_SP_TB;
    pdat_name="soe.msg,v1.0";//"AU";
    unsigned char *pTxPos;
    BYTE n;
    gRecorder_flag.LIST_flag = OFF;
    WORD byCurSoeNum=0;
    BYTE soe_num_max =4;//
    
    char chDevId[30]={0};
    memcpy(chDevId,g_pDevId,24);
    char *pDevID = chDevId;

    #ifndef GetSoeFromRam//���ⲿFLASHȡ����
    unsigned int Flashheadbk  =g_InFlashInfo.m_unSSoeInFlashHead;
    soe_num =  ReadNumSoeFromFlash((WORD *)&Flashheadbk,g_unSSoeInFlashTail);
   #endif
  //WORD FileName;
    //char ch[250];
  if(( soe_recorded.soe_count >= soe_num ) &&(soe_recorded.Soe_count_flag == ON))//���һ��Sample_num
  {
     pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
      //pTxBuf[wSecLenPtr] = 3;//LSQ=3 ����ֹͣ����Ľڴ���
          //GLubocfg_Sum = ChkluboSum(ComtrderCfg1, strlen(ComtrderCfg1));
      pTxBuf += leng;
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
      *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
      *pTxBuf++ = 3;//pTxBuf[wSecLenPtr]LSQ=3 ��ֹͣ����Ľڴ���
      *pTxBuf++ = soe_sum;
      soe_recorded.soe_count = 0;
       //return pTxBuf;
  }
  else
  {
    pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= SEGMENT_TYPE;//���͵Ķ���Ϣ
    pTxBuf += leng;
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
      
    pTxPos = pTxBuf;
    *pTxBuf++  = 0;//SEGMENT_LENGTH;
      
      //for(int i=0;(wSendDATNum < (DAT_NUM - offset_time_Max));wSendDATNum++)//ÿһ�е�ֵSample_num
    if(soe_recorded.Soe_count_flag == OFF)//(!soe_recorded.Soe_Area )&&(!soe_recorded.soe_count)&&(!soe_recorded.Soe_Ptr)
        {
          //sprintf((char *)ch,"%.*s,%04d,%02d\r\n",sizeof(pdat_name),pdat_name,soe_num,object_addr_length);
       sprintf((char *)ch,"%s\r\n%s,%03d,%02d\r\n",pdat_name,pDevID,soe_num,object_addr_length);
      soe_sum =0;
      for(n = 0; n < strlen(ch); n++)
          {
                    *pTxBuf++ = ch[n];
                    soe_sum += ch[n];
           }
      soe_recorded.Soe_count_flag = ON;
      #ifndef GETSOEFROMRAM
        BkFlashhead =g_InFlashInfo.m_unSSoeInFlashHead;
      #endif
    }
    else if(soe_recorded.Soe_count_flag == ON)
    {
      #ifdef GETSOEFROMRAM//n�ڲ�FLASHȡ
      if(soe_recorded.Soe_Ptr == soe_recorded.Soe_Curren_count)
                        {
                    
                          if(soe_recorded.Soe_Area < SOE_SEG)
                          {
                      soe_recorded.Soe_Curren_count = ReadSoe(bySoeDa,soe_recorded.Soe_Area,0,SOE_NUM_SEG);
                        soe_recorded.Soe_Area++;
                        soe_recorded.Soe_Ptr = 0;
                      
                            }
          
                          }
      else
      {
          ReadSoe(bySoeDa,soe_recorded.Soe_Area-1,0,SOE_NUM_SEG);
      }
      
      
            
              if(soe_recorded.Soe_Curren_count - soe_recorded.Soe_Ptr > 4) 
                  byCurSoeNum = 4;
              else
                  byCurSoeNum = (soe_recorded.Soe_Curren_count - soe_recorded.Soe_Ptr);
             
            /*if(soe_recorded.soe_count + byCurSoeNum >= soe_num)
              *pTxBuf++ = 0;//0:�޺��� 1:�к���
             else
              *pTxBuf++ = 1;//0:�޺��� 1:�к���*/
              for(i =0 ;i< byCurSoeNum;i++)
              {
                  second = (MAKEWORD(bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+4],bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+5]))/1000;//+soe_recorded.Soe_Ptr
                  Millisecond = (MAKEWORD(bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+4],bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+5]))%1000;//+soe_recorded.Soe_Ptr
                  sprintf((char *)ch,"%d %02x %02x %d %02d%02d%02d_%02d%02d%02d_%03d\r\n",type,MAKEWORD(bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN],bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+1])+1,(MAKEWORD(bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN],bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+1])+1)>>8,bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+2],bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+9], bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+8], bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+7], bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+6],bySoeDa[(i+soe_recorded.Soe_Ptr)*SOE_SENDDA_LEN+5],
                  second,Millisecond);
              
                  for(n = 0; n < strlen(ch); n++)
                  {
              
        *pTxBuf++ = ch[n];
         soe_sum += ch[n];
                  }
              }
              soe_recorded.Soe_Ptr += byCurSoeNum;
              soe_recorded.soe_count +=byCurSoeNum;
              //*pTxPs = pTxBuf - pTxPs-1;
              if(soe_recorded.Soe_Curren_count ==0)
              pTxBuf = NULL;
            #else
            
                      
        //if(g_Cmid == g_CmIdGPRS)
          //byCurSoeNum =  pGprs->ReadSoeFromFlash((WORD *)&BkFlashhead,g_unSSoeInFlashTail,soe_num_max,bySoeDa);
        //else if(g_Cmid == g_CmIdDBG)
        if(g_Cur101Obj !=NULL)
        byCurSoeNum =  g_Cur101Obj->ReadSoeFromFlash((WORD *)&BkFlashhead,g_unSSoeInFlashTail,soe_num_max,bySoeDa);
      soe_recorded.Soe_Ptr = 0;
      for(i =0 ;i< byCurSoeNum;i++)
            {
                second = (MAKEWORD(bySoeDa[(i)*SOE_DATA_LEN+4],bySoeDa[(i)*SOE_DATA_LEN+5]))/1000;//+soe_recorded.Soe_Ptr
                Millisecond = (MAKEWORD(bySoeDa[(i)*SOE_DATA_LEN+4],bySoeDa[(i)*SOE_DATA_LEN+5]))%1000;//+soe_recorded.Soe_Ptr
         WORD YXValue = bySoeDa[(i)*SOE_DATA_LEN+3];
                 if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_SHUANGDIAN_SET-16]))// if(g_ucPara101[IECP_YX_TYPE])  
                      YXValue = YXValue ? 2 : 1;//ת����˫����ʾ
                //sprintf((char *)ch,"%d %02x %02x %d %02d%02d%02d_%02d%02d%02d_%03d\r\n",type,MAKEWORD(bySoeDa[(i)*SOE_DATA_LEN+1],bySoeDa[(i)*SOE_DATA_LEN+2])+1,(MAKEWORD(bySoeDa[(i)*SOE_DATA_LEN+1],bySoeDa[(i)*SOE_DATA_LEN+2])+1)>>8,bySoeDa[(i)*SOE_DATA_LEN+3],bySoeDa[(i)*SOE_DATA_LEN+10], bySoeDa[(i)*SOE_DATA_LEN+9], bySoeDa[(i)*SOE_DATA_LEN+8], bySoeDa[(i)*SOE_DATA_LEN+7],bySoeDa[(i)*SOE_DATA_LEN+6],
          sprintf((char *)ch,"%04x,%d,%04d-%02d-%02d %02d:%02d:%02d.%03d\r\n",MAKEWORD(bySoeDa[(i)*SOE_DATA_LEN+1],bySoeDa[(i)*SOE_DATA_LEN+2])+1,YXValue,bySoeDa[(i)*SOE_DATA_LEN+10]+2000, bySoeDa[(i)*SOE_DATA_LEN+9], bySoeDa[(i)*SOE_DATA_LEN+8], bySoeDa[(i)*SOE_DATA_LEN+7],bySoeDa[(i)*SOE_DATA_LEN+6],
                second,Millisecond);
              
                for(n = 0; n < strlen(ch); n++)
                {
              
            *pTxBuf++ = ch[n];
            soe_sum += ch[n];
                }
            }
      soe_recorded.soe_count +=byCurSoeNum;
      if (byCurSoeNum == 0)
            pTxBuf = NULL;  
      #endif
           
        }
      if(pTxBuf != NULL)
      {
    *pTxPos = pTxBuf - pTxPos-1;//LL
              
      }
    gRecorder_flag.LIST_flag = TRUE;//ON; 
  }
  return pTxBuf;
}



//***********************************************************************************
//*��������FileDatadat
//*���ܣ��ļ�2���䣬��Ӧ.dat�ļ�
//*����: ���ݴ��䣬һ��һ�еĴ�����
//***********************************************************************************

unsigned char *  FileDatadat(unsigned char *pTxBuf,unsigned char leng,WORD wSecLenPtr,int segment_leng)
{
    
    #ifndef FILTER_COM
    unsigned char datBuff[16] = {0};//datBuff[16] = {0}; //8��ͨ��,ÿ��ͨ������ֵ��2��
    unsigned int offset[4]={0};
    BYTE comtrade_length = 22;//14+8 comtrade_length = 24;//16+8
    #else
    unsigned char datBuff[20] = {0}; //8��ͨ��,ÿ��ͨ������ֵ��2��
    unsigned int offset[5]={0};
    BYTE comtrade_length = 28;//20+8
    #endif
    unsigned char datBuff_BK[30] = {0}; 
  
    int ia,ib,ic,i0,ua,ub,uc,u0;
    //unsigned int com_length;
    //unsigned int Sample_num = CRG_SAMPLE_NUM;
    gRecorder_flag.LIST_flag = OFF;
    WORD FileName;
    char ch[250]={0};
   // FP32 originaBuf_phase_coeffici[3]={1};
    unsigned int  offset_time_Max =0;
    unsigned char *pTxPos;
    unsigned int  DAT_NUM;
    unsigned int  Total_lubo;
  unsigned int offset_num;
    int mid_Value;
  unsigned int start_point ;
   // unsigned int wSendDAT = 0;
    FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng],gRecorder_flag.pRXBuff[leng + 1]);//-1;
    DAT_NUM = DAT_AD_NUM;
    CAT_SpiReadWords(EEPADD_ALEN + FileName*EELEN_LENG, 3, offset);
    CAT_SpiReadWords(EEPADD_COMLEN + FileName*EELEN_LENG, 1, &offset_time_Max); 
  if(g_gRunPara[RP_LUBO_CYCLE] ==0)//||(offset_time_Max ==0))//��ʱû����
    Total_lubo = DAT_NUM-offset_time_Max;
  else
    Total_lubo = (g_gRunPara[RP_LUBO_CYCLE] * 80);
    //for(int j=0;j<3;j++)
 //   CAT_SpiReadBytes(EEPADD_COEFFICIENT+COEFFICIENT_LINE_LENG*FileName+COEFFICIENT_PHASE_LENG*j, COEFFICIENT_PHASE_LENG, (unsigned char*)&originaBuf_phase_coeffici[j]);
    if( wSendDATNum >= Total_lubo)//(DAT_NUM-offset_time_Max) ���һ��Sample_num
    {
      pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= LASTSECTION_TYPE;//���͵�������Ϣ
      //pTxBuf[wSecLenPtr] = 3;//LSQ=3 ����ֹͣ����Ľڴ���
              //GLubocfg_Sum = ChkluboSum(ComtrderCfg1, strlen(ComtrderCfg1));
       pTxBuf += leng;
       *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
       *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
       *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
       *pTxBuf++ = 3;//pTxBuf[wSecLenPtr]LSQ=3 ��ֹͣ����Ľڴ���
       *pTxBuf++ = GLubodat_Sum;
       wSendDATNum = 0;
       //return pTxBuf;
     }
     else
     {
        pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]]= SEGMENT_TYPE;//���͵Ķ���Ϣ
         pTxBuf += leng;
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 2];//������
        
         pTxPos = pTxBuf;
        *pTxBuf++  = 0;//SEGMENT_LENGTH;
        
        for(int i=0;(wSendDATNum < Total_lubo);wSendDATNum++)//(DAT_NUM - offset_time_Max)ÿһ�е�ֵSample_num
        {
          #ifndef FILTER_COM     
          for(int j=0;j<4;j++)//ABC0����
          {
            
    if(Total_lubo == (DAT_NUM-offset_time_Max))
    {
      unsigned long FLbAddrI = FADDR_RECORDER_START+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)(offset[j]*2);
                Sst26vf064b_Read(FLbAddrI,&datBuff[j*2],2);//WRITEPECORDER+����ÿָֻʾ���ĵ�һ�����ݴ������Ϣ
                unsigned long FLbAddrV = FADDR_RECORDER_STARTV+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)(offset[j]*2);
                Sst26vf064b_Read(FLbAddrV,&datBuff[j*2+8],2);//WRITEPECORDER+��ѹÿָֻʾ���ĵ�һ�����ݴ������Ϣ

          if(wSendDATNum > Total_lubo)//DAT_NUM - offset_time_Max�ഫ��������0����Ϊ��flash��ȡ���������ݶ���-1
                {
                  datBuff[j*2]=0;
                  datBuff[j*2+1]=0;
                  datBuff[j*2+8]=0;
                  datBuff[j*2+9]=0;
                }
    }
    else if(Total_lubo < (DAT_NUM-offset_time_Max))
    {
      offset_num = ((DAT_NUM-offset_time_Max) - Total_lubo)>>1;
      unsigned long FLbAddrI = FADDR_RECORDER_START+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)((offset[j] +offset_num)*2);
                Sst26vf064b_Read(FLbAddrI,&datBuff[j*2],2);//WRITEPECORDER+����ÿָֻʾ���ĵ�һ�����ݴ������Ϣ
                unsigned long FLbAddrV = FADDR_RECORDER_STARTV+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)((offset[j] +offset_num)*2);
                Sst26vf064b_Read(FLbAddrV,&datBuff[j*2+8],2);//WRITEPECORDER+��ѹÿָֻʾ���ĵ�һ�����ݴ������Ϣ
    }
    else
    {
      offset_num = Total_lubo-(DAT_NUM-offset_time_Max);
        
      if(wSendDATNum < DAT_NUM-offset_time_Max)
      {
        unsigned long FLbAddrI = FADDR_RECORDER_START+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)(offset[j]*2);
                  Sst26vf064b_Read(FLbAddrI,&datBuff[j*2],2);//WRITEPECORDER+����ÿָֻʾ���ĵ�һ�����ݴ������Ϣ
                  unsigned long FLbAddrV = FADDR_RECORDER_STARTV+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)(offset[j]*2);
                  Sst26vf064b_Read(FLbAddrV,&datBuff[j*2+8],2);//WRITEPECORDER+��ѹÿָֻʾ���ĵ�һ�����ݴ������Ϣ
      }
      else
      {
          
        if((offset_num + (80 - offset_num % 80)) < (DAT_NUM-offset_time_Max-1))
        {
      start_point = (DAT_NUM-offset_time_Max)-(offset_num + (80 - offset_num % 80))+(wSendDATNum - (DAT_NUM-offset_time_Max));//-1
      unsigned long FLbAddrI = FADDR_RECORDER_START+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(start_point*2)+(unsigned long)(offset[j]*2);
                Sst26vf064b_Read(FLbAddrI,&datBuff[j*2],2);//WRITEPECORDER+����ÿָֻʾ���ĵ�һ�����ݴ������Ϣ
                unsigned long FLbAddrV = FADDR_RECORDER_STARTV+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(start_point*2)+(unsigned long)(offset[j]*2);
                Sst26vf064b_Read(FLbAddrV,&datBuff[j*2+8],2);//WRITEPECORDER+��ѹÿָֻʾ���ĵ�һ�����ݴ������Ϣ
        }
          

      }
        
          }
              //if(!(datBuff[j*2+1] &0x08))
              //wSendDAT =1;
          }
         mid_Value = MAKEWORD(datBuff[0],datBuff[1]);
         //ia = mid_Value;//(long)(I_Value) * mid_Value/10000;
         if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_ACURRENT_REVERSE-16]))
          ia =0-((long)(g_gRunPara[RP_CURRENT_CA]) * mid_Value/1000);
         else
          ia = (long)(g_gRunPara[RP_CURRENT_CA]) * mid_Value/1000;
         mid_Value = MAKEWORD(datBuff[2],datBuff[3]);
         //ib = mid_Value;//(long)(I_Value) * mid_Value/10000;
          if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_BCURRENT_REVERSE-16]))
          ib =0-((long)(g_gRunPara[RP_CURRENT_CB]) * mid_Value/1000);
         else
          ib = (long)(g_gRunPara[RP_CURRENT_CB]) * mid_Value/1000;
         
         mid_Value = MAKEWORD(datBuff[4],datBuff[5]);
         //ic = mid_Value;//(long)(I_Value) * mid_Value/10000;
         if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_CCURRENT_REVERSE-16]))
          ic = 0-((long)(g_gRunPara[RP_CURRENT_CC]) * mid_Value/1000);
         else
          ic = (long)(g_gRunPara[RP_CURRENT_CC]) * mid_Value/1000;
          i0 = ia + ib+ ic;
         /*mid_Value = MAKEWORD(datBuff[6],datBuff[7]);
         if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_CURRENT_REVERSE-16]))
           i0 = 0-mid_Value;
         else
          i0 = mid_Value;*///(long)(I_Value) * mid_Value/10000;
           mid_Value = MAKEWORD(datBuff[8],datBuff[9]);
            if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_AVOLTAGE_REVERSE-16]))
               ua = 0-((long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000);
            else
         ua = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;
            
     mid_Value = MAKEWORD(datBuff[10],datBuff[11]);
            if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_BVOLTAGE_REVERSE-16])) 
              ub = 0-((long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000);
            else
        ub = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;
      mid_Value = MAKEWORD(datBuff[12],datBuff[13]);
            if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_CVOLTAGE_REVERSE-16])) 
     uc = 0-((long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000);
            else
                 uc = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;
            u0 = ua+ub+uc;
    /* mid_Value = MAKEWORD(datBuff[14],datBuff[15]);
     u0 = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;*/
          //if(wSendDATNum >917)  
           //wSendDAT=(unsigned int)wSendDATNum*250;
          /*sprintf((char *)ch,"%u,%lu,%d,%d,%d,%d,%d,%d,%d,%d\n",wSendDATNum,(unsigned long)wSendDATNum*250,MAKEWORD(datBuff[0],datBuff[1]),MAKEWORD(datBuff[2],datBuff[3]),
                                 MAKEWORD(datBuff[4],datBuff[5]),MAKEWORD(datBuff[6],datBuff[7]),MAKEWORD(datBuff[8],datBuff[9]),MAKEWORD(datBuff[10],datBuff[11]),
          
            MAKEWORD(datBuff[12],datBuff[13]),MAKEWORD(datBuff[14],datBuff[15]));//ÿ������80���㣬��������250΢��*/
          WORD DatNum = wSendDATNum +1;
          if(g_gRunPara[RP_CFG_KEY] & BIT[RP_COMTRADE_TYPE])
          {
              
               datBuff_BK[0] = LOBYTE(DatNum);
               datBuff_BK[1] = HIBYTE(DatNum);
               datBuff_BK[2] = 0;
               datBuff_BK[3] = 0;
               datBuff_BK[4] = LOBYTE(LOWORD((unsigned long)wSendDATNum*250));
               datBuff_BK[5] = HIBYTE(LOWORD((unsigned long)wSendDATNum*250));
               datBuff_BK[6]  = LOBYTE(HIWORD((unsigned long)wSendDATNum*250));
               datBuff_BK[7]  = HIBYTE(HIWORD((unsigned long)wSendDATNum*250));
               datBuff_BK[8]  = LOBYTE(ua);//LOBYTE(ia); 
               datBuff_BK[9]  = HIBYTE(ua);//HIBYTE(ia);
               datBuff_BK[10]  = LOBYTE(ub);//LOBYTE(ib);
               datBuff_BK[11]  = HIBYTE(ub);//HIBYTE(ib);
               datBuff_BK[12]  = LOBYTE(uc);//LOBYTE(ic);
               datBuff_BK[13] = HIBYTE(uc);//HIBYTE(ic);
               datBuff_BK[14]  = LOBYTE(ia); //LOBYTE(i0); 
               datBuff_BK[15]  = HIBYTE(ia);//HIBYTE(i0);

             datBuff_BK[16]  = LOBYTE(ib); //LOBYTE(ua); 
               datBuff_BK[17]  = HIBYTE(ib);// HIBYTE(ua);
               datBuff_BK[18]  = LOBYTE(ic);//LOBYTE(ub);
               datBuff_BK[19]  = HIBYTE(ic);//HIBYTE(ub);
               datBuff_BK[20]  = LOBYTE(i0);//LOBYTE(uc);
               datBuff_BK[21] = HIBYTE(i0);//HIBYTE(uc);
               //datBuff_BK[22]  = LOBYTE(u0); 
               //datBuff_BK[23]  = HIBYTE(u0);
               /*for(int n=8;n<16;n++)
               {
                 datBuff_BK[n+8]= datBuff[n];
               }*/
               
          }
          else
          {
            sprintf((char *)ch,"%u,%lu,%d,%d,%d,%d,%d,%d,%d\r\n",DatNum,(unsigned long)wSendDATNum*250,ua,ub,uc,ia,ib,ic,i0);
            //sprintf((char *)ch,"%u,%lu,%d,%d,%d,%d,%d,%d,%d,%d\r\n",wSendDATNum,(unsigned long)wSendDATNum*250,ia,ib,
                                // ic,i0,ua,ub,uc,u0);//,MAKEWORD(datBuff[8],datBuff[9]),MAKEWORD(datBuff[10],datBuff[11]),
                                 //MAKEWORD(datBuff[12],datBuff[13]),MAKEWORD(datBuff[14],datBuff[15]));//ÿ������80���㣬��������250΢��
          }
          #else
          //int I_Value = 1414;
          for(int j=0;j<5;j++)//ABC0����
          {
            if(j<4)
            {
            unsigned long FLbAddrI = FADDR_RECORDER_START+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)(offset[j]*2);
            Sst26vf064b_Read(FLbAddrI,&datBuff[j*2],2);//WRITEPECORDER+����ÿָֻʾ���ĵ�һ�����ݴ������Ϣ
            
            unsigned long FLbAddrV = FADDR_RECORDER_STARTV+(unsigned long)FileName*FLINEADDR+(unsigned long)j*FPHASEADDR+(unsigned long)(wSendDATNum*2)+(unsigned long)(offset[j]*2);
            Sst26vf064b_Read(FLbAddrV,&datBuff[j*2+10],2);//WRITEPECORDER+��ѹÿָֻʾ���ĵ�һ�����ݴ������Ϣ
            }
            else
            {
            unsigned long FLbAddrI = FADDR_WAVE_FILTERI+(unsigned long)FileName*FPHASEADDR+(unsigned long)(wSendDATNum*2);//+(unsigned long)j*FPHASEADDR+(unsigned long)(offset[j]*2);
            Sst26vf064b_Read(FLbAddrI,&datBuff[8],2);//WRITEPECORDER+����ÿָֻʾ���ĵ�һ�����ݴ������Ϣ
            
            unsigned long FLbAddrV = FADDR_WAVE_FILTERV+(unsigned long)FileName*FPHASEADDR+(unsigned long)(wSendDATNum*2);//(unsigned long)j*FPHASEADDR++(unsigned long)(offset[j]*2);
            Sst26vf064b_Read(FLbAddrV,&datBuff[18],2);//WRITEPECORDER+��ѹÿָֻʾ���ĵ�һ�����ݴ������Ϣ
            }
            if(wSendDATNum > Total_lubo)//DAT_NUM - offset_time_Max�ഫ��������0����Ϊ��flash��ȡ���������ݶ���-1
            {
                if(j<4)
                {
                  datBuff[j*2]=0;
                  datBuff[j*2+1]=0;
                  datBuff[j*2+10]=0;
                  datBuff[j*2+11]=0;
                }
                else
                {
                  datBuff[18]=0;
                  datBuff[19]=0;
                  datBuff[8]=0;
                  datBuff[9]=0;
                }
            }
            
              //if(!(datBuff[j*2+1] &0x08))
              //wSendDAT =1;
          }
          //if(wSendDATNum >917)  
           //wSendDAT=(unsigned int)wSendDATNum*250;
         /*mid_Value = MAKEWORD(datBuff[0],datBuff[1]);
         ia = mid_Value;//(long)(I_Value) * mid_Value/10000;
         mid_Value = MAKEWORD(datBuff[2],datBuff[3]);
         ib = mid_Value;// (long)(I_Value) * mid_Value/10000;
         mid_Value = MAKEWORD(datBuff[4],datBuff[5]);
         ic = mid_Value;//(long)(I_Value) * mid_Value/10000;*/
          mid_Value = MAKEWORD(datBuff[0],datBuff[1]);
          if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_ACURRENT_REVERSE-16]))
          ia =0-((long)(g_gRunPara[RP_CURRENT_CA]) * mid_Value/1000);
         else
          ia = (long)(g_gRunPara[RP_CURRENT_CA]) * mid_Value/1000;
         mid_Value = MAKEWORD(datBuff[2],datBuff[3]);
         //ib = mid_Value;//(long)(I_Value) * mid_Value/10000;
          if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_BCURRENT_REVERSE-16]))
          ib =0-((long)(g_gRunPara[RP_CURRENT_CB]) * mid_Value/1000);
         else
          ib = (long)(g_gRunPara[RP_CURRENT_CB]) * mid_Value/1000;
         
         mid_Value = MAKEWORD(datBuff[4],datBuff[5]);
         //ic = mid_Value;//(long)(I_Value) * mid_Value/10000;
         if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_CCURRENT_REVERSE-16]))
          ic = 0-((long)(g_gRunPara[RP_CURRENT_CC]) * mid_Value/1000);
         else
          ic = (long)(g_gRunPara[RP_CURRENT_CC]) * mid_Value/1000;
          i0 = ia + ib+ ic;
      
  mid_Value = MAKEWORD(datBuff[10],datBuff[11]);
        if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_AVOLTAGE_REVERSE-16]))
          ua = 0-((long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000);
        else
    ua = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;
  mid_Value = MAKEWORD(datBuff[12],datBuff[13]);
        if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_BVOLTAGE_REVERSE-16]))
          ub = 0-((long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000);
        else
    ub = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;
  mid_Value = MAKEWORD(datBuff[14],datBuff[15]);
        if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_CVOLTAGE_REVERSE-16]))
          uc = 0-((long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000);
        else
    uc = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;
        
         u0 = ua+ub+uc;
  /* mid_Value = MAKEWORD(datBuff[16],datBuff[17]);
    u0 = (long)(g_gRunPara[RP_VOLTAGE_V0]) * mid_Value/1000;
         mid_Value = MAKEWORD(datBuff[6],datBuff[7]);
         if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_CURRENT_REVERSE-16]))
           i0 = 0-mid_Value;
         else
          i0 = mid_Value;//(long)(I_Value) * mid_Value/10000;
         //ia = mid_Value;//(long)(I_Value) * mid_Value/10000;
         ia = (long)(g_gRunPara[RP_CURRENT_CA]) * mid_Value/1000;
         mid_Value = MAKEWORD(datBuff[2],datBuff[3]);
         //ib = mid_Value;//(long)(I_Value) * mid_Value/10000;
         ib = (long)(g_gRunPara[RP_CURRENT_CB]) * mid_Value/1000;
         mid_Value = MAKEWORD(datBuff[4],datBuff[5]);
         //ic = mid_Value;//(long)(I_Value) * mid_Value/10000;
         ic = (long)(g_gRunPara[RP_CURRENT_CC]) * mid_Value/1000;
         mid_Value = MAKEWORD(datBuff[6],datBuff[7]);
         i0 = mid_Value;//(long)(I_Value) * mid_Value/10000;
       
          sprintf((char *)ch,"%u,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",wSendDATNum,(unsigned long)wSendDATNum*250,MAKEWORD(datBuff[0],datBuff[1]),MAKEWORD(datBuff[2],datBuff[3]),
                                 MAKEWORD(datBuff[4],datBuff[5]),MAKEWORD(datBuff[6],datBuff[7]),MAKEWORD(datBuff[8],datBuff[9]),MAKEWORD(datBuff[10],datBuff[11]),
                                 MAKEWORD(datBuff[12],datBuff[13]),MAKEWORD(datBuff[14],datBuff[15]),MAKEWORD(datBuff[16],datBuff[17]),MAKEWORD(datBuff[18],datBuff[19]));//ÿ������80���㣬��������250΢��*/
            WORD DatNum = wSendDATNum +1;
            if(g_gRunPara[RP_CFG_KEY] & BIT[RP_COMTRADE_TYPE])
            {
                datBuff_BK[0]= LOBYTE(DatNum);
                datBuff_BK[1] = HIBYTE(DatNum);
                datBuff_BK[2] = 0;
                datBuff_BK[3] = 0;
               
                datBuff_BK[4]= LOBYTE(LOWORD((unsigned long)wSendDATNum*250));
                datBuff_BK[5]= HIBYTE(LOWORD((unsigned long)wSendDATNum*250));
                datBuff_BK[6] = LOBYTE(HIWORD((unsigned long)wSendDATNum*250));
                datBuff_BK[7] = HIBYTE(HIWORD((unsigned long)wSendDATNum*250));
                datBuff_BK[8]= LOBYTE(ia);
                datBuff_BK[9] = HIBYTE(ia);
                datBuff_BK[10] = LOBYTE(ib); 
                datBuff_BK[11] = HIBYTE(ib);
                datBuff_BK[12] = LOBYTE(ic); 
                datBuff_BK[13] = HIBYTE(ic);
                datBuff_BK[14] = LOBYTE(i0);
                datBuff_BK[15] = HIBYTE(i0);
    datBuff_BK[16] = datBuff[8];
    datBuff_BK[17] = datBuff[9];
         datBuff_BK[18]  = LOBYTE(ua); 
               datBuff_BK[19]  = HIBYTE(ua);
               datBuff_BK[20]  = LOBYTE(ub);
               datBuff_BK[21]  = HIBYTE(ub);
               datBuff_BK[22]  = LOBYTE(uc);
               datBuff_BK[23] = HIBYTE(uc);
               datBuff_BK[24]  = LOBYTE(u0); 
               datBuff_BK[25]  = HIBYTE(u0);
    datBuff_BK[26]  = datBuff[18];; 
               datBuff_BK[27]  = datBuff[19];;
               /*for(int n=8;n<20;n++)
               {
                 datBuff_BK[8+n] = datBuff[n];
                 
               }*/
               
            }
            else
            {
              sprintf((char *)ch,"%u,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",DatNum,(unsigned long)wSendDATNum*250,ia,ib,
                                 ic,i0,MAKEWORD(datBuff[8],datBuff[9]),ua,ub,uc,u0,MAKEWORD(datBuff[18],datBuff[19]));//ÿ������80���㣬��������250΢��MAKEWORD(datBuff[10],datBuff[11]),
                                 //MAKEWORD(datBuff[12],datBuff[13]),MAKEWORD(datBuff[14],datBuff[15]),MAKEWORD(datBuff[16],datBuff[17]),
            }
                                 

          #endif
          if(g_gRunPara[RP_CFG_KEY] & BIT[RP_COMTRADE_TYPE])
          {
            i+=comtrade_length;
          }
          else
          {
            i+= strlen(ch);
          }
          if(i <=segment_leng)
          {
            if(!(g_gRunPara[RP_CFG_KEY] & BIT[RP_COMTRADE_TYPE]))
            {
              for(BYTE n = 0; n < strlen(ch); n++)
                {
                    *pTxBuf++ = ch[n];
                    GLubodat_Sum += ch[n];
                }
            }
            else
            {
              for(BYTE n = 0; n < comtrade_length; n++)
                {
                    *pTxBuf++ = datBuff_BK[n];
                    GLubodat_Sum += datBuff_BK[n];
                }
            }
            *pTxPos = pTxBuf - pTxPos-1;//LL
            gRecorder_flag.LIST_flag = TRUE;//ON;     
            //GLubodat_Sum += ChkluboSum((BYTE *)ch, strlen(ch));  
          }
          else
          {
            
            //wSendDATNum--;
            break;//return pTxBuf;
          }
          if(wSendDATNum == Total_lubo-1)//DAT_NUM -offset_time_MaxSample_num-1
          {
            wSendDATNum++;
            break;//return pTxBuf;
          }
             
        }
        
     }
     return pTxBuf;
 }
//***********************************************************************************
//*��������SectionPrepareFile
//*���ܣ���׼����������
//*��������վ����ڴ��䣬�ն˻ظ���׼����������Ҫ��������ڵĳ���
//***********************************************************************************
unsigned char *  SectionPrepareFile(unsigned char *pTxBuf,unsigned char leng,RECORDER_CFG *pgRecorder_cfg)
{
  if(pTxBuf[leng + 3]==2)  //�����ļ�����SRQ=0
  {
    pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = SECTIONPREPAR_TYPE;
     pTxBuf += leng;   
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
    *pTxBuf++ = section_current;//��ǰ������
          //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
    if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
    {
      *pTxBuf++ = pgRecorder_cfg->CFG_Leng;
      *pTxBuf++ = pgRecorder_cfg->CFG_Leng>>8;  
    }
    else//����.DAT�ڵĳ���
    {
       
       *pTxBuf++ = pgRecorder_cfg->DATA_Leng;
       *pTxBuf++ = pgRecorder_cfg->DATA_Leng>>8;  
    }
       *pTxBuf++ = 0;//SRQ=0,��һ��׼������
  }
  return pTxBuf;
}


//***********************************************************************************
//*��������SectionPrepareFile
//*���ܣ���׼����������
//*��������վ����ڴ��䣬�ն˻ظ���׼����������Ҫ��������ڵĳ���
//***********************************************************************************
unsigned char *  SectionPrepareSoe(unsigned char *pTxBuf,unsigned char leng)
{
  if(pTxBuf[leng + 3]==2)  //�����ļ�����SRQ=0
  {
    pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = SECTIONPREPAR_TYPE;
     pTxBuf += leng;   
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
    *pTxBuf++ = section_current;//��ǰ������
          //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
    if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
    {
      *pTxBuf++ = TOTAL_Leng;//pgRecorder_cfg->CFG_Leng;
      *pTxBuf++ = TOTAL_Leng>>8; //pgRecorder_cfg->CFG_Leng>>8;  
    }
   /* else//����.DAT�ڵĳ���
    {
       
       *pTxBuf++ = pgRecorder_cfg->DATA_Leng;
       *pTxBuf++ = pgRecorder_cfg->DATA_Leng>>8;  
    }*/
       *pTxBuf++ = 0;//SRQ=0,��һ��׼������
  }
  return pTxBuf;
}

unsigned char *SectionPrepareLog(unsigned char *pTxBuf,unsigned char leng)
{

    if(pTxBuf[leng + 3]==2)  //�����ļ�����SRQ=0
    {
    pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = SECTIONPREPAR_TYPE;
     pTxBuf += leng;   
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
    *pTxBuf++ = section_current;//��ǰ������
        //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
  //  if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
    {
      *pTxBuf++ = Log_TOTAL_Leng;//46 +FLASH_PLOAD_LEN;//9 +FLASH_PLOAD_LEN;
      *pTxBuf++ = Log_TOTAL_Leng>>8;//(46 +FLASH_PLOAD_LEN)>>8;//(9 +FLASH_PLOAD_LEN)>>8;  
    }
    /*else//����.DAT�ڵĳ���
    {
       
        *pTxBuf++ = FLASH_PLOAD_LEN;
      *pTxBuf++ = FLASH_PLOAD_LEN>>8;   
    }*/
       *pTxBuf++ = 0;//SRQ=0,��һ��׼������
    }
    return pTxBuf;
}


unsigned char *  SectionPrepareFixpt(unsigned char *pTxBuf,unsigned char leng)
{

    if(pTxBuf[leng + 3]==2)  //�����ļ�����SRQ=0
    {
    pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = SECTIONPREPAR_TYPE;
     pTxBuf += leng;   
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
    *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];
    *pTxBuf++ = section_current;//��ǰ������
        //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
    if(section_current == 1)//������Ϊ0����.CFG�Ľڳ���
    {
      *pTxBuf++ = Fixpt_SECTION_Leng&0x00FF;
      *pTxBuf++ = (Fixpt_SECTION_Leng>>8)&0x00FF; 
    }
    else
    {
      *pTxBuf++ = (Fixpt_SECTION_Leng-67)&0x00FF;
      *pTxBuf++ = ((Fixpt_SECTION_Leng-67)>>8)&0x00FF;  
    }
    /*else//����.DAT�ڵĳ���
    {
       
       *pTxBuf++ = pgRecorder_cfg->DATA_Leng;
       *pTxBuf++ = pgRecorder_cfg->DATA_Leng>>8;  
    }*/
       *pTxBuf++ = 0;//SRQ=0,��һ��׼������
    }
    return pTxBuf;
}


unsigned char LuboGetCtrCode(BYTE control)
 {
  //BYTE PRM = 0;
    //BYTE dwCode = 3;
        BYTE Num = 1;
  BOOL m_DIR;  /*get DIR */
  //CodeTmp= control;
        BYTE lm_fcb;
  if(control & BITS_DIR)
          m_DIR=1;/*get��վdir*/
        else 
          m_DIR=0;
  
        if(m_DIR)//DIR auto fit
          control&=0x7f;
        else 
          control|=0x80;
         control&=0xcf;
        if(Num)
        {
            if(g_Cur101Obj !=NULL)
            {
                if(g_Cur101Obj->m_fcb ==0)//lm_fcb==0)
                {
                //lm_fcb=0x20;
                  g_Cur101Obj->m_fcb =0x20;
                }
                else
                {
                  //lm_fcb=0;
                  g_Cur101Obj->m_fcb =0;
                }
                lm_fcb=g_Cur101Obj->m_fcb;
                //control|=(((CBJ101S *)pGprs)->m_fcb|0x10);
           }
            /*else if(g_Cmid == g_CmIdDBG)
      {
     if(((CBJ101S *)pDbg)->m_fcb ==0)//lm_fcb==0)
                 {
                 //lm_fcb=0x20;
                   ((CBJ101S *)pDbg)->m_fcb =0x20;
                 }
                else
                  {
                    //lm_fcb=0;
                   ((CBJ101S *)pDbg)->m_fcb =0; 
                  }
                  lm_fcb = ((CBJ101S *)pDbg)->m_fcb;
                 // control|=((((CBJ101S *)pDbg)->m_fcb)|0x10);
      }*/
           control|=(lm_fcb|0x10);
            
        }
        return control;
 }


//unsigned long FLbAddrStart;
//unsigned long FLbAddrEnd;
//***********************************************************************************
//*��������Code_Lubo
//*���ܣ�����Ӧ��ASDU����������Ӧ
//*������¼���ļ��Ĵ���
//***********************************************************************************
void Code_Lubo(unsigned char *pRxBuf,unsigned char *pTXBuff)
{//1
  unsigned char *pTxBuf = pTXBuff;//���¶Է��ͻ���д������
  
  unsigned int wave_total;
  //unsigned int lubo_total;
  
  unsigned char leng;
 
  WORD FileName = 0;//�ļ����
//  BYTE lubo_valid[64];
  WORD wSecLenPtr=0; 
  WORD Info_val;
  BYTE off=0;
  RECORDER_CFG mRecorder_cfg;
  off=7+g_ucPara101[IECP_LINKADDR_NUM]+g_ucPara101[IECP_LINKADDR_NUM]+g_ucPara101[IECP_COMNADDR_NUM];//�õ���Ϣ���ַ�������������Ҫ�����ط�������
   
  CAT_SpiReadWords(EEPADD_LUBONUM, 1, &wave_total);
  
  if(g_gRunPara[RP_LBSTORAGE_TIME] != RP_LBSTORAGE_INVALID)
  {
  
  LuBoGetNum(wave_total);//lubo_total = 
  }
  if(g_gRunPara[RP_LUBO_NUM] < RP_LUBO_INVALID)
  {
  
  LuBoNum(wave_total);//lubo_total = 
  }
  
  if(g_Cur101Obj !=NULL)
  {
    if(g_Cur101Obj->m_guiyuepara.infoaddlen==1)
    {
        g_Cur101Obj->m_dwasdu.Info=pRxBuf[off];
    }
    if(g_Cur101Obj->m_guiyuepara.infoaddlen==2)
    {
        g_Cur101Obj->m_dwasdu.Info=MAKEWORD(pRxBuf[off],pRxBuf[off+1]);
    }
    if(g_Cur101Obj->m_guiyuepara.infoaddlen==3)
    {
        g_Cur101Obj->m_dwasdu.Info=MAKEWORD(pRxBuf[off+1], pRxBuf[off+2]);
        g_Cur101Obj->m_dwasdu.Info<<=8;
        g_Cur101Obj->m_dwasdu.Info|=pRxBuf[off];
    }
    Info_val  = g_Cur101Obj->m_dwasdu.Info;
  }
  /*else if(g_Cmid == g_CmIdDBG)
  {
      if(((CBJ101S *)pDbg)->m_guiyuepara.infoaddlen==1)
    {
        ((CBJ101S *)pDbg)->m_dwasdu.Info=pRxBuf[off];
    }
    if(((CBJ101S *)pDbg)->m_guiyuepara.infoaddlen==2)
    {
        ((CBJ101S *)pDbg)->m_dwasdu.Info=MAKEWORD(pRxBuf[off],pRxBuf[off+1]);
    }
    if(((CBJ101S *)pDbg)->m_guiyuepara.infoaddlen==3)
    {
        ((CBJ101S *)pDbg)->m_dwasdu.Info=MAKEWORD(pRxBuf[off+1], pRxBuf[off+2]);
        ((CBJ101S *)pDbg)->m_dwasdu.Info<<=8;
        ((CBJ101S *)pDbg)->m_dwasdu.Info|=pRxBuf[off];
    }
    Info_val  = ((CBJ101S *)pDbg)->m_dwasdu.Info;
  }*/
  if(wave_total >= 0xFE)  wave_total=0;
  gRecorder_flag.LIST_flag = OFF;
  
  //�ӽ��յı����д����͵����ݣ��ӱ���ͷ����Ϣ���ַ68 00 00 68 08
  leng=7+g_ucPara101[IECP_LINKADDR_NUM]+g_ucPara101[IECP_TRANSRSN_NUM]+g_ucPara101[IECP_COMNADDR_NUM]+g_ucPara101[IECP_INFO_NUM];
  if(pRxBuf != null )
  {
    memcpy(gRecorder_flag.pRXBuff,pRxBuf,6+pRxBuf[1]);  
  }
  
    memcpy(pTXBuff,gRecorder_flag.pRXBuff,6+pRxBuf[1]);//leng
    
  if(pTXBuff[7+g_ucPara101[IECP_LINKADDR_NUM]] == REQUEST_COT)//�ٻ�Ŀ¼�Ĵ���ԭ��
  {
    if(pTXBuff[5+g_ucPara101[IECP_LINKADDR_NUM]] == CALL_TYPE)//(wInfoAddr == 0x690A)//�ٻ�¼���ļ���Ŀ¼
    {
      //wSendLISTNum =0;
      if(Info_val == 0x680A)
        pTxBuf = FileDirectory(pTxBuf,leng,wave_total);
    else if(Info_val == 0x6804)
    pTxBuf = SoeDirectory(pTxBuf,leng,wave_total);
     else if(Info_val == 0x6806)
    pTxBuf = FixDirectory(pTxBuf,leng);
     else if(Info_val == 0x6807)
    pTxBuf = LogDirectory(pTxBuf,leng);
     
    
    }
  }
  else if(pTXBuff[7+g_ucPara101[IECP_LINKADDR_NUM]] == FILETRANS_COT)//�ٻ�Ŀ¼����
  {
    FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng],gRecorder_flag.pRXBuff[leng + 1]);//-1;
    if(Info_val == 0x680A)
    {
      for(int j =0;j<3;j++)
      {
          long FADDR_RECORDER = FADDR_RECORDER_INFO +(long)FileName*(long)FLINEADDR + (long)j*(long)FPHASEADDR;
          Sst26vf064b_Read(FADDR_RECORDER ,(unsigned char *)&mRecorder_cfg,sizeof(mRecorder_cfg)); //�������ﱣ��gRecorder_cfg��ֵ����Ϊ�����¼����һ�����ܴ�����        
          if(mRecorder_cfg.lubo_flag==0x55)
          break;
      }
    }
    if(pTXBuff[5+g_ucPara101[IECP_LINKADDR_NUM]]== CALL_TYPE)
    {
      
      if(pTXBuff[leng + 3]==1)  //ѡ���ļ���SCQ=1
      {
         pTxBuf[5+g_ucPara101[IECP_LINKADDR_NUM]] = FILEPREPAR_TYPE;
         pTxBuf += leng;
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng ];//�ļ���������ֽ�
        *pTxBuf++ = gRecorder_flag.pRXBuff[leng + 1];//
        //FileName = MAKEWORD(gRecorder_flag.pRXBuff[leng + 1],gRecorder_flag.pRXBuff[leng + 2]);
        if(Info_val == 0x680A)
        {
        *pTxBuf++ =mRecorder_cfg.TOTAL_Leng;
        *pTxBuf++ =mRecorder_cfg.TOTAL_Leng>>8;
        *pTxBuf++ =mRecorder_cfg.TOTAL_Leng>>16;
      wSendCFGNum =0;
      wSendDATNum =0;
        }
  else if(Info_val == 0x6804)
  {
    *pTxBuf++ =TOTAL_Leng;
          *pTxBuf++ =TOTAL_Leng>>8;
          *pTxBuf++ =TOTAL_Leng>>16;
      
  }
  else if(Info_val == 0x6806)
  {
    *pTxBuf++ =Fixpt_TOTAL_Leng;
          *pTxBuf++ =Fixpt_TOTAL_Leng>>8;
          *pTxBuf++ =Fixpt_TOTAL_Leng>>16;
      
  }
   else if(Info_val == 0x6807)
   {
      *pTxBuf++ =Log_TOTAL_Leng;
        *pTxBuf++ =Log_TOTAL_Leng>>8;
        *pTxBuf++ =Log_TOTAL_Leng>>16;
      
  }
        *pTxBuf++ =0x00;//0x80;//�ļ�׼������
        //log_sum_total =0;
        section_current=1;
      }
      else if(pTXBuff[leng + 3]==2)  //�����ļ�
      {
  if(Info_val == 0x680A)
     {
    pTxBuf = SectionPrepareFile(pTxBuf,leng,&mRecorder_cfg);
  }
  else if(Info_val == 0x6804)
  {
    pTxBuf = SectionPrepareSoe(pTxBuf,leng);
    soe_recorded.Soe_Area =0;
          soe_recorded.Soe_Ptr =0;
          soe_recorded.soe_count =0;
          soe_recorded.Soe_Curren_count =0;
          soe_recorded.Soe_count_flag =OFF;
  }
  else if(Info_val == 0x6806)
    {
      fixpt_recorded.fixpt_Ptr =0;
          fixpt_recorded.fixpt_count =0;
          fixpt_recorded.fixpt_Curren_count =0;
          fixpt_recorded.fixpt_count_flag =OFF;
      pTxBuf = SectionPrepareFixpt(pTxBuf,leng);
  }
  else if(Info_val == 0x6807)
    {
    log_recorded.log_count_flag =OFF; 
    log_recorded.log_Ptr  =0;
        log_recorded.log_Curren_count =0;
    log_recorded.log_count =0;
        //log_sum_section =0;  
      pTxBuf = SectionPrepareLog(pTxBuf,leng);
  }
      }
      else if(pTXBuff[leng + 3]==6)//�����
      {
        int segment_leng = SEGMENT_LENGTH;
  if(Info_val == 0x680A)
        {
         if( gRecorder_flag.pRXBuff[leng + 2]==1)//������Ϊ1 ���������ļ���������Ϣ
         {
          if(wSendCFGNum ==0)
                cfg_dat_length(&mRecorder_cfg,FileName);
           pTxBuf = FileDataCfg(pTxBuf,leng,&mRecorder_cfg,segment_leng);//wSecLenPtr,
              
         }
         else if(gRecorder_flag.pRXBuff[leng + 2]==2)//������Ϊ2 ���������ļ���������Ϣ
         {
              pTxBuf = FileDatadat(pTxBuf,leng,wSecLenPtr,segment_leng);
           
         }
  }
  else if(Info_val == 0x6804)
  {
    pTxBuf = SoeData(pTxBuf,leng,segment_leng);
  } 
    else if(Info_val == 0x6806)
    {
      pTxBuf = FixptData(pTxBuf,leng,segment_leng);
    }
    else if(Info_val == 0x6807)
    {
      pTxBuf = LogData(pTxBuf,leng,segment_leng);
    }
    
      }
        
    }
    else if(pTXBuff[5+g_ucPara101[IECP_LINKADDR_NUM]]== APPROVALFILE_TYPE)//�Ͽ��ļ����ڵ����̴���
    {
    if(Info_val == 0x680A)
        pTxBuf = ApprovalFile(pTxBuf,leng,&mRecorder_cfg,wSecLenPtr);
    else if(Info_val == 0x6804)
      pTxBuf = ApprovalSoeFile(pTxBuf,leng,wSecLenPtr);
    else if(Info_val == 0x6806)
      pTxBuf = ApprovalFixFile(pTxBuf,leng,wSecLenPtr);
    else if(Info_val == 0x6807)
      pTxBuf = ApprovalLogFile(pTxBuf,leng,wSecLenPtr);
      
         
    }
  if(pTxBuf == NULL) return;
    /*else if((pTXBuff[5+g_ucPara101[IECP_LINKADDR_NUM]]== SECTIONPREPAR_TYPE))
    {
        pTxBuf = SectionPrepareFile(pTxBuf,leng,&mRecorder_cfg);
        
         
    } */
   }
    pTXBuff[4]= LuboGetCtrCode(pRxBuf[4]);
    pTXBuff[1] = pTxBuf - &(pTXBuff[4]);//LL
    pTXBuff[2] = pTxBuf - &(pTXBuff[4]);//LL
  
    if(check ==  ChkluboSum(&(pTXBuff[4]),pTXBuff[1]))
    {
      check = 0;
    }
   *pTxBuf++ = ChkluboSum(&(pTXBuff[4]),pTXBuff[1]);
    check = ChkluboSum(&(pTXBuff[4]),pTXBuff[1]);
   
   *pTxBuf++ = 0x16;
    //delayms(1000);//����
    CommSendData(pTXBuff,pTXBuff[1]+6,g_Cmid);//g_Cur101Obj
    if(gRecorder_flag.LIST_flag == TRUE)
    gRecorder_flag.LIST_flag = ON;
  if(g_Cur101Obj != NULL)
  {
        g_Cur101Obj->m_PaWaitflag_lubo = ON;
        g_Cur101Obj->m_PaWaitCt_lubo = g_gRunPara[RP_LUBOGPRS_T];
        g_Cur101Obj->m_TxNum_lubo = 0;

        memcpy(&(g_Cur101Obj->m_gprsSendBuf[0]),pTXBuff,pTXBuff[1]+6);
        g_Cur101Obj->m_gprsSendLen = pTXBuff[1]+6;
  }
 /* else if(g_Cmid == g_CmIdDBG)
  {
    pDbg->m_PaWaitflag_lubo = ON;
        pDbg->m_PaWaitCt_lubo = g_gRunPara[RP_LUBOGPRS_T];
        pDbg->m_TxNum_lubo = 0;
    memcpy(&(pDbg->m_gprsSendBuf[0]),pTXBuff,pTXBuff[1]+6);
        pDbg->m_gprsSendLen = pTXBuff[1]+6;
    
  }*/
  
  
  }

WORD  ReadNumSoeFromFlash(WORD *pHeadPtr,WORD TailPtr)
{
    int RecNum = 0;
    BYTE bSoeBufTmp[SOE_DATA_LEN] = {0};
    while((*pHeadPtr) != TailPtr)
    {
        Sst26vf064b_Read(FADDR_SOE_START + (*pHeadPtr)*FSOE_LEN,bSoeBufTmp, SOE_DATA_LEN);
        if(bSoeBufTmp[0] == SOEVALID)
        {
          //memcpy(pSoeBuf,bSoeBufTmp,SOE_DATA_LEN);
          //pSoeBuf += SOE_DATA_LEN;
          RecNum++; 
        }
        (*pHeadPtr)++;
        (*pHeadPtr) &= (FSOE_TOLNUM - 1);
     }
   return RecNum;
}

//FLbAddr = FADDR_RECORDER_START + lubo_absolute_time;//����ʱ���ʱ�䶼ȡA���ʱ��

//FLbAddr = FADDR_RECORDER_START + lubo_absolute_time;//�ڶ��ξ���ʱ���ʱ��

//Sst26vf064b_Read(FLbAddr,ReadBack,RTC_NUM);//��ȡ���Ե�ַ��ʱ��



