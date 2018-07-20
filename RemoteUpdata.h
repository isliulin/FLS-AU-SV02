#ifndef _REMOTE_UPDATA_H_
#define _REMOTE_UPDATA_H_


#ifdef  REMOTE_UPDATA_GLOBALS
#define REMOTE_UPDATA_EXT
#else
#define REMOTE_UPDATA_EXT  extern
#endif



//============================   在线升级   ======================================
#define WRITEPACK  232//写入包数据个数 基地址（3）+偏移地址（3）+数据量（1）+数据内容（224）+累加和（1）
#define V_OFFSET 2//软件版本内容在VerNum中的偏移量，取以后5个字节为软件版本号 
#define STATE_LOADING        0xAA //代码下载中
#define STATE_ALLCHECK1_YES  0x67//代码校验1成功
#define STATE_ALLCHECK2_YES  0xCD//打包校验2成功
#define STATE_COMPLETE       0x58//更新完成
#define STATE_FLASHWRITE_NO  0x42//flash写入错误
#define STATE_SCHECK_NO      0x49//单帧数据读取校验错误
#define STATE_ALLCHECK1_NO   0x45//打包校验1错误
#define STATE_ALLCHECK2_NO   0x23//打包校验2错误


//在线更新存储
//上一次更新参数
#define EEPUPDATE_LAST_VERSION     0x2130 //上次更新版本号
#define EEPUPDATE_LAST_STATE       0X2136 //上次的升级状态
#define EEPUPDATE_LAST_SEG         0X213A //段总数    、、以下数据连续存储
#define EEPUPDATE_LAST_SEGNo       0X213B //断点段序号
#define EEPUPDATE_LAST_LENGTH      0X213C //段总长
#define EEPUPDATE_LAST_BASE        0X2140 //段基址
#define EEPUPDATE_LAST_OFFSET      0x2144 //偏移地址
#define EEPUPDATE_LAST_COUNTER     0x2148 //包序号
#define EEPUPDATE_LAST_INCOUNTER   0x214A //段内包序号
#define EEPUPDATE_LAST_CHECK       0X214C //打包校验值
#define EEPUPDATE_LAST_TOTAL       0x214E //数据总数
//备份
#define EEPUPDATE_LAST_BKSEG         0X2170 //段总数    、、以下数据连续存储
#define EEPUPDATE_LAST_BKSEGNo       0X2171 //断点段序号
#define EEPUPDATE_LAST_BKLENGTH      0X2172 //段总长
#define EEPUPDATE_LAST_BKBASE        0X2176 //段基址
#define EEPUPDATE_LAST_BKOFFSET      0x217A //偏移地址
#define EEPUPDATE_LAST_BKCOUNTER     0x217E //包序号
#define EEPUPDATE_LAST_BKINCOUNTER   0x2180 //段内包序号
#define EEPUPDATE_LAST_BKCHECK       0X2182 //打包校验值
#define EEPUPDATE_LAST_BKTOTAL       0x2184 //数据总数

#define EEPUPDATE_STATE_NO         26//代码下载过程中的中间状态此时升级状态为升级进行中
#define EEPUPDATE_STATE_SEG         0//段总数
#define EEPUPDATE_STATE_SEGNo       1//段序号
#define EEPUPDATE_STATE_LENGTH1     2//段总长低字节
#define EEPUPDATE_STATE_LENGTH2     3
#define EEPUPDATE_STATE_LENGTH3     4//段总长高字节
#define EEPUPDATE_STATE_LENGTH4     5//固定为零，预留
#define EEPUPDATE_STATE_BASE1       6//基地址低字节
#define EEPUPDATE_STATE_BASE2       7
#define EEPUPDATE_STATE_BASE3       8//基地址高字节
#define EEPUPDATE_STATE_BASE4       9//固定为零，预留
#define EEPUPDATE_STATE_OFFSET1     10//偏移地址低字节
#define EEPUPDATE_STATE_OFFSET2     11//
#define EEPUPDATE_STATE_OFFSET3     12//高字节
#define EEPUPDATE_STATE_OFFSET4     13//固定为零，预留
#define EEPUPDATE_STATE_COUNTER1     14//包序号低字节
#define EEPUPDATE_STATE_COUNTER2     15//包序号高字节
#define EEPUPDATE_STATE_INCOUNTER1   16 //段内包序号低字节
#define EEPUPDATE_STATE_INCOUNTER2   17 //段内包序号高字节
#define EEPUPDATE_STATE_CHECK1       18 //打包校验值低字节
#define EEPUPDATE_STATE_CHECK2       19 //打包校验值高字节
#define EEPUPDATE_STATE_TOTAL1       20 //数据总数低字节
#define EEPUPDATE_STATE_TOTAL2       21 //
#define EEPUPDATE_STATE_TOTAL3       22 //
#define EEPUPDATE_STATE_TOTAL4       23 //数据总数高字节
#define EEPUPDATE_STATE_CRCL         24 //数据总数低字节
#define EEPUPDATE_STATE_CRCH         25 //数据总数高字节

//    FUNC_LOAD
#define Reason_160 200//从机响应帧 主机发送帧
#define Reason_163 203
#define Reason_164 204
#define Reason_165 205
#define Reason_166 206
#define Reason_170 210


REMOTE_UPDATA_EXT unsigned char g_unGPRSTX_Buff[195];          //GPRS数据发送数组 
//REMOTE_UPDATA_EXT unsigned char g_unRX_Buff[MAX_RXBUFF_LEN];            //数据接收数组
REMOTE_UPDATA_EXT int g_Cmid;


REMOTE_UPDATA_EXT void RemoteUpdata(unsigned char *p , unsigned char len);
REMOTE_UPDATA_EXT void Code_Load(unsigned char *p,unsigned char *pTxBuf);


#endif
