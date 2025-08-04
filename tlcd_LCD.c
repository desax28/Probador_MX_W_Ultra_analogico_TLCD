#include "tlcd_LCD.h"
#include "temporizadores.h"

/*****************************************************************************/
//  FUNCIONES PRIVADAS
/*****************************************************************************/
void delay64msegs (void);
void delay40usegs (void);
u8 convertDRAM (u8 data);
u8 ascii2DRAM (u8 ascii);


/*****************************************************************************/
//  VARIABLES PRIVADAS
/*****************************************************************************/
Temporizador t_delay64msegs;
u8 punteroDRAM;
u8 DRAM[32];

u8 pwmLevel;
u8 timerPWM;

u8 tablaLCD[]={
	0x3b,		// 24h - ;
	0x3a,		// 25h - :
	0x5f,		// 26h - _ (underscore)
	0x00,		// 27h - cuadrado (custom)
	0x9c,		// 28h - Ñ
	0x2d,		// 29h - -
	0x2f,		// 2ah - /
	0x20,		// 2bh - (espacio)
	0x03,		// 2ch -  campanita (custom)
	0x2a,		// 2dh - *
	0x2c,		// 2eh - ,
	0x2e,		// 2fh - .

	0x9a,		// 30h - Ü
	0x28,		// 31h - (
	0x29,		// 32h - )
	0x01,		// 33h - flecha up (custom)
	0x02,		// 34h - flecha down (custom)
	0x9f,		// 35h - ¿
	0x3f,		// 36h - ?
	0xe9,		// 37h - ¡
	0x21,		// 38h - !
	0x9d,		// 39h - ª
	0x9e,		// 3ah - º
	0x22,		// 3bh - "
    0x23,		// 3ch - #
	0x2b,		// 3dh - +
	0x7f,		// 3eh - LEFT
	0x7e,		// 3fh - RIGHT

	0x20,		// 40h - ;4xh son comandos, nunca se escriben en dram
	0x20,		// 41h -
	0x20,		// 42h -
	0x20,		// 43h -
	0x20,		// 44h -
	0x20,		// 45h -
	0x20,		// 46h -
	0x20,		// 47h -
	0x20,		// 48h -
	0x20,		// 49h -
	0x20,		// 4ah -
	0x20,		// 4bh -
	0x20,		// 4ch -
	0x20,		// 4dh -
	0x20,		// 4eh -
	0x20,		// 4fh -

	0x20,		// 50h - ;5xh, son comandos también
	0x20,		// 51h -
	0x20,		// 52h -
	0x20,		// 53h -
	0x20,		// 54h -
	0x20,		// 55h -
	0x20,		// 56h -
	0x20,		// 57h -
	0x20,		// 58h -
	0x20,		// 59h -
	0x20,		// 5ah -
	0x20,		// 5bh -
	0x20,		// 5ch -
	0x20,		// 5dh -
	0x20,		// 5eh -
	0x20,		// 5fh -

	0x04,		// 60h - 3 rayas (custom)
	0x05,		// 61h - 5 rayas (custom)
	0x06,		// 62h - tick (custom)
	0x07,		// 63h - arena (custom)
	0x1e,		// 64h - omega
	0x7c,		// 65h - palito
	0x6f,		// 66h - o minusc
	0xf7,		// 67h - cruz a 45 grados
	0x20,		// 68h -
	0x20,		// 69h -
	0x20,		// 6ah -
	0x20,		// 6bh -
	0x20,		// 6ch -
	0x20,		// 6dh -
	0x20,		// 6eh -
	0x20,		// 6fh -

	0x20,		// 70h -
	0x20,		// 71h -
	0x20,		// 72h -
	0x20,		// 73h -
	0x20,		// 74h -
	0x20,		// 75h -
	0x20,		// 76h -
	0x20,		// 77h -
	0x20,		// 78h -
	0x20,		// 79h -
	0x20,		// 7ah -
	0x20,		// 7bh -
	0x20,		// 7ch -
	0x20,		// 7dh -
	0x20,		// 7eh -
	0x20		// 7fh -
};

void tlcd_lcd_init ()
{
    u8 i;

    /*****************************************************************************/
    //  INICIALIZACIÓN DE LOS PINES
    /*****************************************************************************/
    LATC = 0;
    bit_clear(LCD_DATA_TRIS, LCD_DATA);
    bit_clear(LCD_CLOCK_TRIS, LCD_CLOCK);
    bit_clear(LCD_CMD_DATA_TRIS, LCD_CMD_DATA);
    bit_clear(LCD_RESET_TRIS, LCD_RESET);
    bit_clear(BACKLIGHT_TRIS, BACKLIGHT);

    /*****************************************************************************/
    //  INCIALIZACIÓN DE LAS VARIABLES
    /*****************************************************************************/
    punteroDRAM = 0;
    for (i = 0; i < 32; i++)
        DRAM[i] = 0x2b;

    timerPWM = 0;
    tlcd_backlight_setLevel(2);

    /*****************************************************************************/
    //  INICIALIZACIÓN DEL DISPLAY
    /*****************************************************************************/
    timer_Init(&t_delay64msegs, 64);

    delay64msegs();
    output_low(LCD_RESET_PORT, LCD_RESET);
    delay64msegs();
    output_high(LCD_RESET_PORT, LCD_RESET);
    delay64msegs();

    // Manda 4 veces el set inicial. 8bits, 2 líneas
    tlcd_lcd_send_cmd(LCD_SETD);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_SETD);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_SETD);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_SETD | 0b00001001);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_OSC);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_POWER);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_FOLLOWER);
    delay64msegs();
    delay64msegs();
    delay64msegs();
    delay64msegs();

    tlcd_lcd_send_cmd(LCD_CONTRASTE | 0x03);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_DISPLAYON);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_SETCUR);
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_CLRD);
    delay64msegs();

    
    tlcd_lcd_send_cmd(LCD_SETD | 0b00001000);       // IS=0 para acceder a la CGRAM
    delay40usegs();

    tlcd_lcd_send_cmd(0b01000000);                  // Dirección 0 de CGRAM
    delay40usegs();


//--------------------------------------------------------------------------------------
    // Cuadrado en la 00
    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    // Flecha UP en la 01
    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00001110);
    delay40usegs();

    tlcd_lcd_send_data(0b00010101);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    // Flecha DOWN en la 02
    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00010101);
    delay40usegs();

    tlcd_lcd_send_data(0b00001110);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    // Comapanita en la 03
    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00001110);
    delay40usegs();

    tlcd_lcd_send_data(0b00001110);
    delay40usegs();

    tlcd_lcd_send_data(0b00001110);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    // Tres Rayas en la 04
    tlcd_lcd_send_data(0b00000000);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    // Cinco Rayas en la 05
    tlcd_lcd_send_data(0b00000000);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    // Tick en la 06
    tlcd_lcd_send_data(0b00000001);
    delay40usegs();

    tlcd_lcd_send_data(0b00000011);
    delay40usegs();

    tlcd_lcd_send_data(0b00000010);
    delay40usegs();

    tlcd_lcd_send_data(0b00100110);
    delay40usegs();

    tlcd_lcd_send_data(0b00110100);
    delay40usegs();

    tlcd_lcd_send_data(0b00011100);
    delay40usegs();

    tlcd_lcd_send_data(0b00001000);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    // Arena en la 07
    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00001110);
    delay40usegs();

    tlcd_lcd_send_data(0b00000100);
    delay40usegs();

    tlcd_lcd_send_data(0b00001110);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00011111);
    delay40usegs();

    tlcd_lcd_send_data(0b00000000);
    delay40usegs();
//--------------------------------------------------------------------------------------
    
    tlcd_lcd_send_cmd(LCD_SETD | 0b00001001);       // manda 39, para que se pueda controlar contraste
    delay40usegs();

    tlcd_lcd_send_cmd(LCD_SETCUR);
    delay40usegs();
    
    tlcd_lcd_send_cmd(0b10000000);                  // vuelve a dram posicionando el cursor arriba a la izquierda
    delay40usegs();
    
}


void tlcd_lcd_send_data (u8 data)
{
    i8 n;

    output_high(LCD_CMD_DATA_PORT, LCD_CMD_DATA);

    for (n = 7; n >= 0; n --)
    {
        if (bit_test(data,n))
            output_high(LCD_DATA_PORT, LCD_DATA);
        else
            output_low(LCD_DATA_PORT, LCD_DATA);

        NOP();
        NOP();
        NOP();
        NOP();
        NOP(); 

        output_high(LCD_CLOCK_PORT, LCD_CLOCK);

        NOP();

        output_low(LCD_CLOCK_PORT, LCD_CLOCK);
    }
}


void tlcd_lcd_send_cmd  (u8 cmd)
{
    i8 n;

    output_low(LCD_CMD_DATA_PORT, LCD_CMD_DATA);

    for (n = 7; n >= 0; n --)
    {
        if (bit_test(cmd,n))
            output_high(LCD_DATA_PORT, LCD_DATA);
        else
            output_low(LCD_DATA_PORT, LCD_DATA);

        NOP();
        NOP();
        NOP();
        NOP();
        NOP();                                

        output_high(LCD_CLOCK_PORT, LCD_CLOCK);

        NOP();

        output_low(LCD_CLOCK_PORT, LCD_CLOCK);
    }
}


void tlcd_lcd_handler (void)
{
    u8 data;

//--------------------------------------------------------------------------------------
    // PWM por software del backlight (el handler se lo llama cada 1 msegs)
    if (timerPWM == pwmLevel)
    {
        output_low(BACKLIGHT_PORT, BACKLIGHT);
    }

    timerPWM++;
    if (timerPWM == 8)
    {
        output_high(BACKLIGHT_PORT, BACKLIGHT);
        timerPWM = 0;
    }
//--------------------------------------------------------------------------------------

    if (punteroDRAM == 0)
    {
        // Ubica el cursor al principio de la primera línea
        tlcd_lcd_send_cmd(0b10000000);
        delay40usegs();
    }
    else if (punteroDRAM == 16)
    {
        // Ubica el cursor al principio de la segunda línea
        tlcd_lcd_send_cmd(0b11000000);
        delay40usegs();
    }

    data = convertDRAM (DRAM[punteroDRAM]);
    tlcd_lcd_send_data(data);

    
    punteroDRAM ++;

    if (punteroDRAM > 31)
        punteroDRAM = 0;

}


void tlcd_DRAM_clear (void)
{
    u8 i = 0;

    for (i = 0; i < 32; i++)
        DRAM[i] = PERBLANCO;
}

void tlcd_DRAM_setChar (u8 pos, u8 data)
{
    if (pos >= 0 && pos < 32)
        DRAM[pos] = data;
}


void tlcd_DRAM_setRAMString (u8 pos, u8* string)
{
    u8 i = 0;

    while ((pos >= 0 && pos < 32) && (string[i] != '\0'))
    {
        DRAM[pos] = ascii2DRAM(string[i]);
        i++;
        pos++;
    }
}


void tlcd_DRAM_setROMString (u8 pos, const u8* string)
{
    u8 i = 0;

    while ((pos >= 0 && pos < 32) && (string[i] != '\0'))
    {
        DRAM[pos] = ascii2DRAM(string[i]);
        i++;
        pos++;
    }
}


void tlcd_DRAM_setArray (u8 pos, u8* array, u8 len)
{
    u8 i = 0;

    while ((pos >= 0 && pos < 32) && (i < len))
    {
        DRAM[pos] = array[i];
        i++;
        pos++;
    }
}


void tlcd_backlight_setLevel (u8 level)
{
    pwmLevel = level;
}


/*****************************************************************************/
//  FUNCIONES PRIVADAS
/*****************************************************************************/
void delay64msegs (void)
{
    timer_Restart(&t_delay64msegs);
    while (!timer_Expiro(t_delay64msegs));
}


void delay40usegs (void)
{
    u8 i, j;

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 160; j++)
        {
            
        }
    }
}


u8 convertDRAM (u8 data)
{
    u8 out;

    // Convierte el contenido de DRAM al valor que se debe enviar al LCD
    // Para 0 a 9 suma 0x30
    if (data >= 0 && data <= 9)
    {
        out = data + 0x30;
    }
    else if (data >= 0x0a && data <= 0x23)
    {
        out = data - 0x0a + 0x41;
    }
    else
    {
        out = tablaLCD[data - 0x24];
    }

    return out;
}

u8 ascii2DRAM (u8 ascii)
{
    if (ascii >= '0' && ascii <= '9')
        return (ascii - '0');

    else if (ascii >= 'A' && ascii <= 'Z')
        return (ascii - 'A' + 0x0A);

    else if (ascii == ';')
        return 0x24;

    else if (ascii == '_')
        return 0x26;

    else if (ascii == 'c')      // cuadrado ????
        return 0x24;

    else if (ascii == ':')
        return 0x25;

    else if (ascii == 'Ñ')
        return 0x28;

    else if (ascii == '-')
        return 0x29;

    else if (ascii == '/')
        return 0x2A;

    else if (ascii == ' ')
        return 0x2B;

    else if (ascii == '*')
        return 0x2D;

    else if (ascii == ',')
        return 0x2E;

    else if (ascii == '.')
        return 0x2F;

    else if (ascii == '(')
        return 0x31;

    else if (ascii == ')')
        return 0x32;

    else if (ascii == 'u')      // Flecha arriba ?????
        return 0x33;

    else if (ascii == 'd')      // Flecha abajo ??????
        return 0x34;

    else if (ascii == '?')
        return 0x35;

    else if (ascii == '¿')
        return 0x36;

    else if (ascii == '¡')
        return 0x37;

    else if (ascii == '!')
        return 0x38;

    else if (ascii == 'a')          // A superindice ?????
        return 0x39;

    else if (ascii == 'o')          // O superindice ?????
        return 0x3A;

    else if (ascii == '"')
        return 0x3B;

    else if (ascii == '#')
        return 0x3C;

//    else if (ascii == '$')
//        return 0x3D;

    else if (ascii == '+')
        return 0x3D;

    else if (ascii == '<')
        return 0x3E;

    else if (ascii == '>')
        return 0x3F;
}