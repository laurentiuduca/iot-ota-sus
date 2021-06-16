// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2021 Laurentiu-Cristian Duca
 * Modifications: code to stop the wdog and watchdog_time_left().
 *
 * (C) Copyright 2012 Stephen Warren
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 */

#include <common.h>
#include <cpu_func.h>
#include <asm/io.h>
#include <asm/arch/base.h>
#include <asm/arch/wdog.h>
#include <efi_loader.h>

#define RESET_TIMEOUT 10

#ifdef CONFIG_BCM2835_WATCHDOG
extern void __efi_runtime hw_watchdog_disable(void);
#else
void hw_watchdog_disable(void) {}
#endif

#define SECS_TO_WDOG_TICKS(x) ((x) << 16)
#define WDOG_TICKS_TO_SECS(x) ((x) >> 16)

__efi_runtime_data struct bcm2835_wdog_regs *wdog_regs;

void __efi_runtime __reset_cpu(struct bcm2835_wdog_regs *wdog_regs,
		ulong ticks)
{
	uint32_t rstc, timeout;

	if (ticks == 0) {
		hw_watchdog_disable();
		timeout = RESET_TIMEOUT;
	} else
		timeout = ticks & BCM2835_WDOG_MAX_TIMEOUT;

	rstc = readl(&wdog_regs->rstc);
	rstc &= ~BCM2835_WDOG_RSTC_WRCFG_MASK;
	rstc |= BCM2835_WDOG_RSTC_WRCFG_FULL_RESET;

	writel(BCM2835_WDOG_PASSWORD | timeout, &wdog_regs->wdog);
	writel(BCM2835_WDOG_PASSWORD | rstc, &wdog_regs->rstc);
}

void reset_cpu(ulong ticks)
{
	struct bcm2835_wdog_regs *regs =
		(struct bcm2835_wdog_regs *)BCM2835_WDOG_PHYSADDR;

	__reset_cpu(regs, ticks);
}

#ifdef DEBUG_BCM2835_WATCHDOG
unsigned int watchdog_time_left(void)
{
        unsigned int ret, timeleft;
        struct bcm2835_wdog_regs *wdog_regs =
                (struct bcm2835_wdog_regs *)BCM2835_WDOG_PHYSADDR;

        ret = readl(&wdog_regs->wdog);
        timeleft = WDOG_TICKS_TO_SECS(ret & BCM2835_WDOG_MAX_TIMEOUT);

	return timeleft;
}

bool watchdog_is_running(void)
{
        uint32_t cur;
        struct bcm2835_wdog_regs *wdog_regs =
                (struct bcm2835_wdog_regs *)BCM2835_WDOG_PHYSADDR;

        cur = readl(&wdog_regs->rstc);

        return !!(cur & BCM2835_WDOG_RSTC_WRCFG_FULL_RESET);
}
#endif

#ifdef CONFIG_EFI_LOADER

void __efi_runtime EFIAPI efi_reset_system(
			enum efi_reset_type reset_type,
			efi_status_t reset_status,
			unsigned long data_size, void *reset_data)
{
	u32 val;

	if (reset_type == EFI_RESET_COLD ||
	    reset_type == EFI_RESET_WARM ||
	    reset_type == EFI_RESET_PLATFORM_SPECIFIC) {
		__reset_cpu(wdog_regs, 0);
	} else if (reset_type == EFI_RESET_SHUTDOWN) {
		/*
		 * We set the watchdog hard reset bit here to distinguish this reset
		 * from the normal (full) reset. bootcode.bin will not reboot after a
		 * hard reset.
		 */
		val = readl(&wdog_regs->rsts);
		val |= BCM2835_WDOG_PASSWORD;
		val |= BCM2835_WDOG_RSTS_RASPBERRYPI_HALT;
		writel(val, &wdog_regs->rsts);
		__reset_cpu(wdog_regs, 0);
	}

	while (1) { }
}

efi_status_t efi_reset_system_init(void)
{
	wdog_regs = (struct bcm2835_wdog_regs *)BCM2835_WDOG_PHYSADDR;
	return efi_add_runtime_mmio(&wdog_regs, sizeof(*wdog_regs));
}

#endif
