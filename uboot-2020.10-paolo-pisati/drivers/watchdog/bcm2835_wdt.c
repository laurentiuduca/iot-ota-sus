/*
 * Watchdog driver for Broadcom BCM2835
 *
 * Copyright (c) 2021 Laurentiu-Cristian Duca (laurentiu dot duca at gmail dot com)
 * Modifications to hw_watchdog_disable() 
 * and use CONFIG_WATCHDOG_TIMEOUT_MSECS in reset_cpu()
 *
 * Copyright (C) 2017 Paolo Pisati <p.pisati@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#include <common.h>
#include <efi_loader.h>
#include <asm/io.h>
#include <asm/arch/wdog.h>
#include <mach/wdog.h>

#define SECS_TO_WDOG_TICKS(x) ((x) << 16)
#define WDOG_TICKS_TO_SECS(x) ((x) >> 16)

extern void reset_cpu(ulong ticks);

#define MAX_TIMEOUT   0xf /* ~15s */

static __efi_runtime_data bool enabled = true;

extern void reset_cpu(ulong ticks);

void hw_watchdog_reset(void)
{
	if (enabled)
		reset_cpu(SECS_TO_WDOG_TICKS(MAX_TIMEOUT));
}

void hw_watchdog_init(void)
{
	hw_watchdog_reset();
}

void __efi_runtime hw_watchdog_disable(void)
{
	enabled = false;
}

