/*------------------------------------------------------------------------
 Module:            PrtcRF.cpp
 Author:            linxueqin
 Project:           ��Լƽ̨
 State:             V1.0
 Creation Date:     2015-3-16
 Description:       ��RFָʾ��ͨ��--primary
------------------------------------------------------------------------*/



#ifndef _PRTC_RF

  #define _PRTC_RF
  #include "PrtcRF.h"
#include "..\Main.h"
//���캯��
/*extern char ComtrderCfg1[];*/

CPrtcRF::CPrtcRF() : CProtocol()
{

}
  void CPrtcRF::Run(void)
  {
     RcvData(&m_uartId);
     DoCommSendIdle();
     EarthFaultLocation();
  }
/***************************************************************
      Function��DoCommSendIdle
          �ӵع���ѡ�߶�λ����
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
  ***************************************************************/  
  void CPrtcRF::EarthFaultLocation(void)
  {
      
      switch(g_step_earth)
      {
          case 1: 
              Init_RECpara();//��ʼ����ز���
              RecZeroSequence(BKrelative_timel,&group11); //�ж���ָֻʾ���Ƿ���ͬ��
              break;
          case 2: 
              SaveLBZS(BKrelative_timel,group11,&Max_Len,&phase_flag,VI_flag1);//�����ͬ������������ϳɣ������ж��Ƿ��нӵ������Լ��������ѡ��
              break;
          case 3: 
              
                //chebyshev_filter(RECORDER_INDI-Max_Len,VI_flag1,group11,phase_flag);//�������������ѹ�����˲�
                if(VI_flag1 == 0x30)
                {
                  VI_flag1 = 0x31;
                  g_step_earth = 2;
                }
                else if(VI_flag1 == 0x31)
                {
                  if(phase_flag)
                  {
                    chebyshev_filter(RECORDER_INDI-Max_Len,0x30,group11,phase_flag);
                    chebyshev_filter(RECORDER_INDI-Max_Len,0x31,group11,phase_flag);
                  }
                  else // eif(phase_flag ==0)lse if(VI_flag1 == 0x31) //(&&��ʹû�нӵ����ϣ��ȵ�ѹ���겨
                  {
                    g_step_earth = 5;//ֻ�ж�·���ϡ�ң��¼�����ӵع��ϲ��ϴ�¼����־λ�������ж����ң��¼��������
                    cfg_lubo_start();//�����ж��Ƿ���ң�ش�����¼������Ҫ�������Ʋ��ǹ��ϵ�¼����������վ�ϴ����ϱ�־λ
          
                  }
               }
              break; 
          case 4: 
              differential_filter(RECORDER_INDI-Max_Len,phase_flag,group11);//�жϽӵع��ϵ�����
              g_step_earth =5;
              break; 
          case 5: 
              g_FRecorder_Current_COUNT ++;
              if(g_FRecorder_Current_COUNT >= FRECORDER_TOLNUM)//����FLASH���ͷ��ʼ�ٴ�
              {
                g_FRecorder_Current_COUNT=0;//Recorder.FRecorder_Current_COUNT-=1;
              }
           
            g_Recorder_Total_COUNT ++;                  
            if( g_Recorder_Total_COUNT > FRECORDER_TOLNUM)
              g_Recorder_Total_COUNT = FRECORDER_TOLNUM;
      /*if((Recorder.FRecorder_Current_COUNT > Recorder.Recorder_Total_COUNT) &&(Recorder.Recorder_Total_COUNT < FRECORDER_TOLNUM))
      {
        Recorder.Recorder_Total_COUNT = Recorder.FRecorder_Current_COUNT;
      }
      else if((Recorder.FRecorder_Current_COUNT < Recorder.Recorder_Total_COUNT) &&(Recorder.Recorder_Total_COUNT < FRECORDER_TOLNUM))
      {
        Recorder.FRecorder_Current_COUNT = Recorder.Recorder_Total_COUNT;
      }*/
            SetLbNum();
            CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &(g_Recorder_Total_COUNT));
      
            CAT_SpiWriteWords(EEPADD_LUBOFLAG, 1, &(g_FRecorder_Current_COUNT));
            Set_Lubo_flag();
          
            break; 
          default:
              break;
      }
      /*if(g_step_earth ==1)
      {
        VI_flag1 =0x30;
        RecZeroSequence(BKrelative_timel,group);     
      }
      else if(g_step_earth ==2)
        SaveLBZS(BKrelative_timel,group,MAX_Len,phase_flag,VI_flag1);//BKrelative_timel[0],BKrelative_timel[1],BKrelative_timel[2],group,0x30
      else if(g_step_earth == 3)
      {
       chebyshev_filter(RECORDER_INDI-MAX_Len,VI_flag1,group,phase_flag);
       if(VI_flag1 == 0x30)
       {
         VI_flag1 = 0x31;
         g_step_earth = 2;
       }
        
      }//������·�ж�originaBuf,
      else if(g_step_earth == 4)
      {
        differential_filter(RECORDER_INDI-MAX_Len,phase_flag,group);
        g_step_earth =5;
      }
      else if(g_step_earth == 5)
      {
        CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &(Recorder.FRecorder_Current_COUNT));//�ȶ�������ֹ�ϵ���0
        Recorder.FRecorder_Current_COUNT ++;
        if(Recorder.FRecorder_Current_COUNT >= FRECORDER_TOLNUM)//����FLASH���ͷ��ʼ�ٴ�
        {
          Recorder.FRecorder_Current_COUNT=0;//Recorder.FRecorder_Current_COUNT-=1;
        }
        CAT_SpiWriteWords(EEPADD_LUBOFLAG, 1, &(Recorder.FRecorder_Current_COUNT));
                    
        CAT_SpiReadWords(EEPADD_LUBONUM, 1, &(Recorder.Recorder_Total_COUNT));//�ȶ�������ֹ�ϵ���0
        if( Recorder.Recorder_Total_COUNT >0xFE)
            Recorder.Recorder_Total_COUNT =0;
         Recorder.Recorder_Total_COUNT ++;
                   
         if( Recorder.Recorder_Total_COUNT >= FRECORDER_TOLNUM)
             Recorder.Recorder_Total_COUNT = FRECORDER_TOLNUM;
          CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &(Recorder.Recorder_Total_COUNT));
         g_step_earth =0;
         
      }*/
      
       
  }
  /***************************************************************
      Function��Init
          ��Լ��ʼ��
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
  ***************************************************************/
  BOOL CPrtcRF::Init(WORD uartId)
  {
      BOOL rc;
      m_uartId = uartId;
      m_PaWaitflag = 0;
      m_TxNum = 0;
      m_PaWaitflag_rf003 =0;
      m_Waitflag_rf003 =0;
      m_wTxIPBit = 0;  
      m_TxNum_rf003 =0;
      mrecorder_long=0;
      m_reNum=0;
      fault_type =0;
      VI_flag1 =0;
      rc = CProtocol::Init(1);
      if (!rc)
      {
        return FALSE;
      }
      return 1;
  }

  /***************************************************************
      Function��DoReceive
          ���ձ��Ĵ���
      ��������
  
      ���أ�TRUE �ɹ���FALSE ʧ��
  ***************************************************************/
    BOOL CPrtcRF::DoReceive()
    {
       while(1)
       {
          if (SearchFrame() != TRUE)
          return FALSE;
    
          pReceiveFrame = (VIecRfFrame *)m_RecFrame.pBuf;
          if (pReceiveFrame->Frame55.Start1 == 0x55)
          {   
              //char byBuf1[2] = {0XE5,0XE5};
              //CommWrite(byBuf1,2);
              RecFrame55(&pReceiveFrame->Frame55); 
             if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF])//����ʹ��
             {//�����˽�RF�յ�������ת�������Կ�
                MyPrintfD(&(pReceiveFrame->Frame55.Start1),pReceiveFrame->Frame55.FrameLen);
             }
              continue;
          }
          if (pReceiveFrame->Frame55.Start1 == 0xC5)
          {
              m_PaWaitflag_rf003 = OFF;
              m_PaWaitCt_rf003 = 0;
              m_TxNum_rf003 = 0;
              if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF])
              {//�����˽�RF�յ�������ת�������Կ�
                MyPrintfD(&(pReceiveFrame->Frame55.Start1),2);
              }
              continue;
          }
          if (pReceiveFrame->Frame69.Start1 == 0x69)
          {
            RecFrame69();
            continue;
          }
          
       }
    }
  
  
  //ͨ�����Ϳ��д���
  void CPrtcRF::DoCommSendIdle(void)
  {
     char secName;
   
   //BYTE uartNum = 0;
     if(Recorder.Recorder_flag == ON)
     {
        Recorder.Recorder_flag = OFF;
        for(int i=0;i<6;i++)
        {            
              if(Recorder.Recorder_Allflag==ON)
              {
                if(Recorder.line_phase[i] == YES) //˳���ȡ������һ��iֵΪYESʱ����Ӧ
                 {
                    
                    //if(Recorder.Recorder_reflag[i] == ON)
                       //Recorder.line_phase[i]= YES;
                    //else
                    Recorder.Recorder_Allflag=OFF;
                    if(Phase_Detection(i)==0)
                    {
                      Recorder.line_phase[i]= NO;
                      Recorder.Recorder_reflag[i] = OFF;
                      //Recorder.Recorder_Allflag=OFF;
                      g_step_earth = 1;
      gRecorder_flagall = OFF;
                      g_detephase_flag = OFF;
                    }
        else
      g_detephase_flag = ON;//�����������ȥ���ʱ���ȱ�����ñ�־λ
                    //RecZeroSequence() ;
                    /*CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &(Recorder.FRecorder_Current_COUNT));//�ȶ�������ֹ�ϵ���0
                    Recorder.FRecorder_Current_COUNT ++;
                    if(Recorder.FRecorder_Current_COUNT >= FRECORDER_TOLNUM)//����FLASH���ͷ��ʼ�ٴ�
                    {
                      Recorder.FRecorder_Current_COUNT=0;//Recorder.FRecorder_Current_COUNT-=1;
                    }
                    CAT_SpiWriteWords(EEPADD_LUBOFLAG, 1, &(Recorder.FRecorder_Current_COUNT));
                    
                    CAT_SpiReadWords(EEPADD_LUBONUM, 1, &(Recorder.Recorder_Total_COUNT));//�ȶ�������ֹ�ϵ���0
                    if( Recorder.Recorder_Total_COUNT >0xFE)
                       Recorder.Recorder_Total_COUNT =0;
                    Recorder.Recorder_Total_COUNT ++;
                   
                    if( Recorder.Recorder_Total_COUNT >= FRECORDER_TOLNUM)
                       Recorder.Recorder_Total_COUNT = FRECORDER_TOLNUM;
                    CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &(Recorder.Recorder_Total_COUNT)); */
                    
                 }
              }
              
         }
         for(int i=0;i<6;i++)
         {   
            if((Recorder.line_phase[i]== YES) &&(g_step_earth == 0)&&(g_detephase_flag == OFF))//&&((Recorder.Recorder_reflag == ON))
              {
                 
                 if(Recorder.Recorder_flag1 == ON)//B��
                 {
                  secName=((i+1)<< 2)+2;
                  Recorder.Recorder_flag1 =OFF;
                 }
                 else if(Recorder.Recorder_flag2 == ON)//C��
                 {
                  secName=((i+1)<< 2)+3;
                  Recorder.Recorder_flag2 = OFF;
                 }
                 else
                 {
                   secName=((i+1)<< 2)+1;//A��
                    //CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &(g_FRecorder_Current_COUNT));
                   // if(Recorder.FRecorder_Current_COUNT >= 0xFE)  Recorder.FRecorder_Current_COUNT=0;
                    AddrBlock_Erase(g_FRecorder_Current_COUNT);
                    Recorder.Recorder_reflag[i] = OFF;
                 }
         
                 Recorder.Current_line_phase = secName;
                 g_Current_line = secName;
                 SendRCToRf003(secName,7);
                 
                 break;              
              }
          }
      }
    
    if((g_RWFiInf.Fi_rflag == ON) ||(g_RWFiInf.Fi_wflag == ON))
    {
        
    
      if(g_RWFiInf.FiphaseA_flag == ON)//B��
            {
                  secName=5;
                  g_RWFiInf.FiphaseA_flag =OFF;
             }
             else if(g_RWFiInf.FiphaseB_flag == ON)//C��
             {  
                  secName=6;
                  g_RWFiInf.FiphaseB_flag= OFF;
             }
             else if(g_RWFiInf.FiphaseC_flag == ON)//C��
             {
                   secName=7;
                   g_RWFiInf.FiphaseC_flag= OFF;
                    //CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &(g_FRecorder_Current_COUNT));
                   // if(Recorder.FRecorder_Current_COUNT >= 0xFE)  Recorder.FRecorder_Current_COUNT=0;
                    //AddrBlock_Erase(g_FRecorder_Current_COUNT);
                    //Recorder.Recorder_reflag[i] = OFF;
             }
       
       g_RWFiInf.Filinephase = secName;
       if(g_RWFiInf.Fi_rflag == ON)
       {
        SendRCmdToFI(secName,0x90);//this);(bySec,wFile,this);
        g_RWFiInf.Fi_rflag = OFF;
       }
       else if(g_RWFiInf.Fi_wflag == ON)
       {
        g_RWFiInf.Fi_wflag = OFF;
        SendWCmdToFI(secName,0x90);//this);(bySec,wFile,this);
       }
    }
      #define RESENDNUM_MAX 5
     if(m_PaWaitflag == TRUE && m_PaWaitCt == 0)
     {
          if(m_TxNum < RESENDNUM_MAX)
          {
            
            m_PaWaitCt = 2;
            m_PaWaitflag = ON;
            SendRfRetry();
            m_TxNum++;
            return;
          }
          else
          {
            m_PaWaitflag = OFF;
            m_TxNum = 0;
            m_PaWaitCt = 0;
      g_phase_loss[(Recorder.Current_line_phase & 0x03)-1] =ON;
            //if((Recorder.Current_line_phase & 0x03) == 3)//C�಻��ֱ���˳�����·
            SendRCToRf003(Recorder.Current_line_phase,8);//��ָʾ�����������ٻ�����Ӧʱ���Ͽ���ָʾ��
          }
     }

   if( g_RWFiInf.PaWaitflag_fi == TRUE && g_RWFiInf.PaWaitCt_fi ==0 )
   {
      if( g_RWFiInf.TxNum_fi < 1)
          {
            
            g_RWFiInf.PaWaitCt_fi = 10;//���һ���ط��ȴ�10S 20+(g_gRunPara[RP_CALL_TIME]*2);
            g_RWFiInf.PaWaitflag_fi  = ON;
            SendRfRetry();
            g_RWFiInf.TxNum_fi++;
            return;
          }
          else
          {
            g_RWFiInf.PaWaitflag_fi  = OFF;
            g_RWFiInf.PaWaitCt_fi = 0;
            g_RWFiInf.TxNum_fi = 0;
      if (g_RWFiInf.Filinephase == 5)
      {
        g_RWFiInf.Fi_rflag =ON;
        g_RWFiInf.FiphaseB_flag =ON;
      }
      else if (g_RWFiInf.Filinephase == 6)
      {
        g_RWFiInf.Fi_rflag =ON;
        g_RWFiInf.FiphaseC_flag =ON;
      }
      
            //if((Recorder.Current_line_phase & 0x03) == 3)//C�಻��ֱ���˳�����·
            
          }
   }

   if( g_RWFiInf.WPaWaitflag_fi == TRUE && g_RWFiInf.WPaWaitCt_fi ==0 )
   {
      /*if( g_RWFiInf.TxNum_fi < 1)
          {
            
            g_RWFiInf.PaWaitCt_fi = 20;
            g_RWFiInf.PaWaitflag_fi  = ON;
            SendRfRetry();
            g_RWFiInf.TxNum_fi++;
            return;
          }
          else*/
          {
            g_RWFiInf.WPaWaitflag_fi  = OFF;
            g_RWFiInf.WPaWaitCt_fi = 0;
            g_RWFiInf.WTxNum_fi = 0;
      if (g_RWFiInf.Filinephase == 5)
      {
        g_RWFiInf.Fi_wflag =ON;
        g_RWFiInf.FiphaseB_flag =ON;
      }
      else if (g_RWFiInf.Filinephase == 6)
      {
        g_RWFiInf.Fi_wflag =ON;
        g_RWFiInf.FiphaseC_flag =ON;
      }
      else if(g_RWFiInf.Filinephase == 7)
      {
        
     /* if(g_ZsqCmdRBit & BITA)
            {
                uartNum |= BIT0;
                g_ZsqCmdRBit &= (~BITA);         
            }
            if(g_ZsqCmdRBit & BITB)
            {
                uartNum |= BIT1;
                g_ZsqCmdRBit &= (~BITB);         
            }*/
    if(g_RWFiInf.Para_guiyue == OFF)
    {
        if(g_RWFiInf.Fi_Info_Num <=2)
        {
          /*if(uartNum & 0x01)
            
          ((CBJ101S *)pGprs)->RecWriteData(48,1,0);
          else if(uartNum & 0x02)
          ((CBJ101S *)pDbg)->RecWriteData(48,1,0);*///RecWriteData(48,g_RWFiInf.Fi_Info_Num);
          ((CBJ101S *)m_RWPaSrcObj)->RecWriteData(48,1,0);
        }
        else
        {
          /*if(uartNum & 0x01)  
          ((CBJ101S *)pGprs)->RecWriteData(136,1,0x80);
          else if(uartNum & 0x02)
          ((CBJ101S *)pDbg)->RecWriteData(136,1,0x80);*/
           ((CBJ101S *)m_RWPaSrcObj)->RecWriteData(136,1,0x80);    
        }
    }
    else if(g_RWFiInf.Para_guiyue == ON)
    {
      BYTE Reason =7;
      g_RWFiInf.Para_guiyue = OFF;
       /*if(uartNum & 0x01)
            ((CBJ101S *)pGprs)->SendCuringcode(Reason);
          else if(uartNum & 0x02)
            ((CBJ101S *)pDbg)->SendCuringcode(Reason);*/
             ((CBJ101S *)m_RWPaSrcObj)->SendCuringcode(Reason);
            
    }
          //RecWriteData(136,g_RWFiInf.Fi_Info_Num);
  
      }
      
            //if((Recorder.Current_line_phase & 0x03) == 3)//C�಻��ֱ���˳�����·
            
          }
   }

   
     if(m_Waitflag_rf003 == TRUE && m_WaitCt_rf003 == 0)
     {         
          {
            m_Waitflag_rf003 = OFF;
            //m_TxNum_rf003 = 0;
            m_WaitCt_rf003 = 0;
    if(g_detephase_flag == OFF)
          {
              if((Recorder.Current_line_phase & 0x03) == 1)
      
                  Recorder.Recorder_flag1 = ON;
      
              else if((Recorder.Current_line_phase & 0x03) == 2)
                Recorder.Recorder_flag2 = ON;
              else if((Recorder.Current_line_phase & 0x03) == 3)
                Recorder.Recorder_Allflag = ON;
    }
    else if(g_detephase_flag == ON)
    { 
                  Recorder.Recorder_Allflag = ON;
                  
                }
            //A���Ѵ������
            Recorder.Recorder_flag =ON;
            //if((Recorder.Current_line_phase & 0x03) == 3)//C�಻��ֱ���˳�����·
            //SendRCToRf003(Recorder.Current_line_phase,8);//��ָʾ�����������ٻ�����Ӧʱ���Ͽ���ָʾ��
          } 
     }

  
   
     if(m_PaWaitflag_rf003 == TRUE && m_PaWaitCt_rf003 == 0)
     {
          if(m_TxNum_rf003 < 3)
          {
            if(gRecorder_flagall == OFF)
              m_PaWaitCt_rf003 = 2+g_gRunPara[RP_CALL_TIME];
            else//¼��ʱ�����ָߵ͹��ģ�ָʾ�����ڸ߹���
               m_PaWaitCt_rf003 = 2;
            m_PaWaitflag_rf003 = ON;
            SendRfRetry();
            m_TxNum_rf003++;
            return;
          }
          else
          {
            m_PaWaitflag_rf003 = OFF;
            m_TxNum_rf003 = 0;
            m_PaWaitCt_rf003 = 0;
            /*if((Recorder.Current_line_phase & 0x03) == 1)
      
              Recorder.Recorder_flag1 = ON;
      
            else if((Recorder.Current_line_phase & 0x03) == 2)
            Recorder.Recorder_flag2 = ON;
          else if((Recorder.Current_line_phase & 0x03) == 3)
            Recorder.Recorder_Allflag = ON;
       //A���Ѵ������
        Recorder.Recorder_flag =ON;*/
            //if((Recorder.Current_line_phase & 0x03) == 3)//C�಻��ֱ���˳�����·
            //SendRCToRf003(Recorder.Current_line_phase,8);//��ָʾ�����������ٻ�����Ӧʱ���Ͽ���ָʾ��
          }
      
     }
     
      /*if(Recorder.Recorder_flag1 == ON)
      {
         Recorder.Recorder_flag1 == OFF;
           SendRf003Recorder(0);
        for(int i=0;i<6;i++)
        {            
              if(Recorder.line_phase[i]== YES )
              {
              
                 Recorder.line_phase[i]= NO;
                 Recorder.Recorder_flag1= ON;
                 
                 break;              
              }
         }
      }*/
     
     
  }
  void CPrtcRF::SendRfRetry(void)
  {
    CommWrite((char*)m_rfSendBuf,m_rfSendLen);
  }
  /***************************************************************
      Function��OnTimeOut
          ��ʱ����
      ������TimerID
          TimerID ��ʱ��ID
      ���أ���
  ***************************************************************/
  BOOL CPrtcRF::DoTimeOut(WORD wTimerID)
  {
    if(m_PaWaitflag == ON)
    {
      if(m_PaWaitCt > 0)
      {//ODUд�����ط���ʱ
          m_PaWaitCt--;
          if(m_PaWaitCt <= 0)
          {
            m_PaWaitflag = TRUE;
            return TRUE;
          }
       }
    }
    if(m_PaWaitflag_rf003 == ON)
    {
      if(m_PaWaitCt_rf003 > 0)
      {//ODUд�����ط���ʱ
          m_PaWaitCt_rf003--;
          if(m_PaWaitCt_rf003 <= 0)
          {
            m_PaWaitflag_rf003 = TRUE;
            return TRUE;
          }
       }
    }

    if(g_RWFiInf.PaWaitflag_fi == ON)
    {

    if(g_RWFiInf.PaWaitCt_fi > 0)
    {
      g_RWFiInf.PaWaitCt_fi--;
      if(g_RWFiInf.PaWaitCt_fi <= 0)
      {
        g_RWFiInf.PaWaitflag_fi = TRUE;
        return TRUE;
      }
        //g_RWFiInf.TxNum_fi = 0;
    }
    }
    if(g_RWFiInf.WPaWaitflag_fi== ON)
    {
      if(g_RWFiInf.WPaWaitCt_fi > 0)
    {
      g_RWFiInf.WPaWaitCt_fi--;
      if(g_RWFiInf.WPaWaitCt_fi <= 0)
      {
        g_RWFiInf.WPaWaitflag_fi = TRUE;
        return TRUE;
      }
        //g_RWFiInf.TxNum_fi = 0;
    }
    }
  
    if(m_Waitflag_rf003 == ON)
    {
      if(m_WaitCt_rf003 > 0)
      {//ODUд�����ط���ʱ
          m_WaitCt_rf003--;
          if(m_WaitCt_rf003 <= 0)
          {
            m_Waitflag_rf003 = TRUE;
            return TRUE;
          }
       }
    }
    return 1;
  }
  DWORD CPrtcRF::max(DWORD x,DWORD y,DWORD z)
  {    
        DWORD max;    
        if(x<y)    max=y;    
        else max=x;    
        if(max<z)    max=z; 
        
        return max;
 }
  FP32 CPrtcRF::max_s(FP32 x,FP32 y,FP32 z)
  {    
        FP32 max;    
        if(x<y)    max=y;    
        else max=x;    
        if(max<z)    max=z; 
        
        return max;
 }
 void CPrtcRF::Volatge_Coefficient(unsigned int LINE)
 {

    unsigned long FLbAddr = 0;
    int  origina_phase[80] = {0};
    INT32S originaBuf_phaseRms11;
    FP32   originaBuf_phaseRms;
    FP32   originaBuf_coeffi;
    INT32S  CurrentTempRe,CurrentTempIm; 
    DFT_DATA_EXT DFT_DATA_BUF1;
     memset(&DFT_DATA_BUF1, 0, sizeof(DFT_DATA_BUF1));   
    for(int j=0;j<3;j++)
    {
      FLbAddr =FADDR_RECORDER_STARTV+(long)LINE*(long)FLINEADDR+(long)j*(long)FPHASEADDR;
      Sst26vf064b_Read(FLbAddr,(unsigned char *)&origina_phase,160);
      for(unsigned char i=0;i<80;i++)
      {   
         DftCalc(origina_phase[i],&DFT_DATA_BUF1,&CurrentTempRe,&CurrentTempIm);
      }
           originaBuf_phaseRms11 = DataDeal(CurrentTempRe,CurrentTempIm);     
           originaBuf_phaseRms = sqrt((FP32)originaBuf_phaseRms11);
           originaBuf_coeffi = (FP32)g_gRunPara[RP_COEFFI_VOLTAGE]/originaBuf_phaseRms;
           CAT_SpiWriteBytes(EEPADD_COEFFICIENT+COEFFICIENT_LINE_LENG*LINE+j, 4, (unsigned char *)&(originaBuf_coeffi));//              
     }
    
 }
void CPrtcRF::Set_Lubo_flag()
{
   if(g_gRmtLockLB ==0)
   {
      if(m_EarthFault_flag != ON)
      {
         CreatNewSoe(RI_LUBO_FLAG,1);
         g_gRmtLockLB = g_gRunPara[RP_LUBO_RESETTIME];//60;//Ϊ�˷�ֹң�⸴��ʱ�����õĽϳ������¸�ң�Ų����ϴ����ؽ��˲�������Ϊ60Sg_gRunPara[RP_FLTLK_T];
      }
    }
    for(int i =0;i< 6;i++)
    {
      if(Recorder.line_phase[i] == YES)            
        Recorder.Recorder_flag = ON;       
    }
    m_EarthFault_flag = OFF;
    g_step_earth =0; 
    fault_type =0;
    g_RF003_timeInfo.LUBO_time_flag = OFF;
}
void CPrtcRF::cfg_lubo_start()
{
  BYTE type[3];
  for(int j =0;j<3;j++)//��CFG�ļ��м���ABC¼��������ԭ��0 û�յ�1 �糡����2 �½�3����ͻ��4ͬ��5ң��
  {
     long FADDR_RECORDER = FADDR_RECORDER_INFO +(long)group11*(long)FLINEADDR + (long)j*(long)FPHASEADDR;
     RECORDER_CFG recorder_cfg;       
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
     if((j==2) &&(g_gRunPara[RP_CFG_KEYH]& (BIT[RP_EARTH_FLAG-16]))) //ң�ش���
     {
              if(!((type[0] == 0x05)||(type[1] == 0x05)||(type[2] == 0x05)))//����¼������
                m_EarthFault_flag = ON;
     }
            
  }
}
 
 void CPrtcRF::Init_RECpara()
 {
     VI_flag1 =0x30;
              BKrelative_timel[0]=0;
              BKrelative_timel[1]=0;
              BKrelative_timel[2]=0;
              group11 = 0;
              Max_Len=0;
              //Phase_Len =0;
              phase_flag =0;
              g_fault_phase =0;
              g_differI_weizhi =0;
              g_differV_weizhi=0;
              g_faultI_Valse=0;
              g_faultV_Valse=0;
        m_EarthFault_flag = OFF;
 }
 BOOL CPrtcRF::RecZeroSequence(unsigned int *pBK_relative,unsigned char *Pgroup)
 {
    DWORD relative_time[3]= {0};
    DWORD relative_short_time[3] ={0};
    //DWORD relative_long_time[3] ={0};
    //unsigned int BKrelative_timel[3]={0};
    DWORD relative_time_Max=0;
    DWORD relative_time_Min =0;
    DWORD relative_time_Mid =0;
    DWORD rela_time;
    unsigned  int relative_timeBK = 0;
    unsigned  int rela_time_max =0;
     unsigned int DAT_AD;
    unsigned int  DAT_MAX;
    BYTE   relative_flag = 0;
  //unsigned  int rela_time_max =0;
    //DWORD relative_timeV_Max;
    //DWORD relative_timeV[3];
    struct RECORDER_CFG record_cfg;
    //unsigned char max_flag;
    //struct RECORDER_CFG record_cfgV;
    int i,j;
    unsigned char group;
    CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, (unsigned int *)&group);
    DAT_AD = DAT_AD_NUM;
    DAT_MAX = DAT_MAX_OFFSET;
    *Pgroup = group;
    //Ԭ������㷨
   /* for(i=0;i<3;i++)
    {
       
        long FADDR_RECORDER =FADDR_RECORDER_INFO+(long)group*(long)FLINEADDR+(long)i*(long)FPHASEADDR;
        Sst26vf064b_Read(FADDR_RECORDER,(unsigned char *)&record_cfg,sizeof(record_cfg));//��A������һ��
        //Sst26vf064b_Read(FADDR_RECORDER_VINFO+i*FLASH_PAGE_SIZE,(unsigned char *)&record_cfg,sizeof(record_cfg));
        if(record_cfg.lubo_flag==0x55)
        {
          relative_short_time[i] = record_cfg.relative_time1;
          relative_time[i] = record_cfg.relative_time1+record_cfg.relative_time2+record_cfg.relative_time3;
          if(i == 0)
          {
            relative_time_Min = record_cfg.relative_time1;
            relative_time_Max = record_cfg.relative_time1;
          }
          else
          {
            
              if(relative_time_Max < record_cfg.relative_time1)
                relative_time_Max = record_cfg.relative_time1;
              if(relative_time_Min > record_cfg.relative_time1)
                relative_time_Min = record_cfg.relative_time1;
              if((i==2) &&((relative_time_Max - relative_time_Min)< 960))
              {
                
                  for(int j=0;j<3;j++)
                  {
                    if(relative_flag !=  0x55)
                      relative_time[j] = relative_short_time[j];                     
                  }  
              }
            
          }
          
        }   //MAKEDWORD(record_cfg.relative_timel,record_cfg.relative_timeh);
        else
        {
          relative_time[i] = 0xffffffff;
          relative_flag = 0x55;
        }
    }*/
    for(i=0;i<3;i++)
    {
       
        long FADDR_RECORDER =FADDR_RECORDER_INFO+(long)group*(long)FLINEADDR+(long)i*(long)FPHASEADDR;
        Sst26vf064b_Read(FADDR_RECORDER,(unsigned char *)&record_cfg,sizeof(record_cfg));//��A������һ��
        //Sst26vf064b_Read(FADDR_RECORDER_VINFO+i*FLASH_PAGE_SIZE,(unsigned char *)&record_cfg,sizeof(record_cfg));
        if(record_cfg.lubo_flag==0x55)
        {
    relative_time_Mid = record_cfg.relative_time1 % 240000;
    relative_short_time[i] = relative_time_Mid;//record_cfg.relative_time1;
          //relative_time[i] = record_cfg.relative_time1+record_cfg.relative_time2+record_cfg.relative_time3;
          if(i == 0)
          {
            relative_time_Min = relative_time_Mid;//record_cfg.relative_time1 % 240000;
            relative_time_Max = relative_time_Mid;//record_cfg.relative_time1 % 240000;
          }
          else
          {
            
              if(relative_time_Max < relative_time_Mid)//record_cfg.relative_time1
                relative_time_Max = relative_time_Mid;//record_cfg.relative_time1 % 240000;
              if(relative_time_Min > relative_time_Mid)//record_cfg.relative_time1
                relative_time_Min = relative_time_Mid;//record_cfg.relative_time1 % 240000;
              /*if((i==2) &&((relative_time_Max - relative_time_Min)< 960)&&(relative_flag !=  0x55))
              {
                
                  for( j=0;j<3;j++)
                  {
                      relative_time[j] = relative_short_time[j];                     
                  }
               
              }*/
        if((i==2) &&((relative_time_Max - relative_time_Min)> 239000)&&(relative_flag !=  0x55))
        {

      for( j=0;j<3;j++)
                  {
                    if(relative_short_time[j] < 1000)
            
                      relative_time[j] = relative_short_time[j] + 240000;                     
                  }  
        }
              else
              {
                 for( j=0;j<3;j++)
                  {
                      relative_time[j] = relative_short_time[j];                     
                  }  
              }
            
          }
          
        }   //MAKEDWORD(record_cfg.relative_timel,record_cfg.relative_timeh);
        else
        {
          relative_time[i] = 0xffffffff;
          relative_flag = 0x55;
        }
    }
  if((relative_time[0] == 0xffffffff)&&(relative_time[1] == 0xffffffff)&&(relative_time[2] == 0xffffffff))
    {
      memset((unsigned char *)&g_recorder,0,sizeof(g_recorder));
                g_recorder.VI_flag =COMTRADE_ITYPE;
    SaveLBFlash(0,0,0,0);
    }
    relative_time_Max = max(relative_time[0],relative_time[1],relative_time[2]);
    for(i=0;i<3;i++)
    {
      rela_time = relative_time_Max-relative_time[i];
      if((relative_time[i] ==0xffffffff)||(rela_time > DAT_MAX))//160960
      {//||((relative_timeV[i] ==0xffffffff)))
           CAT_SpiWriteWords(EEPADD_ALEN+ group*EELEN_LENG, 1, &(relative_timeBK));//A��ƫ����
           CAT_SpiWriteWords(EEPADD_BLEN+ group*EELEN_LENG, 1, &(relative_timeBK));
           CAT_SpiWriteWords(EEPADD_CLEN+ group*EELEN_LENG, 1, &(relative_timeBK));
           CAT_SpiWriteWords(EEPADD_COMLEN+ group*EELEN_LENG, 1,&(relative_timeBK));//���ƫ����*/
           fault_type =0;
       if(g_gRunPara[RP_CFG_KEYH]& (BIT[RP_ERROR_FLAG-16])) //ȱ����ʱû�ж��ϵ��Ƿ������ϴ�¼����YX
      {     
        m_EarthFault_flag = ON;
      }
           g_step_earth =5;
           return FALSE; 
      }
          
    }
    
    {
      
       long FADDR_RECORDER = FADDR_RECORDER_INFO+(long)group*(long)FLINEADDR+(long)3*(long)FPHASEADDR;
       Sector_Erase(FADDR_RECORDER);
       
       //Sector_Erase(FADDR_RECORDER_VINFO0);
       memcpy(record_cfg.comtrade_time,g_sRtcManager.m_gRealTimer,RTC_NUM*2);
       //memcpy(record_cfgV.comtrade_time,g_sRtcManager.m_gRealTimer,RTC_NUM*2);
       
       Sst26vf064b_Page_WriteBytes(FADDR_RECORDER,(unsigned char *)&(record_cfg),sizeof(record_cfg));
       //Sst26vf064b_Page_WriteBytes(FADDR_RECORDER_VINFO0,(unsigned char *)&(record_cfg),sizeof(record_cfg));
       pBK_relative[0]=relative_time_Max-relative_time[0];;
       pBK_relative[1]=relative_time_Max-relative_time[1];
       pBK_relative[2]=relative_time_Max-relative_time[2];
       //rela_time_max = (unsigned  int)relative_time_Max;
       rela_time_max = max(pBK_relative[0],pBK_relative[1],pBK_relative[2]);
       CAT_SpiWriteWords(EEPADD_ALEN+ group*EELEN_LENG, 1, &(pBK_relative[0]));//A��ƫ����
       CAT_SpiWriteWords(EEPADD_BLEN+ group*EELEN_LENG, 1, &(pBK_relative[1]));//
       CAT_SpiWriteWords(EEPADD_CLEN+ group*EELEN_LENG, 1, &(pBK_relative[2]));//
       CAT_SpiWriteWords(EEPADD_COMLEN+ group*EELEN_LENG, 1,&(rela_time_max));//���ƫ����*/
       for(i=0;i<4;i++)
       {
        long FADDR_RECORDER =FADDR_RECORDER_INFO+(long)group*(long)FLINEADDR+(long)i*(long)FPHASEADDR;
                  Sst26vf064b_Read(FADDR_RECORDER,(unsigned char *)&record_cfg,sizeof(record_cfg));//��A������һ��
               if(g_gRunPara[RP_LUBO_CYCLE] ==0)
                {
                  #ifndef FILTER_COM
                record_cfg.DATA_Leng = (DAT_AD * 22) -(rela_time_max * 22);  
              #else
                record_cfg.DATA_Leng = (DAT_AD * 28) -(rela_time_max * 28); 
              #endif
                }
         else
         {
            #ifndef FILTER_COM
                record_cfg.DATA_Leng = ((g_gRunPara[RP_LUBO_CYCLE]*80) * 22);// -(rela_time_max * 24);  
              #else
                record_cfg.DATA_Leng = ((g_gRunPara[RP_LUBO_CYCLE]*80) * 28);// -(rela_time_max * 28); 
              #endif
         }
                  //record_cfg.DATA_Leng = (DAT_AD << 4) -(rela_time_max << 4);
                        
        record_cfg.TOTAL_Leng = record_cfg.CFG_Leng+record_cfg.DATA_Leng ;
        Sector_Erase(FADDR_RECORDER);
        Sst26vf064b_Page_WriteBytes(FADDR_RECORDER,(unsigned char *)&(record_cfg),sizeof(record_cfg));
      
      }
       g_step_earth =2;
       //SaveLBZS(BKrelative_timel[0],BKrelative_timel[1],BKrelative_timel[2],group,0x30);
       //SaveLBZS(BKrelative_timel[0],BKrelative_timel[1],BKrelative_timel[2],group,0x31);  
       return TRUE;  
    }
     //MAKEDWORD(low,high) ((DWORD)(((WORD)(low))|(((DWORD)((WORD)(high)))<< 16)))
 }
extern INT16U g_EarthAcuLock; //��̬�ӵ�ǰ�ĵ糡���ݼ�¼����
 void  CPrtcRF::SaveLBZS(unsigned int *pBK_relative,unsigned char group,unsigned int *pMAX_Len,INT8U *pphase_flag,unsigned char VI_flag)//,unsigned char LINE,
 {

    int valseA=0;
    int valseB=0;
    int valseC=0;
    int valse0=0;
    DWORD A_Len,B_Len,C_Len;
    //unsigned int MAX_Len=0;
    //unsigned int COM_Len=0;
    unsigned long FLbAddr = 0;
    unsigned long FLbAddrA = 0;
    unsigned long FLbAddrB = 0;
    unsigned long FLbAddrC = 0;
    unsigned long FLbAddr0 = 0;
    unsigned int FLbAddrA_Num = 0;
    unsigned int FLbAddrB_Num = 0;
    unsigned int FLbAddrC_Num = 0;
    unsigned int FLbAddr0_Num = 0;
    //int  originaBuf[RECORDER_INDI] = {0};//RECORDER_INDI
    int  originaBuf_phase[3][80] = {0};
    int  originaBuf_phase_fault[3][80] = {0};
    INT32S originaBuf_phaseRms11,originaBuf_phase_faultRms11;
    FP32   originaBuf_phaseRms[3];
    FP32   originaBuf_phase_faultRms[3];
    FP32   originaBuf_phase_diff[3];
    FP32   originaBuf_phase_diffmax;
    INT32S  CurrentTempRe,CurrentTempIm,UTempRe,UTempIm; 
    DFT_DATA_EXT DFT_DATA_BUF;
    DFT_DATA_EXT DFT_UDATA_BUF;
    INT8U fault_flag1 = 0;
    INT8U fault_flag2 = 0;
    unsigned char LINE;
    unsigned int MAX_Len =0;
    INT8U phase_flag =0;
    A_Len = pBK_relative[0];
    B_Len = pBK_relative[1];
    C_Len = pBK_relative[2];
    LINE = group;
    
    //INT8U phase_flag =0; 
    //FP32   originaBuf_phase_coeffici[3];
    //for(int j=0;j<3;j++)
   // CAT_SpiReadBytes(EEPADD_COEFFICIENT+COEFFICIENT_LINE_LENG*LINE+COEFFICIENT_PHASE_LENG*j, COEFFICIENT_PHASE_LENG, (unsigned char*)&originaBuf_phase_coeffici[j]);
    
    memset(&DFT_DATA_BUF, 0, sizeof(DFT_DATA_BUF));   
    memset(&DFT_UDATA_BUF, 0, sizeof(DFT_UDATA_BUF)); 
    //unsigned char ReadBack[228];
    if(VI_flag == 0x30)
      FLbAddr =FADDR_RECORDER_START+(long)LINE*(long)FLINEADDR;//+(long)3*(long)FPHASEADDR;
    else if(VI_flag == 0x31)
      FLbAddr =FADDR_RECORDER_STARTV+(long)LINE*(long)FLINEADDR;//+(long)3*(long)FPHASEADDR;;
    MAX_Len = max(A_Len,B_Len,C_Len);
    *pMAX_Len = MAX_Len;
    //COM_Len = RECORDER-MAX_Len;
    /*CAT_SpiWriteWords(EEPADD_ALEN, 1, &A_Len);//A��ƫ����
    CAT_SpiWriteWords(EEPADD_BLEN, 1, &B_Len);//
    CAT_SpiWriteWords(EEPADD_CLEN, 1, &C_Len);//
    CAT_SpiWriteWords(EEPADD_COMLEN, 1, &MAX_Len);//���ƫ����*/

    Sector_Erase(FLbAddr+FPHASEADDR*3);//FLbAddr+FPHASEADDR*3
    
     if((RECORDER_INDI-MAX_Len) >560)//��ʱ֮����������8���ܲ��ż���
     {
      for(INT16U i=0;i< RECORDER_INDI-MAX_Len;i++)//RECORDER-MAX_Len
      {
       
         if((A_Len*2 > FLASH_PAGE_SIZE) && (FLbAddrA_Num==0))//WRITEPECORDER
         {
             FLbAddrA_Num = (A_Len<<1)/FLASH_PAGE_SIZE;//WRITEPECORDER;
         }
         FLbAddrA = A_Len*2+i*2-FLbAddrA_Num*FLASH_PAGE_SIZE;//WRITEPECORDER;
         
         if(FLbAddrA >= FLASH_PAGE_SIZE)//WRITEPECORDER
         {
           FLbAddrA_Num++;
           Sst26vf064b_Read(FLbAddr+FLbAddrA_Num*FLASH_PAGE_SIZE,(unsigned char *)&valseA,2);//+FLbAddrA
         }
        else
           Sst26vf064b_Read(FLbAddr+FLbAddrA+FLbAddrA_Num*FLASH_PAGE_SIZE,(unsigned char *)&valseA,2);
        if(i< 80)   originaBuf_phase[0][i] = valseA;
        else if((i >= 480) &&(i < 560))originaBuf_phase_fault[0][i-480]=valseA;//ȡ��7�������й��Ϻ�糡�ļ���else if((i >= RECORDER_INDI-MAX_Len-80) &&(i < RECORDER_INDI-MAX_Len)) originaBuf_phase_fault[0][i-(RECORDER_INDI-MAX_Len-80)]=valseA;
        //if(i == 46)   valseA++;
        if((B_Len*2 > FLASH_PAGE_SIZE) && (FLbAddrB_Num==0))//WRITEPECORDER
        {
            FLbAddrB_Num = (B_Len<<1)/FLASH_PAGE_SIZE;//WRITEPECORDER;
        }
        FLbAddrB = B_Len*2+i*2-FLbAddrB_Num*FLASH_PAGE_SIZE;//WRITEPECORDER;
        if(FLbAddrB >= FLASH_PAGE_SIZE)//WRITEPECORDER
        {
          FLbAddrB_Num++;
          Sst26vf064b_Read(FLbAddr+FLbAddrB_Num*FLASH_PAGE_SIZE+FPHASEADDR,(unsigned char *)&valseB,2);//+FLbAddrB
        }
        else
           Sst26vf064b_Read(FLbAddr+FLbAddrB+FLbAddrB_Num*FLASH_PAGE_SIZE+FPHASEADDR,(unsigned char *)&valseB,2);
        if(i< 80)   originaBuf_phase[1][i] = valseB;
        else if((i >= 480) &&(i < 560))originaBuf_phase_fault[1][i-480] = valseB;//else if((i >= RECORDER_INDI-MAX_Len-80) &&(i < RECORDER_INDI-MAX_Len)) originaBuf_phase_fault[1][i-(RECORDER_INDI-MAX_Len-80)]=valseB;
        
        if((C_Len*2 > FLASH_PAGE_SIZE) && (FLbAddrC_Num==0))//WRITEPECORDER
        {
            FLbAddrC_Num = (C_Len<<1)/FLASH_PAGE_SIZE;//WRITEPECORDER;
        }
        FLbAddrC = C_Len*2+i*2-FLbAddrC_Num*FLASH_PAGE_SIZE;//WRITEPECORDER;
        if(FLbAddrC >= FLASH_PAGE_SIZE)//WRITEPECORDER
        {
          FLbAddrC_Num++;
          Sst26vf064b_Read(FLbAddr+FLbAddrC_Num*FLASH_PAGE_SIZE+FPHASEADDR*2,(unsigned char *)&valseC,2);//+FLbAddrC
        }
        else
           Sst26vf064b_Read(FLbAddr+FLbAddrC+FLbAddrC_Num*FLASH_PAGE_SIZE+FPHASEADDR*2,(unsigned char *)&valseC,2);
        if(i< 80)   originaBuf_phase[2][i] = valseC;
        else if((i >= 480) &&(i < 560))originaBuf_phase_fault[2][i-480] =valseC;//else if((i >= RECORDER_INDI-MAX_Len-80) &&(i < RECORDER_INDI-MAX_Len)) originaBuf_phase_fault[2][i-(RECORDER_INDI-MAX_Len-80)]=valseC;
        
        //valse0 = (int)(valseA*originaBuf_phase_coeffici[0]+valseB*originaBuf_phase_coeffici[1]+valseC*originaBuf_phase_coeffici[2]);
        valse0 = valseA+valseB+valseC;
        
        FLbAddr0 = i*2-FLbAddr0_Num*FLASH_PAGE_SIZE;//WRITEPECORDER
        if(FLbAddr0 >= FLASH_PAGE_SIZE)//WRITEPECORDER
        {
          FLbAddr0_Num++;
          Sst26vf064b_Page_WriteBytes(FLbAddr+FLbAddr0_Num*FLASH_PAGE_SIZE+FPHASEADDR*3,(unsigned char *)&valse0,2);
          //originaBuf[i] = valse0;//+FLbAddr0
          //Sst26vf064b_Read(FADDR_RECORDER_START+FLbAddr0+FLbAddr0_Num*FLASH_PAGE_SIZE+FPHASEADDR*3,(unsigned char *)&valse0,2);
          //Sst26vf064b_Read(FADDR_RECORDER_START+FLbAddr0+FLbAddr0_Num*FLASH_PAGE_SIZE+FPHASEADDR*3,(unsigned char *)&valseC,2);
        }
        else
        {
          Sst26vf064b_Page_WriteBytes(FLbAddr+FLbAddr0+FLbAddr0_Num*FLASH_PAGE_SIZE+FPHASEADDR*3,(unsigned char *)&valse0,2);
          //originaBuf[i] = valse0;
        }
        //Sst26vf064b_Read(FLbAddr+i*FLASH_PAGE_SIZE+FPHASEADDR*3,ReadBack,228);  
      
     
      //Sst26vf064b_Read(FADDR_RECORDER_START+(SecName-1)*FPHASEADDR+g_SendLuboNum*FLASH_PAGE_SIZE,ucTemp,228);//����ÿָֻʾ���ĵ�һ�����ݴ������Ϣ+FileName*FLINEADDR
      
    }
  }
    if(fault_type!= 0x08)//˲ʱ�����ò������ӵع����ж�
    {
      if(VI_flag == 0x31)
      {
          unsigned char i;
          INT16S originaBufU0;
          DFT_DATA_EXT DFT_U0_BUF;
          memset(&DFT_U0_BUF, 0, sizeof(DFT_U0_BUF));   
         //lzy ��̬�ӵع���ǰ�����ѹ�ļ��� 
         for(i=0;i<80;i++) 
         {
             originaBufU0 = originaBuf_phase[0][i] + originaBuf_phase[1][i] + originaBuf_phase[2][i];
             DftCalc(originaBufU0,&DFT_U0_BUF,&CurrentTempRe,&CurrentTempIm);
         }
         originaBuf_phaseRms11 = DataDeal(CurrentTempRe,CurrentTempIm);
         g_EarthAcuLock = (INT16S)sqrt((FP32)originaBuf_phaseRms11);   //�������ѹ����Чֵ�������������ڼ����ż���ֵ�仯
                   
        for(int j=0;j<3;j++)//�������ж�
        {
            for(i=0;i<80;i++)
            {   
              DftCalc(originaBuf_phase[j][i],&DFT_DATA_BUF,&CurrentTempRe,&CurrentTempIm);
              DftCalc(originaBuf_phase_fault[j][i],&DFT_UDATA_BUF,&UTempRe,&UTempIm);
            }
            originaBuf_phaseRms11 = DataDeal(CurrentTempRe,CurrentTempIm);
            originaBuf_phase_faultRms11 = DataDeal(UTempRe,UTempIm);
            
            originaBuf_phaseRms[j] = sqrt((FP32)originaBuf_phaseRms11);
            originaBuf_phase_faultRms[j]=sqrt((FP32)originaBuf_phase_faultRms11);
            originaBuf_phase_diff[j] =((FP32)originaBuf_phase_faultRms[j] - (FP32)originaBuf_phaseRms[j])/originaBuf_phaseRms[j];
            if((originaBuf_phase_diff[j] < 0-((FP32)g_gRunPara[RP_electric_Vari]/100))&&(originaBuf_phaseRms[j] >= (FP32)g_gRunPara[RP_VOLTAGE_RMS]))
            { 
              fault_flag1 ++;
              phase_flag = j+1;
            }
            else if ((originaBuf_phase_diff[j] > ((FP32)g_gRunPara[RP_electric_Vari]/100)) &&(originaBuf_phaseRms[j] >= (FP32)g_gRunPara[RP_VOLTAGE_RMS]))
              fault_flag2++;
            if(j ==2)
            {
              
              if((fault_flag2 ==1) &&(fault_flag1 ==2))
              {
                originaBuf_phase_diffmax = max_s(originaBuf_phase_diff[0],originaBuf_phase_diff[1],originaBuf_phase_diff[2]);
                if(originaBuf_phase_diffmax == originaBuf_phase_diff[0])  phase_flag =2;//B�����
                else if(originaBuf_phase_diffmax == originaBuf_phase_diff[1])  phase_flag =3;//C�����
                else if(originaBuf_phase_diffmax == originaBuf_phase_diff[2])  phase_flag =1;//A�����
              }
            }
            g_fault_phase = phase_flag;
          }
          if((fault_flag1 > 0) &&(fault_flag2 > 0))
          {
           
            *pphase_flag = phase_flag;
          }
          else
          {            
             //g_step_earth = 5;
             *pphase_flag = 0;
          }
          g_step_earth = 3;
         
      }
        
      else if(VI_flag == 0x30)
      {
        g_step_earth = 3;
        *pphase_flag = phase_flag;
      }
      //chebyshev_filter(RECORDER_INDI-MAX_Len,VI_flag,LINE,phase_flag);//������·�ж�originaBuf,
    } 
    else
    {
      if(VI_flag == 0x31)
      {
        fault_type =0;
        g_step_earth = 5;
      }
      else if(VI_flag == 0x30)
      {
        //fault_type =0;
        g_step_earth = 3;
      }  
    }
 }
 
 INT8U CPrtcRF::DftCalc(INT32S buf,DFT_DATA_EXT * DftDate,INT32S* TempRe,INT32S* TempIm)
 {
      INT32S temp1,temp2;
 
 
        temp1 = buf * CosTab[DftDate->Cnt];      
        temp2 = buf * SinTab[DftDate->Cnt];       
        DftDate->Real    += (temp1 >> 16);             
        DftDate->Imag    += (temp2 >> 16);
        DftDate->LowReal += (temp1 & 0xFFFF);
        DftDate->LowImag += (temp2 & 0xFFFF);
        DftDate->Cnt++;     
        if((DftDate->Cnt + 1)> 80)
        {
             *TempRe = ((DftDate->Real >> 4) + (DftDate->LowReal >> 20));  
             *TempIm = ((DftDate->Imag >> 4) + (DftDate->LowImag >> 20));
             DftDate->Real = 0;            
             DftDate->Imag = 0;            
        
             DftDate->LowReal = 0;
             DftDate->LowImag = 0; 
             DftDate->Cnt = 0;
             return TRUE;
             
        }
 
    return FALSE;
 }

 
 INT32S CPrtcRF::DataDeal(INT32S TempRe,INT32S TempIm)
 {
      INT32S  val;   
   
      val = (INT32S)((FP32)TempRe * TempRe + (FP32)TempIm * TempIm);
      if(val > 36695396)         
        val = 36695396;
   // val=(INT32S)sqrt((FP32)val);
      return val;
 }
  /***************************************************************
      Function��SearchOneFrame
          ����һ֡��ȷ�ı���
      ������Buf, Len
          Buf ���ջ�����ͷָ��
          Len ���ջ���������Ч���ݵĳ���
      ���أ�DWORD���ݣ�����
          ���֣�������
            #define FRAME_OK       0x00010000      //��⵽һ��������֡
            #define FRAME_ERR      0x00020000      //��⵽һ��У������֡
            #define FRAME_LESS     0x00030000      //��⵽һ����������֡����δ���룩
          ���֣��Ѿ���������ݳ���
  ***************************************************************/
  DWORD CPrtcRF::SearchOneFrame(BYTE *Buf, WORD Len)
  {
      unsigned short FrameLen;
      //if (Len < 5)
          //return FRAME_LESS;
  
      pReceiveFrame = (VIecRfFrame *)Buf;
      switch(pReceiveFrame->Frame55.Start1)
      {
          case 0x69:
            if (pReceiveFrame->Frame69.Length1 != pReceiveFrame->Frame69.Length2)
                return FRAME_ERR|1;
            if (pReceiveFrame->Frame69.Start2 != 0x69)
                return FRAME_ERR|1;
            FrameLen=pReceiveFrame->Frame69.Length1+6;
            if (FrameLen > Len)
                return FRAME_LESS;
            if (Buf[FrameLen-1] != 0x16)
                return FRAME_ERR|1;
            if (Buf[FrameLen-2] != (BYTE)ChkSum((BYTE *)&pReceiveFrame->Frame69.Control,pReceiveFrame->Frame69.Length1))
                return FRAME_ERR|1;
            return FRAME_OK|FrameLen;
            
          case 0x55:
            if(pReceiveFrame->Frame55.Start2 == 0xaa)
            {
              FrameLen = pReceiveFrame->Frame55.FrameLen;
              if(Len < FrameLen)
                return FRAME_LESS;
              if(Buf[FrameLen - 1]!=0x16)
                return FRAME_ERR|1;
              if (Buf[FrameLen - 2] != (BYTE)ChkSum((BYTE *)&pReceiveFrame->Frame55.FrameLen,FrameLen-4 )
                  && (Buf[FrameLen - 2] != (BYTE)ChkSum((BYTE *)&pReceiveFrame->Frame55.Start1,FrameLen-2))
                  )
                return FRAME_ERR|1;
              
              return FRAME_OK|FrameLen; 
            }
            else
              return FRAME_ERR|1;
          case 0xc5:
            if(pReceiveFrame->Frame55.Start2 == 0xc5)
            {
              FrameLen = 2;
              /*m_PaWaitflag_rf003 = OFF;
              m_PaWaitCt_rf003 = 0;
              m_TxNum_rf003 = 0;*/
              return FRAME_OK|FrameLen; 
            }
            else
              return FRAME_ERR|1;
          default:
              return FRAME_ERR|1;
      }
  }
  
  
  BOOL CPrtcRF:: SendZsqPara(void)
  {
      return true;
  }

  BOOL CPrtcRF:: SendWCmdToFI(unsigned char chLinePhase,unsigned int Val)
  {

      //unsigned char chGroup[3] = {0xff,0xff,0xff};
      BYTE group;
      BYTE phase;
      BYTE i ;
      BYTE groupPhase;
      BYTE bitflag =0;
    BYTE bitnum = 0;
    WORD wInfoAddr =0;
    WORD InfoAddr =0;
      phase = chLinePhase & 0x03;
      group = chLinePhase >> 2;
      group &= 0x3F;
      groupPhase = (group - 1) * 3 +  phase;
    InfoAddr = g_RWFiInf.Fi_Info;
      //if(pDaSrc != null )  m_RWPaSrcObj = pDaSrc;
      
      m_SendBuf.wWritePtr=0;
//      for(int i = 0; i < 5; i++)
//          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
//      pSendFrame = (VIecRfFrame *)&m_SendBuf.pBuf[m_SendBuf.wWritePtr];    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = groupPhase;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_gRunPara[RP_CALL_TIME];//0xFF;      
      
   
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x17;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;
      
    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x90;
   
  if(g_RWFiInf.Para_guiyue ==OFF)
  {
    for(  i =0; i<g_RWFiInf.Fi_Info_Num;i+=2 )
    {
        wInfoAddr = InfoAddr+(i>>1);
      if(wInfoAddr == 0x5008)
      {
      bitflag |=BIT0;
      bitnum ++;
      }
      if(wInfoAddr == 0x5009)
      {
      bitflag |=BIT1;
      bitnum ++;
      }
      if(wInfoAddr == 0x500B)
      {
      bitflag |=BIT2;
      bitnum ++;
      }
      if(wInfoAddr == 0x500D)//��λ��ʽ
      {
      bitflag |=BIT3;
      bitnum ++;
      }
      if(wInfoAddr == 0x500E)//�Զ���ʱ��λʱ��
      {
      bitflag |=BIT4;
      bitnum ++;
      }
      if(wInfoAddr == 0x500F)//�ϵ縴λʱ��
      {
      bitflag |=BIT5;
      bitnum ++;
      }
      if(wInfoAddr == 0x5011)//��ص͵�ѹ��ֵ
      {
      bitflag |=BIT6;
      bitnum ++;
      }
      if(wInfoAddr == 0x5014)//�������㼯��Ԫʱ����
      {
      bitflag |=BIT7;
      bitnum ++;
      }
     }
   }
else if(g_RWFiInf.Para_guiyue ==ON)
{
    for(  i =0; i<g_RWFiInf.Fi_Info_Num;i++ )
    {    
    if(g_wUpPaAddr[i] == 0x8020)
        {
                bitflag |=BIT0;
        bitnum ++;
        }
        else if(g_wUpPaAddr[i] == 0x8021)
        {
                bitflag |=BIT3;
        bitnum ++;
        }
        else if(g_wUpPaAddr[i] == 0x8022)
        {
                bitflag |=BIT2;
        bitnum ++;
        }
        else if(g_wUpPaAddr[i] == 0x8023)
        {
                bitflag |=BIT4;
        bitnum ++;
        }
        else if(g_wUpPaAddr[i] == 0x8024)
        {
                bitflag |=BIT5;
        bitnum ++;
        }
        else if(g_wUpPaAddr[i] == 0x8026)
        {
                bitflag |=BIT7;
        bitnum ++;
        }
        else if(g_wUpPaAddr[i] == 0x8028)
        {
                bitflag |=BIT6;
        bitnum ++;
        }
    }
   }
      //g_RWFiInf.Fi_InfoPara = wval;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = bitflag;//0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 16;//bitnum * 2;
      //for(int i = 0; i <= 16 ; i+=2)//bitnum * 2
      {
    //if(bitflag & BIT0)
    {
      WORD YC = (MAKEWORD(g_RWFiInf.Fi_InfoPara[14],g_RWFiInf.Fi_InfoPara[15]))/10;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = YC;//g_RWFiInf.Fi_InfoPara[14];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = YC >>8;//g_RWFiInf.Fi_InfoPara[15];
    }
    //if(bitflag & BIT1)
    {
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[16];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[17];
      
    }
    //if(bitflag & BIT2)
    {
      WORD v_flag = (MAKEWORD(g_RWFiInf.Fi_InfoPara[20],g_RWFiInf.Fi_InfoPara[21]))/10;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = v_flag;//g_RWFiInf.Fi_InfoPara[20];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = v_flag>>8;//g_RWFiInf.Fi_InfoPara[21];
    }
    //if(bitflag & BIT3)
    {
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[24];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[25];
    }
    //if(bitflag & BIT4)
    {
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[26];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[27];
    }
    //if(bitflag & BIT5)
    {
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[28];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[29];
    }
    //if(bitflag & BIT6)
    {
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[32];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_RWFiInf.Fi_InfoPara[33];
    }
    //if(bitflag & BIT7)
    {
      WORD Inter_time = (MAKEWORD(g_RWFiInf.Fi_InfoPara[38],g_RWFiInf.Fi_InfoPara[39]))/60;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = Inter_time;//g_RWFiInf.Fi_InfoPara[38];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = Inter_time>>8;//g_RWFiInf.Fi_InfoPara[39];
    }
      }
     /* if(pdata[0] > 22)
      {
          return false;
      }
      else
      {
          for(int i = 0; i < 22 - pdata[0]; i++)  //
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;  
      }*/
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
     
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;      
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      
    
      m_rfSendLen = m_SendBuf.wWritePtr;

    
    m_PaWaitflag_rf003 = ON;
    m_PaWaitCt_rf003 = 2+g_gRunPara[RP_CALL_TIME];
    m_TxNum_rf003 = 0;
    
      g_RWFiInf.WPaWaitflag_fi = ON;
      g_RWFiInf.WPaWaitCt_fi = g_gRunPara[RP_CALL_TIME]*3+10;//+g_gRunPara[RP_CALL_TIME];
      g_RWFiInf.WTxNum_fi = 1;//д�����ȴ�10S������һ��ı���
    return TRUE;
 
  }
  BOOL CPrtcRF:: SendRCmdToFI(unsigned char chLinePhase,unsigned int Val)//����·д����ʱ���õ��ú���,CProtocol *pDaSrc)
  {
      //unsigned char chGroup[3] = {0xff,0xff,0xff};
     // unsigned int temp;
      //BYTE  chDaSrcCm;
      BYTE group;
      BYTE phase;
      BYTE groupPhase;
      
      phase = chLinePhase & 0x03;
      group = chLinePhase >> 2;
      group &= 0x3F;
      groupPhase = (group - 1) * 3 +  phase;
      //chDaSrcCm = m_uartId;//pRf->m_uartId;
      
       //if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
//      if(chLinePhase != 0)
//      {
//          chGroup[0] = g_gRunPara[RP_COMM_ADDR] >> 8;
//          chGroup[1] = g_gRunPara[RP_COMM_ADDR] & 0xff;
//          chGroup[2] = 0;
//      }
     
      
      //m_SendBuf.wReadPtr = 0;
      m_SendBuf.wWritePtr=0;
//      for(int i = 0; i < 5; i++)
//          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
//      pSendFrame = (VIecRfFrame *)&m_SendBuf.pBuf[m_SendBuf.wWritePtr];    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = groupPhase;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_gRunPara[RP_CALL_TIME];//0xFF;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x18;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x00;
    
    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x90;
    
//      pSendFrame->Frame55.Type = 0xD2;
//      pSendFrame->Frame55.RFASDU.RfAsduSubTp.bySubType = 0x18;     //������
//      pSendFrame->Frame55.RFASDU.RfAsduSubTp.byInfoNum = 0x01;
//      m_SendBuf.wWritePtr += 6;
      //����5λΪ����ģ������������
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;    //����ʱ��
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x1E;    //����ʱ��
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x05;    //���о���
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x05;     //����Ƶ��
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0F;
      
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;    
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;      
  //    //����4λΪ�����ַ��ȫд0                            //��Э�飬����д�����ַ
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;    
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
      
//      //4�ֽ�IP = ���ַ��2�� + ��ƫ��ƫ��1�� + 0x00(1)
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chLinePhase;    //��ƫ ��ƫ
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[0];
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[1]; 
      
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[2];
  /*    int cmd = Val - 21;
      switch(cmd)
      {
      case R_OTHERPARA: 
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x05;
          break;
      case R_YCPARA:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0E;
          break;
      case R_BATPARA:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0F;
          break;
      case R_CURRENT:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x07;
          break;
      case CONNECT:
      case DISCONNECT:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0F;   //�˴�Э��δ��
          break;
      default:
          break;
      }*/
      for(int i = 0; i < 4; i++)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;

      
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
      
//      pSendFrame->Frame55.FrameLen = m_SendBuf.wWritePtr - TEMP + 2;//���ĳ��Ȱ���֡ͷ֡β��У��Ͳ�����֡ͷ֡β
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
    
    m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 2+g_gRunPara[RP_CALL_TIME];//10��1
      m_TxNum_rf003 = 0;

    
      g_RWFiInf.PaWaitflag_fi = ON;
      g_RWFiInf.PaWaitCt_fi = 20+(g_gRunPara[RP_CALL_TIME]*2);
      g_RWFiInf.TxNum_fi = 0;
      return TRUE;
  }

   BOOL CPrtcRF::SendRCmdToRf003(CProtocol *pDaSrc)
   {
     // BYTE  chDaSrcCm;
      if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
   /* chDaSrcCm = pDaSrc->m_uartId;
     if(chDaSrcCm == g_CmIdGPRS)
       {
           g_ZsqCmdRBit |= BIT[12];
       }
       else if(chDaSrcCm == g_CmIdDBG)
       {                                 
           g_ZsqCmdRBit |= BIT[13];    
       }*/  
      m_SendBuf.wWritePtr=0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x68;
     
      for(int i = 0; i < 3; i++)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;

      
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
      
//      pSendFrame->Frame55.FrameLen = m_SendBuf.wWritePtr - TEMP + 2;//���ĳ��Ȱ���֡ͷ֡β��У��Ͳ�����֡ͷ֡β
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
    
    m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 10;//10��1
      m_TxNum_rf003 = 0;
      return TRUE;
   }

  
  BOOL CPrtcRF:: SendRCmdToRf001(unsigned char chLinePhase,unsigned int Val,CProtocol *pDaSrc)
  {
      //unsigned char chGroup[3] = {0xff,0xff,0xff};
      //unsigned int temp;
      //BYTE  chDaSrcCm;
      BYTE group;
      BYTE phase;
      BYTE groupPhase;
      
      phase = chLinePhase & 0x03;
      group = chLinePhase >> 2;
      group &= 0x3F;
      groupPhase = (group - 1) * 3 +  phase;
      //chDaSrcCm = pDaSrc->m_uartId;
      
       if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
//      if(chLinePhase != 0)
//      {
//          chGroup[0] = g_gRunPara[RP_COMM_ADDR] >> 8;
//          chGroup[1] = g_gRunPara[RP_COMM_ADDR] & 0xff;
//          chGroup[2] = 0;
//      }
     /* if(chDaSrcCm == g_CmIdGPRS)
      {
          temp = Val - 26;
          if(Val == 30)//;��¼����ز������⴦����ΪBITλ����
            temp =8;
          g_ZsqCmdRBit |= BIT[temp];
      }
      else if(chDaSrcCm == g_CmIdDBG)
      {
          temp = Val - 21;
          g_ZsqCmdRBit |= BIT[temp];    
      }*/
          
      //m_SendBuf.wReadPtr = 0;
      m_SendBuf.wWritePtr=0;
//      for(int i = 0; i < 5; i++)
//          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
//      pSendFrame = (VIecRfFrame *)&m_SendBuf.pBuf[m_SendBuf.wWritePtr];    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = groupPhase;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_gRunPara[RP_CALL_TIME];//0xFF;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x18;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x00;
//      pSendFrame->Frame55.Type = 0xD2;
//      pSendFrame->Frame55.RFASDU.RfAsduSubTp.bySubType = 0x18;     //������
//      pSendFrame->Frame55.RFASDU.RfAsduSubTp.byInfoNum = 0x01;
//      m_SendBuf.wWritePtr += 6;
      //����5λΪ����ģ������������
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;    //����ʱ��
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x1E;    //����ʱ��
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x05;    //���о���
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x05;     //����Ƶ��
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0F;
      
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;    
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;      
  //    //����4λΪ�����ַ��ȫд0                            //��Э�飬����д�����ַ
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;    
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
      
//      //4�ֽ�IP = ���ַ��2�� + ��ƫ��ƫ��1�� + 0x00(1)
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chLinePhase;    //��ƫ ��ƫ
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[0];
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[1]; 
      
//      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
  //    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[2];
      int cmd = Val - 21;
      switch(cmd)
      {
      case R_OTHERPARA: 
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x05;
          break;
      case R_YCPARA:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0E;
          break;
      case R_BATPARA:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0F;
          break;
    case R_LUBOPARA:
         m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x90;
      case R_CURRENT:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x07;
          break;
      case CONNECT:
      case DISCONNECT:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0F;   //�˴�Э��δ��
          break;
      default:
          break;
      }
      for(int i = 0; i < 4; i++)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;

      
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
      
//      pSendFrame->Frame55.FrameLen = m_SendBuf.wWritePtr - TEMP + 2;//���ĳ��Ȱ���֡ͷ֡β��У��Ͳ�����֡ͷ֡β
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
      m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 2+g_gRunPara[RP_CALL_TIME];
      m_TxNum_rf003 = 0;
      return TRUE;
  }
  BOOL CPrtcRF::SendWCmdToRf003(unsigned int cmd,unsigned char* pdata,CProtocol *pDaSrc)
  {
      BYTE  chDaSrcCm;
    chDaSrcCm = pDaSrc->m_uartId;
      if(chDaSrcCm == g_CmIdGPRS)
       {
           g_ZsqCmdRBit |= BIT[12];
       }
       else if(chDaSrcCm == g_CmIdDBG)
       {                                 
           g_ZsqCmdRBit |= BIT[13];    
       }  
      m_SendBuf.wWritePtr=0;
         m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;   
    BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x67;
      for(int i = 1; i <= pdata[0]; i++)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pdata[i];
    for(int i = 0; i < 14 - pdata[0]; i++)  //
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00; 
     m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
     
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;      
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
      m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 12;
      m_TxNum_rf003 = 0;
    return TRUE;
  }
  BOOL CPrtcRF:: SendWCmdToRf001(unsigned char chLinePhase,unsigned int cmd,unsigned char* pdata,CProtocol *pDaSrc)
  {//pData[0]��ʾ�ڳ���
      //unsigned char chGroup[3] = {0xff,0xff,0xff};
      BYTE group;
      BYTE phase;
      BYTE groupPhase;
     // BYTE  chDaSrcCm;
    //unsigned int temp;
   m_wTxIPBit = 0;//����0
     if(chLinePhase < 0xFF)
     {
      phase = chLinePhase & 0x03;
      group = chLinePhase >> 2;
      group &= 0x3F;
      groupPhase = (group - 1) * 3 +  phase;
    if(phase >0)
      m_wTxIPBit |= BIT[phase-1];
     }
     else
     {
       groupPhase = 0xFF;
     m_wTxIPBit |= 0x07;
     }
      if(pDaSrc != null )  m_RWPaSrcObj = pDaSrc;
      //chDaSrcCm = pDaSrc->m_uartId;
      /*if(chDaSrcCm == g_CmIdGPRS)
      {
          temp = cmd - 26;
          if(cmd == 30)
            temp = 8;
          g_ZsqCmdRBit |= BIT[temp];
      }
      else if(chDaSrcCm == g_CmIdDBG)
      {
          temp = cmd - 21;
          g_ZsqCmdRBit |= BIT[temp];    
      }*/
      m_SendBuf.wWritePtr=0;
//      for(int i = 0; i < 5; i++)
//          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
//      pSendFrame = (VIecRfFrame *)&m_SendBuf.pBuf[m_SendBuf.wWritePtr];    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = groupPhase;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_gRunPara[RP_CALL_TIME];//0xFF;      
      
   
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x17;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;
      
      cmd = cmd - 11;
      switch(cmd)
      {        
      case REVERT:
          //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0A;
          //SetRIStatus((group - 1) * 4  +  (phase-1) + RI_1A_STATUS,0); 
         //// CreatNewSoe((group - 1) * 4  +  (phase-1) + RI_1A_STATUS,0);//����״̬��0
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x08;  //��ָʾ��
          *pdata = 0;
          break;
      case VERT:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x09;
          *pdata = 0;
          break;
      case W_OTHERPARA:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x05;
          break;
      case W_YCPARA:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0E;
          break;
      case W_BATPARA:
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0F;
          break;
      case EARTHFAULT:
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0A;
          break;
    case W_LUBOPARA:
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x90;
          break;
      default:
         break;
      }
    if(cmd == W_LUBOPARA)
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xFF;
    else
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pdata[0];
      for(int i = 1; i <= pdata[0]; i++)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pdata[i];
      if(pdata[0] > 22)
      {
          return false;
      }
      else
      {
          for(int i = 0; i < 22 - pdata[0]; i++)  //
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;  
      }
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
     
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;      
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
      m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 2+g_gRunPara[RP_CALL_TIME];
      m_TxNum_rf003 = 0;
    return TRUE;
  }
  BOOL CPrtcRF:: AddrBlock_Erase(unsigned char chLinePhase)//������¼���ĵ�ַ������д��ô�¼��������
  {
     unsigned long address = 0;
     unsigned long addressV = 0;
     unsigned long addressF = 0;
     unsigned long addressLI = 0;
     unsigned long addressLV = 0;
     
    unsigned char LbBuff[256] = {0};
    unsigned char i;
    address = FADDR_RECORDER_START+(unsigned long )chLinePhase*FLINEADDR;
    addressV = FADDR_RECORDER_STARTV+(unsigned long )chLinePhase*FLINEADDR; 
    addressF = FADDR_RECORDER_INFO+(unsigned long )chLinePhase*FLINEADDR;   
    addressLI =  FADDR_WAVE_FILTERI+(unsigned long )chLinePhase*FPHASEADDR; //�����˲���ʼλ��
    addressLV = FADDR_WAVE_FILTERV+(unsigned long )chLinePhase*FPHASEADDR;
    Sector_Erase(addressLI); //Block_Erase
    Sector_Erase(addressLV);
    for(BYTE j=0;j<4;j++)
    {
      Sector_Erase(address+(unsigned long)j*FPHASEADDR); //Block_Erase
      Sector_Erase(addressV+(unsigned long)j*FPHASEADDR);//Block_Erase
      Sector_Erase(addressF+(unsigned long)j*FPHASEADDR);//Block_Erase
    }
    //Sector_Erase(address); //Block_Erase
    //Sector_Erase(addressV);//Block_Erase
    for( i=0;i<16;i++)
    {
      
      Sst26vf064b_Page_WriteBytes(addressLI,LbBuff,FLASH_PAGE_SIZE);
      Sst26vf064b_Page_WriteBytes(addressLV,LbBuff,FLASH_PAGE_SIZE);
      //Sst26vf064b_Page_WriteBytes(addressF,LbBuff,FLASH_PAGE_SIZE);
      addressLI+=FLASH_PAGE_SIZE;
      addressLV+=FLASH_PAGE_SIZE;
      //addressF+=FLASH_PAGE_SIZE;    
    }
    for( i=0;i<64;i++)
    {
      
      Sst26vf064b_Page_WriteBytes(address,LbBuff,FLASH_PAGE_SIZE);
      Sst26vf064b_Page_WriteBytes(addressV,LbBuff,FLASH_PAGE_SIZE);
      Sst26vf064b_Page_WriteBytes(addressF,LbBuff,FLASH_PAGE_SIZE);
      address+=FLASH_PAGE_SIZE;
      addressV+=FLASH_PAGE_SIZE;
      addressF+=FLASH_PAGE_SIZE;
      
    }
    return TRUE;
  }
  BOOL CPrtcRF::SendYKLBToRf003(BYTE flag)//ң��¼��
  {
      //if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
      m_SendBuf.wWritePtr=0;
 
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0A;
      if(flag == 0x66)
      {
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x65;//0x64;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;
          for(int i = 0; i < 3; i++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;          
      }
      else if(flag == 0x67)
      {
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x65;//0x64;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x02;
          for(int i = 0; i < 3; i++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;            
      }
      else
      {
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = flag;//0x64;
          for(int i = 0; i < 4; i++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      }
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
      

      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
      
      m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 2+g_gRunPara[RP_CALL_TIME];
      m_TxNum_rf003 = 0;
      return TRUE;
  }
  BOOL CPrtcRF:: SendRCToRf003(unsigned char chLinePhase,unsigned int Val,CProtocol *pDaSrc)
  {
    if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
      m_SendBuf.wWritePtr=0;
 
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xD2;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = Val;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;
      for(int i = 0; i < 9; i++)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = chLinePhase;
      for(int i = 0; i < 4; i++)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
      

      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
      
      m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 2;//+g_gRunPara[RP_CALL_TIME];
      m_TxNum_rf003 = 0;
       
      if(Val == 7)
      {
        m_WaitCt_rf003 = 35;
        m_Waitflag_rf003 = ON;
    gRecorder_flagall = ON;
      }
      else if(Val == 8)
      {
        m_Waitflag_rf003 = ON;
        m_WaitCt_rf003 = 10;
    gRecorder_flagall = TRUE;//OFF;
      }
      return TRUE;
     
    
  }
  BOOL CPrtcRF:: SendYkToRf003(unsigned char chLinePhase,unsigned int Val,CProtocol *pDaSrc)
  {
    unsigned char chGroup[3] = {0xff,0xff,0xff};

    if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
      if(chLinePhase != 0)
      {
        chGroup[0] = 0x01;//g_gRunPara[RP_COMM_ADDR] >> 8;
        chGroup[1] = 0xff;//g_gRunPara[RP_COMM_ADDR] & 0xff;
        chGroup[2] = 0x01;//0;
      }
      m_SendBuf.wReadPtr = 0;
      m_SendBuf.wWritePtr=0;
      pSendFrame = (VIecRfFrame *)m_SendBuf.pBuf;
      pSendFrame->Frameff.Start0  = 0xff;
      pSendFrame->Frameff.Start1  = 0x55;
      pSendFrame->Frameff.Start2 = 0xAA;
      pSendFrame->Frameff.FrameLen = 0;
      pSendFrame->Frameff.Type = 0xD2;
      if(Val == 1)//�϶�Ӧ����,�����֣�0x1F=���� 0x20=����
        pSendFrame->Frameff.RFASDU.RfAsduSubTp.bySubType = 0x1F;     
      else
        pSendFrame->Frameff.RFASDU.RfAsduSubTp.bySubType = 0x20;
      pSendFrame->Frameff.RFASDU.RfAsduSubTp.byInfoNum = 0x01;
      m_SendBuf.wWritePtr += 7;
      //����5λΪ����ģ������������
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;//0x05;    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;//0x30;    //����ʱ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;//0x05;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;//0x07;    //����Ƶ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0;//0x0C;
  
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[0];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[1];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = chGroup[2];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x07;//chLinePhase;    //��ƫ ��ƫ
    
      for(int i = 0 ;i < 20; i++)
         m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = i;
      pSendFrame->Frameff.FrameLen = m_SendBuf.wWritePtr + 1;//���ĳ��Ȱ���֡ͷ֡β����������ͷ��0xff;У��Ͳ�����֡ͷ֡β�Լ���ͷ��oxff
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&pSendFrame->Frameff.FrameLen, pSendFrame->Frameff.FrameLen-4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      CommWrite((char *)m_SendBuf.pBuf,m_SendBuf.wWritePtr);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
      m_PaWaitflag_rf003 = ON;
      m_PaWaitCt_rf003 = 2;//+g_gRunPara[RP_CALL_TIME];
      m_TxNum_rf003 = 0;
      return TRUE;
  
  }
  
  //RF�����ݽ���
BOOL CPrtcRF::RecFrame55(VFrame55 *pReceiveFrame)
{ 
    WORD     relative_timel;
    WORD     relative_timeh;
  if(pReceiveFrame->Type == 0x06)
  {//rf001
      if(pReceiveFrame->RFASDU.RfAsdu.byInfoNum%RF001FTDANUM_A0 == 0)
        RecRf001Fault_A0(pReceiveFrame);
      else if(pReceiveFrame->RFASDU.RfAsdu.byInfoNum%RF001FTDANUM_3P4O== 0)
        RecRf001Fault_3P4O(pReceiveFrame);
      else
        RecRf001Fault(pReceiveFrame);
  }
  else if(pReceiveFrame->Type == 0x07)
  {
      if(pReceiveFrame->RFASDU.Rf003Asdu.ByConten[1] == 0xD2)
      {
          if(pReceiveFrame->RFASDU.Rf003Asdu.ByConten[2] == 0x18)
          {
              RecRf001Para(pReceiveFrame);
          }
          
          else
          {
              //if(g_YkOrderFlag == ON) 
                 //g_YkOrderFlag = TRUE;
              RecRf003Fault(pReceiveFrame);//¼����־λ�Ľ���
          }
      }
      else if(pReceiveFrame->RFASDU.Rf003Asdu.ByConten[1] == 0x70)//д����ָʾ���ظ�����
      {
        if(g_RWFiInf.WPaWaitflag_fi == ON)
        {
         g_RWFiInf.WPaWaitflag_fi = TRUE;
         g_RWFiInf.WPaWaitCt_fi =0;
        }
        else
        Send_Wirter_Ack(pReceiveFrame->RFASDU.Rf003Asdu.ByConten[2]);//RecRepeateryc(pReceiveFrame);
      }
      
  }
  else if(pReceiveFrame->Type == 0xd2)
  {
      if(pReceiveFrame->RFASDU.RfAsduSubTp.bySubType== 0x18)
      {//ָʾ������
          RecRf001Para(pReceiveFrame);
      }
      if(pReceiveFrame->RFASDU.RfAsduSubTp.bySubType== 0x30)
      {
          RecRf003Fault(pReceiveFrame);
      }
      if(pReceiveFrame->RFASDU.RfAsduSubTp.bySubType== 0x27)
      {
          m_Waitflag_rf003 = OFF;
          m_WaitCt_rf003 =0;
          SendRf003Recorder(0,COMTRADE_ITYPE);//�ٻ�������·
          Recorder.Recorder_Current_Num=0;
          mrecorder_long = 0;
          m_PaWaitflag_rf003 = OFF;
          m_PaWaitCt_rf003 = 0;
          
          //Recorder.Recorder_flag1= ON;//¼���н���͸������ģʽ
      }
      if((pReceiveFrame->RFASDU.RfAsduSubTp.bySubType== 0x28)||(pReceiveFrame->RFASDU.RfAsduSubTp.bySubType== 0x37)||(pReceiveFrame->RFASDU.RfAsduSubTp.bySubType== 0x38))
      {
         m_Waitflag_rf003 = OFF;
         m_WaitCt_rf003 =0;
         m_PaWaitflag_rf003 = OFF;
         m_PaWaitCt_rf003 = 0;
         //m_TxNum_rf003 = 0;
         if(g_detephase_flag == OFF)
         {
          if((Recorder.Current_line_phase & 0x03) == 1)
      
              Recorder.Recorder_flag1 = ON;
      
          else if((Recorder.Current_line_phase & 0x03) == 2)
              Recorder.Recorder_flag2 = ON;
          else if((Recorder.Current_line_phase & 0x03) == 3)
              Recorder.Recorder_Allflag = ON;
         }
    else if(g_detephase_flag == ON)
          {
     Recorder.Recorder_Allflag = ON;
              
          }
       //A���Ѵ������
       Recorder.Recorder_flag =ON;
          //Recorder.Recorder_flag1= ON;//¼���н���͸������ģʽ
      }
      //55 AA 08 D2 A0 02 7C 16
      if(pReceiveFrame->RFASDU.RfAsduSubTp.bySubType== 0xa0)
      {//yk ����2
          if(pReceiveFrame->RFASDU.RfAsduSubTp.byInfoNum== 0x02)
          {//02������ָ��IP
            if(g_YkOrderFlag == ON) 
              g_YkOrderFlag = TRUE;
            else if(g_WPaOrderFlag == ON)
            {
              if(m_RWPaSrcObj !=null)
                 m_RWPaSrcObj->SendWrPaSuc(); 
              g_WPaOrderFlag = OFF;
            }
          }
          else if(pReceiveFrame->RFASDU.RfAsduSubTp.byInfoNum== 0x03
          || pReceiveFrame->RFASDU.RfAsduSubTp.byInfoNum== 0x04
          ||pReceiveFrame->RFASDU.RfAsduSubTp.byInfoNum== 0x05)
          {//03δ������ָ��IP 04����ģ����� 05���ڱ��Ĵ���
            if(g_YkOrderFlag == ON) 
              g_YkOrderFlag = FALSE;
            else if(g_WPaOrderFlag == ON)
              g_WPaOrderFlag = OFF;
          }
      }
  }
  else if(pReceiveFrame->Type == 0xd6)//�յ�ָʾ����¼�����Ľ���֡
  {
        SendRCToRf003(Recorder.Current_line_phase,8);
        m_PaWaitflag = OFF;
  }
  else if(pReceiveFrame->Type == 0x63)//����ģ�鷢�Ķ�ʱ���ģ���Ҫ���ͷ��ӵļ���
  {
    g_RF003_timeInfo.RF003_mini_num[g_RF003_timeInfo.RF003_num] = MAKEWORD(pReceiveFrame->RFASDU.rf003timesyn.minute_countl,pReceiveFrame->RFASDU.rf003timesyn.minute_counth);
  g_RF003_timeInfo.RF003_time_flag =ON;
  g_RF003_timeInfo.RF003_num ++;
  if(g_RF003_timeInfo.RF003_num >2)
    g_RF003_timeInfo.RF003_num =0;  //g_RF003_timeInfo.RF003_state = TRUE;
  }
  else if(pReceiveFrame->Type == 0x68)//����ģ�����
  { 
  RecRf003Para(pReceiveFrame);
   m_PaWaitflag_rf003 = OFF;
     m_PaWaitCt_rf003 = 0;
     m_TxNum_rf003 = 0;
  }
  else if(pReceiveFrame->Type == 0x67)//����ģ�����
  { 
   Send_Wirter_Ack(pReceiveFrame->Type);//��д����ģ�������������13
   m_PaWaitflag_rf003 = OFF;
     m_PaWaitCt_rf003 = 0;
     m_TxNum_rf003 = 0;
  }
  else if(pReceiveFrame->Type == 0xd5)//ֻ�е�0�����ݵĵ�һ���ֽ�Ϊ0XAAʱ��ʾ��ָʾ����¼������
  {
    //pReceiveFrame->FrameLen == 0x06
    
    
    if((pReceiveFrame->RFASDU.rf003recorder.Current_Num==0)&&(Recorder.Recorder_Current_Num == 0))//����һ�����е����ݶ���������
      {
        g_recorder.lubo_flag = pReceiveFrame->RFASDU.rf003recorder.Content[0];
        relative_timel = MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[1],pReceiveFrame->RFASDU.rf003recorder.Content[2]);
        relative_timeh = MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[3],pReceiveFrame->RFASDU.rf003recorder.Content[4]);
        g_recorder.relative_time1= MAKEDWORD(relative_timel,relative_timeh);
        relative_timel = MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[5],pReceiveFrame->RFASDU.rf003recorder.Content[6]);
        relative_timeh = MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[7],pReceiveFrame->RFASDU.rf003recorder.Content[8]);
        g_recorder.relative_time2= MAKEDWORD(relative_timel,relative_timeh);
        relative_timel = MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[9],pReceiveFrame->RFASDU.rf003recorder.Content[10]);
        relative_timeh = MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[11],pReceiveFrame->RFASDU.rf003recorder.Content[12]);
        g_recorder.relative_time3= MAKEDWORD(relative_timel,relative_timeh);
        
        g_recorder.total_num1= MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[13],pReceiveFrame->RFASDU.rf003recorder.Content[14]);
        g_recorder.recorder_long= MAKEWORD(pReceiveFrame->RFASDU.rf003recorder.Content[15],pReceiveFrame->RFASDU.rf003recorder.Content[16]);
        g_recorder.VI_flag=pReceiveFrame->RFASDU.rf003recorder.Content[17];
    g_recorder.lubo_start_type = pReceiveFrame->RFASDU.rf003recorder.Content[18];
    if(g_RF003_timeInfo.LUBO_time_flag ==ON)//��ָֻʾ����һֻ��ʱ��Ϊ��Ч����ָֻʾ��¼����ʱ�����óɸ�ʱ��
    {
      memcpy(g_recorder.absolute_time,g_RF003_timeInfo.LUBO_absolute_time,RTC_NUM*2);
      //g_RF003_timeInfo.LUBO_time_flag =OFF;
    }
    else
      memcpy(g_recorder.absolute_time,g_sRtcManager.m_gRealTimer,RTC_NUM*2);
    /*g_recorder.absolute_time[7] = g_sRtcManager.m_gRealTimer[RTC_YEAR];
        g_recorder.absolute_time[6]  = g_sRtcManager.m_gRealTimer[RTC_MONTH];
        g_recorder.absolute_time[5]  = g_sRtcManager.m_gRealTimer[RTC_DATE];
        g_recorder.absolute_time[4]  = g_sRtcManager.m_gRealTimer[RTC_WEEK];
        g_recorder.absolute_time[3]  = g_sRtcManager.m_gRealTimer[RTC_HOUR];  
        g_recorder.absolute_time[2]  = g_sRtcManager.m_gRealTimer[RTC_MINUT];
        g_recorder.absolute_time[1]  = g_sRtcManager.m_gRealTimer[RTC_SEC];//(g_sRtcManager.m_gRealTimer[RTC_MICROSEC]+g_sRtcManager.m_gRealTimer[RTC_SEC]*1000)&0xff;
        g_recorder.absolute_time[0] = g_sRtcManager.m_gRealTimer[RTC_MICROSEC];//+g_sRtcManager.m_gRealTimer[RTC_SEC]*1000)>>8;*/
        g_recorder.line_recorder = (Recorder.Current_line_phase>>2)&0xFF;//��ƫ��1��ʼ
        g_recorder.phase_recorder = (Recorder.Current_line_phase & 0x03);//��1��ʼ
        //precorder= &g_recorder;
        SaveLBFlash((unsigned char *)&g_recorder,0,pReceiveFrame->RFASDU.rf003recorder.Current_Num,pReceiveFrame->RFASDU.rf003recorder.Data_length);
       
        
      }

    if((pReceiveFrame->RFASDU.rf003recorder.Content[0]==0xAA) &&(pReceiveFrame->RFASDU.rf003recorder.Current_Num==0))//��ָʾ����¼������ֱ���˳�͸������
    {
      //SaveLBFlash(pReceiveFrame->RFASDU.rf003recorder.Content,0,pReceiveFrame->RFASDU.rf003recorder.Current_Num);
      SendRCToRf003(Recorder.Current_line_phase,8);
      
      m_PaWaitflag = OFF;
      /*if(Recorder.Current_line_phase & 0x03 == 0x03)
      {
        CAT_SpiReadWords(EEPADD_LUBONUM, 1, &(Recorder.FRecorder_Current_COUNT));//�ȶ�������ֹ�ϵ���0
        Recorder.FRecorder_Current_COUNT ++;
        CAT_SpiWriteWords(EEPADD_LUBONUM, 1, &(Recorder.FRecorder_Current_COUNT));
      }*/
    }
    else
    {
      g_recorder.total_num1= pReceiveFrame->RFASDU.rf003recorder.Total_Num;
      g_recorder.current_num= pReceiveFrame->RFASDU.rf003recorder.Current_Num;
      
      if((g_recorder.current_num <= g_recorder.total_num1)&&(g_recorder.current_num == Recorder.Recorder_Current_Num))
      {
        m_PaWaitflag = OFF;//�յ�д������ȷ�ϱ��ĺ�ر��ش�
        m_TxNum = 0;
       if(g_recorder.current_num) mrecorder_long+=pReceiveFrame->RFASDU.rf003recorder.Data_length;
        if(g_recorder.current_num == g_recorder.total_num1)
        {
           //mrecorder_long=0;
         if(mrecorder_long == g_recorder.recorder_long*2)//������յĳ�����ָʾ�������ܳ��Ȳ�һ�£��������ٻ�
          {
           if(g_recorder.VI_flag == COMTRADE_ITYPE)//��ǰ�ǵ���¼���������ٻ���ѹ¼��
           {
             SendRf003Recorder(0,COMTRADE_VTYPE);
             Recorder.Recorder_Current_Num=0;
             mrecorder_long = 0;
           }
           else if(g_recorder.VI_flag ==COMTRADE_VTYPE)
           {
             SendRf003Recorder(0,COMTRADE_FINISH);//��ָʾ������¼�������ı�־λ
             //SendRCToRf003(Recorder.Current_line_phase,8);
           }
           
          }
          else
          {
            m_reNum++;
            if(m_reNum < 2)//�������뷢�͵ĳ��Ȳ�һ��ʱ�������ٻ�
            {
              if(g_recorder.VI_flag == COMTRADE_ITYPE)//��ǰ�ǵ���¼�����ش�����¼��
                SendRf003Recorder(0,COMTRADE_ITYPE);
              else if(g_recorder.VI_flag ==COMTRADE_VTYPE)
                SendRf003Recorder(0,COMTRADE_VTYPE);
              Recorder.Recorder_Current_Num=0;
              mrecorder_long = 0;
            }
            else
            {
              //mrecorder_long=0;
             // for(int i = pReceiveFrame->RFASDU.rf003recorder.Data_length+1;i<256;i++)
                //pReceiveFrame->RFASDU.rf003recorder.Content[i]=0;
              //memset(&pReceiveFrame->RFASDU.rf003recorder.Content, 0, sizeof(pReceiveFrame->RFASDU.rf003recorder.Content);      
              SaveLBFlash(pReceiveFrame->RFASDU.rf003recorder.Content,0x55,pReceiveFrame->RFASDU.rf003recorder.Current_Num,pReceiveFrame->RFASDU.rf003recorder.Data_length);//���һ�����ݣ��ڶ����ֽ�Ϊ0x55����ʾ����ָʾ����¼�����ݲ���
              m_reNum = 0;
              SendRCToRf003(Recorder.Current_line_phase,8);
            }
           
          }
          
        }
        else
        {
          if(g_recorder.VI_flag ==COMTRADE_ITYPE)//��ǰ�ǵ���¼�����ش�����¼��
            SendRf003Recorder(g_recorder.current_num+1,COMTRADE_ITYPE);
          else if(g_recorder.VI_flag ==COMTRADE_VTYPE)
            SendRf003Recorder(g_recorder.current_num+1,COMTRADE_VTYPE);
          Recorder.Recorder_Current_Num=g_recorder.current_num+1;
        }
        if(g_recorder.current_num > 0)
        {
          //for(int i = pReceiveFrame->RFASDU.rf003recorder.Data_length+1;i<256;i++)
                //pReceiveFrame->RFASDU.rf003recorder.Content[i]=0;
          SaveLBFlash(pReceiveFrame->RFASDU.rf003recorder.Content,0,pReceiveFrame->RFASDU.rf003recorder.Current_Num,pReceiveFrame->RFASDU.rf003recorder.Data_length);
          //mrecorder_long+=pReceiveFrame->RFASDU.rf003recorder.Data_length;
        }
      }
      else if(g_recorder.current_num != Recorder.Recorder_Current_Num)
        SendRfRetry();
      /*if( g_recorder.current_num == g_recorder.total_num)
      {
       SaveLBFlash(pReceiveFrame->RFASDU.rf003recorder.Content,0x55,pReceiveFrame->RFASDU.rf003recorder.Current_Num)
       SendRCToRf003(Recorder.Current_line_phase,8);
       
      }*/
    }
    
      
  }
  return TRUE;
}

void  CPrtcRF:: Recorder_time(unsigned int *precorder_time)
{
  //DWORD time_total;
  unsigned int s_num;
  unsigned  int ms;
  
  //time_total = (g_recorder.relative_time1>>2) + g_recorder.absolute_time[RTC_MICROSEC];
  s_num = (g_recorder.relative_time1>>2)/1000;
  precorder_time[RTC_SEC] +=s_num ;
  ms =(g_recorder.relative_time1>>2)%1000 + precorder_time[RTC_MICROSEC];
  if(ms >=1000)
  {
     precorder_time[RTC_MICROSEC] = (ms - 1000);
     precorder_time[RTC_SEC] ++;   
  }
  else
     precorder_time[RTC_MICROSEC] = ms ;
  if(precorder_time[RTC_SEC] >= 60)     //���Ӽ�ʱ
    {
      precorder_time[RTC_SEC]  = precorder_time[RTC_SEC]%60;
            precorder_time[RTC_MINUT]+=precorder_time[RTC_SEC]/60;
      if(precorder_time[RTC_MINUT] >= 60)
            {
                precorder_time[RTC_MINUT] = precorder_time[RTC_MINUT]%60;
                precorder_time[RTC_HOUR]=precorder_time[RTC_MINUT]/60;
      }
  }
    
}
void CPrtcRF:: SaveLBFlash(unsigned char *pLbBuff,unsigned char error_flag,unsigned int Val,BYTE Data_length1)
{
  unsigned long FLbAddr = 0;
  unsigned long FAddr = 0;
  unsigned char ReadBack[256] = {0};//д��flash�����¶�ȡ����
  //unsigned int FLbInfo[FLOADINFONUM]; 
  unsigned int PageSize;//��ǰҳ��ʣ��ռ�
  char group,phase;
  
  //unsigned long address = 0;
  //unsigned long addressV = 0;
  //unsigned long addresscfgV =0;
   //unsigned long addresscfg =0;
   //int i=0;
  
 
 // WORD MSecond;
  //unsigned char old_group;
 /*if(Val ==0)//���͵�һ֡����ʱ���´���
  {
    CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, &(g_FRecorder_Current_COUNT));
    if(Recorder.FRecorder_Current_COUNT >= 0xFE)  
    {  
      Recorder.FRecorder_Current_COUNT=0;
      CAT_SpiWriteWords(EEPADD_LUBOFLAG, 1, &(Recorder.FRecorder_Current_COUNT));
    }
  }*/
  RECORDER_CFG Recorder_cfg;
  group =g_FRecorder_Current_COUNT;//����
  //CAT_SpiWriteWords(EEPADD_LUBONUM, 1, group);
  //unsigned long FLoadAddr;
  //unsigned int FLuboInfo[RECORDER_TOTALNUM];      
  //wave_total = Recorder.FRecorder_Current_COUNT;//�ܵ�����
  if((Data_length1 ==0) &&(Val ==0))
    phase=0;  
  else
    phase=(Recorder.Current_line_phase & 0x03)-1;
  
  
  if(Val >= 1)
  {
    //FLbAddr = Val-1;
        //FLbAddr *= Data_length1;//FLASH_PAGE_SIZE;
    if(Val ==1)
      FLbAddr_offset =0;
    
    if(g_recorder.VI_flag == COMTRADE_ITYPE)
    {
        FLbAddr = FLbAddr_offset + FADDR_RECORDER_START+(unsigned long)group*FLINEADDR+(unsigned long)phase*FPHASEADDR;
    }
    else if(g_recorder.VI_flag == COMTRADE_VTYPE)
        FLbAddr = FLbAddr_offset + FADDR_RECORDER_STARTV+(unsigned long)group*FLINEADDR+(unsigned long)phase*FPHASEADDR;//�ܵ�ַ 
 
    FLbAddr_offset +=Data_length1;
  }
  else
  {
    FLbAddr = 0;
    //address = FADDR_RECORDER_START; 
  }
 
    if(FLbAddr % 4096 == 0)//||(FLbAddr == FADDR_RECORDER_START)||(FLbAddr == FADDR_RECORDER_STARTV))&&(FLbAddr != 0))
    {   
        if(FLbAddr)//¼�������߳������ݵĵ�ַ
        {
          Sector_Erase(FLbAddr);
        }
        else //¼��Ŀ¼�Ĵ���
        {
          Recorder_cfg.lubo_total_num = group;//����һ���µ�¼����Ϣ��������������������
        
            //if(!(g_gRunPara[RP_CFG_KEY] & BIT[RP_COMTRADE_TYPE]))
            Recorder_cfg.CFG_Leng = strlen(ComtrderCfg);
            //else
             //Recorder_cfg.CFG_Leng = strlen(ComtrderCfg)+1;
             if((Data_length1 ==0) &&(Val ==0))
             {
              Recorder_cfg.DATA_Leng = 0;
              Recorder_cfg.TOTAL_Leng = 0;
              Recorder_cfg.lubo_flag =0x55;
        Recorder_cfg.lubo_type=0;
        memcpy(Recorder_cfg.comtrade_time, g_sRtcManager.m_gRealTimer,RTC_NUM*2);
             }
        else
        {
              //Recorder_cfg.DATA_Leng = DAT_LENGTH;
         if(g_gRunPara[RP_LUBO_CYCLE] ==0)
                                 {
          Recorder_cfg.DATA_Leng = DAT_LENGTH;
                                 }
         else
         {
          #ifndef FILTER_COM
                Recorder_cfg.DATA_Leng = ((g_gRunPara[RP_LUBO_CYCLE]*80) * 22);// -(rela_time_max * 24);  
              #else
                Recorder_cfg.DATA_Leng = ((g_gRunPara[RP_LUBO_CYCLE]*80) * 28);// -(rela_time_max * 28); 
              #endif
         }
              Recorder_cfg.TOTAL_Leng = Recorder_cfg.CFG_Leng+Recorder_cfg.DATA_Leng;
              Recorder_cfg.lubo_flag =g_recorder.lubo_flag;
        Recorder_cfg.lubo_type=g_recorder.lubo_start_type;
        //if(GetRIStatus(RI_RF_ERR))//gps״̬�쳣����ʱ��gps
          //memcpy(Recorder_cfg.comtrade_time, g_sRtcManager.m_gRealTimer,RTC_NUM*2);
        //else
        {
          memcpy(Recorder_cfg.comtrade_time,g_recorder.absolute_time,RTC_NUM*2);
          //Recorder_time(Recorder_cfg.comtrade_time);
          MyPrintfD((BYTE *)Recorder_cfg.comtrade_time,RTC_NUM*2);//����ʹ��
        }
    
        }
            Recorder_cfg.relative_time1=g_recorder.relative_time1;
            Recorder_cfg.relative_time2=g_recorder.relative_time2;
            Recorder_cfg.relative_time3=g_recorder.relative_time3;
            //Recorder_cfg.relative_timeh =g_recorder.relative_timeh;
        //ABC�κ�һ�������ݶ��ᱣ����ʱ���Ĭ��ABC�Ĵ���ʱ����һ����
            
        //}
          if(g_recorder.VI_flag == COMTRADE_ITYPE)
          {
               FAddr = FADDR_RECORDER_INFO+(unsigned long)group*(unsigned long)FLINEADDR+(unsigned long)phase*FPHASEADDR;
          
         // else
               //FAddr = FADDR_RECORDER_VINFO+phase*FPHASEADDR+group*FLINEADDR;
            if(FAddr % 4096 == 0)
            {
                Sector_Erase(FAddr);
                Sst26vf064b_Page_WriteBytes(FAddr,(unsigned char *)&Recorder_cfg,sizeof(Recorder_cfg));//(unsigned char *)
                Sst26vf064b_Read(FAddr,(unsigned char *)&Recorder_cfg,sizeof(Recorder_cfg));//(unsigned char *)&gRecorder_cfg[group],sizeof(gRecorder_cfg[group]));//��������һ��
            }
          }
       }
     
    }
    if(FLbAddr)//���ָʾ�����ı�־λ�����򲻽���
    {
      PageSize = 0x100 - (FLbAddr%0x100);//��ǰҳ��ʣ��ռ�
      
      if(PageSize < Data_length1)//WRITEPECORDER��ǰҳ��ʣ��ռ�С��Ҫд��������
      {
        Sst26vf064b_Page_WriteBytes(FLbAddr,pLbBuff,PageSize);//ÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����                    
        Sst26vf064b_Page_WriteBytes(((FLbAddr&0xffffff00) + 0x100),&pLbBuff[PageSize],(Data_length1-PageSize)); //��һҳ����ʼ��ַ               
        Sst26vf064b_Read((FLbAddr&0xffffff00),ReadBack,256);
        Sst26vf064b_Read(((FLbAddr&0xffffff00) + 0x100),ReadBack,256);//����ÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����            
        //Sst26vf064b_Read((FLbAddr&0xffffff00) + 0x100,&ReadBack[PageSize],Data_length1-PageSize);
      }
      else
      {
        Sst26vf064b_Page_WriteBytes(FLbAddr,pLbBuff,Data_length1);//FLASH_PAGE_SIZE
        Sst26vf064b_Read(FLbAddr,ReadBack,256);//����ÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����
      }
    //Sst26vf064b_Read(FLbAddr,ReadBack,3);
      /*if(error_flag==0x55)
       Sst26vf064b_Page_WriteBytes(FLbAddr-1,&error_flag,1);*/
     /*if(Recorder.FRecorder_Current_COUNT == FRECORDER_TOLNUM)
     {
        //FLbAddr = FADDR_RECORDER_START;
        Recorder.FRecorder_Current_COUNT-=1;
     }*/
    }
}


BOOL CPrtcRF::RecRf001Fault_3P4O(VFrame55 *pReceiveFrame)
{
    BYTE phase = 0;
    BYTE line = 0;
    BYTE InfoNum = 0;
    RF001FTDA_3P4O *pFaultCon;
    PHASEFTDA  FtConPh;
    WORD wFltYxIndex = 0;//�������
    const BYTE byFtTypes[FTTPYE_NUM] = {0x0C,0X08,0xff,0X40};//��·,�ӵ�,��ѹ(Ԥ��)��ָʾ���¶ȸ澯
    const BYTE byCommFtTys[10] = {0X01,0X02,0X04,0X08,0X10,0X20,0X40};//A,B,C,Oͨ�����ϣ�0x10 �糡��ѹ����,0x20 �������,0x40 �Լ챨��
    const BYTE byCurFtTys[10] = {0X01,0X02,0X04,0X40,0X08,0X10,0X20};//A,B,C,O�������ϣ�A,B,C�¶ȹ���
    BYTE bHaveFt = 0;//��ʶ�������й�������
    
    InfoNum = (pReceiveFrame->FrameLen-7)/RF001FTDANUM_3P4O;
    pFaultCon = (RF001FTDA_3P4O *)&pReceiveFrame->RFASDU.RfAsdu.byContent;
    for(int i = 0; i < InfoNum;i++)
    {
        line = pFaultCon->byLinePhase;
        line = (line >> 2) & 0x3F;//ȡ����·ƫ�� ��6λ
        //�ж�ͨ��״̬==========================================================
        WORD wComIndex = 0;
        WORD wAllComIndex = 0;//��ͨ��״̬ң�����
        int YcIndex = 0;
        int YcTIndex = 0;//�¶�index
        BYTE ComFltFlag = 0;//ͨ�����ϱ�ʶ
        for(BYTE byComm = 0;byComm < 4;byComm++)
        {
          wAllComIndex  = line-1 + RI_1ZSQST;
          if((pFaultCon->byFaultComm & byCommFtTys[byComm]) == byCommFtTys[byComm])
          {//����ͨ������
              wComIndex = (line-1)*PHASE_NUM + byComm + RI_1A_ZSQST;
              if(GetRIStatus(wComIndex) == 0)
              {
                  CreatNewSoe(wComIndex,1);
                  if(!(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_COMMFLTVAL_MODE]))
                  {
                    //��Ӧ��ң�����ó���Ч
                    YcIndex = YxIdToYcId(wComIndex);
                    YcTIndex = YxIdToYcId(wComIndex,1);
                    if(YcIndex != 255) 
                    {
                      g_gRmtMeas[YcIndex + RM_1A_FAULT_I] = INVALIDYC;
                      g_gRmtMeas[YcIndex + RM_1A_LOAD_I] = INVALIDYC;
            
                      g_ucRefYxYcTs |= BIT1;
                    }
                    //�¶����ó���Ч
                    if(YcTIndex != 255)
                    {
                      //g_gRmtMeas[YcTIndex + RM_1A_CABLE_T] = INVALIDYC;
                      g_gRmtMeas[YcTIndex + RM_1A_CABLE_T_INT] = INVALIDYC;
                      g_ucRefYxYcTs |= BIT1;
                    }
                  }
                  
              }
              ComFltFlag = 1;
          }
          else
          {//����ͨ�����ϻָ�
              wComIndex = (line-1)*PHASE_NUM + byComm + RI_1A_ZSQST;
              if(GetRIStatus(wComIndex) == 1)
              {
                  CreatNewSoe(wComIndex,0);
              }
          }
        }
        //��ͨ��״̬
        if(ComFltFlag && (GetRIStatus(wAllComIndex) == 0))
        {
          CreatNewSoe(wAllComIndex,1);
        }
        else if((ComFltFlag == 0) && (GetRIStatus(wAllComIndex) == 1))
        {
          CreatNewSoe(wAllComIndex,0);
        }
        //============================================================================
        for(phase = 1; phase <= PHASE_NUM;phase++)
        {
            if(phase < PHASE_NUM)
            {
              memcpy(&FtConPh.byExtTmpt,&pFaultCon->byPhData[5*(phase-1)],5);
            }
            else
            {
              memcpy(&FtConPh.byIL,&pFaultCon->byPhData[5*(phase-1)],3);
            }
            //ȡ������ң��
            for(int iFtType = 0;iFtType < FTTPYE_NUM;iFtType++ )
            {
               if(byFtTypes[iFtType] == 0x08)
               {
                  if(phase != PHASE_NUM)
                      continue;//�ӵع���ֻ��o����
                }
                BYTE FtFlag = 0;
                if((byFtTypes[iFtType] != 0)&& (FtConPh.byPhFaultInfo & byFtTypes[iFtType]) == byFtTypes[iFtType])
                {
                  if(byFtTypes[iFtType] != 0X40)
                  {//��������
                    if(phase == 1 && ((pFaultCon->byFaultCur & byCurFtTys[0]) == byCurFtTys[0]) ) FtFlag = 1;
                    if(phase == 2 && ((pFaultCon->byFaultCur & byCurFtTys[1]) == byCurFtTys[1]) ) FtFlag = 1;
                    if(phase == 3 && ((pFaultCon->byFaultCur & byCurFtTys[2]) == byCurFtTys[2]) ) FtFlag = 1;
                    if(phase == 4 && ((pFaultCon->byFaultCur & byCurFtTys[3]) == byCurFtTys[3]) ) FtFlag = 1;  
                  }
                  else 
                  {//�¶ȹ���
                    if(phase == 1 && ((pFaultCon->byFaultCur & byCurFtTys[4]) == byCurFtTys[4]) ) FtFlag = 1;
                    if(phase == 2 && ((pFaultCon->byFaultCur & byCurFtTys[5]) == byCurFtTys[5]) ) FtFlag = 1;
                    if(phase == 3 && ((pFaultCon->byFaultCur & byCurFtTys[6]) == byCurFtTys[6]) ) FtFlag = 1;
                  }
                  if(FtFlag == 0)
                    continue;
                  wFltYxIndex = LPFToYxId(line,phase,iFtType);
                  if(RI_IN_DAERR == wFltYxIndex)  
                  {
                    SetRIStatus(RI_IN_DAERR, 1);
                    continue;
                  }
                  if(g_gRmtLock[wFltYxIndex + RI_1A_SHORT] == 0)
                  {
                    g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
                    CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);
                    if(iFtType == 0)
                      bHaveFt = 1;
                  }
                  //break;//���һ��ָʾ�������а������ֹ�������ȥ���˴���break
                }   
            }
            //ȡ�����������븺�ɵ���������(�����ֹ��ϵ������Ǹ��ɵ���)
            WORD wYcIndex = 0;
            wYcIndex = (line-1)*PHASE_NUM + (phase-1);
           if(GetRIStatus(wYcIndex+ RI_1A_ZSQST) == 0)//ͨ�����Ϻ󲻸���ң��ֵ
            {
              g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] = MAKEWORD(FtConPh.byIL,FtConPh.byIH & 0x7f);
              g_ucRefYxYcTs |= BIT1;
              //ȡ�����¶�
              WORD wTmpIndex = 0;
              if(phase != 4 )
              {
                wTmpIndex = (line-1)*(PHASE_NUM-1) + (phase-1);//O��û���¶�
               // g_gRmtMeas[wTmpIndex + RM_1A_CABLE_T] = FtConPh.byExtTmpt;//�ⲿ�¶�
                g_gRmtMeas[wTmpIndex + RM_1A_CABLE_T_INT] = FtConPh.byIntTmpt;//�ⲿ�¶�
                g_ucRefYxYcTs |= BIT1;
              }
              //if(FtConPh.byPhFaultInfo & (byFtTypes[0]|byFtTypes[1]|byFtTypes[2]|byFtTypes[3]))
              if(bHaveFt)
              {
                g_FltChgSdYc = 1;//����ң��Ӧ�����ϴ�
              }
           }
        
        }
       pFaultCon ++;     
    }
 return TRUE;

}

BOOL CPrtcRF::RecRf001Fault_A0(VFrame55 *pReceiveFrame)
{
      BYTE phase = 0;
      BYTE line = 0;
      BYTE InfoNum = 0;
      //WORD group = 0;
      RF001FTDA_A0 *pFaultCon;
      WORD wFltYxIndex = 0;//�������
      const BYTE byFtTypes[FTTPYE_NUM] = {0x01,0X02,0X04,0xff};//0x01=��· 0x02=�ӵ� 0x04=�ⴥ��
  
    InfoNum = (pReceiveFrame->FrameLen-7)/RF001FTDANUM_A0;
    pFaultCon = (RF001FTDA_A0 *)&pReceiveFrame->RFASDU.RfAsdu.byContent;
    for(int i = 0; i < InfoNum;i++)
    {
        phase = pFaultCon->byLinePhase & 0x03;//ȡ����λƫ��
        if(phase == 0) phase = 4;//���⴦�� O��
        line = pFaultCon->byLinePhase;
        line = (line >> 2) & 0x3F;//ȡ����·ƫ�� ��6λ 
//        group = MAKEWORD(pFaultCon->byGroup[1],pFaultCon->byGroup[0]);
//        if(group != GetOwnAddr())
//           continue;
        //�ж�ͨ��״̬
        WORD wComIndex = 0;
        wComIndex = (line-1)*PHASE_NUM + (phase-1);
        g_sCommStatCount[wComIndex] = g_gRunPara[RP_ZSQCST_T];
      
        //ȡ������ң��
        for(int iFtType = 0;iFtType < FTTPYE_NUM;iFtType++ )
        {
          if((byFtTypes[iFtType] != 0)&& (pFaultCon->byFaultInfo & byFtTypes[iFtType]) == byFtTypes[iFtType])
          {
            wFltYxIndex = LPFToYxId(line,phase,iFtType);
            if(RI_IN_DAERR == wFltYxIndex)  
            {
              SetRIStatus(RI_IN_DAERR, 1);
              continue;
            }
            if(g_gRmtLock[wFltYxIndex + RI_1A_SHORT] == 0)
            {
              g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
              if((iFtType == 1) && (g_gSmsPhone[PHONE_ENCTL] & BIT2))  //�����ӵع��ϣ���ѯ���ź�Դ
              {
                  g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = 10 * g_gRunPara[RP_FLTLK_T];  //�ӵر��ֱ����������ϱ��Ժ����¼�ʱ
                  g_yxChangeflag |= BITF;//��ʶ�б�����Ҫ�ϴ�
                  if(g_sTimer[TM_WAITXINHAO].m_TmCount == 0) //�ź�Դ2�ְ����
                  {
                      g_EnSendToXinhao = ON;
                  }
                  //OpenTimer(TM_WAITXINHAO);
                  //�˴��ݴ�ӵع���
                  g_gGroundBuff[g_unGroundBuffTail++] = wFltYxIndex + RI_1A_SHORT;
              }
              else
              {
                CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);
              }              
            }
            //break;//���һ��ָʾ�������а������ֹ�������ȥ���˴���break
          }   
        }
       pFaultCon ++;     
    }
 return TRUE;

}

BOOL CPrtcRF::Phase_Detection(int line)
{

  struct RECORDER_CFG record_cfg;
    int i;
    unsigned char group;
  BYTE secName =0;
  CAT_SpiReadWords(EEPADD_LUBOFLAG, 1, (unsigned int *)&group);
   
    //*Pgroup = group;
    
    for(i=0;i<3;i++)
    {
       
        long FADDR_RECORDER =FADDR_RECORDER_INFO+(long)group*(long)FLINEADDR+(long)i*(long)FPHASEADDR;
        Sst26vf064b_Read(FADDR_RECORDER,(unsigned char *)&record_cfg,sizeof(record_cfg));//��A������һ��
        //Sst26vf064b_Read(FADDR_RECORDER_VINFO+i*FLASH_PAGE_SIZE,(unsigned char *)&record_cfg,sizeof(record_cfg));
        if((record_cfg.lubo_flag!=0x55)||(g_phase_loss[i]== ON))//û����͸����������ָʾ��û����
        {     
                if(record_cfg.lubo_flag!=0xAA)//����͸������
                {
                      if(gloss_phase_flag[i] ==OFF)
        gloss_phase_flag[i] =ON;
      else if(gloss_phase_flag[i] ==ON)
                        {
        gloss_phase_flag[i] = TRUE;
                                break;
                        }
                }
                else//��һ��û�н���͸�������û���꣬�ڶ���ָʾ����¼������
                {
                        if(gloss_phase_flag[i] ==ON)
                        {
        gloss_phase_flag[i] = TRUE;
                                break;
                        }
                }
        }
  else
  {
    if(gloss_phase_flag[i] ==ON)
                {
        gloss_phase_flag[i] = TRUE;
                    break;
                }
                else
                    gloss_phase_flag[i] =OFF;
  }
    }
  if(gloss_phase_flag[0] ==ON)
     secName=((line+1)<< 2)+1;
  else if(gloss_phase_flag[1] ==ON)
      secName=((line+1)<< 2)+2;
  else if(gloss_phase_flag[2] ==ON)
      secName=((line+1)<< 2)+3;
  
  if(secName ==0)
  {    
     return FALSE;
  }
  else
        {
      Recorder.Current_line_phase = secName;           
          SendRCToRf003(secName,7);
            return TRUE;  
        }
}


BOOL CPrtcRF::RecRf001Fault(VFrame55 *pReceiveFrame)
{//A2
    BYTE phase = 0;
    BYTE line = 0;
    BYTE InfoNum = 0;
    RF001FTDA *pFaultCon;
    WORD wFltYxIndex = 0;//�������
    const BYTE byFtTypes[FTTPYE_NUM] = {0x08,0X40,0X10,0x04,0xff};//��·,�ӵ�,ָʾ����ص�ѹ�澯  
    //const BYTE byFtTypes[FTTPYE_NUM] = {0x08,0X40,0X04,0xff};//��·,�ӵ�,ָʾ����ص�ѹ�澯  //��ϳ���ָʾ����������
    BYTE bHaveFt = 0;//��ʶ�������й�������
  
    InfoNum = (pReceiveFrame->FrameLen-7)/RF001FTDANUM;
    pFaultCon = (RF001FTDA *)&pReceiveFrame->RFASDU.RfAsdu.byContent;
    for(int i = 0; i < InfoNum;i++)
    {
        phase = pFaultCon->byLinePhase & 0x03;//ȡ����λƫ��
        if(phase == 0) phase = 4;//���⴦�� O��
        line = pFaultCon->byLinePhase;
        line = (line >> 2) & 0x3F;//ȡ����·ƫ�� ��6λ
        //�ж�ͨ��״̬
        WORD wComIndex = 0;
        wComIndex = (line-1)*PHASE_NUM + (phase-1);
        g_sCommStatCount[wComIndex] = g_gRunPara[RP_ZSQCST_T];
      
        //ȡ������ң��
        for(int iFtType = 0;iFtType < FTTPYE_NUM;iFtType++ )
        {
            if((byFtTypes[iFtType] != 0)&& (pFaultCon->byFaultInfo & byFtTypes[iFtType]) == byFtTypes[iFtType])
            {
                wFltYxIndex = LPFToYxId(line,phase,iFtType);
                if(RI_IN_DAERR == wFltYxIndex)  
                {
                  SetRIStatus(RI_IN_DAERR, 1);
                  continue;
                }
                if(g_gRmtLock[wFltYxIndex + RI_1A_SHORT] == 0)
                {
                    g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
                    if((iFtType == 1) && (g_gSmsPhone[PHONE_ENCTL] & BIT2))  //�����ӵع��ϣ���ѯ���ź�Դ
                    {
                        g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = 10 * g_gRunPara[RP_FLTLK_T];  //�ӵر��ֱ����������ϱ��Ժ����¼�ʱ
                        g_yxChangeflag |= BITF;//��ʶ�б�����Ҫ�ϴ�
                        
                        if(g_sTimer[TM_WAITXINHAO].m_TmCount == 0) //�ź�Դ2�ְ����
                        {
                            g_EnSendToXinhao = ON;
                        }
                        //OpenTimer(TM_WAITXINHAO);
                        //�˴��ݴ�ӵع���
                        g_gGroundBuff[g_unGroundBuffTail++] = wFltYxIndex + RI_1A_SHORT;
                    }
                    else
                    {
                        CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);
                    }
                    //��ѹ�澯�����⴦��
                    if((iFtType == 2) || (iFtType == 3))
                    {
                        g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = 0;
                    }
                    else if(iFtType == 0)  
                      bHaveFt = 1;
                }
            
                //break;//���һ��ָʾ�������а������ֹ�������ȥ���˴���break
            }
            else if((iFtType == 2) || (iFtType == 3))  //���鴦�� ��ʱֻ�����ѹ��������
            {
                  wFltYxIndex = LPFToYxId(line,phase,iFtType);
                  if(RI_IN_DAERR == wFltYxIndex)  
                  {
                    SetRIStatus(RI_IN_DAERR, 1);
                    continue;
                  }
                  if(GetRIStatus(wFltYxIndex + RI_1A_SHORT) == 1)
                  {
                      CreatNewSoe(wFltYxIndex + RI_1A_SHORT,0);
                  }
            }
        }
        //ȡ�����ϵ����͸��ɵ���
        const BYTE byLoadYcType = 0x01;
        WORD wYcIndex = 0;
        wYcIndex = (line-1)*PHASE_NUM + (phase-1);
        if(bHaveFt)
        {
            g_gRmtMeas[wYcIndex + RM_1A_FAULT_I] = MAKEWORD(pFaultCon->byIL,pFaultCon->byIH);
            //g_gRmtMeas[wYcIndex + RM_1A_FAULT_I] /= 10;
            //g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] = 0;
            g_ucRefYxYcTs |= BIT1;
            g_FltChgSdYc = 1;//����ң��Ӧ�����ϴ�
        }
        else if(pFaultCon->byFaultInfo & byLoadYcType)
        {
            g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] = MAKEWORD(pFaultCon->byIL,pFaultCon->byIH);
            //g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] /= 10;
            //g_gRmtMeas[wYcIndex + RM_1A_FAULT_I] = 0;
            g_ucRefYxYcTs |= BIT1;
        }
    #ifdef ZDSENDADDR        //����ָʾ�������¶Ⱥ͵�ص�ѹ
        wYcIndex = (line-1)*3 + (phase-1);  //�¶ȵ�ص�ѹ��ʱ��O��
        
        if((pFaultCon->byUH) & 0x80)                    //�¶�
        {
            g_gRmtMeas[wYcIndex + RM_1A_CABLE_T_INT] = MAKEWORD(pFaultCon->byUH,0xff);
        }
        else
        {
            g_gRmtMeas[wYcIndex + RM_1A_CABLE_T_INT] = MAKEWORD(pFaultCon->byUH,0x0);
        }
        g_gRmtMeas[wYcIndex + RN_1A_BAT_U] = MAKEWORD(pFaultCon->byIMinL, pFaultCon->byIMinH);
        //g_gRmtMeas[wYcIndex + RM_1A_BAT_U] /= 10;
        #endif
        pFaultCon ++;     
    }
    return TRUE;
}
/*BOOL CPrtcRF::RecRepeateryc(VFrame55 *pReceiveFrame)
{
    
    BYTE batline = 0;
    REPEATERASDU *pFaultCon;
    pFaultCon = (REPEATERASDU *)&pReceiveFrame->RFASDU.Rf003Asdu.NetID;
    //BYTE fileName;
    //BYTE secName;
    WORD wYcIndex = 0;
    batline = pReceiveFrame->RFASDU.Rf003Asdu.NetIP ;
    //if(phase == 0) phase = 3;
    if(batline >= REPEAT_START_NUM)
      wYcIndex = batline-REPEAT_START_NUM ;//������ȡ�м�����ص�ѹ��ƫ�Ƶ�ַ
    else
      return FALSE;
    g_gRmtMeas[wYcIndex + REPEATER1_CAP_U] = MAKEWORD(pFaultCon->CapValt[0],pFaultCon->CapValt[1]);
    g_gRmtMeas[wYcIndex + REPEATER1_BAT_U] = MAKEWORD(pFaultCon->BatValt[0],pFaultCon->BatValt[1]);
    //g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] /= 10;  
     g_ucRefYxYcTs |= BIT1; 
    //�Ӵ��ڲ�����
     char byBuf1[2] = {0XC5,0XC5};
     CommWrite(byBuf1,2); 
     return TRUE;    
}*/
BOOL CPrtcRF::SendRf003Recorder(unsigned int Num,unsigned char VI_flag)
{
      m_SendBuf.wWritePtr=0;
      //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0xFF;
      BYTE dataStart = m_SendBuf.wWritePtr;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x55;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xAA;
      BYTE framelen = m_SendBuf.wWritePtr;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0B;
      //if(VI_flag == COMTRADE_FINISH)
       // m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x17;
     // else
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x18;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = VI_flag;
      
      //for(int i = 0; i < 9; i++)
         // m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = Num;
     // for(int i = 0; i < 4; i++)
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x00;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[framelen] = m_SendBuf.wWritePtr - dataStart + 2;
      

      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[framelen], m_SendBuf.pBuf[framelen] - 4);//У���У��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdRF);
      memcpy(&m_rfSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
      m_rfSendLen = m_SendBuf.wWritePtr;
      m_PaWaitflag = ON;
      m_PaWaitCt = 2;
      m_TxNum = 0;
      return TRUE;
     
  
}

BOOL CPrtcRF::  Send_Wirter_Ack(BYTE Type)
{
      
        WORD addr = GetOwnAddr();
    //BYTE uartNum = 0;
   /* switch(Type)
    {
      case 0x0E:
       
          if(g_ZsqCmdRBit & BIT0)
          {
              uartNum |= BIT0;
              g_ZsqCmdRBit &= (~BIT0);
          }
          if(g_ZsqCmdRBit & BIT5)
          {
              uartNum |= BIT1;
              g_ZsqCmdRBit &= (~BIT5);
          }
        break;
   case 0x67:
       
          if(g_ZsqCmdRBit & BITC)
          {
              uartNum |= BIT0;
              g_ZsqCmdRBit &= (~BITC);
          }
          if(g_ZsqCmdRBit & BITD)
          {
              uartNum |= BIT1;
              g_ZsqCmdRBit &= (~BITD);
          }
        break;
      case 0x0F:
          if(g_ZsqCmdRBit & BIT1)
          {
              uartNum |= BIT0;
              g_ZsqCmdRBit &= (~BIT1);        
          }
          if(g_ZsqCmdRBit & BIT6)
          {
              uartNum |= BIT1;
              g_ZsqCmdRBit &= (~BIT6);        
          }      
        break;
      case 0x05:
        if(g_ZsqCmdRBit & BIT2)
          {
              uartNum |= BIT0;
              g_ZsqCmdRBit &= (~BIT2);         
          }
          if(g_ZsqCmdRBit & BIT7)
          {
              uartNum |= BIT1;
              g_ZsqCmdRBit &= (~BIT7);         
          }      
        break;
    case 0x90:
    
      if(g_ZsqCmdRBit & BIT8)
          {
              uartNum |= BIT0;
              g_ZsqCmdRBit &= (~BIT8);         
          }
          if(g_ZsqCmdRBit & BIT9)
          {
              uartNum |= BIT1;
              g_ZsqCmdRBit &= (~BIT9);         
          }      
    break;
      default:
          break;
    }*/

    
        m_SendBuf.wWritePtr=0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;    //����ʱ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;     //����Ƶ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xF3;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)addr & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)(addr>>8) & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x7D;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0D;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)addr & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)(addr>>8) & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x02;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;
    /*if(Type == 0x0E)
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 26;
    else if(Type == 0x0F)
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 27;
    else if(Type == 0x05)
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 28;
    else if(Type == 0x90)
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 30;*/
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_WFile;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = g_Sec;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
    //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->SubType2;
    //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      
      m_SendBuf.pBuf[1] = m_SendBuf.wWritePtr - 4;
      m_SendBuf.pBuf[2] = m_SendBuf.pBuf[1];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ChkSum(&m_SendBuf.pBuf[4],m_SendBuf.pBuf[2]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
      /*if(uartNum & 0x01)
      {
          CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdGPRS);
      }
      if(uartNum & 0x02)
      {
          CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdDBG);
      }*/
        BYTE uartId = ((CBJ101S *)m_RWPaSrcObj)->m_uartId;
        CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,uartId);
        return true;
}

void CPrtcRF::Reset_Short(WORD YxIndex,WORD YxIndex1)
{
  if((RI_IN_DAERR == YxIndex)  ||(RI_IN_DAERR == YxIndex1))
        {
      SetRIStatus(RI_IN_DAERR, 1);
              return;//continue;
    }
        if(GetRIStatus(YxIndex + RI_1A_SHORT) == 1)
        {
          CreatNewSoe(YxIndex + RI_1A_SHORT,0);
                  //g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
        g_gRmtLock[YxIndex + RI_1A_SHORT] = 0;
              
        }
  if(GetRIStatus(YxIndex1 + RI_1A_SHORT) == 1)
        {
          CreatNewSoe(YxIndex1 + RI_1A_SHORT,0);
                    //g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
          g_gRmtLock[YxIndex1 + RI_1A_SHORT] = 0;          
        }
}
void CPrtcRF::Mult_Record_Process(BYTE group,RF003FTDA *PFaultCon,BYTE bLuboFt,unsigned int *pabsolute_time)
{
      BYTE num =0;
      BYTE fault_line =0;
      if((PFaultCon->byIMinH & 0x01) &&(g_gRunPara[RP_CFG_KEY]& BIT[RP_START_LUBO]))
        {
              if(Recorder.line_phase[group] ==  YES)
                 Recorder.Recorder_reflag[group] = ON;
              else
              {
                Recorder.line_phase[group] =  YES;
                Recorder.Recorder_reflag[group] = OFF;
                for(unsigned char i =0;i<6;i++)
                {
                  if(Recorder.line_phase[i] == YES)
                  {
                    num++;
                    fault_line =i;   
                  }
                  
                }
                if((num ==1) &&(fault_line == group))
                 {
                  // Recorder.line_phase[group] =  YES;
                   OpenTimer(TM_RECORDED);  
                 }
                  
              }
        if(g_RF003_timeInfo.LUBO_time_flag !=ON)//��һ���յ�¼������
        {
          if((bLuboFt ==1)&&(!(g_gRunPara[RP_CFG_KEYH]& (BIT[RP_FAULT_TIME-16]))))//�յ���ʱ����ҿ��Ը�ָʾ����ʱ����ѡ����ʱ�겻�������յ���ʱ�̴�ʱ��
           memcpy(g_RF003_timeInfo.LUBO_absolute_time,pabsolute_time,RTC_NUM*2);//��ָʾ���Ķ�ʱ���Ĵ�ʧ��Ŷ
        else
           memcpy(g_RF003_timeInfo.LUBO_absolute_time,g_sRtcManager.m_gRealTimer,RTC_NUM*2);//����ǰʱ���ʱ��
          g_RF003_timeInfo.LUBO_time_flag =ON;  
        }            
     }
}
void CPrtcRF::YC_AD(BYTE Group,BYTE Phase,RF003FTDA *PFaultCon,WORD WYcIndex,BYTE *ByFtTypes)
{
    
      /*************�ɼ�ң��������ǰ��Ϊ�����ò������ϵ�ʱ��CreatNewSoe�����вɼ���ң��Ϊ����ң��******************************/
    //�Ƚ������ݵ�ѹ����ص�ѹ���糡ֵ �����ϴ����ɵ�������ʱ�������¼��ĵ糡ֵ���²���
     unsigned long aa_C,aa_V;//int
     BYTE aa_T,aa_B;
     WORD wYcIndex1 = 0;
     WORD WFltYxIndex = 0;//�������
     WORD M_Value;
    wYcIndex1 = Group*3 + (Phase-1);  //��ص�ѹ��O��
    g_gRmtMeas[wYcIndex1 + RN_1A_BAT_U] = MAKEWORD(PFaultCon->byIMaxL, PFaultCon->byIMaxH);
    g_gRmtMeas[wYcIndex1 + RN_1A_BAT_U] *=10;         
    aa_C = ((PFaultCon->byUL)<<8);
    aa_C = aa_C*50;
    aa_C = aa_C >>12;//4096;
    g_gRmtMeas[wYcIndex1 + RN_1A_CAP_U]= aa_C*100;//��ѹ�Ŵ�1000��
    
    aa_V = ((PFaultCon->byUH)<<4);
    g_gRmtMeas[wYcIndex1 + RN_1A_EFIELD_U]= aa_V;
  
  if(PFaultCon->byIMinH & 0x01)//��¼�����ݱ�־λʱȡ��������λ�ñ�ʾȡ���ѹ���¶�
  {
      aa_B = (PFaultCon->by_TakePower);//ȡ���ѹ
    g_gRmtMeas[wYcIndex1 + RM_1A_LUBO]= aa_B*100;//ȡ���ѹ�Ŵ�10�� ��ͳһ��ѹ�Ŵ�100��
      aa_T = (PFaultCon->by_TemperaTure);//�¶�
    g_gRmtMeas[wYcIndex1 + RM_1A_CABLE_T_INT]=(signed char) aa_T;
  }
  else 
  {
    aa_B = (PFaultCon->minute_countL);//ȡ���ѹ
    g_gRmtMeas[wYcIndex1 + RM_1A_LUBO]= aa_B*100;
      aa_T = (PFaultCon->minute_countH);//�¶�
    g_gRmtMeas[wYcIndex1 + RM_1A_CABLE_T_INT]= (signed char)aa_T;
  }
    
    if(((PFaultCon->byFaultInfo & ByFtTypes[0]) == ByFtTypes[0])//˲ʱ ���� ��· 
       ||((PFaultCon->byFaultInfo & ByFtTypes[5]) == ByFtTypes[5])
       ||((PFaultCon->byFaultInfo & ByFtTypes[6]) == ByFtTypes[6])) 
    {
        M_Value = MAKEWORD(PFaultCon->byIL,PFaultCon->byIH);
            if(Phase ==1)
         g_gRmtMeas[WYcIndex + RM_1A_FAULT_I] = (DWORD)(g_gRunPara[RP_CURRENT_CA]) * M_Value/1000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CA];
      else if(Phase ==2)
        g_gRmtMeas[WYcIndex + RM_1A_FAULT_I] = (DWORD)(g_gRunPara[RP_CURRENT_CB]) * M_Value/1000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CB];
      else
        g_gRmtMeas[WYcIndex + RM_1A_FAULT_I] = (DWORD)(g_gRunPara[RP_CURRENT_CC]) * M_Value/1000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CC];
        //g_gRmtMeas[wYcIndex + RM_1A_FAULT_I] /= 10;

        //g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] = 0;
        g_ucRefYxYcTs |= BIT1;
        g_FltChgSdYc = 1;//����ң��Ӧ�����ϴ�
    }
    else //if(bLuboFt == 0)//¼��ʱ����ң��ֵ��׼ȷif((pFaultCon->byFaultInfo & byLoadYcType)&&(bLuboFt ==0))
    {
        M_Value = MAKEWORD(PFaultCon->byIL,PFaultCon->byIH);
        if(Phase ==1)
        {
          g_gRmtMeas[WYcIndex + RM_1A_LOAD_I] = (DWORD)(g_gRunPara[RP_CURRENT_CA]) * M_Value/1000;//()/10000)*g_gRunPara[RP_CURRENT_CA];
        }
        else if(Phase ==2)
        {                
          g_gRmtMeas[WYcIndex + RM_1A_LOAD_I] = (DWORD)(g_gRunPara[RP_CURRENT_CB]) * M_Value/1000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CB];
        }
        else
            g_gRmtMeas[WYcIndex + RM_1A_LOAD_I] = (DWORD)(g_gRunPara[RP_CURRENT_CC]) * M_Value/1000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CC];
      //g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] = (MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10;
            //iFtType =5;//ң�ⳬ��
            //if((g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] > ) &&())
        WFltYxIndex = LPFToYxId(Group + 1,Phase,4);
        WORD wLoad = g_gRmtMeas[WYcIndex + RM_1A_LOAD_I]/10;
        if((GetRIStatus(WFltYxIndex + RI_1A_SHORT) == 0) &&(wLoad >= g_gRunPara[RP_CURRENT_CY]))
           CreatNewSoe(WFltYxIndex + RI_1A_SHORT,1);
        else if((GetRIStatus(WFltYxIndex + RI_1A_SHORT) == 1) &&(wLoad < g_gRunPara[RP_CURRENT_CY]))
           CreatNewSoe(WFltYxIndex + RI_1A_SHORT,0);
        //g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] /= 10;
        //g_gRmtMeas[wYcIndex + RM_1A_FAULT_I] = 0;
        g_ucRefYxYcTs |= BIT1;
        if(PFaultCon->byIMinH & 0x02)//ָʾ�����ٲ⹦�ܣ������ϱ�ң��
          OpenTimer(TM_SENDYC,5);
    }
    //g_ucRefYxYcTs |= BIT1;  
}
BOOL CPrtcRF::RecRf003Fault(VFrame55 *pReceiveFrame)
{
    BYTE phase = 0;
    BYTE group = 0;
    BYTE InfoNum = 0;
    RF003FTDA *pFaultCon;
    WORD wFltYxIndex = 0;//�������
    WORD wFltYxIndex1 = 0;//�������
    WORD wFltYxIndex2 =0;
     WORD wFltYxIndex3 =0;
    const BYTE byFtTypes[FTTPYE_NUM] = {0x08,0X40,0X10,0xff,0xff,0x02,0x80,0x04,0x20};//˲ʱ��·,�ӵ�,ָʾ����ص�ѹ�澯,�¶ȡ��糡�е磬����(�����ö�·����)����·��ң�ط���,ͣ��(ע���״̬Ϊ0x20��ʾ�е磬�е��״̬Ϊ0)
    //const BYTE byYcTypes[YCTPYE_NUM] ={0x04,0X08,0X10,0x20,0x40,0x80};//ͻ�䡢��ʱ���ϵ硢ͣ�硢�糡�½�������ң����Чλ
    //BYTE bHaveFt = 0;//��ʶ�������й�������
    BYTE bLuboFt = 0;//��ʶ�������й�������
    unsigned int absolute_time[RTC_NUM];
    WORD minute_count,Relative_timeL,Relative_timeH;
    DWORD Relative_time;
    WORD wYcIndex =0;
    //WORD m_Value;
  
  //InfoNum = (pReceiveFrame->FrameLen-8)/RF003FTDANUM;
    InfoNum = 1;
    if(pReceiveFrame->RFASDU.Rf003Asdu.NetIP <= 0)
        return false;
    phase = pReceiveFrame->RFASDU.Rf003Asdu.NetIP % 3;
    if(phase == 0) phase = 3;
  if(phase > 0)
  m_wTxIPBit &= ~BIT[phase-1];
    group = (pReceiveFrame->RFASDU.Rf003Asdu.NetIP - 1)/3;
    wYcIndex = group*PHASE_NUM + (phase-1);
    pFaultCon = (RF003FTDA *)&pReceiveFrame->RFASDU.Rf003Asdu.ByConten[0];
    for(int i = 0; i < InfoNum;i++)
    { 
        //�ж�ͨ��״̬
        WORD wComIndex = 0;
        wComIndex = group*PHASE_NUM + (phase-1);
        g_sCommStatCount[wComIndex] = g_gRunPara[RP_ZSQCST_T];
    if((pFaultCon->byIMinH & 0x01)&&(pFaultCon->lubo_type < 4))//¼������Ϊ123ʱ��Ч,4ͬ�鴥��5ң�ش���
    {
      minute_count = MAKEWORD(pFaultCon->minute_countL,pFaultCon->minute_countH);//���Ӽ���
      Relative_timeL =MAKEWORD(pFaultCon->Relative_time[0],pFaultCon->Relative_time[1]);
      Relative_timeH =MAKEWORD(pFaultCon->Relative_time[2],pFaultCon->Relative_time[3]);
      Relative_time = MAKEDWORD(Relative_timeL,Relative_timeH);
      if(Absolute_time_calculated(minute_count,Relative_time,absolute_time))
      {
        if((absolute_time[RTC_MONTH]>0) &&(absolute_time[RTC_DATE]>0))//��Ҫ�������߲�����ʱ����ʱ��û��ʱ�䣬��ʱ�䰴�յ�ǰʱ���
          bLuboFt = 1;
      }
    }
    
    YC_AD(group,phase,pFaultCon,wYcIndex,(BYTE *)byFtTypes);//ң��ɼ�
    Mult_Record_Process(group,pFaultCon,bLuboFt,absolute_time);//��·¼������
    //Multichannel recording processing
        
        /*if(((pFaultCon->byFaultInfo & 0x01) == 0x01) &&((pFaultCon->byIMinL & byYcTypes[2])== byYcTypes[2])&&(g_TimeRF003_flag == OFF))//�ϵ�ң��
        {
          SendRCToRf003(0,9);//�������߶�ʱ
          g_TimeRF003_flag = ON;
          
        }
        else if(((pFaultCon->byFaultInfo & 0x01) == 0x01) &&((pFaultCon->byIMinL & byYcTypes[3])== byYcTypes[3])&&(g_TimeRF003_flag == ON))//ͣ��ң��
        {
          SendRCToRf003(0,10);//�˳����߶�ʱ
          g_TimeRF003_flag = OFF;
          
        }*/
        //ȡ������ң��
        pFaultCon->byFaultInfo = ((pFaultCon->byFaultInfo) ^ 0x20); //��ʾ�е磬״̬Ϊ0  ��Ҫȡ����״̬
        for(int iFtType = 0;iFtType < FTTPYE_NUM;iFtType++ )
        {
            if((byFtTypes[iFtType] != 0)&& ((pFaultCon->byFaultInfo & byFtTypes[iFtType]) == byFtTypes[iFtType]))//&&(g_YkOrderFlag != YES)
            {
                wFltYxIndex = LPFToYxId(group + 1,phase,iFtType);
                if(RI_IN_DAERR == wFltYxIndex)  
                {
                    SetRIStatus(RI_IN_DAERR, 1);
                    continue;
                }
                if(g_gRmtLock[wFltYxIndex + RI_1A_SHORT] == 0)
                {
                    if((wFltYxIndex <=RI_3O_TOTALSHORT)&&(wFltYxIndex >=RI_1A_TOTALSHORT))
            g_gRmtLock[wFltYxIndex + RI_1A_SHORT] =g_gRunPara[RP_SHORTFAULT_RESETTIME];//��·���ϵ������� 
              else
            g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
                    if((Recorder.line_phase[group] == YES) &&((iFtType==0)||(iFtType==5)||(iFtType==6)))//RI_12O_SHORT
                      fault_type = 0x08;//˲ʱ�Զ�·����ʱ,�����Զ�·����ʱ,��·����ʱ�������ӵع����ж�
                if((iFtType == 1) && (g_gSmsPhone[PHONE_ENCTL] & BIT2))  //�����ӵع��ϣ���ѯ���ź�Դ
                    {
                        g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = 10 * g_gRunPara[RP_FLTLK_T];  //�ӵر��ֱ����������ϱ��Ժ����¼�ʱ
                        g_yxChangeflag |= BITF;//��ʶ�б�����Ҫ�ϴ�
                        
                        if(g_sTimer[TM_WAITXINHAO].m_TmCount == 0) //�ź�Դ2�ְ����
                        {
                            g_EnSendToXinhao = ON;
                        }
                        //OpenTimer(TM_WAITXINHAO);
                        //�˴��ݴ�ӵع���
                        g_gGroundBuff[g_unGroundBuffTail++] = wFltYxIndex + RI_1A_SHORT;
                    }
       
                    else
                    {
                        
                    
                      if((bLuboFt) &&(!(g_gRunPara[RP_CFG_KEYH]& (BIT[RP_FAULT_TIME-16]))))//��¼������ʱ��ָʾ���Ż��ϴ����ʱ��
                      {
                  
                        CreatNewSoe_time(wFltYxIndex + RI_1A_SHORT,1,absolute_time);//CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);
                      }
                      else
                        CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);
              
                        
                    }
                    //��ѹ�澯�����⴦��
                    if((iFtType == 2) || (iFtType == 3)|| (iFtType == 8))//�Ƿ��е����ָʾ���ϱ���״̬�ϱ�
                    {
                        g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = 0;
                    }
                   //else if((iFtType == 0) ||(iFtType == 5)) 
                      //bHaveFt = 1;
                //else if(iFtType == 5)
                //bLuboFt =0;
              }
         //if((iFtType == 0) ||(iFtType == 5)||(iFtType == 6)) //˲ʱ ���� ��· ���ǹ��ϣ��������޴���
                      //bHaveFt = 1;
        /*if((g_YkOrderFlag == YES) &&(iFtType ==7))//˫��ң��ָʾ������ʱ�ϴ�˫��ң�ţ���Ӧ123�ֱ�Ϊ1��ABC
        {
           CreatNewSoe(RI_ZD_DYX1 +group*PHASE_NUM+ phase-1,(2),1); 
        }*/
      }
      else//��λ
      {
     
        if(iFtType == 7)//&&(((pFaultCon->byFaultInfo)|0xFB) ==0xFB))//���Ƹ���/˲ʱ���ø���
        {
          //����ָʾ���ĸ����źŸ����ն�
          wFltYxIndex = LPFToYxId(group + 1,phase,5);//����
          wFltYxIndex1= LPFToYxId(group + 1,phase,0);//˲ʱphase
          wFltYxIndex2= LPFToYxId(group + 1,phase,1);//�ӵ�
          wFltYxIndex3= LPFToYxId(group + 1,phase,7);//����
          if((RI_IN_DAERR == wFltYxIndex)  ||(RI_IN_DAERR == wFltYxIndex1)||(RI_IN_DAERR == wFltYxIndex2)||(RI_IN_DAERR == wFltYxIndex3))
          {
            SetRIStatus(RI_IN_DAERR, 1);
              continue;
          }
          if((GetRIStatus(wFltYxIndex + RI_1A_SHORT) == 1) ||(GetRIStatus(wFltYxIndex1 + RI_1A_SHORT) == 1)||(GetRIStatus(wFltYxIndex2 + RI_1A_SHORT) == 1)||(GetRIStatus(wFltYxIndex3 + RI_1A_SHORT) == 1))//�����й��ϲ��ܸ���������Ĺ���
          {
      
            if(GetRIStatus(wFltYxIndex2 + RI_1A_SHORT) == 1)//����ӵ�
            {
              CreatNewSoe(wFltYxIndex2 + RI_1A_SHORT,0);
              g_gRmtLock[wFltYxIndex2 + RI_1A_SHORT] = 0;
              
            }
            if(GetRIStatus(wFltYxIndex3 + RI_1A_SHORT) == 1)//���鷭��
            {
              CreatNewSoe(wFltYxIndex3 + RI_1A_SHORT,0);
              g_gRmtLock[wFltYxIndex3 + RI_1A_SHORT] = 0;
              
            }
      if(g_gRunPara[RP_SET_KEY] == 0)//�����ն˸������ж�·����
            {
    Reset_Short(wFltYxIndex,wFltYxIndex1);    
            }
            else//�յ�һֱָʾ���Ķ�·����͸���ȫ���ź�
            {
            for(BYTE i =1;i<4;i++)
            {
                wFltYxIndex = LPFToYxId(group + 1,i,5);//����phase
                wFltYxIndex1= LPFToYxId(group + 1,i,0);//˲ʱphase
                Reset_Short(wFltYxIndex,wFltYxIndex1);
            }
            }
         }    
       } 
       else if(iFtType == 8)//&&(((pFaultCon->byFaultInfo)|0xDF) ==0xDF))//�е���λ1
       {
        wFltYxIndex = LPFToYxId(group + 1,phase,8);
        if(RI_IN_DAERR == wFltYxIndex)
        {
          SetRIStatus(RI_IN_DAERR, 1);
          continue;
        }
        if(GetRIStatus(wFltYxIndex + RI_1A_SHORT) == 1)
        {
            CreatNewSoe(wFltYxIndex + RI_1A_SHORT,0);    
            g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = 0;
              
        }
      
      }
      else if(iFtType == 2) //���鴦�� ��ʱֻ�����ѹ��������
      {
          wFltYxIndex = LPFToYxId(group + 1,phase,iFtType);
           if(RI_IN_DAERR == wFltYxIndex)  
           {
              SetRIStatus(RI_IN_DAERR, 1);
               continue;
            }
            if(GetRIStatus(wFltYxIndex + RI_1A_SHORT) == 1)
            {
                CreatNewSoe(wFltYxIndex + RI_1A_SHORT,0);
            }
        }
     }
  }
    //m_Value = MAKEWORD(pFaultCon->byIL,pFaultCon->byIH);
    //wYcIndex = group*PHASE_NUM + (phase-1);
    /*if(bLuboFt)
    {
             m_Value = MAKEWORD(pFaultCon->byIL,pFaultCon->byIH);
      if(phase ==1)
          g_gRmtMeas[wYcIndex + RM_1A_LUBO] = (DWORD)(g_gRunPara[RP_CURRENT_CA]) * m_Value/10000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CA];
      else if(phase ==2)
          g_gRmtMeas[wYcIndex + RM_1A_LUBO] = (DWORD)(g_gRunPara[RP_CURRENT_CB]) * m_Value/10000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CB];
      else
          g_gRmtMeas[wYcIndex + RM_1A_LUBO] = (DWORD)(g_gRunPara[RP_CURRENT_CC]) * m_Value/10000;//((MAKEWORD(pFaultCon->byIL,pFaultCon->byIH))/10000)*g_gRunPara[RP_CURRENT_CC];
      g_ucRefYxYcTs |= BIT1;
            g_FltChgSdYc = 1;//����ң��Ӧ�����ϴ�
    }*/        
  
        /*****************************����ָʾ���󱸵�ԴͶ��ң��λ**********************************/
        /**************************************¼�����ݱ�־λ���λ-byIMinH***************************************************/
    wFltYxIndex = group*PHASE_NUM + (phase - 1);
    if(pFaultCon->byIMinH & BIT7)  //�󱸵��Ͷ��
    {
        CreatNewSoe(wFltYxIndex+RI_1A_BATPOW_EN,1);
    }
    else
    {
        CreatNewSoe(wFltYxIndex+RI_1A_BATPOW_EN,0);
    }
      
    #ifdef ZDSENDADDR        //����ָʾ�������¶Ⱥ͵�ص�ѹ
        wYcIndex = group*3 + (phase-1);  //�¶ȵ�ص�ѹ��ʱ��O��
        
        if((pFaultCon->byUH) & 0x80)                    //�¶�
        {
            g_gRmtMeas[wYcIndex + RM_1A_CABLE_T_INT] = MAKEWORD(pFaultCon->byUH,0xff);
        }
        else
        {
            g_gRmtMeas[wYcIndex + RM_1A_CABLE_T_INT] = MAKEWORD(pFaultCon->byUH,0x0);
        }
        g_gRmtMeas[wYcIndex + RN_1A_BAT_U] = MAKEWORD(pFaultCon->byIMinL, pFaultCon->byIMinH);
        //g_gRmtMeas[wYcIndex + RM_1A_BAT_U] /= 10;
        #endif
        pFaultCon ++;     
    }
    if(m_wTxIPBit == 0)//��ʱm_wTxIPBit������0˵����Ϊ�㲥���鲥�����m_wTxIPBit���¸�ֵ������SwitchToAddress�����лὫ��һ��ң�ر������ص�
    {
          if((g_YkOrderFlag == ON) ||(g_YkOrderFlag == YES))
         g_YkOrderFlag = TRUE;// m_byRxFrmTp = M_WFILE_FI;//��������
    }
    return TRUE;
}

BOOL CPrtcRF::RecRf003Para(VFrame55 *pReceiveFrame)
{
  RF003PARA *pFaultCon;
   BYTE fileName;
    BYTE secName=0;
  //BYTE uartNum = 0;
         WORD addr = GetOwnAddr();
  pFaultCon = (RF003PARA *)&pReceiveFrame->RFASDU.rf003para.TYPE2;
  switch(pFaultCon ->TYPE2)
    {
      case 0x00://�㼯��Ԫ���߲���
        fileName =13;
      /*if(g_ZsqCmdRBit & BITC)
          {
              uartNum |= BIT0;
              g_ZsqCmdRBit &= (~BITC);         
          }
          if(g_ZsqCmdRBit & BITD)
          {
              uartNum |= BIT1;
              g_ZsqCmdRBit &= (~BITD);         
          }*/
      break;
     default:
          break;
  }
  
   m_SendBuf.wWritePtr=0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;  //����ʱ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;   //����Ƶ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xF3;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)addr & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)(addr>>8) & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x7D;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0D;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)addr & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)(addr>>8) & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x02;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = fileName;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = secName;
    
    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 5; //���ָʾ�����ȴ����BUG
      
      memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&(pFaultCon ->TYPE2),5);
                         m_SendBuf.wWritePtr += 5;
      m_SendBuf.pBuf[1] = m_SendBuf.wWritePtr - 4;
      m_SendBuf.pBuf[2] = m_SendBuf.pBuf[1];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ChkSum(&m_SendBuf.pBuf[4],m_SendBuf.pBuf[2]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;

      BYTE uartId = ((CBJ101S *)m_RWPaSrcObj)->m_uartId;
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,uartId);
      /*if(uartNum & 0x01)
      {
        CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdGPRS);
      }
      if(uartNum & 0x02)
      {
        CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdDBG);
      }*/
      return TRUE;
}
    
BOOL CPrtcRF::RecRf001Para(VFrame55 *pReceiveFrame)
{
    //BYTE uartNum = 0;
    BYTE group = 0;
    BYTE phase = 0;
  //BYTE PRM = 0;
    //BYTE dwCode = 3;
    //BYTE Num = 1;
    RF001ASDU *pFaultCon;
    pFaultCon = (RF001ASDU *)&pReceiveFrame->RFASDU.Rf003Asdu.NetID;
    WORD addr = GetOwnAddr();
    BYTE fileName;
    BYTE secName;
    BYTE fipara_flag = OFF;
    phase = pReceiveFrame->RFASDU.Rf003Asdu.NetIP % 3;
    if(phase == 0) phase = 3;
    group = (pReceiveFrame->RFASDU.Rf003Asdu.NetIP - 1)/3 + 1;
    secName = (group << 2) + phase;
    WORD wYcIndex = 0;
    wYcIndex = (group-1)*PHASE_NUM + (phase-1);
    /*if((secName & 0x3) == 0x3)
        wYcIndex = (secName & 0xFC)*PHASE_NUM + 3;
    else
        wYcIndex = (secName & 0xFC)*PHASE_NUM + (secName & 0x3 - 1);*/
    
    switch(pFaultCon -> SubType)
    {
    case 0x0E:
        fileName = 26;
        /*if(g_ZsqCmdRBit & BIT0)
        {
            uartNum |= BIT0;
            g_ZsqCmdRBit &= (~BIT0);
        }
        if(g_ZsqCmdRBit & BIT5)
        {
            uartNum |= BIT1;
            g_ZsqCmdRBit &= (~BIT5);
        }*/
        break;
    case 0x07:    //�ٻ�����ֵ���⴦��
        g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] = MAKEWORD(pFaultCon->ByConten[0],pFaultCon->ByConten[1]);
        //g_gRmtMeas[wYcIndex + RM_1A_LOAD_I] /= 10;
        g_gRmtMeas[wYcIndex + RN_1A_EFIELD_U] = MAKEWORD(pFaultCon->ByConten[2],pFaultCon->ByConten[3]);
        g_gRmtMeas[wYcIndex + RN_1A_BAT_U] = MAKEWORD(pFaultCon->ByConten[4],pFaultCon->ByConten[5]);
        g_gRmtMeas[wYcIndex + RN_1A_BAT_U] *=10;
        g_gRmtMeas[wYcIndex + RN_1A_CAP_U] = MAKEWORD(pFaultCon->ByConten[6],pFaultCon->ByConten[7]);
        g_gRmtMeas[wYcIndex + RN_1A_CAP_U] =  g_gRmtMeas[wYcIndex + RN_1A_CAP_U]*50;
        g_gRmtMeas[wYcIndex + RN_1A_CAP_U]  = g_gRmtMeas[wYcIndex + RN_1A_CAP_U]  >>12;
        g_gRmtMeas[wYcIndex + RN_1A_CAP_U] *=100;
        //g_gRmtMeas[wYcIndex + RM_1A_FAULT_I] = 0;
        g_ucRefYxYcTs |= BIT1;
        OpenTimer(TM_SENDYC,10);
        break;
    case 0x0F:
        fileName = 27;
        /*if(g_ZsqCmdRBit & BIT1)
        {
            uartNum |= BIT0;
            g_ZsqCmdRBit &= (~BIT1);        
        }
        if(g_ZsqCmdRBit & BIT6)
        {
            uartNum |= BIT1;
            g_ZsqCmdRBit &= (~BIT6);        
        } */     
        break;
    case 0x05:
        fileName = 28;
        /*if(g_ZsqCmdRBit & BIT2)
        {
            uartNum |= BIT0;
            g_ZsqCmdRBit &= (~BIT2);         
        }
        if(g_ZsqCmdRBit & BIT7)
        {
            uartNum |= BIT1;
            g_ZsqCmdRBit &= (~BIT7);         
        } */     
        break;
  case 0x90:
  {

    fileName = 30;
    if(g_RWFiInf.PaWaitflag_fi  == ON)
      fipara_flag = ON;
    else
      fipara_flag = TRUE;
    g_RWFiInf.PaWaitflag_fi  = OFF;
        g_RWFiInf.PaWaitCt_fi = 0;
        g_RWFiInf.TxNum_fi = 0;
    /*if(g_ZsqCmdRBit & BIT8)
        {
            uartNum |= BIT0;
            g_ZsqCmdRBit &= (~BIT8);         
        }
        if(g_ZsqCmdRBit & BIT9)
        {
            uartNum |= BIT1;
            g_ZsqCmdRBit &= (~BIT9);         
        } */
        //uartNum |= BIT1;
  }
    
    default:
        break;
    }
    //if(uartNum == 0)
        //return false;
  if(fipara_flag == ON)
  {
    //for g_RWFiInf.Fi_Info_Num
    //if(g_Cmid == g_CmIdGPRS)
         //{
            //((CBJ101S *)pGprs)->SendFrameHead(102,5);
                
            //((CBJ101S *)pGprs)->write_infoadd(g_RWFiInf.Fi_Info);
      
          //{
             //case 0x5008://��·ͻ�����A
           WORD YC = (MAKEWORD(pFaultCon->ByConten[2],pFaultCon->ByConten[3]))*10;
          g_RWFiInf.Fi_InfoPara[(0x5008 - 0x5001)*2] = YC;//pFaultCon->ByConten[2];
            g_RWFiInf.Fi_InfoPara[(0x5008 - 0x5001)*2+1] = YC>>8;//pFaultCon->ByConten[3];
          
        // case 0x5009://��·�������ʶ��ʱ��ms
          g_RWFiInf.Fi_InfoPara[(0x5009 - 0x5001)*2] = pFaultCon->ByConten[4];
            g_RWFiInf.Fi_InfoPara[(0x5009 - 0x5001)*2+1] = pFaultCon->ByConten[5];
          
         //�糡�½�����
          WORD v_flag = (MAKEWORD(pFaultCon->ByConten[6],pFaultCon->ByConten[7]))*10;  
          g_RWFiInf.Fi_InfoPara[(0x500B - 0x5001)*2]  = v_flag;//pFaultCon->ByConten[6];
            g_RWFiInf.Fi_InfoPara[(0x500B - 0x5001)*2+1]= v_flag>>8;//pFaultCon->ByConten[7];
          
         //��λ��ʽ(�Ƿ������ϵ㸴��0 ������ 1 ����)
          g_RWFiInf.Fi_InfoPara[(0x500D - 0x5001)*2]  = pFaultCon->ByConten[8];
            g_RWFiInf.Fi_InfoPara[(0x500D - 0x5001)*2+1] = pFaultCon->ByConten[9];
          
         //�Զ���ʱ��λʱ��min
          g_RWFiInf.Fi_InfoPara[(0x500E - 0x5001)*2]  = pFaultCon->ByConten[10];
            g_RWFiInf.Fi_InfoPara[(0x500E - 0x5001)*2+1] = pFaultCon->ByConten[11];
          
          //�ϵ縴λʱ��S
          g_RWFiInf.Fi_InfoPara[(0x500F - 0x5001)*2]  = pFaultCon->ByConten[12];
            g_RWFiInf.Fi_InfoPara[(0x500F - 0x5001)*2+1] = pFaultCon->ByConten[13];
          
          //��ص͵�ѹ��ֵmV
          g_RWFiInf.Fi_InfoPara[(0x5011 - 0x5001)*2]  = pFaultCon->ByConten[14];
            g_RWFiInf.Fi_InfoPara[(0x5011 - 0x5001)*2+1] = pFaultCon->ByConten[15];
          
          //�����������㼯��Ԫʱ����
            WORD Inter_time = (MAKEWORD(pFaultCon->ByConten[16],pFaultCon->ByConten[17]))*60;
          g_RWFiInf.Fi_InfoPara[(0x5014 - 0x5001)*2] = Inter_time;//pFaultCon->ByConten[16];
            g_RWFiInf.Fi_InfoPara[(0x5014 - 0x5001)*2+1] = Inter_time >> 8;//pFaultCon->ByConten[17];
      if(g_RWFiInf.Para_guiyue ==ON)
      {
        /*if(uartNum & 0x01)
        ((CBJ101S *)pGprs)->SendReadPaDis();
        else if(uartNum & 0x02)
        ((CBJ101S *)pDbg)->SendReadPaDis();*/
       ((CBJ101S *)m_RWPaSrcObj)->SendReadPaDis();
        g_RWFiInf.Para_guiyue = OFF;
      }
      else
      {
        if(g_RWFiInf.Fi_Info_Num <=2)
            {
             /* if(uartNum & 0x01)
            
              ((CBJ101S *)pGprs)->RecReadParaData(102,0);
              else if(uartNum & 0x02)
              ((CBJ101S *)pDbg)->RecReadParaData(102,0);*/
              ((CBJ101S *)m_RWPaSrcObj)->RecReadParaData(102,0);
              
            }
            else  
            {
              /*if(uartNum & 0x01)
            
                    ((CBJ101S *)pGprs)->RecReadParaData(132,0x80);
              else if(uartNum & 0x02)
              ((CBJ101S *)pDbg)->RecReadParaData(132,0x80);*/
              ((CBJ101S *)m_RWPaSrcObj)->RecReadParaData(132,0x80);
            }
      }
      
    }
          
          
      
           /* ((CBJ101S *)pGprs)->SendFrameTail(PRM, dwCode, Num);
        
          }
      else if(g_Cmid == g_CmIdDBG)
          {
          ((CBJ101S *)pDbg)->SendFrameHead(102,5);
                
              ((CBJ101S *)pDbg)->write_infoadd(g_RWFiInf.Fi_Info);
        switch(g_RWFiInf.Fi_Info)
            {
             case 0x5008://��·ͻ�����A
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[0];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[1];
          break;
         case 0x5009://��·�������ʶ��ʱ��ms
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[2];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[3];
          break;
          case 0x500B://�糡�½�����
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[4];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[5];
          break;
          case 0x500D://��λ��ʽ(�Ƿ������ϵ㸴��0 ������ 1 ����)
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[6];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[7];
          break;
          case 0x500E://�Զ���ʱ��λʱ��min
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[8];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[9];
          break;
           case 0x500F://�ϵ縴λʱ��S
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[10];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[11];
          break;
          case 0x5011://��ص͵�ѹ��ֵmV
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[12];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[13];
          break;
          case 0x5014://�����������㼯��Ԫʱ����
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[14];
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[15];
          break;
        }
            ((CBJ101S *)pDbg)->SendFrameTail(PRM, dwCode, Num);
          
          }

      
  }*/
    else
    {
      
      
      m_SendBuf.wWritePtr=0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;    //����ʱ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;     //����Ƶ��
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0xF3;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)addr & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)(addr>>8) & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x7D;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0D;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)addr & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (BYTE)(addr>>8) & 0xff;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x02;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = fileName;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = secName;
    //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
    //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->SubType2;
    //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
      if(fipara_flag == TRUE)
      {
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (pFaultCon->ByContenLen[1] + 2)*2; //���ָʾ�����ȴ����BUG
        //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
        for(BYTE i = 0; i <  (pFaultCon->ByContenLen[1] + 2)*2; i++)
        {
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[i+2];
        }
      }
    else
    {
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByContenLen[1] + 2; //���ָʾ�����ȴ����BUG
        //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x0;
        for(BYTE i = 0; i <  pFaultCon->ByContenLen[1] + 2; i++)
        {
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = pFaultCon->ByConten[i];
        }
    }
      m_SendBuf.pBuf[1] = m_SendBuf.wWritePtr - 4;
      m_SendBuf.pBuf[2] = m_SendBuf.pBuf[1];
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ChkSum(&m_SendBuf.pBuf[4],m_SendBuf.pBuf[2]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x16;
       BYTE uartId = ((CBJ101S *)m_RWPaSrcObj)->m_uartId;
       CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,uartId);
     /* if(uartNum & 0x01)
      {
          CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdGPRS);
      }
      if(uartNum & 0x02)
      {
          CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdDBG);
      }*/
    }
  
    return true;
}
 
#endif



