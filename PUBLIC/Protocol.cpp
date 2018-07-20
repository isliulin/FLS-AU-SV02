/*------------------------------------------------------------------------
 Module:      protocol.cpp
 Author:      linxueqin
 Project:     规约平台
 State:       V1.0
 Creation Date:   2014-4-28
 Description:   规约基类的定义
------------------------------------------------------------------------*/

#include "Protocol.h"
#include "..\Main.h"


/***************************************************************
  Function:CProtocol
    规约基类构造函数
  参数：无
  返回：无
***************************************************************/
CProtocol::CProtocol()    
{
  memset(&m_SendBuf, 0, sizeof(m_SendBuf));   
    memset(&m_RecBuf, 0, sizeof(m_RecBuf));      
    memset(&m_RecFrame, 0, sizeof(m_RecFrame));  
}

/***************************************************************
  Function:Init
      规约平台初始化
  参数：无
  返回：TRUE 成功，FALSE 失败
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
    获取自身地址
  参数：无
  
  返回：自身地址
***************************************************************/
WORD CProtocol::GetOwnAddr(void)      
{
  return  g_gRunPara[RP_COMM_ADDR];
}
/***************************************************************
  Function:NeatenCommBuf
    通信缓冲区的规整
  参数：pCommIO
    pCommIO　指向通信缓冲区的指针
  返回：无
***************************************************************/
void CProtocol::NeatenCommBuf(VCommBuf *pCommBuf)
{
 // register unsigned  int i,j;

  if (pCommBuf->wReadPtr == 0)
  {
    return ; //读指针已经为0
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
    搜索信息帧
  参数：无
  
  返回：TRUE 找到正确的一帧报文，FALSE 未找到一帧正确的报文
***************************************************************/
BOOL CProtocol::SearchFrame(void)
{
  DWORD Rc;
  DWORD Len;//已处理过的字节数
  short MsgLen;

  while (1)
  {
    MsgLen = m_RecBuf.wWritePtr - m_RecBuf.wReadPtr;
    if (MsgLen <= 0)
    {
      return FALSE;
    }

    Rc = SearchOneFrame(&m_RecBuf.pBuf[m_RecBuf.wReadPtr], MsgLen);
    /*if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF])//调试使用
             {//设置了将RF收到的数据转发到调试口
                MyPrintfD(&(pReceiveFrame->Frame55.Start1),pReceiveFrame->Frame55.FrameLen);
             }*/
    Len = Rc & ~FRAME_SHIELD; //已处理过的字节数

    switch  (Rc & FRAME_SHIELD)
    {
      case FRAME_OK:
        DisableRetry();
        m_RecFrame.pBuf = &m_RecBuf.pBuf[m_RecBuf.wReadPtr];  //记录有效报文的起始地址
        m_RecFrame.wWritePtr = (WORD)Len; //记录有效报文的长度
        m_RecBuf.wReadPtr += (WORD)Len; //指针移到下一报文处
        return TRUE;

      case FRAME_ERR:
        if (!Len)
        {
          m_RecBuf.wReadPtr++;
        }
        else
        {
          m_RecBuf.wReadPtr += Len; //指针移到下一报文处
        }
        break;

      case FRAME_LESS:
        m_RecBuf.wReadPtr++;  //右移一个字节继续检索正确报文 lxq
        break;
        //m_RecBuf.wReadPtr += (WORD)Len; //指针移到下一报文处
        //return FALSE;
      default:
        break;//??do what
    }//switch
  }//while
}

/***************************************************************
  Function:SearchHead
    搜索报文中的一个关键字
  参数：Buf, Len,　Offset1, Key1
    Buf 报文缓冲区头指针
    Len 要搜索的报文长度
    Offset1 关键字的在报文中的偏移位置
    Key1 关键字
  返回：关键字在报文缓冲区中的偏移位置
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
    规约接收数据并解析
    当socket中断连接时此函数返回（返回后socket重新侦听，将以阻塞的方式停留在select函数处）
  参数：无

  返回：无
***************************************************************/
int CProtocol::RcvData(int* iSocket)
{
    int rxlen=0;
    int nLen=1024;    
    //while(1)//要不要继续收报文? lxq
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
            {//将RF口收到的数据转发出去
              CommSendData(m_RecBuf.pBuf,rxlen,g_CmIdDBG);//g_CmIdEX
            }
          #endif
          DoReceive();
       }
      else if(rxlen <= 0)//recv=0 对端发出断开命令
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
    判断DPRAM中命令反馈区是否正确
  参数：RamBase dpram地址
    buf  偏移量

  返回：1 命令正确； 0 错误
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



//校验和
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
//参数查询及设置部分 + 远程升级====================================================================================
//自定义报文
BOOL CProtocol::RecFrame69(void)
{
  pReceiveFrame = (VParaFrame*)m_RecFrame.pBuf;

    switch(pReceiveFrame->Frame69.Type)
    {
        case 0x7a://读文件
            RecReadFile();
            break;
        case 0x7d://写文件
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
//解析读参数报文
BOOL CProtocol::RecReadFile(void)
{
    BYTE *pData;
    WORD wFile;
    BYTE bySec;//节名
    BYTE bySCQ;//限定词
    WORD wStartNo;//起始序号
    WORD wSOENum;//读取的SOE个数
    WORD wLoadNum;//读取的负荷数据条数(每条以时间为单位)

    pReceiveFrame = (VParaFrame*)m_RecFrame.pBuf;//加这句代码的原因是，若是68报文跳转到此函数，CProtocol中的pReceiveFrame指针未赋值
    pData = &pReceiveFrame->Frame69.Data[0];

    wFile = MAKEWORD(pData[0],pData[1]);
    pData +=2;
    bySec = *pData++;
    bySCQ = *pData++;
    m_wSendPaNum = 0;//多帧参数上传是靠递归调用，所以当收到读参数指令时应该给此变量清0
    if(bySCQ == 5 )
     {
        SendWrPaAck(wFile,bySec);//选择写参数
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
          #ifdef GetSoeFromRam //定义了保存soe到内部flash才编译下边3个函数
            SendSoeSavedStu();//读SOE状态
          #endif
        }
        break;
      case 3:
        if(bySCQ == 6)
        {//触发子站测试
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
        SendReadPa(wFile,bySec);//读终端自身参数
        break;
      //case 9://读ODU参数
        //if(pOdu != null) ((CPrtcOdu *)pOdu)->SendRCmdToOdu(wFile,bySec,this);
        //break;
      //case 10: //读ODU1参数 
       // if(pOdu1 != null)  ((CPrtcOdu *)pOdu1)->SendRCmdToOdu(wFile,bySec,this);
        //break;
      case 12: //读GPS参数 
        if(pGps != null)  ((CPrtcGPS *)pGps)->SendRCmdToGPR(wFile,bySec,this);//RecFrameGPR(wFile,bySec,this);
        break;
    case 13://读无线模块的参数
      if(pRf!= null)((CPrtcRF*)pRf)->SendRCmdToRf003(this);
    break;
    case 20:
          wStartNo = MAKEWORD(pData[0],pData[1]);
          pData +=2;
          wLoadNum = MAKEWORD(pData[0],pData[1]);
          pData +=2;
          SendLoadData(bySec,wStartNo,wLoadNum,bySCQ);
           break;
      case 26:   //节名=线偏相偏
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

//解析写参数报文
BOOL CProtocol::RecWriteFile(void)
{
    BYTE *pData;
    WORD wFile;
    BYTE bySec;//节名
    WORD wSecLen;//段长度
    WORD i = 0,j = 0;
    WORD wPaStartId = 0;
    WORD wInfoAddr = 0;
    //WORD wOldValBk;
    WORD wOldCfgKeyBk;
  WORD wOldCfgKeyHBk;
    //BYTE bLine;
    pReceiveFrame = (VParaFrame*)m_RecFrame.pBuf;//加这句代码的原因是，若是68报文跳转到此函数，CProtocol中的pReceiveFrame指针未赋值
    
    wInfoAddr = MAKEWORD(pReceiveFrame->Frame69.InfoAddr[0],pReceiveFrame->Frame69.InfoAddr[1]);
    wPaStartId = wInfoAddr - 0x6902;

    pData = &pReceiveFrame->Frame69.Data[0];
    wFile = MAKEWORD(pData[0],pData[1]);
    pData += 2;
    bySec = *pData++;
    wSecLen= *pData++;//节长度
    
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
      case 0x81://设置GPRS为配置模式
        GPRS_SETA_L();
        break;
      case 0x82:
        ExitGprsSet();
        break;
      case 4:
        if(bySec == 0 )//写运行参数
        {
          for(i = wPaStartId,j = 0; j < wSecLen/2 && i < RUN_PARA_NUM-1;i++,j++)
          {//b
              if((i == RP_POWER_MODE) &&(g_gRunPara[Terminal_Type] != AERIAL_TERMINAL))
              {//ODU和电缆RF的电源模式自动切换，不从运行参数中取
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
                  //参数赋值
                  g_gRunPara[i] = MAKEWORD(pData[0],pData[1]);
                  pData += 2;
                  
                  //检查两路合并一路的参数是否有变化
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
                  //检查配置控制字中对点模式是否从启用到停用，若是则清空遥信遥测内存
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
        if(g_gRunPara[RP_COMM_ADDR] == 0)//作为复位点表的方式，如果子站地址设置成0，则复位
        {
            RstRunPara();  //运行参数表复位
        } 
        g_ucParaChang |= BIT0;
        SetEqpInfo();
      }
      break;
      case 5:
        if(bySec == 0 )//写IEC参数
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < IEC_PARA_NUM-1;i++,j++)
          {
            g_ucPara101[i] = *pData++;
          }
          if(g_ucPara101[0] == 0)//作为复位点表的方式，如果第一个就设置为0，则利用这种方式复位点表
          {
              RstIEC101Para();  //IEC参数点表复位
          } 
          initpara();//更新通信内存参数
          g_ucParaChang |= BIT1;
        }
        break;  
      case 6:
        if(bySec == 0 )//写遥信点表
        {
          for(i = wPaStartId,j = 0; j < wSecLen/2 && i < IEC_YX_NUM;i++,j++)
          {
            g_ucYXAddr[i] = MAKEWORD(pData[0],pData[1]);
            pData += 2;
          }
          if(g_ucYXAddr[0] == 0)//作为复位点表的方式，如果第一个就设置为0，则利用这种方式复位点表
          {
       RstIEC101YxAddr();  //遥信地址表复位
          }
          InitTsYxInfo();  
          SetEqpInfo();
          g_ucParaChang |= BIT2; 
        }
        
        break;
      case 7:
        if(bySec == 0 )//写遥测点表
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < IEC_YC_NUM;i++,j++)
          {
            g_ucYCAddr[i] = *pData++;
          }
          if(g_ucYCAddr[0] == 0)//作为复位点表的方式，如果第一个就设置为0，则利用这种方式复位点表
          {
              RstIEC101YcAddr();  //遥测地址表复位
          } 
          InitTsYcInfo();
          SetEqpInfo();
          g_ucParaChang |= BIT3;
        }
      break;
      case 8:
        if(bySec == 0 )//写遥控点表
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < IEC_YK_NUM;i++,j++)
          {
            g_ucYKPa[i] = *pData++;
          }
          if(g_ucYKPa[0] == 255)//作为复位点表的方式，如果第一个就设置为255，则利用这种方式复位点表
          {
              RstIEC101YkAddr();  //遥控地址表复位
          } 
          g_ucParaChang |= BIT4;
        }
      break;
      //case 9://写ODU参数
        //if(pOdu != null) ((CPrtcOdu *)pOdu)->SendWCmdToOdu(wFile,bySec,pData-1,this);
          //break;
      //case 10://写ODU1参数
          //if(pOdu1 != null) ((CPrtcOdu *)pOdu1)->SendWCmdToOdu(wFile,bySec,pData-1,this);
          //break;
      case 11:  //写GPRS参数
          if(pGprs != null) ((CPrtcSms *)pGprs)->SendWCmdToIHD(0,bySec,pData-1,this);
          break;
   case 13:
     g_WFile = wFile;
         g_Sec = bySec;
    if(pRf != null) ((CPrtcRF *)pRf)->SendWCmdToRf003(wFile,pData-1,this);
    break;
      case 16:
        if(bySec == 0 )//写电话号码表
        {
          for(i = wPaStartId,j = 0; j < wSecLen && i < PHONE_PA_NUM - 1;i++,j++)
          {
            g_gSmsPhone[i] = *pData++;
          }
          if(g_gSmsPhone[PHONE1] == 'R')//作为复位点表的方式，如果第一个电话号码的第一个字符设置为R，则利用这种方式复位点表
          {
              RstSmsPhoneInfo();
          } 
          g_ucParaChang |= BIT5;
          SendWrPaSuc();
        }
      break;
      case 26:   //节名=线偏相偏
      
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
        if(bySec == 0 )//遥信对点
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
        if(bySec == 0 )//遥测对点
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
    m_SendBuf.wWritePtr += 6;//信息体地址2+文件名2+节名1+节长度1
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
    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(wInfoAddr);//写信息体地址
    m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(wInfoAddr);
    
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(FileName);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(FileName);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = SecName;
    
    wSecLenPtr = m_SendBuf.wWritePtr++;
    WORD i = 0;
    switch(FileName)
    {
      case 4://读运行参数
        wPaTotalNum = RUN_PARA_NUM-1;
        for(i = 0;i < RUN_PARA_NUM-1;i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_gRunPara[i]);
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_gRunPara[i]);
        } 
        break;
     case 5://读IEC参数
       wPaTotalNum = IEC_PARA_NUM-1;
       for(i = 0;i < IEC_PARA_NUM-1;i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_ucPara101[i];
        } 
       break;
     case 6://读遥信点表
        wPaTotalNum = g_ucYxTransNum;//IEC_YX_NUM;
        for(i = m_wSendPaNum;(i < g_ucYxTransNum) && (m_SendBuf.wWritePtr < 250);i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = LOBYTE(g_ucYXAddr[i]);
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = HIBYTE(g_ucYXAddr[i]);
        }
        break;
     case 7://读遥测点表
        wPaTotalNum = g_ucYcTransNum;
        for(i = 0;i < g_ucYcTransNum;i++,wPaSendNum++)
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_ucYCAddr[i];
        } 
        break;
      
      case 8://读遥控点表
         wPaTotalNum = IEC_YK_NUM;
         for(i = 0;i < IEC_YK_NUM;i++,wPaSendNum++)
         {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_ucYKPa[i];
         }
         break;
      case 9://读ODU参数
          //if(pOdu == null) break;
           //wPaTotalNum = (((CPrtcOdu *)pOdu)->pReceiveFrame)->Frame67.Data[6];
           //for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
           //{
             // m_SendBuf.pBuf[m_SendBuf.wWritePtr++] =(((CPrtcOdu *)pOdu)->pReceiveFrame)->Frame67.Data[7+i];
           //}
           break;
    case 10://读ODU参数
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
        case 0://IP和端口
        case 1://短信功能
        case 2://APN
        case 3://用户名
        case 4://密码
            if(pGprs == null) break;
            wPaTotalNum = (((CBJ101S *)pGprs)->pReceiveFrame)->FrameAA.byContent[3];
            for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
            {
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->FrameAA.byContent[i + 4];
            }
            break;
        case 0x0f:
            if(pGprs == null) break;
            wPaTotalNum = 8;//特殊处理，只取以下状态量
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[0] ^ 1;  //模块状态
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[1] ^ 1;  //SIM卡状态
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[9];  //CSQ值
            m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = (((CBJ101S *)pGprs)->pReceiveFrame)->Frame68.Data[10]; //频段
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
     case 12://GPS参数
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
                                  for(BYTE j = 0;j < leng[i];j++,wPaSendNum++)//消息ID
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
                for(i = 0;i < leng_1;i++,wPaSendNum++)//UTC时间第一段参数
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[leng_0+1+i];
                }
                leng_2=((CPrtcGPS *)pGps)->para_Content_Length[2];
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_2;//纬度对应长度
                wPaSendNum++;
                for(i = 0;i < leng_2;i++,wPaSendNum++)//第二段参数
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[2+i+leng_0+leng_1];//每次加1是因为，
                }
                
                leng_3=((CPrtcGPS *)pGps)->para_Content_Length[3];
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_3;//纬度方向对应长度
                wPaSendNum++;
                for(i = 0;i < leng_3;i++,wPaSendNum++)//三段纬度
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[3+i+leng_1+leng_2+leng_0];
                }
               
                leng_4=((CPrtcGPS *)pGps)->para_Content_Length[4];//经度长度
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_4;//纬度对应长度
                wPaSendNum++;
                for(i = 0;i < leng_4;i++,wPaSendNum++)//第四段参数
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[4+i+leng_1+leng_2+leng_0+leng_3];//每次加1是因为，
                }
                leng_5=((CPrtcGPS *)pGps)->para_Content_Length[5];//经度方向
            
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_5;//对应长度
                wPaSendNum++;   
                for(i = 0;i < leng_5;i++,wPaSendNum++)//经度
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[5+i+leng_1+leng_2+leng_0+leng_3+leng_4];
                }
                leng_6=((CPrtcGPS *)pGps)->para_Content_Length[6];//当前定位质量
            
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_6;//对应长度
                wPaSendNum++;   
                for(i = 0;i < leng_6;i++,wPaSendNum++)
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[10+i+leng_1+leng_2+leng_4+leng_3];
                }
        leng_7=((CPrtcGPS *)pGps)->para_Content_Length[7];//用于定位的卫星数目
            
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_7;//对应长度
                wPaSendNum++;   
                for(i = 0;i < leng_7;i++,wPaSendNum++)
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[10+i+leng_1+leng_2+leng_4+leng_3];
                }
                flag = 10+leng_5+leng_1+leng_2+leng_4+leng_3;
            
                leng_6=((CPrtcGPS *)pGps)->para_Content_Length[5];//纬度方向
                leng_7=((CPrtcGPS *)pGps)->para_Content_Length[6];
                leng_8=((CPrtcGPS *)pGps)->para_Content_Length[7];
                leng_9=((CPrtcGPS *)pGps)->para_Content_Length[8];
                m_SendBuf.pBuf[m_SendBuf.wWritePtr++]=leng_9;//对应长度
                wPaSendNum++; 
                for(i = 0;i < leng_9;i++,wPaSendNum++)//9段UTC日期
                {
                    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.byContent[4+i+leng_6+leng_7+leng_8+flag];
                }*/
            }
            else
            {
               return;//若GPS接收缓冲区没有数据则直接返回，避免上传一帧无内容的报文
            }
           break;
        
        default:
           break;
         }
         break;
       case 15://读终端版本号
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
           #if 1 //for test 运行过程中查看变量值
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
      case 16://读电话号码 限制在一帧报文中
           wPaTotalNum = PHONE_PA_NUM - 1;
           for(i = 0;i < wPaTotalNum;i++,wPaSendNum++)
           {
              m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = g_gSmsPhone[i];
           }
           break;
   case 31://读遥信值
        wPaTotalNum = g_ucYxTransNum;//IEC_YX_NUM;
        BYTE bValCur;
        for(i = m_wSendPaNum;(i < g_ucYxTransNum) && (m_SendBuf.wWritePtr < 250);i++,wPaSendNum++)
        {
          bValCur = GetRIStatus(g_ucYXAddr[i]-1);
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = bValCur;
        }
        break;
   case 32://读遥测值
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
      m_SendBuf.pBuf[4] |= 0x80;//控制域最高位用来表示还有没后续报文
    SendFrameTailForPa();
    
    if(FileName != 15)//读终端版本号报文附带运行过程中的实时内存数据查看
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
    m_SendBuf.wWritePtr += 5;//信息体地址2+文件名2+节名1
    switch(wFile)
    {//节长度
      case 4:
        {
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (RUN_PARA_NUM-1)*2;
        }
        break;
      case 5:
        {//读IEC参数状态
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
    m_SendBuf.wWritePtr += 5;//信息体地址2+文件名2+节名1
    wSecLenPtr = m_SendBuf.wWritePtr++;
    
    FLoadAddr = FADDR_LOAD_START + ((unsigned long)wCurLoadNum * FLASH_PLOAD_LEN);
    Sst26vf064b_Read(FLoadAddr,byLoadDa, FLASH_PLOAD_LEN);
    if(byLoadDa[0] != 0xEA || wFlag == 1)
      m_SendBuf.pBuf[wSecLenPtr] = 0;//节长度
    else
    {
      memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&(byLoadDa[2]),6);//5字节时间
      m_SendBuf.wWritePtr += 6;
      BYTE YcIndex = 0;
      memcpy(wLoadMeas,&byLoadDa[8],FLASH_PLOAD_LEN-8);
      for(int i = 0; i < g_ucYcTransNum; i++)   //计算需要上传的遥测值，按照具体情况进行换算
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
//  函数名称   : SendLoadData
//  功能描述   : 发送负荷数据
//  输入参数   : bySec:节名 =0表示读取最近一条 1:读所有负荷数据 2:读取指定条数
//               wStartNo:读取的负荷数据起始序号 min=0 
//               wLoadNum:需要读取的负荷数据条数(暂时未使用)
//               byQds
//  输出参数   : <无>
//  返回值     : <无>
//  其他说明   : 
//  作者       : 
//==============================================================================

void CProtocol::SendLoadData(BYTE bySec,WORD wStartNo,WORD w_LoadNum,BYTE byScq)
{
      //WORD wLoadNum = w_LoadNum;
      WORD wCurLoadNum = 0;
      static unsigned int FLoadInfo[FLOADINFONUM];
      
      if(byScq == 0)
        CAT_SpiReadWords(EEPADD_LOADNUM, FLOADINFONUM, FLoadInfo);
      if((FLoadInfo[FLOAD_CS] != FLoadInfo[FLOAD_TOTALNUM] + FLoadInfo[FLOAD_NEW] + FLoadInfo[FLOAD_OLD]) || FLoadInfo[FLOAD_TOTALNUM] > FLASH_LOAD_MAXNUM || FLoadInfo[FLOAD_NEW] > FLASH_LOAD_MAXNUM)//如果FLASH地址不在负荷记录保存区域内
      {
          FLoadInfo[FLOAD_TOTALNUM] = 0; //清空负荷记录总条数
          FLoadInfo[FLOAD_NEW] = 0; //即将存储的记录与第一条记录的偏移位置(不是已存储的最新一条记录的位置)
          FLoadInfo[FLOAD_OLD] = 0;//最老一条记录的位置
      }
      //if(bySec == 0 || bySec == 1)
      { 
          if(FLoadInfo[FLOAD_NEW] == 0)
            wCurLoadNum = FLASH_LOAD_MAXNUM -1;
          else
            wCurLoadNum = FLoadInfo[FLOAD_NEW]-1; 
          if(FLoadInfo[FLOAD_TOTALNUM] == 0)//若没有负荷记录则上传一帧没有内容的报文
          {
            SendOneLoadData(wCurLoadNum,1);
            return;
          }
          if(bySec == 0)
          {//读最新1条
            SendOneLoadData(wCurLoadNum); 
          }
          else
          {//从最新记录开始，读取N条
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
//        {//从最老记录开始，读取N条
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
    {//读所有SOE
      for(int i = 0; i < SOE_SEG;i++)
      {
    WatchDog();
    bySoeNum = ReadSoe(bySoeDa,i,0,SOE_NUM_SEG);
        if(bySoeNum <= SOE_NUM_SEG/2)
          byCurSoeNum = bySoeNum; 

        SendFrameHeadForPa(Style, Reason);
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
       m_SendBuf.wWritePtr += 5;//信息体地址2+文件名2+节名1
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = byCurSoeNum*SOE_SENDDA_LEN; 
        
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],bySoeDa,byCurSoeNum*SOE_SENDDA_LEN);
        m_SendBuf.wWritePtr += byCurSoeNum*SOE_SENDDA_LEN;
        bySdSoeNum = byCurSoeNum;
        SendFrameTailForPa();
        //delayms(1000);//从接口软件中查看报文
        
        if(bySoeNum < SOE_NUM_SEG/2)
          return;
        if(bySdSoeNum < bySoeNum)
        {
          byCurSoeNum = bySoeNum - bySdSoeNum;
          SendFrameHeadForPa(Style, Reason);
          memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
          m_SendBuf.wWritePtr += 5;//信息体地址2+文件名2+节名1
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = byCurSoeNum*SOE_SENDDA_LEN; 
          
          memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&bySoeDa[bySdSoeNum*SOE_SENDDA_LEN],byCurSoeNum*SOE_SENDDA_LEN);
          m_SendBuf.wWritePtr += byCurSoeNum*SOE_SENDDA_LEN;
          SendFrameTailForPa();
         delayms(1000); //从接口软件中查看报文
        }
      }
    }
    else if(bySec == 2)
    {//按段读
        BYTE read_seg = 0;
        BYTE StartNo_seg = 0;
        read_seg = ( read_seg + (wStartNo / SOE_NUM_SEG)) % SOE_SEG;
        StartNo_seg = wStartNo%SOE_NUM_SEG;
        bySoeNum = ReadSoe(bySoeDa,read_seg,StartNo_seg,wSOENum);
        if(bySoeNum <= SOE_NUM_SEG/2)
          byCurSoeNum = bySoeNum; 

        SendFrameHeadForPa(Style, Reason);
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame69.InfoAddr[0],5);
        m_SendBuf.wWritePtr += 5;//信息体地址2+文件名2+节名1
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
    m_SendBuf.wWritePtr += 5;//信息体地址2+文件名2+节名1
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

//组织报文头,固定格式报文
BOOL CProtocol::SendFrameHeadForPa(BYTE Style, BYTE Reason)
{
      m_SendBuf.wReadPtr = 0;
      m_SendBuf.wWritePtr = 0;
      if(pReceiveFrame->Frame69.Start1 == 0x69)
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr]= 0x69;
      else
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr]= 0x68;
      m_SendBuf.wWritePtr += 3;//报文长度，预留，在帧尾时处理
      if(pReceiveFrame->Frame69.Start1 == 0x69)
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x69;
      else
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x68;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x53; //控制域，设置成固定值 index=4
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(g_gRunPara[RP_COMM_ADDR]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(g_gRunPara[RP_COMM_ADDR]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = Style;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = 0x01;//可变结构限定词，设置成固定值
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(Reason);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(Reason);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = LOBYTE(g_gRunPara[RP_COMM_ADDR]);
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HIBYTE(g_gRunPara[RP_COMM_ADDR]);

    return TRUE;
}

//组织报文尾，并发送整帧报文
BOOL CProtocol::SendFrameTailForPa(void)
{
    m_SendBuf.pBuf[2] = m_SendBuf.wWritePtr - 4;//
    m_SendBuf.pBuf[1] = m_SendBuf.pBuf[2]; //= m_SendBuf.wWritePtr - 4;报文长度
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&m_SendBuf.pBuf[4], m_SendBuf.pBuf[2]);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x16;
    CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,0);
    

    return TRUE;
}





