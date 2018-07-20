
#ifndef _FUNCDEF_H

  #define _FUNCDEF_H
#ifdef __cplusplus
extern "C"
{
#endif
//======================================================================
//                main.c--主函数
//======================================================================
#ifdef _MAIN
        void app();
        void ExitGprsSet(void);
        void WatchDog(void);
        void InitSys(void); //完成系统初始化的工作，包括系统时钟设置，外设寄存器初始化，
        void HandleTimerEvent(void);
    	bool TimeOut(int flag);
    	void CheckRmtLock(void);
		void RefreshPara(void);
		bool CheckInLPM3(void);
		void InitCommObj(void);
                void InitGPRS(void);	
				void InitGps(void);
				void GPS_Config(BYTE Type);
                void CloseGPRS(BYTE bDa = 0,BYTE Close_Type = 0);//第一个参数区别是GPRS打开5分钟后关还是进入准实时模式关
                                                                   //第二个参数说明时遥控关还是普通的关用以在关闭GPRS模块时检测是否是实时模式
		void OpenGPRS(BYTE bDa = 0);
		BOOL CheckHaveDataToSd();
               void Computed_error_difference(void);
#else
        extern void ExitGprsSet(void);
        extern void WatchDog(void);
    	extern bool TimeOut(int flag);
    	extern void CheckRmtLock(void);
		extern void RefreshPara(void);
		extern void InitCommObj(void);
                extern void InitGPRS(void);	
				extern void InitGps(void);
				extern void GPS_Config(BYTE Type);
        extern void CloseGPRS(BYTE bDa = 0,BYTE Close_Type = 0);
		extern void OpenGPRS(BYTE bDa = 0);
		extern BOOL CheckHaveDataToSd();
                extern void app();
          extern void Computed_error_difference(void);

#endif

//======================================================================
//                DataProcess.c--数据处理模块
//======================================================================
#ifdef _DATA_PROCESS
        void InitDataProc(void);
        void CalcuRmtMeas(void);
        void SaveSoeData(void);
        void VoltLogicCtr();
		void VoltLogicCtr_cable();
        void CreatNewSoe(unsigned int SOEtype,unsigned long SOEvalue,unsigned char Flag = 0);
		void CreatNewSoe_time(unsigned int SoeType,unsigned long SoeValue,unsigned int * pAbsolutetime);
		BOOL Absolute_time_calculated(unsigned int minute_count,DWORD relative_time,unsigned int * pabsolute_time);
		int ReadSoe(unsigned char *pBuf,int iSegNo,int iStartNo,int iSoeNum);
        
        void RstRMTInfo(unsigned int SoeType);
        void RstRMTMeas();
        void SetYxTrans(void);
        void SetYcTrans(void);
        unsigned int RealYxToTsYx(unsigned int RYxNo);
        void read_ptr(void);
		void SaveLoad(unsigned char LoadFlag);
		void SaveLog(unsigned int LogType,char *pLogText,unsigned char LogValue,INT8U Log_Length);
		unsigned int GetRIStatus( unsigned int RIIndex , BYTE RmtInfoFlag = 0);
		void SetRIStatus( unsigned int RIIndex , unsigned int Status , BYTE RmtInfoFlag = 0);
        void OpenBatSmsGprs();
		void CopySoeToFlash(void);
		void Check_LogFile(void);
		void Check_LogFlag(void);
        float GetYcCoef(WORD wYcId);  
		
		
#else
        extern void InitDataProc(void);
        extern void CalcuRmtMeas(void);
        extern void SaveSoeData(void);
        extern void VoltLogicCtr();
		extern void VoltLogicCtr_cable();
        extern void CreatNewSoe(unsigned int SOEtype, unsigned long SOEvalue,unsigned char Flag = 0);
		extern void CreatNewSoe_time(unsigned int SoeType,unsigned long SoeValue,unsigned int * pAbsolutetime);
		extern BOOL Absolute_time_calculated(unsigned int minute_count,DWORD relative_time,unsigned int * pabsolute_time);
		extern int ReadSoe(unsigned char *pBuf,int iSegNo,int iStartNo,int iSoeNum);
        
        extern void RstRMTInfo(unsigned int SoeType);
        extern void RstRMTMeas();
        extern void SetYxTrans(void);
        extern void SetYcTrans(void);
        extern unsigned int RealYxToTsYx(unsigned int RYxNo);
        extern void read_ptr(void);
		extern void SaveLoad(unsigned char LoadFlag);;
		extern void SaveLog(unsigned int LogType,char *pLogText,unsigned char LogValue,INT8U Log_Length);
		extern unsigned int GetRIStatus( unsigned int RIIndex , BYTE RmtInfoFlag = 0);
		extern void SetRIStatus( unsigned int RIIndex , unsigned int Status, BYTE RmtInfoFlag = 0 );
        extern void OpenBatSmsGprs();
		extern void CopySoeToFlash(void);
		extern void Check_LogFile(void);
		extern void Check_LogFlag(void);
        extern float GetYcCoef(WORD wYcId);
#endif
//======================================================================
//                  ParaCfg.c--参数配置模块                
//======================================================================
#ifdef _PARA_CFG
        void InitcfgPara(void);
        void SaveCfgPara(void); 
        void CheckCfgPara(void);
    	void InitTsYxInfo(void);
        void InitTsYcInfo(void);
        void RstRunPara(void);
    	void RstIEC101Para(void );
        void RstIEC101YxAddr(void);
        void RstIEC101YcAddr(void);
        void RstIEC101YkAddr(void);
        void RstIEC101YcSiqu(void);
		void RstSmsPhoneInfo(void);
                void SetLbNum();
                
#else 
        extern void InitcfgPara(void);   
        extern void SaveCfgPara(void);
        extern void CheckCfgPara(void);
        extern void InitTsYxInfo(void);
        extern void InitTsYcInfo(void);
        extern void RstRunPara(void);
        extern void RstIEC101Para(void );
        extern void RstIEC101YxAddr(void);
        extern void RstIEC101YcAddr(void);
        extern void RstIEC101YkAddr(void);
        extern void RstIEC101YcSiqu(void);
		extern void RstSmsPhoneInfo(void);
                extern void SetLbNum();
        
#endif


//======================================================================
//                  Comm.c--通信模块                
//======================================================================
#ifdef _COMM
        void InitComm(void);
        void InitCommProgram(void);
        int CommRcvData(unsigned char *pBuf,int MaxLen,int uartId);
        int CommSendData(unsigned char *pBuf,int BufLen,int uartId);
        void CheckCommSt(void);
        unsigned int YxIdToYcId(unsigned int RYxNo,unsigned int IorT = 0);
		unsigned int YcIdToYxId(unsigned int RYxNo);
        unsigned int CrcCount(unsigned int *p, unsigned int Length);
        unsigned char AddChar(unsigned char *p, unsigned int length);
		unsigned char XorChar(unsigned char *p, unsigned int length);
		unsigned char CharToHex(unsigned char charl,unsigned char charh);
                
         unsigned int Char3ToHex(unsigned char char1,unsigned char char2,unsigned char char3);
		unsigned char HexToChar(unsigned char bHex);
		unsigned short Fcs16(unsigned short fcs, unsigned char *cp, unsigned short len);
        unsigned short ByAddCrc16(unsigned char *cp, unsigned short len);
		void MyPrintf(char *ch);
		void MyPrintfD(BYTE *p,WORD wLen);
		WORD LPFToYxId(BYTE bLine,BYTE bPhase,BYTE bFtType);
		void SetAllYxStatus(WORD RYxNo);
		
                
#else 
        extern void InitComm(void); 
        extern void InitCommProgram(void);
        extern void CheckCommSt(void);
        extern unsigned int YxIdToYcId(unsigned int RYxNo,unsigned int IorT = 0);
		extern unsigned int YcIdToYxId(unsigned int RYxNo);
        extern int CommRcvData(unsigned char *pBuf,int MaxLen,int uartId);
        extern int CommSendData(unsigned char *pBuf,int BufLen,int uartId);
        extern unsigned int CrcCount(unsigned int *p, unsigned int Length);
       extern unsigned char AddChar(unsigned char *p, unsigned int length);
	   extern unsigned char XorChar(unsigned char *p, unsigned int length);
	   //extern unsigned char CharToHex(unsigned char bHex);
	   	extern unsigned char CharToHex(unsigned char charl,unsigned char charh);
        
       extern unsigned int Char3ToHex(unsigned char char1,unsigned char char2,unsigned char char3);
        extern unsigned char HexToChar(unsigned char bHex);
       extern unsigned short Fcs16(unsigned short fcs, unsigned char *cp, unsigned short len);
        extern unsigned short ByAddCrc16(unsigned char *cp, unsigned short len);
		extern void MyPrintf(char *ch);
		extern void MyPrintfD(BYTE *p,WORD wLen);
		extern WORD LPFToYxId(BYTE bLine,BYTE bPhase,BYTE bFtType);
		extern void SetAllYxStatus(WORD RYxNo);
        
#endif

  //======================================================================
//                  DriveI2C.c--I2C模块                
//======================================================================
#ifdef _DRIVE_DMA
          void I2C_start(void);
          void I2C_senddata(unsigned char I_data);
          void I2C_stop(void);
          void I2C_sendack(void);
          void I2C_sendnotack(void);
          INT8U  I2C_ack(void);
          INT8U I2C_receivedata(void);
          void TurnOffI2C(void);

#else
          extern void I2C_start(void);
          extern void I2C_senddata(unsigned char I_data);
          extern void I2C_stop(void);
          extern void I2C_sendack(void);
          extern void I2C_sendnotack(void);
          extern INT8U  I2C_ack(void);
          extern INT8U I2C_receivedata(void);
          extern void TurnOffI2C(void);
        
#endif      
        

//======================================================================
//                  DriverTimer.c--定时器驱动模块                
//======================================================================
#ifdef _DRIVER_TIMER
        void InitTimer(void);
        void OpenTimer(int TmId,unsigned int TmVal = 0);
		void RefreshTmCount(unsigned int TmId);
        void TimeAdd(WORD *pRtClock,WORD wYear,BYTE byMonth,BYTE byDay,BYTE byHour,BYTE byMin,BYTE bySec,WORD wMs);
		void TimeSub(WORD *pRtClock,WORD wYear,BYTE byMonth,BYTE byDay,BYTE byHour,BYTE byMin,BYTE bySec,WORD wMs);
#else
        extern void InitTimer(void);
        extern void OpenTimer(int TmId,unsigned int TmVal = 0);
		extern void RefreshTmCount(unsigned int TmId);
		extern void TimeAdd(WORD *pRtClock,WORD wMs = 0,BYTE bySec = 0,BYTE byMin = 0,BYTE byHour = 0,BYTE byDay = 0,BYTE byMonth = 0,WORD wYear = 0);
		extern void TimeSub(WORD *pRtClock,WORD wMs = 0,BYTE bySec = 0,BYTE byMin = 0,BYTE byHour = 0,BYTE byDay = 0,BYTE byMonth = 0,WORD wYear = 0);
        
#endif

//======================================================================
//                  DriverAD.c--通信驱动模块                
//======================================================================
#ifdef _DRIVER_AD
        void InitAdc(void);
        void DisAd0Sample(void);
        void EnAd0Sample(void);
        void Open_BatSunC_Para(unsigned int BatNo);
        void Close_BatSunC_Para(void);
        
#else
        extern void InitAdc(void);
        extern void DisAd0Sample(void);
        extern void EnAd0Sample(void);
        extern void Open_BatSunC_Para(unsigned int BatNo);
        extern void Close_BatSunC_Para(void);
        
#endif

//======================================================================
//                  DriverComm.c--通信驱动模块                
//======================================================================
#ifdef _DRIVER_COMM
        void InitSCI(void);
        void SendBytes_232(unsigned char *Data, int ucRxdlen);
        BOOL Operation_Rxbuf(unsigned char RxBuf);
        void SendBytes_GPRS(unsigned char *Data,int ucRxdlen);
        void SendBytes_RF(unsigned char *Data, int ucRxdlen);
        void SendBytes_UART2(unsigned char *Data,int ucRxdlen);
		void Uart3Init(void);
		void CloseUart3(void);
        
#else
        extern void InitSCI(void);
        extern void InitSCI0(void);
        extern void SendBytes_232(unsigned char *Data,int ucRxdlen);
        extern BOOL Operation_Rxbuf(unsigned char RxBuf);
        extern void SendBytes_GPRS(unsigned char *Data,int ucRxdlen);
        extern void SendBytes_RF(unsigned char *Data,int ucRxdlen);
        extern void SendBytes_UART2(unsigned char *Data,int ucRxdlen);
		extern void Uart3Init(void);
		extern void CloseUart3(void);
#endif

//======================================================================
//                  DriverEEprom.c--通信驱动模块                
//======================================================================
#ifdef _DRIVER_EEPROM
        void UCB2IICInit(void);
        unsigned char ReadWord(unsigned int  unAddr, unsigned int unSlaveAddr);
        void WriteWord(unsigned int unAddr, unsigned int unSlaveAddr, unsigned char ucWriteData);

        void EEP_ReadBytes(unsigned int unAddr,unsigned int unLength,unsigned char *Data);
        void EEP_WriteBytes(unsigned int unAddr, unsigned int unLength,unsigned int unSlaveAddr, unsigned char *Data);

        unsigned char BCDtoINT(unsigned char ucValue);
        unsigned char INTtoBCD(unsigned char ucValue);
        void UCB0SPIInit(void);
        
        void CAT_SpiConcelWP(void);
        void CAT_SpiWriteByte(unsigned int unAddr,unsigned char ucData);
        void CAT_SpiWriteBytes(unsigned int unAddr,unsigned int unLength,unsigned char *Data);
        unsigned char CAT_SpiReadByte(unsigned int unAddr);
        void CAT_SpiReadBytes(unsigned int unAddr,unsigned int unLength,unsigned char *Data);
        void CAT_SpiWriteWord(unsigned int unAddr,unsigned int unData);
        void CAT_SpiWriteWords(unsigned int unAddr,unsigned int unLength,unsigned int *Data);
        unsigned int CAT_SpiReadWord(unsigned int unAddr);
        void CAT_SpiReadWords(unsigned int unAddr,unsigned int unLength,unsigned int *Data);

        
#else
        extern void UCB2IICInit(void);

        extern unsigned char ReadWord(unsigned int  unAddr, unsigned int unSlaveAddr);
        extern void WriteWord(unsigned int unAddr, unsigned int unSlaveAddr, unsigned char ucWriteData);

        extern void EEP_ReadBytes(unsigned int unAddr,unsigned int unLength,unsigned char *Data);
        extern void EEP_WriteBytes(unsigned int unAddr, unsigned int unLength,unsigned int unSlaveAddr, unsigned char *Data);

        extern unsigned char BCDtoINT(unsigned char ucValue);
        extern unsigned char INTtoBCD(unsigned char ucValue);
        extern void UCB0SPIInit(void);
        extern void CAT_SpiConcelWP(void);
        extern void CAT_SpiWriteByte(unsigned int unAddr,unsigned char ucData);
        extern void CAT_SpiWriteBytes(unsigned int unAddr,unsigned int unLength,unsigned char *Data);
        extern unsigned char CAT_SpiReadByte(unsigned int unAddr);
        extern void CAT_SpiReadBytes(unsigned int unAddr,unsigned int unLength,unsigned char *Data);
        extern void CAT_SpiWriteWord(unsigned int unAddr,unsigned int unData);
        extern void CAT_SpiWriteWords(unsigned int unAddr,unsigned int unLength,unsigned int *Data);
        extern unsigned int CAT_SpiReadWord(unsigned int unAddr);
        extern void CAT_SpiReadWords(unsigned int unAddr,unsigned int unLength,unsigned int *Data);

#endif

//======================================================================
//                  DriverFlash.c--通信驱动模块                
//======================================================================
#ifdef _DRIVER_FLASH
        //内部flash
        void EraseFlash(INT32U addr);
        void WriteFlashNByte(INT32U dst, INT16U src, INT8U len);
        void WriteFlashStru(INT32U dst);
        void WriteFlashBuff(INT32U dst, INT8U *data, INT8U len);
        void WriteFlashBuff1(INT32U dst, INT8U *data, INT8U len);
        void Read_para(long *src, char *des,int count);
        void WriteFlashBuffWord(INT32U dst, INT16S *data, INT8U len);
        void WriteFlashBuffLongWord(INT32U dst, INT32S *data, INT8U len);
        void WriteFlashByte(INT32U dst, INT8U data);
        void WriteFlashWord(INT32U dst, INT16U data);
        
        //外部flash

        void PageErase(unsigned int unPage);
        void ReadContinuePage(unsigned long ulAddr,unsigned char *PcBuf, unsigned int unsize);
        void DBWriteBytes(unsigned long ulAddr,unsigned char *PcBuf,unsigned int unSize);
        void DBWriteWords(unsigned long ulAddr,unsigned int *PcBuf,unsigned int unSize);

		
        void Sst26vf064b_Page_WriteBytes(long unAddr,unsigned char *PcData,unsigned int Size);
        void Sst26vf064b_Page_WriteWords(long unAddr,unsigned int *PcData,unsigned int Size);
        void Sst26vf064b_Read(long unAddr,unsigned char *PcData,unsigned int Size);
        void Sst_Reset(void);
        void Sst_Unlock(void);
        void Sector_Erase(long unBlock);
        void Sst_ReadID(unsigned char *PcData,unsigned int Size);
        void Block_Erase(long unBlock);
        void SpiWaitBusy(void);

        
#else
        //内部flash
        extern  void EraseFlash(INT32U addr);
        extern  void WriteFlashNByte(INT32U dst, INT16U src, INT8U len);
        extern  void WriteFlashStru(INT32U dst);
        extern  void WriteFlashBuff(INT32U dst, INT8U *data, INT8U len);
        extern  void WriteFlashBuff1(INT32U dst, INT8U *data, INT8U len);
        extern  void Read_para(long *src, char *des,int count);
        extern  void WriteFlashBuffWord(INT32U dst, INT16S *data, INT8U len);
        extern  void WriteFlashBuffLongWord(INT32U dst, INT32S *data, INT8U len);
        extern  void WriteFlashByte(INT32U dst, INT8U data);
        extern  void WriteFlashWord(INT32U dst, INT16U data);
        //外部flash
        extern void ReadContinuePage(unsigned long ulAddr,unsigned char *PcBuf, unsigned int unsize);
        extern void DBWriteBytes(unsigned long ulAddr,unsigned char *PcBuf,unsigned int unSize);
        extern void DBWriteWords(unsigned long ulAddr,unsigned int *PcBuf,unsigned int unSize);
        extern void DBWriteBytes_ForLoad(unsigned long ulAddr,unsigned char *PcBuf,unsigned int unSize);

		
        extern void Sst26vf064b_Page_WriteBytes(long unAddr,unsigned char *PcData,unsigned int Size);
        extern void Sst26vf064b_Page_WriteWords(long unAddr,unsigned int *PcData,unsigned int Size);
        extern void Sst26vf064b_Read(long unAddr,unsigned char *PcData,unsigned int Size);
        extern void Sst_Reset(void);
        extern void Sst_Unlock(void);
        extern void Sector_Erase(long unBlock);
        extern void Sst_ReadID(unsigned char *PcData,unsigned int Size);
        extern void Block_Erase(long unBlock);
        extern void SpiWaitBusy(void);


#endif
//======================================================================
//                  DriverIO.c--IO驱动模块                
//======================================================================
#ifdef _DRIVER_IO
        void InitClk(void);
        void IoInit(void);
        void InitDin(void);
        bool ScanDin(void);
        void ScanOut(void);
        void ActOut(unsigned int OutType, unsigned int OutValue);
        void ScanClsTrip(void);
        bool WatchWire(void);               //测试无线模块喂狗信号
        
#else
        extern void InitClk(void);
        extern void IoInit(void);
        extern void InitDin(void);
        extern bool ScanDin(void);
        extern void ScanOut(void);
        extern void ActOut(unsigned int OutType, unsigned int OutValue);
        extern void ScanClsTrip(void);
        extern bool WatchWire(void);    
#endif

//======================================================================
//                  DriverRTC.c--时钟芯片驱动模块                
//======================================================================
#ifdef _DRIVER_RTC
        unsigned int* ReadRealTime(unsigned char type);
        void WriteRealTime(unsigned int *RealTime);
        void InitRtc(void);
		void WriteRTCTime(void);
        void RunRTClock(INT16U AddHour);
#else
        extern unsigned int* ReadRealTime(unsigned char type);
        extern void WriteRealTime(unsigned int *RealTime);
        extern void InitRtc(void);
		extern void WriteRTCTime(void);
        extern void RunRTClock(INT16U AddHour);
#endif

//======================================================================
//                  Delay.c--时钟芯片驱动模块                
//======================================================================
#ifdef _DELAY
        void delay(unsigned long unDelayNum);
        void delayms(unsigned int unMSNum);
        void delay_1ms(INT16U dat);
        
#else
        extern void delay(unsigned long unDelayNum);
        extern void delayms(unsigned int unMSNum);
        extern void delay_1ms(INT16U dat);

#endif
        
#ifdef __cplusplus
}
#endif

//======================================================================
//                  智芯微加密芯片驱动模块                
//======================================================================
#ifdef _DRIVER_ENCRYPT
        unsigned char CheckLrc (unsigned char *ptr,int num);
        int SendCmdToEncChip(const char *sCmd,BYTE *EncDate,WORD DataLen);
        int RecBytesFromEsam(unsigned char *pRecDataTemp);
        
        
#else
        extern unsigned char CheckLrc (unsigned char *ptr,int num);
        extern int SendCmdToEncChip(const char *sCmd,BYTE *EncDate,WORD DataLen);
        extern int RecBytesFromEsam(unsigned char *pRecDataTemp);
		
#endif
//================================== MD5校验 ===========================================
                
//#define MD5_LONG unsigned int
#define MD5_LONG unsigned long int


#define MD5_CBLOCK	64
#define MD5_LBLOCK	(MD5_CBLOCK/4)
#define MD5_DIGEST_LENGTH 16

typedef struct MD5state_st
{
	MD5_LONG state[5];
	MD5_LONG count[2];
	unsigned char buffer[MD5_CBLOCK];
} MD5_CTX;
#ifdef _MD5_DGST_
    void MD5_Init(MD5_CTX *ctx);
    void MD5_Update(MD5_CTX *ctx, const unsigned char *data, size_t len);
    void MD5_Final(MD5_CTX *ctx, unsigned char *md);
    void MD5(const unsigned char *d, size_t n, unsigned char *md);
#else
    extern void MD5_Init(MD5_CTX *ctx);
    extern void MD5_Update(MD5_CTX *ctx, const unsigned char *data, size_t len);
    extern void MD5_Final(MD5_CTX *ctx, unsigned char *md);
    extern void MD5(const unsigned char *d, size_t n, unsigned char *md);
#endif

//======================================================================
//                  PrtcSms.c--短信上报驱动模块                
//======================================================================


#endif

