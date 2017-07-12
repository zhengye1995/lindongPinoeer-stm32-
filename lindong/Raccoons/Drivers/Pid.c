
#include "pid.h"

s32 VV_MAX = 1800;  //���PID �������
s32 VV_MIN = -1800;    //���PID ������

s16 TV_ADD_MAX = 100;
s16 TV_ADD_MIN = -100;

s16 PV_ADD_MAX = 20;
s16 PV_ADD_MIN = -20;
s32	POSE_DEADLINE = 5000;
s32 VV_DEADLINE = 3;//�ٶ�PID������������Χ
s32 TT_DEADLINE = 100;	//ת��PID������������Χ
PID L_PID;
PID R_PID;
PID TURN_PID;
PID POSE_PID;
PID left;
PID right;



void final_PIDInit (void)
{
		left.v_Kp = 3;
		left.v_Ki = 1;
		left.v_Kd = 0;
		right.v_Kp = 3;
		right.v_Ki = 1;
		right.v_Kd = 0;
		V_PIDreset(&left);
		V_PIDreset(&right);
}
	
/********************************************************************************************************
Function Name: V_PIDInit
Description  :
Inputs       : None
Outputs      : None
Notes        : PID��ʼ��
Revision     :
********************************************************************************************************/
void V_PIDInit (void)
{
    L_PID.vi_Ref = 0;	      	     //�ٶ��趨ֵ
    L_PID.vi_FeedBack = 0 ;		//�ٶȷ���ֵ

    L_PID.vi_PreError = 0 ;	  	//ǰһ�Σ��ٶ����,,vi_Ref - vi_FeedBack
    L_PID.vi_PreDerror = 0 ;	  	//ǰһ�Σ��ٶ����֮�d_error-PreDerror;

    L_PID.v_Kp = 100;
    L_PID.v_Ki = 100;
    L_PID.v_Kd = 5;

    L_PID.motor_PreU = 500;		//����������ֵ

    R_PID.vi_Ref = 0;	      	     //�ٶ��趨ֵ
    R_PID.vi_FeedBack = 0 ;		//�ٶȷ���ֵ

    R_PID.vi_PreError = 0 ;	  	//ǰһ�Σ��ٶ����,,vi_Ref - vi_FeedBack
    R_PID.vi_PreDerror = 0 ;	  	//ǰһ�Σ��ٶ����֮�d_error-PreDerror;

    R_PID.v_Kp = 100;
    R_PID.v_Ki = 100;
    R_PID.v_Kd = 5;

    R_PID.motor_PreU = 500;		//����������ֵ

    TURN_PID.vi_Ref = 0;	      	     //�ٶ��趨ֵ
    TURN_PID.vi_FeedBack = 0;		//�ٶȷ���ֵ

    TURN_PID.vi_PreError = 0 ;	  	//ǰһ�Σ��ٶ����,,vi_Ref - vi_FeedBack
    TURN_PID.vi_PreDerror = 0 ;	  	//ǰһ�Σ��ٶ����֮�d_error-PreDerror;

    TURN_PID.v_Kp = 1000;
    TURN_PID.v_Ki = 50;
    TURN_PID.v_Kd = 2;

    TURN_PID.motor_PreU = 0;		//����������ֵ

    POSE_PID.vi_Ref = 0;	      	     //�Ƕ��趨ֵ
    POSE_PID.vi_FeedBack = 0;		//�ٶȷ���ֵ

    POSE_PID.vi_PreError = 0 ;	  	//ǰһ�Σ��ٶ����,,vi_Ref - vi_FeedBack
    POSE_PID.vi_PreDerror = 0 ;	  	//ǰһ�Σ��ٶ����֮�d_error-PreDerror;

    POSE_PID.v_Kp = 10000;
    POSE_PID.v_Ki = 10;
    POSE_PID.v_Kd = 5;

    POSE_PID.motor_PreU = 0;		//����������ֵ
		
		
		left.v_Kp = 3;
		left.v_Ki = 1;
		left.v_Kd = 0;
		right.v_Kp = 3;
		right.v_Ki = 1;
		right.v_Kd = 0;
}

void V_PIDreset(PID * pp)
{
	pp->motor_PreU=0;
	pp->PreI=0;
	pp->vi_FeedBack=0;
	pp->vi_PreDerror=0;
	pp->vi_PreError=0;
	pp->vi_Ref=0;
	pp->vl_PreU=0;
}
/********************************************************************************************************
Function Name:V_PIDCalc
Description  :
Inputs       : None
Outputs      : None
Notes        : �ٶ�PID����
Revision     :
********************************************************************************************************/
s32 V_PIDCalc( PID *pp )
{
    SINT32 error = 0;
		SINT32 d_error = 0;
	  SINT32 dd_error = 0;
	  SINT32 PreIntegral = 0;
	
		//pp->vi_FeedBack = pp->vi_FeedBack;
    error = pp->vi_Ref - pp->vi_FeedBack;	// ƫ�����
	  //pp->vi_SUMDerror+=error;
    d_error = error - pp->vi_PreError;
    //dd_error = d_error - pp->vi_PreDerror;
    PreIntegral = pp->PreI+ error;
	
    pp->vi_PreError = error;		//�洢��ǰƫ��
    pp->vi_PreDerror = d_error;
	  pp->PreI = PreIntegral;

    if( ( error < VV_DEADLINE ) && ( error > -VV_DEADLINE ) )	//���õ�������
		{
				pp->motor_PreU = pp->vl_PreU;
		}
    else								//�ٶ�PID ����
    {
       pp->motor_PreU =  (pp -> v_Kp * error + (SINT32)((pp -> v_Ki *  PreIntegral)) + pp->v_Kd * d_error) >> 1;
			//pp->motor_PreU =  (s32)(pp -> v_Kp * error + pp->v_Kd * (d_error-dd_error) ) >> 1;
			//pp->motor_PreU =  (s32)(pp -> v_Kp * error+ ((float)(pp -> v_Ki * pp->vi_SUMDerror))/100 + pp->v_Kd * (d_error-dd_error) ) >> 3;
    }

    if( pp->motor_PreU >= VV_MAX ) 		//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = VV_MAX;
    }
    else if( pp->motor_PreU <= VV_MIN)	//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = VV_MIN;
    }
		pp->vl_PreU = pp->motor_PreU;
    return (pp->motor_PreU);		//  ����Ԥ����ռ�ձ�
}

/********************************************************************************************************
Function Name: V_PIDCalc
Description  :
Inputs       : None
Outputs      : None
Notes        : ת��PID����
Revision     :
********************************************************************************************************/
s32 TURN_PIDCalc( PID *pp )
{

    s32  error,d_error,dd_error;
    error = (s32)(pp->vi_Ref) - (s32)(pp->vi_FeedBack);	// ƫ�����
    d_error = error - (s32)(pp->vi_PreError);
    dd_error = d_error - (s32)pp->vi_PreDerror;

    pp->vi_PreError = error;		//�洢��ǰƫ��
    pp->vi_PreDerror = d_error;
    if( ( error < TT_DEADLINE ) && ( error > -TT_DEADLINE ) )
        ;	//���õ�����
    else								//�ٶ�PID ����
    {
        pp->motor_PreU +=  (s32)(pp -> v_Kp * d_error+ pp -> v_Ki * error+ pp->v_Kd*dd_error) / 1280000;
    }
    if( pp->motor_PreU >= TV_ADD_MAX ) 		//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = TV_ADD_MAX;
    }
    else if( pp->motor_PreU <= TV_ADD_MIN)	//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = TV_ADD_MIN;
    }
    return (pp->motor_PreU);		//  ����Ԥ����ռ�ձ�
}

/********************************************************************************************************
Function Name:V_PIDCalc
Description  :
Inputs       : None
Outputs      : None
Notes        : ��̬PID����
Revision     :
********************************************************************************************************/
s32 POSE_PIDCalc( PID *pp )
{
    s32  error,d_error,dd_error;
    error = (s32)(pp->vi_Ref) - (s32)(pp->vi_FeedBack);	// ƫ�����
    d_error = error - (s32)(pp->vi_PreError);
    dd_error = d_error - (s32)pp->vi_PreDerror;

    pp->vi_PreError = error;				//�洢��ǰƫ��
    pp->vi_PreDerror = d_error;
    if( ( error < POSE_DEADLINE ) && ( error > -POSE_DEADLINE ) )
        ;	//���õ�����
    else								//�ٶ�PID ����
    {
        pp->motor_PreU +=  (s32)(pp -> v_Kp * d_error+ pp -> v_Ki * error+ pp->v_Kd*dd_error) / 80000;
    }
    if( pp->motor_PreU >= PV_ADD_MAX ) 		//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = PV_ADD_MAX;
    }
    else if( pp->motor_PreU <= PV_ADD_MIN)	//�ٶ�PID ����ֹ����������
    {
        pp->motor_PreU = PV_ADD_MIN;
    }
    return (pp->motor_PreU);		//  ����Ԥ����ռ�ձ�
}