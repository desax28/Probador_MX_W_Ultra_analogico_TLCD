#include "temporizadores.h"

u16 systemTicks = 0;

#define STATE_RUNNING       0
#define STATE_STOPPED       1

// **************************************************
// Contador global del sistema
void timer_Handler(void)
{
	systemTicks ++;
}
// **************************************************


u16 timer_getTicks (void)
{
	return systemTicks;
}

u16 timer_Expiro (Temporizador timer)
{
    if (timer.state == STATE_RUNNING)
    {
        if (systemTicks >= timer.start)
            return ((systemTicks - timer.start) > timer.intervalo);
        else
            return (((65535 - timer.start) + systemTicks) > timer.intervalo);
    }
    else
    {
        return 0;
    }
}

void timer_Stop (Temporizador* timer)
{
    timer->state = STATE_STOPPED;
}

void timer_Init (Temporizador* timer, u16 intervalo)
{
    timer->state = STATE_RUNNING;
    timer->start = timer_getTicks();
    timer->intervalo = intervalo;
}

void timer_Restart (Temporizador* timer)
{
    timer->state = STATE_RUNNING;
    timer->start = timer_getTicks();
}
