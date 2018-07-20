//��MSP430�е�Long�͵������ǵ�����ǰ�������ں�

#ifndef _CONSTANT_H
#define _CONSTANT_H 

#define DEBUG
//#define GETSOEFROMRAM   ���͵�SOE���ⲿFLASHȡ�����Ǵ��ڲ�ȡ�����ⲿȡ
//#define ZDSENDADDR //����������վ��ʱ��������վ��ַ(����Ӧ��)
//#define FILTER_COM �����ᷢ���˲���Ĳ���
//#define PHASELUBO


#define         TYPE_EQU                0x1111  //�豸����

#define        INVALIDYC  0xD8F0//-9999
#define        INVALTEYC  0xFF9D//-99
#define        INITYC     0xDD47//-8888
#define        ODUINITYC     0xFFFF//ODU��ʼ��ֵ


 
#define         YES                     0x55
#define         NO                      0xAA
#define         ON                      0x5F
#define         OFF                     0x50
#define         CLOSE                   0x55
#define         TRIP                    0xAA

#define  RP_LBSTORAGE_INVALID        0XFFFF  //¼����Ч����Ϊ��ֵʱ����ʾ¼�����ܸò���������
#define  RP_LUBO_INVALID                 64//������Ϊ��ֵʱ����Ч

#define  OPERATING_LOGO1 1 //������ʶ1 ��Ŀ¼
#define  OPERATING_LOGO2 2//������ʶ2 ��Ŀ¼ȷ��
#define  OPERATING_LOGO3 3//���ļ�����
#define  OPERATING_LOGO4 4 //���ļ�����ȷ��
#define  OPERATING_LOGO5 5 //���ļ�������Ӧ
#define  OPERATING_LOGO6 6 //���ļ�������Ӧȷ��

#define  GPS_PCAS00     0 //����������Ϣ
#define  GPS_PCAS01     1//���ڲ���������
#define  GPS_PCAS02     2//���ö�λ������
#define  GPS_PCAS03     3 //��ʼ��ֹͣ���NMEA���
#define  GPS_PCAS04     4 //���ù���ϵͳ
#define  GPS_PCAS05     5 //����NMEA����еķ�������ʶ��
#define  GPS_CCSIR      6 //��ǰϵͳ�����źż��豸״̬
#define  GPS_CCRM0      7 //��ʼ��ֹͣ���NMEA���
#define  GPS_PCAS10     10 //���ջ�����
#define  GPS_PCAS11     11 //���õ�ǰ����ƽ̨ģ��
#define FSOE_NUM_MAX    8//22  ���������˹����¼� 22��Ϊ8

#define  LUBO_FILE     0x01 //¼���ļ�
#define  FIXPT_FILE     0x02 //�����¼�ļ�
#define  SOE_FILE       0x04 //SOE�ļ�
#define  ULOG_FILE       0x08 //LOG�ļ�




/*-------------------------------------------------------------------------
 *   Standard Bits
 *-------------------------------------------------------------------------*/
#define NBIT0                (0xFFFE)
#define NBIT1                (0xFFFD)
#define NBIT2                (0xFFFB)
#define NBIT3                (0xFFF7)
#define NBIT4                (0xFFEF)
#define NBIT5                (0xFFDF)
#define NBIT6                (0xFFBF)
#define NBIT7                (0xFF7F)
#define NBIT8                (0xFEFF)
#define NBIT9                (0xFDFF)
#define NBITA                (0xFBFF)
#define NBITB                (0xF7FF)
#define NBITC                (0xEFFF)
#define NBITD                (0xDFFF)
#define NBITE                (0xBFFF)
#define NBITF                (0x7FFF)

#define LINE_NUM   3 //12  //֧�ֵ�ָʾ����·����
#define PHASE_NUM  4  //֧�ֵ�ָʾ����λ A,B,C,0
#define FTTPYE_NUM   9//8//7//6 //˲ʱ��·���ӵأ���ѹ���¶� �糡�е磬���ö�· ��· ң�ط��Ƹ��� �е��޵�
#define TEMP_PHASE_NUM  3  //�¶�ֻ��A,B,C����
#define YCTPYE_NUM   6 //ͻ��ң�⣬��ʱ���ϵ磬ͣ�硢 �糡�½�ң�⣬����ң�� 6��//zxx

#define REPEAT_START_NUM  37  //�м�����ص�ѹ����ʼ��ַ

#define PA_ADDR_NUM 20

//===================�������Ͷ���============================
typedef unsigned char  INT8U;       //�޷���8λ��
typedef          char  INT8S;       //�з���8λ��
typedef unsigned int   INT16U;      //�޷���16λ��
typedef signed   int   INT16S;      //�з���16λ��
typedef unsigned long  INT32U;      //�޷���32λ��
typedef signed   long  INT32S;      //�з���32λ��
typedef float          FP32;        //�����ȸ�����
typedef double         FP64;            //˫���ȸ�����
//===================����ָʾ����Ϣ�ı�־λ============================
#define  COMTRADE_ITYPE 0x30//ָʾ�����͵���¼������ʱ�ı�־λ

#define  COMTRADE_VTYPE 0x31 //ָʾ�����͵�ѹ¼������ʱ�ı�־λ

#define  COMTRADE_FINISH 0x50 //����ָʾ��¼���ı�־λ

////============================  AD�ɼ�  =============================================
#define AC_AD_CHAN_NUM                          4 //����AD����ͨ����

#define AC_BUFF_LEN                             10//�����ɼ�����������10����

//����ͨ���Ŷ���
#define CHAN_CAP                                 0               
#define CHAN_SUN                                 1//������ͬһ��ʱ���ɼ�

#define CHAN_BAT                                 2   //������ͬһ��ʱ���ɼ�
#define CHAN_BATF                                3
////============================  AD�ɼ�  ��ѹ��=============================================
#define OPEN_SUNC                                0       //��̫���ܵ��ݵĲɼ��ܽ�        
#define OPEN_BAT                                 1      //��﮵�صĲɼ��ܽ�
#define OPEN_BATSUNC                             2      //�����еĲɼ��ܽ�

//============================  SMS-�绰����        =======================================

#define PHONE_CHARNUM         16     //ÿ���绰����ռ���ֽ���/�ַ��� ����ַ��� ����1�ֽڳ���
#define PHONE_NUM             5

#define PHONE_ENCTL           0 //�绰����ʹ�ܿ�����
#define PHONE_ENCTLH          1
#define PN_START              2//�绰�������ʼλ��
#define PHONE1                (PN_START+0*PHONE_CHARNUM+1)//+1�Ƕ�λ���绰1�ĺ��룬0λ��Ϊ�绰1�ĺ��볤��
#define PHONE2                (PN_START+1*PHONE_CHARNUM+1)
#define PHONE3                (PN_START+2*PHONE_CHARNUM+1)
#define PHONE4                (PN_START+3*PHONE_CHARNUM+1)
#define PHONE5                (PN_START+4*PHONE_CHARNUM+1)
#define VERSION               (PN_START+5*PHONE_CHARNUM)  //��λ���汾�ų����Ǹ��ֽ� ��12���ֽ�
#define COMPNAME              (VERSION + 12)  //�汾��ռ12���ַ�,���а���1�ֽڳ���
#define DEVID                 (VERSION + 15)  //��˾��ռ3���ֽ�,���а���1�ֽڳ��� 

#define PHONE_CS              (VERSION + 40)//��Ʒ�ͺ�ռ25�ֽڣ����а���1�ֽڳ���       
#define PHONE_PA_NUM           PHONE_CS + 1



//============================  ���в���        =======================================

#define RP_CFG_KEY               0      //���ÿ�����1
#define RP_CFG_KEYH              1      //���ÿ�����2

#define YX_MODEl                 2//1     //���廹����ͨI/O ������
#define YX_MODEh                 3//2 
#define YX_DSl                   4//3     //���㻹��˫��
#define YX_DSh                   5 
#define RP_YXYCMODE              6     //·���Ƿ�ϲ�ң��ң��
#define Terminal_Type            7     // 0 ��ʾ�ܿ� 2��ʾ�����ն�ODUͨ��1 ��ʾ�����ն�����ͨ��
#define RP_POWER_MODE            8     //��Դģʽ��0 = ׼ʵʱ���� 1 = ʵʱ����
#define RP_GPRS_COMP             9     //GPRS����ѡ�� 1 = ӳ��ͨ
#define RP_DEVICE_TYPE           10     //�豸���� 0 = һң�� 1 = ��ң
#define RP_ZSQ_TYPE              11    //ָʾ������ 0=RF001 1=RF003 for ң��

#define RP_FLTLK_T               12   //���ϱ���ʱ�� ����10S
#define RP_ZSQCST_T              13   //����ָʾ��ͨ��״̬�������
#define RP_ADCOL_T               14    //AD�ɼ�ʱ����
#define RP_BEAT_T                15    //��ʱ�ϴ����� ����1S
#define RP_SENDYC_T              16    //��ʱ�ϴ�ң��ʱ����
#define RP_YXRSD_T               17   //ң���ط��������
#define YX_delay                 18   //����ʱ����
#define RP_SAVELOAD_T            19  //���渺�ɵ�����ʱ����������1S ��СΪ15����
#define RP_ADBAT_T               20   //﮵�ز��������ÿ�μ����Ҫ��﮵��ȥ�ɼ�
#define RP_valt_T                21   //��Դ����ʱ��׼ʵʱģʽ����ʱ��
#define RP_STATUSRP_TIME         22   //״̬�㱨ʱ��
#define RP_STATUSRP_INTERVAL     23   //״̬�㱨���
#define RP_CLOSEGPRSPWR_T        24   //׼ʵʱģʽ��GPRS����ʱ��(Ĭ��5m������Ĭ��3��)

#define RP_COMM_ADDR             25   //��վվ��ַ 
#define RP_SYX_INFADDR           26    //����ң����ʼ��Ϣ���ַ
#define RP_DYX_INFADDR           27    //˫��ң����ʼ��Ϣ��?
#define RP_RFBAND                28    //RFͨ��Ƶ�Σ�ң��ʹ��
#define RP_YCLIMIT               29   //ң�����Խ��ֵ
#define RP_TELIMIT               30   //�¶�Խ��ֵ
#define RP_YCCAP                 31   //ң�����Խ��ֵ ���ָ�ٷֱ�
#define RP_SNTP_T        32  //SNTP��ʱʱ����
#define RP_ALARM_BAT             33  //��ص�ѹ����ֵ
#define RP_ABSOLUTE_CURREN       34  //��������ͻ��
#define RP_ABSOLUTE_VOLTAGE      35  //��ѹ����ͻ��
#define RP_COEFFI_VOLTAGE        36  //��ѹ��׼ֵ����ʱ����ʹ��
#define RP_diff_NUM              37  //��ѹ�����ͻ���λ�õĲ�
#define RP_electric_Vari         38  //�糡�仯�ٷֱ�
#define RP_VOLTAGE_RMS           39  //��ѹ��ʼ��Чֵ�������жϵ�ѹ����Ч��
#define RP_LUBOGPRS_T            40  //¼��ʱ��λ�����ն˵�ʱ���ش����
#define RP_CURRENT_CY            41  //������������
#define RP_CURRENT_CA            42  //A�����У׼
#define RP_CURRENT_CB            43  //B�����У׼
#define RP_CURRENT_CC            44  //C�����У׼
#define RP_VOLTAGE_V0            45  //�ϴ�����վ�ĵ糡ϵ��
#define RP_LBSTORAGE_TIME        46  //¼���洢��Ч��
#define RP_LUBO_NUM              47  //¼���洢����
#define RP_LUBO_CYCLE            48  //��¼������
#define RP_CALL_TIME             49  //����ʱ��
#define RP_GPS_T                 50  //GPS�����ʱ��min
#define RP_OPENGPS_T             51  //GPS�򿪺�ĳ���ʱ��s
#define RP_OPENGPS_NUM           52  //GPS�򿪺�Ĵ�RP_OPENGPS_NUM�ζ�û���յ�GPS��Ϣ�����ϱ�GPS״̬�쳣��ͬʱ��RTC
#define RP_SHORTFAULT_RESETTIME  53  //��·���ϸ�λʱ��
#define RP_LUBO_RESETTIME        54        //¼����־λ��λʱ��
#define RP_SET_KEY               55        //��·����ʱ�Ƿ����ն˸�λ���ж�·������
#define RP_YXRSD_NUM              56        //��·����ʱ�Ƿ����ն˸�λ���ж�·������










//#define RP_RECORDER_TOLNUM             33  //��ص�ѹ����ֵ



#define RP_CRC                   57      //crcУ����
#define RUN_PARA_NUM             58

//�����־��嶨��
#define RPCFG_ISSENDYC              0      //�Ƿ�ʱ�ϴ�ȫң�� 0=���ϴ� 1=��ʱ�ϴ�
#define RPCFG_ADCHG_SDYC            1     //AD�ɼ�ʱ���Ƿ������ϴ�ȫң�� 1=�����ϴ� 0=����
#define RPCFG_SEND_FTYC             2      //�Ƿ��͹���ң��(ȫң��) 1=�ϴ� 0=����
#define RPCFG_SEND_CHANGEYC         3     //�Ƿ���ң��Խ��ֵ 0=���� 1=�ϴ�
#define RPCFG_ZDDI_MODE             4     //0=������Ӳң�� 1=����,���������򲻻��Ӳң�ŵ�24v��Դ
#define RPCFG_SEND_SMS              5      //0=�����Ͷ��� 1=����
#define RPCFG_COMMFLTVAL_MODE       6      //0=ͨ������ʱң�����ó���Чֵ 1=����
#define RPCFG_ISSENDINITFIN         7     //0=�����ͳ�ʼ����֡��1=���� Ĭ�Ϸ���
#define RPCFG_ISDDMODE              8     //0=����ģʽ 1=�Ե�ģʽ
#define RPCFG_PRINTF                9     //0=��ת��ODU��RF���ݵ����Կ� 1=ת��
#define RPCFG_SENDINITFIN_FIRST     10    //0=���ղ���7�����÷��ͳ�ʼ����֡ 1=ֻ���ϵ��ĵ�һ�η�
#define RPCFG_PRINTDUG               11     //0=�����͵�����Ϣ����������ͨ����ת��1=����
#define RPCFG_SNTP_TIME             12      // �Ƿ�������ʱSNTP��ʱ
#define RP_COMTRADE_TYPE             13  //dat �ļ�����ĸ�ʽ 0= ASCII���ϴ� 1=�������ϴ�
#define RP_DEBUG_RATE                14  //���Դ��ڲ����� 0= 115200 1=9600
#define RP_START_LUBO                15  //�Ƿ�����¼��

#define RP_START_TIME                16  //�Ƿ�ر�¼����ʱ
#define RP_set_TIME                  17  //�Ƿ�ر���վ��ʱ
#define RP_AVOLTAGE_REVERSE          18 //¼��A�糡�����Ƿ�ȡ���������ö�ӦABC0���еĵ糡
#define RP_BVOLTAGE_REVERSE          19 //¼��B�糡�����Ƿ�ȡ���������ö�ӦABC0���еĵ糡
#define RP_CVOLTAGE_REVERSE          20 //¼��C�糡�����Ƿ�ȡ���������ö�ӦABC0���еĵ糡
#define RP_ACURRENT_REVERSE          21 //¼��A����������Ƿ�ȡ����//�����ö�ӦABC0���еĵ���
#define RP_BCURRENT_REVERSE          22 //¼��B����������Ƿ�ȡ����
#define RP_CCURRENT_REVERSE          23 //¼��C����������Ƿ�ȡ����

#define RP_SHUANGDIAN_SET            24 //����ң���Ƿ����ó�˫��
#define RP_SHORT_FLAG                25 //�Ƿ����ն�����˲ʱת���ù���
#define RP_OFF_GPS                   26 //�ر�GPS
#define RP_FAULT_TIME                27 //��·����ʱ�궼����ǰ�յ����ĵ�ʱ��򣬶�����RF003��ʱ��ʱ���ʱ��,Ϊ�˵��Ժ��24H2S�Ĺ���
#define RP_EARTH_FLAG                28 //�Ƿ����ն����β��ǹ��ϵ�¼��
#define RP_ERROR_FLAG                29 //ȱ����ʱ��������Ƿ������ϴ�¼��YX
#define RPCFG_ENCRYPT                30     //�Ƿ���ܱ�־λ
#define RPCFG_ISSENDFEI              31 //�Ƿ��͹����¼�

//========================================================================
#define QUASI_REALTIME           0 //׼ʵʱģʽ
#define REALTIME                 1 //ʵʱģʽ
#define REALTIME_SUNPOWER        2 //̫����ʵʱģʽ

#define AERIAL_TERMINAL         0  //�ܿ��ն�
#define CABLE_TERMINAL_WIRELESS  1  //�����ն�
#define CABLE_TERMINAL_ODU   2  //�����ն�ODU
#define CABLE_TERMINAL_DODU   3  //�����ն�˫ODU

//================================  ��ʱ������  ==========================

#define TIMER_NUM            27  //��ʱ������ 1sΪ��λ
#define TM_BEAT               0     //��ʱ�ϴ����� GPRSͨ������ʱ�� ����1S
#define TM_ADCOLT             1   //AD�ɼ�ʱ���� ��ʱ��AD�ɼ�λ��1s��ʹ��AD�ɼ�
#define TM_SENDYC             2   //��ʱ�ϴ�ң��ʱ����
#define TM_CLOSECHARGE        3   //�رճ��ʱ��
#define TM_GPRSPWOERDOWN      4   //GPRS����ʱ�䣬����׼ʵʱ����ģʽ
#define TM_SAVELOAD           5   //���渺�ɵ���
#define TM_ODU_call2          6   //�ٻ�ODU�Ķ�������
#define TM_ODU_call           7   //�ٻ�ODU����
#define TM_BATPWOERDOWN       8   //﮵�عض�ʱ��
#define TM_SMSSENDALLOW       9  //���ŷ����ӳ�
#define TM_WAITXINHAO         10  //���ŷ������ź�Դ�ĵȴ�ʱ��
#define TM_ADBAT              11  //﮵�ز���ʱ��?#define TM_ADBATTEN      11  //﮵�ص�ʹ��A�Ĳ���
#define TM_Volt               12  //��ѹ����ʱ������Դģʽ��ʱ����
#define TM_SENDSTATUSDELAY    13  //״̬�㱨��ʱ
#define TM_SENDADDR           14  //��վ��ַ������ʱ
#define TM_SENDFAULTDELAY     15  //�������������ӳ� ����������ģʽ
#define TM_SENDSTATE          16  // ����ODU��״̬�㱨����
#define TM_SNTP_TIME          17  //��ʱSNTP��ʱ���
#define TM_GPRS_RW            18  //GPRS��д��ʱ
#define TM_SMS_RW             19  //���Ŷ�д���ʱ
#define TM_RECORDED           20  //�յ�ָʾ����¼�����ݺ���ʱ�ȴ��������ָʾ������
#define TM_YKTRIGGER          21  //�յ�ָʾ����¼�����ݺ���ʱ�ȴ��������ָʾ������
#define TM_SHORT_FLAG         22  //˲ʱת���õ�ʱ��
#define TM_GPS_TIME           23  //�����GPS��ʱ��
#define TM_OPENGPS_TIME       24  //GPS�򿪵ĳ������ʱ��
#define TM_OPENGPRS_RF        25  //GPRS����ת��ģʽ��30Sδ�յ���GPRS�����������ݣ��Զ��˳�ת��ģʽ
#define TM_OPENDEG_RF        26  //DEG����ת��ģʽ��30Sδ�յ���DEUG�����������ݣ��Զ��˳�ת��ģʽ






//==============================IEC 101��ز���======================================

#define IECP_LINKADDR_NUM    0//��·��ַ���� 1-2    
#define IECP_TRANSRSN_NUM    1//����ԭ�򳤶� 1-2   
#define IECP_COMNADDR_NUM    2 //������ַ���� 1-2  
#define IECP_INFO_NUM        3//��Ϣ���ַ���� 1-3 
#define IECP_COM_MODE        4//0Ϊ��ƽ��ʱ��1Ϊƽ��ʽ  Ĭ��Ϊ1
#define IECP_AUT_MODE        5//�����ϴ�ģʽ��0=��վ���ͳ�ʼ���󷽿������ϴ�������
                              //1=�յ���վ�κ�����󷽿������ϴ����κ�ʱ����Ӧ�������
                              //2=�����ϴ������κ�Լ��
#define IECP_OBJ_COM0        6 //����0ָ���Ķ���0=RF,1=GPRS,2=DBG,3=EX,4=ODU1,5=ODU2
#define IECP_OBJ_COM1        7
#define IECP_OBJ_COM2        8
#define IECP_OBJ_COM3        9
#define IECP_YC_TYPE         10 //0=��һ��ֵ[9],1=��Ȼ�ֵ[11],2=�̸�����[13]

#define IECP_CRC             11//crcУ��

#define IEC_PARA_NUM    12  //IEC���еĲ�������

#define IEC_YC_NUM  120//RMT_MEAS_NUM  //��������վ��ң���������
#define IEC_YX_NUM   400          //RMT_INFO_NUM��������վ��ң�Ÿ�������
#define IEC_YK_NUM  100  //OUT_TYPE_NUM  //��������վ��ң�ظ�������
#define FRM_MAX_COS_NUM   70 //ÿ֡������ഫ��COS�������涨Ϊ70��

  
//============================  ң����          =======================================

//���ϵ��������ɵ����������¶ȣ�ÿ��ң�����
#define RM_BAT_U         0 //��ص�ѹ
#define RM_CAP_U         1 //���ݵ�ѹ
#define RM_SUN_U         2 //̫���ܵ�ѹ
#define RM_RSV           3 // �ĳɱ��� ��ظ���ѹRM_BATF_U
#define RM_1A_FAULT_I       4//1a���ϵ���
#define RM_1B_FAULT_I       5
#define RM_1C_FAULT_I       6
#define RM_1O_FAULT_I       7
#define RM_2A_FAULT_I       8
#define RM_2B_FAULT_I       9
#define RM_2C_FAULT_I       10
#define RM_2O_FAULT_I       11
#define RM_3A_FAULT_I       12
#define RM_3B_FAULT_I       13
#define RM_3C_FAULT_I       14
#define RM_3O_FAULT_I       15
/*#define RM_4A_FAULT_I       16
#define RM_4B_FAULT_I       17
#define RM_4C_FAULT_I       18
#define RM_4O_FAULT_I       19
#define RM_5A_FAULT_I       20
#define RM_5B_FAULT_I       21
#define RM_5C_FAULT_I       22
#define RM_5O_FAULT_I       23
#define RM_6A_FAULT_I       24
#define RM_6B_FAULT_I       25
#define RM_6C_FAULT_I       26
#define RM_6O_FAULT_I       27
#define RM_7A_FAULT_I       28
#define RM_7B_FAULT_I       29
#define RM_7C_FAULT_I       30
#define RM_7O_FAULT_I       31
#define RM_8A_FAULT_I       32
#define RM_8B_FAULT_I       33
#define RM_8C_FAULT_I       34
#define RM_8O_FAULT_I       35
#define RM_9A_FAULT_I       36
#define RM_9B_FAULT_I       37
#define RM_9C_FAULT_I       38
#define RM_9O_FAULT_I       39
#define RM_10A_FAULT_I      40
#define RM_10B_FAULT_I      41
#define RM_10C_FAULT_I      42
#define RM_10O_FAULT_I      43
#define RM_11A_FAULT_I      44
#define RM_11B_FAULT_I      45
#define RM_11C_FAULT_I      46
#define RM_11O_FAULT_I      47
#define RM_12A_FAULT_I      48
#define RM_12B_FAULT_I      49
#define RM_12C_FAULT_I      50
#define RM_12O_FAULT_I      51*/

#define RM_1A_LOAD_I        16//52//1a���ɵ���
#define RM_1B_LOAD_I        17//53
#define RM_1C_LOAD_I        18//54
#define RM_1O_LOAD_I        19//55
#define RM_2A_LOAD_I        20//56
#define RM_2B_LOAD_I        21//57
#define RM_2C_LOAD_I        22//58
#define RM_2O_LOAD_I        23//59
#define RM_3A_LOAD_I        24//60
#define RM_3B_LOAD_I        25//61
#define RM_3C_LOAD_I        26//62
#define RM_3O_LOAD_I        27//63
/*#define RM_4A_LOAD_I        64
#define RM_4B_LOAD_I        65
#define RM_4C_LOAD_I        66
#define RM_4O_LOAD_I        67
#define RM_5A_LOAD_I        68
#define RM_5B_LOAD_I        69
#define RM_5C_LOAD_I        70
#define RM_5O_LOAD_I        71
#define RM_6A_LOAD_I        72
#define RM_6B_LOAD_I        73
#define RM_6C_LOAD_I        74
#define RM_6O_LOAD_I        75
#define RM_7A_LOAD_I        76
#define RM_7B_LOAD_I        77
#define RM_7C_LOAD_I        78
#define RM_7O_LOAD_I        79
#define RM_8A_LOAD_I        80
#define RM_8B_LOAD_I        81
#define RM_8C_LOAD_I        82
#define RM_8O_LOAD_I        83
#define RM_9A_LOAD_I        84
#define RM_9B_LOAD_I        85
#define RM_9C_LOAD_I        86
#define RM_9O_LOAD_I        87
#define RM_10A_LOAD_I       88
#define RM_10B_LOAD_I       89
#define RM_10C_LOAD_I       90
#define RM_10O_LOAD_I       91
#define RM_11A_LOAD_I       92
#define RM_11B_LOAD_I       93
#define RM_11C_LOAD_I       94
#define RM_11O_LOAD_I       95
#define RM_12A_LOAD_I       96
#define RM_12B_LOAD_I       97
#define RM_12C_LOAD_I       98
#define RM_12O_LOAD_I       99*/


#define RN_1A_BAT_U             28//172           //ָʾ����ص�ѹ����ʱֻ��3����·
#define RN_1B_BAT_U             29//173     
#define RN_1C_BAT_U             30//174     
#define RN_2A_BAT_U             31//175     
#define RN_2B_BAT_U             32//176     
#define RN_2C_BAT_U             33//177     
#define RN_3A_BAT_U             34//178     
#define RN_3B_BAT_U             35//179     
#define RN_3C_BAT_U             36//180 

#define RN_1A_CAP_U              37//181   //ָʾ���糡��ѹ����ʱֻ��3����·
#define RN_1B_CAP_U              38//182
#define RN_1C_CAP_U              39//183
#define RN_2A_CAP_U              40//184
#define RN_2B_CAP_U              41//185
#define RN_2C_CAP_U              42//186
#define RN_3A_CAP_U              43//187
#define RN_3B_CAP_U              44//188
#define RN_3C_CAP_U              45//189


#define RN_1A_EFIELD_U            46//64//190   //ָʾ���糡��ѹ����ʱֻ��3����·
#define RN_1B_EFIELD_U            47//65//191
#define RN_1C_EFIELD_U            48//66//192
#define RN_2A_EFIELD_U            49//67//193
#define RN_2B_EFIELD_U            50//68//194
#define RN_2C_EFIELD_U            51//69//195
#define RN_3A_EFIELD_U            52//70//196
#define RN_3B_EFIELD_U            53//1//197
#define RN_3C_EFIELD_U            54//72//198

#define RM_1A_LUBO       55        //ȡ���ѹ
#define RM_1B_LUBO       56
#define RM_1C_LUBO       57
//#define RM_1O_LUBO       58
#define RM_2A_LUBO       58
#define RM_2B_LUBO       59
#define RM_2C_LUBO       60
//#define RM_2O_LUBO       62
#define RM_3A_LUBO       61
#define RM_3B_LUBO       62
#define RM_3C_LUBO       63
//#define RM_3O_LUBO       66


/*#define RM_1A_CABLE_T       55//28//100//1a�����ⲿ�¶� 
#define RM_1B_CABLE_T       56//29//101
#define RM_1C_CABLE_T       57//30//102
#define RM_2A_CABLE_T       58//31//103
#define RM_2B_CABLE_T       59//32//104
#define RM_2C_CABLE_T       60//33//105
#define RM_3A_CABLE_T       61//34//106
#define RM_3B_CABLE_T       62//35//107
#define RM_3C_CABLE_T       63//36//108
#define RM_4A_CABLE_T       109
#define RM_4B_CABLE_T       110
#define RM_4C_CABLE_T       111
#define RM_5A_CABLE_T       112
#define RM_5B_CABLE_T       113
#define RM_5C_CABLE_T       114
#define RM_6A_CABLE_T       115
#define RM_6B_CABLE_T       116
#define RM_6C_CABLE_T       117
#define RM_7A_CABLE_T       118
#define RM_7B_CABLE_T       119
#define RM_7C_CABLE_T       120
#define RM_8A_CABLE_T       121
#define RM_8B_CABLE_T       122
#define RM_8C_CABLE_T       123
#define RM_9A_CABLE_T       124
#define RM_9B_CABLE_T       125
#define RM_9C_CABLE_T       126
#define RM_10A_CABLE_T      127
#define RM_10B_CABLE_T      128
#define RM_10C_CABLE_T      129
#define RM_11A_CABLE_T      130
#define RM_11B_CABLE_T      131
#define RM_11C_CABLE_T      132
#define RM_12A_CABLE_T      133
#define RM_12B_CABLE_T      134
#define RM_12C_CABLE_T      135*/

#define RM_1A_CABLE_T_INT       64//64//37//136//1a�����ڲ��¶� �ܿ�ָʾ���¶�
#define RM_1B_CABLE_T_INT       65//65//38//137
#define RM_1C_CABLE_T_INT       66//66//39//138
#define RM_2A_CABLE_T_INT       67//67//40//139
#define RM_2B_CABLE_T_INT       68//68//41//140
#define RM_2C_CABLE_T_INT       69//69//42//141
#define RM_3A_CABLE_T_INT       70//70//43//142
#define RM_3B_CABLE_T_INT       71//71//44//143
#define RM_3C_CABLE_T_INT       72//72//45//144
/*#define RM_4A_CABLE_T_INT       145
#define RM_4B_CABLE_T_INT       146
#define RM_4C_CABLE_T_INT       147
#define RM_5A_CABLE_T_INT       148
#define RM_5B_CABLE_T_INT       149
#define RM_5C_CABLE_T_INT       150
#define RM_6A_CABLE_T_INT       151
#define RM_6B_CABLE_T_INT       152
#define RM_6C_CABLE_T_INT       153
#define RM_7A_CABLE_T_INT       154
#define RM_7B_CABLE_T_INT       155
#define RM_7C_CABLE_T_INT       156
#define RM_8A_CABLE_T_INT       157
#define RM_8B_CABLE_T_INT       158
#define RM_8C_CABLE_T_INT       159
#define RM_9A_CABLE_T_INT       160
#define RM_9B_CABLE_T_INT       161
#define RM_9C_CABLE_T_INT       162
#define RM_10A_CABLE_T_INT      163
#define RM_10B_CABLE_T_INT      164
#define RM_10C_CABLE_T_INT      165
#define RM_11A_CABLE_T_INT      166
#define RM_11B_CABLE_T_INT      167
#define RM_11C_CABLE_T_INT      168
#define RM_12A_CABLE_T_INT      169
#define RM_12B_CABLE_T_INT      170
#define RM_12C_CABLE_T_INT      171*/

/*#define RN_1A_BAT_U             46//172         //ָʾ����ص�ѹ����ʱֻ��3����·
#define RN_1B_BAT_U             47//173     
#define RN_1C_BAT_U             48//174     
#define RN_2A_BAT_U             49//175     
#define RN_2B_BAT_U             50//176     
#define RN_2C_BAT_U             51//177     
#define RN_3A_BAT_U             52//178     
#define RN_3B_BAT_U             53//179     
#define RN_3C_BAT_U             54//180 

#define RN_1A_CAP_U              55//181   //ָʾ���糡��ѹ����ʱֻ��3����·
#define RN_1B_CAP_U              56//182
#define RN_1C_CAP_U              57//183
#define RN_2A_CAP_U              58//184
#define RN_2B_CAP_U              59//185
#define RN_2C_CAP_U              60//186
#define RN_3A_CAP_U              61//187
#define RN_3B_CAP_U              62//188
#define RN_3C_CAP_U              63//189


#define RN_1A_EFIELD_U            64//190   //ָʾ���糡��ѹ����ʱֻ��3����·
#define RN_1B_EFIELD_U            65//191
#define RN_1C_EFIELD_U            66//192
#define RN_2A_EFIELD_U            67//193
#define RN_2B_EFIELD_U            68//194
#define RN_2C_EFIELD_U            69//195
#define RN_3A_EFIELD_U            70//196
#define RN_3B_EFIELD_U            71//197
#define RN_3C_EFIELD_U            72//198*/


/*#define REPEATER1_BAT_U      190//�м�����ص�ѹ
#define REPEATER2_BAT_U      191
#define REPEATER3_BAT_U      192
#define REPEATER4_BAT_U      193
#define REPEATER5_BAT_U      194
#define REPEATER6_BAT_U      195
#define REPEATER7_BAT_U      196
#define REPEATER8_BAT_U      197
#define REPEATER9_BAT_U      198
#define REPEATER10_BAT_U      199
#define REPEATER11_BAT_U      200
#define REPEATER12_BAT_U      201

#define REPEATER1_CAP_U      202//�м������ݵ�ѹ
#define REPEATER2_CAP_U      203
#define REPEATER3_CAP_U      204
#define REPEATER4_CAP_U      205
#define REPEATER5_CAP_U      206
#define REPEATER6_CAP_U      207
#define REPEATER7_CAP_U      208
#define REPEATER8_CAP_U      209
#define REPEATER9_CAP_U      210
#define REPEATER10_CAP_U      211
#define REPEATER11_CAP_U      212
#define REPEATER12_CAP_U      213*/

#define RM_LUBO_NUM        73//72//45//144
#define RM_ALLLUBO_NUM     74

#define RMT_MEAS_NUM       75//199
#define RM_TEMP_START      RM_1A_CABLE_T_INT//RM_1A_CABLE_T//�¶�ң�⿪ʼ����ʼλ��
#define RM_LOADI_NUM       LINE_NUM * 4 //���ɵ�������
#define LOAD_LEN           RM_1A_LUBO*2+8//RM_TEMP_START*2 + 7 //RM_LOADI_NUM*2 + 7 //�������ɼ�¼�ĳ��ȣ���λ�ֽڣ�������ʼ��(1�ֽ�)������(1�ֽ�)��ʱ��(5�ֽ�)


//���ϵ������˳��Ϊ1A,1B,1C,1O,...XA,XB,XC,XO




//============================  ң����          =======================================
//        | ��· |�ӵ� |��ѹ |�¶� |�糡�е� |���� |ͨ��״̬ |�����(��·||�ӵ�)|
//------------------------------------------------------------------------------------
//     A��| C/A  |   A | C/A | C/A |   C/A   | C/A | C/A     |   C/A            |                  |
//------------------------------------------------------------------------------------
//     B��| C/A  |   A | C/A | C/A |   C/A   | C/A | C/A     |   C/A            |     
//------------------------------------------------------------------------------------
//     C��| C/A  |   A | C/A | C/A |   C/A   | C/A | C/A     |   C/A            |     
//------------------------------------------------------------------------------------
//     O��|  -   |   C | -   |  -  |   -     | -   |  C      |   C              |     
//------------------------------------------------------------------------------------
//      ��|      |     |     |     |         |     |         |                  |
//------------------------------------------------------------------------------------
//�ܿ�û��O���࣬���½ӵ�ֻ��O��
//A����� = A��· || A�ӵ� ;   ��B\C\O��ͬ��
//�߶�·  = A��· || B��· || C��·��
//�߽ӵ�  = A�ӵ� || B�ӵ� || C�ӵ�||O�ӵأ�
//���ܹ��� = A����� || B����� ||C����� ||0����� ��


//˲ʱ�Զ�·���ӵأ���ѹ(ָʾ��)���¶ȣ���������(�е�(�糡))�������Զ�·��ң�ط���״̬���е��޵硣ͨ��״̬�������(��·||�ӵ�)��ÿ���ң�Ÿ���
//ʵ��ң��(���Ľ�����ң��)
#define RI_1A_SHORT     0
#define RI_1B_SHORT     1
#define RI_1C_SHORT     2
#define RI_1O_SHORT     3
#define RI_2A_SHORT     4
#define RI_2B_SHORT     5
#define RI_2C_SHORT     6
#define RI_2O_SHORT     7
#define RI_3A_SHORT     8
#define RI_3B_SHORT     9
#define RI_3C_SHORT     10
#define RI_3O_SHORT     11
/*#define RI_4A_SHORT   12
#define RI_4B_SHORT     13
#define RI_4C_SHORT     14
#define RI_4O_SHORT     15
#define RI_5A_SHORT     16
#define RI_5B_SHORT     17
#define RI_5C_SHORT     18
#define RI_5O_SHORT     19
#define RI_6A_SHORT     20
#define RI_6B_SHORT     21
#define RI_6C_SHORT     22
#define RI_6O_SHORT     23
#define RI_7A_SHORT     24
#define RI_7B_SHORT     25
#define RI_7C_SHORT     26
#define RI_7O_SHORT     27
#define RI_8A_SHORT     28
#define RI_8B_SHORT     29
#define RI_8C_SHORT     30
#define RI_8O_SHORT     31
#define RI_9A_SHORT     32
#define RI_9B_SHORT     33
#define RI_9C_SHORT     34
#define RI_9O_SHORT     35
#define RI_10A_SHORT        36
#define RI_10B_SHORT        37
#define RI_10C_SHORT        38
#define RI_10O_SHORT        39
#define RI_11A_SHORT        40
#define RI_11B_SHORT        41
#define RI_11C_SHORT        42
#define RI_11O_SHORT        43
#define RI_12A_SHORT        44
#define RI_12B_SHORT        45
#define RI_12C_SHORT        46
#define RI_12O_SHORT        47*/
#define RI_1A_GND   12//48
#define RI_1B_GND   13//49
#define RI_1C_GND   14//50
#define RI_1O_GND   15//51
#define RI_2A_GND   16//52
#define RI_2B_GND   17//53
#define RI_2C_GND   18//54
#define RI_2O_GND   19//55
#define RI_3A_GND   20//56
#define RI_3B_GND   21//57
#define RI_3C_GND   22//58
#define RI_3O_GND   23//59
/*#define RI_4A_GND     60
#define RI_4B_GND   61
#define RI_4C_GND   62
#define RI_4O_GND   63
#define RI_5A_GND   64
#define RI_5B_GND   65
#define RI_5C_GND   66
#define RI_5O_GND   67
#define RI_6A_GND   68
#define RI_6B_GND   69
#define RI_6C_GND   70
#define RI_6O_GND   71
#define RI_7A_GND   72
#define RI_7B_GND   73
#define RI_7C_GND   74
#define RI_7O_GND   75
#define RI_8A_GND   76
#define RI_8B_GND   77
#define RI_8C_GND   78
#define RI_8O_GND   79
#define RI_9A_GND   80
#define RI_9B_GND   81
#define RI_9C_GND   82
#define RI_9O_GND   83
#define RI_10A_GND      84
#define RI_10B_GND      85
#define RI_10C_GND      86
#define RI_10O_GND      87
#define RI_11A_GND      88
#define RI_11B_GND      89
#define RI_11C_GND      90
#define RI_11O_GND      91
#define RI_12A_GND      92
#define RI_12B_GND      93
#define RI_12C_GND      94
#define RI_12O_GND      95*/
#define RI_1A_LVD   24//96
#define RI_1B_LVD   25//97
#define RI_1C_LVD   26//98
#define RI_1O_LVD   27//99
#define RI_2A_LVD   28//100
#define RI_2B_LVD   29//101
#define RI_2C_LVD   30//102
#define RI_2O_LVD   31//103
#define RI_3A_LVD   32//104
#define RI_3B_LVD   33//105
#define RI_3C_LVD   34//106
#define RI_3O_LVD   35//107
/*#define RI_4A_LVD     108
#define RI_4B_LVD   109
#define RI_4C_LVD   110
#define RI_4O_LVD   111
#define RI_5A_LVD   112
#define RI_5B_LVD   113
#define RI_5C_LVD   114
#define RI_5O_LVD   115
#define RI_6A_LVD   116
#define RI_6B_LVD   117
#define RI_6C_LVD   118
#define RI_6O_LVD   119
#define RI_7A_LVD   120
#define RI_7B_LVD   121
#define RI_7C_LVD   122
#define RI_7O_LVD   123
#define RI_8A_LVD   124
#define RI_8B_LVD   125
#define RI_8C_LVD   126
#define RI_8O_LVD   127
#define RI_9A_LVD   128
#define RI_9B_LVD   129
#define RI_9C_LVD   130
#define RI_9O_LVD   131
#define RI_10A_LVD      132
#define RI_10B_LVD      133
#define RI_10C_LVD      134
#define RI_10O_LVD      135
#define RI_11A_LVD      136
#define RI_11B_LVD      137
#define RI_11C_LVD      138
#define RI_11O_LVD      139
#define RI_12A_LVD      140
#define RI_12B_LVD      141
#define RI_12C_LVD      142
#define RI_12O_LVD      143*/
#define RI_1A_CABT  36//144
#define RI_1B_CABT  37//145
#define RI_1C_CABT  38//146
#define RI_1O_CABT  39//147
#define RI_2A_CABT  40//148
#define RI_2B_CABT  41//149
#define RI_2C_CABT  42//150
#define RI_2O_CABT  43//151
#define RI_3A_CABT  44//152
#define RI_3B_CABT  45//153
#define RI_3C_CABT  46//154
#define RI_3O_CABT  47//155
/*#define RI_4A_CABT    156
#define RI_4B_CABT  157
#define RI_4C_CABT  158
#define RI_4O_CABT  159
#define RI_5A_CABT  160
#define RI_5B_CABT  161
#define RI_5C_CABT  162
#define RI_5O_CABT  163
#define RI_6A_CABT  164
#define RI_6B_CABT  165
#define RI_6C_CABT  166
#define RI_6O_CABT  167
#define RI_7A_CABT  168
#define RI_7B_CABT  169
#define RI_7C_CABT  170
#define RI_7O_CABT  171
#define RI_8A_CABT  172
#define RI_8B_CABT  173
#define RI_8C_CABT  174
#define RI_8O_CABT  175
#define RI_9A_CABT  176
#define RI_9B_CABT  177
#define RI_9C_CABT  178
#define RI_9O_CABT  179
#define RI_10A_CABT     180
#define RI_10B_CABT     181
#define RI_10C_CABT     182
#define RI_10O_CABT     183
#define RI_11A_CABT     184
#define RI_11B_CABT     185
#define RI_11C_CABT     186
#define RI_11O_CABT     187
#define RI_12A_CABT     188
#define RI_12B_CABT     189
#define RI_12C_CABT     190
#define RI_12O_CABT     191*/
#define RI_1A_ELECON    48//192����Խ��
#define RI_1B_ELECON    49//193
#define RI_1C_ELECON    50//194
#define RI_1O_ELECON    51//195
#define RI_2A_ELECON    52//196
#define RI_2B_ELECON    53//197
#define RI_2C_ELECON    54//198
#define RI_2O_ELECON    55//199
#define RI_3A_ELECON    56//200
#define RI_3B_ELECON    57//201
#define RI_3C_ELECON    58//202
#define RI_3O_ELECON    59//203
/*#define RI_4A_ELECON  204
#define RI_4B_ELECON    205
#define RI_4C_ELECON    206
#define RI_4O_ELECON    207
#define RI_5A_ELECON    208
#define RI_5B_ELECON    209
#define RI_5C_ELECON    210
#define RI_5O_ELECON    211
#define RI_6A_ELECON    212
#define RI_6B_ELECON    213
#define RI_6C_ELECON    214
#define RI_6O_ELECON    215
#define RI_7A_ELECON    216
#define RI_7B_ELECON    217
#define RI_7C_ELECON    218
#define RI_7O_ELECON    219
#define RI_8A_ELECON    220
#define RI_8B_ELECON    221
#define RI_8C_ELECON    222
#define RI_8O_ELECON    223
#define RI_9A_ELECON    224
#define RI_9B_ELECON    225
#define RI_9C_ELECON    226
#define RI_9O_ELECON    227
#define RI_10A_ELECON   228
#define RI_10B_ELECON   229
#define RI_10C_ELECON   230
#define RI_10O_ELECON   231
#define RI_11A_ELECON   232
#define RI_11B_ELECON   233
#define RI_11C_ELECON   234
#define RI_11O_ELECON   235
#define RI_12A_ELECON   236
#define RI_12B_ELECON   237
#define RI_12C_ELECON   238
#define RI_12O_ELECON   239*/
#define RI_1A_RSV   60//240���ö�·
#define RI_1B_RSV   61//241
#define RI_1C_RSV   62//242
#define RI_1O_RSV   63//243
#define RI_2A_RSV   64//244
#define RI_2B_RSV   65//245
#define RI_2C_RSV   66//246
#define RI_2O_RSV   67//247
#define RI_3A_RSV   68//248
#define RI_3B_RSV   69//249
#define RI_3C_RSV   70//250
#define RI_3O_RSV   71//251


//��·����
#define RI_1A_TOTALSHORT    72//60//240��·
#define RI_1B_TOTALSHORT    73//61//241
#define RI_1C_TOTALSHORT    74//62//242
#define RI_1O_TOTALSHORT    75//63//243
#define RI_2A_TOTALSHORT    76//64//244
#define RI_2B_TOTALSHORT    77//65//245
#define RI_2C_TOTALSHORT    78//66//246
#define RI_2O_TOTALSHORT    79//67//247
#define RI_3A_TOTALSHORT    80//68//248
#define RI_3B_TOTALSHORT    81//69//249
#define RI_3C_TOTALSHORT    82//70//250
#define RI_3O_TOTALSHORT    83//71//251


//ָʾ��ң�ط��Ƹ���ң��
#define RI_1A_FLOP  84//72//251
#define RI_1B_FLOP  85//73//251
#define RI_1C_FLOP  86//74//251
#define RI_1O_FLOP  87//75//251

#define RI_2A_FLOP  88//76//251
#define RI_2B_FLOP  89//77//251
#define RI_2C_FLOP  90//78//251
#define RI_2O_FLOP  91//79//251

#define RI_3A_FLOP  92//80//251
#define RI_3B_FLOP  93//81//251
#define RI_3C_FLOP  94//82//251
#define RI_3O_FLOP  95//83//251
//ͣ��ң��
#define RI_1A_POWERCUT  96//84//251
#define RI_1B_POWERCUT  97//    85//251
#define RI_1C_POWERCUT  98//    86//251
#define RI_1O_POWERCUT  99//    87//251

#define RI_2A_POWERCUT  100//   88//251
#define RI_2B_POWERCUT  101//   89//251
#define RI_2C_POWERCUT  102//   90//251
#define RI_2O_POWERCUT  103//   91//251

#define RI_3A_POWERCUT  104//   92//251
#define RI_3B_POWERCUT  105//=  93//251
#define RI_3C_POWERCUT  106//   94//251
#define RI_3O_POWERCUT  107//   95//251

/*#define RI_4A_RSV     252
#define RI_4B_RSV   253
#define RI_4C_RSV   254
#define RI_4O_RSV   255
#define RI_5A_RSV   256
#define RI_5B_RSV   257
#define RI_5C_RSV   258
#define RI_5O_RSV   259
#define RI_6A_RSV   260
#define RI_6B_RSV   261
#define RI_6C_RSV   262
#define RI_6O_RSV   263
#define RI_7A_RSV   264
#define RI_7B_RSV   265
#define RI_7C_RSV   266
#define RI_7O_RSV   267
#define RI_8A_RSV   268
#define RI_8B_RSV   269
#define RI_8C_RSV   270
#define RI_8O_RSV   271
#define RI_9A_RSV   272
#define RI_9B_RSV   273
#define RI_9C_RSV   274
#define RI_9O_RSV   275
#define RI_10A_RSV      276
#define RI_10B_RSV      277
#define RI_10C_RSV      278
#define RI_10O_RSV      279
#define RI_11A_RSV      280
#define RI_11B_RSV      281
#define RI_11C_RSV      282
#define RI_11O_RSV      283
#define RI_12A_RSV      284
#define RI_12B_RSV      285
#define RI_12C_RSV      286
#define RI_12O_RSV      287*/

//���¹���Ϣ(���ն˺ϳ�)
//ÿ����·�ܵ�˲ʱ�Զ�·����
#define RI_1SHORT       108//96//72//288
#define RI_2SHORT       109//97//73//289
#define RI_3SHORT       110//98//74//290
/*#define RI_4SHORT     291
#define RI_5SHORT       292
#define RI_6SHORT       293
#define RI_7SHORT       294
#define RI_8SHORT       295
#define RI_9SHORT       296
#define RI_10SHORT      297
#define RI_11SHORT      298
#define RI_12SHORT  299*/
#define RI_1GND     111//99//75//300
#define RI_2GND     112//100//76//301
#define RI_3GND     113//101//77//302
/*#define RI_4GND       303
#define RI_5GND     304
#define RI_6GND     305
#define RI_7GND     306
#define RI_8GND     307
#define RI_9GND     308
#define RI_10GND    309
#define RI_11GND    310
#define RI_12GND    311*/
#define RI_1LVD     114//102//78//312
#define RI_2LVD     115//103//79//313
#define RI_3LVD     116//104//80//314
/*#define RI_4LVD       315
#define RI_5LVD     316
#define RI_6LVD     317
#define RI_7LVD     318
#define RI_8LVD     319
#define RI_9LVD     320
#define RI_10LVD    321
#define RI_11LVD    322
#define RI_12LVD    323*/
#define RI_1CABT    117//105//81//324
#define RI_2CABT    118//106///82//325
#define RI_3CABT    119//107//83//326
/*#define RI_4CABT      327
#define RI_5CABT    328
#define RI_6CABT    329
#define RI_7CABT    330
#define RI_8CABT    331
#define RI_9CABT    332
#define RI_10CABT   333
#define RI_11CABT   334
#define RI_12CABT   335*/
#define RI_1ELECON   120//  108//84//336
#define RI_2ELECON   121//  109//85//337
#define RI_3ELECON   122//  110//86//338
/*#define RI_4ELECON    339
#define RI_5ELECON      340
#define RI_6ELECON      341
#define RI_7ELECON      342
#define RI_8ELECON      343
#define RI_9ELECON      344
#define RI_10ELECON     345
#define RI_11ELECON     346
#define RI_12ELECON 347*/
//ÿ����·�ܵ������Զ�·����
#define RI_1RSV     123//111//87//348
#define RI_2RSV     124//112//88//349
#define RI_3RSV     125//113//89//350

/*#define RI_4RSV       351
#define RI_5RSV     352
#define RI_6RSV     353
#define RI_7RSV     354
#define RI_8RSV     355
#define RI_9RSV     356
#define RI_10RSV    357
#define RI_11RSV    358
#define RI_12RSV    359*/
//ÿ����·�ܵĶ�·���ϣ�������˲ʱ��������
#define RI_1_TOTALSHORT 126
#define RI_2_TOTALSHORT 127
#define RI_3_TOTALSHORT 128


#define RI_1FLOP    129//114//87//348
#define RI_2FLOP    130//115//88//349
#define RI_3FLOP    131//116//89//350

#define RI_1POWERCUT 132//117
#define RI_2POWERCUT 133//118
#define RI_3POWERCUT 134//119


//״̬��Ϣ
//��·ͨ��״̬(�ܿ�ʱ���ն˺ϳ�)
#define RI_1ZSQST           135//120//114//90//360
#define RI_2ZSQST           136//121//115//91//361
#define RI_3ZSQST           137//122//116//92//362
/*#define RI_4ZSQST         363
#define RI_5ZSQST           364
#define RI_6ZSQST           365
#define RI_7ZSQST           366
#define RI_8ZSQST           367
#define RI_9ZSQST           368
#define RI_10ZSQST          369
#define RI_11ZSQST          370
#define RI_12ZSQST          371*/
#define RI_1A_ZSQST         138//123//117//93//372
#define RI_1B_ZSQST         139//124//118//94//373
#define RI_1C_ZSQST         140//125//119//95//374
#define RI_1O_ZSQST         141//126//120//96//375
#define RI_2A_ZSQST         142//127//121//97//376
#define RI_2B_ZSQST         143//128//122//98//377
#define RI_2C_ZSQST         144//129//123////378
#define RI_2O_ZSQST         145//130//124//100//379
#define RI_3A_ZSQST         146//131//125//101//380
#define RI_3B_ZSQST         147//132//126////381
#define RI_3C_ZSQST         148//133//127//103//382
#define RI_3O_ZSQST         149//134//128//104//383
/*#define RI_4A_ZSQST       384
#define RI_4B_ZSQST         385
#define RI_4C_ZSQST         386
#define RI_4O_ZSQST         387
#define RI_5A_ZSQST         388
#define RI_5B_ZSQST         389
#define RI_5C_ZSQST         390
#define RI_5O_ZSQST         391
#define RI_6A_ZSQST         392
#define RI_6B_ZSQST         393
#define RI_6C_ZSQST         394
#define RI_6O_ZSQST         395
#define RI_7A_ZSQST         396
#define RI_7B_ZSQST         397
#define RI_7C_ZSQST         398
#define RI_7O_ZSQST         399
#define RI_8A_ZSQST         400
#define RI_8B_ZSQST         401
#define RI_8C_ZSQST         402
#define RI_8O_ZSQST         403
#define RI_9A_ZSQST         404
#define RI_9B_ZSQST         405
#define RI_9C_ZSQST         406
#define RI_9O_ZSQST         407
#define RI_10A_ZSQST    408
#define RI_10B_ZSQST    409
#define RI_10C_ZSQST    410
#define RI_10O_ZSQST    411
#define RI_11A_ZSQST    412
#define RI_11B_ZSQST    413
#define RI_11C_ZSQST    414
#define RI_11O_ZSQST    415
#define RI_12A_ZSQST    416
#define RI_12B_ZSQST    417
#define RI_12C_ZSQST    418
#define RI_12O_ZSQST    419*/


//����(����� ���ն˺ϳ�)
#define RI_1FLT         150//135//129//105//420
#define RI_2FLT         151//136//130//106//421
#define RI_3FLT         152//137//131//107//422
/*#define RI_4FLT           423
#define RI_5FLT         424
#define RI_6FLT         425
#define RI_7FLT         426
#define RI_8FLT         427
#define RI_9FLT         428
#define RI_10FLT        429
#define RI_11FLT        430
#define RI_12FLT        431*/
#define RI_1A_FLT       153//138//132//108//432
#define RI_1B_FLT       154//139//133//433
#define RI_1C_FLT       155//140//134//434
#define RI_1O_FLT       156//141//135//435
#define RI_2A_FLT       157//142//136//436
#define RI_2B_FLT       158//143//137//437
#define RI_2C_FLT       159//144//138//438
#define RI_2O_FLT       160//145//139//439
#define RI_3A_FLT       161//146//140//440
#define RI_3B_FLT       162//147//141//441
#define RI_3C_FLT       163//148//142//442
#define RI_3O_FLT       164//149//143//443
/*#define RI_4A_FLT         444
#define RI_4B_FLT       445
#define RI_4C_FLT       446
#define RI_4O_FLT       447
#define RI_5A_FLT       448
#define RI_5B_FLT       449
#define RI_5C_FLT       450
#define RI_5O_FLT       451
#define RI_6A_FLT       452
#define RI_6B_FLT       453
#define RI_6C_FLT       454
#define RI_6O_FLT       455
#define RI_7A_FLT       456
#define RI_7B_FLT       457
#define RI_7C_FLT       458
#define RI_7O_FLT       459
#define RI_8A_FLT       460
#define RI_8B_FLT       461
#define RI_8C_FLT       462
#define RI_8O_FLT       463
#define RI_9A_FLT       464
#define RI_9B_FLT       465
#define RI_9C_FLT       466
#define RI_9O_FLT       467
#define RI_10A_FLT    468
#define RI_10B_FLT    469
#define RI_10C_FLT    470
#define RI_10O_FLT    471
#define RI_11A_FLT    472
#define RI_11B_FLT    473
#define RI_11C_FLT    474
#define RI_11O_FLT    475
#define RI_12A_FLT    476
#define RI_12B_FLT    477
#define RI_12C_FLT    478
#define RI_12O_FLT    479*/

//����״̬ ����³������ʹ��
/*#define RI_1A_STATUS  480
#define RI_1B_STATUS  481
#define RI_1C_STATUS  482
#define RI_1O_STATUS  483
#define RI_2A_STATUS  484
#define RI_2B_STATUS  485
#define RI_2C_STATUS  486
#define RI_2O_STATUS  487
#define RI_3A_STATUS  488
#define RI_3B_STATUS  489
#define RI_3C_STATUS  490
#define RI_3O_STATUS  491
#define RI_4A_STATUS  492
#define RI_4B_STATUS  493
#define RI_4C_STATUS  494
#define RI_4O_STATUS  495
#define RI_5A_STATUS  496
#define RI_5B_STATUS  497
#define RI_5C_STATUS  498
#define RI_5O_STATUS  499
#define RI_6A_STATUS  500
#define RI_6B_STATUS  501
#define RI_6C_STATUS  502
#define RI_6O_STATUS  503*/

//�ɼ���Ԫ���Ͷ��
#define RI_1_BATPOW_EN          165//
#define RI_2_BATPOW_EN          166//
#define RI_3_BATPOW_EN          167//
#define RI_BATPOW_EN                168//

#define RI_1A_BATPOW_EN         169//1A�������
#define RI_1B_BATPOW_EN     170//
#define RI_1C_BATPOW_EN         171//
#define RI_1O_BATPOW_EN     172//
#define RI_2A_BATPOW_EN     173//
#define RI_2B_BATPOW_EN     174//
#define RI_2C_BATPOW_EN     175//
#define RI_2O_BATPOW_EN     176//
#define RI_3A_BATPOW_EN     177//
#define RI_3B_BATPOW_EN     178//
#define RI_3C_BATPOW_EN     179//
#define RI_3O_BATPOW_EN     180//

//�¹���
#define RI_TOTAL       181//150//144//480//504


#define RI_ZD_LVD      182//151//145//481//505    //��վ����Դ��ѹ�澯
#define RI_ZD_BVD      183//152//146//482//506    //��վ���õ�Դ��ѹ�澯
#define RI_ZD_ZJ       184//153//147//483//507    //�ն��Լ�
#define RI_REAL_QUASI  185//154//148//484//508    //׼ʵʱ��ʵʱģʽ�л�
#define RI_EEP_ERR     186//155//149//485//509    //E2����
#define RI_IN_DAERR    187//156//150//486//510    //ң�ŵ�Ŵ���(�ڲ�ʹ��)
#define RI_STATE_CYCLE 188//157//151//487//511    //״̬�㱨����

#define RI_LUBO_FLAG   189//158//152//488//511    //¼����־λ
#define RI_GPS_ERR     190//159//153//489//511    //GPS״̬�쳣
#define RI_RF_ERR      191//160//154//489//511    //RF��ʱ�����쳣




//Ӳң�� (Ӳң�ű���ŵ�ң�ű�����)
//���� 
#define RI_ZD_YX1    192//161//155//490//512    
#define RI_ZD_YX2    193//162//156//491//513    
#define RI_ZD_YX3    194//163//157//492//514    
#define RI_ZD_YX4    195//164//158//493//515    
#define RI_ZD_YX5    196//165//159//494//516 
#define RI_ZD_YX6    197//166//160//495//517 
#define RI_ZD_YX7    198//167//161//496//518 
#define RI_ZD_YX8    199//168//162//497//519 
#define RI_ZD_YX9    200//169//163//498//520 
#define RI_ZD_YX10   201//170//164//499//521
#define RI_ZD_YX11   202//171//165//500//522   
#define RI_ZD_YX12   203////166//501//523    
#define RI_ZD_YX13   204//174//168//503//525    
#define RI_ZD_YX15   205//175//169//504//526    
#define RI_ZD_YX16   206//176//170//505//527    
#define RI_ZD_YX17   207//177//171//506//528    
#define RI_ZD_YX18   208//178//172//507//529    
#define RI_ZD_YX19   209//179//173//508//530    
#define RI_ZD_YX20   210//180//174//509//531    
#define RI_ZD_YX21   211//181//175//510//532    
#define RI_ZD_YX22   212//182//176//511//533    
#define RI_ZD_YX23   213//183//177//512//534    
#define RI_ZD_YX24   214//184//178//513//535

//˫��
#define RI_ZD_DYX1   215 //     185//179//514//536   
#define RI_ZD_DYX2   216 //     186//180//515//537   
#define RI_ZD_DYX3   217 //     187//181//516//538   
#define RI_ZD_DYX4   218 //     188//182//517//539   
#define RI_ZD_DYX5   219 //     189//183//518//540   
#define RI_ZD_DYX6   220 //     190//184//519//541   
#define RI_ZD_DYX7   221 // 191//185//520//542   
#define RI_ZD_DYX8   222 //     192////521//543   
#define RI_ZD_DYX9   223 //     193//187//522//544   
#define RI_ZD_DYX10  224 // 194//188//523//545   
#define RI_ZD_DYX11  225 //     195//189//524//546   
#define RI_ZD_DYX12  226  //  196//190//525//547




#define RMT_INFO_NUM           227//197//191//526//548         //��ң�Ÿ���
#define RMT_WORD_NUM           RMT_INFO_NUM/8+1          //��ң��ռ��WORD���� ÿ��ң��ռ2��bit��һ��WORD����8��ң��
#define RMT_WDYX_NUM            2                        // 12 ˫��ң��ռ���� 12/8+1
#define RMT_WSYX_NUM            RMT_INFO_NUM/8-1          //RMT_INFO_NUM - 12 ����ң���ָ��� (RMT_INFO_NUM - 12)/8 +1

#define RMT_ZSQST_NUM          LINE_NUM * PHASE_NUM    //��·ͨ��״̬����  12*4
#define RI_ZSQST_H             RI_3O_ZSQST//RI_12O_ZSQST           //ָʾ��״̬ң������
#define DYX_ADDR               RI_ZD_DYX1             //˫��ң����ʼ��
#define RI_FTINFO_NUM          LINE_NUM * PHASE_NUM * FTTPYE_NUM//��Ҫ������ң�Ÿ��� ��·(5*12)+�ӵ�(5*12)+�¶�(5*12)
#define RMT_SS_YY              60//��Ӧ����˲ʱ���ϵ�λ�������ù���λ�õĲ�
#define RI_FLT_NUM             LINE_NUM*PHASE_NUM    //���õ�ָʾ���ܸ�������3�ߣ�ÿ��4ָֻʾ��



#define EXT_DIN_NUM                 24    //�ܹ��Ŀ������������


#define FTTPYE_SGL   0 //˲ʱ��·
#define FTTPYE_JD    1 //�ӵ�
#define FTTPYE_LOWB  2 //��ѹ
#define FTTPYE_TEM   3 //�¶�
#define FTTPYE_CX    4 //��������
#define FTTPYE_YGL   5 //���ö�·
#define FTTPYE_GL    6 //��·
#define FTTPYE_FP    7 //ң�ط���״̬
#define FTTPYE_YD    8 //�е�
#define FTTPYE_ST    9 //ͨ��״̬(�ն��ж�)
//#define FTTPYE_XFLT  10 //�����(�ն˺ϳ�)
//#define FTTPYE_YYJD  11 //��Դ�ӵ�
//#define FTTPYE_WYJD  12 //��Դ�ӵ�
//#define FTTPYE_WD    13 //�޵�


//#define FTTPYE_NUM   10 //˲ʱ�Զ�·���ӵأ���ѹ���¶ȣ��������ޣ������Զ�·����·��ң�ط���״̬���е硢ͨ��״̬������ϡ�//��Դ�ӵء���Դ�ӵء��޵�




//============================  ʵʱʱ��        ==================================
#define RTC_NUM                         8


#define RTC_MICROSEC                    0       //����
#define RTC_SEC                         1       //��
#define RTC_MINUT                       2       //��
#define RTC_HOUR                        3       //ʱ
#define RTC_WEEK                        4       //����
#define RTC_DATE                        5       //��
#define RTC_MONTH                       6       //��
#define RTC_YEAR                        7       //��

//ң��ѡ���ȴ�ִ��ʱ��
#define REL_WAIT_TIME_101   120  
#define REL_WAIT_TIME_SMS       600          //���ſ�����  10����




//===============================  COMMģ��  =============================
#define COMM_PORT_NUM           4  //����ͨ�Ŷ˿ڸ���

#define CMID0                   0   //RF��ODU1
#define CMID1                   1   //GPRS
#define CMID2                   2   //��չ���ڻ�ODU2
#define CMID3                   3   //���Դ���
#define CMIDINV                 4   //��Чͨ��


#define OBJRF       0  
#define OBJGPRS     1
#define OBJDBG      2   
#define OBJEX       6      
#define OBJODU1     4 
#define OBJODU2     5 
#define OBJGPS      3




#define MAX_TXBUFF_LEN       256//����ʹ����һ��Э�飬�����ʱʹ�õĻ�������ͬ����˻���������ȡ���ֵ��
#define MAX_RXBUFF_LEN       384//����ʹ����һ��Э�飬�����ʱʹ�õĻ�������ͬ����˻���������ȡ���ֵ��

#define MAX_TXBUFF_101_LEN       256 //101Э�鷢�ͻ���������
#define MAX_SOE_TXNUM            8   //16//��������SOE����





//============================  SOE��¼ ===========================================
#define         SOE_BUFF_NUM            32      //SOE�ڴ滺������SOE������
#define         DSOE_BUFF_NUM           8      //˫��SOE�ڴ滺������SOE������
//#define         SOE_DATA_LEN            11       //ÿ��SOE���ݵĳ���
#define         SOE_DATA_LEN           26        //ÿ��SOE���ݵĳ��� ������5��ң�⣬ÿ��3���ֽ�
#define         SOEVALID               0X55
#define         SOE_SENDDA_LEN         10//SOE_DATA_LEN-1
//SOE���ݵĶ���
#define         SOE_FLAG    0  
#define         SOE_TYPEL   1 
#define         SOE_TYPEH   2 
#define         SOE_STVAL   3 
#define         SOE_MSL     4 
#define         SOE_MSH     5
#define         SOE_MINU    6 
#define         SOE_HOUR     7
#define         SOE_DAY     8
#define         SOE_MONTH   9  
#define         SOE_YEAR    10  


////================================  ң�س��� =======================

#define OUT_RMT1            0  //�ն˼̵�������1
#define OUT_RMT2            1  //�ն˼̵�������2
#define OUT_CLEAR_SOE       2    //���SOE��¼
#define OUT_CLEAR_LOAD      3   //�����������
#define OUT_GPRS_PW         4    //��/�ض�GPRS��Դ
#define OUT_BAT_ACT         5        //��/�ضϵ�ػ
#define OUT_CLEAR_COMTRADE  6  //���¼����¼

#define OUT_ODU_RESET       7  //YKָʾ��ͬ�鴥��¼��
#define OUT_ODU2_RESET      8  //���LOG��־
#define OUT_GPS_PW          9  //��/�ض�GPS��Դ
#define OUT_CALL_YC         10 //�ٻ�ָʾ����ң��
#define OUT_TEST_MODE       11   //�㲥����ָʾ�������Զ�������ģʽ
#define OUT_SET_MODE        12   //�㲥����ָʾ������ң������ģʽ
#define OUT_RST_ALL1        13           //��·1�ܷ���/����
#define OUT_RST_ALL2        14           //��·2�ܷ���/����
#define OUT_RST_ALL3        15           //��·3�ܷ���/����


#define OUT_RST_ZSQ1        16   //ָʾ��1����/����
#define OUT_RST_ZSQ2        17      
#define OUT_RST_ZSQ3        18  
#define OUT_RST_ZSQ4        19
#define OUT_RST_ZSQ5        20  
#define OUT_RST_ZSQ6        21  
#define OUT_RST_ZSQ7        22  
#define OUT_RST_ZSQ8        23  
#define OUT_RST_ZSQ9        24



/*#define OUT_RST_ZSQ10       22  
#define OUT_RST_ZSQ11       23  
#define OUT_RST_ZSQ12       24
#define OUT_RST_ZSQ13       22
#define OUT_RST_ZSQ14       23
#define OUT_RST_ZSQ15       24
#define OUT_RST_ZSQ16       25
#define OUT_RST_ZSQ17       26
#define OUT_RST_ZSQ18       27
#define OUT_RST_ZSQ19       28
#define OUT_RST_ZSQ20       29
#define OUT_RST_ZSQ21       30
#define OUT_RST_ZSQ22       31
#define OUT_RST_ZSQ23       32
#define OUT_RST_ZSQ24       33
#define OUT_RST_ZSQ25       34
#define OUT_RST_ZSQ26       35
#define OUT_RST_ZSQ27       36
#define OUT_RST_ZSQ28       37
#define OUT_RST_ZSQ29       38
#define OUT_RST_ZSQ30       39
#define OUT_RST_ZSQ31       40
#define OUT_RST_ZSQ32       41
#define OUT_RST_ZSQ33       42
#define OUT_RST_ZSQ34       43
#define OUT_RST_ZSQ35       44
#define OUT_RST_ZSQ36       45
//��ָʾ��ң��
#define OUT_RYC_ZSQ1        46
#define OUT_RYC_ZSQ2        47
#define OUT_RYC_ZSQ3        48
#define OUT_RYC_ZSQ4        49
#define OUT_RYC_ZSQ5        50
#define OUT_RYC_ZSQ6        51
#define OUT_RYC_ZSQ7        52
#define OUT_RYC_ZSQ8        53
#define OUT_RYC_ZSQ9        54
#define OUT_RYC_ZSQ10       55
#define OUT_RYC_ZSQ11       56
#define OUT_RYC_ZSQ12       57
#define OUT_RYC_ZSQ13       58
#define OUT_RYC_ZSQ14       59
#define OUT_RYC_ZSQ15       60
#define OUT_RYC_ZSQ16       61
#define OUT_RYC_ZSQ17       62
#define OUT_RYC_ZSQ18       63
#define OUT_RYC_ZSQ19       64
#define OUT_RYC_ZSQ20       65
#define OUT_RYC_ZSQ21       66
#define OUT_RYC_ZSQ22       67
#define OUT_RYC_ZSQ23       68
#define OUT_RYC_ZSQ24       69
#define OUT_RYC_ZSQ25       70
#define OUT_RYC_ZSQ26       71
#define OUT_RYC_ZSQ27       72
#define OUT_RYC_ZSQ28       73
#define OUT_RYC_ZSQ29       74
#define OUT_RYC_ZSQ30       75
#define OUT_RYC_ZSQ31       76
#define OUT_RYC_ZSQ32       77
#define OUT_RYC_ZSQ33       78
#define OUT_RYC_ZSQ34       79
#define OUT_RYC_ZSQ35       80
#define OUT_RYC_ZSQ36       81

//����ң�ط��Ƹ���
#define OUT_RST_ZSQ_GROUP1   82
#define OUT_RST_ZSQ_GROUP2   83
#define OUT_RST_ZSQ_GROUP3   84
#define OUT_RST_ZSQ_GROUP4   85
#define OUT_RST_ZSQ_GROUP5   86
#define OUT_RST_ZSQ_GROUP6   87
#define OUT_RST_ZSQ_GROUP7   88
#define OUT_RST_ZSQ_GROUP8   89
#define OUT_RST_ZSQ_GROUP9   90
#define OUT_RST_ZSQ_GROUP10  91
#define OUT_RST_ZSQ_GROUP11  92
#define OUT_RST_ZSQ_GROUP12  93

//�����ٻ�ң��
#define OUT_RYC_ZSQ_GROUP1   94
#define OUT_RYC_ZSQ_GROUP2   95
#define OUT_RYC_ZSQ_GROUP3   96
#define OUT_RYC_ZSQ_GROUP4   97
#define OUT_RYC_ZSQ_GROUP5   98
#define OUT_RYC_ZSQ_GROUP6   99
#define OUT_RYC_ZSQ_GROUP7   100
#define OUT_RYC_ZSQ_GROUP8   101
#define OUT_RYC_ZSQ_GROUP9   102
#define OUT_RYC_ZSQ_GROUP10  103
#define OUT_RYC_ZSQ_GROUP11  104
#define OUT_RYC_ZSQ_GROUP12  105*/


#define OUT_TYPE_NUM         25//106 

#define OUT_RMT1_ORDER         0x80  //�ն˼̵�������1ָ��ֵ
#define OUT_RMT2_ORDER         0x81  //�ն˼̵�������2ָ��ֵ
#define OUT_CLEAR_SOE_ORDER    0x82    //���SOE��¼ָ��ֵ
#define OUT_CLEAR_LOAD_ORDER   0X83 //������ɼ�¼ָ��ֵ
#define OUT_GPRS_PW_ORDER      0x84   //��/�ض�GPRS��Դָ��ֵ
#define OUT_BAT_ACT_ORDER      0x85  //��/�رյ�ػָ��ֵ
#define OUT_COMTRAD_ORDER      0x86   //���¼����¼

#define OUT_ODU_RESET_ORDER      0x87   //YKָʾ��ͬ�鴥��¼��
#define OUT_ODU2_RESET_ORDER     0x88   //���log��־
#define OUT_GPS_PW_ORDER         0x89   //��/�ض�GPS��Դָ��ֵ
#define OUT_CALL_FI_YC           0x8A   //�ٻ�ָʾ����ң�⹦��
#define OUT_CALL_AUTO_TEST_MODE  0x8B   //�㲥����ָʾ�������Զ�������ģʽ
#define OUT_CALL_SET_MODE        0x8C   //�㲥����ָʾ������ң������ģʽ
#define OUT_LINE1_FI          0x8D   //1//
#define OUT_LINE2_FI          0x8E   //2//2
#define OUT_LINE3_FI          0x8F   //3//


//=====================��������===========================================
#define FLOAD_TOTALNUM  0//��FLASH�б���ĸ��ɼ�¼������
#define FLOAD_NEW       1//�����洢�ļ�¼���һ����¼��ƫ��λ�ã�
#define FLOAD_OLD       2//����һ����¼�洢λ��
#define FLOAD_DAY       3//2//����һ����¼�洢λ��
#define FLOAD_CS        4//3//�����������ݼӺ�У��

#define FLOADINFONUM  5//4
//=====================��־��¼ulog===========================================
#define FLOG_TOTALNUM  0//��FLASH�б������־��¼������
#define FLOG_NEW       1//�����洢�ļ�¼���һ����¼��ƫ��λ�ã�
#define FLOG_OLD       2//����һ����¼�洢λ��
#define FLOG_CS        3//�����������ݼӺ�У��
#define FLOGINFONUM  4

//====================  EEPROM ��ַ����  CAT25C128 (0~0x3FFF �ֽ�) 16K�ֽ� ======================
//����CAT25128��ҳΪ64�ֽڣ������ռ����ʼ��ַ���Ϊҳ��ʼ
//EEPROM�б����������Ĳ���      ����1K�ֽڵĿռ�
#define EEPADD_RP               0x0000  //���в�����ʼ��ַ 200word

//EEPROM��IEC��������
#define EEPADD_IECYXADDR        0x0190 //ң�ŵ�ַ  1000word
#define EEPADD_IECPARA          0x0960 //IEC����   50byte
#define EEPADD_IECYCADDR        0x0992 //ң�����  1000byte 
#define EEPADD_IECYKADDR        0x0D7A //ң�ز���  100byte

//�绰����洢
#define EEPADD_PHONE            0x0DDE 

#define EEPADDBK_IECYXADDR        0x1000
#define EEPADDBK_IECPARA          0x1190
#define EEPADDBK_IECYCADDR        0x1960
#define EEPADDBK_IECYKADDR        0x1992
#define EEPADDBK_PHONE            0x1D7A

//eeprom�и��ɼ�¼��Ϣ
#define EEPADD_LOADNUM          0x0E74  //��FLASH�б���ĸ��ɼ�¼������ ռ��2�ֽ�
#define EEPADD_LOADADDR_NEW     0x0E76  //�����洢�ļ�¼���һ����¼��ƫ��λ�ã�Ϊ(0----FLASH_LOAD_MAXNUM-1) ռ��2�ֽ�
#define EEPADD_LOADADDR_OLD     0x0E78  //����һ����¼�洢λ��
#define EEPADD_LOADADDR_DAY     0x0E7A  //��¼����
#define EEPADD_LOAD_CHK         0x0E7C//0x0E7A  //�����������ݼӺ�У��

//E2�д洢SOEβָ�������ͨ�����͹��ϵ�ͷָ��
#define EEPADD_SOE_NUM          0x0E7E  //�ڲ�Flash�����SOE���ܸ���
#define EEPADD_SOE_FLASHNUM     0x0E88  //�ⲿFlash�����SOE����Ϣ������
#define EEPADD_SOE_FLASHHEAD    0x0E8A  //�ⲿFlash�����SOE����Ϣ��ͷָ��
#define EEPADD_SOE_FLASHFLAG    0x0E8C  //�ⲿFlash�����SOE����Ϣ���Ƿ�д����־λ


 
#define EEPADD_SSOETAIL         0x0E80
#define EEPADD_SSOEHEAD1        0x0E82 //��1������ͨ����ͷָ��
#define EEPADD_SSOEHEAD2        0x0E84 //��2������ͨ����ͷָ��
#define EEPADD_SSOEHEAD3        0x0E86 //��3������ͨ����ͷָ��

#define EEPADD_SFEIHEAD1        0x0E88 //��1������ͨ����ͷָ��  zx20171206 �����¼�
#define EEPADD_SFEIHEAD2        0x0E8A //��2������ͨ����ͷָ��
#define EEPADD_SFEIHEAD3        0x0E8C //��2������ͨ����ͷָ��

//E2�б���ϵͳ������Ϣ
#define EEPADD_RSTARTNUM        0x0E96 


//E2�б���¼���ĸ���
#define EEPADD_LUBOFLAG          0x0E98 //��ǰ¼��FLASH���浽��λ��
// ռ��0.5K�ֽڿռ䣬Լ512���ֽڣ�0x109A
#define EEPADD_ALEN             0x0E9A //A���ƫ����
#define EEPADD_BLEN             0x0E9C //B���ƫ����
#define EEPADD_CLEN             0x0E9E //C���ƫ����
#define EEPADD_COMLEN           0x0EA0 //���ƫ����
#define EELEN_LENG              8 //ÿ��¼��ռ�õ�ƫ�����ĳ���

#define EEPADD_LUBONUM          0x1100 // ��ǰ¼�����ܸ���

#define EEPADD_VerNum            0x1104 // ��ǰ¼�����ܸ���,ռ��8���ֽڣ�SV01.033
#define EEPADD_LOGNUM           0x1110  //��FLASH�б���ĸ��ɼ�¼������ ռ��2�ֽ�
#define EEPADD_LOGADDR_NEW      0x1112  //�����洢�ļ�¼���һ����¼��ƫ��λ�ã�Ϊ(0----FLASH_LOAD_MAXNUM-1) ռ��2�ֽ�
#define EEPADD_LOGADDR_OLD      0x1114  //����һ����¼�洢λ��
#define EEPADD_LOG_CHK          0x1116  //�����������ݼӺ�У��

#define EEPADD_DAYNUM           0x1200 //���㸺�ɼ�¼����31�죬ÿ������96����ʵ�ʴ������

#define EEPADD_VALIDITY         0x1300 //��Ч�ڣ�64��¼��Ŀ¼��ÿ��¼��Ŀ¼����Ч��Ϊ2���ֽڣ���2*64���ֽ�

#define EEPADD_FIXNUM           0x1400 //ͨ���ļ������涨�����������31*20���ֽ�


#define EEPADD_CFG              0x1700 //����CFG�ļ���ĿǰCFG�ļ����ռ��700���ֽڣ�û��д��FLASH��ԭ����:ÿ�η��͸��ļ�ʱ������Ҫ��CFG�ļ���ĳ�����ط����޸ģ��������

#define EEPADD_DEGBF101            0x1A00 // DEG�� ��д����ʱ�����ݵ�ǰ���е������д�ļ�ʱ�����������Ľ��̰������١���ʱ�������ϱ���������ʱ�����Լ�������
#define EEPADD_GPRSBF101           0x1B00 // GPRS�� ��д����ʱ�����ݵ�ǰ���е������д�ļ�ʱ�����������Ľ��̰������١���ʱ�������ϱ���������ʱ�����Լ�������


#define EEPADD_COEFFICIENT      0x2000 //¼��ϵ����ŵ���ʼλ�ã�ABC����ϵ����ÿ��4���ֽڣ���64����ռ��64*4*3
#define EEPADD_PARA               0x3800 //��������


//eeprom��¼��������Ϣ
/*#define EEPADD_LUBONUM          0x0E9A  //��FLASH�б����¼����¼������ ռ��2�ֽ�
#define EEPADD_LUBOADDR_NEW     0x0E9C  //�����洢�ļ�¼���һ����¼��ƫ��λ�ã�Ϊ(0----FLASH_LOAD_MAXNUM-1) ռ��2�ֽ�
#define EEPADD_LUBOADDR_OLD     0x0E9E  //����һ��¼����¼�洢λ��
#define EEPADD_LUBO_CHK         0x0EA0  //�����������ݼӺ�У��*/




//============================   GPIO��ӦDI DO ����  ===============================

#define   OPEN_DI_POWER()             P6OUT |= BIT6;//p10.3 �ߵ�ƽ��Ӳң�ŵ�Դ((P10OUT |= BIT3) && (P9OUT |= BIT3));//p10.3 �ߵ�ƽ��Ӳң�ŵ�Դ
#define   CLOSE_DI_POWER()            P6OUT &= NBIT6//((P10OUT &= NBIT3)&&(P9OUT &= NBIT3));

#define   READ_DI_POWER_ST          P8IN & BIT6 //P8.6 Ӳң�ŵ�Դ״̬���


#define DO00_ON     (P6OUT &= NBIT5)    //�̵�������DO00--p8.7 �͵�ƽ
#define DO00_OFF    (P6OUT |= BIT5)     //�̵�������DO00--p8.7 �ߵ�ƽ
#define DO01_ON    (P6OUT &= NBIT4)     //�̵�������DO00--p9.0 �͵�ƽ
#define DO01_OFF   (P6OUT |= BIT4)      //�̵�������DO00--p9.0 �ߵ�ƽ

#define OPEN_BAT_ACT     P7OUT |= BIT3//Ӳ������Ԥ������ʱд���IOP5OUT |= BIT5  //�򿪵�ظ���ѹ�ɼ�(��ػҲ�����IO) p5.5 �ߵ�ƽ
#define CLOSE_BAT_ACT    P7OUT &= NBIT3//P5OUT &= NBIT5


//================================led=================
#define   LIGHT_RUN_UP()              P7OUT &= NBIT7;//P7OUT &= NBIT4;//p7.4 ���е�
#define   LIGHT_RUN_DOWN()            P7OUT |= BIT7;//P7OUT |= BIT4;
#define   LIGHT_RUN_FLASH()           P7OUT ^= BIT7;//P7OUT ^= BIT4;

#define   LIGHT_RUN2_UP()              P4OUT &= NBIT7;//p7.4 ���е�
#define   LIGHT_RUN2_DOWN()            P4OUT |= BIT7;
#define   LIGHT_RUN2_FLASH()           P4OUT ^= BIT7;


#define FEED_WATCH_DOG()   P5OUT ^= BIT4;//P7OUT ^= BIT5;// p7.5 ���Ź� 

//============================ AD�ɼ� GPIO����  ===============================

#define OPEN_BAT_SAMPLE   P10OUT |= BIT2 //�򿪵�ص�ѹ�ɼ� p10.2 �ߵ�ƽ
#define CLOSE_BAT_SAMPLE  P10OUT &= NBIT2

#define OPEN_SUN_SAMPLE   P10OUT |= BIT1//��̫���ܵ�ѹ�ɼ� p10.1 �ߵ�ƽ
#define CLOSE_SUN_SAMPLE  P10OUT &= NBIT1

#define OPEN_C_SAMPLE     P10OUT |= BIT0//�򿪵��ݵ�ѹ�ɼ� p10.0 �ߵ�ƽ
#define CLOSE_C_SAMPLE    P10OUT &= NBIT0

#define OPEN_BATF_SAMPLE     P7OUT |= BIT3//P5OUT |= BIT5//�򿪵�ظ���ѹ�ɼ�(��ػҲ�����IO) p5.5 �ߵ�ƽ
#define CLOSE_BATF_SAMPLE    P7OUT &= NBIT3//P5OUT &= NBIT5

#define OPEN_SUN_CHARGE     P5OUT &= NBIT5// P4OUT &= NBIT6 //��س����� P4.6 �͵�ƽ���
#define CLOSE_SUN_CHARGE    P5OUT |= BIT5//P4OUT |= BIT6 //�ߵ�ƽֹͣ���


//============================  ����ģ�� GPIO����  ===============================
#define OPEN_WIRE_POWER               P4OUT &= NBIT6//P4OUT &= NBIT0 //P4.0 ���ߵ�Դ���� �͵�ƽ��ͨ
#define CLOSE_WIRE_POWER              P4OUT |= BIT6//P4OUT |= BIT0

#define   WIRE_LESS_RST_L()           P4OUT |= BIT3;//P4OUT |= BIT1;//p4.1 RF��λ �ߵ�ƽ��λ
#define   WIRE_LESS_RST_H()           P4OUT &= NBIT3;//p4.1 RF��λ �͵�ƽ����

#define   READ_WIRE_LESS_WDT          (P4IN & BIT4) >> 2 //RF���� p4.2

#define OPEN_YK_TRIGGER               P4OUT |= BIT5//P1OUT |= BIT2   //P4.0 ���ߵ�Դ���� �͵�ƽ��ͨ
#define CLOSE_YK_TRIGGER              P4OUT &= NBIT5//P1OUT &= NBIT2



//#define   SET_WIRE_LESS_WDT_IN        (P4DIR &= NBIT2)

//============================  ﮵�ؿ���  ===============================
#define OPEN_BAT_POWER                P7OUT |= BIT2;g_batPowerSt = 1;//P4OUT |= BIT7;P4.0 ���ߵ�Դ���� �͵�ƽ��ͨ
#define CLOSE_BAT_POWER              P7OUT &= NBIT2;g_batPowerSt=0;//P4OUT &= NBIT7;

//============================  GPRS GPIO����  ===============================

#define OPEN_GPRS_POWER    P10OUT |= BIT3;g_GprsPowerSt = 1; //P4OUT |= BIT3GPRS�Դ���� p4.3 �ߵ�ƽ��ͨ��Դ
#define CLOSE_GPRS_POWER   P10OUT &= NBIT3;g_GprsPowerSt = 0;

#define GPRS_SETA_H()  //P1OUT |= BIT5 //P6.5 ��������P6OUT |= BIT5
#define GPRS_SETA_L()  //P1OUT &= NBIT5

#define GPRS_RST_H()  //P1OUT |= BIT6//P6.4 ��λ����P6OUT |= BIT4
#define GPRS_RST_L()  //P1OUT &= NBIT6

#define GPRS_ONOFF_H()  P6OUT |= BIT0 //P6OUT |= BIT6 P6.6 GPRS�����ź�
#define GPRS_ONOFF_L()  P6OUT &= NBIT0
#define GPRS_ONOFF_IN()  P6SEL |= BIT0

//============================  GPS����  ===============================
#define OPEN_GPS_POWER               P1OUT |= BIT7;P9SEL |= 0x30;P9DIR |= BIT4;g_gpsPowerSt = 1;//P9SEL = 0x30;P4OUT |= BIT7;P4.0 ���ߵ�Դ���� �͵�ƽ��ͨ
#define CLOSE_GPS_POWER              P1OUT &= NBIT7;P9SEL &= 0xCF;P9DIR &=NBIT4;g_gpsPowerSt=0;//P4OUT &= NBIT7;P9DIR &=NBIT5;P9OUT &= 0xCF;



//============================  �ڲ�flash�洢  ===============================

#define SOE_RECORD_ADD   0x42000     //SOE��¼��ַ��

#define SOE_NUM_SEG   42//ÿ�δ�42����¼
#define SOE_SEG       24 //24�Σ�ÿ��512���ֽ�

#define SOE_RECORD_LEN  32//30//0x10        //ÿ����¼16�ֽ�
#define SOE_RECORD_NUM  32          //32����¼

//====================  FLASH ��ַ����  SST26VF064  ÿҳ256�ֽڣ�16ҳ= 4K ��2048 * 16 = 32768ҳ  8M======================
#define FLASH_PAGE_SIZE         256

//FLASH�и������� �ӵ�1ҳ��ʼ��ţ����1080ҳ
#define FADDR_LOAD_START         0x0             //��0��ַ���渺������
#define FADDR_LOAD_END          0x5D000 //0x43800�ܹ����1488ҳ�ĸ��ɼ�¼ 1488 * 256 = 0x5d000

#define FLASH_PLOAD_LEN         128      //FLASH�б��浥�����ɼ�¼ռ�õ��ֽ���
#define FLASH_PPAGE_LNUM        2      //FLASH��ÿҳ�ĸ��ɼ�¼����Ϊ2��  256 = 2*128
#define FLASH_PAGE_NUM          ((31*96)/2)//ÿҳ����2�����ɼ�¼���ܹ�ռ1080ҳ //32
#define FLASH_LOAD_MAXNUM       (31*96)  //����ĸ��ɼ�¼���������90��*24��=2160//64
#define FLASH_DAYLOAD_LEN       0x3000 //ÿ�챣���ң���¼��ռ�ĵ�ַ

//FLASH��SOE����        �ӵ�1500(1100)ҳ��ʼ������64ҳ ÿҳ��16��SOE ÿ��ռ16byte
#define FADDR_SOE_START         0x5E000//0x44C00            

#define FSOE_TOLNUM      512 //1024  //�����ⲿflash��SOE�������� 64ҳ�ܴ�1024��
#define FSOE_PNUM        16    //ÿҳ��16��SOE
#define FSOE_LEN         32//16    //û��SOEռ16�ֽ� �ڴ���ÿ��ռ11�ֽڣ�����Ϊ��flash�����״����Ԥ����5�ֽ�




//FLASH��¼������        �ӵ�1200ҳ��ʼ������640ҳ ÿ16ҳ(3840)��һָֻʾ����һ��¼�����ݣ�16*4=64ҳ��һ����������¼�����ݣ��ɴ�10��������¼������

#define FADDR_RECORDER_START         0x66000//0x64000//0x4B000   //����¼����ʼ��ַ         

#define FRECORDER_TOLNUM      64//40//20  //�����ⲿflash��¼��������20��������¼��
//#define FRECORDER_PNUM        16    //ÿ16ҳ��һ��¼������
#define WRITEPECORDER  228//д������ݸ��� ����ַ��3��+ƫ�Ƶ�ַ��3��+��������1��+�������ݣ�224��+�ۼӺͣ�1��
#define FLINEADDR  (64*256)
#define FPHASEADDR (16*256)

#define FADDR_RECORDER_STARTV        0x167000//0x165000//0x14C000   //��ѹ¼����ʼֵ��ַ 
//#define FADDR_RECORDER_STARTV        0x9C000   //��ѹ¼����ʼֵ��ַ 
#define RECORDER  (80*15)//�ϴ�����վ�ı��ĳ���
#define RECORDER_INDI  (80*14)//�޸ĸ�����Ҫ�޸ĵĵط� ָʾ�������ն˵�¼������


#define FADDR_RECORDER_INFO         0x26B000//0x269000//0x250000   //¼��Ŀ¼��Ϣ A20*100A
//#define FADDR_RECORDER_INFO       0xF0000   //¼��Ŀ¼��Ϣ A20*100A
//#define FADDR_RECORDER_INFOB       0xF1000   //¼��Ŀ¼��Ϣ A20*100
//#define FADDR_RECORDER_INFOC       0xF2000   //¼��Ŀ¼��Ϣ A20*100
//#define FADDR_RECORDER_INFO0       0xF3000   //¼��Ŀ¼��Ϣ A20*100
#define FADDR_WAVE_FILTERI           0x36B000//0x369000//0x350000  //����¼�����ݵ��˲�

#define FADDR_WAVE_FILTERV           0x3AB000//0x3A9000//0x390000//��ѹ¼�����ݵ��˲�
//FLASH����־��¼ �ӵ�16384ҳ��ʼ��ţ����1024ҳ
#define FADDR_LOG_START         0x402000//0x400000             //��0��ַ������־��¼
#define FADDR_LOG_END           0x442000//0x440000 //0x400000�ܹ����1024ҳ����־��¼

#define FLASH_PLOG_LEN         256      //FLASH�б��浥����־��¼ռ�õ��ֽ���
//#define FLASH_PPAGE_LNUM        2      //FLASH��ÿҳ�ĸ��ɼ�¼����Ϊ2��  256 = 2*128
//#define FLASH_PAGE_NUM          ((31*96)/2)//ÿҳ����2�����ɼ�¼���ܹ�ռ1080ҳ //32
#define FLASH_LOG_MAXNUM       1024  //�������־��¼���1024

#define RECORDER_FILTER_LENGTH      80   //�˲��������Чֵ�ĳ���   

#define COEFFICIENT_LINE_LENG       12
#define COEFFICIENT_PHASE_LENG      4

//#define FADDR_RECORDER_VINFO       0xF4000   //¼��Ŀ¼��Ϣ 20*100
//#define FADDR_RECORDER_VINFOB       0xF5000   //¼��Ŀ¼��Ϣ 20*100
//#define FADDR_RECORDER_VINFOC       0xF6000   //¼��Ŀ¼��Ϣ 20*100
//#define FADDR_RECORDER_VINFO0       0xF7000   //¼��Ŀ¼��Ϣ 20*100



//#define RECORDER_MULU_LENGTH      13  //�ظ�ÿ��¼��Ŀ¼�ĳ���

//#define FADDR_RECORDER_START_BK         0x100000   //����¼����ʼ��ַ 
//#define FADDR_RECORDER_STARTV_BK        0x151000   //��ѹ¼����ʼֵ��ַ 


//#define FSOE_LEN         16    //û��SOEռ16�ֽ� �ڴ���ÿ��ռ11�ֽڣ�����Ϊ��flash�����״����Ԥ����5�ֽ�




//===============================��ڱ�־���ݼ�����У��洢��================
#define FLASH_UPDATE_COUNTER     0x7AF000 //������
#define FLASH_UPDATE_CHECK       0x7AF002 //У��ֵ
#define FLASH_UPDATE_STATE       0x7AF004 //����״̬
#define FLASH_UPDATE_EARASE       0x7AF005//�ڲ�flash������ʾ���ڱ�ʾ�����������Ƿ��Ѿ����ڲ�flash�������Է�ֹ�ڲ����Ժ�ı�����״̬��������ȥѰ��ԭ�г������
#define FLASH_UPDATE_CCHECK       0x7AF006 //У��
//����
#define FLASH_UPDATEBK_COUNTER   0x7AF008 //������
#define FLASH_UPDATEBK_CHECK     0x7AF00A //У��ֵ
#define FLASH_UPDATEBK_STATE     0x7AF00C //����״̬
#define FLASH_UPDATEBK_EARASE     0x7AF00C //flash������־
#define FLASH_UPDATEBK_CCHECK     0x7AF00D //У��

//===================================Ӧ�ó���洢��===========================
#define FLASH_UPDATE_START      0X7B0000       //
#define FLASH_UPDATE_END        0X7EFFFF       //
#define FLASH_UPCOUNTER          0x7F0000//��Ч��������      


#define ERASE_DELAY 30000//������д������ʱ��̫С������д�����ȫ1��
#define ERASEFLAGADDR   0x45A00//�ڲ�flash��ַ��jtag��д�����־�׵�ַ,ʹ��6���ֽڱ�ʾ���ڳ����������֮��ȫ��дΪ0x55
//============================= SMSУ��=======================================
const static unsigned short fcsTab[256] = {
      0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
      0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
      0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
      0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
      0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
      0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
      0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
      0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
      0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
      0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
      0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
      0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
      0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
      0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
      0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
      0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
      0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
      0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
      0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
      0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
      0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
      0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
      0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
      0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
      0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
      0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
      0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
      0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
      0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
      0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
      0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
      0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
   };



#define  GETBIT(p,i)   ((p[(i>>4)] & BIT[(i&0x0F)]) && 1)
#define  SETBIT(p,i,v) p[(i>>4)] = (v) ? (p[(i>>4)]|BIT[(i&0x0F)]) : (p[(i>>4)]&~BIT[(i&0x0F)])



//============================  ���Զ���  ===============================
//#define GPRS_POWERDOWN_TIME 180   //300

//#define ODU_LINE_NUM  12

//#define BAT_POWERDOWN_TIME  180  //180��ش򿪵�ʱ���GPRS�򿪵�ʱ����Ҫһ��
#endif

