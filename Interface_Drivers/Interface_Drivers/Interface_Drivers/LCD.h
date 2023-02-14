/*
 * LCD.h
 *
 * Created: 1/9/2023 9:31:46 PM
 *  Author: youstina
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "DIO.h"
#include <stdio.h>

/*							LCD Characteristics                         */
#define LCD_NUM_LINES					(4U)
#define LCD_MAX_LINE_WIDTH				(20U)

/*							LCD Configurations                          */
#define LCD_8BIT_DATA_LENGTH			(0U)	/* 1 for 8 bit mode and 0 for 4 bit mode */
#define LCD_MAX_LINES					(1U)	/* 1 for 2/4 lines and 0 for 1/2 line(s) */
#define LCD_FONT_SIZE					(0U)	/* 1 for big font (5x10) and 0 for small font (5x8) */

#define LCD_CURSOR_MODE					(1U)	/* 1 for ON and 0 for OFF */
#define LCD_CURSOR_BLINK_MODE			(1U)	/* 1 for Blinking Cursor and 0 to turn off blinking */

#define LCD_USE_TIMERS_FOR_DELAY		(0U)	/* 1 to use GPT Module for delays and 0 to use delay.h */

/*							LCD Connections                             */
#define LCD_CTRL_PORT					(DIO_PORT_B)
#define LCD_RS_PIN						(DIO_PIN_1)
#define LCD_RW_PIN						(DIO_PIN_2)
#define LCD_ENABLE_PIN					(DIO_PIN_0)

#define LCD_DATA_PORT					(DIO_PORT_A)
#define LCD_DATA_D4_PIN					(DIO_PIN_4)
#define LCD_DATA_D5_PIN					(DIO_PIN_5)
#define LCD_DATA_D6_PIN					(DIO_PIN_6)
#define LCD_DATA_D7_PIN					(DIO_PIN_7)

/*				Some Compiler Checks on Configurations                  */
#if LCD_MAX_LINES == 1 && LCD_FONT_SIZE == 1
#pragma GCC error "You can NOT use big font (5x10) in the 2-line mode!!"
#endif

#define LCD_DDRAM_PREFIX				(0x80U)
#define LCD_CGRAM_PREFIX				(0x40U)
#define LCD_1ST_LINE_ADDRESS_PREFIX		(0x00U)
#define LCD_2ND_LINE_ADDRESS_PREFIX		(0x40U)
#define LCD_3RD_LINE_ADDRESS_PREFIX		(0x14U)
#define LCD_4th_LINE_ADDRESS_PREFIX		(0x54U)

/*				Data Types for LCD Module                               */
typedef enum {
	LCD_CLEAR_DISPLAY			= (0x01U),
	LCD_RETURN_HOME				= (0x02U),
	LCD_DISPLAY_OFF				= (0x08U),
	LCD_DISPLAY_ON				= ((0x0CU) | (LCD_CURSOR_MODE<<1) | (LCD_CURSOR_BLINK_MODE)),
	LCD_DISPLAY_SHIFT_RIGHT		= (0x1CU),
	LCD_DISPLAY_SHIFT_LEFT		= (0x18U),
	LCD_CURSOR_SHIFT_RIGHT		= (0x14U),
	LCD_CURSOR_SHIFT_LEFT		= (0x10U),
	LCD_FUNCTION_SET			= ((0x20U) | (LCD_8BIT_DATA_LENGTH<<4) | (LCD_MAX_LINES<<3) | (LCD_FONT_SIZE<<2))
}LCD_CommandType;

typedef enum {
	LCD_INSTRUCTION_REG,
	LCD_DATA_REG
}LCD_RegisterSelectType;

typedef enum {
	LCD_WRITE_MODE,
	LCD_READ_MODE
}LCD_ReadWriteType;

/*				Function Prototypes for LCD Module                      */
void LCD_Init(void);
void LCD_SendInstruction(uint8 Instruction);
void LCD_SelectRowColumn(uint8 Row, uint8 Column);
void LCD_DisplayCharacter(char Data);
void LCD_DisplayString(char* Data);
void LCD_DisplayInteger(sint16 Data);
void LCD_DisplayFloatingPointNumber(float32 Data);
void LCD_CreateSpecialCharacter(char* Pattern, uint8 CGRAM_Location);

/*					Graphical LCD Characteristics                       */
#define GLCD_MAX_LINES							(8U)
#define GLCD_MAX_LINE_PIXEL_WIDTH				(128U)
#define GLCD_MAX_PAGE_PIXEL_WIDTH				(64U)

/*					Graphical LCD Configurations                        */
#define GLCD_USE_TIMERS_FOR_DELAY				(0U)	/* 1 to use GPT for delay, 0 to use delay.h */

/*					Graphical LCD Connections                           */
#define GLCD_CTRL_PORT							(DIO_PORT_B)
#define GLCD_ENABLE_PIN							(DIO_PIN_0)
#define GLCD_RW_PIN								(DIO_PIN_1)
#define GLCD_RS_PIN								(DIO_PIN_2)
#define GLCD_PAGE_SELECT_PIN_1					(DIO_PIN_4)
#define GLCD_PAGE_SELECT_PIN_2					(DIO_PIN_5)
#define GLCD_RESET_PIN							(DIO_PIN_6)


#define GLCD_DATA_PORT							(DIO_PORT_A)
#define GLCD_DATA_D0_PIN						(DIO_PIN_0)
#define GLCD_DATA_D1_PIN						(DIO_PIN_1)
#define GLCD_DATA_D2_PIN						(DIO_PIN_2)
#define GLCD_DATA_D3_PIN						(DIO_PIN_3)
#define GLCD_DATA_D4_PIN						(DIO_PIN_4)
#define GLCD_DATA_D5_PIN						(DIO_PIN_5)
#define GLCD_DATA_D6_PIN						(DIO_PIN_6)
#define GLCD_DATA_D7_PIN						(DIO_PIN_7)

/*				Alphanumeric Characters (Font = 5 x 8)                  */
#define GLCD_CHAR_A						(0x7C1211127CUL)
#define GLCD_CHAR_F						(0x010909097FUL)
#define GLCD_CHAR_R						(0x4F2919097FUL)
#define GLCD_CHAR_E						(0x414949497FUL)
#define GLCD_CHAR_Q						(0x403E31211EUL)
#define GLCD_CHAR_U						(0x3F4040403FUL)
#define GLCD_CHAR_N						(0x7F201C027FUL)
#define GLCD_CHAR_C						(0x414141413EUL)
#define GLCD_CHAR_Y						(0x030C700C03UL)

#define GLCD_CHAR_D						(0x003E41417FUL)
#define GLCD_CHAR_T						(0x01017F0101UL)

#define GLCD_CHAR_I						(0x41417F4141UL)

#define GLCD_CHAR_K						(0x412214087FUL)
#define GLCD_CHAR_H						(0x7F0808087FUL)
#define GLCD_CHAR_Z						(0x4345495161UL)
#define GLCD_CHAR_M						(0x7F020C027FUL)
#define GLCD_CHAR_S						(0xF94949494FUL)

#define GLCD_CHAR_1						(0x40407F4244UL)
#define GLCD_CHAR_2						(0x404E516146UL)
#define GLCD_CHAR_3						(0x3649494941UL)
#define GLCD_CHAR_4						(0x107F121418UL)
#define GLCD_CHAR_5						(0x215149494FUL)
#define GLCD_CHAR_6						(0x3149494A7CUL)
#define GLCD_CHAR_7						(0x0F19294901UL)
#define GLCD_CHAR_8						(0x7749494977UL)
#define GLCD_CHAR_9						(0x7F4949494FUL)
#define GLCD_CHAR_0						(0x3E4549513EUL)

/*					Symbolic Characters (Font = 5 x 8)                  */
#define GLCD_CHAR_PERCENT				(0xC3CE1873C3UL)
#define GLCD_CHAR_EQUAL					(0x2424242424UL)
#define GLCD_CHAR_SPACE					(0x0000000000UL)
#define GLCD_CHAR_POINT					(0x0070707000UL)
#define GLCD_CHAR_NEGATIVE				(0x1818181818UL)

/*				Data Types for Graphical LCD Module                     */
typedef enum {
	GLCD_DISPLAY_OFF				= (0x3EU),
	GLCD_DISPLAY_ON					= (0x3FU),
	GLCD_SET_Y_ADDRESS_PREFIX		= (0x40U),
	GLCD_SET_X_ADDRESS_PREFIX		= (0xB8U),
	GLCD_SET_Z_ADDRESS_PREFIX		= (0xC0U)
}GLCD_CommandType;

typedef enum {
	GLCD_INSTRUCTION_REG,
	GLCD_DATA_REG
}GLCD_RegisterSelectType;

typedef enum {
	GLCD_NO_PAGES,
	GLCD_PAGE_0,
	GLCD_PAGE_1,
	GLCD_BOTH_PAGES
}GLCD_PageSelectType;

typedef enum {
	GLCD_WRITE_MODE,
	GLCD_READ_MODE
}GLCD_ReadWriteType;

/*			Function Prototypes for Graphical LCD Module                */
void GLCD_Init(void);
void GLCD_SelectPage(GLCD_PageSelectType Page);
void GLCD_SendInstruction(GLCD_CommandType Instruction);
void GLCD_SendData(uint8 Data);
void GLCD_Reset(void);
void GLCD_DisplayCharacter(char Data);
void GLCD_DisplayString(char* Data_Ptr);
void GLCD_DisplayInteger(sint32 Data);
void GLCD_DisplayFloatingPoint(float32 Data);
void GLCD_ClearDisplay(void);
void GLCD_GoToLine(uint8 Line);
void GLCD_DisplaySpecialPattern(uint64 Pattern);

#endif /* LCD_H_ */