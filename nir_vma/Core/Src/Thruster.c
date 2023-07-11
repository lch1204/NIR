#include "Thruster.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;



void Thruster_Init()
{
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);


	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);


	Thruster_Stop();

}

void Thruster_Stop()
{
	int speed = 200;

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, speed);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, speed);

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);

	HAL_Delay(4000);
}

void Thruster_Set_Speed(uint16_t* speed)
{

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, speed[2]);//3
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, speed[1]);//1
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, speed[0]);//7


	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed[5]);//2
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed[4]);//4
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed[3]);//5

}

void Thruster_Set_Perc(int *speed)
{
//	int sec_speed[8];
//	for(int i = 0;i<8;i++)
//	{
//		if((speed[i]<100)&&(speed[i]>-100))
//			sec_speed[i] = speed[i]*kpercsec+150;
//		else
//			sec_speed[i] = 150;
//	}
//	sec_speed[4] = 200;

	Thruster_Set_Speed(speed);

}
