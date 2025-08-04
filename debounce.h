#ifndef DEBOUNCE_H
#define	DEBOUNCE_H

#include "project.h"

/*****************************************************************************/
//  DEFINICIONES
/*****************************************************************************/
#define         GPIO_ALTO_ACTIVO            1
#define         GPIO_BAJO_ACTIVO            0

#define         GPIO_SIN_FLANCO             0
#define         GPIO_FLANCO_A_ACTIVO        1
#define         GPIO_FLANCO_A_PASIVO        2

typedef struct
{
   u8* port;
   u8 nivelActivo;     // GPIO_ALTO_ACTIVO, GPIO_BAJO_ACTIVO
   u8 pin;             // Numero de 1 a 8
   u8 estado;          // BAJO = 0 y ALTO = 1
   u8 cont;            // acumulador
   u8 flanco;          // Cuando se produce un flanco
} gpio_t;


/*****************************************************************************/
//  PROTOTIPOS
/*****************************************************************************/

/*****************************************************************************/
//  debounce_Init
/*****************************************************************************
 *  Descripci�n: funci�n que inicializa la estrucura gpio con los valores
 *               pasados.
 *
 *  Entradas: gpio - puntero a la estructura que describe el pin.
 *            nivelActivo - nivel l�gico en el que se considera activado el pin.
 *                          Se deben usar las definiciones: GPIO_ALTO_ACTIVO,
 *                          GPIO_BAJO_ACTIVO.
 *            puerto - puntero al puerto donde se encuentra el pin.
 *            pin - n�mero de pin al que se le quiere aplicar el anti-rebote.
 *
 *  Salidas:
 *
 *****************************************************************************/
void    debounce_Init       ( gpio_t* gpio, u8 nivelActivo, u8* puerto, u8 pin );


/*****************************************************************************/
//  debounce_Handler
/*****************************************************************************
 *  Descripci�n: actualiza la l�gica de debounce para el gpio indicado. Debe ser
 *               llamado peri�dicamente (preferentemente en la vuelta principal).
 *
 *  Entradas: gpio - puntero a la estructura que describe el pin.
 *
 *  Salidas:
 *
 *****************************************************************************/
void    debounce_Handler    ( gpio_t* gpio);


/*****************************************************************************/
//  debounce_getEstado
/*****************************************************************************
 *  Descripci�n: retorna el estado actual del pin, luego de aplicar el anti-
 *               rebote.
 *
 *  Entradas: gpio - puntero a la estructura que describe el pin.
 *
 *  Salidas: 0 - el pin no est� activo.
 *           1 - el pin est� activo.
 *
 *****************************************************************************/
u8      debounce_getEstado  ( gpio_t* gpio);


/*****************************************************************************/
//  debounce_getFlanco
/*****************************************************************************
 *  Descripci�n: indica si se produjo un flanco creciente, decreciente o no se
 *               produjo ning�n flanco.
 *
 *  Entradas: gpio - puntero a la estructura que describe el pin.
 *
 *  Salidas: retorna una de los siguiente valores: GPIO_SIN_FLANCO,
 *           GPIO_FLANCO_A_ACTIVO, GPIO_FLANCO_A_PASIVO.
 *
 *****************************************************************************/
u8      debounce_getFlanco  ( gpio_t* gpio);

#endif	/* DEBOUNCE_H */

