#include "stm8s.h"
#include "milis.h"
#include "spse_stm8.h"
#include "stm8_hd44780.h"
#include "stdio.h"

void display(void);
void stop(void);
void mezicas(void);
void zobrazcas(void);
uint16_t last_time=0,cas=0;//uloženi èasu, uložení sekund
uint8_t x=0,i=0,a=0,b=0;//pomocné promìné na stisky tlaèítek
uint8_t start=0,ulozeni_mezicas=0,uloz_mezcas=0;
uint8_t mezcas[5];
uint8_t text[32];
void main(void){
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz z interního RC oscilátoru
//inicializace fci
init_milis(); 
lcd_init();
//inicializace vstupù
GPIO_Init(GPIOE, GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT);//start stop
GPIO_Init(GPIOG, GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT);
GPIO_Init(GPIOG, GPIO_PIN_4,GPIO_MODE_IN_PU_NO_IT);


	
	while (1){
		stop();
		mezicas();
		if(milis() - last_time>= 1000 && start==1){
			last_time=milis();
			cas++;
			
			display();
		}
		if(uloz_mezcas==3){
			i=ulozeni_mezicas;
			uloz_mezcas=1;
		}
		if(start==0){zobrazcas();}
	}	
}

void stop(void){
	if(GPIO_ReadInputPin(GPIOE,GPIO_PIN_4)== RESET && x==0){
			last_time=milis();
			start++;
			uloz_mezcas++;
			x=1;
		}
	if(GPIO_ReadInputPin(GPIOE,GPIO_PIN_4)!= RESET){x=0;}
	if(start==2){
		ulozeni_mezicas=i;
		i=0;
		start=0;}
}


void display(void){
static uint16_t desmin=0,min=0,dessec=0,sec=0;
static uint16_t hodnota=0,hodnota2=0;

desmin=cas/600;
hodnota=cas%600;
min=hodnota/60;
hodnota2=hodnota%60;
dessec=hodnota2/10;
sec=hodnota2%10;

lcd_gotoxy(2,0);//stará se zobrazení textu a symbolù na displej.
lcd_puts(":");
//lcd_gotoxy(5,0);
//lcd_puts(".");
sprintf(text,"%01u",sec);
lcd_gotoxy(4,0);
lcd_puts(text);
sprintf(text,"%01u",dessec);
lcd_gotoxy(3,0);
lcd_puts(text);
sprintf(text,"%01u",desmin);
lcd_gotoxy(0,0);
lcd_puts(text);
sprintf(text,"%01u",min);
lcd_gotoxy(1,0);
lcd_puts(text);	
}

void mezicas(void){
if(GPIO_ReadInputPin(GPIOG,GPIO_PIN_4)== RESET && a==0){
	a=1;
	mezcas[i]=cas;
	if(i<5){
	i++;
	}
}
if(GPIO_ReadInputPin(GPIOG,GPIO_PIN_4)!= RESET){a=0;}
}



void zobrazcas(void){
if(GPIO_ReadInputPin(GPIOG,GPIO_PIN_5)== RESET && b==0){
	b=1;
	cas=mezcas[i];
	if(i<5){
	i++;
	}
	display();
}
if(GPIO_ReadInputPin(GPIOG,GPIO_PIN_5)!= RESET){b=0;}
}

		
// pod tímto komentáøem nic nemìòte 
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif