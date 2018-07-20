/*------------------------------------------------------------------------
 Module:      protocol.cpp
 Author:      linxueqin
 Project:     ��Լƽ̨
 State:       V1.0
 Creation Date:   2014-4-28
 Description:   ��Լ����Ķ���
------------------------------------------------------------------------*/

#include "Protocol.h"
#include "..\Main.h"


/***************************************************************
  Function:CProtocol
    ��Լ���๹�캯��
  ��������
  ���أ���
***************************************************************/
CProtocol::CProtocol()    
{
  memset(&m_SendBuf, 0, sizeof(m_SendBuf));   
    memset(&m_RecBuf, 0, sizeof(m_RecBuf));      
    memset(&m_RecFrame, 0, sizeof(m_RecFrame));  
}

/***************************************************************
  Function:Init
      ��Լƽ̨��ʼ��
  ��������
  ���أ�TRUE �ɹ���FALSE ʧ��
***************************************************************/
BOOL CProtocol::Init(WORD wMinEqpNum)
{  
  m_SendBuf.wBufSize = MAX_SEND_LEN;
  m_RecBuf.wBufSize = MAX_RECV_LEN;

  
        
  m_SendBuf.pBuf = m_SBuf;
  m_RecBuf.pBuf = m_RBuf;
  m_wSendPaNum = 0;
  Recorder.Recorder_flag= OFF;
  //Recorder.Recorder_flagall =OFF;
  Recorder.Recorder_flag1 = OFF;
  Recorder.Recorder_flag2 = OFF;
  Recorder.Recorder_Allflag= OFF;
  //Recorder.FRecorder_Current_COUNT=0;
  //Recorder.Recorder_Total_COUNT =0;
  for(int i=0;i<6;i++)
  {
    Recorder.line_phase[i] = NO;
    Recorder.Recorder_reflag[i]= OFF;
  }
  return TRUE;  
}

/***************************************************************
  Function:GetOwnAddr
    ��ȡ�����ַ
  ��������
  
  ���أ������ַ
***************************************************************/
WORD CProtocol::GetOwnAddr(void)      
{
  return  g_gRunPara[RP_COMM_ADDR];
}
/***************************************************************
  Function:NeatenCommBuf
    ͨ�Ż������Ĺ���
  ������pCommIO
    pCommIO��ָ��ͨ�Ż�������ָ��
  ���أ���
***************************************************************/
void CProtocol::NeatenCommBuf(VCommBuf *pCommBuf)
{
 // register unsigned  int i,j;

  if (pCommBuf->wReadPtr == 0)
  {
    return ; //��ָ���Ѿ�Ϊ0
  }


  if (pCommBuf->wReadPtr >= pCommBuf->wWritePtr)
  {
    pCommBuf->wReadPtr = 0;
    pCommBuf->wWritePtr=0;
    return ;
  }


  if (pCommBuf->wWritePtr >= pCommBuf->wBufSize)
  {
    pCommBuf->wReadPtr = 0;
    pCommBuf->wWritePtr = 0;
    return ;
  }
    pCommBuf->wReadPtr = 0; 
    pCommBuf->wWritePtr = 0;
    
    
//  i = 0; 
//  j = pCommBuf->wReadPtr;
//  while (j < pCommBuf->wWritePtr)
//  {
//    pCommBuf->pBuf[i++] = pCommBuf->pBuf[j++];
//  }
//
//  pCommBuf->wReadPtr = 0; 
//  pCommBuf->wWritePtr = i; 
}

/***************************************************************
  Function:SearchFrame
    ������Ϣ֡
  ��������
  
  ���أ�TRUE �ҵ���ȷ��һ֡���ģ�FALSE δ�ҵ�һ֡��ȷ�ı���
***************************************************************/
BOOL CProtocol::SearchFrame(void)
{
  DWORD Rc;
  DWORD Len;//�Ѵ�������ֽ���
  short MsgLen;

  while (1)
  {
    MsgLen = m_RecBuf.wWritePtr - m_RecBuf.wReadPtr;
    if (MsgLen <= 0)
    {
      return FALSE;
    }

    Rc = SearchOneFrame(&m_RecBuf.pBuf[m_RecBuf.wReadPtr], MsgLen);
    /*if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF])//����ʹ��
             {//�����˽�RF�յ�������ת�������Կ�
                MyPrintfD(&(pReceiveFrame->Frame55.Start1),pReceiveFrame->Frame55.FrameLen);
             }*/
    Len = Rc & ~FRAME_SHIELD; //�Ѵ�������ֽ���

    switch  (Rc & FRAME_SHIELD)
    {
      case FRAME_OK:
        DisableRetry();
        m_RecFrame.pBuf = &m_RecBuf.pBuf[m_RecBuf.wReadPtr];  //��¼��Ч���ĵ���ʼ��ַ
        m_RecFrame.wWritePtr = (WORD)Len; //��¼��Ч���ĵĳ���
        m_RecBuf.wReadPtr += (WORD)Len; //ָ���Ƶ���һ���Ĵ�
        return TRUE;

      case FRAME_ERR:
        if (!Len)
        {
          m_RecBuf.wReadPtr++;
        }
        else
        {
          m_RecBuf.wReadPtr += Len; //ָ���Ƶ���һ���Ĵ�
        }
        break;

      case FRAME_LESS:
        m_RecBuf.wReadPtr++;  //����һ���ֽڼ���������ȷ���� lxq
        break;
        //m_RecBuf.wReadPtr += (WORD)Len; //ָ���Ƶ���һ���Ĵ�
        //return FALSE;
      default:
        break;//??do what
    }//switch
  }//while
}

/***************************************************************
  Function:SearchHead
    ���������е�һ���ؼ���
  ������Buf, Len,��Offset1, Key1
    Buf ���Ļ�����ͷָ��
    Len Ҫ�����ı��ĳ���
    Offset1 �ؼ��ֵ��ڱ����е�ƫ��λ��
    Key1 �ؼ���
  ���أ��ؼ����ڱ��Ļ������е�ƫ��λ��
***************************************************************/
WORD CProtocol::SearchHead(BYTE *Buf, WORD Len,short Offset1,BYTE Key1)
{
  WORD i;
  for(i=0;i<(short)(Len-Offset1);i++)
  {
    if(Buf[Offset1+i]==Key1)
      return i;
  }

  return i;
}
/***************************************************************
  Function: RcvData
    ��Լ�������ݲ�����
    ��socket�ж�����ʱ�˺������أ����غ�socket�������������������ķ�ʽͣ����select��������
  ��������

  ���أ���
***************************************************************/
int CProtocol::RcvData(int* iSocket)
{
    int rxlen=0;
    int nLen=1024;    
    //while(1)//Ҫ��Ҫ�����ձ���? lxq
    {
      NeatenCommBuf(&m_RecBuf);
      nLen = m_RecBuf.wBufSize-m_RecBuf.wWritePtr;
      rxlen = CommRcvData((BYTE *)(m_RecBuf.pBuf+m_RecBuf.wWritePtr),nLen,*iSocket);
      
      if(rxlen > 0)
      {
          m_RecBuf.wWritePtr += (WORD)rxlen;
          if (m_RecBuf.wWritePtr > m_RecBuf.wBufSize)
          {
             m_RecBuf.wWritePtr = 0;
          }
          
          #ifdef RF_TEST
            if((*iSocket == g_CmIdRF)||(*iSocket == g_CmIdGps))
            {//��RF���յ�������ת����ȥ
              CommSendData(m_RecBuf.pBuf,rxlen,g_CmIdDBG);//g_CmIdEX
            }
          #endif
          DoReceive();
       }
      else if(rxlen <= 0)//recv=0 �Զ˷����Ͽ�����
      {
         return 0;
      }
   }
  return 0;
}

int CProtocol::CommWrite( char* pbuf, int buflen, DWORD flags)
{
  CommSendData((BYTE*)pbuf,buflen,m_uartId);

  return 1;
}


/***************************************************************
  Function: IsOrderFeedBack
    �ж�DPRAM����������Ƿ���ȷ
  ������RamBase dpram��ַ
    buf  ƫ����

  ���أ�1 ������ȷ�� 0 ����
***************************************************************/
//  WORD CProtocol::IsOrderFeedBack(DWORD RamBase,WORD wOffset,WORD wYTorYK)
//  {
//    WORD wRt=0;
//    int i=0;
//    OrderFeedBack stuFb;
//    for( i = 0; i<500;i++)
//    {
//      delayms(600);
//      //ReadAndClearDPRam16(RamBase,wOffset/2,sizeof(OrderFeedBack)/2,(WORD *)&stuFb);
//      if((stuFb.wSign == wYTorYK) && (stuFb.wStatus ==  0x5A5A))
//      {
//        wRt = 1;
//        break;
//      }
//      else
//        wRt = 0;
//    }
//    //printf("i=%d\n",i);
//    return wRt;
//  }



//У���
BYTE CProtocol::ChkSum(BYTE *buf, WORD len)
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
//������ѯ�����ò��� + Զ������====================================================================================
//�Զ��屨��
BOOL CProtocol::RecFrame69(void)
{
  pReceiveFrame = (VParaFrame*)m_RecFrame.pBuf;

    switch(pReceiveFrame->Frame69.Type)
    {
        case 0x7a://���ļ�
            RecReadFile();
            break;
        case 0x7d://д�ļ�
            RecWriteFile();
            break;
        case 200:
              g_Cmid = m_uartId;
              Code_Load(&pReceiveFrame->Frame69.Start1,m_SendBuf.pBuf);
              break;
        default:
            break;
    }
    return TRUE;
}
//��������������
BOOL CProtocol::RecReadFile(void)
{
    BYTE *pData;
    WORD wFile;
    BYTE bySec;//����
    BYTE bySCQ;//�޶���
    WORD wStartNo;//��ʼ���
    WORD wSOENum;//��ȡ��SOE����
    WORD wLoadNum;//��ȡ�ĸ�����������(ÿ����ʱ��Ϊ��λ)

    pReceiveFrame = (VParaFrame*)m_RecFrame.pBuf;//���������ԭ���ǣ�����68������ת���˺�����CProtocol�е�pReceiveFrameָ��δ��ֵ
    pData = &pReceiveFrame->Frame69.Data[0];

    wFile = MAKEWORD(pData[0],pData[1]);
    pData +=2;
    bySec = *pData++;
    bySCQ = *pData++;
    m_wSendPaNum = 0;//��֡�����ϴ��ǿ��ݹ���ã����Ե��յ�������ָ��ʱӦ�ø��˱�����0
    if(bySCQ == 5 )
     {
        SendWrPaAck(wFile,bySec);//ѡ��д����
        return 1;
     }
    switch(wFile)
    {
      case 1:
        break;
      case 2:
          wStartNo = MAKEWORD(pData[0],pData[1]);
          pData +=2;
          wSOENum = MAKEWORD(pData[0],pData[1]);
          pData +=2;
          SendSavedSoe(bySec,wStartNo,wSOENum);
           break;
      case 0x0102:
        if(bySCQ == 6)
        {
          #ifdef GetSoeFromRam //�����˱���soe���ڲ�flash�ű����±�3������
            SendSoeSavedStu();//��SOE״̬
          #endif
        }
        break;
      case 3:
        if(bySCQ == 6)
        {//������վ����
          SendTrigAck();
        }
        break;
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 15:
      case 16:
      case 31:
      case 32:
        m_wSendPaNum = 0;
        SendReadPa(wFile,bySec);//���ն��������
        break;
      //case 9://��ODU����
        //if(pOdu != null) ((CPrtcOdu *)pOdu)->SendRCmdToOdu(wFile,bySec,this);
        //break;
      //case 10: //��ODU1���� 
       // if(pOdu1 != null)  ((CPrtcOdu *)pOdu1)->SendRCmdToOdu(wFile,bySec,this);
        //break;
      case 12: //��GPS���� 
        if(pGps != null)  ((CPrtcGPS *)pGps)->SendRCmdToGPR(wFile,bySec,this);//RecFrameGPR(wFile,bySec,this);
        break;
    case 13://������ģ��Ĳ���
      if(pRf!= null)((CPrtcRF*)pRf)->SendRCmdToRf003(this);
    break;
    case 20:
          wStartNo = MAKEWORD(pData[0],pData[1]);
          pData +=2;
          wLoadNum = MAKEWORD(pData[0],pData[1]);
          pData +=2;
          SendLoadData(bySec,wStartNo,wLoadNum,bySCQ);
           break;
      case 26:   //����=��ƫ��ƫ
      case 27:         
      case 28:
      case 29:
    case 30:
          if(pRf!= null) ((CPrtcRF*)pRf)->SendRCmdToRf001(bySec,wFile,this);
          break;
      case 11:
          if(pGprs!= null) ((CPrtcSms*)pGprs)->SendRCmdToIHD(bySec,wFile,this);
          break;
      default:
      break; 
    }
    return TRUE;
}

//����д��������
BOOL CProtocol::RecWriteFile(void)
{
    BYTE *pData;
    WORD wFile;
    BYTE bySec;//����
    WORD wSecLen;//�γ���
    WORD i = 0,j = 0;
    WORD wPaStartId = 0;
    WORD wInfoAddr = 0;
    //WORD wOldValBk;
    WORD wOldCfgKeyBk;
  WORD wOldCfgKeyHBk;
    //BYTE bLine;
    pReceiveFrame = (VParaFrame*)m_RecFrame.pBuf;//���������ԭ���ǣ�����68������ת���˺�����CProtocol�е�pReceiveFrameָ��δ��ֵ
    
    wInfoAddr = MAKEWORD(pReceiveFrame->Frame69.InfoAddr[0],pReceiveFrame->Frame69.InfoAddr[1]);
    wPaStartId = wInfoAddr - 0x6902;

    pData = &pReceiveFrame->Frame69.Data[0];
    wFile = MAKEWORD(pData[0],pData[1]);
    pData += 2;
    bySec = *pData++;
    wSecLen= *pData++;//�ڳ���
    
    if(wFile >= 4 && wFile <= 8)
    {
        SendWrPaSuc();
    }
    else if(wFile == 0x81 || wFile == 0x82)
    {
      SendWrPaSuc();//SendBaseFrame(0,0);  
    }
    switch(wFile)
    {
      case 0x81://����GPRSΪ����ģʽ
        GPRS_SETA_L();
        break;
      case 0x82:
        ExitGprsSet();
        break;
      case 4:
        if(bySec == 0 )//д���в���
        {
          for(i = wPaStartId,j = 0; j < wSecLen/2 && i < RUN_PARA_NUM-1;i++,j++)
          {//b
              if((i == RP_POWER_MODE) &&(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL))
              {//ODU�͵���RF�ĵ�Դģʽ�Զ��л����������в�����ȡ
                pData += 2;
              }
              else
              {//a
                #ifdef ODU
                  if(i == RP_YXYCMODE) 
                    wOldValBk = g_gRunPara[i];
                #endif
                  if(i == RP_CFG_KEY)
                    wOldCfgKeyBk = g_gRunPara[i];
      if(i == RP_CFG_KEYH)
                   wOldCfgKeyHBk = g_gRunPara[i];
                  //������ֵ
                  g_gRunPara[i] = MAKEWORD(pData[0],pData[1]);
                  pData += 2;
                  
                  //�����·�ϲ�һ·�Ĳ����Ƿ��б仯
        #ifdef ODU
                  if(wOldValBk != g_gRunPara[RP_YXYCMODE])
                  {
                      for(bLine = 0;bLine < 12;bLine++)
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
                  }
        #endif
                  //������ÿ������жԵ�ģʽ�Ƿ�����õ�ͣ�ã����������ң��ң���ڴ�
                  if(((wOldCfgKeyBk & BIT[RPCFG_ISDDMODE]) != 0) 
                      && ((g_gRunPara[RP_CFG_KEY]& BIT[RPCFG_ISDDMODE]) == 0))
                  {
                    unsigned int i;
                    for(i = 0; i < RMT_WORD_NUM; i++)
                    {
                        g_gRmtInfo[i] = 0;
                    }
                    for(i = 0; i < RMT_MEAS_NUM; i++)
                    {
                      g_gRmtMeas[i] = 0;//INITYC;
                    } 
                    InitTsYxInfo();
                    InitTsYcInfo();
                  }
                  if(((wOldCfgKeyBk & BIT[RP_DEBUG_RATE]) != (g_gRunPara[RP_CFG_KEY]& BIT[RP_DEBUG_RATE])) &&(i == RP_CFG_KEY))
                   
                     g_debug_rate = ON;//Uart3Init();
                   if(((wOldCfgKeyHBk & BIT[RP_START_TIME-16]) != (g_gRunPara[RP_CFG_KEYH]& (BIT[RP_START_TIME-16]))) &&(i == RP_CFG_KEYH))
                   
                     g_rtc_time = ON;//Uart3Init();
      if(((wOldCfgKeyHBk & BIT[RP_OFF_GPS-16]) != (g_gRunPara[RP_CFG_KEYH]& (BIT[RP_OFF_GPS-16]))) &&(i == RP_CFG_KEYH))

      g_off_gps = ON;
                  
                   
             }//a
        }//b
        if(g_gRunPara[RP_COMM_ADDR] == 0)//��Ϊ��λ���ķ�ʽ�������վ��ַ���ó�0����λ
        {
            RstRunPara();  //���в�����λ
        } 
        g_ucParaChang |= BIT0;
        SetEqpInfo();
      }
      break;
      case 5:
        if(bySec == 0 )//дIEC����
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < IEC_PARA_NUM-1;i++,j++)
          {
            g_ucPara101[i] = *pData++;
          }
          if(g_ucPara101[0] == 0)//��Ϊ��λ���ķ�ʽ�������һ��������Ϊ0�����������ַ�ʽ��λ���
          {
              RstIEC101Para();  //IEC�������λ
          } 
          initpara();//����ͨ���ڴ����
          g_ucParaChang |= BIT1;
        }
        break;  
      case 6:
        if(bySec == 0 )//дң�ŵ��
        {
          for(i = wPaStartId,j = 0; j < wSecLen/2 && i < IEC_YX_NUM;i++,j++)
          {
            g_ucYXAddr[i] = MAKEWORD(pData[0],pData[1]);
            pData += 2;
          }
          if(g_ucYXAddr[0] == 0)//��Ϊ��λ���ķ�ʽ�������һ��������Ϊ0�����������ַ�ʽ��λ���
          {
       RstIEC101YxAddr();  //ң�ŵ�ַ��λ
          }
          InitTsYxInfo();  
          SetEqpInfo();
          g_ucParaChang |= BIT2; 
        }
        
        break;
      case 7:
        if(bySec == 0 )//дң����
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < IEC_YC_NUM;i++,j++)
          {
            g_ucYCAddr[i] = *pData++;
          }
          if(g_ucYCAddr[0] == 0)//��Ϊ��λ���ķ�ʽ�������һ��������Ϊ0�����������ַ�ʽ��λ���
          {
              RstIEC101YcAddr();  //ң���ַ��λ
          } 
          InitTsYcInfo();
          SetEqpInfo();
          g_ucParaChang |= BIT3;
        }
      break;
      case 8:
        if(bySec == 0 )//дң�ص��
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < IEC_YK_NUM;i++,j++)
          {
            g_ucYKPa[i] = *pData++;
          }
          if(g_ucYKPa[0] == 255)//��Ϊ��λ���ķ�ʽ�������һ��������Ϊ255�����������ַ�ʽ��λ���
          {
              RstIEC101YkAddr();  //ң�ص�ַ��λ
          } 
          g_ucParaChang |= BIT4;
        }
      break;
      //case 9://дODU����
        //if(pOdu != null) ((CPrtcOdu *)pOdu)->SendWCmdToOdu(wFile,bySec,pData-1,this);
          //break;
      //case 10://дODU1����
          //if(pOdu1 != null) ((CPrtcOdu *)pOdu1)->SendWCmdToOdu(wFile,bySec,pData-1,this);
          //break;
      case 11:  //дGPRS����
          if(pGprs != null) ((CPrtcSms *)pGprs)->SendWCmdToIHD(0,bySec,pData-1,this);
          break;
   case 13:
     g_WFile = wFile;
         g_Sec = bySec;
    if(pRf != null) ((CPrtcRF *)pRf)->SendWCmdToRf003(wFile,pData-1,this);
    break;
      case 16:
        if(bySec == 0 )//д�绰�����
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < PHONE_PA_NUM - 1;i++,j++)
          {
            g_gSmsPhone[i] = *pData++;
          }
          if(g_gSmsPhone[PHONE1] == 'R')//��Ϊ��λ���ķ�ʽ�������һ���绰����ĵ�һ���ַ�����ΪR�����������ַ�ʽ��λ���
          {
              RstSmsPhoneInfo();
          } 
          g_ucParaChang |= BIT5;
          SendWrPaSuc();
        }
      break;
      case 26:   //����=��ƫ��ƫ
      
      case 27:       
      case 28:
      case 30:
          pData--;
          g_WFile = wFile;
          g_Sec = bySec;
          if(pRf != null) ((CPrtcRF *)pRf)->SendWCmdToRf001(bySec,wFile,pData,this);
          g_WPaOrderFlag = ON;
          break;
      //case 0x29:
      //((CPrtcRF *)pRf)->SendWCmdToRf001(bySec,DISCONNECT);    
      //   break;
      case 31:
        if(bySec == 0 )//ң�ŶԵ�
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < RMT_INFO_NUM;i++,j++)
          {
            BYTE bValCur = GetRIStatus(g_ucYXAddr[i]-1);
            BYTE bValnew = *pData++;
            
            if(bValnew > 3) bValnew = 1;
            if(bValCur != bValnew)
              CreatNewSoe(g_ucYXAddr[i]-1,bValnew,2);
          }
          SetYxTrans();
          SendWrPaSuc();
        }
        break;
        case 32:
        if(bySec == 0 )//ң��Ե�
        {
          for(i = wPaStartId,j = 0; j < wSecLen/2 && i < RMT_MEAS_NUM;i++,j++)
          {
            WORD wYcVal = MAKEWORD(pData[0],pData[1]);
            pData += 2;
            
            g_gRmtMeas[g_ucYCAddr[i]-1] = wYcVal;
            g_ucRefYxYcTs |= BIT1;
          }
          SetYcTrans();
          SendWrPaSuc();
        }
        break;
      default:
      break;    
    }
    
 return TRUE;
}
void CProtocol::SendWrPaSuc()
{
    BYTE Style = 125;
    BYTE Reason = 0X0D;
    SendFrameHeadForPa(Style, Reason);
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],6);
    m_SendBuf.wWritePtr += 6;//��Ϣ���ַ2+�ļ���2+����1+�ڳ���1
    SendFrameTailForPa();
    return;

}

void CProtocol::SendReadPa(WORD FileName,BYTE SecName)
{
    BYTE Style = 125;
    BYTE Reason = 0X0D;
    WORD wSecLenPtr;
    WORD wPaTotalNum = 0;
    WORD wPaSendNum = 0;
    WORD wInfoAddr = 0;
    BYTE leng[15];
  BYTE content_position=0;
    //BYTE flag;
    SendFrameHeadForPa(Style, Reason);
    if(FileName==9 ||FileName==10 || FileName ==11 || FileName ==12)
    {
      wInfoAddr = 0x6902;
      m_wSendPaNum = 0;
    }
    else
    {
      wInfoAddr = MAKEWORD(pReceiveFrame->Frame69.InfoAddr[0],pReceiveFrame->Frame69.InfoAddr[1]);
      wInfoAddr += m_wSendPaNum;
    }
    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(wInfoAddr);//д��Ϣ���ַ
    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(wInfoAddr);
    
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(FileName);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(FileName);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = SecName;
    
    wSecLenPtr = m_SendBuf.wWritePtr++;
    WORD i = 0;
    switch(FileName)
    {
      case 4://�����в���
        wPaTotalNum = RUN_PARA_NUM-1;
        for(i = 0;i < RUN_PARA_NUM-1;i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_gRunPara[i]);
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_gRunPara[i]);
        } 
        break;
     case 5://��IEC����
       wPaTotalNum = IEC_PARA_NUM-1;
       for(i = 0;i < IEC_PARA_NUM-1;i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_ucPara101[i];
        } 
       break;
     case 6://��ң�ŵ��
        wPaTotalNum = g_ucYxTransNum;//IEC_YX_NUM;
        for(i = m_wSendPaNum;(i < g_ucYxTransNum) && (m_SendBuf.wWritePtr < 250);i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_ucYXAddr[i]);
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_ucYXAddr[i]);
        }
        break;
     case 7://��ң����
        wPaTotalNum = g_ucYcTransNum;
        for(i = 0;i < g_ucYcTransNum;i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_ucYCAddr[i];
        } 
        break;
      
      case 8://��ң�ص��
         wPaTotalNum = IEC_YK_NUM;
         for(i = 0;i < IEC_YK_NUM;i++,wPaSendNum++)
         {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_ucYKPa[i];
         }
         break;
      case 9://��ODU����
          //if(pOdu == null) break;
           //wPaTotalNum = (((CPrtcOdu *)pOdu)->pReceiveFrame)->Frame67.Data[6];
           //for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
           //{
             // m_SendBuf.pBuf[m_SendBuf.wWritePtr++] =(((CPrtcOdu *)pOdu)->pReceiveFrame)->Frame67.Data[7+i];
           //}
           break;
    case 10://��ODU����
           //if(pOdu1 == null) break;
           //wPaTotalNum = (((CPrtcOdu *)pOdu1)->pReceiveFrame)->Frame67.Data[6];
           //for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
           //{
              //m_SendBuf.pBuf[m_SendBuf.wWritePtr++] =(((CPrtcOdu *)pOdu1)->pReceiveFrame)->Frame67.Data[7+i];
           //}
           break;
    case 11:
        switch(SecName)
        {
        case 0://IP�Ͷ˿�
        case 1://���Ź���
        case 2://APN
        case 3://�û���
        case 4://����
            if(pGprs == null) break;
            wPaTotalNum = (((CBJ101S *)pGprs)->pReceiveFrame)->FrameAA.byContent[3];
            for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
            {
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->FrameAA.byContent[i + 4];
            }
            break;
        case 0x0f:
            if(pGprs == null) break;
            wPaTotalNum = 8;//���⴦��ֻȡ����״̬��
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[0] ^ 1;  //ģ��״̬
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[1] ^ 1;  //SIM��״̬
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[9];  //CSQֵ
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[10]; //Ƶ��
            for(i = 0; i < 4; i++)                                 //IP
            {
                m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[15 + i];
            }
            wPaSendNum = 8;            
            break;
        default:
            break;
        }       
        break;
     case 12://GPS����
        switch(SecName)
        {
        case 0:
       
            if(pGps == null) break;
            if((((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.FrameLen >= 20)//13,+6GPRMC
            {
        
        for(i =0;i<15;i++)
        {
          leng[i]=((CPrtcGPS *)pGps)->para_Content_Length[i];
                                   //m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng[i];
                                  //wPaSendNum++;
                                  for(BYTE j = 0;j < leng[i];j++,wPaSendNum++)//��ϢID
                                  {
          
            content_position = i;
            for(BYTE m = 0;m < i;m++)
              content_position +=leng[i-m-1];

            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[j+content_position];
                                  }
        }
        wPaTotalNum = wPaSendNum;
        /*leng_1=((CPrtcGPS *)pGps)->para_Content_Length[1];
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_1;
                wPaSendNum++;
                for(i = 0;i < leng_1;i++,wPaSendNum++)//UTCʱ���һ�β���
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[leng_0+1+i];
                }
                leng_2=((CPrtcGPS *)pGps)->para_Content_Length[2];
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_2;//γ�ȶ�Ӧ����
                wPaSendNum++;
                for(i = 0;i < leng_2;i++,wPaSendNum++)//�ڶ��β���
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[2+i+leng_0+leng_1];//ÿ�μ�1����Ϊ��
                }
                
                leng_3=((CPrtcGPS *)pGps)->para_Content_Length[3];
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_3;//γ�ȷ����Ӧ����
                wPaSendNum++;
                for(i = 0;i < leng_3;i++,wPaSendNum++)//����γ��
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[3+i+leng_1+leng_2+leng_0];
                }
               
                leng_4=((CPrtcGPS *)pGps)->para_Content_Length[4];//���ȳ���
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_4;//γ�ȶ�Ӧ����
                wPaSendNum++;
                for(i = 0;i < leng_4;i++,wPaSendNum++)//���Ķβ���
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[4+i+leng_1+leng_2+leng_0+leng_3];//ÿ�μ�1����Ϊ��
                }
                leng_5=((CPrtcGPS *)pGps)->para_Content_Length[5];//���ȷ���
            
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_5;//��Ӧ����
                wPaSendNum++;   
                for(i = 0;i < leng_5;i++,wPaSendNum++)//����
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[5+i+leng_1+leng_2+leng_0+leng_3+leng_4];
                }
                leng_6=((CPrtcGPS *)pGps)->para_Content_Length[6];//��ǰ��λ����
            
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_6;//��Ӧ����
                wPaSendNum++;   
                for(i = 0;i < leng_6;i++,wPaSendNum++)
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[10+i+leng_1+leng_2+leng_4+leng_3];
                }
        leng_7=((CPrtcGPS *)pGps)->para_Content_Length[7];//���ڶ�λ��������Ŀ
            
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_7;//��Ӧ����
                wPaSendNum++;   
                for(i = 0;i < leng_7;i++,wPaSendNum++)
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[10+i+leng_1+leng_2+leng_4+leng_3];
                }
                flag = 10+leng_5+leng_1+leng_2+leng_4+leng_3;
            
                leng_6=((CPrtcGPS *)pGps)->para_Content_Length[5];//γ�ȷ���
                leng_7=((CPrtcGPS *)pGps)->para_Content_Length[6];
                leng_8=((CPrtcGPS *)pGps)->para_Content_Length[7];
                leng_9=((CPrtcGPS *)pGps)->para_Content_Length[8];
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_9;//��Ӧ����
                wPaSendNum++; 
                for(i = 0;i < leng_9;i++,wPaSendNum++)//9��UTC����
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[4+i+leng_6+leng_7+leng_8+flag];
                }*/
            }
            else
            {
               return;//��GPS���ջ�����û��������ֱ�ӷ��أ������ϴ�һ֡�����ݵı���
            }
           break;
        
        default:
           break;
         }
         break;
       case 15://���ն˰汾��
           wPaTotalNum = strlen((char *)VerNum);
           for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
           {
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = VerNum[i];
           }
           wPaTotalNum = g_pVersion[0];
           for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
           {
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_pVersion[i+1];
           }      
           #if 1 //for test ���й����в鿴����ֵ
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_GprsPowerSt;
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_batPowerSt;
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = m_linkflag;
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = m_initflag;
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_RstartNum);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_RstartNum);
       
           if(pDbg != null)
           {
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(pDbg->m_SSoeHeadPtr);
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(pDbg->m_SSoeHeadPtr);
           }
           else
           {
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 98;
           }

            if(pGprs != null) 
            {
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(pGprs->m_SSoeHeadPtr);
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(pGprs->m_SSoeHeadPtr);
            }
           else
           {
             m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0; 
             m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 99;
           }
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = pGprs->m_WaitConfTpId;
          
       m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_fault_phase;
       m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_differI_weizhi);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_differI_weizhi);
       m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_faultI_Valse);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_faultI_Valse);
       m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_differV_weizhi);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_differV_weizhi);
       m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_faultV_Valse);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_faultV_Valse);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_ucRefYxYcTs;
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(pGprs->m_resendflag);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(pGprs->m_reSendDelay);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(pGprs->m_reSendDelay);
           m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_yxChangeflag);
           
       
       
           
           #endif
           break;
      case 16://���绰���� ������һ֡������
           wPaTotalNum = PHONE_PA_NUM - 1;
           for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
           {
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_gSmsPhone[i];
           }
           break;
   case 31://��ң��ֵ
        wPaTotalNum = g_ucYxTransNum;//IEC_YX_NUM;
        BYTE bValCur;
        for(i = m_wSendPaNum;(i < g_ucYxTransNum) && (m_SendBuf.wWritePtr < 250);i++,wPaSendNum++)
        {
          bValCur = GetRIStatus(g_ucYXAddr[i]-1);
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = bValCur;
        }
        break;
   case 32://��ң��ֵ
        wPaTotalNum = g_ucYcTransNum;
        for(i = m_wSendPaNum;(i < g_ucYcTransNum) && (m_SendBuf.wWritePtr < 250);i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++ ] = LOBYTE(g_unYcTrans[i]);
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++ ] = HIBYTE(g_unYcTrans[i]);
        }
        break;
     }
    m_SendBuf.pBuf[wSecLenPtr] = m_SendBuf.wWritePtr - wSecLenPtr -1;
    m_wSendPaNum += wPaSendNum;
    if ((m_wSendPaNum < wPaTotalNum) && (m_wSendPaNum != 0))
      m_SendBuf.pBuf[4] |= 0x80;//���������λ������ʾ����û��������
    SendFrameTailForPa();
    
    if(FileName != 15)//���ն˰汾�ű��ĸ������й����е�ʵʱ�ڴ����ݲ鿴
    {
      if ((m_wSendPaNum < wPaTotalNum) && (m_wSendPaNum != 0))
      {
          delayms(300);
          SendReadPa(FileName,SecName);
      }
    }
    return;
}
void CProtocol::SendWrPaAck(WORD wFile,BYTE bySec)
{
    BYTE Style = 121;
    BYTE Reason = 0X0D;

    SendFrameHeadForPa(Style, Reason);
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
    m_SendBuf.wWritePtr += 5;//��Ϣ���ַ2+�ļ���2+����1
    switch(wFile)
    {//�ڳ���
      case 4:
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (RUN_PARA_NUM-1)*2;
        }
        break;
      case 5:
        {//��IEC����״̬
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (IEC_PARA_NUM-1);
        }
        break;
      case 6:
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = IEC_YC_NUM;
        }
        break;
      case 7:
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (BYTE)IEC_YX_NUM;
        }
        break;
      case 8:
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = IEC_YK_NUM;
        }
        break;
    }
    //FOR OLD ZIZHAN
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    SendFrameTailForPa();
    return;
}
void CProtocol::SendOneLoadData(WORD wCurLoadNum,WORD wFlag)
{
    BYTE Style = 125;
    BYTE Reason = 0X0D;
    BYTE byLoadDa[128] = {0};
    WORD wLoadMeas[60] = {0};
    unsigned long FLoadAddr;
    WORD wSecLenPtr;

    SendFrameHeadForPa(Style, Reason);
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
    m_SendBuf.wWritePtr += 5;//��Ϣ���ַ2+�ļ���2+����1
    wSecLenPtr = m_SendBuf.wWritePtr++;
    
    FLoadAddr = FADDR_LOAD_START + ((unsigned long)wCurLoadNum * FLASH_PLOAD_LEN);
    Sst26vf064b_Read(FLoadAddr,byLoadDa, FLASH_PLOAD_LEN);
    if(byLoadDa[0] != 0xEA || wFlag == 1)
      m_SendBuf.pBuf[wSecLenPtr] = 0;//�ڳ���
    else
    {
      memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&(byLoadDa[2]),6);//5�ֽ�ʱ��
      m_SendBuf.wWritePtr += 6;
      BYTE YcIndex = 0;
      memcpy(wLoadMeas,&byLoadDa[8],FLASH_PLOAD_LEN-8);
      for(int i = 0; i < g_ucYcTransNum; i++)   //������Ҫ�ϴ���ң��ֵ�����վ���������л���
       {
      if(g_ucYCAddr[i]!=255)
      {
          YcIndex = g_ucYCAddr[i] - 1;
              if(YcIndex <RM_1A_LUBO )//RM_1A_CABLE_T(YcIndex >= RM_BAT_U) && ((RM_1A_LOAD_I + RM_LOADI_NUM))
              {
                //YcIndex = YcIndex - RM_1A_LOAD_I; 
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(wLoadMeas[YcIndex]);
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(wLoadMeas[YcIndex]);
              }
      }
       }
       m_SendBuf.pBuf[wSecLenPtr] = m_SendBuf.wWritePtr - wSecLenPtr -1;
    }
    SendFrameTailForPa();
    
}

//==============================================================================
//  ��������   : SendLoadData
//  ��������   : ���͸�������
//  �������   : bySec:���� =0��ʾ��ȡ���һ�� 1:�����и������� 2:��ȡָ������
//               wStartNo:��ȡ�ĸ���������ʼ��� min=0 
//               wLoadNum:��Ҫ��ȡ�ĸ�����������(��ʱδʹ��)
//               byQds
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       : 
//==============================================================================

void CProtocol::SendLoadData(BYTE bySec,WORD wStartNo,WORD w_LoadNum,BYTE byScq)
{
      //WORD wLoadNum = w_LoadNum;
      WORD wCurLoadNum = 0;
      static unsigned int FLoadInfo[FLOADINFONUM];
      
      if(byScq == 0)
        CAT_SpiReadWords(EEPADD_LOADNUM, FLOADINFONUM, FLoadInfo);
      if((FLoadInfo[FLOAD_CS] != FLoadInfo[FLOAD_TOTALNUM] + FLoadInfo[FLOAD_NEW] + FLoadInfo[FLOAD_OLD]) || FLoadInfo[FLOAD_TOTALNUM] > FLASH_LOAD_MAXNUM || FLoadInfo[FLOAD_NEW] > FLASH_LOAD_MAXNUM)//���FLASH��ַ���ڸ��ɼ�¼����������
      {
          FLoadInfo[FLOAD_TOTALNUM] = 0; //��ո��ɼ�¼������
          FLoadInfo[FLOAD_NEW] = 0; //�����洢�ļ�¼���һ����¼��ƫ��λ��(�����Ѵ洢������һ����¼��λ��)
          FLoadInfo[FLOAD_OLD] = 0;//����һ����¼��λ��
      }
      //if(bySec == 0 || bySec == 1)
      { 
          if(FLoadInfo[FLOAD_NEW] == 0)
            wCurLoadNum = FLASH_LOAD_MAXNUM -1;
          else
            wCurLoadNum = FLoadInfo[FLOAD_NEW]-1; 
          if(FLoadInfo[FLOAD_TOTALNUM] == 0)//��û�и��ɼ�¼���ϴ�һ֡û�����ݵı���
          {
            SendOneLoadData(wCurLoadNum,1);
            return;
          }
          if(bySec == 0)
          {//������1��
            SendOneLoadData(wCurLoadNum); 
          }
          else
          {//�����¼�¼��ʼ����ȡN��
            if(wStartNo > wCurLoadNum)
              wCurLoadNum = FLASH_LOAD_MAXNUM - (wStartNo - wCurLoadNum);
            else
              wCurLoadNum = wCurLoadNum - wStartNo;
            if(wCurLoadNum >= FLoadInfo[FLOAD_TOTALNUM])
            {
              SendOneLoadData(wCurLoadNum,1);
              return;
            }
            SendOneLoadData(wCurLoadNum);
          }
      }
//        else if(bySec == 2)
//        {//�����ϼ�¼��ʼ����ȡN��
//            wCurLoadNum = FLoadInfo[FLOAD_OLD] + wStartNo;
//            if(wCurLoadNum >= FLASH_LOAD_MAXNUM)
//              wCurLoadNum = 0;
//            SendOneLoadData(wCurLoadNum);
//       }
    return;
  
}

void CProtocol::SendSavedSoe(BYTE bySec,WORD wStartNo,WORD wSOENum)
{
    BYTE Style = 125;
    BYTE Reason = 0X0D;
    BYTE bySoeDa[512];
    BYTE bySoeNum = 0;
    BYTE bySdSoeNum = 0;
    BYTE byCurSoeNum = SOE_NUM_SEG/2;

    if(bySec == 0)
    {//������SOE
      for(int i = 0; i < SOE_SEG;i++)
      {
    WatchDog();
    bySoeNum = ReadSoe(bySoeDa,i,0,SOE_NUM_SEG);
        if(bySoeNum <= SOE_NUM_SEG/2)
          byCurSoeNum = bySoeNum; 

        SendFrameHeadForPa(Style, Reason);
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
       m_SendBuf.wWritePtr += 5;//��Ϣ���ַ2+�ļ���2+����1
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = byCurSoeNum*SOE_SENDDA_LEN; 
        
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],bySoeDa,byCurSoeNum*SOE_SENDDA_LEN);
        m_SendBuf.wWritePtr += byCurSoeNum*SOE_SENDDA_LEN;
        bySdSoeNum = byCurSoeNum;
        SendFrameTailForPa();
        //delayms(1000);//�ӽӿ�����в鿴����
        
        if(bySoeNum < SOE_NUM_SEG/2)
          return;
        if(bySdSoeNum < bySoeNum)
        {
          byCurSoeNum = bySoeNum - bySdSoeNum;
          SendFrameHeadForPa(Style, Reason);
          memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
          m_SendBuf.wWritePtr += 5;//��Ϣ���ַ2+�ļ���2+����1
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = byCurSoeNum*SOE_SENDDA_LEN; 
          
          memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&bySoeDa[bySdSoeNum*SOE_SENDDA_LEN],byCurSoeNum*SOE_SENDDA_LEN);
          m_SendBuf.wWritePtr += byCurSoeNum*SOE_SENDDA_LEN;
          SendFrameTailForPa();
         delayms(1000); //�ӽӿ�����в鿴����
        }
      }
    }
    else if(bySec == 2)
    {//���ζ�
        BYTE read_seg = 0;
        BYTE StartNo_seg = 0;
        read_seg = ( read_seg + (wStartNo / SOE_NUM_SEG)) % SOE_SEG;
        StartNo_seg = wStartNo%SOE_NUM_SEG;
        bySoeNum = ReadSoe(bySoeDa,read_seg,StartNo_seg,wSOENum);
        if(bySoeNum <= SOE_NUM_SEG/2)
          byCurSoeNum = bySoeNum; 

        SendFrameHeadForPa(Style, Reason);
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
        m_SendBuf.wWritePtr += 5;//��Ϣ���ַ2+�ļ���2+����1
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = byCurSoeNum*SOE_SENDDA_LEN; 
        
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],bySoeDa,byCurSoeNum*SOE_SENDDA_LEN);
        m_SendBuf.wWritePtr += byCurSoeNum*SOE_SENDDA_LEN;
        bySdSoeNum = byCurSoeNum;
        SendFrameTailForPa();
    }
    return;

}
void CProtocol::SendSoeSavedStu()
{
    BYTE Style = 125;
    BYTE Reason = 0X0D;
    WORD wSecLenPtr;

    SendFrameHeadForPa(Style, Reason);
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
    m_SendBuf.wWritePtr += 5;//��Ϣ���ַ2+�ļ���2+����1
    wSecLenPtr = m_SendBuf.wWritePtr++;
    read_ptr();
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = soe_area;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = soe_ptr;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(soe_num);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(soe_num);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    
    m_SendBuf.pBuf[wSecLenPtr] = m_SendBuf.wWritePtr - wSecLenPtr -1;
    SendFrameTailForPa();
    return;

}
void CProtocol::SendTrigAck()
{
}

//��֯����ͷ,�̶���ʽ����
BOOL CProtocol::SendFrameHeadForPa(BYTE Style, BYTE Reason)
{
      m_SendBuf.wReadPtr = 0;
      m_SendBuf.wWritePtr = 0;
      if(pReceiveFrame->Frame69.Start1 == 0x69)
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr]= 0x69;
      else
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr]= 0x68;
      m_SendBuf.wWritePtr += 3;//���ĳ��ȣ�Ԥ������֡βʱ����
      if(pReceiveFrame->Frame69.Start1 == 0x69)
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;
      else
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x68;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x53; //���������óɹ̶�ֵ index=4
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(g_gRunPara[RP_COMM_ADDR]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(g_gRunPara[RP_COMM_ADDR]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = Style;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;//�ɱ�ṹ�޶��ʣ����óɹ̶�ֵ
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(Reason);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(Reason);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(g_gRunPara[RP_COMM_ADDR]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(g_gRunPara[RP_COMM_ADDR]);

    return TRUE;
}

//��֯����β����������֡����
BOOL CProtocol::SendFrameTailForPa(void)
{
    m_SendBuf.pBuf[2] = m_SendBuf.wWritePtr - 4;//
    m_SendBuf.pBuf[1] = m_SendBuf.pBuf[2]; //= m_SendBuf.wWritePtr - 4;���ĳ���
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[4], m_SendBuf.pBuf[2]);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x16;
    CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,0);
    

    return TRUE;
}





