##/*-------------------------------------------------------------------
##		Created by REALTEK  
##--------------------------------------------------------------------*/
include ../.config
#CROSS= mips-linux-

CC	= $(CROSS)gcc

ifeq ($(CONFIG_LZMA_ENABLE),y)
EXTRA_CFLAGS += -DLZMA_COMPRESS -D_LZMA_PROB32
endif

ifeq ($(CONFIG_POST_ENABLE),y)
CFLAGS += -DCONFIG_POST_ENABLE
endif

ifeq ($(CONFIG_BOOT_DEBUG_ENABLE),y)
CFLAGS += -DCONFIG_BOOT_DEBUG_ENABLE
endif

ifeq ($(RTL8196),1)
EXTRA_CFLAGS += -DRTL8196=1
endif

MARCH_FLAG = -march=mips32r2 -I../bsp

ifeq ($(CONFIG_FPGA_PLATFORM),1)
EXTRA_CFLAGS += -DCONFIG_FPGA_PLATFORM=1
endif

EXTRA_CFLAGS += -I./ddr -I./

ifeq ($(CONFIG_DRAM_BOOT_TEST),y)
DRAM_BOOT_TEST_OUTPUT = ddr/dramtest.o 
else
DRAM_BOOT_TEST_OUTPUT =
endif

ifeq ($(CONFIG_CUSTOMER_BOOT_SIGNATURE_SUPPORT),y)
BOOT_SIGNATURE=$(CONFIG_CUSTOMER_BOOT_SIGNATURE)
else
BOOT_SIGNATURE=boot
endif

#=============================================================================
ifeq ($(CONFIG_LZMA_ENABLE),y)
bootload:	bootload.c cache_c.c start.S cache_cpu.S LzmaDecode.c piggy.S
	sync;sync;sync
	$(CC) $(MARCH_FLAG) -g  -I../boot/include -fno-pic -mno-abicalls -O2 -fomit-frame-pointer -D__KERNEL__ $(EXTRA_CFLAGS) -c piggy.S -o piggy.o 
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls  -D__KERNEL__ $(EXTRA_CFLAGS)  -c bootload.c -o bootload.o
	$(CC) $(MARCH_FLAG) -g  -I../boot/include -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls  -D__KERNEL__ $(EXTRA_CFLAGS)  -c cache_c.c -o cache_c.o
	$(CC) $(MARCH_FLAG) -g  -I../boot/include -fno-inline -fno-pic -mno-abicalls -O2 -fomit-frame-pointer -D__KERNEL__ $(EXTRA_CFLAGS) -c LzmaDecode.c -o LzmaDecode.o
	
	./lzma e boot.img boot.img.gz
	
	$(CROSS)objcopy --add-section .initrd=boot.img.gz piggy.o
ifeq ($(CONFIG_NAND_FLASH_BOOTING),y)
	$(CROSS)ld  -X -nostdlib -Tpiggy.script_nand  --static piggy.o bootload.o cache_c.o LzmaDecode.o -o piggy.elf
else
	$(CROSS)ld  -X -nostdlib -Tpiggy.script  --static piggy.o bootload.o cache_c.o LzmaDecode.o -o piggy.elf
endif
	$(CROSS)objcopy  -Obinary piggy.elf piggy.bin
	$(CROSS)objdump -h -S piggy.elf > piggy.elf.txt
endif
#==================================================================================
ifeq ($(CONFIG_GZIP_ENABLE),y)
bootload:	bootload.c cache_c.c start.S cache_cpu.S gunzip.c piggy.S cvimg_nand
	sync;sync;sync
	$(CC) $(MARCH_FLAG) -g  -I../boot/include -fno-pic -mno-abicalls -O2 -fomit-frame-pointer -D__KERNEL__ $(EXTRA_CFLAGS) -c piggy.S -o piggy.o 
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls  -D__KERNEL__ $(EXTRA_CFLAGS)  -c bootload.c -o bootload.o
	$(CC) $(MARCH_FLAG) -g -I../boot/include -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls  -D__KERNEL__ $(EXTRA_CFLAGS)  -c cache_c.c -o cache_c.o
	$(CC) $(MARCH_FLAG) -g  -I../boot/include -fno-pic -mno-abicalls -O2 -fomit-frame-pointer -D__KERNEL__ $(EXTRA_CFLAGS) -c gunzip.c -o gunzip.o	
		
	cat boot.img | gzip -9 > boot.img.gz
	
	$(CROSS)objcopy --add-section .initrd=boot.img.gz piggy.o
ifeq ($(CONFIG_NAND_FLASH_BOOTING),y)
	$(CROSS)ld  -X -nostdlib -Tpiggy.script_nand  --static piggy.o bootload.o cache_c.o gunzip.o -o piggy.elf
else
	$(CROSS)ld  -X -nostdlib -Tpiggy.script  --static piggy.o bootload.o cache_c.o gunzip.o -o piggy.elf
endif
	$(CROSS)objcopy  -Obinary piggy.elf piggy.bin
	$(CROSS)objdump -h -S piggy.elf > piggy.elf.txt
endif
#==================================================================================
ifeq ($(CONFIG_NONO_COMPRESS),y)
bootload:	bootload.c start.S cache_cpu.S cvimg_nand

endif
	$(CC) $(MARCH_FLAG) -D__ASSEMBLY__ -I../boot/include -fno-pic -mno-abicalls -O2 -fomit-frame-pointer -D__KERNEL__ $(EXTRA_CFLAGS) -c start.S -o start.o 
ifeq ($(CONFIG_NAND_FLASH_BOOTING),y)
ifeq ($(CONFIG_IRAM_IMAGE),y)
	$(CC) $(MARCH_FLAG) -D__ASSEMBLY__ -I../boot/include -fno-pic -mno-abicalls -Os -fomit-frame-pointer -fdata-sections -ffunction-sections -D__KERNEL__ $(EXTRA_CFLAGS) -c start_nand.S -o start_nand.o
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -Os  -fdata-sections -ffunction-sections -D__KERNEL__ $(EXTRA_CFLAGS) -c start_c.c -o start_c_nand.o 
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -Os  -fdata-sections -ffunction-sections -D__KERNEL__ $(EXTRA_CFLAGS) -c ddr/dram_init.c -o ddr/dram_init_nand.o 
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -Os -fdata-sections -ffunction-sections -D__KERNEL__ $(EXTRA_CFLAGS) -c ddr/efuse.c -o ddr/efuse_nand.o
ifeq ($(CONFIG_PARALLEL_NAND_FLASH),y)
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -Os  -fdata-sections -ffunction-sections -D__KERNEL__ $(EXTRA_CFLAGS) -c iram_parallel_nand.c -o iram_parallel_nand.o 
endif
ifeq ($(CONFIG_SPI_NAND_FLASH),y)
	$(CC) $(MARCH_FLAG) -g	-fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -Os  -fdata-sections -ffunction-sections -D__KERNEL__ $(EXTRA_CFLAGS) -c iram_spi_nand.c -o iram_spi_nand.o 
endif
endif
endif
	$(CC) $(MARCH_FLAG) -D__ASSEMBLY__ -I../boot/include -fno-pic -mno-abicalls -O2 -fomit-frame-pointer -D__KERNEL__ $(EXTRA_CFLAGS) -c cache_cpu.S -o cache_cpu.o 
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -D__KERNEL__ $(EXTRA_CFLAGS) -c start_c.c -o start_c.o 
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -D__KERNEL__ $(EXTRA_CFLAGS) -c ddr/dram_init.c -o ddr/dram_init.o 

ifeq ($(CONFIG_DRAM_BOOT_TEST),y)
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -D__KERNEL__ $(EXTRA_CFLAGS) -c ddr/dramtest.c -o ddr/dramtest.o
endif
	$(CC) $(MARCH_FLAG) -g  -fomit-frame-pointer -nostdinc -fno-pic -mno-abicalls -D__KERNEL__ $(EXTRA_CFLAGS) -c ddr/efuse.c -o ddr/efuse.o
ifeq ($(CONFIG_NONO_COMPRESS),y)
	$(CROSS)objcopy --add-section .initrd=boot.img start.o	# Compress
endif	
ifeq ($(CONFIG_LZMA_ENABLE),y)
	$(CROSS)objcopy --add-section .initrd=piggy.bin start.o	# Compress
endif
ifeq ($(CONFIG_GZIP_ENABLE),y)
	$(CROSS)objcopy --add-section .initrd=piggy.bin start.o	# Compress
endif
ifeq ($(CONFIG_NAND_FLASH_BOOTING),y)
ifeq ($(CONFIG_BOOT_IMAGE),y)
#	$(CROSS)ld -X -nostdlib -Tld.script_nand start.o cache_cpu.o start_c.o ddr/dram_init.o ddr/dramtest.o ddr/efuse.o -o boot.elf
	$(CROSS)ld -X -nostdlib -Tld.script_nand start.o cache_cpu.o start_c.o ddr/dram_init.o $(DRAM_BOOT_TEST_OUTPUT) ddr/efuse.o -o boot.elf
else
#	$(CROSS)ld -X -nostdlib -Tld.script_nand start.o cache_cpu.o start_c.o ddr/dram_init.o ddr/dramtest.o ddr/efuse.o -o boot.elf
	$(CROSS)ld -X -nostdlib -Tld.script_nand start.o cache_cpu.o start_c.o ddr/dram_init.o $(DRAM_BOOT_TEST_OUTPUT) ddr/efuse.o -o boot.elf
ifeq ($(CONFIG_PARALLEL_NAND_FLASH),y)
	$(CROSS)ld -X -nostdlib --gc-sections --strip-all -Tld.script_nand_iram start_nand.o start_c_nand.o ddr/dram_init_nand.o $(DRAM_BOOT_TEST_OUTPUT) ddr/efuse_nand.o  iram_parallel_nand.o -o boot_iram.elf		
else
	$(CROSS)ld -X -nostdlib --gc-sections --strip-all  -Tld.script_nand_iram start_nand.o start_c_nand.o ddr/dram_init_nand.o $(DRAM_BOOT_TEST_OUTPUT) ddr/efuse_nand.o  iram_spi_nand.o -o boot_iram.elf
endif
endif
else
ifeq ($(CONFIG_ROM_BOOT),y)
	$(CROSS)ld -X -nostdlib -Tld.script_rom start.o cache_cpu.o start_c.o ddr/dram_init.o $(DRAM_BOOT_TEST_OUTPUT) ddr/efuse.o -o boot.elf
else
#	$(CROSS)ld -X -nostdlib -Tld.script start.o cache_cpu.o start_c.o ddr/dram_init.o ddr/dramtest.o ddr/efuse.o -o boot.elf
	$(CROSS)ld -X -nostdlib -Tld.script start.o cache_cpu.o start_c.o ddr/dram_init.o $(DRAM_BOOT_TEST_OUTPUT) ddr/efuse.o -o boot.elf
endif
endif
#------------------------------------------------------------------------------------	
	$(CROSS)objcopy -Obinary boot.elf boot
ifeq ($(CONFIG_NAND_FLASH_BOOTING),y)
ifeq ($(CONFIG_IRAM_IMAGE),y)
	$(CROSS)objcopy -Obinary boot_iram.elf boot_iram
endif
endif
	cp boot ../boot/Output/boot -f 			#sync 
	$(CROSS)objdump -h -S   boot.elf > boot.elf.txt 
	$(CROSS)nm boot.elf  | sort > system.map
ifeq ($(CONFIG_NAND_FLASH_BOOTING),y)
ifeq ($(CONFIG_BOOT_IMAGE),y)
	./cvimg_nand boot boot boot.bin 0 0
else
	./cvimg_nand iram boot_iram boot_iram.bin 0 0
	./szcheck.sh boot_iram.bin 10240
	./cvimg_nand $(BOOT_SIGNATURE) boot boot_boot.bin 0 40000
	cat boot_iram.bin > boot.bin
	cat boot_boot.bin >> boot.bin
endif
else
	./cvimg boot boot boot.bin 0 0
endif
#	rm -rf rescue.bin
#	./cvimg boot piggy.bin rescue.bin 0 0	
	hexdump -v -e '4/1 "%02X"' -e '",\r\n"' boot > romcode.txt  #wei add
	cp boot.bin ../boot/Output/boot.bin -f 			#copy file to Output for easy use.

cvimg_nand:
ifeq ($(CONFIG_CUSTOMER_BOOT_SIGNATURE_SUPPORT),y)
	gcc -o cvimg_nand cvimg.c -DCUSTOMER_SIGNATURE_SUPPORT -DCUSTOMER_SIGNATURE=\"$(BOOT_SIGNATURE)\"
else
	gcc -o cvimg_nand cvimg.c
endif
#==================================================================================
clean:
	rm -f boot boot.elf *.txt system.map piggy.bin piggy.elf piggy.o nfjrom.elf nfjrom start_c.o ddr/dram_init.o ddr/dramtest.o cvimg_nand cvimg.o
ifeq ($(CONFIG_LZMA_ENABLE),y)
	rm -f LzmaDecode.o boot.img.gz *.img
endif
ifeq ($(CONFIG_GZIP_ENABLE),y)
	rm -f gunzip.o boot.img.gz *.img
endif	 
	rm -f bootload.o start.o cache_cpu.o boot.bin  boot.img.gz *.img 

nfjrom: bootload
	$(CROSS)ld -X -nostdlib -Tnfjrom.script start.o -o nfjrom.elf
	$(CROSS)objcopy -Obinary nfjrom.elf nfjrom
