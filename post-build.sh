#!/bin/sh

set -x
set -u
set -e

# Add a console on tty1
if [ -e ${TARGET_DIR}/etc/inittab ]; then
    grep -qE '^tty1::' ${TARGET_DIR}/etc/inittab || \
	sed -i '/GENERIC_SERIAL/a\
tty1::respawn:/sbin/getty -L  tty1 0 vt100 # HDMI console' ${TARGET_DIR}/etc/inittab
fi

# remove openssh-server
rm -f ${TARGET_DIR}/etc/init.d/S50sshd
rm -f ${TARGET_DIR}/etc/init.d/S21haveged
cp ${BUILD_DIR}/libcurl-7.74.0/src/curl ${TARGET_DIR}/usr/bin/
if test -f ${TARGET_DIR}/etc/init.d/S99at; then
	mv ${TARGET_DIR}/etc/init.d/S99at ${TARGET_DIR}/etc/init.d/S90at
fi
cp ${BUILD_DIR}/libopenssl-1.1.1j/apps/openssl ${TARGET_DIR}/usr/bin/
cp ${BUILD_DIR}/uboot-tools-2020.04/tools/env/fw_printenv ${TARGET_DIR}/usr/sbin/

printf "DAEMON_ARGS=\"-o /dev/random -r /dev/urandom\" \n" > ${TARGET_DIR}/etc/default/rngd
printf "127.0.0.1	localhost\n127.0.1.1	buildroot\n10.10.10.100	laurPC-100\n" > ${TARGET_DIR}/etc/hosts

mkdir -p ${BINARIES_DIR}/extlinux-orig
printf "label rpi0w-buildroot\n  kernel /zImage\n  devicetree /bcm2835-rpi-zero-w.dtb\n  append 8250.nr_uarts=1 root=/dev/mmcblk0p2 rootwait console=tty1 console=ttyS0,115200\n" > ${BINARIES_DIR}/extlinux-orig/extlinux.conf

cp ${BINARIES_DIR}/rpi-firmware/config.txt ${BINARIES_DIR}/rpi-firmware/config.txt.bak
cat ${BINARIES_DIR}/rpi-firmware/config.txt.bak | sed -e "s/kernel=zImage/kernel=u-boot.bin/g" > ${BINARIES_DIR}/rpi-firmware/config.txt

cp ${BINARIES_DIR}/zImage ${TARGET_DIR}/
cp ${BINARIES_DIR}/bcm2835-rpi-zero-w.dtb ${TARGET_DIR}/

echo "/mnt/uboot.env 0 0x4000" > ${TARGET_DIR}/etc/fw_env.config
cp ${BINARIES_DIR}/rpi-firmware/bootcount.out ${TARGET_DIR}/root/

echo 'watchdog-device = /dev/watchdog' >> ${TARGET_DIR}/etc/watchdog.conf
echo 'watchdog-timeout = 15' >> ${TARGET_DIR}/etc/watchdog.conf
echo 'max-load-1 = 24' >> ${TARGET_DIR}/etc/watchdog.conf

