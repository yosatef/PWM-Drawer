/*
 * PWM.h
 *  Author: youstina
 */ 
#ifndef PWM_H_
#define PWM_H_
#include "MCU.h"
#include "ICU.h"
#include "LCD.h"

#define NUM_OPERATING_PWM_CHANNELS			(1U) /* Input the number of operating Timers for PWM */
#define PWM_TIMER_0_RESOLUTION				(255U)
#define PWM_TIMER_1_RESOLUTION				(65535U)
#define PWM_TIMER_2_RESOLUTION				(255U)


#if NUM_OPERATING_PWM_CHANNELS > 4
#pragma GCC error "ATmega32A has only 3 timers with 4 PWM channels!! Fix NUM_OPERATING_PWM_CHANNELS."
#endif


#define PWM_TIMER_0_FAST_BIT			(3U)
#define PWM_TIMER_0_INVERTING_BIT		(4U)

#define PWM_TIMER_1_FAST_BIT			(3U)
#define PWM_TIMER_1_A_INVERTING_BIT		(6U)
#define PWM_TIMER_1_B_INVERTING_BIT		(4U)

#define PWM_TIMER_2_FAST_BIT			(3U)
#define PWM_TIMER_2_INVERTING_BIT		(4U)

/*					Data Types for PWM Module                           */
typedef enum {
	PWM_TIMER_0_CHANNEL,
	PWM_TIMER_1_CHANNEL_A,
	PWM_TIMER_1_CHANNEL_B,
	PWM_TIMER_2_CHANNEL
}PWM_ChannelType;

typedef enum {
	PWM_PHASE_CORRECT,
	PWM_FAST
}PWM_ModeType;

typedef enum {
	PWM_NO_CLK_SRC,
	PWM_PRESCALE_1,
	PWM_PRESCALE_8,
	PWM_PRESCALE_32,
	PWM_PRESCALE_64,
	PWM_PRESCALE_128,
	PWM_PRESCALE_256,
	PWM_PRESCALE_1024,
	PWM_EXT_CLK_FALLING_EDGE,
	PWM_EXT_CLK_RISING_EDGE
}PWM_ClockPrescaleType;

typedef enum {
	PWM_NON_INVERTED,
	PWM_INVERTED
}PWM_OutputStateType;

typedef struct {
	PWM_ChannelType Tmr_Channel;
	PWM_ModeType Mode;
	PWM_ClockPrescaleType Clk_Prescale;
	PWM_OutputStateType State;
	uint16 Prescale_Value;
	uint16 Period_ms;
	uint8 Duty_Percent;
}PWM_ConfigurationType;

/*				Function Prototypes for PWM Module                      */
void PWM_Init(const PWM_ConfigurationType* Config_Ptr);
void PWM_SetDuty(PWM_ChannelType Tmr_Channel, PWM_OutputStateType State, uint16 Duty_Percent);

/*				External Variables for PWM Module                       */
extern const PWM_ConfigurationType gaStrPWM_Config[NUM_OPERATING_PWM_CHANNELS];



#define GENERATE_PWM_FROM_SAME_MCU			(1U)	/* 1 if the same MCU is used to generate PWM signal and display it using GLCD, 0 otherwise */

#if GENERATE_PWM_FROM_SAME_MCU == 0
/* If the different MCUs is used to generate PWM signal and display it using LCD */
#define GLCD_MCU						(0U)		/* 1 if this is the MCU to display the signal using GLCD only, 0 if this is the MCU generating PWM only */
#endif


#define NUM_SPECIAL_CHARACTERS				(7U)
/*				Signal Patterns Characters (Font = 5 x 8)               */
#define PWM_DRAWER_GLCD_HIGH_LINE					(0x0202020202UL)
#define PWM_DRAWER_GLCD_LOW_LINE					(0x4040404040UL)
#define PWM_DRAWER_GLCD_RISING_EDGE					(0x02027E4040UL)
#define PWM_DRAWER_GLCD_FALLING_EDGE				(0x40407E0202UL)
#define PWM_DRAWER_GLCD_RIGHT_ARROW_HEAD			(0x003E081C2AUL)
#define PWM_DRAWER_GLCD_LEFT_ARROW_HEAD				(0x2A1C083E00UL)
#define PWM_DRAWER_GLCD_ARROW_MIDDLE_BODY			(0x0808080808UL)

/*				Data Types for PWM Drawer Application                   */

typedef struct {
	uint8 Duty_Percent;
	float32 High_Time_ms;
	float32 Low_Time_ms;
	float32 Period_Time_ms;
	float32 Freq_kHz;
}PWMDrawer_SignalAttributesType;

/*			Function Prototypes for PWM Drawer Application              */
void PWMDrawer_Init(void);
void PWMDrawer_MeasureSignal(PWMDrawer_SignalAttributesType* Signal_Data_Ptr);
void PWMDrawer_DrawSignal(PWMDrawer_SignalAttributesType* Signal_Data_Ptr, float32 Scale_ms);

#endif /* PWM_H_ */