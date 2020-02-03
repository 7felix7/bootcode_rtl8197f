#! /bin/sh


filename=$1
size=$2

if [ `ls -l $filename|awk '{print $5}'` -gt $size ]; then
	echo "boot size large then $size,please check btcode/boot size"
	exit 1
fi

exit 0


