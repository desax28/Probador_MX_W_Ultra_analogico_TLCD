#include "tlcd_leds.h"



void tlcd_leds_init (void)
{
    bit_clear(LED_ACT_TRIS,LED_ACT);
    bit_clear(LED_MEM_TRIS,LED_MEM);
    bit_clear(LED_LISTA_TRIS,LED_LISTA);
    bit_clear(LED_ESTOY_TRIS,LED_ESTOY);
    bit_clear(LED_ME_VOY_TRIS,LED_ME_VOY);
    bit_clear(LED_BAT_TRIS,LED_BAT);
    bit_clear(LED_RED_TRIS,LED_RED);

    bit_clear(LED_K_TRIS,LED_K);

    output_high (LED_K_PORT,LED_K);
}


void tlcd_leds_setLed (u8 led)
{
    output_high (LED_ACT_PORT,led);
}


void tlcd_leds_clearLed (u8 led)
{
    output_low (LED_ACT_PORT,led);
}


void tlcd_leds_toggleLed (u8 led)
{
    output_toggle (LED_ACT_PORT,led);
}


u8 tlcd_leds_getLed (u8 led)
{

}