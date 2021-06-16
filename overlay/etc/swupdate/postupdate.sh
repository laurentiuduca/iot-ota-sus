#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2021 Laurentiu-Cristian Duca

# suppose boot.vfat is the first image update   
set -x

        mount /dev/mmcblk0p1 /mnt
	/root/bootcount.out 1
	kernel_addr_r=`fw_printenv kernel_addr_r`
	# strip everything until and including "kernel_addr_r="
	kernel_addr_r="${kernel_addr_r#*kernel_addr_r=}"
	fdt_addr_r=`fw_printenv fdt_addr_r`
        # strip everything until and including "fdt_addr_r="
        fdt_addr_r="${fdt_addr_r#*fdt_addr_r=}"
        mmcstr=`cat /root/mmc-part-id`
        # echo "mmcstr=$mmcstr"
        if test "$mmcstr" == "2" ; then
		fw_setenv bootcmd "setenv bootargs \"8250.nr_uarts=1 root=/dev/mmcblk0p2 rootwait console=tty1 console=ttyS0,115200\"; load mmc 0:2 $kernel_addr_r zImage ; load mmc 0:2 $fdt_addr_r bcm2835-rpi-zero-w.dtb ; bootz $kernel_addr_r - $fdt_addr_r"
		fw_setenv altbootcmd "setenv bootargs \"8250.nr_uarts=1 root=/dev/mmcblk0p3 rootwait console=tty1 console=ttyS0,115200\"; load mmc 0:3 $kernel_addr_r zImage ; load mmc 0:3 $fdt_addr_r bcm2835-rpi-zero-w.dtb ; bootz $kernel_addr_r - $fdt_addr_r"
                mount /dev/mmcblk0p2 /media
        else
		# assert mmcstr is 3
		fw_setenv bootcmd "setenv bootargs \"8250.nr_uarts=1 root=/dev/mmcblk0p3 rootwait console=tty1 console=ttyS0,115200\"; load mmc 0:3 $kernel_addr_r zImage ; load mmc 0:3 $fdt_addr_r bcm2835-rpi-zero-w.dtb ; bootz $kernel_addr_r - $fdt_addr_r"
		fw_setenv altbootcmd "setenv bootargs \"8250.nr_uarts=1 root=/dev/mmcblk0p2 rootwait console=tty1 console=ttyS0,115200\"; load mmc 0:2 $kernel_addr_r zImage ; load mmc 0:2 $fdt_addr_r bcm2835-rpi-zero-w.dtb ; bootz $kernel_addr_r - $fdt_addr_r"
                mount /dev/mmcblk0p3 /media
        fi
        # Add update marker
        touch /media/update-ok
        umount /media
        umount /mnt

        reboot

