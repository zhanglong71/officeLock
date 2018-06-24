#ifdef  ADC_GLOBALS
#define ADC_EXT
#else
#define ADC_EXT extern
#endif
#define INT8U unsigned char        //�޷���8λ��
#define INT8S signed char          //�з���8λ��
#define INT16U unsigned int        //�޷���16λ��
#define INT16S signed int          //�з���16λ��
#define INT32U unsigned long       //�޷���32λ��
#define INT32S signed long         //�з���32λ��
#define FP32 float                 //�����ȸ�����
#define FP64 double                //˫���ȸ�����

#define  CN_INIT_ADC     0x80   //ѡ��һ·ADC��������ADC��Դ,����Ϊ840��ʱ������
#define  CN_INIT_AUXR    0x30   //t0,t1Ϊ��ͳ��ʱ��,UARTΪ��ͳ����,EADCI=1,ESPI=0,ELVDI=0



#define ADC_BUF_SIZE 32
#define ADC_BUF_TIAL 8                        //�����ȥ������ADֵ�ĸ���
                       

ADC_EXT INT8U  ADBuf[ADC_BUF_SIZE];        //ADת������
ADC_EXT INT8U  ADCBuf[ADC_BUF_SIZE];       //ADת������
ADC_EXT INT8U  PointADC;                   //ADת������ָ��
ADC_EXT INT8U  ADCrackDat;                 //ADת��У׼����


ADC_EXT void ADC_Init();
ADC_EXT void ADC_ArrIbub(INT8U p[],INT8U n);     //����������