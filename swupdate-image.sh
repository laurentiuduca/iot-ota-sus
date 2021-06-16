IMG_FILES="sw-description rootfs.ext4.gz"
for f in ${IMG_FILES} ; do
    echo ${f}
done | cpio -ovL -H crc > buildroot.swu

