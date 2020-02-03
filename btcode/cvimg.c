#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define CERT_HEADER			((char *)"cert")
#define BOOT_HEADER			((char *)"boot")
#define INIT_RAM_HEADER	    ((char *)"iram")
#define ALL_HEADER			((char *)"allp")
#define SIGNATURE_LEN		4
#define NAND_FLASH_PAGE_SIZE	2048

/***
 * README
 * cvimg SIG ROM_IMG OUTPUT_IMG 0 0
 * for example
 * ./cvimg iram init_ram.bin init_ram_bfe00010_hdr.bin 0 0
 * ./cvimg boot u-boot_a0500000.bin u-boot_a0500000_hdr.bin 0 0
 * ./cvimg boot u-boot_a0500010.bin u-boot_a0500010_hdr.bin 0 0
 * ./cvimg boot u-boot_b0000000.bin u-boot_b0000000_hdr.bin 0 0
 ***/



typedef struct img_header {
	unsigned char signature[SIGNATURE_LEN];
	unsigned int startAddr;
	unsigned int burnAddr;
	unsigned int len;
}IMG_HEADER_T, *IMG_HEADER_Tp;

unsigned short checksum_calc(unsigned short *addr, unsigned short count)
{
    unsigned long sum;

    sum = 0;
    while (count > 1) {
        sum += *addr++;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if (count)
        sum += *(unsigned char *)addr;

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum  = (sum & 0xffff) + (sum >> 16);

    return (unsigned short)(~sum);
}

unsigned short calculateChecksum(char *buf, int len)
{
	int i, j;
	unsigned short sum=0, tmp;

	j = (len/2)*2;

	for (i=0; i<j; i+=2) {
		tmp = *((unsigned short *)(buf + i));
		sum += tmp;
	}

	if ( len % 2 ) {
		tmp = buf[len-1];
		sum += tmp;
	}

    printf("sum: %d \n", sum);
    
	return ~sum+1;
}

unsigned short verifyChecksum(char *buf, int len)
{
	int i, j;
	unsigned short sum=0, tmp;

    printf("len: %d \n", len);

	j = (len/2)*2;

	for (i=0; i<j; i+=2) {
		tmp = *((unsigned short *)(buf + i));
		sum += tmp;
	}

    tmp = *((unsigned short *)(buf + i));

	if ( len % 2 ) {
		tmp = buf[len-1];
		sum += tmp;
	}
    printf("sum: %d \n", sum);
	return sum;
}

void printf_usage(void)
{
    printf("Version: 1.0\n");
    printf("Usage: cvimg <option> input-filename output-filename start-addr burn-addr [signature]\n");
    printf("<option>: root|linux|boot\n");
    printf("[signature]: user-specified signature (4 characters)\n");
}
		
int main(int argc, char** argv)
{
	char inFile[80]={0}, outFile[80]={0};
	int fh, size;
	struct stat status;
	char *buf;
	IMG_HEADER_Tp pHeader;
	unsigned int startAddr;
	unsigned int burnAddr;
	unsigned short checksum;
	unsigned int lchecksum, padding_len;
	unsigned int start_addr;	

	if ( argc != 6) {
	    printf_usage();
		exit(1);
	}

	sscanf(argv[2], "%s", inFile);
	sscanf(argv[3], "%s", outFile);

	sscanf(argv[4], "%x", &startAddr);
	sscanf(argv[5], "%x", &burnAddr);

	// check input file and allocate buffer
	if ( stat(inFile, &status) < 0 ) {
		printf("Can't stat file! [%s]\n", inFile );
		exit(1);
	}

	size = status.st_size + sizeof(IMG_HEADER_T) + sizeof(checksum);
	if (size%2)
		size++; // pad

	/* fix bug */
	if(((size % NAND_FLASH_PAGE_SIZE) <= sizeof(IMG_HEADER_T))
		&& ((size % NAND_FLASH_PAGE_SIZE) != 0)){
		/* last 16 bytes may not copy to ram/sram */
		size += sizeof(IMG_HEADER_T) + 2 - (size % NAND_FLASH_PAGE_SIZE);
	}

	buf = malloc(size);
	if (buf == NULL) {
		printf("Malloc buffer failed!\n");
		exit(1);
	}
	memset(buf, '\0', size);
	pHeader = (IMG_HEADER_Tp)buf;

	buf += sizeof(IMG_HEADER_T);

	// Read data and generate header
	fh = open(inFile, O_RDONLY);
	if ( fh == -1 ) {
		printf("Open input file error!\n");
		free( pHeader );
		exit(1);
	}
	lseek(fh, 0L, SEEK_SET);
	if ( read(fh, buf, status.st_size) != status.st_size) {
		printf("Read file error!\n");
		close(fh);
		free(pHeader);
		exit(1);
	}
	close(fh);

	if (!strcmp("boot", argv[1])) {
        memcpy(pHeader->signature, BOOT_HEADER, SIGNATURE_LEN);
	} else if (!strcmp("iram", argv[1])) {
    	memcpy(pHeader->signature, INIT_RAM_HEADER, SIGNATURE_LEN);
	}
#ifdef CUSTOMER_SIGNATURE_SUPPORT
	else if (!strcmp(CUSTOMER_SIGNATURE, argv[1])) {
    	memcpy(pHeader->signature, CUSTOMER_SIGNATURE, SIGNATURE_LEN);
    } 
#endif
	else {
		printf("not supported signature\n");
		exit(1);
	}
	pHeader->len = size-sizeof(IMG_HEADER_T);
	pHeader->startAddr = startAddr;
	pHeader->burnAddr = burnAddr;

    printf("%x, %x \n", *buf, *(buf+1));
	checksum = calculateChecksum(buf, status.st_size);
       
	*((unsigned short *)&buf[size-sizeof(IMG_HEADER_T)-sizeof(checksum)]) = checksum;
	
	// Write image to output file
	fh = open(outFile, O_RDWR|O_CREAT|O_TRUNC);
	if ( fh == -1 ) {
		printf("Create output file error! [%s]\n", outFile);
		free(pHeader);
		exit(1);
	}
	write(fh, pHeader, size);
	close(fh);
	chmod(outFile, DEFFILEMODE);
	
	printf("Generate image successfully, length=%d, checksum=0x%x\n", pHeader->len, checksum);
    
	free(pHeader);

    /////////////////////////////////////////////
    printf("verify checksum\n");

    char *outbuf;
    unsigned short chksum;
    
    if ( stat(outFile, &status) < 0 ) {
        printf("Can't stat file! [%s]\n", outFile );
        exit(1);
    }

    size = status.st_size;
    outbuf = malloc(size);
	if (outbuf == NULL) {
		printf("Malloc buffer failed!\n");
		exit(1);
	}
	memset(outbuf, '\0', size);

	fh = open(outFile, O_RDONLY);   
	if ( fh == -1 ) {
		printf("Open output file error!\n");
		exit(1);
	}
	lseek(fh, 0L, SEEK_SET);
    read(fh, outbuf, status.st_size+sizeof(IMG_HEADER_T));

    outbuf += sizeof(IMG_HEADER_T);
    printf("size:%d\n", size);
    printf("%x, %x, %x, %x \n", *outbuf, *(outbuf+1), *(outbuf+2), *(outbuf+3));
    chksum = verifyChecksum(outbuf, size-sizeof(IMG_HEADER_T));
    printf("chksum:%d\n", chksum);
    
	return 0;
}

