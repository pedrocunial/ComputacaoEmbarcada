/**************************************************************************
* Rafael Corsi   - Insper 
* rafael.corsi@insper.edu.br        
*
* Computação Embarcada
*
* 08-PIO-ENTRADA
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
#define LED_PIN			8
#define LED_PIN_MASK	(1<<LED_PIN) 

/**
 * Botão
 */ 
#define BUT_PIO_ID		ID_PIOA
#define BUT_PIO         PIOA
#define BUT_PIN			11
#define BUT_PIN_MASK	(1 << BUT_PIN)

/************************************************************************/
/* Prototipação                                                        */
/************************************************************************/
void ledConfig();

/************************************************************************/
/* Funções	                                                            */
/************************************************************************/

void toggleLed(void)
{
	if (BUT_PIO->PIO_PDSR & BUT_PIN_MASK)
		LED_PIO->PIO_SODR = LED_PIN_MASK;
	else
		LED_PIO->PIO_CODR = LED_PIN_MASK;
}

/**
 * @Brief Inicializa o pino do LED
 */
void ledConfig(void)
{
	/**
	* Periférico : PMC 
	* @Brief Peripheral Clock Enable Register
	* O PMC é o periférico responsável pelo controle de energia dos
	* demais periféricos.
	* Inicializamos aqui o clock do periférico PIO C.
	*/ 
	PMC->PMC_PCER0 = (1<<LED_PIO_ID);
	
	/**
	* Periférico : PIO C
	* @Brief Output Enable Register
	* Configuramos o pino como saída
	*/ 
	PIOC->PIO_OER = (1 << 8);
	
	/**
	* Periférico : PIO C
	* @Brief Peripheral Enable Register
	* Fazemos com que o controle do pino seja realizado pelo PIO
	* e não por outro periférico
	*/ 
	PIOC->PIO_PER = (1 << 8);

	/**
	* Periférico : PIO C
	* @Brief Clear/Set Output Data Register
	* Aqui controlamos o valor da saída desse pino
	* no caso especifico colocamos 0 (acende o LED)
	* O registrador :
	*   - PIO_SODR : coloca 1 no pino 
	*	- PIO_CODR : coloca 0 no pino 
    */
	PIOC->PIO_SODR =  (1 << 8);
}

/**
 * @Brief Inicializa o pino do LED
 */
void butConfig(void)
{
	PMC->PMC_PCER0 = (1 << ID_PIOA);
	BUT_PIO->PIO_PER = BUT_PIN_MASK;
	BUT_PIO->PIO_ODR = BUT_PIN_MASK;
	BUT_PIO->PIO_PUER = BUT_PIN_MASK;
	BUT_PIO->PIO_IFER = BUT_PIN_MASK;
}
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
	/* Inicializa perifericos                                               */
	/************************************************************************/
	// Configura LED em modo saída
	ledConfig();

	butConfig();
	// Configura botao
	
	/************************************************************************/
	/* Super loop                                                           */
	/************************************************************************/
	while(1){
		toggleLed();
	}
}