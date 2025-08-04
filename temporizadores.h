#ifndef TEMPORIZADOR_H_
#define TEMPORIZADOR_H_

#include "project.h"

// **************************************************
// Temporizadores
typedef struct
{
	u16 start;
	u16 intervalo;
        u8 state;
}Temporizador;

void    timer_Init      (Temporizador* timer, u16 intervalo);
void    timer_Restart   (Temporizador* timer);
u16     timer_Expiro    (Temporizador timer);
void    timer_Stop      (Temporizador* timer);

void    timer_Handler   (void);
u16     timer_getTicks  (void);
// **************************************************


#endif
