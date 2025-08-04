#include "project.h"
#include "mpxh.h"
#include "init.h"
#include "eeprom.h"
#include "tlcd_LCD.h"
#include "tlcd_Teclado.h"
#include "tmp.h"
#include "temporizadores.h"
#include "tlcd_leds.h"

/*****************************************************************************/
//  DEFINICIONES
/*****************************************************************************/
#define POS_SENSOR_1    2
#define POS_SENSOR_2    3
#define POS_SENSOR_3    7
#define POS_SENSOR_4    8
#define POS_SENSOR_5    12
#define POS_SENSOR_6    13
#define POS_SENSOR_7    18
#define POS_SENSOR_8    19
#define POS_SENSOR_9    23
#define POS_SENSOR_10   24


/*****************************************************************************/
//  PROTOTIPOS
/*****************************************************************************/
u8 sensorState2TLCD (u8 sensorState);


/*****************************************************************************/
//  VARIABLES GLOBALES
/*****************************************************************************/
volatile u8 t0;
volatile u8 t1;
volatile u8 t2;

volatile u8 dataL;
volatile u8 dataH;

volatile _ap1 ap1 = {.bytes = 0};

volatile _timers1 timers1;
volatile _flags1 flags1;
volatile _comandos1 comandos1;
volatile _comandosTMP1 comandosTMP1;



/*****************************************************************************/
//  DISPLAYS
/*****************************************************************************/
//                                          0             1516             31
//                                          |              ||              |
const u8 display_Inicio[] =                "PROB. LINEA MX52 PRESIONE START ";
const u8 display_Sensores[] =              "1:   2:   3:    4:   5:  ";
const u8 display_pirX_tpX[] =              "PROBANDO        OBSERVE MX  TP  ";
const u8 display_desalimentado[] =         "      CAMAS      DESALIMENTADAS";
const u8 display_corriente_x[] =           "    MIDIENDO    CORRIENTE  MX 1";

// agregados
const u8 display_Seleccion[] =             "SELECCION MODELO PRESIONE u O d ";
const u8 display_PresioneStart[] =         " PRESIONE START  PARA CONTINUAR "; 

const u8 display_MX42[] =                  "PROBADOR ANALOG.    MX41-42     ";
const u8 display_MX52[] =                  "PROBADOR ANALOG.    MX51-52     ";
const u8 display_MX52W[] =                 "PROBADOR ANALOG.    MX51-52W    ";
const u8 display_MVD97[] =                 "PROBADOR ANALOG.    MD-MVD97    ";
        
enum{
    __modeloXX = 0,
    __modeloSoy42,
    __modeloSoy52,
    __modeloSoy52W,
    __modeloSoy97
};

u8 modelo = __modeloXX;

/*****************************************************************************/
//  PROGRAMA PRINCIPAL
/*****************************************************************************/
void main(void)
{
    u8 auxTecla;
    u8 yaMandoTecla = 0;
    u8 auxSensor;
    u8 mostrandoDisplaySensores = 1;
    u8 mostrandoMidiendoCorriente = 0;
    u8 countBounceKey = 0;
    
    
    /*****************************************************************************/
    //  INICIALIZACIONES
    /*****************************************************************************/
    Inicializacion();
    tlcd_tec_init();
    tlcd_lcd_init();
    tmp_init();
    tlcd_leds_init();


    tlcd_DRAM_clear();
    tlcd_DRAM_setRAMString(0, display_Inicio);
    mostrandoDisplaySensores = 0;


    while(1)
    {
        if (timers1.bits.mseg1)
        {
            /*****************************************************************************/
            //  CADA 1 mseg
            /*****************************************************************************/
            timers1.bits.mseg1 = 0;

            tlcd_lcd_handler();
        }

        if ( timers1.bits.mseg4 )
        {
            /*****************************************************************************/
            //  CADA 4 mseg
            /*****************************************************************************/
            timers1.bits.mseg4 = BAJO;

            // output_high (DUTY_3_PORT, DUTY_3);


            /*****************************************************************************/
            //  RECEPCIÓN MPXH : 17 bits
            /*****************************************************************************/
            if ( mpxh_recibio ( MPXH_BITS_17 ) )
            {
                mpxh_getRecibido( &dataH, &dataL);
                mpxh_clearRecFlag( MPXH_BITS_17 );
                
                if (bit_test(dataH,6) == 0)
                {
                    // Se muestra el display del estado de los sensores si se estaba
                    // mostrando otro display
                    if (mostrandoDisplaySensores == 0)
                    {
                        tlcd_DRAM_clear();
                        tlcd_DRAM_setRAMString(0, display_Sensores);
                        mostrandoDisplaySensores = 1;
                    }
                        
                    
                    // Vienen los bits de los sensores 1, 2, 3, 4 y 5
                    auxSensor = (dataH >> 4) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_1, sensorState2TLCD(auxSensor));
                   
                    auxSensor = (dataH >> 2) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_2, sensorState2TLCD(auxSensor));
                    
                    auxSensor = dataH & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_3, sensorState2TLCD(auxSensor));
                    
                    auxSensor = (dataL >> 6) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_4, sensorState2TLCD(auxSensor));
                    
                    auxSensor = (dataL >> 4) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_5, sensorState2TLCD(auxSensor));
                }
                else 
                {
                    // Vienen los bits de los sensores 6, 7, 8, 9 y 10
                    auxSensor = (dataH >> 4) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_6, sensorState2TLCD(auxSensor));
                   
                    auxSensor = (dataH >> 2) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_7, sensorState2TLCD(auxSensor));
                    
                    auxSensor = dataH & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_8, sensorState2TLCD(auxSensor));
                    
                    auxSensor = (dataL >> 6) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_9, sensorState2TLCD(auxSensor));
                    
                    auxSensor = (dataL >> 4) & 0x03;
                    tlcd_DRAM_setChar(POS_SENSOR_10, sensorState2TLCD(auxSensor));
                }
            }


            /*****************************************************************************/
            //  RECEPCIÓN MPXH : 9 bits
            /*****************************************************************************/
            if ( mpxh_recibio ( MPXH_BITS_9 ) )
            {
                mpxh_getRecibido( &dataH, &dataL );
                mpxh_clearRecFlag( MPXH_BITS_9 );
                
                
                
                if (dataL == PER0)
                {
                    tlcd_DRAM_clear();
                    tlcd_DRAM_setRAMString(0, display_Inicio);
                    mostrandoDisplaySensores = 0;
                    mostrandoMidiendoCorriente = 0;
                }
                else if (dataL >= PER1 && dataL <= PER5)
                {                   
                    if (mostrandoMidiendoCorriente == 1)
                    {
                        tlcd_DRAM_clear();
                        tlcd_DRAM_setRAMString(0, display_corriente_x);

                        mostrandoDisplaySensores = 0;
                        
                        switch (dataL)
                        {
                            case PER1:
                                tlcd_DRAM_setChar(31, PER1);
                            break;

                            case PER2:
                                tlcd_DRAM_setChar(31, PER2);
                            break;

                            case PER3:
                                tlcd_DRAM_setChar(31, PER3);
                            break;                   

                            case PER4:
                                tlcd_DRAM_setChar(31, PER4);
                            break;

                            case PER5:
                                tlcd_DRAM_setChar(31, PER5);
                            break;
                        }
                    }
                    else
                    {
                        tlcd_DRAM_clear();
                        tlcd_DRAM_setRAMString(0, display_pirX_tpX);

                        mostrandoDisplaySensores = 0;
                        mostrandoMidiendoCorriente = 0;
                    
                        switch (modelo)
                        {
                            case __modeloSoy42:
                                tlcd_DRAM_setChar(9, PERM);
                                tlcd_DRAM_setChar(10, PERX);
                                tlcd_DRAM_setChar(11, PER4);
                                tlcd_DRAM_setChar(12, PER1);
                                tlcd_DRAM_setChar(13, PERPRAYA);
                                tlcd_DRAM_setChar(14, PER2);
                                tlcd_DRAM_setChar(15, PERBLANCO);
                                break;

                            case __modeloSoy52:
                                tlcd_DRAM_setChar(9, PERM);
                                tlcd_DRAM_setChar(10, PERX);
                                tlcd_DRAM_setChar(11, PER5);
                                tlcd_DRAM_setChar(12, PER1);
                                tlcd_DRAM_setChar(13, PERPRAYA);
                                tlcd_DRAM_setChar(14, PER2);
                                tlcd_DRAM_setChar(15, PERBLANCO);
                                break;


                            case __modeloSoy52W:
                                tlcd_DRAM_setChar(9, PERM);
                                tlcd_DRAM_setChar(10, PERX);
                                tlcd_DRAM_setChar(11, PER5);
                                tlcd_DRAM_setChar(12, PER1);
                                tlcd_DRAM_setChar(13, PERPRAYA);
                                tlcd_DRAM_setChar(14, PER2);
                                tlcd_DRAM_setChar(15, PERW);
                                break;


                            case __modeloSoy97:
                                tlcd_DRAM_setChar(9, PERM);
                                tlcd_DRAM_setChar(10, PERV);
                                tlcd_DRAM_setChar(11, PERD);
                                tlcd_DRAM_setChar(12, PER9);
                                tlcd_DRAM_setChar(13, PER7);
                                tlcd_DRAM_setChar(14, PERBLANCO);
                                tlcd_DRAM_setChar(15, PERBLANCO);
                                break;

                        }

                        switch (dataL)
                        {
                            case PER1:
                                tlcd_DRAM_setChar(26, PER1);
                                tlcd_DRAM_setChar(30, PER1);
                                tlcd_DRAM_setChar(31, PER2);
                            break;

                            case PER2:
                                tlcd_DRAM_setChar(26, PER2);
                                tlcd_DRAM_setChar(30, PER1);
                                tlcd_DRAM_setChar(31, PER2);
                            break;

                            case PER3:
                                tlcd_DRAM_setChar(26, PER3);
                                tlcd_DRAM_setChar(30, PER1);
                                tlcd_DRAM_setChar(31, PER2);
                            break;

                            case PER4:
                                tlcd_DRAM_setChar(26, PER4);
                                tlcd_DRAM_setChar(30, PER1);
                                tlcd_DRAM_setChar(31, PER2);
                            break;

                            case PER5:
                                tlcd_DRAM_setChar(26, PER5);
                                tlcd_DRAM_setChar(30, PER1);
                                tlcd_DRAM_setChar(31, PER2);
                            break;
                        }
                    }
                    
                }
                else if (dataL == PERM)
                {
                    tlcd_DRAM_clear();
                    tlcd_DRAM_setRAMString(0, display_desalimentado);
                    mostrandoDisplaySensores = 0;
                    mostrandoMidiendoCorriente = 0;
                }
                else if (dataL == PERN)
                {
                    tlcd_DRAM_clear();
                    tlcd_DRAM_setRAMString(0, display_corriente_x);
                    mostrandoDisplaySensores = 0;
                    mostrandoMidiendoCorriente = 1;
                }
                else if (dataL >= PERB && dataL <= PERG)
                {
                    tlcd_DRAM_clear();
                    mostrandoDisplaySensores = 0;
                    
                    switch (dataL)
                    {
                        case PERB:
                            tlcd_DRAM_setRAMString(0, display_Seleccion);
                        break;
                        
                        case PERC:
                            // tlcd_DRAM_setRAMString(0, display_PresioneStart);
                        break;
                        
                        case PERD:
                            tlcd_DRAM_setRAMString(0, display_MX42);
                            modelo = __modeloSoy42;
                        break;
                        
                        case PERE:
                            tlcd_DRAM_setRAMString(0, display_MX52);
                            modelo = __modeloSoy52;
                        break;
                        
                        case PERF:
                            tlcd_DRAM_setRAMString(0, display_MX52W);
                            modelo = __modeloSoy52W;
                        break;
                        
                        case PERG:
                            tlcd_DRAM_setRAMString(0, display_MVD97);
                            modelo = __modeloSoy97;
                        break;
                    }                                   
                }
            }

            /*****************************************************************************/
            //   GENERACIÓN EVENTOS AP
            /*****************************************************************************/

            if ( timers1.bits.seg1 )    // vencio timer de los segundos
            {
                /*****************************************************************************/
                //  CADA 1 seg
                /*****************************************************************************/
                timers1.bits.seg1 = BAJO;

            }

            /*****************************************************************************/
            //  ACCIONES
            /*****************************************************************************/
            if (tlcd_tec_isPressed() && yaMandoTecla == 0)
            {
                countBounceKey += 2;
                
                if (countBounceKey >= 32) 
                {
                    countBounceKey = 0;
                    yaMandoTecla = 1;

                    auxTecla = tlcd_tec_getTecla();
                    
                    if ( (auxTecla >= TLCD_TEC_0 && auxTecla <= TLCD_TEC_9) || (auxTecla == TLCD_TEC_PANICO) || (auxTecla == TLCD_TEC_INCENDIO) || (auxTecla == TLCD_TEC_MODO))
                    {
                        if (auxTecla == TLCD_TEC_PANICO)
                            auxTecla = 0x0e;
                        if (auxTecla == TLCD_TEC_INCENDIO)
                            auxTecla = 0x0f;
                        if (auxTecla == TLCD_TEC_MODO)
                            auxTecla = 0x0d;

                        comandos1.bits.mandarTec = 1;
                        comandosTMP1.bits.mandar_click = 1;
                    }
                    else if (auxTecla == TLCD_TEC_ARRIBA)  
                    {
                        auxTecla = 0x00;
                        flags1.bits.soyFlechita = ALTO;
                        comandos1.bits.mandarTec = 1;
                        comandosTMP1.bits.mandar_click = 1;
                    }
                    else if (auxTecla == TLCD_TEC_ABAJO)
                    {
                        auxTecla = 0x01;
                        flags1.bits.soyFlechita = ALTO;
                        comandos1.bits.mandarTec = 1;
                        comandosTMP1.bits.mandar_click = 1;
                    }
                }
            }
            else
            {
                if (!tlcd_tec_isPressed())
                {
                    if (countBounceKey > 0)
                        countBounceKey -= 1;
                    
                    yaMandoTecla = 0;
                }
            }

            /*****************************************************************************/
            //  MAQUINAS DE ESTADO
            /*****************************************************************************/
            tlcd_tec_handler();

            
            /*****************************************************************************/
            //  SACARBUF MPXH
            /*****************************************************************************/
            if ( !mpxh_Ocupado() )   // entro si no esta ocupado
            {
                if (comandos1.bits.mandarTec)
                {
                    comandos1.bits.mandarTec = 0;
                    
                    if ( flags1.bits.soyFlechita )
                    {
                        flags1.bits.soyFlechita = BAJO;
                        mpxh_ArmaMensaje(0x61, nibble_swap(auxTecla), 0, MPXH_BITS_17); 
                    }
                    else
                        mpxh_ArmaMensaje(0x00, nibble_swap(auxTecla), 0, MPXH_BITS_17);
                }
            }

            /*****************************************************************************/
            //  SACARBUF TMP
            /*****************************************************************************/
            if (!tmp_isSending())
            {
                if (comandosTMP1.bits.mandar_click)
                {
                    comandosTMP1.bits.mandar_click = 0;

                    tmp_sendSound(TMP_CLICK);
                }
            }

            /*****************************************************************************/
            //  REINICIO EVENTOS AP
            /*****************************************************************************/
            ap1.bytes = BAJO;       // clereo siempre!!!

//          output_low (DUTY_3_PORT, DUTY_3);

            CLRWDT();
        }
    }
}


// #int_timer0
void __interrupt() timer0_isr(void)
{
    INTCONbits.TMR0IF = BAJO;
    TMR0bits.TMR0 += t_250us_8MHz;

    /*****************************************************************************/
    //  Cada 125 useg
    /*****************************************************************************/
    t0++;

    if ( bit_test( t0,0 ) )
    {
        /*****************************************************************************/
        //  Cada 250 useg
        /*****************************************************************************/
        timers1.bits.useg250 = ALTO;

//      output_high (DUTY_4_PORT, DUTY_4);

        mpxh_Analizar ();
        tmp_handler();


        if ( bit_test(t0, 3) )
        {
            /*****************************************************************************/
            //  Cada 1 mseg
            /*****************************************************************************/
            bit_clear(t0, 3);
            timers1.bits.mseg1 = ALTO;
            t1++;

            timer_Handler();

            if ( bit_test( t1,2 ) )
            {
                /*****************************************************************************/
                //  Cada 4 mseg
                /*****************************************************************************/
                bit_clear( t1,2 );
                timers1.bits.mseg4 = ALTO;
                t2++;


                if ( t2 >= SEGUNDOS )
                {
                    /*****************************************************************************/
                    //  Cada 1 seg
                    /*****************************************************************************/
                    t2 = 0;
                    timers1.bits.seg1 = ALTO;

                }

            }
        }

//      output_low (DUTY_4_PORT, DUTY_4);
    }
}


u8 sensorState2TLCD (u8 sensorState)
{
    switch (sensorState)
    {
        case 0:
            return PERX;
           
        case 1:
            return PERPDOWN;
            
        case 2:
            return PERPUP;
            
        case 3:
            return PERTICK;
    }
}
