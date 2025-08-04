#include "project.h"
#include "mpxh.h"


/*****************************************************************************/
//  ESTRUCTURAS PRIVADAS
/*****************************************************************************/
typedef union {
   uint8_t byte;
   struct {
        uint8_t send_9:1;
        uint8_t send_12:1;
        uint8_t send_15:1;
        uint8_t send_16:1;
        uint8_t send_17:1;
        uint8_t send_4:1;
        uint8_t send_8:1;
        uint8_t sending:1;
   } bits;
} _sendActivo;

typedef union {
   uint8_t byte;
   struct {
        uint8_t received_9:1;
        uint8_t received_12:1;
        uint8_t received_15:1;
        uint8_t received_16:1;
        uint8_t received_17:1;
        uint8_t received_4:1;
        uint8_t received_8:1;
        uint8_t receiving:1;
   } bits;
} _received;

typedef union {
   uint8_t byte;
   struct {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t lbitent:1;
        uint8_t bitent:1;
        uint8_t bit7:1;
   } bits;
} _regent;

typedef union {
        uint8_t bytes[2];
        uint16_t word;
} _auxwSend;

typedef union {
        uint8_t bytes[2];
        uint16_t word;
} _wRecAux;

typedef union {
   uint8_t byte;
   struct {
        uint8_t mpxError:1;
        uint8_t es_mpxc:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
   } bits;
} _mpxFlags;

const volatile u8 tablaHamMpx [18] =      {0,                       //entre 0 y 16 (17 entradas)
                                           0b10000000,
                                           0b10001111,
                                           0b10001110,
                                           0b10001101,
                                           0b10001011,
                                           0b10000111,
                                           0b10001100,
                                           0b10001010,
                                           0b10001001,
                                           0b10000110,
                                           0b10000101,
                                           0b10000011,
                                           0b10001000,
                                           0b10000100,
                                           0b10000010,
                                           0b10000001,
                                           0};               // 17 (nbr=18, no puede subir más)



// static para que solo la vea este .c
// volatile porque la usa la ISR y para que no la optimice el compi
static volatile u8 tbSend = 0;
static volatile u8 tbr0 = 0;
static volatile u8 tbr1 = 0;
static volatile u8 wSendL = 0;
static volatile u8 wSendH = 0;
static volatile _auxwSend auxwSend = {.bytes = 0};

static volatile u8 nbs = 0;
static volatile u8 nSent = 0;
static volatile u8 nBits = 0;
static volatile u8 tMistake = 0;
static volatile u8 basurita = 0;
static volatile _sendActivo sendActivo = {.byte = 0};
static volatile _received received = {.byte = 0};
static volatile _regent regent = {.byte = 0};

static volatile u8 nbr = 0;
static volatile u8 regHamR = 0;
static volatile u8 wRecL = 0;
static volatile u8 wRecH = 0;
static volatile _wRecAux wRecAux = {.bytes = 0};

static volatile u8 Taux = 0;
static volatile _mpxFlags mpxFlags = {.byte = 0};

static volatile u8 t_alto = 0;
static volatile u8 t_bajo = 0;


static void mpxh_txFallido (void);
static void mpxh_verMistake (void);
static void mpxh_armaHamm (u8 esParidad);
static void mpxh_subirSalidaMPX (void);
static void mpxh_bajarSalidaMPX (void);


void mpxh_Analizar (void)
{
    // uint8_t test;

    tbSend++;

// muestras:
    regent.byte = regent.byte >> 1;
    bit_clear(regent.byte,7);

    ( input(ENT_PORT, ENT) ) ? (regent.bits.bit7 = 1) : (regent.bits.bit7 = 0);

// corregir:

    if ( (regent.byte & 0b11100000) == 0b10100000)
        bit_set(regent.byte, 6);
    else if ( ( regent.byte & 0b11100000 ) == 0b01000000)
        bit_clear(regent.byte, 6);


// tbr1inc:
    if ( regent.bits.bitent )
    {
        if ( tbr1 < 255 )
        {
            tbr1++;
        }
        mpxFlags.bits.mpxError = 0;
    }
    else
    {
// tbr0inc:
        tbr0++;
    }
// fintbr1inc:

// trasmitir
    if (!sendActivo.bits.sending)
    {


        if ( tbr1 < MI_PRIORIDAD )
        {
            mpxh_verMistake();
            goto fintransmi;

        }
        if ( !sendActivo.byte ) // entra si es 0
        {
            mpxh_verMistake();

            goto fintransmi;
        }
        if ( !regent.bits.bit7 )
        {
            mpxh_verMistake();

            goto fintransmi;
        }

            // nbs = 32 - nBits;
        if ( sendActivo.bits.send_17 )
        {
            nbs = 15;
        }
        else if ( sendActivo.bits.send_16 )
        {
            nbs = 16;
        }
        else if ( sendActivo.bits.send_15 )
        {
            nbs = 17;
        }
        else if ( sendActivo.bits.send_12 )
        {
            nbs = 20;
        }
        else if ( sendActivo.bits.send_9 )
        {
            nbs = 23;
        }
        else if ( sendActivo.bits.send_4 )
        {
            nbs = 28;
        }
        else if ( sendActivo.bits.send_8 )
        {
            nbs = 24;
        }

        sendActivo.bits.sending = 1;
        auxwSend.bytes[1] = wSendH;
        auxwSend.bytes[0] = wSendL;
        if ((sendActivo.bits.send_9 == 1) || (sendActivo.bits.send_12 == 1) || (sendActivo.bits.send_8 == 1))
        {
            tbSend = 2*T9;
        }
        else
        {
            tbSend = 2*T;
        }

    }
// sen1:
    if ( bit_test( tMistake,6 ) )   // no hubo error
    {
        if((regent.bits.bit7 == 0) && (input(SAL_PORT, SAL) == 0) )   // si tengo 00 entro
        {
            if (basurita == 0)
            {
                basurita = 1;

            }
            else
            {
                mpxh_txFallido();
                mpxh_verMistake();
                goto fintransmi;
            }

        }
        else        // si la combinacion es distinto de 00
        {
// sen8:
            basurita = 0;

        }
    }
    else    // error de mistake
    {
        mpxh_txFallido();
        mpxh_verMistake();
        goto fintransmi;
    }

// send81:
    if ( bit_test( nbs,5 ))
    {
        if ( tbSend < 6*MSEG )
        {
            goto fintransmi;
        }
        // tbSend >= 6*MSEG
        sendActivo.byte = 0;
        mpxh_verMistake();
        goto fintransmi;
    }

// sen4:
    if ((sendActivo.bits.send_9 == 1) || (sendActivo.bits.send_12 == 1) || (sendActivo.bits.send_8 == 1))
    {
        Taux = T9;
    }
    else
    {
        Taux = T;
    }

    if ( tbSend == Taux )
    {
        if ( !bit_test( auxwSend.bytes[1],7 ) )        // pregunto antes por el carry si es 0(asi no lo pierdo)
        {
            output_high(SAL_PORT, SAL);
            //output_high(SAL_PORT, SAL);
        }
        auxwSend.word = auxwSend.word << 1;
    }
// sen5:
    if ((sendActivo.bits.send_9 == 1) || (sendActivo.bits.send_12 == 1) || (sendActivo.bits.send_8 == 1))
    {
        Taux = 2*T9;
    }
    else
    {
        Taux = 2*T;
    }

    if ( tbSend == Taux )
    {
        output_high(SAL_PORT, SAL);
    }
// sen3:
    if ((sendActivo.bits.send_9 == 1) || (sendActivo.bits.send_12 == 1) || (sendActivo.bits.send_8 == 1))
    {
        Taux = 3*T9;
    }
    else
    {
        Taux = 3*T;
    }
    if ( tbSend == Taux )
    {
        output_low(SAL_PORT, SAL);
        nbs++;
        tbSend = 0;
    }
fintransmi:

// recibir:
    if ( received.bits.receiving == 0 )
    {
        if ( regent.bits.bitent == 0)
        {
            if( tbr1 >= 6*MSEG )
            {
                received.bits.receiving = ALTO;
                regHamR = 0;
                nbr = 0;
                tbr0 = 0;
            }


            tbr1 = 0;
        }
    }
    else
    {
// rec20:
        if ( tbr0 >= 4*MSEG )       // Pulso largo en bajo (error)
        {
            tbr1 = 0;
            received.bits.receiving = BAJO;

            mpxFlags.bits.mpxError = 1;
        }
        else
        {
// rec2:
            if ( tbr1 < 4*MSEG )
            {
                // Pregunto si vino flanco positivo para chequear bit MPX)
                if ( regent.bits.bitent == ALTO && regent.bits.lbitent == BAJO )
                {
                    wRecAux.word = wRecAux.word << 1;
                    if ( tbr1 > tbr0 )
                        bit_set( wRecAux.bytes[0], 0);  // Vino un 1

                    else
                        bit_clear( wRecAux.bytes[0], 0);    // Vino un 0

                    if ( bit_test(wRecAux.bytes[0],0) )
                        regHamR ^= tablaHamMpx[nbr];

                    nbr++;
                    tbr1 = 0;
                    tbr0 = 0;

                    if ( nbr == 17 )
                    {
                        if ( regHamR != 0 )
                        {
                            tMistake = 0;
                            received.bits.receiving = BAJO;
                        }

                    }
                    else if ( nbr > 17 )
                    {
                        tMistake = 0;
                        received.bits.receiving = BAJO;
                    }

                }

            }
            else    // tbr1 >= 4*MSEG
            {
                if ( sendActivo.bits.sending )
                {
                    received.bits.receiving = BAJO;                 // no me recibo a mi mismo
                }
                else
                {
                    if ( nbr == 16 )
                    {
                        if ( (regHamR & 0x80) == 0 )  // si no es cero me tengo q ir
                        {
                            if ( tbr1 >= 5*MSEG )
                            {
                                wRecAux.word = wRecAux.word << 1;   // justifico a izquierda
                                bit_clear(wRecAux.bytes[1],7);      // clereo la paridad
                                received.bits.received_16 = ALTO;
                                wRecH = wRecAux.bytes[1];
                                wRecL = wRecAux.bytes[0];
                                received.bits.receiving = BAJO;
                            }
                        }
                        else
                        {
                            tMistake = 0;
                            received.bits.receiving = BAJO;
                        }

                    }
                    else // ( nbr != 16 )
                    {
    // rec31
    // rec32
                        switch (nbr)
                        {
                            case 4: received.bits.received_4 = ALTO; break;
                            case 8: received.bits.received_8 = ALTO; break;
                            case 9: received.bits.received_9 = ALTO; break;
                            case 12: received.bits.received_12 = ALTO; break;
                            case 15: received.bits.received_15 = ALTO; break;
                            case 17: received.bits.received_17 = ALTO;
                                     bit_clear (wRecAux.bytes[1],7);    // clereo bit paridad
                                     wRecAux.bytes[0] &= 0xF0;                // elimino Hamming
                                     break;
                        }

                        wRecH = wRecAux.bytes[1];
                        wRecL = wRecAux.bytes[0];

                        received.bits.receiving = BAJO;

                    }
                }
            }
        }
    }
}

static void mpxh_txFallido (void)
{
// sen10:
    nSent++;
    if ( bit_test(nSent,4))
    {
        sendActivo.byte = 0;
    }
    sendActivo.bits.sending = 0;

}

u8 mpxh_Ocupado (void)
{
    return( sendActivo.byte != 0 );
}

void mpxh_ArmaMensaje( u8 dataH, u8 dataL, u8 layer, u8 nbits )
{
    wSendH = dataH;
    wSendL = dataL;
    nSent = 0;
    /*switch (nbits)
    {
        case MPXH_BITS_17: sendActivo.bits.send_17 = 1;
                            break;

    }*/
    if ( nbits == MPXH_BITS_17)
    {
        mpxh_armaHamm( 0 );
    }
    else if (nbits == MPXH_BITS_16)
    {
        wSendL = wSendL & 0xF0;
        wSendL = wSendL | (layer << 1);

        mpxh_armaHamm( 1 );
    }
    else if (nbits == MPXH_BITS_8)
    {
        wSendH = wSendH << 1;
    }
    else if (nbits == MPXH_BITS_12)
    {
        asm("RRF    _wSendH,f");
        asm("RRF    _wSendL,f");

        asm("RRF    _wSendH,f");
        asm("RRF    _wSendL,f");

        asm("RRF    _wSendH,f");
        asm("RRF    _wSendL,f");

        wSendH = wSendH | (layer & 0x07) << 5;
    }
    else if (nbits == MPXH_BITS_15)
    {
        asm("LSLF    _wSendL,f");
        asm("RLF     _wSendH,f");

        asm("LSLF    _wSendL,f");
        asm("RLF     _wSendH,f");
    }

    bit_set(sendActivo.byte, nbits);
}

static void mpxh_verMistake (void)
{
// ver_mistake:
    if ( bit_test(tMistake,6) )     // entro si esta OK
    {

        if ( !sendActivo.bits.sending )
        {
            output_low(SAL_PORT, SAL);
        }
    }
// hay_mistake:
    else        // entro si hay mistake
    {
        output_high(SAL_PORT, SAL);
        tMistake++;
        if ( bit_test(tMistake,6) )
        {
            output_low(SAL_PORT, SAL);
        }
    }
// fin_mistake:
}

static void mpxh_armaHamm (u8 esParidad)
{
    register uint8_t wAux = 0;

    if (!esParidad)
        wSendL = wSendL & 0xF0;

    if ( bit_test( wSendH,6 ) )
        wAux ^= 0b10001111 ;
    if ( bit_test( wSendH,5 ) )
        wAux ^= 0b10001110 ;
    if ( bit_test( wSendH,4 ) )
        wAux ^= 0b10001101 ;
    if ( bit_test( wSendH,3 ) )
        wAux ^= 0b10001011 ;
    if ( bit_test( wSendH,2 ) )
        wAux ^= 0b10000111 ;
    if ( bit_test( wSendH,1 ) )
        wAux ^= 0b10001100 ;
    if ( bit_test( wSendH,0 ) )
        wAux ^= 0b10001010 ;
    if ( bit_test( wSendL,7 ) )
        wAux ^= 0b10001001 ;
    if ( bit_test( wSendL,6 ) )
        wAux ^= 0b10000110 ;
    if ( bit_test( wSendL,5 ) )
        wAux ^= 0b10000101 ;
    if ( bit_test( wSendL,4 ) )
        wAux ^= 0b10000011 ;

    if (!esParidad)
        wSendL = wSendL | ( wAux & 0x0F );
    
    wAux = wAux & 0x80;

    if ( bit_test( wSendL,3 ) )
        wAux ^= 0b10000000 ;
    if ( bit_test( wSendL,2 ) )
        wAux ^= 0b10000000 ;
    if ( bit_test( wSendL,1 ) )
        wAux ^= 0b10000000 ;
    if ( bit_test( wSendL,0 ) )
        wAux ^= 0b10000000 ;

    wSendH = wSendH | wAux ;

}

u8 mpxh_recibio ( u8 flag )
{
    return ( bit_test (received.byte, flag) );
}

void mpxh_clearRecFlag ( u8 flag )
{
    bit_clear ( received.byte, flag );
}

void mpxh_getRecibido ( u8 *dataH, u8 *dataL )
{
    *dataH = wRecH;
    *dataL = wRecL;

}

u8 mpxh_tiempoIdle ( u8 tiempo)
{
    return ( tbr1 > tiempo );
}

void mpxh_forceMPXHLow (void)
{
    output_high(SAL_PORT, SAL);
}

void mpxh_releaseMPXH (void)
{
    output_low(SAL_PORT, SAL);

    tbr1 = 0;
    tbr0 = 0;
}

void mpxh_abortTx (void)
{
    sendActivo.byte = 0x00;
}