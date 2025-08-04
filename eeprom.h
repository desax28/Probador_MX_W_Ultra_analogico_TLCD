#ifndef EEPROM_H
#define	EEPROM_H

#include "project.h"


/*****************************************************************************/
//  PROTOTIPOS
/*****************************************************************************/

/*****************************************************************************/
//  eeprom_wr
/*****************************************************************************
 *  Descripción: escribe la posición addr de la EEPROM con data.
 *
 *  Entradas: addr - dirección de la EEPROM a escribir
 *            data - dato a escribir.
 *            check - si es 1 se lee lo escrito para comprobar que esté bien.
 *
 *  Salidas:
 *
 *****************************************************************************/
void eeprom_wr (u8 addr, u8 data, u8 check);


/*****************************************************************************/
//  eeprom_rd
/*****************************************************************************
 *  Descripción: lee la posición addr de la EEPROM.
 *
 *  Entradas: addr - posición de la EEPROM a leer.
 *
 *  Salidas: dato leido.
 *
 *****************************************************************************/
u8 eeprom_rd  (u8 addr);



#endif	/* EEPROM_H */

