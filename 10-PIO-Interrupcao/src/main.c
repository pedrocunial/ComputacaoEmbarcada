/************************************************************************
* Rafael Corsi   - Insper
* rafael.corsi@insper.edu.br
*
* Computação Embarcada
*
* 10-PIO-INTERRUPCAO
*
* [ref] http://www.atmel.com/Images/Atmel-42142-SAM-AT03258-Using-Low-Power-Mode-in-SAM4E-Microcontroller_Application-Note.pdf
* [ref] https://www.eecs.umich.edu/courses/eecs373/labs/refs/M3%20Guide.pdf
************************************************************************/


#include "asf.h"
#include "conf_clock.h"


/************************************************************************/
/* Defines                                                              */
/************************************************************************/

/**
 * LEDs
 */
#define LED_PIO_ID		ID_PIOC
#define LED_PIO         PIOC
#define LED_PIN		    8
#define LED_PIN_MASK    (1<<LED_PIN)

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
/* prototype                                                             */
/************************************************************************/
void led_init(
		Pio *p_pio,
		const u_int32_t pio_id,
		const u_int32_t pin_mask,
		int estado);
void but_init(
		Pio *p_but_pio,
		const u_int32_t pio_id,
		const u_int32_t but_pin_mask);
void but_Handler();
void but1_Handler();
void but2_Handler();
void but3_Handler();

/************************************************************************/
/* globals (flags)                                                      */
/************************************************************************/
char flag = 0;

/************************************************************************/
/* Interrupçcões                                                        */
/************************************************************************/
void blink(void)
{
	for (char i = 0; i < 15; i++) {
		if(pio_get_output_data_status(LED_PIO, LED_PIN_MASK))
			pio_clear(LED_PIO, LED_PIN_MASK);
		else
			pio_set(LED_PIO, LED_PIN_MASK);
		delay_ms(200);
	}
}

void but_Handler()
{
    /*
     *  limpa interrupcao do PIO
     */
    uint32_t pioIntStatus;
    pioIntStatus =  pio_get_interrupt_status(BUT_PIO);
	/**
    *  Toggle status led
    */
	if(pio_get_output_data_status(LED_PIO, LED_PIN_MASK))
		pio_clear(LED_PIO, LED_PIN_MASK);
	else
		pio_set(LED_PIO, LED_PIN_MASK);
    
}

void but1_Handler()
{
    /*
     *  limpa interrupcao do PIO
     */
	flag = 1;
    uint32_t pioIntStatus;
    pioIntStatus =  pio_get_interrupt_status(BUT1_PIO);
    
	/**
    *  Toggle status led
    */
	if(pio_get_output_data_status(LED1_PIO, LED1_PIN_MASK))
		pio_clear(LED1_PIO, LED1_PIN_MASK);
	else
		pio_set(LED1_PIO, LED1_PIN_MASK);
	blink();
}

void but2_Handler()
{
    /*
     *  limpa interrupcao do PIO
     */
	flag = 1;
    uint32_t pioIntStatus;
    pioIntStatus =  pio_get_interrupt_status(BUT2_PIO);
    
	/**
    *  Toggle status led
    */
	if(pio_get_output_data_status(LED2_PIO, LED2_PIN_MASK))
		pio_clear(LED2_PIO, LED2_PIN_MASK);
	else
		pio_set(LED2_PIO, LED2_PIN_MASK);
    blink();
}

void but3_Handler()
{
    /*
     *  limpa interrupcao do PIO
     */
    flag = 1;
    uint32_t pioIntStatus;
    pioIntStatus =  pio_get_interrupt_status(BUT3_PIO);
    
	/**
    *  Toggle status led
    */
	if(pio_get_output_data_status(LED3_PIO, LED3_PIN_MASK))
		pio_clear(LED3_PIO, LED3_PIN_MASK);
	else
		pio_set(LED3_PIO, LED3_PIN_MASK);
	blink();
    
}

/************************************************************************/
/* Funções	                                                            */
/************************************************************************/

/**
 * @Brief Inicializa o pino do LED
 */
void led_init(
		Pio *p_pio,
		const u_int32_t pio_id,
		const u_int32_t pin_mask,
		int estado)
{
    pmc_enable_periph_clk(pio_id);
    pio_set_output(p_pio, pin_mask, 1, 0, 0 );
};

/**
 * @Brief Inicializa o pino do BUT
 *  config. botao em modo entrada enquanto 
 *  ativa e configura sua interrupcao.
 */
void but_init(
		Pio *p_but_pio,
		const u_int32_t pio_id,
		const u_int32_t but_pin_mask)
{
    /* config. pino botao em modo de entrada */
    pmc_enable_periph_clk(pio_id);
    pio_set_input(p_but_pio, but_pin_mask, PIO_PULLUP | PIO_DEBOUNCE);
    
    /* config. interrupcao em borda de descida no botao do kit */
    /* indica funcao (but_Handler) a ser chamada quando houver uma interrupção */
    pio_enable_interrupt(p_but_pio, but_pin_mask);
	switch (but_pin_mask) {
		case BUT_PIN_MASK:
		    pio_handler_set(p_but_pio, pio_id, but_pin_mask, PIO_IT_FALL_EDGE, but_Handler);
			break;
		case BUT1_PIN_MASK:
			pio_handler_set(p_but_pio, pio_id, but_pin_mask, PIO_IT_RISE_EDGE, but1_Handler);
			break;
		case BUT2_PIN_MASK:
			pio_handler_set(p_but_pio, pio_id, but_pin_mask, PIO_IT_FALL_EDGE, but2_Handler);
			break;
		case BUT3_PIN_MASK:
			pio_handler_set(p_but_pio, pio_id, but_pin_mask, PIO_IT_RE_OR_HL, but3_Handler);
			break;		
	}

    
    /* habilita interrupçcão do PIO que controla o botao */
    /* e configura sua prioridade                        */
    NVIC_EnableIRQ(pio_id);
    NVIC_SetPriority(pio_id, 1);
};

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void)
{
	/************************************************************************/
	/* Inicialização básica do uC                                           */
	/************************************************************************/
	sysclk_init();
	WDT->WDT_MR = WDT_MR_WDDIS;

	/************************************************************************/
	/* Inicializao I/OS                                                     */
	/************************************************************************/
	led_init(LED_PIO, LED_PIO_ID, LED_PIN_MASK, 1);
	led_init(LED1_PIO, LED1_PIO_ID, LED1_PIN_MASK, 1);
	led_init(LED2_PIO, LED2_PIO_ID, LED2_PIN_MASK, 1);
	led_init(LED3_PIO, LED3_PIO_ID, LED3_PIN_MASK, 1);
    but_init(BUT_PIO, BUT_PIO_ID, BUT_PIN_MASK);
    but_init(BUT1_PIO, BUT1_PIO_ID, BUT1_PIN_MASK);
    but_init(BUT2_PIO, BUT2_PIO_ID, BUT2_PIN_MASK);
    but_init(BUT3_PIO, BUT3_PIO_ID, BUT3_PIN_MASK);
	/************************************************************************/
	/* Super loop                                                           */
	/************************************************************************/
	while(1){
       /* entra em modo sleep */
		pmc_sleep(SLEEPMGR_SLEEP_WFI);
		if (flag) {
			flag = 0;
			blink();
		}
	};
}


