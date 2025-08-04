#include "eeprom.h"


void eeprom_wr (u8 addr, u8 data, u8 check)
{
    u8 retry = 5;

    do
    {
        EEPGD = 0;      // Opera sobre la EEPROM

        EEADRL = addr;
        EEDATL = data;

        WREN = 1;
        EECON2 = 0x55;
        EECON2 = 0xaa;

        WR = 1;
        while (WR);         // Se clerea por hardware cuando termina la operación

        retry --;

        if (check)
        {
            if (retry)
            {
                // Se lee el dato grabado
                EEDATL = 0x00;
                RD = 1;

                if (EEDATL == data)
                {
                    break;
                }
            }
            else
            {
                // No hay más intentos
                break;
            }
        }
        else
        {
            break;
        }
    }while (1);
}

u8 eeprom_rd  (u8 addr)
{
    EEPGD = 0;      // Opera sobre la EEPROM

    EEADRL = addr;

    RD = 1;

    return EEDATL;
}
