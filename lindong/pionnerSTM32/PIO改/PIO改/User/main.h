#ifndef __MAIN_H__
#define __MAIN_H__

/*-----------------------------------------  I N C L U D E S  -----------------------------------------*/
#include "Global.h"

/*---------------------------------------  D E F I N I T I O N  ---------------------------------------*/

/*-------------------------------------------  M A C R O S  -------------------------------------------*/

/*--------------------------------------  D E C L A R A T I O N  --------------------------------------*/
/* Internal Variable */

/* External Variable */

/* Internal Function */
void SystemStart(void);

void Test_InputIO(void); //�����������
void Test_OutputIO(void);//�����������
void Test_Sensor(void);  //ģ���������
void Test_Usart(void);   //���ڲ���
void Test_ZLG7290(void); //���̲���
void Test_Motor(void);   //�������
void Test_Servo(void);   //�������
void Test_Encoder(void); //����������
void Test_V_PID(void);   //����ջ�����
void Motor_CtrlLR(s32 RefL,s32 RefR);
void Motor_CtrlLR2(s32 RefL,s32 RefR);

/* External Function */
extern s16 recvSpeed;
extern s16 recvAngle;
extern unsigned char UART4rec[9];
extern int UART4_RecvFin;
extern void SpeedSeed(void);
extern unsigned char UART4recdma[9];
#endif

//===========================================  End Of File  ===========================================//

