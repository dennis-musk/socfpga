#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "socal.h"
#include "hps.h"
#include "alt_gpio.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )


#define BIT_SW_0	( 0x00100000 )
#define BIT_SW_1	( 0x00080000 )
#define BIT_SW_2	( 0x00040000 )
#define BIT_SW_3	( 0x00020000 )
#define BIT_SW_ALL	( BIT_SW_0 | BIT_SW_1 | BIT_SW_2 | BIT_SW_3 )

#define BIT_KEY_0	( 0x00200000 )
#define BIT_KEY_1	( 0x00400000 )
#define BIT_KEY_2	( 0x00800000 )
#define BIT_KEY_3	( 0x01000000 )
#define BIT_KEY_ALL	( BIT_KEY_0 | BIT_KEY_1 | BIT_KEY_2 | BIT_KEY_3 )

#define BIT_LED_0   (0x01000000)
#define BIT_LED_1   (0x02000000)
#define BIT_LED_2   (0x04000000)
#define BIT_LED_3   (0x08000000)
#define BIT_LED_ALL  (BIT_LED_0 | BIT_LED_1 | BIT_LED_2 | BIT_LED_3)

int main(int argc, char **argv) {

	void *virtual_base;
	int fd;
	uint32_t led_display, scan_input;
	

		
	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	// initialize the pio controller
	
	// led: set the direction of the HPS GPIO1 bits attached to LEDs to output
	alt_setbits_word( ( virtual_base + ( ( uint32_t )( ALT_GPIO1_SWPORTA_DDR_ADDR ) & ( uint32_t )( HW_REGS_MASK ) ) ), BIT_LED_ALL );
	
	// key: the pio is input only controller, so we don't need to configure its direction

	printf("pio demo\r");
	while(1){
		scan_input = alt_read_word( ( virtual_base + ( ( uint32_t )( ALT_GPIO2_EXT_PORTA_ADDR ) & ( uint32_t )( HW_REGS_MASK ) ) ) );		
		
		
		led_display = 0;
		
		// led 0 
		if (~scan_input & BIT_KEY_0) // key is active-low
			led_display |= BIT_LED_0;
			
		if (scan_input & BIT_SW_0) // switch up is high
			led_display |= BIT_LED_0;


		// led 1 
		if (~scan_input & BIT_KEY_1) // key is active-low
			led_display |= BIT_LED_1;
			
		if (scan_input & BIT_SW_1) // switch up is high
			led_display |= BIT_LED_1;
			
		// led 2 
		if (~scan_input & BIT_KEY_2) // key is active-low
			led_display |= BIT_LED_2;
			
		if (scan_input & BIT_SW_2) // switch up is high
			led_display |= BIT_LED_2;
			
		// led 3 
		if (~scan_input & BIT_KEY_3) // key is active-low
			led_display |= BIT_LED_3;
			
		if (scan_input & BIT_SW_3) // switch up is high
			led_display |= BIT_LED_3;

			
		// set led
		// off all led	
		alt_write_word( ( virtual_base + ( ( uint32_t )( ALT_GPIO1_SWPORTA_DR_ADDR ) & ( uint32_t )( HW_REGS_MASK ) ) ), led_display );

	}
	
	// clean up our memory mapping and exit
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return( 0 );
}
