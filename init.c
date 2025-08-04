#include "project.h"
#include "init.h"



void Inicializacion (void)
{
    Osc_Init ();
    GPIO_Init ();
    Timer0Init ();
}

void Osc_Init (void)
{
    OSCCON = 0b01110000;       // 8 MHz
}

void Timer0Init (void)
{
    OPTION_REG = 0x81;
    INTCONbits.GIE=1;                           // When IPEN = 0: 1 = Enables all unmasked interrupts
    INTCONbits.TMR0IE=1;
}

void GPIO_Init (void)
{
    ANSELA = 0x00;
    ANSELB = 0x00;

    // Clereo las salidas...
    // PortA
    PORTA &= ~(  SAL_ );

    // Configuro I/O...
    // PortA
    TRISA &= ~( SAL_  );


}
