/*
 * PWM.c
 *  Author: El-Badr Store
 */ 
#include "PWM.h"
#include "Bit_Math.h"

uint16 gu16_PWM_TimerTopValue = 0U;

/*				Private Functions Prototypes                            */
void PWM_InitTimer0(const PWM_ConfigurationType* Config_Ptr);
void PWM_InitTimer1(const PWM_ConfigurationType* Config_Ptr);
void PWM_InitTimer2(const PWM_ConfigurationType* Config_Ptr);

/*				Functions Implementations                               */
void PWM_Init(const PWM_ConfigurationType* Config_Ptr) {
	switch (Config_Ptr->Tmr_Channel) {
		case PWM_TIMER_0_CHANNEL: PWM_InitTimer0(Config_Ptr); break;
		case PWM_TIMER_1_CHANNEL_A:
		case PWM_TIMER_1_CHANNEL_B: PWM_InitTimer1(Config_Ptr); break;
		case PWM_TIMER_2_CHANNEL: PWM_InitTimer2(Config_Ptr); break;
		default: break;
	}
}

void PWM_SetDuty(PWM_ChannelType Tmr_Channel, PWM_OutputStateType State, uint16 Duty_Percent) {
	switch (Tmr_Channel) {
		case PWM_TIMER_0_CHANNEL:
		switch (State) {
			case PWM_NON_INVERTED: *TIMER0_CMP_REG = (uint8)(Duty_Percent * PWM_TIMER_0_RESOLUTION / 100.0); break;
			case PWM_INVERTED: *TIMER0_CMP_REG = (uint8)((100.0 - Duty_Percent) * PWM_TIMER_0_RESOLUTION / 100.0); break;
			default: break;
		}
		break;
		case PWM_TIMER_1_CHANNEL_A:
		switch (State) {
			case PWM_NON_INVERTED: *(uint16*)TIMER1_CMP_A_REG_L = (uint16)(Duty_Percent * gu16_PWM_TimerTopValue / 100.0); break;
			case PWM_INVERTED: *(uint16*)TIMER1_CMP_A_REG_L = (uint16)((100.0 - Duty_Percent) * gu16_PWM_TimerTopValue / 100.0); break;
			default: break;
		}
		break;
		case PWM_TIMER_1_CHANNEL_B:
		switch (State) {
			case PWM_NON_INVERTED: *(uint16*)TIMER1_CMP_B_REG_L = (uint16)(Duty_Percent * gu16_PWM_TimerTopValue / 100.0); break;
			case PWM_INVERTED: *(uint16*)TIMER1_CMP_B_REG_L = (uint16)((100.0 - Duty_Percent) * gu16_PWM_TimerTopValue / 100.0); break;
			default: break;
		}
		break;
		case PWM_TIMER_2_CHANNEL:
		switch (State) {
			case PWM_NON_INVERTED: *TIMER2_CMP_REG = (uint8)(Duty_Percent * PWM_TIMER_2_RESOLUTION / 100.0); break;
			case PWM_INVERTED: *TIMER2_CMP_REG = (uint8)((100.0 - Duty_Percent) * PWM_TIMER_2_RESOLUTION / 100.0); break;
			default: break;
		}
		break;
		default: break;
	}
}

/*				Private Functions Implementations                       */
void PWM_InitTimer0(const PWM_ConfigurationType* Config_Ptr) {
	/* Make PB3=OC0 as output */
	// DIO_SetPinDirection(DIO_PORT_B, DIO_PIN_3, DIO_OUTPUT);
	SET_BIT(*PORTB_DIR_REG, 3U);
	/* Clear TCCR0 Register Configurations */
	*TIMER0_CTRL_REG = 0x00U;
	/* Set PWM Mode */
	*TIMER0_CTRL_REG |= (1U<<6U) | ((Config_Ptr->Mode)<<PWM_TIMER_0_FAST_BIT);
	/* Set PWM Output State (inverting or not) */
	*TIMER0_CTRL_REG |= (1U<<5U) | ((Config_Ptr->State)<<PWM_TIMER_0_INVERTING_BIT);
	/* Select Clock Source and Prescaler */
	switch (Config_Ptr->Clk_Prescale) {
		case PWM_NO_CLK_SRC:
		case PWM_PRESCALE_1:
		case PWM_PRESCALE_8:
		*TIMER0_CTRL_REG |= (Config_Ptr->Clk_Prescale); break;
		case PWM_PRESCALE_64:
		*TIMER0_CTRL_REG |= ((Config_Ptr->Clk_Prescale)-1U); break;
		case PWM_PRESCALE_256:
		case PWM_PRESCALE_1024:
		case PWM_EXT_CLK_FALLING_EDGE:
		case PWM_EXT_CLK_RISING_EDGE:
		*TIMER0_CTRL_REG |= ((Config_Ptr->Clk_Prescale)-2U); break;
		default: break;
	}
	/* Set Duty Value in OCR0 Register */
	*TIMER0_CMP_REG = 0x00U;
	switch (Config_Ptr->State) {
		case PWM_NON_INVERTED: *TIMER0_CMP_REG = (uint8)((Config_Ptr->Duty_Percent) * PWM_TIMER_0_RESOLUTION / 100.0); break;
		case PWM_INVERTED: *TIMER0_CMP_REG = (uint8)((100.0 - Config_Ptr->Duty_Percent) * PWM_TIMER_0_RESOLUTION / 100.0); break;
		default: break;
	}
	/* Clear TCNT0 Register Configurations */
	*TIMER0_CNTR_REG = 0x00U;
}

void PWM_InitTimer1(const PWM_ConfigurationType* Config_Ptr) {
	/* Make PD5=OC1A or PD4=OC1B as output */
	switch (Config_Ptr->Tmr_Channel) {
		case PWM_TIMER_1_CHANNEL_A:
		//DIO_SetPinDirection(DIO_PORT_D, DIO_PIN_5, DIO_OUTPUT);
		SET_BIT(*PORTD_DIR_REG, 5U);
		break;
		case PWM_TIMER_1_CHANNEL_B:
		//DIO_SetPinDirection(DIO_PORT_D, DIO_PIN_4, DIO_OUTPUT);
		SET_BIT(*PORTD_DIR_REG, 4U);
		break;
		default: break;
	}
	/* Clear TCCR1A and TCCR1B Register Configurations */
	*TIMER1_CTRL_REG_A = 0x00U;
	*TIMER1_CTRL_REG_B = 0x00U;
	/* Set PWM Mode */
	*TIMER1_CTRL_REG_A |= (0x02U);
	*TIMER1_CTRL_REG_B |= (1U<<4U) | ((Config_Ptr->Mode)<<PWM_TIMER_1_FAST_BIT);
	/* Set Output State and Channel OC1A or OC1B */
	switch (Config_Ptr->Tmr_Channel) {
		case PWM_TIMER_1_CHANNEL_A:
		*TIMER1_CTRL_REG_A |= (1U<<7U) | ((Config_Ptr->State)<<PWM_TIMER_1_A_INVERTING_BIT);
		case PWM_TIMER_1_CHANNEL_B:
		*TIMER1_CTRL_REG_A |= (1U<<5U) | ((Config_Ptr->State)<<PWM_TIMER_1_B_INVERTING_BIT);
		break;
		default: break;
	}
	/* Select Clock Source and Prescaler */
	switch (Config_Ptr->Clk_Prescale) {
		case PWM_NO_CLK_SRC:
		case PWM_PRESCALE_1:
		case PWM_PRESCALE_8:
		*TIMER1_CTRL_REG_B |= (Config_Ptr->Clk_Prescale); break;
		case PWM_PRESCALE_64:
		*TIMER1_CTRL_REG_B |= ((Config_Ptr->Clk_Prescale)-1U); break;
		case PWM_PRESCALE_256:
		case PWM_PRESCALE_1024:
		case PWM_EXT_CLK_FALLING_EDGE:
		case PWM_EXT_CLK_RISING_EDGE:
		*TIMER1_CTRL_REG_B |= ((Config_Ptr->Clk_Prescale)-2U); break;
		default: break;
	}
	/* Set Period through TOP Value in ICR1 Register */
	gu16_PWM_TimerTopValue = (uint16)((F_CPU * (Config_Ptr->Period_ms) / ((Config_Ptr->Prescale_Value) * 1000.0)) - 1U);
	*(uint16*)TIMER1_INP_CAPT_REG_L = gu16_PWM_TimerTopValue;
	/* Set Duty Value in OCR1A or OCR1B Register */
	switch (Config_Ptr->Tmr_Channel) {
		case PWM_TIMER_1_CHANNEL_A:
		switch (Config_Ptr->State) {
			case PWM_NON_INVERTED: *(uint16*)TIMER1_CMP_A_REG_L = (uint16)((Config_Ptr->Duty_Percent) * gu16_PWM_TimerTopValue / 100.0); break;
			case PWM_INVERTED: *(uint16*)TIMER1_CMP_A_REG_L = (uint16)((100.0 - Config_Ptr->Duty_Percent) * gu16_PWM_TimerTopValue / 100.0); break;
			default: break;
		}
		break;
		case PWM_TIMER_1_CHANNEL_B:
		switch (Config_Ptr->State) {
			case PWM_NON_INVERTED: *(uint16*)TIMER1_CMP_B_REG_L = (uint16)((Config_Ptr->Duty_Percent) * gu16_PWM_TimerTopValue / 100.0); break;
			case PWM_INVERTED: *(uint16*)TIMER1_CMP_B_REG_L = (uint16)((100.0 - Config_Ptr->Duty_Percent) * gu16_PWM_TimerTopValue / 100.0); break;
			default: break;
		}
		break;
		default: break;
	}
	/* Clear TCNT1 Register Configuration */
	*(uint16*)TIMER1_CNTR_REG_L = 0x0000U;
}

void PWM_InitTimer2(const PWM_ConfigurationType* Config_Ptr) {
	/* Make PD7=OC2 as output */
	// DIO_SetPinDirection(DIO_PORT_D, DIO_PIN_7, DIO_OUTPUT);
	SET_BIT(*PORTD_DIR_REG, 7U);
	/* Clear TCCR2 Register Configurations */
	*TIMER2_CTRL_REG = 0x00U;
	/* Set PWM Mode */
	*TIMER2_CTRL_REG |= (1U<<6U) | ((Config_Ptr->Mode)<<PWM_TIMER_2_FAST_BIT);
	/* Set PWM Output State (inverting or not) */
	*TIMER0_CTRL_REG |= (1U<<5U) | ((Config_Ptr->State)<<PWM_TIMER_2_INVERTING_BIT);
	/* Select Clock Source and Prescaler */
	if ((Config_Ptr->Clk_Prescale) != PWM_EXT_CLK_FALLING_EDGE && (Config_Ptr->Clk_Prescale) != PWM_EXT_CLK_RISING_EDGE) {
		*TIMER2_CTRL_REG |= (Config_Ptr->Clk_Prescale);
	}
	/* Set Duty Value in OCR2 Register */
	*TIMER2_CMP_REG = 0x00U;
	switch (Config_Ptr->State) {
		case PWM_NON_INVERTED: *TIMER2_CMP_REG = (uint8)((Config_Ptr->Duty_Percent) * PWM_TIMER_2_RESOLUTION / 100.0); break;
		case PWM_INVERTED: *TIMER2_CMP_REG = (uint8)((100.0 - Config_Ptr->Duty_Percent) * PWM_TIMER_2_RESOLUTION / 100.0); break;
		default: break;
	}
	/* Clear TCNT2 Register Configurations */
	*TIMER2_CNTR_REG = 0x00U;
}


const PWM_ConfigurationType gaStrPWM_Config[NUM_OPERATING_PWM_CHANNELS] =
{
	{
		PWM_TIMER_0_CHANNEL,
		PWM_FAST,
		PWM_PRESCALE_8,
		PWM_INVERTED,
		256U,
		4U,
		25U
	}
};



char gac_Line_Patterns[NUM_SPECIAL_CHARACTERS][8] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00},
	{0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1C, 0x00},
	{0x1C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07, 0x00},
	{0x01, 0x11, 0x09, 0x1D, 0x09, 0x11, 0x00, 0x00},
	{0x10, 0x11, 0x12, 0x17, 0x12, 0x11, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00}
};

void PWMDrawer_Init(void) {
	#if GENERATE_PWM_FROM_SAME_MCU == 1
	GLCD_Init();
	GLCD_ClearDisplay();
	PWM_Init(&gaStrPWM_Config[0]);		/* Timer 0 on OC0=PB3 */
	ICU_Init(&gStrICU_Config);			/* Timer 1 is used for Input Capture */
	#elif GLCD_MCU == 1
	GLCD_Init();
	GLCD_ClearDisplay();
	ICU_Init(&gStrICU_Config);			/* Timer 1 is used for Input Capture */
	#else
	PWM_Init(&gaStrPWM_Config[0]);		/* Timer 0 on OC0=PB3 */
	#endif
}

void PWMDrawer_MeasureSignal(PWMDrawer_SignalAttributesType* Signal_Data_Ptr) {
	ICU_DutyCycleType strLocal_Duty = {0, 0};
	ICU_GetDutyCycle(ICU_TIMER_1, 8U, &strLocal_Duty);
	Signal_Data_Ptr->Period_Time_ms = strLocal_Duty.Period_Time;
	Signal_Data_Ptr->High_Time_ms = strLocal_Duty.High_Time;
	Signal_Data_Ptr->Low_Time_ms = strLocal_Duty.Period_Time - strLocal_Duty.High_Time;
	
	Signal_Data_Ptr->Freq_kHz = (1.0 / (Signal_Data_Ptr->Period_Time_ms));
	Signal_Data_Ptr->Duty_Percent = (uint8)((Signal_Data_Ptr->High_Time_ms) * 100.0 / (Signal_Data_Ptr->Period_Time_ms));
}

void PWMDrawer_DrawSignal(PWMDrawer_SignalAttributesType* Signal_Data_Ptr, float32 Scale_ms) {
	GLCD_ClearDisplay();
	/* Display Signal Data (Frequency and Duty) Values */
	GLCD_GoToLine(0);
	char txt1[15] = "Frequency=\0";
	GLCD_DisplayString(txt1);
	GLCD_DisplayFloatingPoint(Signal_Data_Ptr->Freq_kHz);
	char txt2[5] = " kHz\0";
	GLCD_DisplayString(txt2);

	GLCD_GoToLine(1);
	char txt3[15] = "Duty=\0";
	GLCD_DisplayString(txt3);
	GLCD_DisplayInteger(Signal_Data_Ptr->Duty_Percent);
	char txt4[5] = " %\0";
	GLCD_DisplayString(txt4);

	/* Display Signal Shape */
	GLCD_GoToLine(6);
	uint8 pixel_width_count = 0;
	uint8 i;
	while (pixel_width_count < GLCD_MAX_LINE_PIXEL_WIDTH - 8U) {
		GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_RISING_EDGE); pixel_width_count += 5;
		for (i = 0; i < ((uint8)(Signal_Data_Ptr->High_Time_ms / Scale_ms)); i++) {
			GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_HIGH_LINE); pixel_width_count += 5;
		}
		GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_FALLING_EDGE); pixel_width_count += 5;
		for (i = 0; i < ((uint8)(Signal_Data_Ptr->Low_Time_ms / Scale_ms)); i++) {
			GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_LOW_LINE); pixel_width_count += 5;
		}
	}

	/* Display Arrow for Time Period */
	GLCD_GoToLine(5);
	GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_LEFT_ARROW_HEAD);
	for (i = 0; i < ((uint8)(Signal_Data_Ptr->High_Time_ms / Scale_ms)); i++) {
		GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_ARROW_MIDDLE_BODY);
	}
	GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_ARROW_MIDDLE_BODY);
	for (i = 0; i < ((uint8)(Signal_Data_Ptr->Low_Time_ms / Scale_ms)); i++) {
		GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_ARROW_MIDDLE_BODY);
	}
	GLCD_DisplaySpecialPattern(PWM_DRAWER_GLCD_RIGHT_ARROW_HEAD);

	/* Display Time Period Value */
	GLCD_GoToLine(4);
	char txt5[15] = " Time=\0";
	GLCD_DisplayString(txt5);
	GLCD_DisplayFloatingPoint(Signal_Data_Ptr->Period_Time_ms);
	char txt6[5] = " ms\0";
	GLCD_DisplayString(txt6);
}

