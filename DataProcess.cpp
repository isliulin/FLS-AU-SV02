/*------------------------------------------------------------------------
 Module:      DataProcess.cpp
 Author:      linxueqin
 Project:     子站平台
 State:       V1.0
 Creation Date:   2014-4-29
 Description:   AD采样初始化，计算子站采集电压，创建和保存SOE，组织遥测遥信发送缓冲区
------------------------------------------------------------------------*/


#ifndef _DATA_PROCESS

    #define _DATA_PROCESS
    #include "DataStruct.h"
#include "Main.h"
#endif

//==============================================================================
//  函数名称   : InitDataProc
//  功能描述   : 数据处理初始化
//  输入参数   : <无>
//  输出参数   : <无>
//  返回值     : <无>
//  其他说明   : 
//  作者       : 
//==============================================================================
void InitDataProc(void)
{
    unsigned int i,j;
    for(i = 0; i < AC_AD_CHAN_NUM;i++)
      for(j = 0; j < AC_BUFF_LEN;j++)
        g_sSampleData[i][j] = 0;
    if(g_gRunPara[Terminal_Type] == AERIAL_TERMINAL)
    {//架空终端的通道状态判断才使用g_sCommStatCount这个数组
      for(i = 0; i < RMT_ZSQST_NUM; i++)
      {
          if( (i+1)%4 == 0)
            g_sCommStatCount[i] = 0;//架空没有0序，因此屏蔽其产生通道故障的条件。
          else
          g_sCommStatCount[i] = g_gRunPara[RP_ZSQCST_T];
      }
    }
    g_sSampleFinish = OFF;
    
    //soe变量初始化
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
    g_LogType = 0; //日志文件不同类型
    g_Linkflag =0;
    g_LogValue =0;
	g_buchangnum =0;
	g_buchangnum2 =0;
	g_ccr0 = 0;
	
}

//检测低电压告警及终端自检
void CheckZDAlm()
{
   #define PtMonCntMax 30
    //#define PtLoseCntMax    //电压低的时候不立即上传，连续监测低三次后上传
      if(g_gRmtMeas[RM_SUN_U] <= 11000 ) //600检测到电容式PT电压不足，清空遥测电流值
     {
        //RstRMTMeas();   
        if(g_PtLoseFlag == 0)
        {
          g_PtLoseFlag = 1;
          CreatNewSoe(RI_ZD_LVD,1);//失压，置位失压遥信区域  
        }
        else if(g_PtLoseFlag == 2)
        {
          g_PtMonitorCnt = PtMonCntMax;//电压回复过程中,若出现电压下降，则重新计时
        }  
     }
     else if(g_gRmtMeas[RM_SUN_U] > 11000)//600
     {
          if(g_PtLoseFlag == 1)//失压时发现电压有所升高，计时10次，如果连续有压，退出失压状态
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
          CreatNewSoe(RI_ZD_LVD,0); //电压恢复，置位遥信区域
       }
     }

      if(g_gRmtMeas[RM_BAT_U] <= g_gRunPara[RP_ALARM_BAT] ) //电池不足
     {  
        if(g_BatLoseFlag == 0)
        {
          g_BatLoseFlag = 1;
          CreatNewSoe(RI_ZD_BVD,1);//失压，置位失压遥信区域  
        }
//        else if(g_BatLoseFlag == 2)
//        {
//          g_BatMonitorCnt = PtMonCntMax;//电压回复过程中,若出现电压下降，则重新计时
//        }  
     }
     else if(g_gRmtMeas[RM_BAT_U] > g_gRunPara[RP_ALARM_BAT])
     {
          if(g_BatLoseFlag != 0)//失压时发现电压有所升高，计时10次，如果连续有压，退出失压状态
          {
              if(g_gRmtMeas[RM_BAT_U] > g_gRunPara[RP_ALARM_BAT] + 20)  //回差值0.2V
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
//终端自检判断，主电源和备用电源都失压则报自检错误 自检内容???
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
//  函数名称   : Check_LogFlag
//  功能描述   : 日志文件类型的产生
//  输入参数   : <无>
//  输出参数   : <无>
//  返回值     : <无>
//  其他说明   : 
//  作者       :
//==============================================================================

void Check_LogFlag(void)
{
  
    if((g_GprsPowerSt ==1) &&(pGprs->m_linkflag ==1))//通道建立连接与断开连接
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
      if(((g_LogType & BIT2) == 0)&&(g_LogType & BIT1))//建立连接之后若断开再记录
      {
          g_LogType |= BIT2;
          g_LogType &= NBIT1; 
          g_LogValue|= BIT2;
      }
    }
    if(g_Linkflag == 1)//通道过程异常，重发没有响应
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
      if(((g_LogType & BIT4) == 0) &&(g_LogType & BIT3))//通道过程异常之后再记录恢复
      {
        g_LogType |= BIT4;
        g_LogType &= NBIT3;
        g_LogValue|= BIT4;
      }
    }
    if(g_PtLoseFlag ==1)//主电源失压
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
      if(((g_LogType & BIT8) == 0) &&(g_LogType & BIT7))//主电源失压之后
      {
        g_LogType |= BIT8;
        g_LogType &= NBIT7; 
        g_LogValue|= BIT8;
      }
    }
    if(g_BatLoseFlag ==1)//电池电压低压
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
//  函数名称   : Check_LogFile
//  功能描述   : 日志文件的生成
//  输入参数   : <无>
//  输出参数   : <无>
//  返回值     : <无>
//  其他说明   : 
//  作者       :
//==============================================================================

void Check_LogFile(void)
{
  unsigned int  LogType ;
  char *PLogText;
  unsigned char  LogValue;
  INT8U leng_log;
   char ch[30];
    Check_LogFlag();
    if((g_LogType & BIT0) == BIT0)//终端重启记录
    {
      LogType =1;
      PLogText ="Terminal Restart";
      LogValue =1;
      g_LogType &= NBIT0; 
    }
    
    else if(((g_LogType & BIT1) == BIT1) &&((g_LogValue & BIT1) == BIT1))//通道建立连接与断开记录
    {
        LogType =2;  
        PLogText="Establish Connection";
        LogValue =1; 
        g_LogValue &= NBIT1;
    }
    else if(((g_LogType & BIT2) == BIT2)&&((g_LogValue & BIT2) == BIT2))//通道建立连接与断开记录
    {
        LogType =2;  
        PLogText="Disconnect Connection";
        LogValue =0; 
        g_LogValue &= NBIT2;

    }
    else if(((g_LogType & BIT3) == BIT3)&&((g_LogValue & BIT3) == BIT3))//通道过程异常
    {
      LogType =3;
      PLogText="Commmunication Process Exception";
      g_LogValue &= NBIT3;
      LogValue =1; 
    }
    else if(((g_LogType & BIT4) == BIT4)&&((g_LogValue & BIT4) == BIT4))//通道过程异常
    {
      LogType =3;
      PLogText="Commmunication Process Exception";
      g_LogValue &= NBIT4;
      LogValue =0; 
    }
    else if((g_LogType & BIT5) == BIT5)//元件自检异常
    {
      LogType =4;
      PLogText="Abnormal Element Self-Inspection";
      LogValue =1;  
    }
     else if((g_LogType & BIT6) == BIT6)//装置内部软件异常
     {
      LogType =5;
      PLogText="Abnormal Device Software";
      LogValue =1;  
    }
     else if(((g_LogType & BIT7) == BIT7)&&((g_LogValue & BIT7) == BIT7))
    {
      LogType =6;
      PLogText="The Main Power Supply Voltage Abnormal";//主电源电压异常
      
        LogValue =1;  
        g_LogValue &= NBIT7;
        
    }
     else if(((g_LogType & BIT8) == BIT8)&&((g_LogValue & BIT8) == BIT8))
    {
      LogType =6;
      PLogText="The Main Power Supply Voltage Abnormal";//主电源电压异常
      LogValue =0;
      g_LogValue &= NBIT8;
    }
    else if(((g_LogType & BIT9) == BIT9)&&((g_LogValue & BIT9) == BIT9))
    {
      LogType =7;
      PLogText="The Standby Power Supply Voltage";//备用电源电压异常 
      LogValue =1;  
      g_LogValue &= NBIT9;
    }
    else if(((g_LogType & BITA) == BITA)&&((g_LogValue & BITA) == BITA))
    {
      LogType =7;
      PLogText="The Standby Power Supply Voltage";//备用电源电压异常
      LogValue =0;
      g_LogValue &= NBITA;
    } 
     else if((g_LogType & BITB) == BITB)
    {
      LogType =8;
      PLogText="Control Circuit Disconnection";//控制回路断线异常
      LogValue =1;
      g_LogType &= NBITB; 
    }
     else if((g_LogType & BITC) == BITC)
    {
      LogType =9;
      PLogText="Abnormal Switch Position";//开关位置异常记录
      LogValue =1;
      g_LogType &= NBITC; 
    }
      else if((g_LogType & BITD) == BITD)
    {
      LogType = 8;//10;
      PLogText="Terminal Parameter Modification";//终端参数修改记录
      
      LogValue =1;
      g_LogType &= NBITD; 
    }
     else if((g_LogType & BITE) == BITE)
    {
      LogType = 10;//11;
      PLogText="Software Version Upgrade";//软件版本升级记录   
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
//  函数名称   : CalcuRmtMeas
//  功能描述   : 有效值计算，对采集到的数据进行处理，去掉最大值，最小值，取平均,并更新对应的遥测值，并判断遥测值的突变
//  输入参数   : <无>
//  输出参数   : <无>
//  返回值     : <无>
//  其他说明   : 
//  作者       :
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
    return;//对点模式下不处理实际数据，不判断电源低压，不做电源管理
  //if(!(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ISDDMODE]))
  {//aa在对点模式下不处理实际数据
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
//           if(i!=CHAN_BAT)//打开电池管教，等待两秒后有时前几次的值为0.在这里取最大值，
//                //若改变等待时间，则会延时较长时间进入实时模式
               lSumVal[i] = lSumVal[i]- iMinVal-iMaxVal;//去掉最大值，最小值 
//           else
//               lSumVal[i] =iMaxVal;
          
        }
        if(g_OpenBatFlag == TRUE)
        { 
            ff1=(float)lSumVal[CHAN_BAT]/4096/8*2.5*10*1000;
            g_gRmtMeas[RM_BAT_U]=(unsigned int) ff1;//电池电压计算值
        
            //ff1=(float)lSumVal[CHAN_BATF];
            //ff1/=16.187;
            //g_gRmtMeas[RM_BATF_U] =(unsigned int) ff1;//电池浮电压计算值
            g_OpenBatFlag = OFF;
            CLOSE_BAT_SAMPLE;
        }

        //电容电压计算值
        ff1=(float)lSumVal[CHAN_CAP]/4096/8*2.5*10*1000;
        //ff1/=16.03;
        g_gRmtMeas[RM_CAP_U]=(unsigned int) ff1;

        //太阳能电压计算值
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
        SetYcTrans();//在这里直接整理要上传的遥测，电源管理中关GPRS时要判断g_ucRefYxYcTs标识符，需要它=0
    }//aa
    
    Close_BatSunC_Para();

    
    //电源管理
   g_SendYc = 1;//每次电源管理前都设置为二遥，在电源管理中可能会变更为一遥
   if (g_gRunPara[Terminal_Type]==AERIAL_TERMINAL)
    {
      if(g_gZCZSQ == 0)   //003联调时增加
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
      g_AdChgSdYc = OFF;//第一次AD采集不启动立即上传
    }
    else
      g_AdChgSdYc = ON;      
}
//*****************************************************
//    Date    :  
//   Mender   :  
//   Function :  检测电源，看看打开那个发送等级
//****************************************************
void VoltLogicCtr(void)
{
    #define SENDYC_4HOUR (60*60*2)
    //#define SENDYC_4HOUR (60 * 5)
    OPEN_WIRE_POWER;
    static unsigned int Before4HourSdYcTm = 0;//保存准实时模式下切换为4小时之前的值
    g_SendYc = 1;
    switch(g_gRunPara[RP_POWER_MODE])
    {
    case QUASI_REALTIME:
        //当AD采集VBAT大于6.5V时，关断充电回路30分钟，然后继续充电。
        if(g_gRmtMeas[RM_BAT_U] > 14400)
        {
            CLOSE_SUN_CHARGE;
            OpenTimer(TM_CLOSECHARGE);
            //MyPrintf("Close sun charge\r\n");
        }
    
        if((g_gRmtMeas[RM_SUN_U] >= 11000) && (g_gRmtMeas[RM_BAT_U] >= g_gRunPara[RP_ALARM_BAT]))
        {//按用户设置的定时间隔上报全遥测，支持遥测突变上报(越限遥测和故障遥测)，支持遥信上报；
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
        {//4小时上报一次全遥测，支持遥测突变上报(越限遥测和故障遥测)，支持遥信上报；
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
        {//当电池电压>5.5v且<6v时，关闭遥测上报，只支持遥信上报；
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
            if(g_BatLoseFlag == 0)  //电池充足，未失压
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
                    OPEN_GPRS_POWER;  //电压回复，打开GPRS电源
                    
                    g_sTimer[TM_GPRSPWOERDOWN].m_TmCount = 0;                    
                }
                //g_EnSendStatusFlag = 0;
            }
            else    //电池不足，切换成一遥模式
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
                if(g_GprsPowerSt && (g_sTimer[TM_GPRSPWOERDOWN].m_TmCount == 0)) // 检查是否有关闭GPRS计时
                    OpenTimer(TM_GPRSPWOERDOWN,300); 
                //g_EnSendStatusFlag = ON;//设置一遥模式标志位,若打开这里，终端将变成一遥模式        
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
//  函数名称   : VoltLogicCtr_cable
//  功能描述   : 判断实时非实时状态切换
//  输入参数   : 
//  输出参数   : <无>
//  返回值     : 
//  其他说明   : 终端AD只要采到一次数值小于5v，则关GPRS，电源模式遥信位设置成非实时,2小时内不再判断模式切换。
//               终端AD采集10s数据，都大于6.5v，则打开GPRS，再继续检测40s，没有出现小于5v的，则电源模式遥信位设置成实时模式
//  作者       :
//==============================================================================

void VoltLogicCtr_cable(void)
{
    if((g_sTimer[TM_Volt].m_TmCount==0))
    {
      if(g_gRmtMeas[RM_SUN_U] < 5000 && g_gRmtMeas[RM_SUN_U] != 0)
      {//有一次小于5v则关GPRS，设置遥信位为准实时，启动闭锁计数(默认2小时内不再判断模式切换)
        g_gRunPara[RP_POWER_MODE]=QUASI_REALTIME;
        CreatNewSoe(RI_REAL_QUASI,1);
        if(g_GprsPowerSt)
        {
            //若没有通信任务才关GPRS，有通信任务且GPRS已经打开则打开电池，保证故障上传
            if(CheckHaveDataToSd() == FALSE)
              CloseGPRS(2);
            else
            {
              if((g_gRmtMeas[RM_CAP_U] < 5500)&&(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL)) // 电压小于6V，开启电池保护
              {
                OPEN_BAT_POWER; //  开启电池    
                 OpenTimer(TM_BATPWOERDOWN);//   统计电池使用时间
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
      else if((g_gRmtMeas[RM_SUN_U] > 6500)&&(!g_batPowerSt))//电池关闭时才判断是否切换到实时模式
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
    if(g_SunHighNum > 5 && (g_gRunPara[RP_POWER_MODE] == QUASI_REALTIME))//采集的电压连续5次大于6.5v,开GPRS
    {
        if(!g_GprsPowerSt)
          OpenGPRS(2);
        if( g_SunHighNum > 20)//采集的电压连续20次大于6.5v,设置为实时模式
        {
          g_SunHighNum = 0;
          g_sTimer[TM_GPRSPWOERDOWN].m_TmCount=0; 
          g_gRunPara[RP_POWER_MODE]= REALTIME;
          CreatNewSoe(RI_REAL_QUASI,0);
          CLOSE_BAT_POWER;//切换到实时模式说明CT电足，就该保证电池是关闭状态
        }
    } 
    if((g_SunHighNum > 5) && (g_SunHighNum <= 20))
      g_JdgPwMode = 1;
    else
      g_JdgPwMode = 0;
}
  //==============================================================================
  //  函数名称   : GetRIChgFlag
  //  功能描述   : 获取遥信信息中的单点状态
  //  输入参数   : RIIndex:遥信信息表中的全局索引号，具体见constant.h中RMT_INFO部分
  //  输出参数   ：<无>
  //  返回值     : 1或0
  //  其他说明   : 
  //  作者       ：
  //==============================================================================
//  unsigned char GetRIChgFlag( unsigned int RIIndex)
//  {
//        if( m_gRmtChgFlag[(RIIndex >> 4)] & BIT[(RIIndex & 0x0F)])
//            return 1;
//        else
//          return 0;
//  }
  //==============================================================================
   //  函数名称   : SetRIChgFlag
   //  功能描述   : 设置遥信信息中的单点状态，根据实际写入1或者0
   //  输入参数   : RIIndex:遥信信息表中的全局索引号，具体见constant.h中RMT_INFO部分
   //                         : Status: 写入状态，1或者0
   //  输出参数   : <无>
   //  返回值     : 
   //  其他说明   : 
   //  作者       ：
   //==============================================================================
   
//   void SetRIChgFlag( unsigned int RIIndex , unsigned int Status)
//   {
//         if( Status > 0 )
//         {
//             //新状态设为高，则进行或操作
//             g_gRmtChgFlag[(RIIndex >> 4)] |=  BIT[(RIIndex & 0x0F)];
//         }
//         else
//         {
//             //新状态为低，操作数取反，然后进行与操作
//             g_gRmtChgFlag[(RIIndex >> 4)] &= ~BIT[(RIIndex & 0x0F)];
//         }
//   }

//==============================================================================
//  函数名称   : GetRIStatus
//  功能描述   : 获取遥信信息中的单点状态或双点状态
//  输入参数   : RIIndex:遥信信息表中的全局索引号，具体见constant.h中RMT_INFO部分
//             : RmtInfoFlag 0 =g_gRmtInfo；1=g_unYxTrans；2=g_unDYxTrans
//  输出参数   ：<无>
//  返回值     : 1或0，2,3
//  其他说明   : 每个遥信信息占两个bit位
//  作者       ：
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
    iRtStVal = pRmtInfo[(RIIndex >> 3)] & iStVal;//取出要判断的位
    iRtStVal = iRtStVal >> ((RIIndex & 7)*2);//将要判断的位右移至最低位
    return iRtStVal;
}
//==============================================================================
//  函数名称   : SetRIStatus
//  功能描述   : 设置遥信信息中的单点状态，根据实际写入1或者0,2,3
//  输入参数   : RIIndex:遥信信息表中的全局索引号，具体见constant.h中RMT_INFO部分
//                         : Status: 写入状态，1或者0,2,3
//             : RmtInfoFlag 0 =g_gRmtInfo；1=g_unYxTrans；2=g_unDYxTrans
//  输出参数   : <无>
//  返回值     : 
//  其他说明   : 每个遥信信息占两个bit位
//  作者       ：
//==============================================================================

void SetRIStatus( unsigned int RIIndex , unsigned int Status, BYTE RmtInfoFlag)
{
   unsigned int iStVal = 0;
   unsigned int *pRmtInfo = null;
    if(RmtInfoFlag == 1)       pRmtInfo = g_unYxTrans;
    else if(RmtInfoFlag == 2)  pRmtInfo = g_unDYxTrans;
    else                       pRmtInfo = g_gRmtInfo;
   iStVal = 3 << ((RIIndex & 7)*2);
   pRmtInfo[(RIIndex >> 3)] &= ~iStVal;//清除要设置的位
   pRmtInfo[(RIIndex >> 3)] |=  ((Status & 3) << ((RIIndex & 7)*2));//将新状态写入要设置的位
   
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
		if(g_RF003_timeInfo.RF003_mini_num[i] == minute_count)//无线发的分钟计数和指示器的分钟计数一致
		{
			ms_count = (relative_time >>2)%1000;//得到MS的计数
			s_count = (relative_time >>2)/1000;//得到S的计数
			if((g_RF003_timeInfo.RF003_absolute_time[i][RTC_MICROSEC] + ms_count)>=1000)
			{
				s_count++;
				pabsolute_time[RTC_MICROSEC] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_MICROSEC] + ms_count -1000;
			}
			else
				pabsolute_time[RTC_MICROSEC] = g_RF003_timeInfo.RF003_absolute_time[i][RTC_MICROSEC] + ms_count;
			if((g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count)>=60)
			{
				m_count =(g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count)/60;//得到min的加数
				pabsolute_time[RTC_SEC] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count)%60;//得到min的加数
			}
			else
				pabsolute_time[RTC_SEC] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_SEC] + s_count);
			if(g_RF003_timeInfo.RF003_absolute_time[i][RTC_MINUT] + m_count >=60)
			{
				h_count = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_MINUT] + m_count)/60;
				pabsolute_time[RTC_MINUT] = (g_RF003_timeInfo.RF003_absolute_time[i][RTC_MINUT] + m_count)%60;//得到min的加数
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
			 goto CombYx;//return;//若是遥信新状态与老状态一样，则不再重新产生
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
			   //区分单点soe和双点soe
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
			  pchSoeBuf[SOE_TYPEL] = TsSoeType; //SOE类型(0-4095
			  pchSoeBuf[SOE_TYPEH] = TsSoeType >> 8;
			  pchSoeBuf[SOE_STVAL] = SoeValue;
			  pchSoeBuf[SOE_MSL] = (pAbsolutetime[RTC_MICROSEC]+pAbsolutetime[RTC_SEC]*1000)&0XFF;
			  pchSoeBuf[SOE_MSH] = (pAbsolutetime[RTC_MICROSEC]+pAbsolutetime[RTC_SEC]*1000)>>8;
			  pchSoeBuf[SOE_HOUR] = pAbsolutetime[RTC_HOUR];
			  pchSoeBuf[SOE_MINU] = pAbsolutetime[RTC_MINUT];
			  pchSoeBuf[SOE_DAY] =	pAbsolutetime[RTC_DATE];
			  pchSoeBuf[SOE_MONTH] = pAbsolutetime[RTC_MONTH];
			  pchSoeBuf[SOE_YEAR] = pAbsolutetime[RTC_YEAR]-2000;
			  //保存事件发生时刻的遥测值，为上报带遥测的事件记录
          	  PtrFlashStruct = (sSOE_RECORD_FLASH*)pchSoeBuf;
              PtrFlashStruct->YC_1_NO = 255;//先赋初值，不能让它为0，因为遥测序号0有含义
              PtrFlashStruct->YC_2_NO = 255;
             PtrFlashStruct->YC_3_NO = 255;
             PtrFlashStruct->YC_4_NO = 255;
          
           if(wFltTp != FTTPYE_ST  && wFltTp != 0xFFFF)//&& wFltTp != FTTPYE_XFLT
           {
              unsigned char Phase;
              unsigned int Line;
              unsigned int wYxYcNo;
              BYTE byRm1aFault;//判断当前soe是故障还是负荷
              
              Phase = (SoeType - RI_1A_SHORT) % PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType - RI_1A_SHORT) %(PHASE_NUM*LINE_NUM)) / PHASE_NUM; //从0开始
              wYxYcNo = Line * PHASE_NUM + Phase;
              if((wFltTp == FTTPYE_GL || wFltTp == FTTPYE_SGL || wFltTp == FTTPYE_YGL) && (SoeValue == 1))  //发生短路/瞬时/永久故障的时候
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
              if((g_gRunPara[Terminal_Type] == 0) && (Line <= 2))  //架空终端
              {
                  PtrFlashStruct->YC_3_NO = RN_1A_EFIELD_U + wYxYcNo;
                  PtrFlashStruct->YC_3_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  PtrFlashStruct->YC_3_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  //PtrFlashStruct->YC_4_NO = 255;    //架空暂无温度 
              }
              else   //电缆终端无电场电压
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
		//处理单双点soe尾指针
		  if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
		  {  
			g_unDCosBuffTail++;
			g_unDCosBuffTail &= (DSOE_BUFF_NUM - 1);
			
			//=========判断头指针与尾指针位置================
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
			
			  //=========判断头指针与尾指针位置================
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
		//根据分相遥信计算线路总遥信状态以及相故障,放到产生分相故障之后
		//if(Flag !=2)
		{
		  if(((int)SoeType >= RI_1A_SHORT && SoeType <= RI_3O_POWERCUT)
                     ||((SoeType >= RI_1A_BATPOW_EN) && (SoeType <= RI_3O_BATPOW_EN)))//RI_12O_RSVRI_3O_RSV
			SetAllYxStatus(SoeType); 
		}
		   
}

//Flag = 1 表示扫描硬遥信出来的数据 2 表示遥信对点产生的SOE(不做合成)
void CreatNewSoe(unsigned int SoeType,unsigned long SoeValue,unsigned char Flag)
{//SoeType:实遥信
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
           goto CombYx;//return;//若是遥信新状态与老状态一样，则不再重新产生
      }
      SetRIStatus(SoeType,SoeValue);
      
      
      TsSoeType = RealYxToTsYx(SoeType);
      if(TsSoeType == 0xffff)
          goto CombYx;//return;
      
      if(pGprs != null) SETBIT(pGprs->m_gRmtChgFlag,SoeType,1);
      if(pDbg != null)  SETBIT(pDbg->m_gRmtChgFlag,SoeType,1);
      //保存SOE之前就区分单点和双点，因为保存的soe类型采用遥信的信息体地址
      //这样在协议中传输soe就不用再分别处理信息体地址
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
          if(Flag == 1)  //扫描硬遥信出来的数据
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

           //区分单点soe和双点soe
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
          pchSoeBuf[SOE_TYPEL] = TsSoeType; //SOE类型(0-4095
          pchSoeBuf[SOE_TYPEH] = TsSoeType >> 8;
          pchSoeBuf[SOE_STVAL] = SoeValue;
          pchSoeBuf[SOE_MSL] = (pchSoeTm[RTC_MICROSEC]+pchSoeTm[RTC_SEC]*1000)&0XFF;
          pchSoeBuf[SOE_MSH] = (pchSoeTm[RTC_MICROSEC]+pchSoeTm[RTC_SEC]*1000)>>8;
          pchSoeBuf[SOE_HOUR] = pchSoeTm[RTC_HOUR];
          pchSoeBuf[SOE_MINU] = pchSoeTm[RTC_MINUT];
          pchSoeBuf[SOE_DAY] =  pchSoeTm[RTC_DATE];
          pchSoeBuf[SOE_MONTH] = pchSoeTm[RTC_MONTH];
          pchSoeBuf[SOE_YEAR] = pchSoeTm[RTC_YEAR]-2000;
		  //保存事件发生时刻的遥测值，为上报带遥测的事件记录
          PtrFlashStruct = (sSOE_RECORD_FLASH*)pchSoeBuf;
          PtrFlashStruct->YC_1_NO = 255;//先赋初值，不能让它为0，因为遥测序号0有含义
          PtrFlashStruct->YC_2_NO = 255;
          PtrFlashStruct->YC_3_NO = 255;
          PtrFlashStruct->YC_4_NO = 255;
          
          if(wFltTp != FTTPYE_ST  && wFltTp != 0xFFFF)//&& wFltTp != FTTPYE_XFLT
          {
              unsigned char Phase;
              unsigned int Line;
              unsigned int wYxYcNo;
              BYTE byRm1aFault;//判断当前soe是故障还是负荷
              
              Phase = (SoeType - RI_1A_SHORT) % PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType - RI_1A_SHORT) %(PHASE_NUM*LINE_NUM)) / PHASE_NUM; //从0开始
              wYxYcNo = Line * PHASE_NUM + Phase;
              if((wFltTp == FTTPYE_GL || wFltTp == FTTPYE_SGL || wFltTp == FTTPYE_YGL) && (SoeValue == 1))  //发生短路/瞬时/永久故障的时候
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
              if((g_gRunPara[Terminal_Type] == 0) && (Line <= 2))  //架空终端
              {
                  PtrFlashStruct->YC_3_NO = RN_1A_EFIELD_U + wYxYcNo;
                  PtrFlashStruct->YC_3_L  = LOBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  PtrFlashStruct->YC_3_H  = HIBYTE(g_gRmtMeas[PtrFlashStruct->YC_3_NO]);
                  //PtrFlashStruct->YC_4_NO = 255;    //架空暂无温度 
              }
              else   //电缆终端无电场电压
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
          PtrFlashStruct = (sSOE_RECORD_FLASH*)pchSoeBuf;      //zx 故障事件遥测 
          if(((SoeType <= RI_3O_SHORT) || ((SoeType>=RI_1A_RSV)&&(SoeType<=RI_3O_RSV))) && (SoeValue == 1))  //发生短路/瞬时/永久故障的时候
          { 
              Phase = SoeType%PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType/PHASE_NUM)%LINE_NUM); //从0开始
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
              PtrFlashStruct->YC_4_NO = 255;    //架空暂无温度              
          }
          else if(((SoeType >= RI_1A_GND) && (SoeType <= RI_3O_ELECON)) || (((SoeType <= RI_3O_SHORT) || ((SoeType>=RI_1A_RSV)&&(SoeType<=RI_3O_RSV))) && (SoeValue == 0)))
          {
              Phase = SoeType%PHASE_NUM;    //0-A,1-B,2-C,3-O
              Line  = ((SoeType/PHASE_NUM)%LINE_NUM); //从0开始
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
	    //处理单双点soe尾指针
      if(SoeType >= RI_ZD_DYX1 && SoeType <= RI_ZD_DYX12)
      {  
        g_unDCosBuffTail++;
        g_unDCosBuffTail &= (DSOE_BUFF_NUM - 1);
        
        //=========判断头指针与尾指针位置================
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
        
          //=========判断头指针与尾指针位置================
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
    //根据分相遥信计算线路总遥信状态以及相故障,放到产生分相故障之后
    if(Flag !=2)
    {
      if(((int)SoeType >= RI_1A_SHORT && SoeType <= RI_3O_POWERCUT)
         ||((SoeType >= RI_1A_BATPOW_EN) && (SoeType <= RI_3O_BATPOW_EN)))//RI_3O_RSVRI_12O_RSV
        SetAllYxStatus(SoeType); 
    }
       
}
//==============================================================================
//  函数名称   : OpenBatSmsGprs
//  功能描述   : 有故障产生时或有遥测需要上传，判断是否开SMS,GPRS和BAT
//  输入参数   : 无
//  返回值     : 无
//  其他说明   : 在GPRS通道对象中调用
//  作者       ：
//==============================================================================
void OpenBatSmsGprs()
{
    if(g_yxChangeflag & BITF)//g_yxChangeflag的最高位标识有报文需要上传
    {
      g_yxChangeflag &= ~BITF;

      if(g_gRunPara[RP_POWER_MODE]!= REALTIME)
       {//非实时模式
           //if(!g_GprsPowerSt)//去掉这句的原因是，有新故障时重新启动关闭GPRS倒计时(默认5m)
           {
               if(!g_sTimer[TM_SMSSENDALLOW].m_TmCount && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
               {//已经启动短信延迟发送计时则不能再启动
                   OpenTimer(TM_SMSSENDALLOW);
				   if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
				   {
                     BYTE bBuf[2] = {0xFA,0XFA};
                     MyPrintfD(bBuf,2);
				   }
               }
               if(!g_GprsPowerSt)//若产生新故障时GPRS已经打开则不再延时
               {
                  OpenTimer(TM_SENDFAULTDELAY); //子站主动发送延迟，给GPRS上电时间 
               }
               OpenGPRS(1);
               //OpenTimer(TM_SENDADDR);  
               OpenTimer(TM_GPRSPWOERDOWN);
               if((g_gRmtMeas[RM_CAP_U] < 5500)&&(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL)) // 电压小于6V，开启电池保护
                {
                  OPEN_BAT_POWER;//  开启电池    
                   OpenTimer(TM_BATPWOERDOWN);//   统计电池使用时间
                   if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
                   	{
                   		BYTE bBuf[2] = {0xBA,0XBA};
                     	MyPrintfD(bBuf,2);
                   	}
                }
           }
       }
       else//实时模式
       {
          OpenGPRS(3);
          g_SendFault = ON;
          if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))
            g_EnSmsSend = ON;
       }
    } 
}

//*******************************************************************
//*                     读取存储SOE的指针                      *
//* 功能：读取存储SOE的指针，为存储SOE提供存储位置指针
//* 参数：无
//********************************************************************/
void read_ptr(void)
{  
 char *Flash_ptr;    // *********2011-03-08 修改********************************************  
 char qq;  
 char wrt_over;
 unsigned char i,j;
 
 soe_ptr=0;
 soe_area=1;
 
 
 for(i=0;i<SOE_SEG;i++)      // 获取 段数
   {
    Flash_ptr = (char *) (SOE_RECORD_ADD + i * 512 + 511);  // 是否有数据写入    基地址0X27000
    qq=*Flash_ptr--;
    if(qq == 0x055)                                         ////已经写入，在判断这个段内是否已经写满
      {
       wrt_over = *Flash_ptr;
         
       if(wrt_over != 0x055)                                ////没有写满，             
         {
          soe_area = i + 1;
          break; 
         }
      }
    else if(qq != 0x55)    // 第1段没有写，要么是空，要么是最后一段已经写完导致第一段被擦除，这一段还没有被占用。
      {
       soe_area = i+1;
       break;
      }
   }

 for(i=0;i<SOE_NUM_SEG;i++)         // 获取 段内 指针
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
 for(i=0;i<SOE_SEG;i++)      // 获取soe 总数
   {
    Flash_ptr = (char *) (SOE_RECORD_ADD + i * 512 + 511);  // 是否有数据写入    基地址0X27000
    qq=*Flash_ptr--;
    if(qq == 0x055)                                         ////已经写入，在判断这个段内是否已经写满
      {
       wrt_over = *Flash_ptr;
       if(wrt_over == 0x055)        /////////已经写满
         {soe_num+=SOE_NUM_SEG;}
       else                         ////没有写满，             
         {
          for(j=0;j<SOE_NUM_SEG;j++)         // 开始判断段内写到了那里
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
    WDTCTL = WDTPW + WDTHOLD;          // 写高8位密码+停止看门狗
     
    Flash_ptr = (char *)(SOE_RECORD_ADD);      // 初始化指针
    FCTL3 = FWKEY;                     // FLASH解锁
    FCTL1 = FWKEY + ERASE;             // 使能擦除操作
    
    *Flash_ptr = 0;                    // 空写，启动擦除
    
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;              //写完后，锁定
    
    WDTCTL = WDT_ARST_1000;   //开启WDT
    _EINT();
    
    soe_ptr=0;
    soe_area=1;
   }
}


//save 单个SOE到flash
void  DiSoeSave(unsigned char *pSoeBuff)
{
 INT32U addr;             
// unsigned int j;

 _DINT();
 WDTCTL = WDTPW + WDTHOLD;          ////停止内部看门狗
 WatchDog();                        ////清外部看门狗
 
 if(soe_area ==0 || soe_area > SOE_SEG || soe_ptr >= SOE_NUM_SEG)   // 程序异常时，重新读取SOE存储指针 获取这两个值soe_ptr  ;soe_area;
   {read_ptr();}                                      //不要超过24个存储空间1-24  每个空间存储数量不超过42个0-41
 
 addr =(SOE_RECORD_ADD + (soe_area-1) * 512 + (soe_ptr*12) ); 
 WriteFlashBuff1(addr, &pSoeBuff[SOE_FLAG], 11); 

 
 if(soe_ptr == 0)                                          //每段倒数第二个字节为0x55，说明有数据写入；最后一个字节写为0x55，说明这一段已经占满了
   {
    addr = SOE_RECORD_ADD + (soe_area) * 512 - 1 ;         //说明有写入数据，但是不确定写满没有
    WriteFlashByte(addr,0x55);
    //写一个字节
   }
 else if(soe_ptr == SOE_NUM_SEG-1)
   {
    addr = SOE_RECORD_ADD + (soe_area) * 512 - 2;         //说明数据已经写满了
    WriteFlashByte(addr,0x55);
    //写一个字节
   }
 WatchDog();                             ////清外部看门狗
 soe_num++;
 soe_ptr++;
 if(soe_ptr >= SOE_NUM_SEG)                       //////////每段最多存储42个SOE记录 
   {
    soe_ptr=0;

    if(soe_num > SOE_NUM_SEG*SOE_SEG)                  /////检测是否超过了存储的最大数量，超过了，
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
 WatchDog();                             ////清外部看门狗
 //WDTCTL = WDT_ARST_1000;
 _EINT();
}
void SaveSoeData(void)
{
 // extern unsigned int g_unDSoeSavePtr;//待保存的双点SOE指针
  //    extern unsigned int g_unSSoeSavePtr;//待保存的单点SOE指针

    int SaveNum = 0;
    unsigned int BfT;
    //保存单点SOE
   BfT  = g_unSCosBuffTail;
    if(g_unSSoeSavePtr <= BfT) 
      SaveNum = BfT - g_unSSoeSavePtr;
    else
      SaveNum = SOE_BUFF_NUM - g_unSSoeSavePtr + BfT; 
    if(SaveNum == 0) 
      return;

    for(int i = 0; i < SaveNum;i++)
    {
      //将soe保存到flash
      DiSoeSave(g_gSCosBuff[g_unSSoeSavePtr]); 
      g_unSSoeSavePtr++;
      g_unSSoeSavePtr &= (SOE_BUFF_NUM-1);
    }
    
    //保存双点SOE
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
      //将soe保存到flash
      DiSoeSave(g_gDCosBuff[g_unDSoeSavePtr]); 
      g_unDSoeSavePtr++;
      g_unDSoeSavePtr &= (DSOE_BUFF_NUM-1);
    }
    #endif
  return;
}
//==============================================================================
//  函数名称   : ReadSoe
//  功能描述   : 读取一个SEG的SOE
//  输入参数   : pBuf,装读取到的SOE数据，
//                             iSegNo,扇区id，0到SOE_SEG-1
//                            iStartNo,起始序号 ,0到SOE_NUM_SEG-1
//                             iSoeNum,需要读取的SOE个数
//  输出参数   : <无>
//  返回值     : 实际读到的SOE个数
//  其他说明   : 
//  作者       :
//==============================================================================

int ReadSoe(unsigned char *pBuf,int iSegNo,int iStartNo,int iSoeNum)
{
  unsigned char *pAddr;
  int num_cnt = 0,RealNum=0;
  int j = 0;
    pAddr = (unsigned char *)(SOE_RECORD_ADD + (iSegNo*512) + 511);    //////检测是否有数据写入
    if(*pAddr == 0x55) // 已有数据写入，发送
    {
      pAddr = (unsigned char *)(SOE_RECORD_ADD + iSegNo*512 + iStartNo*(SOE_DATA_LEN+1)); 
      for(num_cnt=0;num_cnt<SOE_NUM_SEG;num_cnt++)
      {
        //检测第一个数据是否是有数据
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
          pAddr++;//每条SOE结尾空一个字节
        }
        else
          break;
       }
   }
  return RealNum;
}


//根据设备实遥信序号得到需要传输的遥信序号
//若返回为0xffff，表示此遥信不在上传点表中
unsigned int RealYxToTsYx(unsigned int RYxNo)
{
  unsigned int TsYxNo = 0xffff;
    for(int m = 0; m < g_ucYxTransNum; m++) //判断该遥信变位是否需要上传
    {
        if(RYxNo == (g_ucYXAddr[m] - 1))    //是需要上传的遥信
        {
            TsYxNo = m; //SOE类型(0-4095
            break;
        }
    }
   return TsYxNo;
}

//==============================================================================
//  函数名称   : 
//  功能描述   : 得到IEC101新的配置点表的遥信信息
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
//==============================================================================
void SetYxTrans(void)
{
  unsigned int i, RIIndex;
  int sYxCount = 0;
  int dYxCount = 0;

  if((g_ucRefYxYcTs & BIT0) == BIT0)//对遥信根据配置的遥信地址重新排列
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
//  函数名称   : 
//  功能描述   : 得到IEC101新的配置点表的遥测信息
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
//==============================================================================
void SetYcTrans(void)
{
    unsigned char i;
    if((g_ucRefYxYcTs & BIT1) == BIT1)
    {
      g_ucRefYxYcTs &= NBIT1;
      for(i = 0; i < g_ucYcTransNum; i++)   //计算需要上传的遥测值，按照具体情况进行换算
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
          g_sTimer[TM_SENDYC].m_TmFlag = ON;//启动遥测传输
        g_AdChgSdYc = 0;
      }
      else if(g_FltChgSdYc)
      {
        if(g_SendYc && (g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_FTYC]))
          g_sTimer[TM_SENDYC].m_TmFlag = ON;//启动遥测传输
        g_FltChgSdYc = 0;
      }
    }                                         
}



//==============================================================================
//  函数名称   : RstRMTMeas
//  功能描述   : 复位遥测值
//  输入参数   : 
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
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
//  函数名称   : RstRMTInfo
//  功能描述   : 复位遥信位
//  输入参数   : 
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
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
//  函数名称   : SaveLog
//  功能描述   : 把日志记录数据保存到flash中
//  输入参数   : <无>
//  输出参数   : <无>
//  返回值     : <无>
//  其他说明   : 记录整点负荷数据，保存格式为
//                   起始码(1)  记录字节数(1)   时间(5)     内容(X)
//                   0xEA         L             YYMMDDhhmm     xx   
//  作者       : 
//==============================================================================
//unsigned char ucLOADData[64][7]={0};//for test
//unsigned char min = 0;//for test
void SaveLog(unsigned int LogType,char *pLogText,unsigned char LogValue,INT8U Log_Length)
{
    unsigned char LogData[256];
    unsigned char LogDataPtr=0;
    unsigned long FLogAddr;
    unsigned int FLogInfo[FLOGINFONUM];      //从EEPROM中读取出来的FLASH中保存负荷记录的相关信息，总条数+即将存储记录的位置+最老一条记录的位置+校验
    LogData[LogDataPtr++] = 0xEA;     //起始码
    LogData[LogDataPtr++] = 9 + Log_Length; //5 + RM_LOADI_NUM*2; //单条负荷记录的长度为5字节时间加上负荷电流数据
	
	LogData[LogDataPtr++] = LogValue;
	LogData[LogDataPtr++] = LogType;
	LogData[LogDataPtr++] = LogType>>8;
    //时间 记录年、月、日、时、分
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_YEAR] - 2000;
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MONTH];
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_DATE];

    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_HOUR];
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
    LogData[LogDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_SEC];

    memcpy(&LogData[LogDataPtr],pLogText,Log_Length);//g_gRmtMeas[RM_1A_LOAD_I],2*RM_LOADI_NUM

    CAT_SpiReadWords(EEPADD_LOGNUM, FLOGINFONUM, FLogInfo);
    if((FLogInfo[FLOG_CS] != FLogInfo[FLOG_TOTALNUM] + FLogInfo[FLOG_NEW] + FLogInfo[FLOG_OLD]) || FLogInfo[FLOG_TOTALNUM] > FLASH_LOG_MAXNUM || FLogInfo[FLOG_NEW] > FLASH_LOG_MAXNUM)//如果FLASH地址不在负荷记录保存区域内
    {
        FLogInfo[FLOG_TOTALNUM] = 0; //清空负荷记录总条数
        FLogInfo[FLOG_NEW] = 0; //即将存储的记录与第一条记录的偏移位置(不是已存储的最新一条记录的位置)
        FLogInfo[FLOG_OLD] = 0;//最老一条记录的位置
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
    Sst26vf064b_Page_WriteBytes(FLogAddr, LogData, (11 + Log_Length));//把数据保存到FLASH中
    
//    for(int i = 0; i<64; i++)
//    {
//      FLoadAddr = i*FLASH_PLOAD_LEN;
//      Sst26vf064b_Read(FLoadAddr,ucLOADData[i], 7);
//    }
 
    
    if(FLogInfo[FLOG_TOTALNUM] < FLASH_LOG_MAXNUM)//如果没有存满，则FLASH中保存的负荷记录条数增加
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
    //在EEPROM中记录最新一条负荷记录的位置
    CAT_SpiWriteWords(EEPADD_LOGNUM, FLOGINFONUM, FLogInfo);
  
}

//==============================================================================
//  函数名称   : SaveLoad
//  功能描述   : 把负荷统计的数据保存到flash中
//  输入参数   : <无>
//  输出参数   : <无>
//  返回值     : <无>
//  其他说明   : 记录整点负荷数据，保存格式为
//                   起始码(1)  记录字节数(1)   时间(5)     内容(X)
//                   0xEA         L             YYMMDDhhmm     xx   
//  作者       : 
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
    unsigned int FLoadInfo[FLOADINFONUM];      //从EEPROM中读取出来的FLASH中保存负荷记录的相关信息，总条数+即将存储记录的位置+最老一条记录的位置+校验
    LoadData[LoadDataPtr++] = 0xEA;     //起始码
    LoadData[LoadDataPtr++] = 6 + RM_1A_LUBO*2; //RM_1A_CABLE_T5 + RM_LOADI_NUM*2; //单条负荷记录的长度为5字节时间加上负荷电流数据

    //时间 记录年、月、日、时、分
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_YEAR] - 2000;
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MONTH];
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_DATE];

    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_HOUR];

    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
    LoadData[LoadDataPtr++] = g_sRtcManager.m_gRealTimer[RTC_SEC];

    memcpy(&LoadData[LoadDataPtr],&g_gRmtMeas[RM_BAT_U],2*RM_1A_LUBO);//RM_1A_CABLE_Tg_gRmtMeas[RM_1A_LOAD_I],2*RM_LOADI_NUM
    CAT_SpiReadBytes(EEPADD_DAYNUM,31,DayLoad);
    CAT_SpiReadWords(EEPADD_LOADNUM, FLOADINFONUM, FLoadInfo);
	
    if((FLoadInfo[FLOAD_CS] != FLoadInfo[FLOAD_TOTALNUM] + FLoadInfo[FLOAD_NEW] + FLoadInfo[FLOAD_OLD]+FLoadInfo[FLOAD_DAY]) || FLoadInfo[FLOAD_TOTALNUM] > FLASH_LOAD_MAXNUM || FLoadInfo[FLOAD_NEW] > FLASH_LOAD_MAXNUM)//如果FLASH地址不在负荷记录保存区域内
    {
        FLoadInfo[FLOAD_TOTALNUM] = 0; //清空负荷记录总条数
        FLoadInfo[FLOAD_NEW] = 0; //即将存储的记录与第一条记录的偏移位置(不是已存储的最新一条记录的位置)
        FLoadInfo[FLOAD_OLD] = 0;//负荷记录存储的地址偏移量
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
			//由于RTC对时，可能会对当前的时间修改，导致存两个相同时间的负荷记录，在此判断如果和上次的负荷记录时间一样，则本次负荷不再保存
			if((LoadDa[2] == (g_sRtcManager.m_gRealTimer[RTC_YEAR] - 2000)) &&(LoadDa[3] == g_sRtcManager.m_gRealTimer[RTC_MONTH])&&(LoadDa[4] ==g_sRtcManager.m_gRealTimer[RTC_DATE] )&&(LoadDa[5]== g_sRtcManager.m_gRealTimer[RTC_HOUR])&&(LoadDa[6] == g_sRtcManager.m_gRealTimer[RTC_MINUT]))
				return;
			if(g_sRtcManager.m_gRealTimer[RTC_DATE] != LoadDa[4])//天数不一致保存的最近一条与本次保存的天数不一致
	 			LoadFlag = 0x55;
     }
	//Sst26vf064b_Page_WriteBytes(FLoadAddr, LoadData, LOAD_LEN);//把数据保存到FLASH中
	
	if((LoadFlag == 0x55) &&(FLoadInfo[FLOAD_TOTALNUM]>0))//新的文件，对应新一天的记录
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
			
			if(FLoadInfo[FLOAD_TOTALNUM] < FLASH_LOAD_MAXNUM)//如果没有存满，则FLASH中保存的负荷记录条数增加
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
	if(FLoadAddr % FLASH_DAYLOAD_LEN ==0)//(FLoadAddr % 4096 ==0)一天的记录条数
	{
        if(FLoadInfo[FLOAD_TOTALNUM] == FLASH_LOAD_MAXNUM)
        {
          FLoadInfo[FLOAD_TOTALNUM] -= 96;//(4096/FLASH_PLOAD_LEN);
          //FLoadInfo[FLOAD_OLD] += 96;//(4096/FLASH_PLOAD_LEN);
          //if(FLoadInfo[FLOAD_OLD] >= FLASH_LOAD_MAXNUM)
            //FLoadInfo[FLOAD_OLD] = 0;
        }
        Sector_Erase(FLoadAddr);//每次擦除12K;
		Sector_Erase(FLoadAddr +4096);
		Sector_Erase(FLoadAddr +(4096*2));
		
    }
    if(FLoadInfo[FLOAD_NEW] == 0)//FLASH_LOAD_MAXNUM
        FLoadAddr = FADDR_LOAD_START;
    Sst26vf064b_Page_WriteBytes(FLoadAddr, LoadData, LOAD_LEN);//把数据保存到FLASH中
    
//    for(int i = 0; i<64; i++)
//    {
//      FLoadAddr = i*FLASH_PLOAD_LEN;
//      Sst26vf064b_Read(FLoadAddr,ucLOADData[i], 7);
//    }
 
    
    if(FLoadInfo[FLOAD_TOTALNUM] < FLASH_LOAD_MAXNUM)//如果没有存满，则FLASH中保存的负荷记录条数增加
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
    //在EEPROM中记录最新一条负荷记录的位置
    CAT_SpiWriteWords(EEPADD_LOADNUM, FLOADINFONUM, FLoadInfo);
	CAT_SpiWriteBytes(EEPADD_DAYNUM+FLoadInfo[FLOAD_OLD], 1, &DayLoad[FLoadInfo[FLOAD_OLD]]);//FLoadInfo[FLOAD_DAY]
  
}
//==============================================================================
//  函数名称   : CopySoeToFlash
//  功能描述   : 内存中的SOE数据导入外部FLASH
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void CopySoeToFlash(void)
{
     int SaveNum = 0;
      unsigned int BfT;
      unsigned long FSoeAddrOfset;//最新SOE的地址偏移量
      //BYTE a[16];
      
      //保存单点SOE
      if(g_unSSoeSaveFlashPtr == g_unSCosBuffTail) return;

      BfT  = g_unSCosBuffTail;
      if(g_unSSoeSaveFlashPtr <= BfT) 
        SaveNum = BfT - g_unSSoeSaveFlashPtr;
      else                            
        SaveNum = SOE_BUFF_NUM - g_unSSoeSaveFlashPtr + BfT; 
        
      if(SaveNum == 0) return;

      
      //将soe保存到flash
      for(int i = 0; i < SaveNum;i++)
      {
        //判断要不要檫除
        g_unSSoeInFlashTail &= (FSOE_TOLNUM-1);//放在前面的原因是SOE到1024后，在后面变成0，与头指针相同，不发送SOE
        FSoeAddrOfset = g_unSSoeInFlashTail * FSOE_LEN;
        if( !(FSoeAddrOfset % 4096) )//每次檫除都要擦4k空间,256条SOE
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
        Sst26vf064b_Page_WriteBytes(FADDR_SOE_START + FSoeAddrOfset, g_gSCosBuff[g_unSSoeSaveFlashPtr],SOE_DATA_LEN); //把数据保存进入FLASH中
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
        
        //=========判断头指针与尾指针位置================
        //在檫除块时不移动头指针，读SOE记录时判断保存的数据即可
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
      //在E2中保存flash中的SOE尾指针
      CAT_SpiWriteWords(EEPADD_SSOETAIL, 1, &g_unSSoeInFlashTail);
    return;




}

float GetYcCoef(WORD wYcId)
{
    float wYcCoef = 1;
    //DWORD dwRpCoef=1;   //无系数，系数默认全为1
    if((wYcId <= RM_SUN_U)
      	|| (wYcId >= RN_1A_CAP_U && wYcId <= RN_3C_CAP_U)
       	|| (wYcId >= RN_1A_BAT_U && wYcId <= RN_3C_BAT_U)  //电池电压和电容电压
     	|| (wYcId >= RM_1A_LUBO && wYcId <= RM_3C_LUBO)) //电容电压和取电电压
    {
//        dwRpCoef = MAKEDWORD(g_gRunPara[RP_COEF_UL],g_gRunPara[RP_COEF_UH]);
        if(g_ucPara101[IECP_YC_TYPE] == 2)  //浮点数，电压单位转换为V
            wYcCoef = wYcCoef / 1000;
    }
    else if(wYcId >= RM_1A_FAULT_I && wYcId <= RM_3O_LOAD_I)//电流都放大10倍
    {
        if(g_ucPara101[IECP_YC_TYPE] == 2)  //浮点数，电压单位转换为V
            wYcCoef = wYcCoef / 10;         
    }
      
    //else if(wYcId >= RM_1A_FAULT_I && wYcId <= RM_12O_LOAD_I)
//    else if(wYcId >= RM_1A_FAULT_I && wYcId <= RM_3O_LOAD_I)
//    {
//        //dwRpCoef = MAKEDWORD(g_gRunPara[RP_COEF_IL],g_gRunPara[RP_COEF_IH]);
//        if((g_gRunPara[Terminal_Type] == AERIAL_TERMINAL) && (g_ucPara101[IECP_YC_TYPE] == 2))  //浮点数，电流单位转换为A
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


