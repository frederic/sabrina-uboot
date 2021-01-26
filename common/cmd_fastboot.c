/*
 * Copyright 2008 - 2009 Windriver, <www.windriver.com>
 * Author: Tom Rix <Tom.Rix@windriver.com>
 *
 * (C) Copyright 2014 Linaro, Ltd.
 * Rob Herring <robh@kernel.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <command.h>
#include <g_dnl.h>
#ifdef CONFIG_SEI_FASTBOOT
#include <asm/io.h>
#include <asm/arch/secure_apb.h>
#include <asm/cpu_id.h>
#endif

static int do_fastboot(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret;

	g_dnl_clear_detach();
	ret = g_dnl_register("usb_dnl_fastboot");
	if (ret)
		return ret;
#ifdef CONFIG_SEI_FASTBOOT
        run_command("osd clear;imgread pic logo fastboot $loadaddr;bmp display $fastboot_offset;bmp scale;",0);
#endif
	while (1) {
		if (g_dnl_detach())
			break;
		if (ctrlc())
			break;
		usb_gadget_handle_interrupts();
#ifdef CONFIG_SEI_FASTBOOT
		setbits_le32(P_AO_GPIO_O_EN_N, (1 << 2)); //GPIOAO_2 input
		readl(AO_GPIO_I);
		if(!(readl(AO_GPIO_I) & (0x01 << 2)))
		{
                    run_command("osd clear;imgread pic logo recovery_boot $loadaddr;bmp display $recovery_boot_offset;bmp scale;",0);
                    run_command("run recovery_from_flash;",0);
		}
#endif
	}

	g_dnl_unregister();
	g_dnl_clear_detach();
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	fastboot,	1,	0,	do_fastboot,
	"use USB Fastboot protocol",
	"\n"
	"    - run as a fastboot usb device"
);
