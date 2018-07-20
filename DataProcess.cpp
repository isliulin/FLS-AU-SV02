/*------------------------------------------------------------------------
 Module:      DataProcess.cpp
 Author:      linxueqin
 Project:     ��վƽ̨
 State:       V1.0
 Creation Date:   2014-4-29
 Description:   AD������ʼ����������վ�ɼ���ѹ�������ͱ���SOE����֯ң��ң�ŷ��ͻ�����
------------------------------------------------------------------------*/


#ifndef _DATA_PROCESS

    #define _DATA_PROCESS
    #include "DataStruct.h"
#include "Main.h"
#endif

//==============================================================================
//  ��������   : InitDataProc
//  ��������   : ���ݴ����ʼ��
//  �������   : <��>
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       : 
//==============================================================================
void InitDataProc(void)
{
    unsigned int i,j;
    for(i = 0; i < AC_AD_CHAN_NUM;i++)
      for(j = 0; j < AC_BUFF_LEN;j++)
        g_sSampleData[i][j] = 0;
    if(g_gRunPara[Terminal_Type] == AERIAL_TERMINAL)
    {//�ܿ��ն˵�ͨ��״̬�жϲ�ʹ��g_sCommStatCount�������
      for(i = 0; i < RMT_ZSQST_NUM; i++)
      {
          if( (i+1)%4 == 0)
            g_sCommStatCount[i] = 0;//�ܿ�û��0��������������ͨ�����ϵ�������
          else
          g_sCommStatCount[i] = g_gRunPara[RP_ZSQCST_T];
      }
    }
    g_sSampleFinish = OFF;
    
    //soe������ʼ��
    g_unSSoeSavePtr = 0;
    #ifdef LOCALYX
    g_unDSoeSavePtr = 0;
    #endif
    soe_ptr = 0;
    soe_area = 0;
    soe_num = 0;
    g_AdChgSdYc = 0;
    g_FltChgSdYc = 0;
    g_PtLoseFlag = 0;
    g_PtMonitorCnt = 0;
    g_SendLuboNum =0;
    g_SendLuboPage=0;
    g_fault_phase =0;
    g_differI_weizhi =0;
    g_differV_weizhi=0;
    g_faultI_Valse=0;
    g_faultV_Valse=0;
	g_unShortBufftail =0;
	
  //g_PtLoseCnt=0;
    if(g_gRunPara[RP_DEVICE_TYPE])
        g_SendYc = 1;
    else
        g_SendYc = 0;
    if(g_gRunPara[RP_STATUSRP_INTERVAL])
        g_DateCount  = g_gRunPara[RP_STATUSRP_INTERVAL];
        
    g_OpenBatFlag = OFF;
    g_SunHighNum  = 0;
    g_Soenum=0;
    g_LogType = 0; //��־�ļ���ͬ����
    g_Linkflag =0;
    g_LogValue =0;
	g_buchangnum =0;
	g_buchangnum2 =0;
	g_ccr0 = 0;
	
}

//���͵�ѹ�澯���ն��Լ�
void CheckZDAlm()
{
   #define PtMonCntMax 30
    //#define PtLoseCntMax    //��ѹ�͵�ʱ�������ϴ��������������κ��ϴ�
      if(g_gRmtMeas[RM_SUN_U] <= 11000 ) //600��⵽����ʽPT��ѹ���㣬���ң�����ֵ
     {
        //RstRMTMeas();   
        if(g_PtLoseFlag == 0)
        {
          g_PtLoseFlag = 1;
          CreatNewSoe(RI_ZD_LVD,1);//ʧѹ����λʧѹң������  
        }
        else if(g_PtLoseFlag == 2)
        {
          g_PtMonitorCnt = PtMonCntMax;//��ѹ�ظ�������,�����ֵ�ѹ�½��������¼�ʱ
        }  
     }
     else if(g_gRmtMeas[RM_SUN_U] > 11000)//600
     {
          if(g_PtLoseFlag == 1)//ʧѹʱ���ֵ�ѹ�������ߣ���ʱ10�Σ����������ѹ���˳�ʧѹ״̬
          {
            g_PtMonitorCnt = PtMonCntMax;
            g_PtLoseFlag = 2;
          }
     }
     
     if(g_PtMonitorCnt > 0)
     {
       g_PtMonitorCnt--;
       if(g_PtMonitorCnt == 0)
       {
          g_PtLoseFlag = 0;
          CreatNewSoe(RI_ZD_LVD,0); //��ѹ�ָ�����λң������
       }
     }

      if(g_gRmtMeas[RM_BAT_U] <= g_gRunPara[RP_ALARM_BAT] ) //��ز���
     {  
        if(g_BatLoseFlag == 0)
        {
          g_BatLoseFlag = 1;
          CreatNewSoe(RI_ZD_BVD,1);//ʧѹ����λʧѹң������  
        }
//        else if(g_BatLoseFlag == 2)
//        {
//          g_BatMonitorCnt = PtMonCntMax;//��ѹ�ظ�������,�����ֵ�ѹ�½��������¼�ʱ
//        }  
     }
     else if(g_gRmtMeas[RM_BAT_U] > g_gRunPara[RP_ALARM_BAT])
     {
          if(g_BatLoseFlag != 0)//ʧѹʱ���ֵ�ѹ�������ߣ���ʱ10�Σ����������ѹ���˳�ʧѹ״̬
          {
              if(g_gRmtMeas[RM_BAT_U] > g_gRunPara[RP_ALARM_BAT] + 20)  //�ز�ֵ0.2V
              {
                  g_BatLoseFlag = 0;
                  CreatNewSoe(RI_ZD_BVD,0);
              }
              else
              {
                  g_BatLoseFlag = 2;
              }
          } 
     }
//�ն��Լ��жϣ�����Դ�ͱ��õ�Դ��ʧѹ���Լ���� �Լ�����???
     if(GetRIStatus(RI_ZD_BVD) && GetRIStatus(RI_ZD_LVD) && (!GetRIStatus(RI_ZD_ZJ)))
     {  
        CreatNewSoe(RI_ZD_ZJ,1); 
     }
     else if((!GetRIStatus(RI_ZD_BVD)) && (!GetRIStatus(RI_ZD_LVD)) && GetRIStatus(RI_ZD_ZJ))
     {
       CreatNewSoe(RI_ZD_ZJ,0); 
     }
}
//==============================================================================
//  ��������   : Check_LogFlag
//  ��������   : ��־�ļ����͵Ĳ���
//  �������   : <��>
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       :
//==============================================================================

void Check_LogFlag(void)
{
  
    if((g_GprsPowerSt ==1) &&(pGprs->m_linkflag ==1))//ͨ������������Ͽ�����
    {
       
        if((g_LogType & BIT1) == 0)
        {
          g_LogType |= BIT1;
          g_LogType &= NBIT2; 
          g_LogValue|= BIT1;
        }
    }
    else
    {
      if(((g_LogType & BIT2) == 0)&&(g_LogType & BIT1))//��������֮�����Ͽ��ټ�¼
      {
          g_LogType |= BIT2;
          g_LogType &= NBIT1; 
          g_LogValue|= BIT2;
      }
    }
    if(g_Linkflag == 1)//ͨ�������쳣���ط�û����Ӧ
    {
      if((g_LogType & BIT3) == 0)
      {
        g_LogType |= BIT3;
        g_LogType &= NBIT4; 
        g_LogValue|= BIT3;
      }
    }
    else
    {
      if(((g_LogType & BIT4) == 0) &&(g_LogType & BIT3))//ͨ�������쳣֮���ټ�¼�ָ�
      {
        g_LogType |= BIT4;
        g_LogType &= NBIT3;
        g_LogValue|= BIT4;
      }
    }
    if(g_PtLoseFlag ==1)//����Դʧѹ
    {
      if((g_LogType & BIT7) == 0)
      {
        g_LogType |= BIT7;
        g_LogType &= NBIT8; 
        g_LogValue|= BIT7;
      }
    }
    else if(g_PtLoseFlag ==0)
    {
      if(((g_LogType & BIT8) == 0) &&(g_LogType & BIT7))//����Դʧѹ֮��
      {
        g_LogType |= BIT8;
        g_LogType &= NBIT7; 
        g_LogValue|= BIT8;
      }
    }
    if(g_BatLoseFlag ==1)//��ص�ѹ��ѹ
    {
      if((g_LogType & BIT9) == 0)
      {
        g_LogType |= BIT9;
        g_LogType &= NBITA; 
        g_LogValue|= BIT9;
      }
    }
    else if(g_BatLoseFlag ==0)
    {
      if(((g_LogType & BITA) == 0) &&(g_LogType & BIT9))
      {
        g_LogType |= BITA;
        g_LogType &= NBIT9; 
        g_LogValue|= BITA;
      }
    }
}

//==============================================================================
//  ��������   : Check_LogFile
//  ��������   : ��־�ļ�������
//  �������   : <��>
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       :
//==============================================================================

void Check_LogFile(void)
{
  unsigned int  LogType ;
  char *PLogText;
  unsigned char  LogValue;
  INT8U leng_log;
   char ch[30];
    Check_LogFlag();
    if((g_LogType & BIT0) == BIT0)//�ն�������¼
    {
      LogType =1;
      PLogText ="Terminal Restart";
      LogValue =1;
      g_LogType &= NBIT0; 
    }
    
    else if(((g_LogType & BIT1) == BIT1) &&((g_LogValue & BIT1) == BIT1))//ͨ������������Ͽ���¼
    {
        LogType =2;  
        PLogText="Establish Connection";
        LogValue =1; 
        g_LogValue &= NBIT1;
    }
    else if(((g_LogType & BIT2) == BIT2)&&((g_LogValue & BIT2) == BIT2))//ͨ������������Ͽ���¼
    {
        LogType =2;  
        PLogText="Disconnect Connection";
        LogValue =0; 
        g_LogValue &= NBIT2;

    }
    else if(((g_LogType & BIT3) == BIT3)&&((g_LogValue & BIT3) == BIT3))//ͨ�������쳣
    {
      LogType =3;
      PLogText="Commmunication Process Exception";
      g_LogValue &= NBIT3;
      LogValue =1; 
    }
    else if(((g_LogType & BIT4) == BIT4)&&((g_LogValue & BIT4) == BIT4))//ͨ�������쳣
    {
      LogType =3;
      PLogText="Commmunication Process Exception";
      g_LogValue &= NBIT4;
      LogValue =0; 
    }
    else if((g_LogType & BIT5) == BIT5)//Ԫ���Լ��쳣
    {
      LogType =4;
      PLogText="Abnormal Element Self-Inspection";
      LogValue =1;  
    }
     else if((g_LogType & BIT6) == BIT6)//װ���ڲ�����쳣
     {
      LogType =5;
      PLogText="Abnormal Device Software";
      LogValue =1;  
    }
     else if(((g_LogType & BIT7) == BIT7)&&((g_LogValue & BIT7) == BIT7))
    {
      LogType =6;
      PLogText="The Main Power Supply Voltage Abnormal";//����Դ��ѹ�쳣
      
        LogValue =1;  
        g_LogValue &= NBIT7;
        
    }
     else if(((g_LogType & BIT8) == BIT8)&&((g_LogValue & BIT8) == BIT8))
    {
      LogType =6;
      PLogText="The Main Power Supply Voltage Abnormal";//����Դ��ѹ�쳣
      LogValue =0;
      g_LogValue &= NBIT8;
    }
    else if(((g_LogType & BIT9) == BIT9)&&((g_LogValue & BIT9) == BIT9))
    {
      LogType =7;
      PLogText="The Standby Power Supply Voltage";//���õ�Դ��ѹ�쳣 
      LogValue =1;  
      g_LogValue &= NBIT9;
    }
    else if(((g_LogType & BITA) == BITA)&&((g_LogValue & BITA) == BITA))
    {
      LogType =7;
      PLogText="The Standby Power Supply Voltage";//���õ�Դ��ѹ�쳣
      LogValue =0;
      g_LogValue &= NBITA;
    } 
     else if((g_LogType & BITB) == BITB)
    {
      LogType =8;
      PLogText="Control Circuit Disconnection";//���ƻ�·�����쳣
      LogValue =1;
      g_LogType &= NBITB; 
    }
     else if((g_LogType & BITC) == BITC)
    {
      LogType =9;
      PLogText="Abnormal Switch Position";//����λ���쳣��¼
      LogValue =1;
      g_LogType &= NBITC; 
    }
      else if((g_LogType & BITD) == BITD)
    {
      LogType = 8;//10;
      PLogText="Terminal Parameter Modification";//�ն˲����޸ļ�¼
      
      LogValue =1;
      g_LogType &= NBITD; 
    }
     else if((g_LogType & BITE) == BITE)
    {
      LogType = 10;//11;
      PLogText="Software Version Upgrade";//����汾������¼   
      LogValue =1;
      g_LogType &= NBITE; 
    }
	 else if((g_LogType& BITF) == BITF)
	 {
	       LogType = 20;//11;
              
               //LogType =g_YKinfo.YKinfoaddr;
	  	if(g_YKinfo.cmd == MI_YKSELECT)
                { 
      		  PLogText="select";//xuanze
                }
      	        else if(g_YKinfo.cmd == MI_YKOPRATE)
	  		PLogText="oper";//
	  	else
	  		PLogText="cancel";
                sprintf((char *)ch,"%x%s",g_YKinfo.YKinfoaddr,PLogText);
                 PLogText =ch;
      	LogValue =g_YKinfo.cmd_state;
      	g_LogType &= NBITF; 
	 }
    else 
      return;   
    leng_log =strlen(PLogText);
    SaveLog(LogType,PLogText,LogValue,leng_log);
  }

//==============================================================================
//  ��������   : CalcuRmtMeas
//  ��������   : ��Чֵ���㣬�Բɼ��������ݽ��д���ȥ�����ֵ����Сֵ��ȡƽ��,�����¶�Ӧ��ң��ֵ�����ж�ң��ֵ��ͻ��
//  �������   : <��>
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       :
//==============================================================================
void CalcuRmtMeas(void)
{

   float ff1;
   if(g_sSampleFinish != ON)
       return;
   else 
       g_sSampleFinish = OFF;
   unsigned int iMinVal= 0;
   unsigned int iMaxVal = 0;
   unsigned long lSumVal[AC_AD_CHAN_NUM] = {0};

   if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISDDMODE])
    return;//�Ե�ģʽ�²�����ʵ�����ݣ����жϵ�Դ��ѹ��������Դ����
  //if(!(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISDDMODE]))
  {//aa�ڶԵ�ģʽ�²�����ʵ������
       int i = 0,j = 0;
       for(i = 0;i < AC_AD_CHAN_NUM;i++)
       {
           iMinVal = g_sSampleData[i][0];
           iMaxVal = g_sSampleData[i][0]; 
           lSumVal[i] = g_sSampleData[i][0];
           for(j = 1;j < AC_BUFF_LEN;j++)
           {
               lSumVal[i] += g_sSampleData[i][j];
         
              if(iMinVal > g_sSampleData[i][j])
              {
                  iMinVal = g_sSampleData[i][j];
              }
              if(iMaxVal < g_sSampleData[i][j])
              {
                  iMaxVal = g_sSampleData[i][j];
              }
           }
//           if(i!=CHAN_BAT)//�򿪵�ع̣ܽ��ȴ��������ʱǰ���ε�ֵΪ0.������ȡ���ֵ��
//                //���ı�ȴ�ʱ�䣬�����ʱ�ϳ�ʱ�����ʵʱģʽ
               lSumVal[i] = lSumVal[i]- iMinVal-iMaxVal;//ȥ�����ֵ����Сֵ 
//           else
//               lSumVal[i] =iMaxVal;
          
        }
        if(g_OpenBatFlag == TRUE)
        { 
            ff1=(float)lSumVal[CHAN_BAT]/4096/8*2.5*10*1000;
            g_gRmtMeas[RM_BAT_U]=(unsigned int) ff1;//��ص�ѹ����ֵ
        
            //ff1=(float)lSumVal[CHAN_BATF];
            //ff1/=16.187;
            //g_gRmtMeas[RM_BATF_U] =(unsigned int) ff1;//��ظ���ѹ����ֵ
            g_OpenBatFlag = OFF;
            CLOSE_BAT_SAMPLE;
        }

        //���ݵ�ѹ����ֵ
        ff1=(float)lSumVal[CHAN_CAP]/4096/8*2.5*10*1000;
        //ff1/=16.03;
        g_gRmtMeas[RM_CAP_U]=(unsigned int) ff1;

        //̫���ܵ�ѹ����ֵ
//        ff1=(float)lSumVal[CHAN_SUN];       
//        if(g_gRunPara[Terminal_Type] == AERIAL_TERMINAL)
//        {
//            if(g_gRunPara[RP_POWER_MODE] == REALTIME)
//                ff1 /= 13.473;
//            else
//                ff1 /= 16.687;
//        }
//        else
//            ff1/=16.03;//13.473;
        ff1=(float)lSumVal[CHAN_SUN]/4096/8*2.5*10*1000;///4095/8*2.5*6*100;            
        g_gRmtMeas[RM_SUN_U]=(unsigned int) ff1;
        g_ucRefYxYcTs |= BIT1;
        SetYcTrans();//������ֱ������Ҫ�ϴ���ң�⣬��Դ�����й�GPRSʱҪ�ж�g_ucRefYxYcTs��ʶ������Ҫ��=0
    }//aa
    
    Close_BatSunC_Para();

    
    //��Դ����
   g_SendYc = 1;//ÿ�ε�Դ����ǰ������Ϊ��ң���ڵ�Դ�����п��ܻ���Ϊһң
   if (g_gRunPara[Terminal_Type]==AERIAL_TERMINAL)
    {
      if(g_gZCZSQ == 0)   //003����ʱ����
          VoltLogicCtr(); 
    }
    else //if((g_gRunPara[Terminal_Type]==CABLE_TERMINAL_ODU)||g_gRunPara[Terminal_Type]==CABLE_TERMINAL_DODU)
    {
      VoltLogicCtr_cable();
    }
    
    CheckZDAlm(); 
    
    static int AdCalcuInit = 0;
    if(AdCalcuInit == 0)
    {
      AdCalcuInit = 1;
      g_AdChgSdYc = OFF;//��һ��AD�ɼ������������ϴ�
    }
    else
      g_AdChgSdYc = ON;      
}
//*****************************************************
//    Date    :  
//   Mender   :  
//   Function :  ����Դ���������Ǹ����͵ȼ�
//****************************************************
void VoltLogicCtr(void)
{
    #define SENDYC_4HOUR (60*60*2)
    //#define SENDYC_4HOUR (60 * 5)
    OPEN_WIRE_POWER;
    static unsigned int Before4HourSdYcTm = 0;//����׼ʵʱģʽ���л�Ϊ4Сʱ֮ǰ��ֵ
    g_SendYc = 1;
    switch(g_gRunPara[RP_POWER_MODE])
    {
    case QUASI_REALTIME:
        //��AD�ɼ�VBAT����6.5Vʱ���ضϳ���·30���ӣ�Ȼ�������硣
        if(g_gRmtMeas[RM_BAT_U] > 14400)
        {
            CLOSE_SUN_CHARGE;
            OpenTimer(TM_CLOSECHARGE);
            //MyPrintf("Close sun charge\r\n");
        }
    
        if((g_gRmtMeas[RM_SUN_U] >= 11000) && (g_gRmtMeas[RM_BAT_U] >= g_gRunPara[RP_ALARM_BAT]))
        {//���û����õĶ�ʱ����ϱ�ȫң�⣬֧��ң��ͻ���ϱ�(Խ��ң��͹���ң��)��֧��ң���ϱ���
          if(Before4HourSdYcTm != 0)
          {
              if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISSENDYC]) == 1)
              {
                g_gRunPara[RP_SENDYC_T] = Before4HourSdYcTm;
                Before4HourSdYcTm = 0;
                RefreshTmCount(TM_SENDYC);
              }
              
          }
        }
        else if((g_gRmtMeas[RM_SUN_U] < 11000) && (g_gRmtMeas[RM_BAT_U] >= g_gRunPara[RP_ALARM_BAT]))
        {//4Сʱ�ϱ�һ��ȫң�⣬֧��ң��ͻ���ϱ�(Խ��ң��͹���ң��)��֧��ң���ϱ���
          if(g_gRunPara[RP_SENDYC_T] != SENDYC_4HOUR )
          {
            if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISSENDYC]) == 1)
            {
              Before4HourSdYcTm = g_gRunPara[RP_SENDYC_T];
              g_gRunPara[RP_SENDYC_T] = SENDYC_4HOUR;
              RefreshTmCount(TM_SENDYC);
            }
             
          }

    
        }
        else if(g_gRmtMeas[RM_BAT_U] < g_gRunPara[RP_ALARM_BAT])
        {//����ص�ѹ>5.5v��<6vʱ���ر�ң���ϱ���ֻ֧��ң���ϱ���
            g_SendYc = 0;
            
        }                                                   
        break;
    case REALTIME:
        OPEN_GPRS_POWER;
        break;
    case REALTIME_SUNPOWER:
        if(g_gRmtMeas[RM_BAT_U] > 14400)//6501300
        {
            CLOSE_SUN_CHARGE;
            OpenTimer(TM_CLOSECHARGE);
            //MyPrintf("Close sun charge\r\n");
        }      
        if(g_gRunPara[RP_DEVICE_TYPE] == 1)
        {
            if(g_BatLoseFlag == 0)  //��س��㣬δʧѹ
            {
                if(Before4HourSdYcTm != 0)
                {
                    if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISSENDYC]) == 1)
                    {
                        g_gRunPara[RP_SENDYC_T] = Before4HourSdYcTm;
                        Before4HourSdYcTm = 0;
                        RefreshTmCount(TM_SENDYC);
                    }
                }              
                if((g_GprsPowerSt == 0) || (g_sTimer[TM_GPRSPWOERDOWN].m_TmCount > 0))
                {
                    OPEN_GPRS_POWER;  //��ѹ�ظ�����GPRS��Դ
                    
                    g_sTimer[TM_GPRSPWOERDOWN].m_TmCount = 0;                    
                }
                //g_EnSendStatusFlag = 0;
            }
            else    //��ز��㣬�л���һңģʽ
            {
                if(g_gRunPara[RP_SENDYC_T] != SENDYC_4HOUR )
                {
                    if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISSENDYC]) == 1)
                    {
                        Before4HourSdYcTm = g_gRunPara[RP_SENDYC_T];
                        g_gRunPara[RP_SENDYC_T] = SENDYC_4HOUR;
                        RefreshTmCount(TM_SENDYC);
                    }
                }              
                if(g_GprsPowerSt && (g_sTimer[TM_GPRSPWOERDOWN].m_TmCount == 0)) // ����Ƿ��йر�GPRS��ʱ
                    OpenTimer(TM_GPRSPWOERDOWN,300); 
                //g_EnSendStatusFlag = ON;//����һңģʽ��־λ,��������ն˽����һңģʽ        
            }
        }
        break;        
    default:
        OPEN_GPRS_POWER;
        break;
    }
    if(!g_gRunPara[RP_DEVICE_TYPE])
        g_SendYc = 0;
}

//==============================================================================
//  ��������   : VoltLogicCtr_cable
//  ��������   : �ж�ʵʱ��ʵʱ״̬�л�
//  �������   : 
//  �������   : <��>
//  ����ֵ     : 
//  ����˵��   : �ն�ADֻҪ�ɵ�һ����ֵС��5v�����GPRS����Դģʽң��λ���óɷ�ʵʱ,2Сʱ�ڲ����ж�ģʽ�л���
//               �ն�AD�ɼ�10s���ݣ�������6.5v�����GPRS���ټ������40s��û�г���С��5v�ģ����Դģʽң��λ���ó�ʵʱģʽ
//  ����       :
//==============================================================================

void VoltLogicCtr_cable(void)
{
    if((g_sTimer[TM_Volt].m_TmCount==0))
    {
      if(g_gRmtMeas[RM_SUN_U] < 5000 && g_gRmtMeas[RM_SUN_U] != 0)
      {//��һ��С��5v���GPRS������ң��λΪ׼ʵʱ��������������(Ĭ��2Сʱ�ڲ����ж�ģʽ�л�)
        g_gRunPara[RP_POWER_MODE]=QUASI_REALTIME;
        CreatNewSoe(RI_REAL_QUASI,1);
        if(g_GprsPowerSt)
        {
            //��û��ͨ������Ź�GPRS����ͨ��������GPRS�Ѿ�����򿪵�أ���֤�����ϴ�
            if(CheckHaveDataToSd() == FALSE)
              CloseGPRS(2);
            else
            {
              if((g_gRmtMeas[RM_CAP_U] < 5500)&&(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL)) // ��ѹС��6V��������ر���
              {
                OPEN_BAT_POWER; //  �������    
                 OpenTimer(TM_BATPWOERDOWN);//   ͳ�Ƶ��ʹ��ʱ��
                 if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
                 {
                 	BYTE bBuf[2] = {0xBB,0XBB};
                 	MyPrintfD(bBuf,2);
                 }
              }
            }
        }
        OpenTimer(TM_Volt);
        g_SunHighNum = 0;
      }
      else if((g_gRmtMeas[RM_SUN_U] > 6500)&&(!g_batPowerSt))//��عر�ʱ���ж��Ƿ��л���ʵʱģʽ
      {
        if(g_gRunPara[RP_POWER_MODE] == QUASI_REALTIME)  
          g_SunHighNum++;
        else
          g_SunHighNum = 0;
      }
      else
      {
        g_SunHighNum = 0;
      }
    }
    if(g_SunHighNum > 5 && (g_gRunPara[RP_POWER_MODE] == QUASI_REALTIME))//�ɼ��ĵ�ѹ����5�δ���6.5v,��GPRS
    {
        if(!g_GprsPowerSt)
          OpenGPRS(2);
        if( g_SunHighNum > 20)//�ɼ��ĵ�ѹ����20�δ���6.5v,����Ϊʵʱģʽ
        {
          g_SunHighNum = 0;
          g_sTimer[TM_GPRSPWOERDOWN].m_TmCount=0; 
          g_gRunPara[RP_POWER_MODE]= REALTIME;
          CreatNewSoe(RI_REAL_QUASI,0);
          CLOSE_BAT_POWER;//�л���ʵʱģʽ˵��CT���㣬�͸ñ�֤����ǹر�״̬
        }
    } 
    if((g_SunHighNum > 5) && (g_SunHighNum <= 20))
      g_JdgPwMode = 1;
    else
      g_JdgPwMode = 0;
}
  //==============================================================================
  //  ��������   : GetRIChgFlag
  //  ��������   : ��ȡң����Ϣ�еĵ���״̬
  //  �������   : RIIndex:ң����Ϣ���е�ȫ�������ţ������constant.h��RMT_INFO����
  //  �������   ��<��>
  //  ����ֵ     : 1��0
  //  ����˵��   : 
  //  ����       ��
  //==============================================================================
//  unsigned char GetRIChgFlag( unsigned int RIIndex)
//  {
//        if( m_gRmtChgFlag[(RIIndex >> 4)] & BIT[(RIIndex & 0x0F)])
//            return 1;
//        else
//          return 0;
//  }
  //==============================================================================
   //  ��������   : SetRIChgFlag
   //  ��������   : ����ң����Ϣ�еĵ���״̬������ʵ��д��1����0
   //  �������   : RIIndex:ң����Ϣ���е�ȫ�������ţ������constant.h��RMT_INFO����
   //                         : Status: д��״̬��1����0
   //  �������   : <��>
   //  ����ֵ     : 
   //  ����˵��   : 
   //  ����       ��
   //==============================================================================
   
//   void SetRIChgFlag( unsigned int RIIndex , unsigned int Status)
//   {
//         if( Status > 0 )
//         {
//             //��״̬��Ϊ�ߣ�����л����
//             g_gRmtChgFlag[(RIIndex >> 4)] |=  BIT[(RIIndex & 0x0F)];
//         }
//         else
//         {
//             //��״̬Ϊ�ͣ�������ȡ����Ȼ����������
//             g_gRmtChgFlag[(RIIndex >> 4)] &= ~BIT[(RIIndex & 0x0F)];
//         }
//   }

//==============================================================================
//  ��������   : GetRIStatus
//  ��������   : ��ȡң����Ϣ�еĵ���״̬��˫��״̬
//  �������   : RIIndex:ң����Ϣ���е�ȫ�������ţ������constant.h��RMT_INFO����
//             : RmtInfoFlag 0 =g_gRmtInfo��1=g_unYxTrans��2=g_unDYxTrans
//  �������   ��<��>
//  ����ֵ     : 1��0��2,3
//  ����˵��   : ÿ��ң����Ϣռ����bitλ
//  ����       ��
//==============================================================================

unsigned int GetRIStatus( unsigned int RIIndex, BYTE RmtInfoFlag)
{
    unsigned int iRtStVal = 0;
    unsigned int iStVal = 0;
    unsigned int *pRmtInfo = null;

    if(RmtInfoFlag == 1)       pRmtInfo = g_unYxTrans;
    else if(RmtInfoFlag == 2)  pRmtInfo = g_unDYxTrans;
    else                       pRmtInfo = g_gRmtInfo;
    iStVal = 3 << ((RIIndex & 7)*2);
    iRtStVal = pRmtInfo[(RIIndex >> 3)] & iStVal;//ȡ��Ҫ�жϵ�λ
    iRtStVal = iRtStVal >> ((RIIndex & 7)*2);//��Ҫ�жϵ�λ���������λ
    return iRtStVal;
}
//==============================================================================
//  ��������   : SetRIStatus
//  ��������   : ����ң����Ϣ�еĵ���״̬������ʵ��д��1����0,2,3
//  �������   : RIIndex:ң����Ϣ���е�ȫ�������ţ������constant.h��RMT_INFO����
//                         : Status: д��״̬��1����0,2,3
//             : RmtInfoFlag 0 =g_gRmtInfo��1=g_unYxTrans��2=g_unDYxTrans
//  �������   : <��>
//  ����ֵ     : 
//  ����˵��   : ÿ��ң����Ϣռ����bitλ
//  ����       ��
//==============================================================================

void SetRIStatus( unsigned int RIIndex , unsigned int Status, BYTE RmtInfoFlag)
{
   unsigned int iStVal = 0;
   unsigned int *pRmtInfo = null;
    if(RmtInfoFlag == 1)       pRmtInfo = g_unYxTrans;
    else if(RmtInfoFlag == 2)  pRmtInfo = g_unDYxTrans;
    else                       pRmtInfo = g_gRmtInfo;
   iStVal = 3 << ((RIIndex & 7)*2);
   pRmtInfo[(RIIndex >> 3)] &= ~iStVal;//���Ҫ���õ�λ
   pRmtInfo[(RIIndex >> 3)] |=  ((Status & 3) << ((RIIndex & 7)*2));//����״̬д��Ҫ���õ�λ
   
}

BOOL Absolute_time_calculated(unsigned int minute_count,DWORD relative_time,unsigned int * pabsolute_time)
{
	WORD ms_count =0;
	WORD s_count =0;
	WORD m_count =0;
	WORD h_count =0;
	WORD d_count=0;
	for(BYTE i =0;i<3;i++)
	{
		if(g_RF003_timeInfo.RF003_mini_num[i] == minute_count)//���߷��ķ��Ӽ�����ָʾ���ķ��Ӽ���һ��
		{
			ms_count = (relative_time >>2)%1000;//�õ�MS�ļ���
			s_count = (relative_time >>2)/1000;//�õ�S�ļ���
			if((g_RF003_timeInfo.RF003_absolute_time[i][RTC_MICROSEC] + ms_count)>=1000)
			{
				s_count++;
				pabsolute_time[RTC_MICROSEC] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_MICROSEC] + ms_count -1000;
			}
			else
				pabsolute_time[RTC_MICROSEC] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_MICROSEC] + ms_count;
			if((g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count)>=60)
			{
				m_count =(g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count)/60;//�õ�min�ļ���
				pabsolute_time[RTC_SEC] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count)%60;//�õ�min�ļ���
			}
			else
				pabsolute_time[RTC_SEC] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count);
			if(g_RF003_timeInfo.RF003_absolute_time[i][RTC_MINUT] + m_count >=60)
			{
				h_count = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_MINUT] + m_count)/60;
				pabsolute_time[RTC_MINUT] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_MINUT] + m_count)%60;//�õ�min�ļ���
			}
			else
				pabsolute_time[RTC_MINUT] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_MINUT] + m_count);
			if(g_RF003_timeInfo.RF003_absolute_time[i][RTC_HOUR] + h_count >=24)
			{
				d_count++;
				pabsolute_time[RTC_HOUR] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_HOUR] + h_count)-24;
			}
			else
				pabsolute_time[RTC_HOUR] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_HOUR] + h_count;
			pabsolute_time[RTC_DATE] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_DATE] +d_count;
			pabsolute_time[RTC_MONTH] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_MONTH];
			pabsolute_time[RTC_YEAR] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_YEAR];
			pabsolute_time[RTC_WEEK] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_WEEK];
			 return 1;
		}
	}
	return 0;
}
void CreatNewSoe_time(unsigned int SoeType,unsigned long SoeValue,unsigned int * pAbsolutetime)
{
		unsigned int TsSoeType;
		sSOE_RECORD_FLASH* PtrFlashStruct; 

		WORD wFltTp = 0xFFFF;
    	if(((int)SoeType >= RI_1A_SHORT) &&(SoeType <= RI_3O_POWERCUT))
    	{
      		WORD wOffet = SoeType - RI_1A_SHORT;
      		WORD wFltNum = RI_FLT_NUM;
      		wFltTp = wOffet/wFltNum;
    	}
		if((int)SoeType >= RI_1A_SHORT && SoeType < RMT_INFO_NUM)
		{
			if(GetRIStatus(SoeType) == SoeValue)
			 goto CombYx;//return;//����ң����״̬����״̬һ�����������²���
		}
		SetRIStatus(SoeType,SoeValue);
		  
		TsSoeType = RealYxToTsYx(SoeType);
		if(TsSoeType == 0xffff)
			goto CombYx;//return;
		  
		  if(pGprs != null) SETBIT(pGprs->m_gRmtChgFlag,SoeType,1);
		  if(pDbg != null)	SETBIT(pDbg->m_gRmtChgFlag,SoeType,1);
		  
		  if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
			TsSoeType = g_gRunPara[RP_DYX_INFADDR] - 1 + TsSoeType - g_ucDYxTrsStartId;
		  else
			TsSoeType = g_gRunPara[RP_SYX_INFADDR] - 1 + TsSoeType - g_ucSYxTrsStartId;
			
	 
		  g_yxChangeflag = 0xffff;	 
		  g_ucRefYxYcTs |= BIT0;
		  SetYxTrans();
		  //int iDinID = SoeType;
		  //unsigned int *pchSoeTm;
		  unsigned char *pchSoeBuf;
		  
		  if((int)SoeType >= RI_1A_SHORT && SoeType < RMT_INFO_NUM)
		  {
			  
				//pchSoeTm = g_sRtcManager.m_gRealTimer;
	
          #ifdef LOCALYX
			   //���ֵ���soe��˫��soe
			  if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
			  {
				pchSoeBuf = g_gDCosBuff[g_unDCosBuffTail];
			  }
			  else
          #endif
			  {
				pchSoeBuf = g_gSCosBuff[g_unSCosBuffTail];
			  }
			  pchSoeBuf[SOE_FLAG] = SOEVALID;
			  pchSoeBuf[SOE_TYPEL] = TsSoeType; //SOE����(0-4095
			  pchSoeBuf[SOE_TYPEH] = TsSoeType >> 8;
			  pchSoeBuf[SOE_STVAL] = SoeValue;
			  pchSoeBuf[SOE_MSL] = (pAbsolutetime[RTC_MICROSEC]+pAbsolutetime[RTC_SEC]*1000)&0XFF;
			  pchSoeBuf[SOE_MSH] = (pAbsolutetime[RTC_MICROSEC]+pAbsolutetime[RTC_SEC]*1000)>>8;
			  pchSoeBuf[SOE_HOUR] = pAbsolutetime[RTC_HOUR];
			  pchSoeBuf[SOE_MINU] = pAbsolutetime[RTC_MINUT];
			  pchSoeBuf[SOE_DAY] =	pAbsolutetime[RTC_DATE];
			  pchSoeBuf[SOE_MONTH] = pAbsolutetime[RTC_MONTH];
			  pchSoeBuf[SOE_YEAR] = pAbsolutetime[RTC_YEAR]-2000;
			  //�����¼�����ʱ�̵�ң��ֵ��Ϊ�ϱ���ң����¼���¼
          	  PtrFlashStruct = (sSOE_RECORD_FLASH*)pchSoeBuf;
              PtrFlashStruct->YC_1_NO = 255;//�ȸ���ֵ����������Ϊ0����Ϊң�����0�к���
              PtrFlashStruct->YC_2_NO = 255;
             PtrFlashStruct->YC_3_NO = 255;
             PtrFlashStruct->YC_4_NO = 255;
          
           if(wFltTp != FTTPYE_ST  && wFltTp != 0xFFFF)//&& wFltTp != FTTPYE_XFLT
           {
              unsigned char Phase;
              unsigned int Line;
              unsigned int wYxYcNo;
              BYTE byRm1aFault;//�жϵ�ǰsoe�ǹ��ϻ��Ǹ���
              
              Phase = (SoeType - RI_1A_SHORT) % PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType - RI_1A_SHORT) %(PHASE_NUM*LINE_NUM)) / PHASE_NUM; //��0��ʼ
              wYxYcNo = Line * PHASE_NUM + Phase;
              if((wFltTp == FTTPYE_GL || wFltTp == FTTPYE_SGL || wFltTp == FTTPYE_YGL) && (SoeValue == 1))  //������·/˲ʱ/���ù��ϵ�ʱ��
              {
                 byRm1aFault = RM_1A_FAULT_I;
              }
              else
              {
                 byRm1aFault = RM_1A_LOAD_I;
              }
              PtrFlashStruct->YC_1_NO = byRm1aFault + wYxYcNo;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_NO = RN_1A_BAT_U + wYxYcNo;
              PtrFlashStruct->YC_2_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              PtrFlashStruct->YC_2_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              if((g_gRunPara[Terminal_Type] == 0) && (Line <= 2))  //�ܿ��ն�
              {
                  PtrFlashStruct->YC_3_NO = RN_1A_EFIELD_U + wYxYcNo;
                  PtrFlashStruct->YC_3_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  PtrFlashStruct->YC_3_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  //PtrFlashStruct->YC_4_NO = 255;    //�ܿ������¶� 
              }
              else   //�����ն��޵糡��ѹ
              {
                  //PtrFlashStruct->YC_3_NO = 255;
                  PtrFlashStruct->YC_4_NO = RM_1A_CABLE_T_INT + wYxYcNo;
                  PtrFlashStruct->YC_4_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_4_NO]);
                  PtrFlashStruct->YC_4_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_4_NO]);
              }
          }
          else if((SoeType >= RI_ZD_LVD) && (SoeType <= RI_ZD_BVD))
          {
              PtrFlashStruct->YC_1_NO = RM_BAT_U;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_NO = RM_SUN_U;
              PtrFlashStruct->YC_2_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              //PtrFlashStruct->YC_3_NO = 255;
              //PtrFlashStruct->YC_4_NO = 255;
          }
		   
		 }
     #ifdef LOCALYX
		//����˫��soeβָ��
		  if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
		  {  
			g_unDCosBuffTail++;
			g_unDCosBuffTail &= (DSOE_BUFF_NUM - 1);
			
			//=========�ж�ͷָ����βָ��λ��================
			if(g_unDSoeSavePtr == g_unDCosBuffTail) 
			{
			  g_unDSoeSavePtr++;
			  g_unDSoeSavePtr &= (DSOE_BUFF_NUM - 1);
			}
			
			if(pDbg != null)
			{
			  if(pDbg->m_DSoeHeadPtr == g_unDCosBuffTail)
			  {
				pDbg->m_DSoeHeadPtr++;
				pDbg->m_DSoeHeadPtr &= (DSOE_BUFF_NUM - 1);
			  }
			 }
	
			if(pGprs != null)
			{
			  if(pGprs->m_DSoeHeadPtr == g_unDCosBuffTail)
			  {
				pGprs->m_DSoeHeadPtr++;
				pGprs->m_DSoeHeadPtr &= (DSOE_BUFF_NUM - 1);
			  }
			 }
			
		  }
		  else
	#endif
		  {
			g_unSCosBuffTail++;
			g_unSCosBuffTail &= (SOE_BUFF_NUM - 1);
			
			  //=========�ж�ͷָ����βָ��λ��================
			  if(g_unSSoeSavePtr == g_unSCosBuffTail) 
			  {
				g_unSSoeSavePtr++;
				g_unSSoeSavePtr &= (SOE_BUFF_NUM - 1);
			  }
          #ifndef GETSOEFROMRAM
				if(g_unSSoeSaveFlashPtr == g_unSCosBuffTail) 
				{
				  g_unSSoeSaveFlashPtr++;
				  g_unSSoeSaveFlashPtr &= (SOE_BUFF_NUM - 1);
				}
				 if(g_gSmsHeadPt == g_unSCosBuffTail) 
				  {
					g_gSmsHeadPt++;
					g_gSmsHeadPt &= (SOE_BUFF_NUM - 1);
				  } 
          #else
				if(pDbg != null)
				{
				  if(pDbg->m_SSoeHeadPtr == g_unSCosBuffTail)
				  {
					pDbg->m_SSoeHeadPtr++;
					pDbg->m_SSoeHeadPtr &= (SOE_BUFF_NUM - 1);
				  }
				 }
		
				if(pGprs != null)
				{
				  if(pGprs->m_SSoeHeadPtr == g_unSCosBuffTail) 
				  {
					pGprs->m_SSoeHeadPtr++;
					pGprs->m_SSoeHeadPtr &= (SOE_BUFF_NUM - 1);
				  }
				 }
				  if(g_gSmsHeadPt == g_unSCosBuffTail) 
				  {
					g_gSmsHeadPt++;
					g_gSmsHeadPt &= (SOE_BUFF_NUM - 1);
				  } 
           #endif
		  }
	
		g_Soenum++;
		if(g_Soenum >= SOE_BUFF_NUM)
		  g_Soenum = SOE_BUFF_NUM;
	
	
	CombYx:
		//���ݷ���ң�ż�����·��ң��״̬�Լ������,�ŵ������������֮��
		//if(Flag !=2)
		{
		  if(((int)SoeType >= RI_1A_SHORT && SoeType <= RI_3O_POWERCUT)
                     ||((SoeType >= RI_1A_BATPOW_EN) && (SoeType <= RI_3O_BATPOW_EN)))//RI_12O_RSVRI_3O_RSV
			SetAllYxStatus(SoeType); 
		}
		   
}

//Flag = 1 ��ʾɨ��Ӳң�ų��������� 2 ��ʾң�ŶԵ������SOE(�����ϳ�)
void CreatNewSoe(unsigned int SoeType,unsigned long SoeValue,unsigned char Flag)
{//SoeType:ʵң��
    unsigned int TsSoeType;//,TsSoeDNum,TsSoeNum;
    //unsigned int iDinID = 0;
    sSOE_RECORD_FLASH* PtrFlashStruct; 

	WORD wFltTp = 0xFFFF;
    if(((int)SoeType >= RI_1A_SHORT) &&(SoeType <= RI_3O_POWERCUT))
    {
      WORD wOffet = SoeType - RI_1A_SHORT;
      WORD wFltNum = RI_FLT_NUM;
      wFltTp = wOffet/wFltNum;
    }
    //unsigned char Phase;
    //unsigned int Line;
    //unsigned int wYxYcNo;
    
      if((int)SoeType >= RI_1A_SHORT && SoeType < RMT_INFO_NUM)
      {
          if(GetRIStatus(SoeType) == SoeValue)
           goto CombYx;//return;//����ң����״̬����״̬һ�����������²���
      }
      SetRIStatus(SoeType,SoeValue);
      
      
      TsSoeType = RealYxToTsYx(SoeType);
      if(TsSoeType == 0xffff)
          goto CombYx;//return;
      
      if(pGprs != null) SETBIT(pGprs->m_gRmtChgFlag,SoeType,1);
      if(pDbg != null)  SETBIT(pDbg->m_gRmtChgFlag,SoeType,1);
      //����SOE֮ǰ�����ֵ����˫�㣬��Ϊ�����soe���Ͳ���ң�ŵ���Ϣ���ַ
      //������Э���д���soe�Ͳ����ٷֱ�����Ϣ���ַ
      if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
        TsSoeType = g_gRunPara[RP_DYX_INFADDR] - 1 + TsSoeType - g_ucDYxTrsStartId;
      else
        TsSoeType = g_gRunPara[RP_SYX_INFADDR] - 1 + TsSoeType - g_ucSYxTrsStartId;
        
 
      g_yxChangeflag = 0xffff;   
      g_ucRefYxYcTs |= BIT0;
	  SetYxTrans();
      //int iDinID = SoeType;
      unsigned int *pchSoeTm;
      unsigned char *pchSoeBuf;
      
      if((int)SoeType >= RI_1A_SHORT && SoeType < RMT_INFO_NUM)
      {
          if(Flag == 1)  //ɨ��Ӳң�ų���������
          {
              /*if(SoeType>=RI_ZD_DYX1 && SoeType<=RI_ZD_DYX12)
              {
                  iDinID = (SoeType-RI_ZD_DYX1)*2;//TsSoeDNum*2; 
              }
              else if(SoeType>=RI_ZD_YX1 && SoeType<=RI_ZD_YX24)
              {
                iDinID = SoeType-RI_ZD_YX1;//TsSoeNum;
              }*/
              //pchSoeTm = g_gDinRtcLock[iDinID];
              pchSoeTm = g_sRtcManager.m_gRealTimer;
          }
          else
          {
             pchSoeTm = g_sRtcManager.m_gRealTimer;
          } 

           //���ֵ���soe��˫��soe
          #ifdef LOCALYX
          if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
          {
            pchSoeBuf = g_gDCosBuff[g_unDCosBuffTail];
          }
          else
          #endif
          {
            pchSoeBuf = g_gSCosBuff[g_unSCosBuffTail];
          }
          pchSoeBuf[SOE_FLAG] = SOEVALID;
          pchSoeBuf[SOE_TYPEL] = TsSoeType; //SOE����(0-4095
          pchSoeBuf[SOE_TYPEH] = TsSoeType >> 8;
          pchSoeBuf[SOE_STVAL] = SoeValue;
          pchSoeBuf[SOE_MSL] = (pchSoeTm[RTC_MICROSEC]+pchSoeTm[RTC_SEC]*1000)&0XFF;
          pchSoeBuf[SOE_MSH] = (pchSoeTm[RTC_MICROSEC]+pchSoeTm[RTC_SEC]*1000)>>8;
          pchSoeBuf[SOE_HOUR] = pchSoeTm[RTC_HOUR];
          pchSoeBuf[SOE_MINU] = pchSoeTm[RTC_MINUT];
          pchSoeBuf[SOE_DAY] =  pchSoeTm[RTC_DATE];
          pchSoeBuf[SOE_MONTH] = pchSoeTm[RTC_MONTH];
          pchSoeBuf[SOE_YEAR] = pchSoeTm[RTC_YEAR]-2000;
		  //�����¼�����ʱ�̵�ң��ֵ��Ϊ�ϱ���ң����¼���¼
          PtrFlashStruct = (sSOE_RECORD_FLASH*)pchSoeBuf;
          PtrFlashStruct->YC_1_NO = 255;//�ȸ���ֵ����������Ϊ0����Ϊң�����0�к���
          PtrFlashStruct->YC_2_NO = 255;
          PtrFlashStruct->YC_3_NO = 255;
          PtrFlashStruct->YC_4_NO = 255;
          
          if(wFltTp != FTTPYE_ST  && wFltTp != 0xFFFF)//&& wFltTp != FTTPYE_XFLT
          {
              unsigned char Phase;
              unsigned int Line;
              unsigned int wYxYcNo;
              BYTE byRm1aFault;//�жϵ�ǰsoe�ǹ��ϻ��Ǹ���
              
              Phase = (SoeType - RI_1A_SHORT) % PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType - RI_1A_SHORT) %(PHASE_NUM*LINE_NUM)) / PHASE_NUM; //��0��ʼ
              wYxYcNo = Line * PHASE_NUM + Phase;
              if((wFltTp == FTTPYE_GL || wFltTp == FTTPYE_SGL || wFltTp == FTTPYE_YGL) && (SoeValue == 1))  //������·/˲ʱ/���ù��ϵ�ʱ��
              {
                 byRm1aFault = RM_1A_FAULT_I;
              }
              else
              {
                 byRm1aFault = RM_1A_LOAD_I;
              }
              PtrFlashStruct->YC_1_NO = byRm1aFault + wYxYcNo;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_NO = RN_1A_BAT_U + wYxYcNo;
              PtrFlashStruct->YC_2_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              PtrFlashStruct->YC_2_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              if((g_gRunPara[Terminal_Type] == 0) && (Line <= 2))  //�ܿ��ն�
              {
                  PtrFlashStruct->YC_3_NO = RN_1A_EFIELD_U + wYxYcNo;
                  PtrFlashStruct->YC_3_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  PtrFlashStruct->YC_3_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  //PtrFlashStruct->YC_4_NO = 255;    //�ܿ������¶� 
              }
              else   //�����ն��޵糡��ѹ
              {
                  //PtrFlashStruct->YC_3_NO = 255;
                  PtrFlashStruct->YC_4_NO = RM_1A_CABLE_T_INT + wYxYcNo;
                  PtrFlashStruct->YC_4_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_4_NO]);
                  PtrFlashStruct->YC_4_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_4_NO]);
              }
          }
          else if((SoeType >= RI_ZD_LVD) && (SoeType <= RI_ZD_BVD))
          {
              PtrFlashStruct->YC_1_NO = RM_BAT_U;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_NO = RM_SUN_U;
              PtrFlashStruct->YC_2_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              //PtrFlashStruct->YC_3_NO = 255;
              //PtrFlashStruct->YC_4_NO = 255;
          }
          #if 0
          PtrFlashStruct = (sSOE_RECORD_FLASH*)pchSoeBuf;      //zx �����¼�ң�� 
          if(((SoeType <= RI_3O_SHORT) || ((SoeType>=RI_1A_RSV)&&(SoeType<=RI_3O_RSV))) && (SoeValue == 1))  //������·/˲ʱ/���ù��ϵ�ʱ��
          { 
              Phase = SoeType%PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType/PHASE_NUM)%LINE_NUM); //��0��ʼ
              wYxYcNo = Line * PHASE_NUM + Phase;
              PtrFlashStruct->YC_1_NO = RM_1A_FAULT_I+ wYxYcNo;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
//              wYxYcNo = Line * 3 + Phase;
//              PtrFlashStruct->YC_2_NO = RN_1A_BAT_U+ wYxYcNo;
//              PtrFlashStruct->YC_2_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
//              PtrFlashStruct->YC_2_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              PtrFlashStruct->YC_2_NO = 255;
              PtrFlashStruct->YC_3_NO = 255;
              PtrFlashStruct->YC_4_NO = 255;    //�ܿ������¶�              
          }
          else if(((SoeType >= RI_1A_GND) && (SoeType <= RI_3O_ELECON)) || (((SoeType <= RI_3O_SHORT) || ((SoeType>=RI_1A_RSV)&&(SoeType<=RI_3O_RSV))) && (SoeValue == 0)))
          {
              Phase = SoeType%PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType/PHASE_NUM)%LINE_NUM); //��0��ʼ
              wYxYcNo = Line * PHASE_NUM + Phase;
              PtrFlashStruct->YC_1_NO = RM_1A_LOAD_I+ wYxYcNo;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_NO = 255;
//              wYxYcNo = Line * 3 + Phase;
//              PtrFlashStruct->YC_2_NO = RN_1A_BAT_U+ wYxYcNo;
//              PtrFlashStruct->YC_2_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
//              PtrFlashStruct->YC_2_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              PtrFlashStruct->YC_3_NO = 255;
              PtrFlashStruct->YC_4_NO = 255;             
          }
          else if(((SoeType >= RI_1A_BATPOW_EN) && (SoeType <= RI_3O_BATPOW_EN)))
          {
              Phase = (SoeType-RI_1A_BATPOW_EN)%PHASE_NUM; //0-A,1-B,2-C,3-O
              Line  = ((SoeType - RI_1A_BATPOW_EN)/PHASE_NUM)%LINE_NUM;
              wYxYcNo = Line * PHASE_NUM + Phase;
              PtrFlashStruct->YC_1_NO = RM_1A_LOAD_I+ wYxYcNo;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_NO = 255;
              PtrFlashStruct->YC_3_NO = 255;
              PtrFlashStruct->YC_4_NO = 255;                
          }
          else if((SoeType >= RI_ZD_LVD) && (SoeType <= RI_ZD_BVD))
          {
              PtrFlashStruct->YC_1_NO = RM_BAT_U;
              PtrFlashStruct->YC_1_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_1_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_1_NO]);
              PtrFlashStruct->YC_2_NO = RM_SUN_U;
              PtrFlashStruct->YC_2_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              PtrFlashStruct->YC_2_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_2_NO]);
              PtrFlashStruct->YC_3_NO = 255;
              PtrFlashStruct->YC_4_NO = 255;
          }
          else
          {
              PtrFlashStruct->YC_1_NO = 255;
              PtrFlashStruct->YC_2_NO = 255;
              PtrFlashStruct->YC_3_NO = 255;
              PtrFlashStruct->YC_4_NO = 255;              
          }
          #endif        
      }
     #ifdef LOCALYX
	    //����˫��soeβָ��
      if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
      {  
        g_unDCosBuffTail++;
        g_unDCosBuffTail &= (DSOE_BUFF_NUM - 1);
        
        //=========�ж�ͷָ����βָ��λ��================
        if(g_unDSoeSavePtr == g_unDCosBuffTail) 
        {
          g_unDSoeSavePtr++;
          g_unDSoeSavePtr &= (DSOE_BUFF_NUM - 1);
        }
        
        if(pDbg != null)
        {
          if(pDbg->m_DSoeHeadPtr == g_unDCosBuffTail)
          {
            pDbg->m_DSoeHeadPtr++;
            pDbg->m_DSoeHeadPtr &= (DSOE_BUFF_NUM - 1);
          }
         }

        if(pGprs != null)
        {
          if(pGprs->m_DSoeHeadPtr == g_unDCosBuffTail)
          {
            pGprs->m_DSoeHeadPtr++;
            pGprs->m_DSoeHeadPtr &= (DSOE_BUFF_NUM - 1);
          }
         }
        
      }
      else
     #endif
      {
        g_unSCosBuffTail++;
        g_unSCosBuffTail &= (SOE_BUFF_NUM - 1);
        
          //=========�ж�ͷָ����βָ��λ��================
          if(g_unSSoeSavePtr == g_unSCosBuffTail) 
          {
            g_unSSoeSavePtr++;
            g_unSSoeSavePtr &= (SOE_BUFF_NUM - 1);
          }
          #ifndef GETSOEFROMRAM
            if(g_unSSoeSaveFlashPtr == g_unSCosBuffTail) 
            {
              g_unSSoeSaveFlashPtr++;
              g_unSSoeSaveFlashPtr &= (SOE_BUFF_NUM - 1);
            }
			 if(g_gSmsHeadPt == g_unSCosBuffTail) 
              {
                g_gSmsHeadPt++;
                g_gSmsHeadPt &= (SOE_BUFF_NUM - 1);
              } 
          #else
            if(pDbg != null)
            {
              if(pDbg->m_SSoeHeadPtr == g_unSCosBuffTail)
              {
                pDbg->m_SSoeHeadPtr++;
                pDbg->m_SSoeHeadPtr &= (SOE_BUFF_NUM - 1);
              }
             }
    
            if(pGprs != null)
            {
              if(pGprs->m_SSoeHeadPtr == g_unSCosBuffTail) 
              {
                pGprs->m_SSoeHeadPtr++;
                pGprs->m_SSoeHeadPtr &= (SOE_BUFF_NUM - 1);
              }
             }
              if(g_gSmsHeadPt == g_unSCosBuffTail) 
              {
                g_gSmsHeadPt++;
                g_gSmsHeadPt &= (SOE_BUFF_NUM - 1);
              } 
           #endif
      }

    g_Soenum++;
    if(g_Soenum >= SOE_BUFF_NUM)
      g_Soenum = SOE_BUFF_NUM;


CombYx:
    //���ݷ���ң�ż�����·��ң��״̬�Լ������,�ŵ������������֮��
    if(Flag !=2)
    {
      if(((int)SoeType >= RI_1A_SHORT && SoeType <= RI_3O_POWERCUT)
         ||((SoeType >= RI_1A_BATPOW_EN) && (SoeType <= RI_3O_BATPOW_EN)))//RI_3O_RSVRI_12O_RSV
        SetAllYxStatus(SoeType); 
    }
       
}
//==============================================================================
//  ��������   : OpenBatSmsGprs
//  ��������   : �й��ϲ���ʱ����ң����Ҫ�ϴ����ж��Ƿ�SMS,GPRS��BAT
//  �������   : ��
//  ����ֵ     : ��
//  ����˵��   : ��GPRSͨ�������е���
//  ����       ��
//==============================================================================
void OpenBatSmsGprs()
{
    if(g_yxChangeflag & BITF)//g_yxChangeflag�����λ��ʶ�б�����Ҫ�ϴ�
    {
      g_yxChangeflag &= ~BITF;

      if(g_gRunPara[RP_POWER_MODE]!= REALTIME)
       {//��ʵʱģʽ
           //if(!g_GprsPowerSt)//ȥ������ԭ���ǣ����¹���ʱ���������ر�GPRS����ʱ(Ĭ��5m)
           {
               if(!g_sTimer[TM_SMSSENDALLOW].m_TmCount && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
               {//�Ѿ����������ӳٷ��ͼ�ʱ����������
                   OpenTimer(TM_SMSSENDALLOW);
				   if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
				   {
                     BYTE bBuf[2] = {0xFA,0XFA};
                     MyPrintfD(bBuf,2);
				   }
               }
               if(!g_GprsPowerSt)//�������¹���ʱGPRS�Ѿ���������ʱ
               {
                  OpenTimer(TM_SENDFAULTDELAY); //��վ���������ӳ٣���GPRS�ϵ�ʱ�� 
               }
               OpenGPRS(1);
               //OpenTimer(TM_SENDADDR);  
               OpenTimer(TM_GPRSPWOERDOWN);
               if((g_gRmtMeas[RM_CAP_U] < 5500)&&(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL)) // ��ѹС��6V��������ر���
                {
                  OPEN_BAT_POWER;//  �������    
                   OpenTimer(TM_BATPWOERDOWN);//   ͳ�Ƶ��ʹ��ʱ��
                   if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
                   	{
                   		BYTE bBuf[2] = {0xBA,0XBA};
                     	MyPrintfD(bBuf,2);
                   	}
                }
           }
       }
       else//ʵʱģʽ
       {
          OpenGPRS(3);
          g_SendFault = ON;
          if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
            g_EnSmsSend = ON;
       }
    } 
}

//*******************************************************************
//*                     ��ȡ�洢SOE��ָ��                      *
//* ���ܣ���ȡ�洢SOE��ָ�룬Ϊ�洢SOE�ṩ�洢λ��ָ��
//* ��������
//********************************************************************/
void read_ptr(void)
{  
 char *Flash_ptr;    // *********2011-03-08 �޸�********************************************  
 char qq;  
 char wrt_over;
 unsigned char i,j;
 
 soe_ptr=0;
 soe_area=1;
 
 
 for(i=0;i<SOE_SEG;i++)      // ��ȡ ����
   {
    Flash_ptr = (char *) (SOE_RECORD_ADD + i * 512 + 511);  // �Ƿ�������д��    ����ַ0X27000
    qq=*Flash_ptr--;
    if(qq == 0x055)                                         ////�Ѿ�д�룬���ж���������Ƿ��Ѿ�д��
      {
       wrt_over = *Flash_ptr;
         
       if(wrt_over != 0x055)                                ////û��д����             
         {
          soe_area = i + 1;
          break; 
         }
      }
    else if(qq != 0x55)    // ��1��û��д��Ҫô�ǿգ�Ҫô�����һ���Ѿ�д�굼�µ�һ�α���������һ�λ�û�б�ռ�á�
      {
       soe_area = i+1;
       break;
      }
   }

 for(i=0;i<SOE_NUM_SEG;i++)         // ��ȡ ���� ָ��
   {
    Flash_ptr = (char *) (SOE_RECORD_ADD + (soe_area-1) * 512 + i*12); 
    qq=*Flash_ptr;
    if(qq != 0x55)
      {   
       soe_ptr = i;
       break;
      }
   }
 
 soe_num=0;
 for(i=0;i<SOE_SEG;i++)      // ��ȡsoe ����
   {
    Flash_ptr = (char *) (SOE_RECORD_ADD + i * 512 + 511);  // �Ƿ�������д��    ����ַ0X27000
    qq=*Flash_ptr--;
    if(qq == 0x055)                                         ////�Ѿ�д�룬���ж���������Ƿ��Ѿ�д��
      {
       wrt_over = *Flash_ptr;
       if(wrt_over == 0x055)        /////////�Ѿ�д��
         {soe_num+=SOE_NUM_SEG;}
       else                         ////û��д����             
         {
          for(j=0;j<SOE_NUM_SEG;j++)         // ��ʼ�ж϶���д��������
             {
              Flash_ptr = (char *) (SOE_RECORD_ADD + i * 512 + j*12); 
              qq=*Flash_ptr;
              if(qq == 0x55)
                {soe_num+=1;}
              else
                {break;}
             }
         }
      }
   }
   CAT_SpiWriteWords(EEPADD_SOE_NUM, 1, &soe_num);
 if(soe_area==1 && soe_ptr == 0)
   {
    _DINT();
    WDTCTL = WDTPW + WDTHOLD;          // д��8λ����+ֹͣ���Ź�
     
    Flash_ptr = (char *)(SOE_RECORD_ADD);      // ��ʼ��ָ��
    FCTL3 = FWKEY;                     // FLASH����
    FCTL1 = FWKEY + ERASE;             // ʹ�ܲ�������
    
    *Flash_ptr = 0;                    // ��д����������
    
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;              //д�������
    
    WDTCTL = WDT_ARST_1000;   //����WDT
    _EINT();
    
    soe_ptr=0;
    soe_area=1;
   }
}


//save ����SOE��flash
void  DiSoeSave(unsigned char *pSoeBuff)
{
 INT32U addr;             
// unsigned int j;

 _DINT();
 WDTCTL = WDTPW + WDTHOLD;          ////ֹͣ�ڲ����Ź�
 WatchDog();                        ////���ⲿ���Ź�
 
 if(soe_area ==0 || soe_area > SOE_SEG || soe_ptr >= SOE_NUM_SEG)   // �����쳣ʱ�����¶�ȡSOE�洢ָ�� ��ȡ������ֵsoe_ptr  ;soe_area;
   {read_ptr();}                                      //��Ҫ����24���洢�ռ�1-24  ÿ���ռ�洢����������42��0-41
 
 addr =(SOE_RECORD_ADD + (soe_area-1) * 512 + (soe_ptr*12) ); 
 WriteFlashBuff1(addr, &pSoeBuff[SOE_FLAG], 11); 

 
 if(soe_ptr == 0)                                          //ÿ�ε����ڶ����ֽ�Ϊ0x55��˵��������д�룻���һ���ֽ�дΪ0x55��˵����һ���Ѿ�ռ����
   {
    addr = SOE_RECORD_ADD + (soe_area) * 512 - 1 ;         //˵����д�����ݣ����ǲ�ȷ��д��û��
    WriteFlashByte(addr,0x55);
    //дһ���ֽ�
   }
 else if(soe_ptr == SOE_NUM_SEG-1)
   {
    addr = SOE_RECORD_ADD + (soe_area) * 512 - 2;         //˵�������Ѿ�д����
    WriteFlashByte(addr,0x55);
    //дһ���ֽ�
   }
 WatchDog();                             ////���ⲿ���Ź�
 soe_num++;
 soe_ptr++;
 if(soe_ptr >= SOE_NUM_SEG)                       //////////ÿ�����洢42��SOE��¼ 
   {
    soe_ptr=0;

    if(soe_num > SOE_NUM_SEG*SOE_SEG)                  /////����Ƿ񳬹��˴洢����������������ˣ�
      {soe_num-=SOE_NUM_SEG;}
     
    if(soe_area >= SOE_SEG)
      {
       soe_area=1;
       addr = SOE_RECORD_ADD;
      }
    else
      {
       soe_area++;
       addr = SOE_RECORD_ADD+(soe_area-1) * 512;
      }
    EraseFlash(addr);
   }
   CAT_SpiWriteWords(EEPADD_SOE_NUM, 1, &soe_num);
 WatchDog();                             ////���ⲿ���Ź�
 //WDTCTL = WDT_ARST_1000;
 _EINT();
}
void SaveSoeData(void)
{
 // extern unsigned int g_unDSoeSavePtr;//�������˫��SOEָ��
  //    extern unsigned int g_unSSoeSavePtr;//������ĵ���SOEָ��

    int SaveNum = 0;
    unsigned int BfT;
    //���浥��SOE
   BfT  = g_unSCosBuffTail;
    if(g_unSSoeSavePtr <= BfT) 
      SaveNum = BfT - g_unSSoeSavePtr;
    else
      SaveNum = SOE_BUFF_NUM - g_unSSoeSavePtr + BfT; 
    if(SaveNum == 0) 
      return;

    for(int i = 0; i < SaveNum;i++)
    {
      //��soe���浽flash
      DiSoeSave(g_gSCosBuff[g_unSSoeSavePtr]); 
      g_unSSoeSavePtr++;
      g_unSSoeSavePtr &= (SOE_BUFF_NUM-1);
    }
    
    //����˫��SOE
    #ifdef LOCALYX
    BfT  = g_unDCosBuffTail;
    if(g_unDSoeSavePtr <= BfT) 
      SaveNum = BfT - g_unDSoeSavePtr;
    else
      SaveNum = DSOE_BUFF_NUM - g_unDSoeSavePtr + BfT; 
    if(SaveNum == 0) 
      return;

    for(int i = 0; i < SaveNum;i++)
    {
      //��soe���浽flash
      DiSoeSave(g_gDCosBuff[g_unDSoeSavePtr]); 
      g_unDSoeSavePtr++;
      g_unDSoeSavePtr &= (DSOE_BUFF_NUM-1);
    }
    #endif
  return;
}
//==============================================================================
//  ��������   : ReadSoe
//  ��������   : ��ȡһ��SEG��SOE
//  �������   : pBuf,װ��ȡ����SOE���ݣ�
//                             iSegNo,����id��0��SOE_SEG-1
//                            iStartNo,��ʼ��� ,0��SOE_NUM_SEG-1
//                             iSoeNum,��Ҫ��ȡ��SOE����
//  �������   : <��>
//  ����ֵ     : ʵ�ʶ�����SOE����
//  ����˵��   : 
//  ����       :
//==============================================================================

int ReadSoe(unsigned char *pBuf,int iSegNo,int iStartNo,int iSoeNum)
{
  unsigned char *pAddr;
  int num_cnt = 0,RealNum=0;
  int j = 0;
    pAddr = (unsigned char *)(SOE_RECORD_ADD + (iSegNo*512) + 511);    //////����Ƿ�������д��
    if(*pAddr == 0x55) // ��������д�룬����
    {
      pAddr = (unsigned char *)(SOE_RECORD_ADD + iSegNo*512 + iStartNo*(SOE_DATA_LEN+1)); 
      for(num_cnt=0;num_cnt<SOE_NUM_SEG;num_cnt++)
      {
        //����һ�������Ƿ���������
        if(*pAddr==0x55)     //valid data
        {
          pAddr++; 
          for(j=0;j<SOE_SENDDA_LEN;j++) 
          {
            *pBuf++ = *pAddr++;
          }
          RealNum++;
          if(RealNum == iSoeNum)
            break;
          pAddr++;//ÿ��SOE��β��һ���ֽ�
        }
        else
          break;
       }
   }
  return RealNum;
}


//�����豸ʵң����ŵõ���Ҫ�����ң�����
//������Ϊ0xffff����ʾ��ң�Ų����ϴ������
unsigned int RealYxToTsYx(unsigned int RYxNo)
{
  unsigned int TsYxNo = 0xffff;
    for(int m = 0; m < g_ucYxTransNum; m++) //�жϸ�ң�ű�λ�Ƿ���Ҫ�ϴ�
    {
        if(RYxNo == (g_ucYXAddr[m] - 1))    //����Ҫ�ϴ���ң��
        {
            TsYxNo = m; //SOE����(0-4095
            break;
        }
    }
   return TsYxNo;
}

//==============================================================================
//  ��������   : 
//  ��������   : �õ�IEC101�µ����õ���ң����Ϣ
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================
void SetYxTrans(void)
{
  unsigned int i, RIIndex;
  int sYxCount = 0;
  int dYxCount = 0;

  if((g_ucRefYxYcTs & BIT0) == BIT0)//��ң�Ÿ������õ�ң�ŵ�ַ��������
    {
      g_ucRefYxYcTs &= NBIT0;
      for(i = 0; i < g_ucYxTransNum; i++)
      {
        RIIndex = (g_ucYXAddr[i] - 1);
        if(RIIndex < DYX_ADDR)
          SetRIStatus(sYxCount++,GetRIStatus(RIIndex),1);
       else if(RIIndex == 0xfffe)
          SetRIStatus(sYxCount++,0,1);
       else if(RIIndex == 0xfffd)
          SetRIStatus(sYxCount++,1,1);
        else
          SetRIStatus(dYxCount++,GetRIStatus(RIIndex),2);
      }
                             
    }               
                                              
}

//==============================================================================
//  ��������   : 
//  ��������   : �õ�IEC101�µ����õ���ң����Ϣ
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================
void SetYcTrans(void)
{
    unsigned char i;
    if((g_ucRefYxYcTs & BIT1) == BIT1)
    {
      g_ucRefYxYcTs &= NBIT1;
      for(i = 0; i < g_ucYcTransNum; i++)   //������Ҫ�ϴ���ң��ֵ�����վ���������л���
       {
      if(g_ucYCAddr[i]<255)
      {
          g_unYcTrans[i] = g_gRmtMeas[g_ucYCAddr[i] - 1];
      }
      else
        g_unYcTrans[i] = 0;
       }
      if(g_AdChgSdYc)
      {
        if(g_SendYc && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ADCHG_SDYC]) && g_AdChgSdYc == ON)
          g_sTimer[TM_SENDYC].m_TmFlag = ON;//����ң�⴫��
        g_AdChgSdYc = 0;
      }
      else if(g_FltChgSdYc)
      {
        if(g_SendYc && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_FTYC]))
          g_sTimer[TM_SENDYC].m_TmFlag = ON;//����ң�⴫��
        g_FltChgSdYc = 0;
      }
    }                                         
}



//==============================================================================
//  ��������   : RstRMTMeas
//  ��������   : ��λң��ֵ
//  �������   : 
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================
void RstRMTMeas()
{
    unsigned int i;

    for(i = 0; i < RMT_MEAS_NUM; i++)
    {
      g_gRmtMeas[i] = 0;
    }   
  
}



//==============================================================================
//  ��������   : RstRMTInfo
//  ��������   : ��λң��λ
//  �������   : 
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================
void RstRMTInfo(unsigned int SoeType)
{
    unsigned int i;
    for(i = 0; i < RMT_WORD_NUM; i++)
    {
        g_gRmtInfo[i] = 0;
    }
}

//==============================================================================
//  ��������   : SaveLog
//  ��������   : ����־��¼���ݱ��浽flash��
//  �������   : <��>
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : ��¼���㸺�����ݣ������ʽΪ
//                   ��ʼ��(1)  ��¼�ֽ���(1)   ʱ��(5)     ����(X)
//                   0xEA         L             YYMMDDhhmm     xx   
//  ����       : 
//==============================================================================
//unsigned char ucLOADData[64][7]={0};//for test
//unsigned char min = 0;//for test
void SaveLog(unsigned int LogType,char *pLogText,unsigned char LogValue,INT8U Log_Length)
{
    unsigned char LogData[256];
    unsigned char LogDataPtr=0;
    unsigned long FLogAddr;
    unsigned int FLogInfo[FLOGINFONUM];      //��EEPROM�ж�ȡ������FLASH�б��渺�ɼ�¼�������Ϣ��������+�����洢��¼��λ��+����һ����¼��λ��+У��
    LogData[LogDataPtr++] = 0xEA;     //��ʼ��
    LogData[LogDataPtr++] = 9 + Log_Length; //5 + RM_LOADI_NUM*2; //�������ɼ�¼�ĳ���Ϊ5�ֽ�ʱ����ϸ��ɵ�������
	
	LogData[LogDataPtr++] = LogValue;
	LogData[LogDataPtr++] = LogType;
	LogData[LogDataPtr++] = LogType>>8;
    //ʱ�� ��¼�ꡢ�¡��ա�ʱ����
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_YEAR] - 2000;
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MONTH];
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_DATE];

    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_HOUR];
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_SEC];

    memcpy(&LogData[LogDataPtr],pLogText,Log_Length);//g_gRmtMeas[RM_1A_LOAD_I],2*RM_LOADI_NUM

    CAT_SpiReadWords(EEPADD_LOGNUM, FLOGINFONUM, FLogInfo);
    if((FLogInfo[FLOG_CS] != FLogInfo[FLOG_TOTALNUM] + FLogInfo[FLOG_NEW] + FLogInfo[FLOG_OLD]) || FLogInfo[FLOG_TOTALNUM] > FLASH_LOG_MAXNUM || FLogInfo[FLOG_NEW] > FLASH_LOG_MAXNUM)//���FLASH��ַ���ڸ��ɼ�¼����������
    {
        FLogInfo[FLOG_TOTALNUM] = 0; //��ո��ɼ�¼������
        FLogInfo[FLOG_NEW] = 0; //�����洢�ļ�¼���һ����¼��ƫ��λ��(�����Ѵ洢������һ����¼��λ��)
        FLogInfo[FLOG_OLD] = 0;//����һ����¼��λ��
    }
    FLogAddr = FADDR_LOG_START + ((unsigned long)FLogInfo[FLOAD_NEW] * FLASH_PLOG_LEN);
    if(FLogAddr == FADDR_LOG_START || (((unsigned long)FLogInfo[FLOAD_NEW] * FLASH_PLOG_LEN) >= 4096 && ((unsigned long)FLogInfo[FLOAD_NEW] * FLASH_PLOG_LEN) % 4096 == 0))
    {
        if(FLogInfo[FLOG_TOTALNUM] == FLASH_LOG_MAXNUM)
        {
          FLogInfo[FLOG_TOTALNUM] -= (4096/FLASH_PLOG_LEN);
          //FLogInfo[FLOG_OLD] += (4096/FLASH_PLOG_LEN);
          //if(FLogInfo[FLOG_OLD] >= FLASH_LOG_MAXNUM)
            //FLogInfo[FLOG_OLD] = 0;
        }
        if(FLogInfo[FLOG_NEW] == FLASH_LOG_MAXNUM)
        FLogAddr = FADDR_LOG_START;
        Sector_Erase(FLogAddr);
    }
    //if(FLogInfo[FLOG_NEW] == FLASH_LOG_MAXNUM)
       // FLogAddr = FADDR_LOG_START;
    Sst26vf064b_Page_WriteBytes(FLogAddr, LogData, (11 + Log_Length));//�����ݱ��浽FLASH��
    
//    for(int i = 0; i<64; i++)
//    {
//      FLoadAddr = i*FLASH_PLOAD_LEN;
//      Sst26vf064b_Read(FLoadAddr,ucLOADData[i], 7);
//    }
 
    
    if(FLogInfo[FLOG_TOTALNUM] < FLASH_LOG_MAXNUM)//���û�д�������FLASH�б���ĸ��ɼ�¼��������
    {
        FLogInfo[FLOG_TOTALNUM]++;
    }
    //FLogInfo[FLOG_NEW]++;
    if(FLogInfo[FLOG_NEW] >= FLASH_LOG_MAXNUM)
    {
        FLogInfo[FLOG_NEW] = 0;
    }
	 FLogInfo[FLOG_NEW]++;
    FLogInfo[FLOG_CS] = FLogInfo[FLOG_NEW] + FLogInfo[FLOG_OLD] + FLogInfo[FLOG_TOTALNUM];
    //��EEPROM�м�¼����һ�����ɼ�¼��λ��
    CAT_SpiWriteWords(EEPADD_LOGNUM, FLOGINFONUM, FLogInfo);
  
}

//==============================================================================
//  ��������   : SaveLoad
//  ��������   : �Ѹ���ͳ�Ƶ����ݱ��浽flash��
//  �������   : <��>
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : ��¼���㸺�����ݣ������ʽΪ
//                   ��ʼ��(1)  ��¼�ֽ���(1)   ʱ��(5)     ����(X)
//                   0xEA         L             YYMMDDhhmm     xx   
//  ����       : 
//==============================================================================
//unsigned char ucLOADData[64][7]={0};//for test
//unsigned char min = 0;//for test
void SaveLoad(unsigned char LoadFlag)
{
    unsigned char LoadData[128];
    unsigned char LoadDa[10];
	unsigned char DayLoad[31];
	unsigned char i;
    unsigned char LoadDataPtr=0;
    unsigned long FLoadAddr;
    unsigned int FLoadInfo[FLOADINFONUM];      //��EEPROM�ж�ȡ������FLASH�б��渺�ɼ�¼�������Ϣ��������+�����洢��¼��λ��+����һ����¼��λ��+У��
    LoadData[LoadDataPtr++] = 0xEA;     //��ʼ��
    LoadData[LoadDataPtr++] = 6 + RM_1A_LUBO*2; //RM_1A_CABLE_T5 + RM_LOADI_NUM*2; //�������ɼ�¼�ĳ���Ϊ5�ֽ�ʱ����ϸ��ɵ�������

    //ʱ�� ��¼�ꡢ�¡��ա�ʱ����
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_YEAR] - 2000;
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MONTH];
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_DATE];

    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_HOUR];

    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_SEC];

    memcpy(&LoadData[LoadDataPtr],&g_gRmtMeas[RM_BAT_U],2*RM_1A_LUBO);//RM_1A_CABLE_Tg_gRmtMeas[RM_1A_LOAD_I],2*RM_LOADI_NUM
    CAT_SpiReadBytes(EEPADD_DAYNUM,31,DayLoad);
    CAT_SpiReadWords(EEPADD_LOADNUM, FLOADINFONUM, FLoadInfo);
	
    if((FLoadInfo[FLOAD_CS] != FLoadInfo[FLOAD_TOTALNUM] + FLoadInfo[FLOAD_NEW] + FLoadInfo[FLOAD_OLD]+FLoadInfo[FLOAD_DAY]) || FLoadInfo[FLOAD_TOTALNUM] > FLASH_LOAD_MAXNUM || FLoadInfo[FLOAD_NEW] > FLASH_LOAD_MAXNUM)//���FLASH��ַ���ڸ��ɼ�¼����������
    {
        FLoadInfo[FLOAD_TOTALNUM] = 0; //��ո��ɼ�¼������
        FLoadInfo[FLOAD_NEW] = 0; //�����洢�ļ�¼���һ����¼��ƫ��λ��(�����Ѵ洢������һ����¼��λ��)
        FLoadInfo[FLOAD_OLD] = 0;//���ɼ�¼�洢�ĵ�ַƫ����
        FLoadInfo[FLOAD_DAY] =0;
		for(i =0; i<31;i++)
			DayLoad[i] =0;
		CAT_SpiWriteBytes(EEPADD_DAYNUM, 31, DayLoad);
    }
	 if(FLoadInfo[FLOAD_TOTALNUM] >0)
         {
            if(FLoadInfo[FLOAD_NEW] >0)
              FLoadAddr = FADDR_LOAD_START + ((unsigned long)(FLoadInfo[FLOAD_NEW]-1) * FLASH_PLOAD_LEN);
	    else
              FLoadAddr = FADDR_LOAD_START + ((unsigned long)(FLASH_LOAD_MAXNUM-1) * FLASH_PLOAD_LEN);
            Sst26vf064b_Read(FLoadAddr,LoadDa,10);
			//����RTC��ʱ�����ܻ�Ե�ǰ��ʱ���޸ģ����´�������ͬʱ��ĸ��ɼ�¼���ڴ��ж�������ϴεĸ��ɼ�¼ʱ��һ�����򱾴θ��ɲ��ٱ���
			if((LoadDa[2] == (g_sRtcManager.m_gRealTimer[RTC_YEAR] - 2000)) &&(LoadDa[3] == g_sRtcManager.m_gRealTimer[RTC_MONTH])&&(LoadDa[4] ==g_sRtcManager.m_gRealTimer[RTC_DATE] )&&(LoadDa[5]== g_sRtcManager.m_gRealTimer[RTC_HOUR])&&(LoadDa[6] == g_sRtcManager.m_gRealTimer[RTC_MINUT]))
				return;
			if(g_sRtcManager.m_gRealTimer[RTC_DATE] != LoadDa[4])//������һ�±�������һ���뱾�α����������һ��
	 			LoadFlag = 0x55;
     }
	//Sst26vf064b_Page_WriteBytes(FLoadAddr, LoadData, LOAD_LEN);//�����ݱ��浽FLASH��
	
	if((LoadFlag == 0x55) &&(FLoadInfo[FLOAD_TOTALNUM]>0))//�µ��ļ�����Ӧ��һ��ļ�¼
	{
		//if(FLoadInfo[FLOAD_NEW] % 96)
		{
			/*if(FLoadInfo[FLOAD_NEW] <= (FLoadInfo[FLOAD_OLD]+1)*96] )
				DayLoad[FLoadInfo[FLOAD_OLD]] = FLoadInfo[FLOAD_NEW] % 96;//FLoadInfo[FLOAD_DAY]
			else
				DayLoad[FLoadInfo[FLOAD_OLD]] =96;*/
			FLoadInfo[FLOAD_NEW] = (FLoadInfo[FLOAD_OLD]+1)*96;//FLoadInfo[FLOAD_DAY]
			if(FLoadInfo[FLOAD_NEW] >= FLASH_LOAD_MAXNUM)
    		{
        		FLoadInfo[FLOAD_NEW] = 0;
    		}
			
			if(FLoadInfo[FLOAD_TOTALNUM] < FLASH_LOAD_MAXNUM)//���û�д�������FLASH�б���ĸ��ɼ�¼��������
    		{
        		FLoadInfo[FLOAD_TOTALNUM] = (FLoadInfo[FLOAD_OLD]+1)*96;//FLoadInfo[FLOAD_DAY]FLoadInfo[FLOAD_TOTALNUM]++;
    		}
		}
		//else
			//DayLoad[FLoadInfo[FLOAD_OLD]] = 96;//FLoadInfo[FLOAD_DAY]
		 //CAT_SpiWriteBytes(EEPADD_DAYNUM+FLoadInfo[FLOAD_OLD], 1, &DayLoad[FLoadInfo[FLOAD_OLD]]);//FLoadInfo[FLOAD_DAY]
		/*if(FLoadInfo[FLOAD_OLD]>=30) //FLoadInfo[FLOAD_DAY] 
		{
		  //FLoadInfo[FLOAD_DAY] =30;//0;
		  FLoadInfo[FLOAD_OLD]=0;
		  
		}*/
		if( FLoadInfo[FLOAD_TOTALNUM] >0)
		{
			if( FLoadInfo[FLOAD_DAY] >=30)
				FLoadInfo[FLOAD_DAY] =30;
			else
				FLoadInfo[FLOAD_DAY]++;
			FLoadInfo[FLOAD_OLD]++;
			if(FLoadInfo[FLOAD_OLD]>=31) //FLoadInfo[FLOAD_DAY] 
			{
		  //FLoadInfo[FLOAD_DAY] =30;//0;
		  		FLoadInfo[FLOAD_OLD]=0;
			}
			DayLoad[FLoadInfo[FLOAD_OLD]]=0;
		}
		
	}
	
    FLoadAddr = FADDR_LOAD_START + ((unsigned long)FLoadInfo[FLOAD_NEW] * FLASH_PLOAD_LEN);//(FLoadInfo[FLOAD_DAY] * FLASH_DAYLOAD_LEN);//
    //if(FLoadAddr == FADDR_LOAD_START || ((FLoadInfo[FLOAD_NEW] * FLASH_PLOAD_LEN) >= 4096 && (FLoadInfo[1] * FLASH_PLOAD_LEN) % 4096 == 0))
	if(FLoadAddr % FLASH_DAYLOAD_LEN ==0)//(FLoadAddr % 4096 ==0)һ��ļ�¼����
	{
        if(FLoadInfo[FLOAD_TOTALNUM] == FLASH_LOAD_MAXNUM)
        {
          FLoadInfo[FLOAD_TOTALNUM] -= 96;//(4096/FLASH_PLOAD_LEN);
          //FLoadInfo[FLOAD_OLD] += 96;//(4096/FLASH_PLOAD_LEN);
          //if(FLoadInfo[FLOAD_OLD] >= FLASH_LOAD_MAXNUM)
            //FLoadInfo[FLOAD_OLD] = 0;
        }
        Sector_Erase(FLoadAddr);//ÿ�β���12K;
		Sector_Erase(FLoadAddr +4096);
		Sector_Erase(FLoadAddr +(4096*2));
		
    }
    if(FLoadInfo[FLOAD_NEW] == 0)//FLASH_LOAD_MAXNUM
        FLoadAddr = FADDR_LOAD_START;
    Sst26vf064b_Page_WriteBytes(FLoadAddr, LoadData, LOAD_LEN);//�����ݱ��浽FLASH��
    
//    for(int i = 0; i<64; i++)
//    {
//      FLoadAddr = i*FLASH_PLOAD_LEN;
//      Sst26vf064b_Read(FLoadAddr,ucLOADData[i], 7);
//    }
 
    
    if(FLoadInfo[FLOAD_TOTALNUM] < FLASH_LOAD_MAXNUM)//���û�д�������FLASH�б���ĸ��ɼ�¼��������
    {
        FLoadInfo[FLOAD_TOTALNUM]++;
    }
    FLoadInfo[FLOAD_NEW]++;
	DayLoad[FLoadInfo[FLOAD_OLD]]++;//FLoadInfo[FLOAD_DAY]= FLoadInfo[FLOAD_NEW] % 96;
    if(FLoadInfo[FLOAD_NEW] >= FLASH_LOAD_MAXNUM)
    {
        FLoadInfo[FLOAD_NEW] = 0;
    }
	if(DayLoad[FLoadInfo[FLOAD_OLD]] >=96)//FLoadInfo[FLOAD_DAY]
	{
		DayLoad[FLoadInfo[FLOAD_OLD]] =96;//
	}
    FLoadInfo[FLOAD_CS] = FLoadInfo[FLOAD_NEW] + FLoadInfo[FLOAD_OLD] + FLoadInfo[FLOAD_TOTALNUM] + FLoadInfo[FLOAD_DAY];
    //��EEPROM�м�¼����һ�����ɼ�¼��λ��
    CAT_SpiWriteWords(EEPADD_LOADNUM, FLOADINFONUM, FLoadInfo);
	CAT_SpiWriteBytes(EEPADD_DAYNUM+FLoadInfo[FLOAD_OLD], 1, &DayLoad[FLoadInfo[FLOAD_OLD]]);//FLoadInfo[FLOAD_DAY]
  
}
//==============================================================================
//  ��������   : CopySoeToFlash
//  ��������   : �ڴ��е�SOE���ݵ����ⲿFLASH
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void CopySoeToFlash(void)
{
     int SaveNum = 0;
      unsigned int BfT;
      unsigned long FSoeAddrOfset;//����SOE�ĵ�ַƫ����
      //BYTE a[16];
      
      //���浥��SOE
      if(g_unSSoeSaveFlashPtr == g_unSCosBuffTail) return;

      BfT  = g_unSCosBuffTail;
      if(g_unSSoeSaveFlashPtr <= BfT) 
        SaveNum = BfT - g_unSSoeSaveFlashPtr;
      else                            
        SaveNum = SOE_BUFF_NUM - g_unSSoeSaveFlashPtr + BfT; 
        
      if(SaveNum == 0) return;

      
      //��soe���浽flash
      for(int i = 0; i < SaveNum;i++)
      {
        //�ж�Ҫ��Ҫ�߳�
        g_unSSoeInFlashTail &= (FSOE_TOLNUM-1);//����ǰ���ԭ����SOE��1024���ں�����0����ͷָ����ͬ��������SOE
        FSoeAddrOfset = g_unSSoeInFlashTail * FSOE_LEN;
        if( !(FSoeAddrOfset % 4096) )//ÿ���߳���Ҫ��4k�ռ�,256��SOE
        {
            Sector_Erase(FADDR_SOE_START + FSoeAddrOfset);
            if(g_InFlashInfo.m_OverWriteFlag == ON)
            {
                  g_InFlashInfo.m_InFlashTailNum -= 128;
                  g_InFlashInfo.m_unSSoeInFlashHead = (g_unSSoeInFlashTail + 128);
                  g_InFlashInfo.m_unSSoeInFlashHead &= (FSOE_TOLNUM-1);
            }
            else
                g_InFlashInfo.m_unSSoeInFlashHead = 0;
        }
         //Sst26vf064b_Read(FADDR_SOE_START + 1024,a, SOE_DATA_LEN);
        Sst26vf064b_Page_WriteBytes(FADDR_SOE_START + FSoeAddrOfset, g_gSCosBuff[g_unSSoeSaveFlashPtr],SOE_DATA_LEN); //�����ݱ������FLASH��
         Sst26vf064b_Read(FADDR_SOE_START + FSoeAddrOfset,g_gSCosBuff[g_unSSoeSaveFlashPtr], SOE_DATA_LEN);
        g_unSSoeSaveFlashPtr++;
        g_unSSoeSaveFlashPtr &= (SOE_BUFF_NUM-1);

        g_unSSoeInFlashTail++;
		if(g_unSSoeInFlashTail >= FSOE_TOLNUM)
			g_InFlashInfo.m_OverWriteFlag = ON;
        g_unSSoeInFlashTail &= (FSOE_TOLNUM-1);
		//g_InFlashTailNum++;
		//if(g_InFlashInfo.m_OverWriteFlag == ON)
			//g_InFlashInfo.m_InFlashTailNum = FSOE_TOLNUM;	
		//else
			//g_InFlashInfo.m_InFlashTailNum = g_unSSoeInFlashTail 
		g_InFlashInfo.m_InFlashTailNum ++;
		if(g_InFlashInfo.m_InFlashTailNum >= FSOE_TOLNUM)
			g_InFlashInfo.m_InFlashTailNum = FSOE_TOLNUM;	
		CAT_SpiWriteWords(EEPADD_SOE_FLASHNUM, 3, (unsigned int *)&g_InFlashInfo);
        
        //=========�ж�ͷָ����βָ��λ��================
        //���߳���ʱ���ƶ�ͷָ�룬��SOE��¼ʱ�жϱ�������ݼ���
        if(pDbg != null)
        {
          if(pDbg->m_SSoeHeadPtr == g_unSSoeInFlashTail)
          {
            pDbg->m_SSoeHeadPtr ++;
            pDbg->m_SSoeHeadPtr &= (FSOE_TOLNUM - 1);
            CAT_SpiWriteWords(EEPADD_SSOEHEAD1, 1, &(pDbg->m_SSoeHeadPtr));
          }
         }

        if(pGprs != null)
        {
          if(pGprs->m_SSoeHeadPtr == g_unSSoeInFlashTail) 
          {
            pGprs->m_SSoeHeadPtr++;
            pGprs->m_SSoeHeadPtr &= (FSOE_TOLNUM - 1);
            CAT_SpiWriteWords(EEPADD_SSOEHEAD2, 1, &(pGprs->m_SSoeHeadPtr));
          }
         }
      }
      //��E2�б���flash�е�SOEβָ��
      CAT_SpiWriteWords(EEPADD_SSOETAIL, 1, &g_unSSoeInFlashTail);
    return;




}

float GetYcCoef(WORD wYcId)
{
    float wYcCoef = 1;
    //DWORD dwRpCoef=1;   //��ϵ����ϵ��Ĭ��ȫΪ1
    if((wYcId <= RM_SUN_U)
      	|| (wYcId >= RN_1A_CAP_U && wYcId <= RN_3C_CAP_U)
       	|| (wYcId >= RN_1A_BAT_U && wYcId <= RN_3C_BAT_U)  //��ص�ѹ�͵��ݵ�ѹ
     	|| (wYcId >= RM_1A_LUBO && wYcId <= RM_3C_LUBO)) //���ݵ�ѹ��ȡ���ѹ
    {
//        dwRpCoef = MAKEDWORD(g_gRunPara[RP_COEF_UL],g_gRunPara[RP_COEF_UH]);
        if(g_ucPara101[IECP_YC_TYPE] == 2)  //����������ѹ��λת��ΪV
            wYcCoef = wYcCoef / 1000;
    }
    else if(wYcId >= RM_1A_FAULT_I && wYcId <= RM_3O_LOAD_I)//�������Ŵ�10��
    {
        if(g_ucPara101[IECP_YC_TYPE] == 2)  //����������ѹ��λת��ΪV
            wYcCoef = wYcCoef / 10;         
    }
      
    //else if(wYcId >= RM_1A_FAULT_I && wYcId <= RM_12O_LOAD_I)
//    else if(wYcId >= RM_1A_FAULT_I && wYcId <= RM_3O_LOAD_I)
//    {
//        //dwRpCoef = MAKEDWORD(g_gRunPara[RP_COEF_IL],g_gRunPara[RP_COEF_IH]);
//        if((g_gRunPara[Terminal_Type] == AERIAL_TERMINAL) && (g_ucPara101[IECP_YC_TYPE] == 2))  //��������������λת��ΪA
//            dwRpCoef = dwRpCoef/10;
//    }
    //else if(wYcId >= RM_1A_CABLE_T && wYcId <= RM_12C_CABLE_T_INT)
//    else if(wYcId >= RM_1A_CABLE_T && wYcId <= RM_6C_CABLE_T_INT)
//    {
//        dwRpCoef = MAKEDWORD(g_gRunPara[RP_COEF_TL],g_gRunPara[RP_COEF_TH]);
//    }
    //wYcCoef = ((float)dwRpCoef)/100;
//    if(wYcId == RM_READGPRS_S) wYcCoef=1;
        return wYcCoef;
}


