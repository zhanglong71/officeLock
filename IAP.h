#ifdef  IAP_GLOBALS
#define IAP_EXT
#else
#define IAP_EXT  extern
#endif


/*        �������⹦�ܼĴ�������        */
sfr		ISP_DATA	=	0xe2;
sfr   ISP_ADDRH	=	0xe3;
sfr		ISP_ADDRL	= 0xe4;
sfr		ISP_CMD		= 0xe5;
sfr		ISP_TRIG	= 0xe6;
sfr		ISP_CONTR = 0xe7;


#define READ_AP_and_Data_Memory_Command				      0x01        /*  �ֽڶ�Ӧ�ó����������ݴ洢��   */
#define PROGRAM_AP_and_Data_Memory_Command          0x02        /*  �ֽڱ��Ӧ�ó����������ݴ洢�� */
#define SECTOR_ERASE_AP_and_Data_Memory_Command     0x03        /*  ��������Ӧ�ó����������ݴ洢�� */


#define		DELAY_CONST         60000

/* ���峣�� */
#define ERROR   0
#define OK      1

/* ����Flash �����ȴ�ʱ��20MHz */
#define WAIT_TIME        0x03


#define USED_BYTE_QTY_IN_ONE_SECTOR                128

IAP_EXT INT8U xdata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];


#define ADR_FTAB        0x0600   //ָ��ռ��ӳ�����ݴ���׵�ַ
#define ADR_USERPWR     0x0800   //�����û��������ݴ���׵�ַ
#define ADR_SUPERPWR    0x0A00   //���ù����������ݴ���׵�ַ
#define IAP_ADR_DEFAULT 0x0C00   //��ʼ����ʶ��ַ
#define ADR_ADCRACK     0x0e00   //ADת��У׼����


IAP_EXT void ISP_IAP_enable(void);
IAP_EXT void ISP_IAP_disable(void);
IAP_EXT INT8U IAP_read(INT16U byte_addr);
IAP_EXT INT8U IAP_erase(INT16U sector_addr);
IAP_EXT INT8U IAP_pro_ver(INT16U byte_addr, INT8U original_data);
