#include "asf.h"

/************************************************************************/
/* DEFINES                                                              */
/************************************************************************/

/**
 *  Informacoes para o RTC
 *  poderia ser extraida do __DATE__ e __TIME__
 *  ou ser atualizado pelo PC.
 */
#define YEAR        2017
#define MOUNTH      3
#define DAY         27
#define WEEK        13
#define HOUR        9
#define MINUTE      5
#define SECOND      0

/**
 * LEDs
 */

// LED da placa
#define LED_PIO_ID		ID_PIOC
#define LED_PIO         PIOC
#define LED_PIN		    8
#define LED_PIN_MASK    (1 << LED_PIN)

// LED 1
#define LED1_PIO_ID    ID_PIOA
#define LED1_PIO       PIOA
#define LED1_PIN       0
#define LED1_PIN_MASK  (1 << LED1_PIN)

// LED 2
#define LED2_PIO_ID    ID_PIOC
#define LED2_PIO       PIOC
#define LED2_PIN       30
#define LED2_PIN_MASK  (1 << LED2_PIN)

// LED 3
#define LED3_PIO_ID    ID_PIOB
#define LED3_PIO       PIOB
#define LED3_PIN       2
#define LED3_PIN_MASK  (1 << LED3_PIN)

/**
 * Botão da placa
 */
#define BUT_PIO_ID      ID_PIOA
#define BUT_PIO         PIOA
#define BUT_PIN		    11
#define BUT_PIN_MASK    (1 << BUT_PIN)
#define BUT_DEBOUNCING_VALUE  79

// Botão 1
#define BUT1_PIO_ID   ID_PIOD
#define BUT1_PIO      PIOD
#define BUT1_PIN      28
#define BUT1_PIN_MASK (1 << BUT1_PIN)

// Botão 2
#define BUT2_PIO_ID   ID_PIOC
#define BUT2_PIO      PIOC
#define BUT2_PIN      31
#define BUT2_PIN_MASK (1 << BUT2_PIN)

// Botão 3
#define BUT3_PIO_ID   ID_PIOA
#define BUT3_PIO      PIOA
#define BUT3_PIN      19
#define BUT3_PIN_MASK (1 << BUT3_PIN)


/************************************************************************/
/* VAR globais                                                          */
/************************************************************************/
volatile uint8_t flag_led0 = 1;
volatile uint8_t flag_led1 = 1;


/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void BUT_init(void);
void LED_init(int estado);
void TC1_init(void);
void RTC_init(void);
void pin_toggle(Pio *pio, uint32_t mask);

/************************************************************************/
/* Handlers                                                             */
/************************************************************************/

/**
 *  Handle Interrupcao botao 1
 */
static void Button1_Handler(uint32_t id, uint32_t mask)
{

}

/**
 *  Interrupt handler for TC0 interrupt.
 */
void TC1_Handler(void){
	volatile uint32_t ul_dummy;

    /****************************************************************
	* Devemos indicar ao TC que a interrupção foi satisfeita.
    ******************************************************************/
	ul_dummy = tc_get_status(TC0, 1);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/** Muda o estado do LED */
   // if(flag_led0)
        pin_toggle(LED_PIO, LED_PIN_MASK);
}

void TC2_Handler(void)
{
	volatile uint32_t ul_dummy;

    /****************************************************************
	* Devemos indicar ao TC que a interrupção foi satisfeita.
    ******************************************************************/
	ul_dummy = tc_get_status(TC1, 1);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/** Muda o estado do LED */
    if(flag_led1)
        pin_toggle(LED_PIO, LED_PIN_MASK);
}

/**
 * \brief Interrupt handler for the RTC. Refresh the display.
 */
void RTC_Handler(void)
{
	uint32_t ul_status = rtc_get_status(RTC);

	/* Second increment interrupt */
	if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC) {
		rtc_clear_status(RTC, RTC_SCCR_SECCLR);
	} else {
		/* Time or date alarm */
		if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM) {
            /* Toggle LED */
			flag_led0 ^= 1;
			// if the LED is supposed to be off, we stop it
			if (flag_led0)
				tc_stop(TC1, 1);
			else
				tc_start(TC1, 1);
			rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
			// void 	rtc_get_time (Rtc *p_rtc, uint32_t *pul_hour, uint32_t *pul_minute, uint32_t *pul_second)
			uint32_t pul_hour, pul_minute, pul_second;
			rtc_get_time(RTC, &pul_hour, &pul_minute, &pul_second);
			// Set the alarm for the next minute
			// NOTE: Unknown behaviour when dealing with times such as 5:59
			rtc_set_time_alarm(RTC, 0, pul_hour, 1, pul_minute + 1, 0, pul_second);
		}
	}
}


/************************************************************************/
/* Funcoes                                                              */
/************************************************************************/

/**
 *  Toggle pin controlado pelo PIO (out)
 */
void pin_toggle(Pio *pio, uint32_t mask){
   if(pio_get_output_data_status(pio, mask))
    pio_clear(pio, mask);
   else
    pio_set(pio,mask);
}

/**
 * @Brief Inicializa o pino do BUT
 */
void BUT_init(void){
    /* config. pino botao em modo de entrada */
    pmc_enable_periph_clk(BUT_PIO_ID);
    pio_set_input(BUT_PIO, BUT_PIN_MASK, PIO_PULLUP | PIO_DEBOUNCE);

    /* config. interrupcao em borda de descida no botao do kit */
    /* indica funcao (but_Handler) a ser chamada quando houver uma interrupção */
    pio_enable_interrupt(BUT_PIO, BUT_PIN_MASK);
    pio_handler_set(BUT_PIO, BUT_PIO_ID, BUT_PIN_MASK, PIO_IT_FALL_EDGE, Button1_Handler);

    /* habilita interrupçcão do PIO que controla o botao */
    /* e configura sua prioridade                        */
    NVIC_EnableIRQ(BUT_PIO_ID);
    NVIC_SetPriority(BUT_PIO_ID, 1);
}


/**
 * @Brief Inicializa o pino do LED
 */
void LED_init(int estado){
    pmc_enable_periph_clk(LED_PIO_ID);
    pio_set_output(LED_PIO, LED_PIN_MASK, estado, 0, 0 );
};

void generic_led_init(
		int estado,
		Pio *pio,
		const u_int32_t pio_id,
		const u_int32_t pio_mask)
{
	pmc_enable_periph_clk(pio_id);
	pio_set_output(pio, pio_mask, estado, 0, 0);
}

/**
 * Configura TimerCounter (TC0) para gerar uma interrupcao no canal 0-(ID_TC1)
 * a cada 250 ms (4Hz)
 */
void TC1_init(void){
    uint32_t ul_div;
    uint32_t ul_tcclks;
    uint32_t ul_sysclk = sysclk_get_cpu_hz();
    uint32_t channel = 1;

    /* Configura o PMC */
    pmc_enable_periph_clk(ID_TC1);

    /** Configura o TC para operar em  4Mhz e interrupçcão no RC compare */
    tc_find_mck_divisor(4, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
    tc_init(TC0, channel, ul_tcclks | TC_CMR_CPCTRG);
    tc_write_rc(TC0, channel, (ul_sysclk / ul_div) / 4);

    /* Configura e ativa interrupçcão no TC canal 0 */
    NVIC_EnableIRQ((IRQn_Type) ID_TC1);
    tc_enable_interrupt(TC0, channel, TC_IER_CPCS);

    /* Inicializa o canal 0 do TC */
    tc_start(TC0, channel);
}

void TC2_init(void)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();
	uint32_t channel = 1;

	/* Configura o PMC respectivo TC */
	pmc_enable_periph_clk(ID_TC2);

	/* Configura o TC para operar em sua dada frequência
	(no caso 8Hz, pois controla o LED1 da placa externa) */
	tc_find_mck_divisor(8, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC1, channel, ul_tcclks | TC_CMR_CPCTRG);

	/* A divisão na linha de baixo é um "macete" para chegar no valor desejado de freq. */
	tc_write_rc(TC1, channel, (ul_sysclk / ul_div) / 8);

	/* Configura e ativa interrupçcão no TC canal 1 */
    NVIC_EnableIRQ((IRQn_Type) ID_TC2);
    tc_enable_interrupt(TC1, channel, TC_IER_CPCS);

    /* Inicializa o canal 1 do TC */
    tc_start(TC1, channel);
}

/**
 * Configura o RTC para funcionar com interrupcao de alarme
 */
void RTC_init(){
    /* Configura o PMC */
    pmc_enable_periph_clk(ID_RTC);

    /* Default RTC configuration, 24-hour mode */
    rtc_set_hour_mode(RTC, 0);

    /* Configura data e hora manualmente */
    rtc_set_date(RTC, YEAR, MOUNTH, DAY, WEEK);
    rtc_set_time(RTC, HOUR, MINUTE, SECOND);

    /* Configure RTC interrupts */
    NVIC_DisableIRQ(RTC_IRQn);
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 0);
    NVIC_EnableIRQ(RTC_IRQn);

    /* Ativa interrupcao via alarme */
    rtc_enable_interrupt(RTC,  RTC_IER_ALREN);

}

/************************************************************************/
/* Main Code	                                                        */
/************************************************************************/
int main(void){
	/* Initialize the SAM system */
	sysclk_init();

	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;

    /* Configura Leds */
    LED_init(0);
	generic_led_init(0,
					 LED1_PIO,
					 LED1_PIO_ID,
					 LED1_PIN_MASK);
	generic_led_init(0,
					 LED2_PIO,
					 LED2_PIO_ID,
					 LED2_PIN_MASK);
	generic_led_init(0,
 					 LED3_PIO,
					 LED3_PIO_ID,
					 LED3_PIN_MASK);

	/* Configura os botões */
	BUT_init();

    /** Configura timer 0 */
    TC1_init();
    /** Configura timer 1 */
	TC2_init();

    /** Configura RTC */
    RTC_init();

    /* configura alarme do RTC */
    rtc_set_date_alarm(RTC, 1, MOUNTH,1, DAY);
    rtc_set_time_alarm(RTC, 1, HOUR, 1, MINUTE+1, 1, SECOND);


	while (1) {
		/* Entra em modo sleep */

	}
}
