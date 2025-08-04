#include "tmp.h"


/*****************************************************************************/
//  DEFINICIONES
/*****************************************************************************/
#define T_TMP   6

#define VOLLO   63
#define VOLMLO  25
#define VOLMHI  23
#define VOLHI   20
/****************************************************************************/



/*****************************************************************************/
//  VARAIBLES PRIVADAS
/*****************************************************************************/
static volatile union {
   uint8_t bytes;
   struct {
        uint8_t send_sound:1;
        uint8_t sending_tmp:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
   } bits;
} flags1;

static volatile u8 auxsendh;
static volatile u8 sonido;
static volatile u8 nbs;
static volatile u8 tbsend;
static volatile u8 volume = 0x02;

static u8 tabla_volumen[] = {
    VOLLO | 0b01000000,     // f00
    VOLLO,                  // f01
    VOLMLO,                 // f02
    VOLMHI,                 // f03
    VOLHI                   // f04
};
/****************************************************************************/



void tmp_init (void)
{
    bit_set (TMP_RESET_PORT, TMP_RESET);
    bit_clear (TMP_LINE_PORT,TMP_LINE);

    bit_clear (TMP_LINE_TRIS, TMP_LINE);
    bit_clear (TMP_RESET_TRIS, TMP_RESET);

    tmp_setVolume (volume);

}


// Cada 250 microSeg
void tmp_handler (void)
{
    tbsend ++;
 
    if (flags1.bits.send_sound)
    {
        flags1.bits.send_sound = 0;

        auxsendh = sonido;
        flags1.bits.sending_tmp = 1;
        nbs = 7;

        bit_set(auxsendh, 6);       // start
        tbsend = -15 * T_TMP;       // 22.5 msegs en bajo antes de mandar la palabra
    }

    if (flags1.bits.sending_tmp)
    {
        if (tbsend == T_TMP)
        {
            if (bit_test(auxsendh,6) == 0)
            {
                output_high (TMP_LINE_PORT, TMP_LINE);
            }
        }
        else if (tbsend == 2*T_TMP)
        {
            output_high (TMP_LINE_PORT, TMP_LINE);
        }
        else if (tbsend == 3*T_TMP)
        {
            auxsendh = auxsendh << 1;
            tbsend = 0;

            output_low (TMP_LINE_PORT, TMP_LINE);

            nbs --;
            if (nbs == 0)
            {
                flags1.bits.sending_tmp = 0;
            }
        }
    }
}

u8 tmp_isSending (void)
{
    return (flags1.bits.send_sound == 1 ||flags1.bits.sending_tmp == 1);
}


void tmp_sendSound (u8 sound)
{
    sonido = sound;
    flags1.bits.send_sound = 1;
}


void tmp_setVolume (u8 vol)
{
    u8 auxVol;

    auxVol = vol & 0x03;

    sonido = tabla_volumen[auxVol];

   flags1.bits.send_sound = 1;      // Se lo manda como un sonido
}

void tmp_reset (void)
{

}
