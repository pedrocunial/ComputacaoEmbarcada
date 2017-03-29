/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekDay;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}rtc_t;



int main() 
{
	board_init();
	
    rtc_t rtc;

	
    /*Connect SCL->PC0, SDA->PC1*/    
    RTC_Init();
    rtc.hour = 0x10; //  10:40:20 am
    rtc.min =  0x40;
    rtc.sec =  0x00;

    rtc.date = 0x01; //1st Jan 2016
    rtc.month = 0x01;
    rtc.year = 0x16;
    rtc.weekDay = 5; // Friday: 5th day of week considering monday as first day.
    /*##### Set the time and Date only once. Once the Time and Date is set, comment these lines
         and reflash the code. Else the time will be set every time the controller is reset*/
    RTC_SetDateTime(&rtc);  //  10:40:20 am, 1st Jan 2016
    /* Display the Time and Date continuously */
    while(1)
    {
        RTC_GetDateTime(&rtc);
        
        printf("time:%2x:%2x:%2x  \nDate:%2x/%2x/%2x",(uint16_t)rtc.hour,(uint16_t)rtc.min,(uint16_t)rtc.sec,(uint16_t)rtc.date,(uint16_t)rtc.month,(uint16_t)rtc.year);
    }
    return (0);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */


	/* Insert application code here, after the board has been initialized. */
}
