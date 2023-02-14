/*
 * LCD.c
 *
 * Created: 1/9/2023 9:32:04 PM
 *  Author: youstina
 */ 
#include "LCD.h"
#include "MCU.h"
#include <util/delay.h>
#include <stdio.h>
 
#if LCD_USE_TIMERS_FOR_DELAY == 1
#include "GPT.h"
#else
#include <util/delay.h>
#endif

void LCD_Init(void) {
	/* Initialize GPT Module for Delays */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Init(gaStrGPT_Config);
	#endif
	
	/* Initialize DIO Pins */
	DIO_SetPinDirection(LCD_CTRL_PORT, LCD_RS_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(LCD_CTRL_PORT, LCD_RW_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_OUTPUT);

	#if LCD_8BIT_DATA_LENGTH == 1
	/* 8-bit data mode */
	DIO_SetPortDirection(LCD_DATA_PORT, DIO_OUTPUT);
	#else
	/* 4-bit data mode */
	DIO_SetHighHalfPortDirection(LCD_DATA_PORT, DIO_OUTPUT);
	LCD_SendInstruction(LCD_RETURN_HOME);
	#endif
	/* Send Instruction to set function (data length, number of lines and font) */
	LCD_SendInstruction(LCD_FUNCTION_SET);
	/* Turn on display and clear it */
	LCD_SendInstruction(LCD_DISPLAY_ON);
	LCD_SendInstruction(LCD_CLEAR_DISPLAY);
}

void LCD_SendInstruction(uint8 Instruction) {
	/* Select Instruction Register */
	DIO_WritePin(LCD_CTRL_PORT, LCD_RS_PIN, LCD_INSTRUCTION_REG);
	/* Select Write Operation */
	DIO_WritePin(LCD_CTRL_PORT, LCD_RW_PIN, LCD_WRITE_MODE);
	/* Minimum delay for address setup is 60 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	/* Enable Pulse for Write Operation */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_HIGH);
	/* Minimum delay for Enable pulse is 450 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	#if LCD_8BIT_DATA_LENGTH == 1
	/* 8 bit mode */
	DIO_WritePort(LCD_DATA_PORT, Instruction);
	/* Minimum delay for data setup is 195 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	#else
	/* 4 bit mode */
	/* Write Higher Half of Instruction */
	DIO_WriteHighHalfPort(LCD_DATA_PORT, (Instruction & 0xF0U));
	/* Minimum delay for data setup is 195 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	/* bring down the enable pulse */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_LOW);
	/* Minimum delay for data hold is 10 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif

	/* Enable Pulse for Write Operation */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_HIGH);
	/* Minimum delay for Enable pulse is 450 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	/* Write Lower Half of Instruction */
	DIO_WriteHighHalfPort(LCD_DATA_PORT, ((Instruction & 0x0FU)<<4));
	/* Minimum delay for data setup is 195 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	#endif
	/* bring down the enable pulse */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_LOW);
	/* Minimum delay for data hold is 10 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
}

void LCD_SelectRowColumn(uint8 Row, uint8 Column) {
	uint8 u8Local_AddressInstruction = 0x00U;
	switch (Row) {
		case 0:
		u8Local_AddressInstruction = (LCD_DDRAM_PREFIX | LCD_1ST_LINE_ADDRESS_PREFIX | Column);
		break;
		case 1:
		u8Local_AddressInstruction = (LCD_DDRAM_PREFIX | LCD_2ND_LINE_ADDRESS_PREFIX | Column);
		break;
		case 2:
		u8Local_AddressInstruction = (LCD_DDRAM_PREFIX | LCD_3RD_LINE_ADDRESS_PREFIX | Column);
		break;
		case 3:
		u8Local_AddressInstruction = (LCD_DDRAM_PREFIX | LCD_4th_LINE_ADDRESS_PREFIX | Column);
		break;
		default: break;
	}
	LCD_SendInstruction(u8Local_AddressInstruction);
}

void LCD_DisplayCharacter(char Data) {
	/* Select Instruction Register */
	DIO_WritePin(LCD_CTRL_PORT, LCD_RS_PIN, LCD_DATA_REG);
	/* Select Write Operation */
	DIO_WritePin(LCD_CTRL_PORT, LCD_RW_PIN, LCD_WRITE_MODE);
	/* Minimum delay for address setup is 60 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	/* Enable Pulse for Write Operation */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_HIGH);
	/* Minimum delay for Enable pulse is 450 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	#if LCD_8BIT_DATA_LENGTH == 1
	/* 8 bit mode */
	DIO_WritePort(LCD_DATA_PORT, Data);
	/* Minimum delay for data setup is 195 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	#else
	/* 4 bit mode */
	/* Write Higher Half of Instruction */
	DIO_WriteHighHalfPort(LCD_DATA_PORT, (Data & 0xF0U));
	/* Minimum delay for data setup is 195 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	/* bring down the enable pulse */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_LOW);
	/* Minimum delay for data hold is 10 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif

	/* Enable Pulse for Write Operation */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_HIGH);
	/* Minimum delay for Enable pulse is 450 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	/* Write Lower Half of Instruction */
	DIO_WriteHighHalfPort(LCD_DATA_PORT, ((Data & 0x0FU)<<4));
	/* Minimum delay for data setup is 195 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
	#endif
	/* bring down the enable pulse */
	DIO_WritePin(LCD_CTRL_PORT, LCD_ENABLE_PIN, DIO_LOW);
	/* Minimum delay for data hold is 10 ns */
	#if LCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 1);
	#else
	_delay_us(1);
	#endif
}

void LCD_DisplayString(char* Data) {
	while (*Data != '\0') {
		LCD_DisplayCharacter(*Data);
		Data++;
	}
}

void LCD_DisplayInteger(sint16 Data) {
	if (Data < 0) {
		LCD_DisplayCharacter('-');
		Data = -1 * Data;
	}
	uint8 u8Local_Digit = 0U;
	char str_reversed[5] = "";
	char* str_ptr = str_reversed;
	do {
		u8Local_Digit = Data%10;
		*str_ptr = (char)(48 + u8Local_Digit); /* 48 is the ASCII code of '0' */
		Data = Data/10;
		str_ptr++;
	} while (Data > 0);
	do {
		str_ptr--;
		LCD_DisplayCharacter(*str_ptr);
	} while (str_ptr != str_reversed);
}

void LCD_DisplayFloatingPointNumber(float32 Data) {
	/* Will display only 3 Decimal Places */
	uint16 IntegerPart = (uint16)Data;
	LCD_DisplayInteger(IntegerPart);
	LCD_DisplayCharacter('.');
	float32 DecimalPart = (Data - IntegerPart);
	uint8 Digit = 0;
	for (uint8 i = 0; i < 3; i++) {
		Digit = (uint8)(DecimalPart * 10.0);
		LCD_DisplayCharacter((char)(48+Digit));
		DecimalPart = ((DecimalPart*10.0) - Digit);
	}
}

void LCD_CreateSpecialCharacter(char* Pattern, uint8 CGRAM_Location) {
	/* Set CGRAM address. CGRAM stores each character sequence in 8 bytes. */
	LCD_SendInstruction((LCD_CGRAM_PREFIX + (CGRAM_Location*8U)));
	for (uint8 i = 0; i < 8; i++) {
		LCD_DisplayCharacter(Pattern[i]);
	}
}

#if GLCD_USE_TIMERS_FOR_DELAY == 1
#include "GPT.h"
#else
#include <util/delay.h>
#endif

uint8 gu8_Curr_X_Address = 0;
uint8 gu8_Curr_Y_Address = 0;
uint8 gu8_Curr_Page = 0;

void GLCD_Init(void) {
	#if GLCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Init(gaStrGPT_Config);
	#endif
	/* Initialize Port Directions */
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_RS_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_RW_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_OUTPUT);
	DIO_SetPinDirection(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_OUTPUT);

	DIO_SetPortDirection(GLCD_DATA_PORT, DIO_OUTPUT);
	/* Keep reset pin inactive (high) */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_HIGH);
	/* Initialization of GLCD Sequence */
	GLCD_SelectPage(GLCD_BOTH_PAGES);
	GLCD_SendInstruction(GLCD_DISPLAY_OFF);
	GLCD_SendInstruction(GLCD_SET_X_ADDRESS_PREFIX + 0);  /* line=0 */
	gu8_Curr_X_Address = 0;
	GLCD_SendInstruction(GLCD_SET_Y_ADDRESS_PREFIX + 0);  /* column=0 */
	gu8_Curr_Y_Address = 0;
	GLCD_SendInstruction(GLCD_SET_Z_ADDRESS_PREFIX + 0);  /* start_line=0 */
	GLCD_SendInstruction(GLCD_DISPLAY_ON);
	GLCD_SelectPage(GLCD_PAGE_0);
	gu8_Curr_Page = 0;
}

void GLCD_SelectPage(GLCD_PageSelectType Page) {
	switch (Page) {
		case GLCD_NO_PAGES:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_LOW);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_LOW);
		break;
		case GLCD_PAGE_0:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_HIGH);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_LOW);
		break;
		case GLCD_PAGE_1:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_LOW);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_HIGH);
		break;
		case GLCD_BOTH_PAGES:
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_1, DIO_HIGH);
		DIO_WritePin(GLCD_CTRL_PORT, GLCD_PAGE_SELECT_PIN_2, DIO_HIGH);
		break;
		default: break;
	}
}

void GLCD_SendInstruction(GLCD_CommandType Instruction) {
	/* Select Instruction Register */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RS_PIN, GLCD_INSTRUCTION_REG);
	/* Select Write Operation */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RW_PIN, GLCD_WRITE_MODE);
	/* Send Command on Data Pins */
	DIO_WritePort(GLCD_DATA_PORT, Instruction);
	/* Enable Pulse of min. width 1us */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_HIGH);
	#if GLCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 5);
	#else
	_delay_us(5);
	#endif
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_LOW);
}

void GLCD_SendData(uint8 Data) {
	/* Select Data Register */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RS_PIN, GLCD_DATA_REG);
	/* Select Write Operation */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RW_PIN, GLCD_WRITE_MODE);
	/* Send Command on Data Pins */
	DIO_WritePort(GLCD_DATA_PORT, Data);
	/* Enable Pulse of min. width 1us */
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_HIGH);
	#if GLCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 5);
	#else
	_delay_us(5);
	#endif
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_ENABLE_PIN, DIO_LOW);
}

void GLCD_Reset(void) {
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_LOW);
	#if GLCD_USE_TIMERS_FOR_DELAY == 1
	GPT_Delay_us(&gaStrGPT_Config[0], 10);
	#else
	_delay_us(10);
	#endif
	DIO_WritePin(GLCD_CTRL_PORT, GLCD_RESET_PIN, DIO_HIGH);
}

void GLCD_DisplaySpecialPattern(uint64 Pattern) {
	for(uint8 i = 0; i < 5; i++) {
		GLCD_SendInstruction(GLCD_SET_X_ADDRESS_PREFIX + gu8_Curr_X_Address);
		GLCD_SendInstruction(GLCD_SET_Y_ADDRESS_PREFIX + gu8_Curr_Y_Address);
		GLCD_SendData((Pattern>>(i*8U)) & 0x00000000FFUL);
		gu8_Curr_Y_Address++;
		if (gu8_Curr_Y_Address >= GLCD_MAX_PAGE_PIXEL_WIDTH && gu8_Curr_Page == 0) {
			GLCD_SelectPage(GLCD_PAGE_1);
			gu8_Curr_Y_Address = 0;
		}
	}
}

void GLCD_DisplayCharacter(char Data) {
	if ((gu8_Curr_Page == 0) | (gu8_Curr_Y_Address < GLCD_MAX_PAGE_PIXEL_WIDTH - 7U)) {
		/* Enough space to write character */
		switch (Data) {
			case 'A': case 'a': GLCD_DisplaySpecialPattern(GLCD_CHAR_A); break;
			case 'F': case 'f': GLCD_DisplaySpecialPattern(GLCD_CHAR_F); break;
			case 'R': case 'r': GLCD_DisplaySpecialPattern(GLCD_CHAR_R); break;
			case 'E': case 'e': GLCD_DisplaySpecialPattern(GLCD_CHAR_E); break;
			case 'Q': case 'q': GLCD_DisplaySpecialPattern(GLCD_CHAR_Q); break;
			case 'U': case 'u': GLCD_DisplaySpecialPattern(GLCD_CHAR_U); break;
			case 'N': case 'n': GLCD_DisplaySpecialPattern(GLCD_CHAR_N); break;
			case 'C': case 'c': GLCD_DisplaySpecialPattern(GLCD_CHAR_C); break;
			case 'Y': case 'y': GLCD_DisplaySpecialPattern(GLCD_CHAR_Y); break;
			case 'D': case 'd': GLCD_DisplaySpecialPattern(GLCD_CHAR_D); break;
			case 'T': case 't': GLCD_DisplaySpecialPattern(GLCD_CHAR_T); break;
			case 'K': case 'k': GLCD_DisplaySpecialPattern(GLCD_CHAR_K); break;
			case 'H': case 'h': GLCD_DisplaySpecialPattern(GLCD_CHAR_H); break;
			case 'Z': case 'z': GLCD_DisplaySpecialPattern(GLCD_CHAR_Z); break;
			case 'M': case 'm': GLCD_DisplaySpecialPattern(GLCD_CHAR_M); break;
			case 'S': case 's': GLCD_DisplaySpecialPattern(GLCD_CHAR_S); break;
			case 'I': case 'i': GLCD_DisplaySpecialPattern(GLCD_CHAR_I); break;
			case '%': GLCD_DisplaySpecialPattern(GLCD_CHAR_PERCENT); break;
			case '=': GLCD_DisplaySpecialPattern(GLCD_CHAR_EQUAL); break;
			case '.': GLCD_DisplaySpecialPattern(GLCD_CHAR_POINT); break;
			case ' ': GLCD_DisplaySpecialPattern(GLCD_CHAR_SPACE); break;
			case '-': GLCD_DisplaySpecialPattern(GLCD_CHAR_NEGATIVE); break;
			case '0': GLCD_DisplaySpecialPattern(GLCD_CHAR_0); break;
			case '1': GLCD_DisplaySpecialPattern(GLCD_CHAR_1); break;
			case '2': GLCD_DisplaySpecialPattern(GLCD_CHAR_2); break;
			case '3': GLCD_DisplaySpecialPattern(GLCD_CHAR_3); break;
			case '4': GLCD_DisplaySpecialPattern(GLCD_CHAR_4); break;
			case '5': GLCD_DisplaySpecialPattern(GLCD_CHAR_5); break;
			case '6': GLCD_DisplaySpecialPattern(GLCD_CHAR_6); break;
			case '7': GLCD_DisplaySpecialPattern(GLCD_CHAR_7); break;
			case '8': GLCD_DisplaySpecialPattern(GLCD_CHAR_8); break;
			case '9': GLCD_DisplaySpecialPattern(GLCD_CHAR_9); break;
			default: break;
		}
		GLCD_SendInstruction(GLCD_SET_X_ADDRESS_PREFIX + gu8_Curr_X_Address);
		GLCD_SendInstruction(GLCD_SET_Y_ADDRESS_PREFIX + gu8_Curr_Y_Address);
		GLCD_SendData(0);
		gu8_Curr_Y_Address++;
		if (gu8_Curr_Y_Address >= GLCD_MAX_PAGE_PIXEL_WIDTH && gu8_Curr_Page == 0) {
			GLCD_SelectPage(GLCD_PAGE_1);
			gu8_Curr_Y_Address = 0;
		}
	}
}

void GLCD_DisplayString(char* Data_Ptr) {
	while (*Data_Ptr != '\0') {
		GLCD_DisplayCharacter(*Data_Ptr);
		Data_Ptr++;
	}
}

void GLCD_DisplayInteger(sint32 Data) {
	if (Data < 0) {
		GLCD_DisplayCharacter('-');
		Data = -1 * Data;
	}
	uint8 u8Local_Digit = 0U;
	char str_reversed[5] = "";
	char* str_ptr = str_reversed;
	do {
		u8Local_Digit = Data%10;
		*str_ptr = (char)(48 + u8Local_Digit); /* 48 is the ASCII code of '0' */
		Data = Data/10;
		str_ptr++;
	} while (Data > 0);
	do {
		str_ptr--;
		GLCD_DisplayCharacter(*str_ptr);
	} while (str_ptr != str_reversed);
}

void GLCD_DisplayFloatingPoint(float32 Data) {
	/* Will display only 3 Decimal Places */
	uint16 IntegerPart = (uint16)Data;
	GLCD_DisplayInteger(IntegerPart);
	GLCD_DisplayCharacter('.');
	float32 DecimalPart = (Data - IntegerPart);
	uint8 Digit = 0;
	for (uint8 i = 0; i < 3; i++) {
		Digit = (uint8)(DecimalPart * 10.0);
		GLCD_DisplayCharacter((char)(48+Digit));
		DecimalPart = ((DecimalPart*10.0) - Digit);
	}
}

void GLCD_GoToLine(uint8 Line) {
	if (Line < GLCD_MAX_LINES) {
		GLCD_SelectPage(GLCD_PAGE_0);
		gu8_Curr_X_Address = Line;
		GLCD_SendInstruction(GLCD_SET_X_ADDRESS_PREFIX + gu8_Curr_X_Address);
		gu8_Curr_Y_Address = 0;
		GLCD_SendInstruction(GLCD_SET_Y_ADDRESS_PREFIX + gu8_Curr_Y_Address);
	}
}

void GLCD_ClearDisplay(void) {
	for (uint8 Line = 0; Line < GLCD_MAX_LINES; Line++) {
		GLCD_GoToLine(Line);
		GLCD_SelectPage(GLCD_BOTH_PAGES);
		for (uint8 i = 0; i < GLCD_MAX_PAGE_PIXEL_WIDTH; i++) {
			GLCD_SendInstruction(GLCD_SET_Y_ADDRESS_PREFIX + i);
			GLCD_SendData(0);
		}
	}
	GLCD_Reset();
	GLCD_SelectPage(GLCD_PAGE_0);
	gu8_Curr_Page = 0;
	GLCD_GoToLine(0);
}
