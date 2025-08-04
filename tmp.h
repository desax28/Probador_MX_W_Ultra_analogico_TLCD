#ifndef TMP_H
#define	TMP_H

#include "project.h"

/*****************************************************************************/
//  SONIDOS TMP
/*****************************************************************************/
#define TMP_TRITONAL    0x3c
#define TMP_ATENCION    0x16
#define TMP_PIRUPIRU    0x04
#define TMP_GRAVE       0x3b
#define TMP_PIP         0x1a
#define TMP_CLICK       0x3d
#define TMP_TIMBRE      0x3c
/****************************************************************************/

void tmp_init (void);
void tmp_handler (void);
u8 tmp_isSending (void);
void tmp_sendSound (u8 sound);
void tmp_setVolume (u8 vol);
void tmp_reset (void);


#endif	/* TMP_H */

