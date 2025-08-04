#ifndef MPXH_H
#define	MPXH_H


/*****************************************************************************/
//  DEFINICIONES
/*****************************************************************************/
#define             MPXH_BITS_8             6
#define             MPXH_BITS_4             5
#define             MPXH_BITS_17            4
#define             MPXH_BITS_16            3
#define             MPXH_BITS_15            2
#define             MPXH_BITS_12            1
#define             MPXH_BITS_9             0


/*****************************************************************************/
//  PROTOTIPOS
/*****************************************************************************/

/*****************************************************************************/
//  mpxh_Analizar
/*****************************************************************************
 *  Descripci�n: funci�n que realiza todo el procesamiento de MPXH: recepci�n
 *               y transmisi�n. Debe ser llamada en la interrupcion peri�dica
 *               de la aplicaci�n.
 *
 *  Entradas: no tiene entradas pero utiliza algunas definiciones de project.h.
 *            De acuerdo a la frecuencia de la interrupci�n peri�dica, se debe
 *            cambiar el valor de las constantes: T, T9, MI_PRIORIDAD, MSEG.
 *            Se debe definir los pines ENT y SAL, que son usados por la rutina.
 *
 *  Salidas: -
 *
 *****************************************************************************/
void        mpxh_Analizar       ( void );


/*****************************************************************************/
//  mpxh_Ocupado
/*****************************************************************************
 *  Descripci�n: indica si el m�dulo MPXH est� transmitiendo algo.
 *
 *  Entradas: -
 *
 *  Salidas: 0 - si no est� transmitiendo.
 *           1 - si el m�dulo est� ocupado transmitiendo.
 *
 *****************************************************************************/
u8          mpxh_Ocupado        ( void );


/*****************************************************************************/
//  mpxh_ArmaMensaje
/*****************************************************************************
 *  Descripci�n: funci�n que carga en el buffer de salida de MPXH un mensaje.
 *               El mensaje sale haciendo corrimientos a izquierda.
 *               Para los mensajes de 15 bits se los debe cargar justificados a
 *               DERECHA (del corrimiento se encarga la funci�n).
 *               Para los mensajes de 9 bits se los debe cargar en dataH.
 *
 *  Entradas: dataH - byte alto del mensaje a enviar.
 *            dataL - byte bajo del mensaje a enviar.
 *            layer - layer en el que se env�a el mensaje (0 a 7)
 *            nbits - cantidad de bits del mensaje. Se debe usar una de las
 *                    definiciones: MPXH_BITS_17 ... MPXH_BITS_9.
 *
 *  Salidas: -
 *
 *****************************************************************************/
void        mpxh_ArmaMensaje    ( u8 dataH, u8 dataL, u8 layer, u8 nbits );


/*****************************************************************************/
//  mpxh_recibio
/*****************************************************************************
 *  Descripci�n: indica si se recibi� un mensaje de la cantidad de bits indicado
 *               por flag.
 *
 *  Entradas: flag - longitud del mensaje por el que se pregunta. Se debe usar
 *                   una de las definiciones: MPXH_BITS_17 ... MPXH_BITS_9.
 *
 *  Salidas: 0 - no se reicib� un mensaje con esa longitud.
 *           1 - se recibi� un mensaje de esa longitud.
 *
 *****************************************************************************/
u8          mpxh_recibio        ( u8 flag );


/*****************************************************************************/
//  mpxh_getRecibido
/*****************************************************************************
 *  Descripci�n: permite acceder al buffer de recepci�n del m�dulo MPXH.
 *
 *  Entradas: dataH - puntero a la variable donde guardar el byte alto recibido.
 *            dataL - puntero a la variable donde guardar el byte bajo recibido.
 *
 *  Salidas: -
 *
 *****************************************************************************/
void        mpxh_getRecibido    ( u8 *dataH, u8 *dataL );


/*****************************************************************************/
//  mpxh_tiempoIdle
/*****************************************************************************
 *  Descripci�n: indica si la l�nea estuvo en idle el tiempo indicado.
 *
 *  Entradas: tiempo - cantidad de tienpo que la l�nea tuvo que estar en idle.
 *                     Armar este tiempo con la constante MSEG. Ej: 16*MSEG
 *                     (4 milisegundos en idle la l�nea MPXH)
 *
 *  Salidas: 0 - la l�nea MPXH estuvo menos tiempo en idle que el indicado.
 *           1 - la l�nea MPXH estuvo en idle por lo menos el tiempo indicado.
 *
 *****************************************************************************/
u8          mpxh_tiempoIdle     ( u8 tiempo);


/*****************************************************************************/
//  mpxh_clearRecFlag
/*****************************************************************************
 *  Descripci�n: permite bajar un flag de recepci�n cuando se atendio una 
 *               recepci�n.
 *
 *  Entradas: flag - longitud del mensaje que se recibi�. Se debe usar
 *                   una de las definiciones: MPXH_BITS_17 ... MPXH_BITS_9.
 *
 *  Salidas: -
 *
 *****************************************************************************/
void        mpxh_clearRecFlag   ( u8 flag );


/*****************************************************************************/
//  mpxh_forceMPXHLow
/*****************************************************************************
 *  Descripci�n: fuerza la l�nea MPXH a un estado bajo.
 *
 *  Entradas: -
 *
 *  Salidas: -
 *
 *****************************************************************************/
void        mpxh_forceMPXHLow   (void);


/*****************************************************************************/
//  mpxh_releaseMPXH
/*****************************************************************************
 *  Descripci�n: libera la l�nea MPXH despu�s de haberla forzado a bajo.
 *
 *  Entradas: -
 *
 *  Salidas: -
 *
 *****************************************************************************/
void        mpxh_releaseMPXH    (void);


/*****************************************************************************/
//  mpxh_abortTx
/*****************************************************************************
 *  Descripci�n: aborta la transmisi�n MPXH actual.
 *
 *  Entradas: -
 *
 *  Salidas: -
 *
 *****************************************************************************/
void        mpxh_abortTx        (void);

#endif	/* MPXH_H */

