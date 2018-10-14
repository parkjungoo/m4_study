#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h" // again, added because ST didn't put it here ?

GPIO_InitTypeDef  GPIO_InitStructure;

void Delay(__IO uint32_t nCount);
int main(void)
{
	/* LED GPIO Write Configure...  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	/* Switch GPIO Read Configure.. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);

	while (1)
	{

		if( GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 1)	GPIO_SetBits(GPIOC, GPIO_Pin_6);
		else												GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		if( GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 1)	GPIO_SetBits(GPIOB, GPIO_Pin_0);
		else												GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		if( GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 1)	GPIO_SetBits(GPIOB, GPIO_Pin_5);
		else												GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	}
}
void Delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif
