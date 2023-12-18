#include "stm32f4xx_hal.h";
#include "timer14_driver.h";

static void oc_led_init(){
	//OC OUTPUT ==> GPIO-7 (AF47)

	// Enable Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Select Mode : Alternate Function
	// BIT 14:0
	// BIT 15:1
	GPIOA->MODER &= ~(1 << 14); // BIT 14 : 0
	GPIOA->MODER |= (1 << 15); // BIT 15 : 1

	// Select Output Type
	// Bit 7 : 0
	GPIOA->OTYPER &= ~(1<<7);

	// Select Speed : Medium
	// Bit 14: 1
	// Bit 15: 0
	GPIOA->OSPEEDR |= (1<<14); // Bit 14: 1
	GPIOA->OSPEEDR &= ~(1<<15); // Bit 15: 0

	// No pull-up, no pull-down
	// Bit 14: 0
	// Bit 15: 0
	GPIOA->PUPDR &= ~(1 << 14);
	GPIOA->PUPDR &= ~(1 << 15);

	// Alternate Function: GPIOA = 7 = AF9
	// AFRL_LOW
	// Bit 28: 1
	// Bit 29: 0
	// Bit 30: 0
	// Bit 31: 1
	GPIOA->AFR[0] |= (1<<28);
	GPIOA->AFR[0] &= ~(1<<29);
	GPIOA->AFR[0] &= ~(1<<30);
	GPIOA->AFR[0] |= (1<<31);
}

void timer14_init(void){
	oc_led_init();
	__HAL_RCC_TIM14_CLK_ENABLE();

	// Timer clock = 42mhz / 420000 = 1000 Hz (1ms period)
	TIM14->PSC = 42000-1;

	// Output Compare Mode
	TIM14->CCMR1 &= ~TIM_CCMR1_CC1S_0;
	TIM14->CCMR1 &= ~TIM_CCMR1_CC1S_1;

	// Output Compare Mode - Toggle Mod
	TIM14->CCMR1 |= TIM_CCMR1_OC1M_0;
	TIM14->CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM14->CCMR1 &= ~TIM_CCMR1_OC1M_2;

	// Enable OC1REF Output
	TIM14->CCER |= TIM_CCER_CC1E;

	// Output Polarity: Active High
	TIM14->CCER &= ~TIM_CCER_CC1P;

	// Set Period to 100ms
	TIM14->ARR = 99;
	TIM14->CCR1 = 99;

}

void timer14_enable(void){
	TIM14->CR1 |= TIM_CR1_CEN;
}

void timer14_disable(void){
	TIM14->CR1 &= ~TIM_CR1_CEN;
}

void timer14_capture_set_period(uint32_t ms){
	TIM14->CCR1 = ms - 1;
	TIM14->ARR = ms - 1;
}
