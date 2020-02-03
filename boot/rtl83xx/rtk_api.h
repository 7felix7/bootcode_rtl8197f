/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : Definition function prototype of RTK API.
 *
 * Feature : Function prototype definition
 *
 */

#ifndef __RTK_API_H__
#define __RTK_API_H__

/*
 * Include Files
 */
#include <rtk_types.h>
#include <rtk_error.h>

/*
 * Data Type Declaration
 */
#define ENABLE                                      1
#define DISABLE                                     0

typedef enum rtk_switch_maxPktLen_e
{
    MAXPKTLEN_1522B = 0,
    MAXPKTLEN_1536B,
    MAXPKTLEN_1552B,
    MAXPKTLEN_16000B,
    MAXPKTLEN_END
} rtk_switch_maxPktLen_t;

#endif /* __RTK_API_H__ */

