#include <linux/types.h>

typedef struct bootm_headers{
	unsigned long		ep;	
	unsigned long		initrd_start;
	unsigned long		initrd_end;
} bootm_headers_t;


#define	LINUX_MAX_ENVS		256
#define	LINUX_MAX_ARGS		256

//#define UNCACHED_SDRAM(a) (((unsigned long)(a)) | 0x20000000)
#define UNCACHED_SDRAM(a)	(a)	//can only use cache addr

static int linux_argc;
static char **linux_argv;
static char *linux_argp;

static char **linux_env;
static char *linux_env_p;
static int linux_env_idx;

static bootm_headers_t boot_image;
static unsigned int enable_initrd = 0;

#ifdef CONFIG_PARAM_ADDR
#define PARAM_ADDR	CONFIG_PARAM_ADDR
#else
#define PARAM_ADDR	0x81f00000	
#endif

#define DEBUG
#ifdef DEBUG
#define debug(fmt,args...)	prom_printf(fmt ,##args)
#else
#define debug(fmt,args...)
#endif

#define REG32(reg)		(*(volatile unsigned int   *)(reg))

#define mips_boot_malta	0

static void linux_cmdline_init(void)
{
	linux_argc = 1;
	linux_argv = (char **)UNCACHED_SDRAM(PARAM_ADDR);
	linux_argv[0] = 0;
	linux_argp = (char *)(linux_argv + LINUX_MAX_ARGS);
}

static void linux_cmdline_set(const char *value, size_t len)
{
	linux_argv[linux_argc] = linux_argp;
	memcpy(linux_argp, value, len);
	linux_argp[len] = 0;

	linux_argp += len + 1;
	linux_argc++;
}

static void linux_cmdline_dump(void)
{
	int i;

	debug("## cmdline argv at 0x%p, argp at 0x%p\n",
	      linux_argv, linux_argp);

	for (i = 1; i < linux_argc; i++)
		debug("   arg %03d: %s\n", i, linux_argv[i]);
}

static void boot_cmdline_linux(bootm_headers_t *images)
{
	const char *bootargs, *next, *quote;

	linux_cmdline_init();


#if 0
	bootargs = getenv("bootargs");
	if (!bootargs)
		return;
#else
	bootargs = CONFIG_PARAM_CMDLINE;
#endif

	next = bootargs;

	while (bootargs && *bootargs && linux_argc < LINUX_MAX_ARGS) {
		quote = strchr(bootargs, '"');
		next = strchr(bootargs, ' ');

		while (next && quote && quote < next) {
			/*
			 * we found a left quote before the next blank
			 * now we have to find the matching right quote
			 */
			next = strchr(quote + 1, '"');
			if (next) {
				quote = strchr(next + 1, '"');
				next = strchr(next + 1, ' ');
			}
		}

		if (!next)
			next = bootargs + strlen(bootargs);

		linux_cmdline_set(bootargs, next - bootargs);

		if (*next)
			next++;

		bootargs = next;
	}

	linux_cmdline_dump();
}

static void linux_env_dump(void)
{
	int i;

	debug("## env argv at 0x%p, argp at 0x%p\n",
	      linux_env_idx, linux_env);

	for (i = 0; i < linux_env_idx; i++)
		debug("   arg %03d: %s\n", i, linux_env[i]);
	
}

static void linux_env_init(void)
{
	linux_env = (char **)(((ulong) linux_argp + 15) & ~15);
	linux_env[0] = 0;
	linux_env_p = (char *)(linux_env + LINUX_MAX_ENVS);
	linux_env_idx = 0;
}

static void linux_env_set(const char *env_name, const char *env_val)
{
	if (linux_env_idx < LINUX_MAX_ENVS - 1) {
		linux_env[linux_env_idx] = linux_env_p;

		strcpy(linux_env_p, env_name);
		linux_env_p += strlen(env_name);

		if (mips_boot_malta) {
			linux_env_p++;
			linux_env[++linux_env_idx] = linux_env_p;
		} else {
			*linux_env_p++ = '=';
		}

		strcpy(linux_env_p, env_val);
		linux_env_p += strlen(env_val);

		linux_env_p++;
		linux_env[++linux_env_idx] = 0;
	}
}

static void boot_prep_linux(bootm_headers_t *images)
{
	char env_buf[12];
	const char *cp;
	ulong rd_start, rd_size;

#if 0
#ifdef CONFIG_MEMSIZE_IN_BYTES
	sprintf(env_buf, "%lu", (ulong)gd->ram_size);
	debug("## Giving linux memsize in bytes, %lu\n", (ulong)gd->ram_size);
#else
	sprintf(env_buf, "%lu", (ulong)(gd->ram_size >> 20));
	debug("## Giving linux memsize in MB, %lu\n",
	      (ulong)(gd->ram_size >> 20));
#endif /* CONFIG_MEMSIZE_IN_BYTES */
#else
	SprintF(env_buf, "%d", (ulong)(REG32(0xB8000F00) << 20));
#endif

	if(images != NULL){
		rd_start = UNCACHED_SDRAM(images->initrd_start);
		rd_size = images->initrd_end - images->initrd_start;
	}

	linux_env_init();

	linux_env_set("memsize", env_buf);

	if(enable_initrd == 1){
		SprintF(env_buf, "%x", rd_start);
		linux_env_set("initrd_start", env_buf);

		SprintF(env_buf, "%x", rd_size);
		linux_env_set("initrd_size", env_buf);
	}
	/* env only support memsize/initrd_start/initrd_size now */
#if 0
	sprintf(env_buf, "0x%08X", (uint) (gd->bd->bi_flashstart));
	linux_env_set("flash_start", env_buf);

	sprintf(env_buf, "0x%X", (uint) (gd->bd->bi_flashsize));
	linux_env_set("flash_size", env_buf);

	cp = getenv("ethaddr");
	if (cp)
		linux_env_set("ethaddr", cp);

	cp = getenv("eth1addr");
	if (cp)
		linux_env_set("eth1addr", cp);

	if (mips_boot_malta) {
		sprintf(env_buf, "%un8r", gd->baudrate);
		linux_env_set("modetty0", env_buf);
	}
#endif
	linux_env_dump();
}

void boot_jump_linux()
{
	bootm_headers_t *images = (bootm_headers_t*)&boot_image;
	typedef void (*kernel_entry_t)(int, ulong, ulong, ulong);
	kernel_entry_t kernel = (kernel_entry_t) images->ep;
	ulong linux_extra = 0;
#if 0
	debug("## Transferring control to Linux (at address %p) ...\n", kernel);

	bootstage_mark(BOOTSTAGE_ID_RUN_OS);

	if (mips_boot_malta)
		linux_extra = gd->ram_size;

	/* we assume that the kernel is in place */
	printf("\nStarting kernel ...\n\n");
#endif
	kernel(linux_argc, (ulong)linux_argv, (ulong)linux_env, linux_extra);
}


void boot_linux_param_init(void)
{
	bootm_headers_t *images = (bootm_headers_t*)&boot_image;
	boot_cmdline_linux(images);
	boot_prep_linux(images);
}


void boot_linux_set_initrd(unsigned long initrd_start,unsigned long initrd_end)
{
	bootm_headers_t *images = (bootm_headers_t*)&boot_image;
	images->initrd_start = initrd_start;
	images->initrd_end = initrd_end;
	enable_initrd = 1;
}

void boot_linux_set_ep(unsigned long ep)
{
	bootm_headers_t *images = (bootm_headers_t*)&boot_image;
	images->ep = ep;
}

