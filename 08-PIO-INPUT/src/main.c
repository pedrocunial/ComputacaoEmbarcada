/**************************************************************************
* Rafael Corsi   - Insper 
* rafael.corsi@insper.edu.br        
*
* Computa��o Embarcada
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
 * Bot�o
 */ 
#define BUT_PIO_ID		ID_PIOA
#define BUT_PIO         PIOA
#define BUT_PIN			11
#define BUT_PIN_MASK	(1 << BUT_PIN)

/************************************************************************/
/* Prototipa��o                                                        */
/************************************************************************/
void ledConfig();

/************************************************************************/
/* Fun��es	                                                            */
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
	* Perif�rico : PMC 
	* @Brief Peripheral Clock Enable Register
	* O PMC � o perif�rico respons�vel pelo controle de energia dos
	* demais perif�ricos.
	* Inicializamos aqui o clock do perif�rico PIO C.
	*/ 
	PMC->PMC_PCER0 = (1<<LED_PIO_ID);
	
	/**
	* Perif�rico : PIO C
	* @Brief Output Enable Register
	* Configuramos o pino como sa�da
	*/ 
	PIOC->PIO_OER = (1 << 8);
	
	/**
	* Perif�rico : PIO C
	* @Brief Peripheral Enable Register
	* Fazemos com que o controle do pino seja realizado pelo PIO
	* e n�o por outro perif�rico
	*/ 
	PIOC->PIO_PER = (1 << 8);

	/**
	* Perif�rico : PIO C
	* @Brief Clear/Set Output Data Register
	* Aqui controlamos o valor da sa�da desse pino
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
	/* Inicializa��o b�sica do uC                                           */
	/************************************************************************/
	sysclk_init();
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	/************************************************************************/
	/* Inicializa perifericos                                               */
	/************************************************************************/
	// Configura LED em modo sa�da
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