//**********************************************************************************
// Proyecto :   Fu3A - MPXH
// Micro:       PIC12F1840
//                              ----------
//                          VDD |1      8| GND
//             LED_ROJO <-- RA5 |2      7| RA0 -->  BAJA_FUENTE
//  ENT_BAT / LED_VERDE <-- RA4 |3      6| RA1 -->  ENT
//              ENT_RED <-- RA3 |4      5| RA2 -->  SAL
//                              ----------
//***********************************************************************************

#ifndef PROJECT_H
#define	PROJECT_H


#include "header_v1p0.h"
#include <xc.h>



// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF    // Clock Out Enable (CLKOUT function is enabled on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable (All VCAP pin functionality is disabled)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)



/****************************************************************************/
#define         VERSION                     01
#define         MSEG                        4           // unidad de tiempo de t0
#define         SEGUNDOS                    250
#define         MI_PRIORIDAD                20*MSEG     // unidad de tiempo de t0
#define         T                           5           // unidad de tiempo de tbSend
#define         T9                          3           // unidad de tiempo de tbSend
/****************************************************************************/



/****************************************************************************/
// Definiciones de HW

// PortA
#define         ENT_                            ( 1 << 0 )
#define         TMP_LINE_                       ( 1 << 1 )
#define         SAL_                            ( 1 << 3 )
#define         TMP_RESET_                      ( 1 << 5 )


// PortA
#define         ENT                             0
#define         ENT_PORT                        PORTA
#define         ENT_TRIS                        TRISA
#define         TMP_LINE                        1
#define         TMP_LINE_PORT                   LATA
#define         TMP_LINE_TRIS                   TRISA
#define         SAL                             3
#define         SAL_PORT                        LATA
#define         SAL_TRIS                        TRISA
#define         TMP_RESET                       5
#define         TMP_RESET_PORT                  LATA
#define         TMP_RESET_TRIS                  TRISA


// PortB
#define         TEC_FILA_0_                     ( 1 << 0 )
#define         TEC_FILA_1_                     ( 1 << 1 )
#define         TEC_FILA_2_                     ( 1 << 2 )
#define         TEC_FILA_3_                     ( 1 << 3 )
#define         TEC_FILA_4_                     ( 1 << 4 )
#define         TEC_FILA_5_                     ( 1 << 5 )
#define         TEC_COL_1_                      ( 1 << 6 )
#define         TEC_COL_2_                      ( 1 << 7 )


// PortB
#define         TEC_FILA_0                       0
#define         LED_ACT                          0
#define         TEC_FILA_0_PORT                  LATB
#define         LED_ACT_PORT                     LATB
#define         LED_ACT_TRIS                     TRISB

#define         TEC_FILA_1                       1
#define         LED_MEM                          1
#define         TEC_FILA_1_PORT                  LATB
#define         LED_MEM_PORT                     LATB
#define         LED_MEM_TRIS                     TRISB

#define         TEC_FILA_2                       2
#define         LED_LISTA                        2
#define         TEC_FILA_2_PORT                  LATB
#define         LED_LISTA_PORT                   LATB
#define         LED_LISTA_TRIS                   TRISB

#define         TEC_FILA_3                       3
#define         LED_ESTOY                        3
#define         TEC_FILA_3_PORT                  LATB
#define         LED_ESTOY_PORT                   LATB
#define         LED_ESTOY_TRIS                   TRISB

#define         TEC_FILA_4                       4
#define         LED_ME_VOY                       4
#define         TEC_FILA_4_PORT                  LATB
#define         LED_ME_VOY_PORT                  LATB
#define         LED_ME_VOY_TRIS                  TRISB

#define         TEC_FILA_5                       5
#define         LED_BAT                          5
#define         TEC_FILA_5_PORT                  LATB
#define         LED_BAT_PORT                     LATB
#define         LED_BAT_TRIS                     TRISB

#define         TEC_COL_1                        6
#define         LED_RED                          6
#define         TEC_COL_1_PORT                   PORTB
#define         TEC_COL_1_TRIS                   TRISB
#define         LED_RED_PORT                     LATB
#define         LED_RED_TRIS                     TRISB

#define         TEC_COL_2                        7
#define         TEC_COL_2_PORT                   PORTB
#define         TEC_COL_2_TRIS                   TRISB



// PortC
#define         TEC_COL_3_                      ( 1 << 0 )
#define         TEC_COL_4_                      ( 1 << 1 )
#define         LCD_CMD_DATA_                   ( 1 << 2 )
#define         LCD_CLOCK_                      ( 1 << 3 )
#define         BACKLIGHT_                      ( 1 << 4 )
#define         LCD_DATA_                       ( 1 << 5 )
#define         LED_K_                          ( 1 << 6 )
#define         LCD_RESET_                      ( 1 << 7 )



// PortC
#define         TEC_COL_3                        0
#define         TEC_COL_3_PORT                   PORTC
#define         TEC_COL_3_TRIS                   TRISC
#define         TEC_COL_4                        1
#define         TEC_COL_4_PORT                   PORTC
#define         TEC_COL_4_TRIS                   TRISC
#define         LCD_CMD_DATA                     2
#define         LCD_CMD_DATA_PORT                LATC
#define         LCD_CMD_DATA_TRIS                TRISC
#define         LCD_CLOCK                        3
#define         LCD_CLOCK_PORT                   LATC
#define         LCD_CLOCK_TRIS                   TRISC
#define         BACKLIGHT                        4
#define         BACKLIGHT_PORT                   PORTC
#define         BACKLIGHT_TRIS                   TRISC
#define         LCD_DATA                         5
#define         LCD_DATA_PORT                    LATC
#define         LCD_DATA_TRIS                    TRISC
#define         LED_K                            6
#define         LED_K_PORT                       PORTC
#define         LED_K_TRIS                       TRISC
#define         LCD_RESET                        7
#define         LCD_RESET_PORT                   LATC
#define         LCD_RESET_TRIS                   TRISC


/****************************************************************************/



/****************************************************************************/
typedef union {
   uint8_t bytes;
   struct {
        uint8_t mandarTec:1;
        uint8_t mandarTec2:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
   } bits;
} _comandos1;


typedef union {
   uint8_t bytes;
   struct {
        uint8_t mandar_click:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
   } bits;
} _comandosTMP1;


typedef union {
   uint8_t bytes;
   struct {
        uint8_t soyFlechita:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
   } bits;
} _flags1;

typedef union {
   uint8_t bytes;
   struct {
        uint8_t mseg256:1;
        uint8_t mseg250:1;
        uint8_t hora1:1;
        uint8_t min1:1;
        uint8_t seg1:1;
        uint8_t mseg4:1;
        uint8_t mseg1:1;
        uint8_t useg250:1;
   } bits;
} _timers1;



typedef union {
   uint8_t bytes;
   struct {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t ap_pp:1;
        uint8_t ap_probador:1;
        uint8_t ap_downTLCD:1;
        uint8_t ap_upTLCD:1;
        uint8_t ap_progAvanzada:1;
        uint8_t ap_tecla:1;
   } bits;
} _ap1;
/****************************************************************************/

#endif	/* PROJECT_H */

