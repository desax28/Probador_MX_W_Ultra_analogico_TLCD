#include "debounce.h"



void debounce_Init ( gpio_t* gpio, u8 nivelActivo, u8* puerto, u8 pin )
{
    gpio->port = puerto;
    gpio->pin = pin;
    gpio->cont = 0;
    gpio->estado = BAJO;
    gpio->nivelActivo = nivelActivo;
}


void debounce_Handler (gpio_t* gpio)
{
    if ( bit_test(*(gpio->port), gpio->pin))
    {
        _inch4(gpio->cont);
    }
    else
    {
        _dech0(gpio->cont);
    }

    if ((gpio->cont) >= 4)
    {
        if ( gpio->nivelActivo == GPIO_ALTO_ACTIVO )
        {
            if (gpio->estado == BAJO)
                gpio->flanco = GPIO_FLANCO_A_ACTIVO;

            gpio->estado = ALTO;
        }
        else
        {
            if (gpio->estado == ALTO)
                gpio->flanco = GPIO_FLANCO_A_PASIVO;

            gpio->estado = BAJO;
        }
    }
    else if ((gpio->cont) == 0)
    {
        if ( gpio->nivelActivo == GPIO_ALTO_ACTIVO )
        {
            if (gpio->estado == ALTO)
                gpio->flanco = GPIO_FLANCO_A_PASIVO;

            gpio->estado = BAJO;
        }
        else
        {
            if (gpio->estado == BAJO)
                gpio->flanco = GPIO_FLANCO_A_ACTIVO;

            gpio->estado = ALTO;
        }
    }

}

u8 debounce_getEstado ( gpio_t* gpio)
{
    return ( gpio->estado );
}

u8 debounce_getFlanco (gpio_t* gpio)
{
    u8 aux_flanco;
    aux_flanco = gpio->flanco;
    gpio->flanco = GPIO_SIN_FLANCO;
    return (aux_flanco);
}