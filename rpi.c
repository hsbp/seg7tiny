#include <stdio.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>

#define BCM2708_PERI_BASE       0x20000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)	// GPIO controller

#define BLOCK_SIZE 		(4*1024)

// IO Access
struct bcm2835_peripheral {
	unsigned long addr_p;
	int mem_fd;
	void *map;
	volatile unsigned int *addr;
};

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x)
#define INP_GPIO(g)   *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)   *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET  *(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR  *(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0

#define GPIO_READ(g)  *(gpio.addr + 13) &= (1<<(g))

struct bcm2835_peripheral gpio = {GPIO_BASE};

// Exposes the physical address defined in the passed structure using mmap on /dev/mem
int map_peripheral(struct bcm2835_peripheral *p)
{
	// Open /dev/mem
	if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
		printf("Failed to open /dev/mem, try checking permissions.\n");
		return -1;
	}

	p->map = mmap(
			NULL,
			BLOCK_SIZE,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
			p->addr_p       // Address in physical map that we want this memory block to expose
			);

	if (p->map == MAP_FAILED) {
		perror("mmap");
		return -1;
	}

	p->addr = (volatile unsigned int *)p->map;

	return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p)
{
	munmap(p->map, BLOCK_SIZE);
	close(p->mem_fd);
}

#define BIT1() GPIO_SET = 1 << PIN
#define BIT0() GPIO_CLR = 1 << PIN

int main()
{
	int ch;
	if (map_peripheral(&gpio) == -1) {
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		return -1;
	}

	// Define pin as output
	INP_GPIO(PIN);
	OUT_GPIO(PIN);

	BIT1();
	while((ch = getchar()) != EOF) {
		short bit;
		usleep(4000);
		BIT0(); // start
		usleep(BITLEN);
		for (bit = 1; bit != 0x100; bit <<= 1) {
			if (bit & ch) BIT1(); else BIT0();
			usleep(BITLEN);
		}
		BIT1(); // stop
		usleep(BITLEN);
	}

	return 0;
}
