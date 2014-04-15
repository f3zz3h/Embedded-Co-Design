mount  /dev/scsi/host0/bus0/target0/lun0/part1 /mnt/cf
mount -t devfs devfs /mnt/cf/dev
chroot /mnt/cf /etc/init.d/rcS.USB

for I in /proc/[0-9]*; do 
	ls -al $I/root; 
done | grep '/mnt/cf' | cut -d'/' -f3 | xargs kill -9
umount /mnt/cf/proc/bus/usb /mnt/cf/dev/pts /mnt/cf/proc /mnt/cf/dev /mnt/cf
