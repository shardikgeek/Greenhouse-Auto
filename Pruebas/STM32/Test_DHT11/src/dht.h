/*
 * dht.h
 *
 *  Created on: 11 ago. 2018
 *      Author: diego
 */
// Pin 1 GPIOA DHT11
#ifndef DHT_H_
#define DHT_H_
#define TIME_TIMEOUT 100e3

#define STATE_DHT_TIMEOUT 0
#define STATE_DHT_DESCONECTADO 1
#define STATE_DHT_CHECKSUM_GOOD 2
#define STATE_DHT_CHECKSUM_BAD 3


struct{
	uint32_t contador;
	uint8_t flag;
	uint8_t check;
	uint32_t timeout;
	uint8_t flag_timeout;
	uint8_t estado;
	char temperatura_string[6];
	double temperatura;
	double humedad;
}dht;

void DHT11_start(void);
void check_response(void);
uint8_t read_data (void);
void delay(uint32_t tiempo);

void set_gpio_output (void)
{
	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion del pin.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_1;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP ;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_Init_Pins); // Carga de la estrucura de datos.
}

void set_gpio_input (void)
{
	GPIO_InitTypeDef GPIO_Init_Pins; // Estrucura de datos para configurar el GPIO

	//
	//Inicializacion del pin.
	//

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_Init_Pins.GPIO_Pin= GPIO_Pin_1;
	GPIO_Init_Pins.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init_Pins.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_Init_Pins.GPIO_OType= GPIO_OType_PP;
	GPIO_Init_Pins.GPIO_PuPd= GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_Init_Pins); // Carga de la estrucura de datos.
}

void DHT11_start (void)
{
	set_gpio_output ();  // set the pin as output
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,0); // pull the pin low
	delay(18000);   // wait for 18ms
	set_gpio_input ();   // set as input
}

void check_response (void)
{
	delay(40);
	if (!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)))
	{
		delay(80);
		if ((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))) dht.check = 1;
	}

	dht.timeout = TIME_TIMEOUT;
	while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) && !dht.flag_timeout);   // wait for the pin to go low
}

uint8_t read_data (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		dht.timeout = TIME_TIMEOUT;
		while (!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) && !dht.flag_timeout);   // wait for the pin to go high

		delay(40);   // wait for 40 us
		if ((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) == 0)   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1

		dht.timeout = TIME_TIMEOUT; // set the timeout
		while ((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) && !dht.flag_timeout);  // wait for the pin to go low or the timeout dead.
	}
	return i;
}




#endif /* DHT_H_ */
