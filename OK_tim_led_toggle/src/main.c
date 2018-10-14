#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h" // again, added because ST didn't put it here ?

uint32_t led_cnt_tim2 = 1;
uint32_t led_cnt_tim3 = 1;
uint32_t led_cnt_tim4 = 1;
uint32_t led_cnt_tim5 = 1;

void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);
void TIM2_Init(void);
void TIM3_Init(void);
void TIM4_Init(void);
void TIM5_Init(void);

int main(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	TIM2_Init();
	TIM3_Init();
	TIM4_Init();
	TIM5_Init();

	while (1)
	{
	}
}

void TIM2_Init(void)
{
	//    TIMER interrupt TIM2
	NVIC_InitTypeDef TIM2_NVIC;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM2_NVIC.NVIC_IRQChannel = TIM2_IRQn;  // IRQ 채널 설정
	TIM2_NVIC.NVIC_IRQChannelPreemptionPriority = 0; // interrupt 우선순위 0
	TIM2_NVIC.NVIC_IRQChannelSubPriority =1;
	TIM2_NVIC.NVIC_IRQChannelCmd = ENABLE; // IRQ 활성화
	NVIC_Init(&TIM2_NVIC);  // 매개변수 초기화

	/*
	 * TIM3~4 = 16bit ( 2^16 = 65,536 )
	 Yuo have to consider when you make the period because of the 16bit of TIM3~4.
	 It means that it can't exceed 65,536 of the period.
	*/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	// TIM2클럭 공급// APB1 max clk= 84Mhz     1~6    5535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84 -1;  
	// TIM2 APB1클럭 발생// 84Mhz /84hz = 1us
	TIM_TimeBaseInitStruct.TIM_Period = 100000 -1;     
	// 주기설정      //  1us * 100000 = 100ms(0.1s)
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  
	//Timer Up Counter
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);   // TIM2 interrupt enable
	TIM_Cmd(TIM2, ENABLE);  // TIM2 enable
}

void TIM3_Init(void)
{
	//    TIMER interrupt TIM3
	NVIC_InitTypeDef TIM3_NVIC;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM3_NVIC.NVIC_IRQChannel = TIM3_IRQn;  // IRQ 채널 설정
	TIM3_NVIC.NVIC_IRQChannelPreemptionPriority = 0; // interrupt 우선순위 0
	TIM3_NVIC.NVIC_IRQChannelSubPriority =1;
	TIM3_NVIC.NVIC_IRQChannelCmd = ENABLE; // IRQ 활성화
	NVIC_Init(&TIM3_NVIC);  // 매개변수 초기화

	/*
	 * TIM3~4 = 16bit ( 2^16 = 65,536 )
	 Yuo have to consider when you make the period because of the 16bit of TIM3~4.
	 It means that it can't exceed 65,536 of the period.
	*/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// TIM3클럭 공급// APB1 max clk= 84Mhz     1~6    5535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 840 - 1;
	// TIM3 APB1클럭 발생//		84000000hz / 840hz = 100000s
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;
	// 주기설정      //			10000 / 100000 = 0.1s ( 100ms )
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//Timer Up Counter
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);   // TIM3 interrupt enable
	TIM_Cmd(TIM3, ENABLE);  // TIM3 enable
}

void TIM4_Init(void)
{
	//    TIMER interrupt TIM4
	NVIC_InitTypeDef TIM4_NVIC;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM4_NVIC.NVIC_IRQChannel = TIM4_IRQn;  // IRQ 채널 설정
	TIM4_NVIC.NVIC_IRQChannelPreemptionPriority = 0; // interrupt 우선순위 0
	TIM4_NVIC.NVIC_IRQChannelSubPriority =1;
	TIM4_NVIC.NVIC_IRQChannelCmd = ENABLE; // IRQ 활성화
	NVIC_Init(&TIM4_NVIC);  // 매개변수 초기화

	/*
	 * TIM3~4 = 16bit ( 2^16 = 65,536 )
	 Yuo have to consider when you make the period because of the 16bit of TIM3~4.
	 It means that it can't exceed 65,536 of the period.
	 */

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// TIM3클럭 공급// APB1 max clk= 84Mhz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 840 - 1;
	// TIM3 APB1클럭 발생//     84000000hz / 840hz = 100000s
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;
	// 주기설정      //         10000 / 100000 = 0.1s ( 100ms )
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//Timer Up Counter
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);   // TIM4 interrupt enable
	TIM_Cmd(TIM4, ENABLE);  // TIM4 enable
}


void TIM5_Init(void)
{
	//    TIMER interrupt TIM5
	NVIC_InitTypeDef TIM5_NVIC;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM5_NVIC.NVIC_IRQChannel = TIM5_IRQn;  // IRQ 채널 설정
	TIM5_NVIC.NVIC_IRQChannelPreemptionPriority = 0; // interrupt 우선순위 0
	TIM5_NVIC.NVIC_IRQChannelSubPriority =1;
	TIM5_NVIC.NVIC_IRQChannelCmd = ENABLE; // IRQ 활성화
	NVIC_Init(&TIM5_NVIC);  // 매개변수 초기화

	/*
	 * TIM3~4 = 16bit ( 2^16 = 65,536 )
	 Yuo have to consider when you make the period because of the 16bit of TIM3~4.
	 It means that it can't exceed 65,536 of the period.
	*/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	// TIM5클럭 공급// APB1 max clk= 84Mhz     1~6    5535
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84 -1;
	// TIM5 APB1클럭 발생// 84Mhz /84hz = 1us
	TIM_TimeBaseInitStruct.TIM_Period = 100000 -1;
	// 주기설정      //  1us * 100000 = 100ms(0.1s)
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//Timer Up Counter
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);   // TIM5 interrupt enable
	TIM_Cmd(TIM5, ENABLE);  // TIM5 enable
}



void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)   // interrupt 발생시 캐치
	{
		if(led_cnt_tim2++ == 10)	// LED toggles every 1s
		{
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
			led_cnt_tim2 = 1;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //   interrupt flag clear
	}
}

void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   // interrupt 발생시 캐치
	{
		if(led_cnt_tim3++ == 20)  // LED toggles every 1s
		{
			GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
			led_cnt_tim3 = 1;             // increase led_cnt every 100ms
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //   interrupt flag clear
	}
}

void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)   // interrupt 발생시 캐치
	{
		if(led_cnt_tim4++ == 30)  // LED toggles every 1s
		{
			GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
			led_cnt_tim4 = 1;             // increase led_cnt every 100ms
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //   interrupt flag clear
	}
}

void TIM5_IRQHandler()
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)   // interrupt 발생시 캐치
	{
		if(led_cnt_tim5++ == 40)   // LED toggles every 1s
		{
			GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
			led_cnt_tim5 = 1;
		}
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //   interrupt flag clear
	}
}


