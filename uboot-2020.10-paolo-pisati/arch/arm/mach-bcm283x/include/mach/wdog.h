/* SPDX-License-Identifier: GPL-2.0 */
/*
 * (C) Copyright 2021 Laurentiu-Cristian Duca
 * Moved some defines from reset.c to wdog.h
 *
 * (C) Copyright 2012,2015 Stephen Warren
 */

#ifndef _BCM2835_WDOG_H
#define _BCM2835_WDOG_H

#include <asm/arch/base.h>

#define BCM2835_WDOG_PHYSADDR ({ BUG_ON(!rpi_bcm283x_base); \
				 rpi_bcm283x_base + 0x00100000; })

struct bcm2835_wdog_regs {
	u32 unknown0[7];
	u32 rstc;
	u32 rsts;
	u32 wdog;
};

#define BCM2835_WDOG_PASSWORD			0x5a000000

/*
 * The Raspberry Pi firmware uses the RSTS register to know which partiton
 * to boot from. The partiton value is spread into bits 0, 2, 4, 6, 8, 10.
 * Partiton 63 is a special partition used by the firmware to indicate halt.
 */
#define BCM2835_WDOG_RSTS_RASPBERRYPI_HALT      0x555

/* max ticks timeout */
#define BCM2835_WDOG_MAX_TIMEOUT        	0x000fffff
#define BCM2835_WDOG_RSTC_RESET         	0x00000102
#define BCM2835_WDOG_RSTC_WRCFG_MASK		0x00000030
#define BCM2835_WDOG_RSTC_WRCFG_FULL_RESET	0x00000020

#endif
