#ifndef __PASSWORD_H
#define __PASSWORD_H
/**********************************************�����ṹ��**************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

//������
enum
{
	receive_keyNumber = 1,
};
#define Password_buffer_len 20	//���뻺���С
#define	Passwore_Digit			6		//����λ��
#define door_count					4 	//������
typedef __packed struct
{
	u8 state;
	u8 keyNumber;			//��������
	u8 right_flag;		//������ȷ��־λ
	u8 end;						//���뻻����λ
	u8 start;					//���뻻��ʼλ
	u8 count;					//��������λ��
	u8 buffer[Password_buffer_len];				//�������뻺��
	u8 deal_buffer[Password_buffer_len];	//��������뻺��
}PasswordStruct;
extern PasswordStruct Password[door_count];

void Password_Deal(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
