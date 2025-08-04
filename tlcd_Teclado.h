#ifndef TLCD_TECLADO_H
#define	TLCD_TECLADO_H

#include "project.h"


/*****************************************************************************/
//  DEFINICIONES
/*****************************************************************************/
#define     TLCD_TEC_1          0x01
#define     TLCD_TEC_2          0x02
#define     TLCD_TEC_3          0x03
#define     TLCD_TEC_4          0x04
#define     TLCD_TEC_5          0x05
#define     TLCD_TEC_6          0x06
#define     TLCD_TEC_7          0x07
#define     TLCD_TEC_8          0x08
#define     TLCD_TEC_9          0x09
#define     TLCD_TEC_0          0x00
#define     TLCD_TEC_P          0x10
#define     TLCD_TEC_F          0x11
#define     TLCD_TEC_MODO       0x12
#define     TLCD_TEC_ZONA       0x13
#define     TLCD_TEC_PANICO     0x14
#define     TLCD_TEC_INCENDIO   0x15
#define     TLCD_TEC_CRUZ       0x16
#define     TLCD_TEC_ARRIBA     0x17
#define     TLCD_TEC_DERECHA    0x18
#define     TLCD_TEC_IZQUIERDA  0x19
#define     TLCD_TEC_ABAJO      0x20
#define     TLCD_TEC_MENU       0x21
#define     TLCD_TEC_ACEPTAR    0x22


/*****************************************************************************/
//  PROTOTIPOS
/*****************************************************************************/
void tlcd_tec_init (void);
void tlcd_tec_handler (void);
u8 tlcd_tec_isPressed (void);
u8 tlcd_tec_isPressed_2segs (void);
u8 tlcd_tec_getTecla (void);

#endif	/* TLCD_TECLADO_H */

