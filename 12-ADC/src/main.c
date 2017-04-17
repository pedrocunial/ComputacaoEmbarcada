/**
 * 12- ADC
 * Rafael Corsi @ insper.edu.br
 * Abril 2017
 *
 * Configura o ADC do SAME70 para fazer leitura
 * do sensor de temperatura interno
 */

/************************************************************************/
/* Includes                                                             */
/************************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "asf.h"

/************************************************************************/
/* Defines                                                              */
/************************************************************************/

/** Header printf */
#define STRING_EOL    "\r"
#define STRING_HEADER "-- AFEC Temperature Sensor Example --\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/** Reference voltage for AFEC,in mv. */
#define VOLT_REF        (3300)

/** The maximal digital value */
/** 2^12 - 1                  */
#define MAX_DIGITAL     (4095UL)

/** The conversion data is done flag */
volatile bool is_conversion_done = false;

/** The conversion data value */
volatile uint32_t g_ul_value = 0;

/* Canal do sensor de temperatura */
#define AFEC_CHANNEL_TEMP_SENSOR 11

// Valores de tempo
#define YEAR   2017
#define MONTH  4
#define DAY    17
#define WEEK   17
#define HOUR   17
#define MINUTE 18
#define SECOND 0

// TC
#define TC1_CHANNEL 1
#define FREQ 1

/************************************************************************/
/* Funcoes                                                              */
/************************************************************************/

/**
 * \brief Configure UART console.
 * BaudRate : 115200
 * 8 bits
 * 1 stop bit
 * sem paridade
 */

static void configure_console(void)
{

	const usart_serial_options_t uart_serial_options = {
		.baudrate   = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits   = CONF_UART_STOP_BITS,
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * converte valor lido do ADC para temperatura em graus celsius
 * input : ADC reg value
 * output: Temperature in celsius
 */
static int32_t convert_adc_to_temp(int32_t ADC_value){

  int32_t ul_vol;
  int32_t ul_temp;

	ul_vol = ADC_value * VOLT_REF / MAX_DIGITAL;

  /*
   * According to datasheet, The output voltage VT = 0.72V at 27C
   * and the temperature slope dVT/dT = 2.33 mV/C
   */
  ul_temp = (ul_vol - 720)  * 100 / 233 + 27;
  return(ul_temp);
}

/************************************************************************/
/* Call backs / Handler                                                 */
/************************************************************************/

/**
 * \brief AFEC interrupt callback function.
 */
static void AFEC_Temp_callback(void)
{
	g_ul_value = afec_channel_get_value(AFEC0, AFEC_CHANNEL_TEMP_SENSOR);
	is_conversion_done = true;
}


// Configuração do RTC (exemplo 11)
void RTC_init(void)
{
	// configura o PMC
	pmc_enable_periph_clk(ID_RTC);

	// configura o RTC para o modo de 24hrs
	rtc_set_hour_mode(RTC, 0);

	// configura data e hora manualmente
	// no caso estou fazendo uma "gambiarra", pois só
	// preciso de interrupções de 1 em 1 segundo
	rtc_set_date(RTC, YEAR, MONTH, DAY, WEEK);
	rtc_set_time(RTC, HOUR, MINUTE, SECOND);
}

void TC1_init(void)
{
	uint32_t ul_div, ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	// Configura o PMC
	pmc_enable_periph_clk(ID_TC1);

	// Configura o TC para operar em 1MHz (1s) e interrupcao
	// no RC compare
	tc_find_mck_divisor(FREQ, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, TC1_CHANNEL, ul_tcclks | TC_CMR_CPCTRG);
	// a divisao abaixo é um macete para chegar no valor desejado
	tc_write_rc(TC0, TC1_CHANNEL, (ul_sysclk / ul_div) / FREQ);

	// Configura e ativa a interrupção no TC canal 0
	NVIC_EnableIRQ((IRQn_Type) ID_TC1);
	tc_enable_interrupt(TC0, TC1_CHANNEL, TC_IER_CPCS);

	// inicializa
	tc_start(TC0, TC1_CHANNEL);
}

void TC1_Handler(void)
{
	volatile uint32_t ul_dummy;
	uint32_t pul_hour, pul_minute, pul_second;
	uint32_t pul_day, pul_month, pul_year, pul_week;

	// devemos indicar ao TC que a interrupcao foi satisfeita
	ul_dummy = tc_get_status(TC0, TC1_CHANNEL);
	UNUSED(ul_dummy);
	// chamado apos o intervalo desejado (1s)
	// printa temperatura
	rtc_get_date(RTC, &pul_year, &pul_month, &pul_day, &pul_week);
	rtc_get_time(RTC, &pul_hour, &pul_minute, &pul_second);
	printf("%02d/%02d/%04d - %02d:%02d:%02d - Temp : %dºC \r\n",
			pul_day,
			pul_month,
			pul_year,
			pul_hour,
			pul_minute,
			pul_second,
			(uint32_t) convert_adc_to_temp(g_ul_value));
	afec_start_software_conversion(AFEC0);
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{

	/* Initialize the SAM system. */
	sysclk_init();
	ioport_init();
	board_init();

	/* inicializa delay */
	delay_init(sysclk_get_cpu_hz());

	/* inicializa console (printf) */
	configure_console();

	/* Output example information. */
	puts(STRING_HEADER);

	/*************************************
	* Ativa e configura AFEC
	*************************************/

	/* Ativa AFEC - 0 */
	afec_enable(AFEC0);

	/* struct de configuracao do AFEC */
	struct afec_config afec_cfg;

	/* Carrega parametros padrao */
	afec_get_config_defaults(&afec_cfg);

	/* Configura AFEC */
	afec_init(AFEC0, &afec_cfg);

	/* Configura trigger por software */
	afec_set_trigger(AFEC0, AFEC_TRIG_SW);

	/* configura call back */
 	afec_set_callback(AFEC0, AFEC_INTERRUPT_EOC_11,	AFEC_Temp_callback, 1);

	/*** Configuracao específica do canal AFEC ***/
	struct afec_ch_config afec_ch_cfg;
	afec_ch_get_config_defaults(&afec_ch_cfg);
	afec_ch_cfg.gain = AFEC_GAINVALUE_0;
	afec_ch_set_config(AFEC0, AFEC_CHANNEL_TEMP_SENSOR, &afec_ch_cfg);

	/*
	* Calibracao:
	* Because the internal ADC offset is 0x200, it should cancel it and shift
	* down to 0.
	*/
	afec_channel_set_analog_offset(AFEC0, AFEC_CHANNEL_TEMP_SENSOR, 0x200);

	/***  Configura sensor de temperatura ***/
	struct afec_temp_sensor_config afec_temp_sensor_cfg;

	afec_temp_sensor_get_config_defaults(&afec_temp_sensor_cfg);
	afec_temp_sensor_set_config(AFEC0, &afec_temp_sensor_cfg);

	/* Selecina canal e inicializa conversão */
	afec_channel_enable(AFEC0, AFEC_CHANNEL_TEMP_SENSOR);
	afec_start_software_conversion(AFEC0);

	// Configuração de interrupções de timer
	TC1_init();
	RTC_init();

	while (1) {
		//if(is_conversion_done == true) {
			//is_conversion_done = false;
//
			//printf("Temp : %dºC \r\n", (uint32_t) convert_adc_to_temp(g_ul_value));
			//afec_start_software_conversion(AFEC0);
			//delay_s(1);
		//}
	}
}
