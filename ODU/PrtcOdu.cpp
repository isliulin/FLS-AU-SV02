/*------------------------------------------------------------------------
 Module:            PrtcOdu.cpp
 Author:            linxueqin
 Project:           规约平台
 State:             V1.0
 Creation Date:     2015-3-16
 Description:       与ODU通信--primary
------------------------------------------------------------------------*/




#ifndef _PRTC_ODU

  #define _PRTC_ODU
  #include "PrtcOdu.h"
#include "..\Main.h"
//构造函数
CPrtcOdu::CPrtcOdu() : CProtocol()
{
  m_OduAddr = 0;
  m_open_gprs = 0;
}


void CPrtcOdu::Run(void)
{
    
    RcvData(&m_uartId);
    DoCommSendIdle();
}

/***************************************************************
    Function：Init
        规约初始化
    参数：
    返回：TRUE 成功，FALSE 失败
***************************************************************/
BOOL CPrtcOdu::Init(WORD uartId)
{
    BOOL rc;
    m_uartId = uartId;
    if(uartId == g_CmIdODU1)  
      m_OduAddr = 1; 
    else if(uartId == g_CmIdODU2)  
      m_OduAddr = 2;
    rc = CProtocol::Init(1);
    if (!rc)
    {
      return FALSE;
    }
    
    m_ODUCall2Flag = 0;
    m_ODUCallFlag = 0;
    m_WrPaWaitflag = 0;
    m_ReTxNum = 0;
   
    if(m_uartId == g_CmIdODU1)
    {
      m_ODUCall2Bit = BIT0;
      m_ODUCallBit= BIT0;

    }
    else if(m_uartId == g_CmIdODU2)
    { 
      m_ODUCall2Bit = BIT1;
      m_ODUCallBit= BIT1;
    }
    return TRUE;
}

/***************************************************************
    Function：DoReceive
        接收报文处理
    参数：无

    返回：TRUE 成功，FALSE 失败
***************************************************************/
  BOOL CPrtcOdu::DoReceive()
  {
     while(1)
     {
        if (SearchFrame() != TRUE)
        return FALSE;
        
        pReceiveFrame = (VIecOduFrame *)m_RecFrame.pBuf;
        if (pReceiveFrame->Frame67.Start1 == 0x67)
        {
             RecFrame67(&pReceiveFrame->Frame67);
             continue;
        }
        if (pReceiveFrame->Frame69.Start1 == 0x69)
        {
          RecFrame69();
          continue;
        }
     
     }
  }
/***************************************************************
    Function：RecFrame67
        解析67帧
    参数：无

    返回：TRUE 成功，FALSE 失败
***************************************************************/

BOOL CPrtcOdu::RecFrame67(VFrame67 *pframe67)
{
    unsigned char Type_odu = pframe67->Type_odu;
  //unsigned char Addr_odu = pframe67->Addr_odu;
    static BYTE bJYbk = 0;//遥测有变化则转发
    BYTE bBuf[2] = {0,0};
    if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF]) && (Type_odu != 0x15) && (Type_odu != 0x64))
    {//设置了将ODU收到的数据转发到调试口 遥测不转发 响应总召和结束总召不转发
      BYTE bVal = m_OduAddr | 0xD0;
      MyPrintfD(&bVal,1);
      MyPrintfD(&(pReceiveFrame->Frame67.Start1),pReceiveFrame->Frame67.Length1+6);
    }

    if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF]) && (Type_odu == 0x15)&&(bJYbk != m_RecFrame.pBuf[pReceiveFrame->Frame67.Length1+4]))
      {//遥测有变化则转发
         bJYbk = m_RecFrame.pBuf[pReceiveFrame->Frame67.Length1+4];
         BYTE bVal = m_OduAddr | 0xD0;
         MyPrintfD(&bVal,1);
         MyPrintfD(&(pReceiveFrame->Frame67.Start1),pReceiveFrame->Frame67.Length1+6);
      }
     
    
    switch(Type_odu)
    {
        case 0x01:
          RecODUFault(m_OduAddr);
          break;
        case 0x15://解析接收到的ODU遥测报文
              RecODUFault_yaoce(m_OduAddr);
            break;
        case 0x51://ODU电源
            RecODUPower_Supply(); 
            break;
         case 0x66://ODU状态汇报
            //RecODUPower_Supply();
            bBuf[0] = 0xEF;
            bBuf[1] = 0XEF;
            MyPrintfD(bBuf,2);
            CreatNewSoe(RI_STATE_CYCLE,1);
            OpenTimer(TM_SENDSTATE); //定时1分钟后发送遥信变0的信息
            break;
        case 0x2E://遥控
             if(g_YkOrderFlag ==ON)
              g_YkOrderFlag=TRUE;
            break;
        case 0x7D://写参数返回
                RecOduWPa();
        case 0x7A://读参数返回
                RecODUPara();
            break;
        default:
            break;
    }
    return TRUE;
}




//通道发送空闲处理
void CPrtcOdu::DoCommSendIdle(void)
{
  if(m_ODUCallFlag == 1)
    {
        m_ODUCallFlag=0;
        SendODUCallFrame();//总召
        return;
    }
    if(m_ODUCall2Flag == 1)
    {
          m_ODUCall2Flag=0;
          SendODUFrame();//召二级数据
      return;
    }

    //开关GPRS时发信息给ODU 新的电源管理中不需要再发
//        if(m_open_gprs==0x55) 
//          SendWCmdToOdu(0,6);
//        else if(m_open_gprs==0x66)
//          SendWCmdToOdu(0,7);
//        m_open_gprs=0;

    //ODU询问时回复了延迟关断指令，之后终端处理完信息或GPRS已经关闭，则主动给ODU发请求关主板命令
    if(m_RcvCloseZdFlag == ON)
    {
      if((!g_batPowerSt)&&(CheckHaveDataToSd() == FALSE || !g_GprsPowerSt))
      {
        
        BYTE bBuf[3] = {0xDA,0XDA};
        bBuf[2] = m_OduAddr | 0xD0;
        MyPrintfD(bBuf,3);

        SendWCmdToOdu(0x55,3);
        m_RcvCloseZdFlag = OFF;//回复ODU可以关主板后此标识复位 
      }
    }

    //写参数重发
    #define MAX_RESENDNUM 3
    if(m_WrPaWaitflag == TRUE && m_WrPaWaitCt == 0)
    {
          if(m_ReTxNum < MAX_RESENDNUM)
          {
            
            m_WrPaWaitCt = 10;
            m_WrPaWaitflag = ON;
            SendOduRetry();
            m_ReTxNum++;
            return;
          }
          else
          {
            m_WrPaWaitflag = OFF;
            m_ReTxNum = 0;
            m_WrPaWaitCt = 0;
          }
      
     }
}


//ODU写参数重传
void CPrtcOdu::SendOduRetry(void)
{
  CommWrite((char*)m_oduSendBuf,m_oduSendLen);
}
/***************************************************************
    Function：OnTimeOut
        定时处理
    参数：TimerID
        TimerID 定时器ID
    返回：无
***************************************************************/
BOOL CPrtcOdu::DoTimeOut(WORD wTimerID)
{
  if(m_WrPaWaitflag == ON)
  {
    if(m_WrPaWaitCt > 0)
      {//ODU写参数重发计时
          m_WrPaWaitCt--;
          if(m_WrPaWaitCt <= 0)
          {
            m_WrPaWaitflag = TRUE;
            return TRUE;
          }
       }
   }
   
   if(g_sTimer[TM_ODU_call].m_TmFlag & m_ODUCallBit)
   {//启动ODU总召
     g_sTimer[TM_ODU_call].m_TmFlag &= ~m_ODUCallBit;
      m_ODUCallFlag = 1;
   }
   if(!m_ODUCallFlag)
   {//若这一秒发了总召就不再召二级数据
     if(g_sTimer[TM_ODU_call2].m_TmFlag & m_ODUCall2Bit)
     {//启动ODU召唤二级数据
       g_sTimer[TM_ODU_call2].m_TmFlag &= ~m_ODUCall2Bit;
       m_ODUCall2Flag = 1;
     }
  }
  if(g_sTimer[TM_ODU_call2].m_TmCount == 0)
      OpenTimer(TM_ODU_call2);
  if(g_sTimer[TM_ODU_call].m_TmCount == 0)
      OpenTimer(TM_ODU_call);
  if( m_ODUCall2Flag || m_ODUCallFlag)
    return TRUE;
  return FALSE;
}

//返回写参数成功
void CPrtcOdu::SendOWrPaSuc()
{
    m_SendBuf.wReadPtr = 0;
     m_SendBuf.wWritePtr=0;
    pSendFrame = (VIecOduFrame *)m_SendBuf.pBuf;
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame67.Start1,(pReceiveFrame->Frame67.Length1)+6);
    m_SendBuf.wWritePtr += (pReceiveFrame->Frame67.Length1)+6;
    
    if(m_RWPaSrcObj !=null)
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,m_RWPaSrcObj->m_uartId);
    return;

}
//解析收到的写文件确认报文
void CPrtcOdu::RecOduWPa(void)
{
  if(m_ReadPaTp == 3 || m_ReadPaTp == 6 || m_ReadPaTp == 7)
  {
    m_WrPaWaitflag = OFF;//收到写参数的确认报文后关闭重传
    m_ReTxNum = 0;
    m_WrPaWaitCt = 0;
    SendOWrPaSuc();
  }
  else
  {
    if(m_RWPaSrcObj !=null)
      m_RWPaSrcObj->SendWrPaSuc(); 
  }

  
}

//ODU召二级数据
void CPrtcOdu::SendODUFrame(void)
{
    m_SendBuf.wReadPtr = m_SendBuf.wWritePtr = 0;

    #define ODUCALL2LEN 5
    BYTE bySdFrame[ODUCALL2LEN]={0x10,0x5B,0x01,0x5c,0x16};
  //BYTE bySdFrame[ODUCALL2LEN]={0x10,0x5B,0x02,0x5d,0x16};//ODU2    
  // bySdFrame[2] = m_OduAddr;

    memcpy(m_SendBuf.pBuf,bySdFrame,ODUCALL2LEN);
    m_SendBuf.wWritePtr += ODUCALL2LEN;
    CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,1);
}
//ODU总召
void CPrtcOdu::SendODUCallFrame(void)
{
    m_SendBuf.wReadPtr = 0;
    m_SendBuf.wWritePtr=0;
    
    #define ODUCALLLEN 15
    BYTE bySdFrame[ODUCALLLEN]={0x67,0x09,0x09,0x67,0x53,0x01,0x64,0x01,0x06,0x01,0x00,0x00,0x14,0xD4,0x16};
  //BYTE bySdFrame[ODUCALLLEN]={0x67,0x09,0x09,0x67,0x53,0x02,0x64,0x01,0x06,0x02,0x00,0x00,0x14,0xD6,0x16};//ODU2 
  //  bySdFrame[5] = m_OduAddr;
  //  bySdFrame[9] = m_OduAddr;
    memcpy(m_SendBuf.pBuf,bySdFrame,ODUCALLLEN);
     
     m_SendBuf.wWritePtr +=ODUCALLLEN;
     CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,1);  
}


BOOL CPrtcOdu::SendFrameHead_ODU(BYTE Style, BYTE Reason,BYTE OduPara1, BYTE OduPara2, BYTE OduPara)
{
      m_SendBuf.wReadPtr = 0;
      m_SendBuf.wWritePtr=0;
      pSendFrame = (VIecOduFrame *)m_SendBuf.pBuf;
     
      //pSendFrame->Frame67.Start1  = pSendFrame->Frame67.Start2 = 0x67;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr]=0x67;
      m_SendBuf.wWritePtr+=3;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0x67;
    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0x48;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]= 1;//m_OduAddr;

      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=Style;
      if(Style == 0x2e)
      {
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=6;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0x24;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0;
      }
      else
      {
          pSendFrame->Frame67.VSQ_odu=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=Reason;
      //if(OduPara==10)
          //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=2;
      //else
         m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=OduPara1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=OduPara2;
      }

    return TRUE;
}


BOOL CPrtcOdu::SendFrameTail_ODU(BYTE Num,BYTE OduPara)
{
    pSendFrame->Frame67.Length1 = pSendFrame->Frame67.Length2 = m_SendBuf.wWritePtr - 4;
    if(Num==0)
      pSendFrame->Frame67.Control =0x53;
  else
    pSendFrame->Frame67.Control =0x48;
    

    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&pSendFrame->Frame67.Control, pSendFrame->Frame67.Length1);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x16;

    CommWrite((char *)m_SendBuf.pBuf,m_SendBuf.wWritePtr);
    return TRUE;
}


/***************************************************************
    Function：SearchOneFrame
        搜索一帧正确的报文
    参数：Buf, Len
        Buf 接收缓冲区头指针
        Len 接收缓冲区内有效数据的长度
    返回：DWORD数据，其中
        高字：处理结果
            #define FRAME_OK       0x00010000      //检测到一个完整的帧
            #define FRAME_ERR      0x00020000      //检测到一个校验错误的帧
            #define FRAME_LESS     0x00030000      //检测到一个不完整的帧（还未收齐）
        低字：已经处理的数据长度
***************************************************************/
DWORD CPrtcOdu::SearchOneFrame(BYTE *Buf, WORD Len)
{
    unsigned short FrameLen;
    //WORD wLinkAddress;
    if (Len < 5)
        return FRAME_LESS;

    pReceiveFrame = (VIecOduFrame *)Buf;
    switch(pReceiveFrame->Frame67.Start1)
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
            
         case 0x67:
            if (pReceiveFrame->Frame67.Length1 != pReceiveFrame->Frame67.Length2)
                return FRAME_ERR|1;
            if (pReceiveFrame->Frame67.Start2 != 0x67)
                return FRAME_ERR|1;
            FrameLen=pReceiveFrame->Frame67.Length1+6;
            if (FrameLen > Len)
                return FRAME_ERR|1;
            if (Buf[FrameLen-1] != 0x16)
                return FRAME_ERR|1;
            if (Buf[FrameLen-2] != (BYTE)ChkSum((BYTE *)&pReceiveFrame->Frame67.Control,pReceiveFrame->Frame67.Length1))
                return FRAME_ERR|1;
            return FRAME_OK|FrameLen;
        default:
            return FRAME_ERR|1;
    }
}
/***************************************************************
    Function：RecODUFault
        解析ODU故障报文
    参数：bOduAddr 只能为1或2(从pOdu串口得到的数据认为是前6路，pOdu1串口得到的数据为后6路)
    返回：未使用
***************************************************************/

BOOL CPrtcOdu::RecODUFault(BYTE bOduAddr)
{
  BYTE InfoNum;//故障个数
  BYTE bFltType;//故障类型
  ODUFTDA *pFaultCon;
  BYTE phase = 0;
  BYTE line = 0;
  int j = 0;
  int YcIndex = 0;
  int YcTIndex = 0;
  WORD wFltYxIndex = 0;//故障序号 
  WORD wComIndex = 0;//分相通道故障遥信序号
  WORD wAllComIndex = 0;//总通道状态遥信序号
  static WORD wComFltBk[2][2] = {0};//该数组用来备份24个相通道故障,按bit存储
  BYTE bAllComFltBk[12] = {0};//用来备份总通道故障,不需要保存 由6改为12
  BYTE *pCom;//指向通道故障标识位
  BYTE ComFltFlag = 0;//通道故障标识
  BYTE bComValTmp = 0;
  
  
  BYTE bAddrOffset = 0;//ODU地址偏移量
  if(bOduAddr == 1 || bOduAddr == 2) 
    bAddrOffset = bOduAddr - 1;
  else 
   return 0;
      
  InfoNum = (pReceiveFrame->Frame67.VSQ_odu & 0x7f);
  bFltType = pReceiveFrame->Frame67.COT_odu;//传输原因用来标识故障类型
  
  if(pReceiveFrame->Frame67.VSQ_odu > 0x80)//结构限定词用来区分通道故障和其他故障
  {//通道故障
    if(InfoNum == 0x4c)
    {//a
        for(j=0;j<24;j++)//将通道故障的24个遥信量放入g_gRmtInfo数组中，同时将之前的故障状态保存
        {//b
          
              pCom = &(pReceiveFrame->Frame67.Data[3]);
              phase = j&3;
              line = (j>>2)+(bAddrOffset*6);//根据点号得到对应的线路号，从0开始
              if(phase == 0) ComFltFlag = 0;
              if(pCom[j]) ComFltFlag = 1;
              
              //对应的遥测设置成无效
              if(pCom[j] && ( !(GETBIT(wComFltBk[bAddrOffset],j)) ) && ((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_COMMFLTVAL_MODE]) == 0))//通道故障时遥测为无效值
              {
                   wComIndex = RI_1A_ZSQST + j + (bAddrOffset*PHASE_NUM*6);
                   //对应的遥测设置成无效
                    YcIndex = YxIdToYcId(wComIndex);
                    YcTIndex = YxIdToYcId(wComIndex,1);
                    if(YcIndex != 255) 
                    {
                      g_gRmtMeas[YcIndex + RM_1A_FAULT_I] = INVALIDYC;
                      g_gRmtMeas[YcIndex + RM_1A_LOAD_I] = INVALIDYC;
                      g_ucRefYxYcTs |= BIT1;
                    }
                    //温度设置成无效
                    if(YcTIndex != 255)
                    {
                      //g_gRmtMeas[YcTIndex + RM_1A_CABLE_T] = INVALIDYC;
                      g_gRmtMeas[YcTIndex + RM_1A_CABLE_T_INT] = INVALIDYC;
                      g_ucRefYxYcTs |= BIT1;
                    }
              }
              
              //备份分相通道故障
              BYTE fLag = 0;
              if(g_LineFlag[line] == 1)//合成时的第一路不处理，第二路时一起处理并备份
                fLag = 1;
              else if(g_LineFlag[line] == 2)
              {
                if( ((1&&pCom[j]) == GETBIT(wComFltBk[bAddrOffset],j)) && ((1&&pCom[j-4]) == GETBIT(wComFltBk[bAddrOffset],(j-4))) )
                {
                  fLag = 1;
                }
                
                SETBIT(wComFltBk[bAddrOffset],j-4,(pCom[j-4]&&1));
                SETBIT(wComFltBk[bAddrOffset],j,(pCom[j]&&1));
              }
              else if(g_LineFlag[line] == 0)
              {
                  if((1&&pCom[j]) == GETBIT(wComFltBk[bAddrOffset],j))
                     fLag = 1;
                  SETBIT(wComFltBk[bAddrOffset],j,(pCom[j]&&1));
              }
              
              //SETBIT(wComFltBk[bAddrOffset],j,(pCom[j]&&1));
              
              if(!fLag)
              {
                //分相通道故障的合成
                if(g_LineFlag[line] == 2)//在后一路进行合成并生成SOE，合成的放在第一路的位置
                {
                    wComIndex = RI_1A_ZSQST + j - 4 + (bAddrOffset*PHASE_NUM*6);
                    bComValTmp = (pCom[j] || pCom[j-4])&&1;
                    CreatNewSoe(wComIndex,bComValTmp);
                }
                else if(g_LineFlag[line] == 0)
                {//不合并则直接生产SOE
        
                    wComIndex = RI_1A_ZSQST + j + (bAddrOffset*PHASE_NUM*6);
                    CreatNewSoe(wComIndex,pCom[j]);
                } 
              }
               //备份总通道故障===============
              if(phase == 3)
              {
                if(ComFltFlag) 
                  bAllComFltBk[line] = 1;
                else
                 bAllComFltBk[line] = 0;
                //总通道故障的合成
                if(g_LineFlag[line] == 2)//在后一路进行合成并生成SOE
                {
                  wAllComIndex = line - 1 + RI_1ZSQST; //+ bAddrOffset*6;//总故障序号
                  bComValTmp = (bAllComFltBk[line] || bAllComFltBk[line-1])&&1;
                  CreatNewSoe(wAllComIndex,bComValTmp);
                }
                else if(g_LineFlag[line] == 0)
                {//不合并则直接生产SOE
                  wAllComIndex = line+ RI_1ZSQST; //+ bAddrOffset*6;//总故障序号
                  CreatNewSoe(wAllComIndex,bAllComFltBk[line]);
                }
              }
              //==================================
        }//b
     }//a
  }
  else//短路接地温度故障
  {
    BYTE iFtType;
    if(bFltType == 0x05) iFtType = 0;//短路或接地故障
    else if(bFltType == 0x06) iFtType = 3;//温度故障
    else return 0;//类型错误则不解析
    
    pFaultCon = (ODUFTDA *)&(pReceiveFrame->Frame67.Data[1]);
    for(int i = 0; i < InfoNum;i++)
    {
        if((pFaultCon->byFaultVal && 1) != 1)  //该位为1时代表故障有效
         continue;
 
        line = pFaultCon->byLinePhase;
        line = (line >> 2) & 0x3F;//取出线路偏移 高6位  从0开始
        phase = pFaultCon->byLinePhase & 0x03;//取出相位偏移
        if(phase == 0) 
        {
          phase = 4;//特殊处理 O相
          line -= 1;//从报文中看出0序解出来的线路号比其他相的线路号大1
          if(bFltType == 0x05) iFtType = 1;//这里判断接地故障，因为短路,接地的类型标识都等于0x05，
        }
        else
        {
            if(bFltType == 0x05) iFtType = 0;
            else if(bFltType == 0x06)iFtType = 3;//温度故障
        }
        line += bAddrOffset*6;
        
        //取出故障遥信
        if(g_LineFlag[line] == 2) //当前路为合成时的后一路，则取前一路对应的故障点号
          line = line - 1;
        wFltYxIndex = LPFToYxId(line +1,phase,iFtType);
        
        if(RI_IN_DAERR == wFltYxIndex)  
        {
          SetRIStatus(RI_IN_DAERR, 1);
          continue;
        }
        if(g_gRmtLock[wFltYxIndex + RI_1A_SHORT] == 0)
        {
          g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
          CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);              
        }  
       pFaultCon++;     
    }
  }
  return TRUE; 
}
//解析ODU读参数报文
BOOL CPrtcOdu::RecODUPara()
  {
     BYTE *pData,*pData1;
     WORD wFileNa = 0;//文件名
     BYTE bSecNa = 0;//节名
     if(m_OduAddr == 1)
        wFileNa = 9;
     else if(m_OduAddr == 2)
        wFileNa = 10;
    
      pData = &pReceiveFrame->Frame67.Data[3];
      pData1= &pReceiveFrame->Frame67.Data[5];
      switch((*pData))
      {
            case 0x04://遥控
              bSecNa = 0;
              break;
            case 0x05://召唤某一组数据
              bSecNa = 1;
              break;
            case 0x09:
              if((*pData1)==1)      bSecNa = 2;
              else if((*pData1)==2) bSecNa = 3;
              break;
            case 0x01:
               bSecNa = 4;
               break;
            default:
               break;
       }
      if(m_RWPaSrcObj !=null)
        m_RWPaSrcObj->SendReadPa(wFileNa,bSecNa);
    return TRUE;    
  }
BOOL CPrtcOdu::RecODUPower_Supply()
{
    BYTE *pData;
    pData = &pReceiveFrame->Frame67.COT_odu;
   
    if((*pData == 4) && (pGprs != null))
    {
        m_RcvCloseZdFlag = ON;
        if((!g_batPowerSt)&&(CheckHaveDataToSd() == FALSE || !g_GprsPowerSt))
        {
          
          if(g_GprsPowerSt)
          {
            BYTE bBuf[2] = {0xD2,0XD2};
            MyPrintfD(bBuf,2);
            SendWCmdToOdu(0x02,3);
            CloseGPRS(0);
            OpenTimer(TM_Volt);
            g_SunHighNum = 0;
            g_gRunPara[RP_POWER_MODE]=QUASI_REALTIME;
           }
          else
          {
            BYTE bBuf[2] = {0xDB,0XDB};
            MyPrintfD(bBuf,2);
            SendWCmdToOdu(0x55,3);
          }
          m_RcvCloseZdFlag = OFF;//回复ODU可以关主板后此标识复位
          
        }
        else
        {
          SendWCmdToOdu(0x0A,3);//让ODU延时10分钟
          
        }
//          if(pGprs->m_reSendDelay > 0 || pGprs->m_resendflag)
//          {//GPRS通道正在故障重发计时中或正要重发故障，告诉ODU延迟10分钟关电源
//            SendWCmdToOdu(0x10,3);
//          }
//          else if((g_ucRefYxYcTs & BIT1) == BIT1)
//          {//有遥测要发送，告诉ODU延迟3分钟关电源
//            SendWCmdToOdu(0,3);
//          }
//          else
//          {
//            SendWCmdToOdu(0x55,3);
//            m_RcvCloseZdFlag = OFF;//回复ODU可以关主板后此标识复位
//          } 
    }
    return TRUE; 
}
BOOL CPrtcOdu::RecODUFault_yaoce(BYTE bOduAddr)
{
    BYTE InfoNum = 0;
    ODUYCDA *pOduYcCon;
    BYTE line,phase;//接收遥测报文的长度，线号
    BYTE line_NUM;//取值1-6，解析温度遥测
    WORD wComIndex = 0; //当前通道序号 
    WORD wYcIndex = 0;
    WORD wTmpIndex = 0;
    WORD wCurInfoOfset = 0;//当前信息偏移量
    WORD wCurTmInfoOfset = 0;//当前温度信息偏移量
    int iTmp1,iTmp2;
    BYTE bYcFlag[24]={0};//0=未收到有效数据 1=已收到有效数据
    BYTE bAddrOffset = 0;
     if(bOduAddr == 1 || bOduAddr == 2) //bOduAddr只能为1或2(从pOdu串口得到的数据认为是前6路，pOdu1串口得到的数据为后6路)
       bAddrOffset = bOduAddr - 1;
     else 
      return 0;
     InfoNum = pReceiveFrame->Frame67.VSQ_odu & 0x7f;
     pOduYcCon = (ODUYCDA *)&(pReceiveFrame->Frame67.Data[3]);

     
     for(int i = 0; i < InfoNum;i++)
     {
        if((pOduYcCon->byZsqType == 0x66) || (pOduYcCon->byZsqType == 0x55))//0x55=接收是3C1O指示器的值,0x66=接收是3C1O-T指示器的值
        {//a
          if(MAKEWORD(pOduYcCon->byIL,pOduYcCon->byIH) == ODUINITYC)//如果该相的电流值为ODU初始化的值则不进行解析
            continue; 
            
          //line = i>>2;//根据点号得到对应的线路号，从0开始
          line = (i>>2)+(bAddrOffset*6);
          line_NUM=(i>>2);
          phase = i&3;
          wCurInfoOfset = i + bAddrOffset*PHASE_NUM*6;

          if(g_LineFlag[line] == 2)
          {
            wComIndex = RI_1A_ZSQST + wCurInfoOfset - 4;//当前为合成的第2路，则通道状态应判断前一路
          }
          else
            wComIndex = RI_1A_ZSQST + wCurInfoOfset;
          if(GetRIStatus(wComIndex))//判断合成后的线路是否有通道故障，若合成的两路任何一相有通道故障，则对应另一路的该相遥测也不解析
            continue;
          
            //电流解析部分
            wYcIndex = RM_1A_LOAD_I + wCurInfoOfset;//负荷电流的点表
            g_gRmtMeas[wYcIndex] = MAKEWORD(pOduYcCon->byIL,pOduYcCon->byIH);

           
            g_gRmtMeas[wYcIndex] &= 0x7fff;
            if(pOduYcCon->byIH >= 0x80)//根据指示器报文的解析，>0x80时，遥测放大了10倍
              g_gRmtMeas[wYcIndex] = (g_gRmtMeas[wYcIndex]/10);
            bYcFlag[i] = 1;
          

             if(g_LineFlag[line] == 2)//在第二路进行合成
             {
                if(bYcFlag[i-4] == 1)//第一路数据有效则上传两路加和值
                {
                  g_gRmtMeas[wYcIndex-4] += g_gRmtMeas[wYcIndex];  
                }
                else//第一路为初始化值(未收到有效数据)则上传第二路的遥测
                {
                  g_gRmtMeas[wYcIndex-4] = g_gRmtMeas[wYcIndex];
                }
             }

          if((pOduYcCon->byZsqType == 0x66) && (phase != 3))//解温度 0序没有温度
          {
              wCurTmInfoOfset = line_NUM*(PHASE_NUM-1) + phase+ bAddrOffset*TEMP_PHASE_NUM*6;
              //外温内温解析部分===========
              for(int k = 0;k < 2;k++)
              {
                  if(k == 0)
                  {
                   // wTmpIndex = RM_1A_CABLE_T + wCurTmInfoOfset;
                    //g_gRmtMeas[wTmpIndex] = pOduYcCon->byExtTmpt;
                  }
                  else
                  {
                    wTmpIndex = RM_1A_CABLE_T_INT + wCurTmInfoOfset;
                    g_gRmtMeas[wTmpIndex] = pOduYcCon->byIntTmpt;
                  }
                  if((g_gRmtMeas[wTmpIndex])>0x80)
                  {
                     g_gRmtMeas[wTmpIndex] |= 0xFF00;
                  }
                  
                  if(g_LineFlag[line] == 2)//在第二路进行合成
                  {
                    if(bYcFlag[i - 4] == 1)//第一路数据有效
                    {
                      if((g_gRmtMeas[wTmpIndex - 3] != 0x80)&&(g_gRmtMeas[wTmpIndex] != 0x80))
                      {//两路数据都不等于0x80则上传两路中较大的温度值【0x80表示感温探头没插上】
                        iTmp1 = g_gRmtMeas[wTmpIndex - 3];
                        iTmp2 = g_gRmtMeas[wTmpIndex];
                        if(iTmp1 < iTmp2)
                          g_gRmtMeas[wTmpIndex - 3] = g_gRmtMeas[wTmpIndex];
                      }
                      else if(g_gRmtMeas[wTmpIndex - 3] == 0x80)
                      {
                        g_gRmtMeas[wTmpIndex - 3] = g_gRmtMeas[wTmpIndex];
                      }
                      
                    }
                    else//第一路为初始化值(未收到有效数据)则上传第二路的遥测
                    {
                     g_gRmtMeas[wTmpIndex - 3] = g_gRmtMeas[wTmpIndex];
                    }
                  }
              }
             //======================
          }
          g_ucRefYxYcTs |= BIT1;
        }//a
        pOduYcCon++;
     }
   return TRUE; 
}


//发送写指令到ODU
void CPrtcOdu::SendWCmdToOdu(WORD OduPara,BYTE ParaName, BYTE *pDa,CProtocol *pDaSrc)
{//pDa[0]表示具体数据
    BYTE Style = 125;
    BYTE Reason = 0X0D;
  BYTE ODU_para1 = 0Xea;
  BYTE ODU_para2 = 0X0c;
    SendFrameHead_ODU(Style, Reason,ODU_para1,ODU_para2,OduPara);
    
    if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
    switch(ParaName)
    {
      case 0://写光口配置        
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 4;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 24;
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],pDa+1,pDa[0]);
      m_SendBuf.wWritePtr +=24;
        break;
     case 1://写延时参数
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 5;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 4;
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],pDa+1,pDa[0]);//修改
        m_SendBuf.wWritePtr +=13;
        break;
   case 4://写延时参数
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],pDa+1,1);
      m_SendBuf.wWritePtr +=1;
        break;
   case 3://
    if(OduPara==0x55)
    {
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x4;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x55;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      //memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame68.Data[m_dwasdu.Infooff+6],4);
        //m_SendBuf.wWritePtr +=4;
    }
    else if(OduPara==0x10)
    {
    
    //m_reSendodu = 5;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x4;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x0A;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      //memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame68.Data[m_dwasdu.Infooff+6],4);
        //m_SendBuf.wWritePtr +=4;
    }
    else if(OduPara==0)
    {
    
    //m_reSendodu = 5;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x4;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 3;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      //memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame68.Data[m_dwasdu.Infooff+6],4);
        //m_SendBuf.wWritePtr +=4;
    }
     break;
  case 6://打开GPRS电源
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 7;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x55;
     break;
  case 7://关断狦PRS电源
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 7;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      break;  
     
     }
    SendFrameTail_ODU(1,OduPara);
   if((ParaName==3)||(ParaName==6)||(ParaName==7))
   {
    memcpy(&m_oduSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
    m_oduSendLen = m_SendBuf.wWritePtr;
    m_WrPaWaitflag = ON;
    m_WrPaWaitCt = 10;
    m_ReTxNum = 0;
   }
   m_ReadPaTp = ParaName;
     return ;
}
//复位ODU
void CPrtcOdu::RstOdu(void)
{
        m_SendBuf.wReadPtr = 0;
        #define RSTODU_LEN  15
        BYTE bySdFrame[RSTODU_LEN]={0x67,0x09,0x09,0x67,0x53,0x01,0x2E,0x06,0x01,0x01,0x00,0x24,0x00,0xAE,0x16};
        memcpy(m_SendBuf.pBuf,bySdFrame,RSTODU_LEN);
        m_SendBuf.wWritePtr += RSTODU_LEN;
        CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,1);    
        return;
}

void CPrtcOdu::SendRCmdToOdu(WORD OduPara,BYTE ParaName,CProtocol *pDaSrc)
{
      BYTE Style = 122;
      BYTE Reason = 0X0D;
      BYTE ODU_para1 = 0Xea;
      BYTE ODU_para2 = 0X0c;
      if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
          
       SendFrameHead_ODU(Style, Reason,ODU_para1,ODU_para2,OduPara);
       switch(ParaName)
        {
          case 0://读光口配置
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 4;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;
            break;
         case 1://读延时参数
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 5;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
       case 2://读主板149版本号
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 9;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
         case 3://读子板2247版本号
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 9;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
      case 4://读ODU地址
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
  
         }
      SendFrameTail_ODU(0,OduPara);
      return;
}

#endif



