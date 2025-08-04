#include "tlcd_Teclado.h"
#include "temporizadores.h"

/*****************************************************************************/
//  VARIABLES PRIVADAS
/*****************************************************************************/
static union {
   uint8_t bytes;
   struct {
        uint8_t teclap:1;
        uint8_t teclap_2segs:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
   } bits;
} flags1;

u8 tecApretada;
Temporizador t_Apretada;

/****************************************************************************/


/*****************************************************************************/
//  DEFINICIONES PRIVADAS
/*****************************************************************************/
#define     TEC_1          0
#define     TEC_2          1
#define     TEC_3          2
#define     TEC_4          4
#define     TEC_5          5
#define     TEC_6          6
#define     TEC_7          8
#define     TEC_8          9
#define     TEC_9          10
#define     TEC_0          13
#define     TEC_P          12
#define     TEC_F          14
#define     TEC_MODO       7
#define     TEC_ZONA       3
#define     TEC_PANICO     11
#define     TEC_INCENDIO   15
#define     TEC_CRUZ       22
#define     TEC_ARRIBA     16
#define     TEC_DERECHA    17
#define     TEC_IZQUIERDA  20
#define     TEC_ABAJO      21
#define     TEC_MENU       18
#define     TEC_ACEPTAR    19
#define     TEC_           23


/****************************************************************************/

void tlcd_tec_init (void)
{
    PORTB = 0x00;
    TRISB = 0b11000000;

    TRISC |= (TEC_COL_3_ | TEC_COL_4_);

    tecApretada = 0xFF;
    timer_Init(&t_Apretada, 2000);
}

void tlcd_tec_handler (void)
{
    u8 filas = 0x01;
    u8 tecOut = 0;
    u8 auxLATB;

    // Se confguran las entradas y salidas compartidas
    bit_set(TEC_COL_1_TRIS,TEC_COL_1);

    // Se guarda el LATB que había antes de entrar
    auxLATB = LATB;


    if (!flags1.bits.teclap)
    {
        // Barre todas las filas poniendo de a una en alto
        while (filas != 0x40)
        {
            LATB = filas;

            NOP();
            NOP();

            if (bit_test(TEC_COL_1_PORT,TEC_COL_1))
                break;

            tecOut++;

            if (bit_test(TEC_COL_2_PORT,TEC_COL_2))
                break;

            tecOut++;

            if (bit_test(TEC_COL_3_PORT,TEC_COL_3))
                break;

            tecOut++;

            if (bit_test(TEC_COL_4_PORT,TEC_COL_4))
                break;

            tecOut++;

            
            filas = filas << 1;
        }

        if (filas != 0x40)
        {
            // Hay ninguna tecla apretada
            flags1.bits.teclap = 1;
            tecApretada = tecOut;

            timer_Restart(&t_Apretada);
        }
    }
    else
    {
        // Ver si solto
        LATB = 0b00111111;

        if (bit_test(TEC_COL_1_PORT,TEC_COL_1) || bit_test(TEC_COL_2_PORT,TEC_COL_2) || bit_test(TEC_COL_3_PORT,TEC_COL_3) || bit_test(TEC_COL_4_PORT,TEC_COL_4))
        {
            if (timer_Expiro(t_Apretada))
            {
                // Se mantuvo la tecla apretada durante 2 segundos
                flags1.bits.teclap_2segs = 1;
            }
        }
        else
        {
            flags1.bits.teclap = 0;
            flags1.bits.teclap_2segs = 0;
        }
    }

    // Se recupera el LATB que había antes de entrar
    LATB = auxLATB;

    // Se dejan las entradas compartidas como salidas
    bit_clear(TEC_COL_1_TRIS,TEC_COL_1);
}


u8 tlcd_tec_isPressed (void)
{
    return flags1.bits.teclap;
}


u8 tlcd_tec_isPressed_2segs (void)
{
    return flags1.bits.teclap_2segs;
}


u8 tlcd_tec_getTecla (void)
{
    u8 ret = 0xFF;
    
    switch (tecApretada)
    {
        case TEC_1:
            ret = TLCD_TEC_1; break;

        case TEC_2:
            ret = TLCD_TEC_2; break;

        case TEC_3:
            ret = TLCD_TEC_3; break;

        case TEC_4:
            ret = TLCD_TEC_4; break;

        case TEC_5:
            ret = TLCD_TEC_5; break;

        case TEC_6:
            ret = TLCD_TEC_6; break;

        case TEC_7:
            ret = TLCD_TEC_7; break;

        case TEC_8:
            ret = TLCD_TEC_8; break;

        case TEC_9:
            ret = TLCD_TEC_9; break;

        case TEC_0:
            ret = TLCD_TEC_0; break;

        case TEC_P:
            ret = TLCD_TEC_P; break;

        case TEC_F:
            ret = TLCD_TEC_F; break;

        case TEC_PANICO:
            ret = TLCD_TEC_PANICO; break;

        case TEC_INCENDIO:
            ret = TLCD_TEC_INCENDIO; break;

        case TEC_CRUZ:
            ret = TLCD_TEC_CRUZ; break;

        case TEC_ZONA:
            ret = TLCD_TEC_ZONA; break;

        case TEC_MODO:
            ret = TLCD_TEC_MODO; break;

        case TEC_MENU:
            ret = TLCD_TEC_MENU; break;

        case TEC_ACEPTAR:
            ret = TLCD_TEC_ACEPTAR; break;

        case TEC_ARRIBA:
            ret = TLCD_TEC_ARRIBA; break;

        case TEC_ABAJO:
            ret = TLCD_TEC_ABAJO; break;

        case TEC_IZQUIERDA:
            ret = TLCD_TEC_IZQUIERDA; break;

        case TEC_DERECHA:
            ret = TLCD_TEC_DERECHA; break;

        default:
            ret = 0xFF; break;
    }

    return ret;
}