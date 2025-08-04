#ifndef TLCD_LCD_H
#define	TLCD_LCD_H

#include "project.h"

/*****************************************************************************/
//  COMANDOS
/*****************************************************************************/
#define LCD_CLRD           0b00000001          // 1.64mseg requiere
#define LCD_HOME           0b00000010
#define LCD_LINEA1         0b10000000          // Posicion home
#define LCD_LINEA2         0b11000000          // Posicion 40h
#define LCD_SETD           0b00110000          // Pone dos lineas (y bus de 8bits, AMTES 38H)
#define LCD_SETCUR         0b00000110          // Hace que el cursor escriba y vaya a derecha
#define LCD_CURSORPARP     0b00001101          // Parpadea
#define LCD_CURSORINV      0b00001100          // Invisible
#define LCD_MOVERCURSOR    0b00010100  	// Desplaza el cursor a la derecha
#define LCD_DISPLAYOFF     0b00001000  	// Apaga todo el display
//#define LCD_DISPLAYON    0b00000001  	// Enciende el display en home
#define LCD_DISPLAYON      0b00001100  	// Enciende el display en home
#define LCD_OSC            0b00010100
#define LCD_POWER          0b01011100  	// Dos bits de contraste
#define LCD_FOLLOWER       0b01101101
#define LCD_CONTRASTE      0b01110000  	// Agregar los bits

/*****************************************************************************/
//  PROTOTIPOS
/*****************************************************************************/
void tlcd_lcd_init ();
void tlcd_lcd_send_data (u8 data);
void tlcd_lcd_send_cmd  (u8 cmd);

void tlcd_lcd_handler (void);

void tlcd_DRAM_clear (void);
void tlcd_DRAM_setChar (u8 pos, u8 data);
void tlcd_DRAM_setRAMString (u8 pos, u8* string);
void tlcd_DRAM_setROMString (u8 pos, const u8* string);
void tlcd_DRAM_setArray (u8 pos, u8* array, u8 len);

void tlcd_backlight_setLevel (u8 level);


/*****************************************************************************/
//  LETRAS TLCD
/*****************************************************************************/
#define		PER0		0x0		//
#define		PER1		0x1		//
#define		PER2		0x2		//
#define		PER3		0x3		//
#define		PER4		0x4		//
#define		PER5		0x5		//
#define		PER6		0x6		//
#define		PER7		0x7		//
#define		PER8		0x8		//
#define		PER9		0x9		//
#define		PERA		0x0a		// posición en que está en la tabla lcd
#define		PERB		0x0b		// el ascii de cada per o caracter
#define		PERC		0x0c		//
#define		PERD		0x0d		//
#define		PERE		0x0e		//
#define		PERF		0x0f		//
#define		PERG		0x10		//
#define		PERH		0x11		//
#define		PERI		0x12		//
#define		PERJ		0x13		//
#define		PERK		0x14		//
#define		PERL		0x15		//
#define		PERM		0x16		//
#define		PERN		0x17		//
#define		PERO		0x18		//
#define		PERP		0x19		//
#define		PERQ		0x1a		//
#define		PERR		0x1b		//
#define		PERS		0x1c		//
#define		PERT		0x1d		//
#define		PERU		0x1e		//
#define		PERV		0x1f		//
#define		PERW		0x20		//
#define		PERX		0x21		//
#define		PERY		0x22		//
#define		PERZ		0x23		//
#define		PERPDOSPUN	0x25		//
#define		PERPUNDER	0x26		// _ (underscore)
#define		PERPCUADRADO	0x27		// cuadrado
#define		PEREGNE		0x28		// Ñ
#define		PERPRAYA	0x29		// -
#define		PERPSLASH	0x2a		///
#define		PERBLANCO	0x2b		//
#define		PERCAMPANITA	0x2c		// campanita (custom)
#define		PERPAST		0x2d		// *
#define		PERCOMA		0x2e		// ,
#define		PERPUNTO	0x2f		// .
#define		PERPAREN	0x31		// (
#define		PERPAREN2	0x32		// )
#define		PERPUP		0x33		// flecha up
#define		PERPDOWN	0x34		// flecha down
#define		PERPAEXC	0x37		// ¡ todo con el set de caracteres DS0066F05
#define		PERPEXC		0x38		// !
#define		PERASUP		0x39		// ª a superindice
#define		PEROSUP		0x3a		// º o superindice
#define		PERPNUM		0x3c		// #
#define		PERPLEFT	0x3e		// left
#define		PERPRIGHT	0x3f		// right
#define		PER3RAYAS	0x60		//
#define		PER5RAYAS	0x61		//
#define		PERTICK		0x62		//
#define		PERARENA	0x63		//




#endif	/* TLCD_LCD_H */

