
//#include <common.h>
//#include <command.h>

//#include <basic_types.h>
//#include "cmd_monitor.h"

//#include <malloc.h>

#include <rom_def.h>
//#include <string.h>
#define Strtoul simple_strtoul

//#define dram_test_printf    printf
#define dram_test_printf(fmt, args...)

#undef printf
//#define printf    btprintf
#define printf(fmt, args...)
//#include "SPIC_deliver/spi_flash/spi_flash_rtk.h"
//#include "spi_flash/DW_common.h"
//#include "spi_flash/spi_flash_public.h"
//#include "spi_flash/spi_flash_private.h"

#if 1   //DRAM test
typedef unsigned int uint32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef int int32;
typedef char int8;
typedef short int16;

//#define HANDLE_FAIL while(1);

#define HANDLE_FAIL return 1
#define HANDLE_PASS return 0

#ifndef FALSE
#define _FALSE	0
#else
#define _FALSE	FALSE
#endif

#if 0
unsigned int dram_patterns[] = {

  0x00000000,
  0xffffffff,
  0x55555555,
  0xaaaaaaaa,
  0x01234567,
  0x76543210,
  0x89abcdef,
  0xfedcba98,
  0xA5A5A5A5,
  0x5A5A5A5A,
  0x5A5AA5A5,
  0xA5A55A5A,
};
#else
const unsigned int dram_patterns[] = {
  0x01234567,
  0xfedcba98,
  0xA5A5A5A5,
  0x5A5A5A5A,
  0x5A5AA5A5,
  0xA5A55A5A,
};

#endif

//---------------------------------------------------------------------------
// michael
void * memset(void * s,int c, unsigned int  count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

void * memcpy(void * dest,const void *src,unsigned int count)
{
	char *tmp = (char *) dest, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;

	return dest;
}
//---------------------------------------------------------------------------

#ifdef READ_ONLY
int
dram_normal_patterns_read_only(uint32 dram_start, uint32 dram_size, uint32 area_size
)
{
  int i;
  int j;
  uint32 start_value;
  volatile uint32 *read_start;
  volatile uint32 *start;

  // printf("dram_start=0x%x\r\n",dram_start);
//      printf("dram_size=0x%x\r\n",dram_size);
//      printf("area_size=0x%x\r\n",area_size);



  for (i = 0; i < (sizeof(dram_patterns) / sizeof(uint32)); i++)
    //for (i=1; i < 2; i++)
  {
    //_cache_flush();

    /* write pattern */
    start = (uint32 *) (dram_start + (i * dram_size) % (area_size));
    // read_start = (uint32 *)(TO_UNCACHED_ADDR((uint32)start+dram_size-4));

    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*start);
		if((j % 16) == 0)
			printf("\n0x%x : ",(unsigned int)start );
	printf("%x    ", start_value);
      start++;
    }
    //printf("pattern[%d](0x%x) 0x%x pass\n\n", i, dram_patterns[i], start);
  }
  return 0;
}
#endif

#if 0  // michael
static int do_dram_read(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[]) {
#if 1                           //CONFIG_AVOID_ERROR_INPUT_HANG
  if ((argv[1]
      ) && (argv[2]
      ) ) {

  }
  else {
    printf("Wrong argument number!\r\n");
    printf("Usage:dramtest <dram_start><dram_size> in hex\r\n");

    return CMD_RET_FAILURE;
  }
#endif


  u32 dram_start;
  u32 dram_size;



  dram_start = Strtoul((const char *) (argv[1]), (char **) NULL, 16);
  dram_size = Strtoul((const char *) (argv[2]), (char **) NULL, 16);


  /*Avoid error input format hang */
  if (dram_size == 0) {
    printf("area_size can't be 0 , instead of 0x10  \r\n");     //Divide by zero will cause exception
    dram_size = 0x10;

  }

   dram_normal_patterns_read_only(dram_start, dram_size, dram_size);

    return 0;
}


U_BOOT_CMD(dramr, 3, 0, do_dram_read,
           "DramTest <dram_start><dram_size>\n", "");
#endif

#ifdef ONLY_WRITE
int
dram_normal_patterns_write_only(uint32 dram_start, uint32 dram_size, uint32 area_size
)
{
  int i;
  int j;
  uint32 start_value;
  volatile uint32 *read_start;
  volatile uint32 *start;

  // printf("dram_start=0x%x\r\n",dram_start);
//      printf("dram_size=0x%x\r\n",dram_size);
//      printf("area_size=0x%x\r\n",area_size);


while(1)
{
  // printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < (sizeof(dram_patterns) / sizeof(uint32)); i++)
  {
    //_cache_flush();

    /* write pattern */
    start = (uint32 *) (dram_start + (i * dram_size) % (area_size));

    for (j = 0; j < dram_size; j = j + 4) {
      *start = dram_patterns[i];
      start++;
	 if( (j%0x100000) == 0)
	 	printf("~");
	 	
    }

  }
}
  return 0;
}
#endif

#if 0  // michael
static int do_dram_write(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[]) {
#if 1                           //CONFIG_AVOID_ERROR_INPUT_HANG
  if ((argv[1]
      ) && (argv[2]
      ) ) {

  }
  else {
    printf("Wrong argument number!\r\n");
    printf("Usage:dramtest <dram_start><dram_size> in hex\r\n");

    return CMD_RET_FAILURE;
  }
#endif


  u32 dram_start;
  u32 dram_size;



  dram_start = Strtoul((const char *) (argv[1]), (char **) NULL, 16);
  dram_size = Strtoul((const char *) (argv[2]), (char **) NULL, 16);


  /*Avoid error input format hang */
  if (dram_size == 0) {
    printf("area_size can't be 0 , instead of 0x10  \r\n");     //Divide by zero will cause exception
    dram_size = 0x10;

  }

   dram_normal_patterns_write_only(dram_start, dram_size, dram_size);

    return 0;
}


U_BOOT_CMD(dramw, 3, 0, do_dram_write,
           "DramTest <dram_start><dram_size>\n", "");
#endif

int
dram_normal_patterns(uint32 dram_start, uint32 dram_size, uint32 area_size
)
{
  uint32 i;
  uint32 j;
  uint32 start_value;
  volatile uint32 *read_start;
  volatile uint32 *start;

  // printf("dram_start=0x%x\r\n",dram_start);
//      printf("dram_size=0x%x\r\n",dram_size);
//      printf("area_size=0x%x\r\n",area_size);



  // printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < (sizeof(dram_patterns) / sizeof(uint32)); i++)
    //for (i=1; i < 2; i++)
  {
    //_cache_flush();

    /* write pattern */
    start = (uint32 *) (dram_start + (i * dram_size) % (area_size));
    // read_start = (uint32 *)(TO_UNCACHED_ADDR((uint32)start+dram_size-4));
    read_start = (uint32 *) (((uint32) start + dram_size - 4));

    for (j = 0; j < dram_size; j = j + 4) {
      *start = dram_patterns[i];
      start++;
    }

    //_cache_flush();

    /* check data */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != dram_patterns[i]) {
        printf("addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
        read_start, start_value, dram_patterns[i], __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start--;
    }
    //printf("pattern[%d](0x%x) 0x%x pass\n\n", i, dram_patterns[i], start);
  }
  //   printf("%s test completed.\n\n", __FUNCTION__);

  HANDLE_PASS;
test_fail:
  // printf("%s test fails.\n", __FUNCTION__);
  HANDLE_FAIL;
}

int dram_addr_rot(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  uint32 i;
  uint32 j;
  uint32 start_value;
  uint32 read_start_addr;
  volatile uint32 *start;
  volatile uint32 *read_start;

  printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 32; i = i + 4)
    //for (i=0; i < 4; i=i+4)
  {

    //_cache_flush();
    /* write pattern */
    start = (uint32 *) (dram_start + ((i / 4) * dram_size) % (area_size));
    read_start = (uint32 *) ((uint32) start + dram_size - 4);
    read_start_addr = ((uint32) start);
    //start = (uint32 *)(TO_UNCACHED_ADDR(((uint32)start)));
    start = (uint32 *) (((uint32) start));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = (read_start_addr << i);
      start++;
      read_start_addr = read_start_addr + 4;
    }

    //_cache_flush();
    read_start_addr = ((uint32) read_start);
    /* check data reversing order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != ((read_start_addr) << i)) {
        printf("decr addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
               read_start, start_value, ((read_start_addr) << i),
               __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr - 4;
      read_start--;
    }

    read_start_addr += 4;
    read_start++;

    /* check data sequential order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != ((read_start_addr) << i)) {
        printf("seq addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
               read_start, start_value, ((read_start_addr) << i),
               __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr + 4;
      read_start++;
    }

    printf("rotate %d 0x%x passed\r\n", i, (uint32)start);
  }
  printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n\n", __FUNCTION__);
  HANDLE_FAIL;
}

int dram_addr_rot_read_repeat(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  uint32 i;
  uint32 j;
  uint32 k;
  uint32 start_value;
  uint32 read_start_addr;
  volatile uint32 *start;
  volatile uint32 *read_start, read_start_repeat;

  dram_test_printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 4; i = i + 4)
    //for (i=0; i < 4; i=i+4)
  {

    //_cache_flush();
    /* write pattern */
    start = (uint32 *) (dram_start + ((i / 4) * dram_size) % (area_size));
    read_start = (uint32 *) ((uint32) start + dram_size - 4);
    read_start_repeat = read_start;
    read_start_addr = ((uint32) start);
    //start = (uint32 *)(TO_UNCACHED_ADDR(((uint32)start)));
    start = (uint32 *) (((uint32) start));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = (read_start_addr << i);
      start++;
      read_start_addr = read_start_addr + 4;
    }
	for(k = 0; k < 100; k++)
	{
	    read_start = read_start_repeat;
	    //_cache_flush();
	    read_start_addr = ((uint32) read_start);
	    /* check data reversing order */
	    for (j = 0; j < dram_size; j = j + 4) {
	      start_value = (*read_start);
	      if (start_value != ((read_start_addr) << i)) {
	        dram_test_printf("decr addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
	               read_start, start_value, ((read_start_addr) << i),
	               __FUNCTION__, __LINE__);
	        goto test_fail;
	      }
	      read_start_addr = read_start_addr - 4;
	      read_start--;
	    }

	    read_start_addr += 4;
	    read_start++;

	    /* check data sequential order */
	    for (j = 0; j < dram_size; j = j + 4) {
	      start_value = (*read_start);
	      if (start_value != ((read_start_addr) << i)) {
	        dram_test_printf("seq addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
	               read_start, start_value, ((read_start_addr) << i),
	               __FUNCTION__, __LINE__);
	        goto test_fail;
	      }
	      read_start_addr = read_start_addr + 4;
	      read_start++;
	    }

	    dram_test_printf("rotate %d 0x%x passed\r\n", i, start);
	  }
  }
  dram_test_printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  dram_test_printf("%s test fails.\n\n", __FUNCTION__);
  HANDLE_FAIL;
}

#if 0
int dram_com_addr_rot(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  int i;
  int j;
  uint32 start_value;
  uint32 read_start_addr;
  volatile uint32 *start;
  volatile uint32 *read_start;

  printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 32; i = i + 4)
    //for (i=0; i < 4; i=i+4)
  {

    //_cache_flush();
    /* write pattern */
    start = (uint32 *) (dram_start + ((i / 4) * dram_size) % (area_size));
    read_start = (uint32 *) ((uint32) start + dram_size - 4);
    read_start_addr = ((uint32) start);
    //start = (uint32 *)(TO_UNCACHED_ADDR(((uint32)start)));
    start = (uint32 *) (((uint32) start));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = ~(read_start_addr << i);
      start++;
      read_start_addr = read_start_addr + 4;
    }

    //_cache_flush();
    read_start_addr = ((uint32) read_start);
    /* check data reversing order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != (~(read_start_addr << i))) {
        printf("decr addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n\n",
               read_start, start_value, ~((read_start_addr) << i),
               __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr - 4;
      read_start--;
    }

    read_start_addr += 4;
    read_start++;

    /* check data sequential order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != (~(read_start_addr << i))) {
        printf("seq addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n\n",
               read_start, start_value, ~((read_start_addr) << i),
               __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr + 4;
      read_start++;
    }

    printf("~rotate %d 0x%x passed\r\n", i, start);
  }
  printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n\n", __FUNCTION__);
  HANDLE_FAIL;
}


#endif

#if 1   //comment for code size
int dram_byte_access(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  uint32 i;
  uint32 j;
  uint8 byte;
  uint32 start_value;
  volatile uint8 *start_b;
  volatile uint32 *start_w;

  printf("JSW 20141208:=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < (sizeof(dram_patterns) / sizeof(uint32)); i++) {

    //_cache_flush();

    /* write byte pattern */
    //start_w = (uint32 *)(TO_UNCACHED_ADDR(dram_start+(i*dram_size)%(area_size)+dram_size-4));
    start_w =
        (uint32 *) (dram_start + (i * dram_size) % (area_size) + dram_size -
                    4);
    start_b = (uint8 *) (dram_start + (i * dram_size) % (area_size));
    for (j = 0; j < dram_size; j = j + 4) {
#if 0   //fail =>for big endian //pattern= (MSB) 0x01234567 (LSB)
      byte = (uint8) (dram_patterns[i]);        //0x67
      *(start_b + 3) = byte;
      byte = (uint8) (dram_patterns[i] >> 8);
      *(start_b + 2) = byte;
      byte = (uint8) (dram_patterns[i] >> 16);
      *(start_b + 1) = byte;
      byte = (uint8) (dram_patterns[i] >> 24);
      *(start_b) = byte;
#else //pass ARM =>little endian
      byte = (uint8) (dram_patterns[i] >> 24);
      *(start_b + 3) = byte;
      byte = (uint8) (dram_patterns[i] >> 16);
      *(start_b + 2) = byte;
      byte = (uint8) (dram_patterns[i] >> 8);
      *(start_b + 1) = byte;
      byte = (uint8) (dram_patterns[i]);
      *(start_b) = byte;
#endif
      start_b += 4;
    }

    //_cache_flush();

    /* read word and check data */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = *start_w;
      if (start_value != dram_patterns[i]) {
        printf("JSW 20141208:addr:0x%x(0x%x) != pattern(0x%x) %s, %d\n",
               start_w, start_value, dram_patterns[i], __FUNCTION__,
               __LINE__);
        goto test_fail;
      }
      start_w--;
    }
    printf("JSW 20141208:pattern[%d](0x%x) 0x%x pass\r\n", i,
           dram_patterns[i], start_b);
  }
  printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n\n", __FUNCTION__);
  HANDLE_FAIL;
}


/*
 * write two half-words and read word.
 */
int dram_half_word_access(uint32 dram_start, uint32 dram_size,
                          uint32 area_size) {
  uint32 i;
  uint32 j;
  uint16 h_word;
  uint32 start_value;
  volatile uint16 *start_h;
  volatile uint32 *start_w;

  printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < (sizeof(dram_patterns) / sizeof(uint32)); i++) {
    //_cache_flush();

    /* write half-word pattern */
    start_h = (uint16 *) (dram_start + (i * dram_size) % (area_size));
    //start_w = (uint32 *)(TO_UNCACHED_ADDR(dram_start+ (i*dram_size)%(area_size)+dram_size-4));
    start_w =
        (uint32 *) (dram_start + (i * dram_size) % (area_size) + dram_size -
                    4);
    for (j = 0; j < dram_size; j = j + 4) {
#if 0
      h_word = (uint16) (dram_patterns[i]);
      *(start_h + 1) = h_word;
      h_word = (uint16) (dram_patterns[i] >> 16);
      *start_h = h_word;
      start_h += 2;
#else
      h_word = (uint16) (dram_patterns[i] >> 16);
      *(start_h + 1) = h_word;
      h_word = (uint16) (dram_patterns[i]);
      *start_h = h_word;
      start_h += 2;

#endif
    }

    //_cache_flush();

    /* read word and check data */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*start_w);
      if (start_value != dram_patterns[i]) {
        printf("addr:0x%x(0x%x) != pattern(0x%x) %s, %d\n",
               start_w, start_value, dram_patterns[i], __FUNCTION__,
               __LINE__);
        goto test_fail;
      }
      start_w--;
    }
    printf("pattern[%d](0x%x) 0x%x pass\r\n", i, dram_patterns[i], (uint32)start_h);
  }
  printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n\n", __FUNCTION__);
  HANDLE_FAIL;

}




int dram_walking_of_1(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  int i;
  uint32 j;
  uint32 walk_pattern;
  uint32 start_value;
  volatile uint32 *read_start;
  volatile uint32 *start;

  printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 32; i++) {

    //_cache_flush();

    /* generate pattern */
    walk_pattern = (1 << i);

    /* write pattern */
    start = (uint32 *) (dram_start + (i * dram_size) % (area_size));
    //read_start = (uint32 *)(TO_UNCACHED_ADDR((uint32)start+dram_size-4));
    read_start = (uint32 *) (((uint32) start + dram_size - 4));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = walk_pattern;
      start++;
    }

    //_cache_flush();

    /* check data */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != walk_pattern) {
        printf("addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
               read_start, start_value, walk_pattern, __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start--;
    }
    printf("pattern[%d](0x%x) 0x%x passed\r\n", i, walk_pattern, (uint32)start);
  }
  printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n", __FUNCTION__);
  HANDLE_FAIL;
}



int dram_walking_of_0(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  int i;
  uint32 j;
  uint32 start_value;
  uint32 walk_pattern;
  volatile uint32 *start;
  volatile uint32 *read_start;

  printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 32; i++) {

    //_cache_flush();

    /* generate pattern */
    walk_pattern = ~(1 << i);

    /* write pattern */
    start = (uint32 *) (dram_start + (i * dram_size) % (area_size));
    // read_start = (uint32 *)(TO_UNCACHED_ADDR(((uint32)start)+dram_size-4));
    read_start = (uint32 *) (((uint32) start + dram_size - 4));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = walk_pattern;
      start++;
    }

    //_cache_flush();

    /* check data */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != walk_pattern) {
        printf("addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
               read_start, start_value, walk_pattern, __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start--;
    }
    printf("pattern[%d](0x%x) 0x%x passed\r\n", i, walk_pattern, (uint32)start);
  }
  printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n", __FUNCTION__);
  HANDLE_FAIL;
}


#pragma pack(1)
/* Data structures used for testing unaligned load/store operations. */
typedef struct {
  unsigned char c1;
  unsigned int w;
} t_off_1;

typedef struct {
  unsigned char c1;
  unsigned char c2;
  unsigned int w;
} t_off_2;
typedef struct {
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  unsigned int w;
} t_off_3;

#pragma pack(4)
int unaligned_test(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  unsigned int i, j;
  volatile t_off_1 *off1;
  volatile t_off_2 *off2;
  volatile t_off_3 *off3;

  off1 = (t_off_1 *) dram_start;
  off2 = (t_off_2 *) dram_start;
  off3 = (t_off_3 *) dram_start;


  /* patterns loop */
  for (j = 0; j < sizeof(dram_patterns) / sizeof(unsigned int); j++) {
    /* offset 1 bytes */
    off1 = (t_off_1 *) (dram_start + (j * dram_size) % (area_size));
    /* set value */
    for (i = 0; i < (dram_size / sizeof(t_off_1)); i++) {
      off1[i].c1 = 0xcc;
      off1[i].w = dram_patterns[j];
    }
    //_cache_flush();
    /* check data */
    for (i = 0; i < (dram_size / sizeof(t_off_1)); i++) {
      if ((off1[i].w != dram_patterns[j]) || (off1[i].c1 != 0xcc)) {
        printf("%s(%d) ", __FUNCTION__, __LINE__);
        printf("offset 1 error:addr(0x%x) write 0x%x, read 0x%x, c1(%02x)\n",
               &off1[i], dram_patterns[j], off1[i].w, off1[i].c1);
        goto test_fail;
      }
    }


    /* offset 2 bytes */
    off2 = (t_off_2 *) (dram_start + (j * dram_size) % (area_size));
    /* set value */
    for (i = 0; i < (dram_size / sizeof(t_off_2)); i++) {
      off2[i].c1 = 0xcc;
      off2[i].c2 = 0xdd;
      off2[i].w = dram_patterns[j];
    }
    //_cache_flush();
    /* check data */
    for (i = 0; i < (dram_size / sizeof(t_off_2)); i++) {
      if (off2[i].w != dram_patterns[j] || (off2[i].c1 != 0xcc)
          || (off2[i].c2 != 0xdd)) {
        printf("%s(%d) ", __FUNCTION__, __LINE__);
        printf
            ("offset 2 error:addr(0x%x) write 0x%x, read 0x%x, c1(0x%x), c2(0x%x)\n",
             &off2[i], dram_patterns[j], off2[i].w, off2[i].c1, off2[i].c2);
        printf("&dram_pattern[%d](0x%p) = 0x%x\n", j, &dram_patterns[j],
               dram_patterns[j]);
        goto test_fail;
      }
    }

    /* offset 3 bytes */
    off3 = (t_off_3 *) (dram_start + (j * dram_size) % (area_size));
    /* set value */
    for (i = 0; i < (dram_size / sizeof(t_off_3)); i++) {
      off3[i].c1 = 0xcc;
      off3[i].c2 = 0xdd;
      off3[i].c3 = 0xee;
      off3[i].w = dram_patterns[j];
    }
    //_cache_flush();
    /* check data */
    for (i = 0; i < (dram_size / sizeof(t_off_3)); i++) {
      if (off3[i].w != dram_patterns[j] || (off3[i].c1 != 0xcc)
          || (off3[i].c2 != 0xdd)
          || (off3[i].c3 != 0xee)) {
        printf("%s(%d) ", __FUNCTION__, __LINE__);
        printf("offset 3 error:addr(0x%x) write 0x%x, "
               "read 0x%x, c1(%02x), c2(%02x), c3(%02x)\n",
               &off1[i], dram_patterns[j], off3[i].w, off3[i].c1,
               off3[i].c2, off3[i].c3);
        goto test_fail;
      }
    }
    printf("pattern[%d](0x%x) 0x%x pass\r\n", j, dram_patterns[j], (uint32)off3);

  }
  printf("%s test passed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test failed.\n\n", __FUNCTION__);
  HANDLE_FAIL;

}



int dram_com_addr_rot(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  int i;
  uint32 j;
  uint32 start_value;
  uint32 read_start_addr;
  volatile uint32 *start;
  volatile uint32 *read_start;

  printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 32; i = i + 4)
    //for (i=0; i < 4; i=i+4)
  {

    //_cache_flush();
    /* write pattern */
    start = (uint32 *) (dram_start + ((i / 4) * dram_size) % (area_size));
    read_start = (uint32 *) ((uint32) start + dram_size - 4);
    read_start_addr = ((uint32) start);
    //start = (uint32 *)(TO_UNCACHED_ADDR(((uint32)start)));
    start = (uint32 *) (((uint32) start));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = ~(read_start_addr << i);
      start++;
      read_start_addr = read_start_addr + 4;
    }

    //_cache_flush();
    read_start_addr = ((uint32) read_start);
    /* check data reversing order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != (~(read_start_addr << i))) {
        printf("decr addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n\n",
               read_start, start_value, ~((read_start_addr) << i),
               __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr - 4;
      read_start--;
    }

    read_start_addr += 4;
    read_start++;

    /* check data sequential order */
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != (~(read_start_addr << i))) {
        printf("seq addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n\n",
               read_start, start_value, ~((read_start_addr) << i),
               __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start_addr = read_start_addr + 4;
      read_start++;
    }

    printf("~rotate %d 0x%x passed\r\n", i, (uint32)start);
  }
  printf("%s test completed.\r\n", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n\n", __FUNCTION__);
  HANDLE_FAIL;
}


#endif

int memcpy_test(uint32 dram_start, uint32 dram_size, uint32 area_size) {
  int i;
  uint32 j;
  uint32 start_value;
  uint32 read_start_addr;
  volatile uint32 *start;
  volatile uint32 *read_start;

  printf("=======start %s test=======\r\n", __FUNCTION__);
  for (i = 0; i < 32; i = i + 4) {

    //     _cache_flush();

    /* write pattern */
    start = (uint32 *) (dram_start + (i * dram_size) % (area_size));
    read_start_addr = ((uint32) start + (dram_size - 4));
    // read_start = (uint32 *)(TO_UNCACHED_ADDR(((uint32)start)+dram_size-4));
    read_start = (uint32 *) (((uint32) start + dram_size - 4));
    for (j = 0; j < dram_size; j = j + 4) {
      *start = ((uint32) start << i);
      start++;
    }


    memcpy((char *) (dram_start + dram_size),
           (char *) (dram_start + (i * dram_size) % (area_size)), dram_size);

    //_cache_flush();

    /* check uncached data */
    read_start = (uint32 *) (dram_start + dram_size + dram_size - 4);
    for (j = 0; j < dram_size; j = j + 4) {
      start_value = (*read_start);
      if (start_value != ((read_start_addr) << i)) {
        printf("addr(0x%x): 0x%x != pattern(0x%x) %s, %d\n",
               read_start, start_value, ((read_start_addr) << i),
               __FUNCTION__, __LINE__);
        goto test_fail;
      }
      read_start = read_start - 1;
      read_start_addr = read_start_addr - 4;
    }
    printf("\nmemcpy %d 0x%x passed\r\n ", i, (uint32)start);
  }
  printf("%s test completed.\r\n\n  ", __FUNCTION__);
  HANDLE_PASS;
test_fail:
  printf("%s test fails.\n\n ", __FUNCTION__);
  HANDLE_FAIL;
}



#if 0 // michael
static int do_dram_test(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[]) {
#if 1                           //CONFIG_AVOID_ERROR_INPUT_HANG
  if ((argv[1]
      ) && (argv[2]
      ) && (argv[3]
      )) {

  }
  else {
    printf("Wrong argument number!\r\n");
    printf("Usage:dramtest <dram_start><dram_size><area_size> in hex\r\n");

    return CMD_RET_FAILURE;
  }
#endif


  u32 dram_start;
  u32 dram_size, area_size;



  dram_start = Strtoul((const u8 *) (argv[1]), (u8 **) NULL, 16);
  dram_size = Strtoul((const u8 *) (argv[2]), (u8 **) NULL, 16);
  area_size = Strtoul((const u8 *) (argv[3]), (u8 **) NULL, 16);


  /*Avoid error input format hang */
  if (area_size == 0) {
    printf("area_size can't be 0 , instead of 0x10  \r\n");     //Divide by zero will cause exception
    area_size = 0x10;

  }

   if(dram_normal_patterns(dram_start, dram_size, area_size))
   {
	  printf("memory dram_normal_patterns failed\n");
	  return -1;//fail
   }
  else
	  printf("memory dram_normal_patterns OK\n");
  
  if(dram_addr_rot(dram_start, dram_size, area_size))
  {
	  printf("memory dram_addr_rot failed\n");   
	  return -1;//fail
  }
  else
	  printf("memory dram_addr_rot OK\n");   
  
  if(dram_com_addr_rot(dram_start, dram_size, area_size))
  {
	  printf("memory dram_com_addr_rot failed\n");
	  return -1;//fail
  }
  else
	  printf("memory dram_com_addr_rot OK\n");
  
  //comment for code size 
  if(dram_byte_access(dram_start, dram_size, area_size))
  {
	  printf("memory dram_byte_access failed\n");
	  return -1;//fail
  }
  else
	  printf("full memory dram_byte_access OK\n");
  
  if(dram_half_word_access(dram_start, dram_size, area_size))
  {
	  printf("memory dram_half_word_access failed\n");
	  return -1;//fail
  }
  else
	  printf("memory dram_half_word_access OK\n"); 	  
	  
  if(dram_walking_of_1(dram_start, dram_size, area_size))
  {
	  printf("memory dram_walking_of_1 failed\n");
	  return -1;//fail
  }
  else
	  printf("memory dram_walking_of_1 OK\n");
  
   if(dram_walking_of_0(dram_start, dram_size, area_size))
   {
	  printf("memory dram_walking_of_0 failed\n");
	  return -1;//fail
   }
  else
	  printf("memory dram_walking_of_0 OK\n");   
	  
   if(memcpy_test(dram_start, dram_size, area_size))
   {
	   printf("memory memcpy_test failed\n");
	  return -1;//fail
   }
   else
	   printf("memory memcpy_test OK\n");


  //unaligned_test( dram_start, dram_size, area_size);//8198E fail

    return 0;
}


U_BOOT_CMD(dramtest, 4, 0, do_dram_test,
           "DramTest <dram_start><dram_size><area_size>\n", "");
#endif

#if 0

struct cmd_tbl_s {

  char *name;                   /* Command Name?是命令名字，不需要用?引?括起? */

  int maxargs;                  /* maximum number of arguments最大????      */

  int repeatable;               /* autorepeat allowed?               命令是否可重复，就是?下一次按回??再?行     */

  /*Implementation function      */

  int (*cmd) (structcmd_tbl_s *, int, int, char *const[]);      //??的?行函?

  char *usage;                  /* Usage message          (short)字符串?尖的?短?明             */

#ifdef    CONFIG_SYS_LONGHELP

  char *help;                   /* Help  message            (long)　?助   */

#endif

#define U_BOOT_CMD_MKENT_COMPLETE(_name, _maxargs, _rep, _cmd,		\
				_usage, _help, _comp)
#endif


#endif

