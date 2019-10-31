#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include "xvcserver.h"
#include "xvc_axi2pl.h"

int verbose = 0;

static void set_tck(unsigned long nsperiod, unsigned long *result) {
	*result = nsperiod;
}

static void shift_tms_tdi(
	void * map_base,
	unsigned long bitcount,
	unsigned char *tms_buf,
	unsigned char *tdi_buf,
	unsigned char *tdo_buf) {
	struct timeval stop, start;

	if (verbose) {
		  gettimeofday(&start, NULL);
	}

	struct xil_xvc_ioc xvc_ioc;

	xvc_ioc.opcode = 0x01; // 0x01 for normal, 0x02 for bypass
	xvc_ioc.length = bitcount;
	xvc_ioc.tms_buf = tms_buf;
	xvc_ioc.tdi_buf = tdi_buf;
	xvc_ioc.tdo_buf = tdo_buf;

	int ret = xvc_axiioc(map_base, &xvc_ioc);
	if (ret < 0)
	{
		  int errsv = errno;
		  printf("IOC Error %d\n", errsv);
	}

	if (verbose) {
		  gettimeofday(&stop, NULL);
		  printf("IOC shift internal took %lu u-seconds with %lu bits. Return value %d\n", stop.tv_usec - start.tv_usec, bitcount, ret);
	}
}

XvcServerHandlers handlers = {
	set_tck,
	shift_tms_tdi
};

void daemonize(){
	int process_id = fork();
	if (process_id < 0)
		exit(1);
	else if (process_id > 0) {
		printf("launched xvc server in daemon mode, pid = %d\n", process_id);
		exit(0);
	}
	umask(0);
	int sid = setsid();
	if(sid < 0)
	{
		exit(1);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}


int main(int argc, char **argv) {
	const char * url = "tcp::2542";
	int i = 1;
	void * map_base;
	int fd;
	unsigned long addr = 0xA0000000;
	bool daemon = false;
	while (i < argc) {
		if (strcmp(argv[i], "-s") == 0) {
			if (i + 1 >= argc) {
				 fprintf(stderr, "option -s requires an argument\n");
				 return 1;
			}
			url = argv[++i];
		} else if (strcmp(argv[i], "-a") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "option -a requires an argument\n");
				return 1;
			}
			addr = strtol(argv[++i], NULL, 16);
		} else if (strcmp(argv[i], "-d") == 0) {
			daemon = true;
		} else if (strcmp(argv[i], "-v") == 0) {
			verbose = 1;
		} else if (strcmp(argv[i], "-help") || strcmp(argv[i], "--help") || strcmp(argv[i], "-h")) {
			printf("Usage: xvcserver [OPTION1] [VALUE1] [OPTION2] [VALUE2] ...\n\n"
				"Options :\n"
				"\t-a : specify the physical address of the debug bridge, default value is 0xA0000000\n"
				"\t-s : specify the socket and protocol of the xvcserver, default value is TCP::2542\n"
				"\t-d : run xvcserver in daemon mode, by default this option is off\n"
				"\t-v : display verbose info, by default this option is off\n\n"
				"Example: xvcserver -a 0xA0010000 -d -s TCP::12345\n"
				"Thank you for using this utility!\n\n");
			return 0;
		} else {
			fprintf(stderr, "unknown option: %s\n", argv[i]);

			printf("unknown option: %s !!!\n", argv[i]);
			printf("Usage: xvcserver [OPTION1] [VALUE1] [OPTION2] [VALUE2] ...\n\n"
				"Options :\n"
				"\t-a : specify the physical address of the debug bridge, default value is 0xA0000000\n"
				"\t-s : specify the socket and protocol of the xvcserver, default value is TCP::2542\n"
				"\t-d : run xvcserver in daemon mode, by default this option is off\n"
				"\t-v : display verbose info, by default this option is off\n\n"
				"Example: xvcserver -a 0xA0010000 -d -s TCP::12345\n"
				"Thank you for using this utility!\n\n");
			return 1;
		}
		i++;
	}
	printf("#######################################################\nCRITICAL NOTICE: by launching this utility, you confirmed that there is a AXI to BSCAN mode \"debug bridge\" with the mapped address 0x%lx running in PL, and the PS has no problem communicating with the debug bridge!\nMake sure above configuration has been set before attempting to connect to this XVC server in Vivado, otherwise you may hang this board!\n#######################################################\n\n", addr);

	if ((fd = open("/dev/mpsoc_axiregs", O_RDWR | O_SYNC)) != -1) {
		map_base = mmap(NULL, 0x00001000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr - 0xA0000000);
		if (!map_base) {
			fprintf(stderr, "unable to map to physical address %lx\n", addr);
			return 1;
		}
	} else {
		fprintf(stderr, "unable to open device /dev/mpsoc_axiregs\n");
		return 1;
	}
	if (daemon) daemonize();
	else printf("launched xvc server in non-daemon mode\n");
	return xvcserver_start(url, map_base, &handlers);
}

