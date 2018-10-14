#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h" // again, added because ST didn't put it here ?

char cBT;
volatile uint32_t led_cnt_tim2 = 1;

void TIM2_IRQHandler(void);
void TIM2_Init(void);

void GPIO_Init_LED(void);
void Init_USART2_BT(void);
void USART2_IRQHandler(void);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);

int main(void)
{
	GPIO_Init_LED();
	Init_USART2_BT();
	TIM2_Init();

	GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	GPIO_SetBits(GPIOD, GPIO_Pin_12);

	while (1)
	{
		//		USART_SendData(USART2, 'a');
	}
}

void GPIO_Init_LED(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void Init_USART2_BT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// PD5 = TX, PD6 = RX (You have to cross the rx<->tx, tx<->rx with the bluetooth.)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_6 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	// AF mode
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = 57600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
}

/* USART interrupt (USART통신(블루투스)에 신호 들어올때 발생) */
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		cBT = USART_ReceiveData(USART2);
		USART_SendData(USART2, cBT);

		if(cBT == '1')		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		if(cBT == '2')		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		if(cBT == 'a')		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		if(cBT == 's')		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
	}
}

void USART_puts(USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		while( !(USARTx->SR & 0x00000040) );

		USART_SendData(USARTx, *s);
		*s++;
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

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)   // interrupt 발생시 캐치
	{
		if(led_cnt_tim2++ == 20)    // LED toggles every 2s
		{
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
			led_cnt_tim2 = 1;
			USART_puts(USART2, "HelloWorld!\r\n");
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //   interrupt flag clear
	}
}


