#include <stdio.h>
#include <fcntl.h> //open
#include <sys/mman.h> //mmap, munmap
#include <unistd.h> //close
#include <string.h> //perror

char *usage = 
"Usage: mpsoc_poke 0xADDR [0xDATA]\n"
"\n"
"Accesses memory at address 0xADDR. If DATA is given, a 32-bit\n"
"write is performed. Otherwise, a 32 bit read is performed\n";

int main(int argc, char **argv) {
	
	//STEP 1: read command line inputs

	if (argc != 2 && argc != 3) {
		printf("%s", usage);
		return 0;
	}

	unsigned long addr;
	int matches = sscanf(argv[1], "%lx", &addr);
	
	//Check that value parsed correctly
	if (matches != 1) {
		printf("Error: incorrect address format [%s]\n", argv[1]);
		return -1;
	}
	
	//Check that address is in range
	if (addr < 0xA0000000 || addr > 0xA0FFFFFF) {
		//I don't actually know the maximum allowable address
		printf("Address is out of range!\n");
		return -1;
	}

	//Check address has 32 bit alignment
	if (addr & 0b11) {
		printf("Error! Address must be 32-bit aligned\n");
		return -1;
	}	

	unsigned data;
	char writing = 0;
	if (argc == 3) {
		writing = 1;
		matches = sscanf(argv[2], "%x", &data);
		if (matches != 1) {
			printf("Error: incorrect data format [%s]\n", argv[2]);
			return -1;
		}
	}

	//STEP 2: Perform the memory access

	int fd = -1;
	void *base = MAP_FAILED;

	//Open the device file
	fd = open("/dev/mpsoc_axiregs", O_RDWR | O_SYNC); //O_SYNC is critical for IO mem 
	if (fd < 0) {
		perror("Could not open /dev/mpsoc_axiregs");
		goto open_failed;
	}
	
	unsigned long pg_aligned = (addr | 0xFFF) - 0xFFF; //Mask out lower bits
	unsigned long pg_off = addr & 0xFFF; //Get only lower bits

	//Perform the memory mapping
	base = mmap(
		0, //addr: Can be used to pick & choose virtual addresses. Ignore it.
		4096, //len: We'll (arbitrarily) map a whole page
		PROT_READ | PROT_WRITE, //prot: We want to read and write this memory
		MAP_SHARED, //flags: Allow others to use this memory
		fd, //fildes: File descriptor for device file we're mmmapping
		(pg_aligned - 0xA0000000) //off: (Page-aligned) offset into FPGA memory
	);
	
	if (base == MAP_FAILED) {
		perror("Could not mmap device memory");
		goto mmap_failed;
	}
	
	//Actually access the memory
	if (writing) {
		*(volatile unsigned *)(base+pg_off) = data;
	}
	printf("Read back: 0x%x (%u)\n", *(volatile unsigned *)(base+pg_off), *(volatile unsigned *)(base+pg_off));

	munmap(base, 4096);
	close(fd);

	//Success
	return 0;



	//Correctly clean up after a failureà

	mmap_failed:
	close(fd);

	open_failed:
	return -1;
}
