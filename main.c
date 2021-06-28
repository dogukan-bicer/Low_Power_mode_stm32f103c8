#include "stm32f10x.h"                  
int durum2=0;
void systick_init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 72000000/8000; ///72mhz 
	SysTick->VAL = 0;
	SysTick->CTRL |= 5;

}

void DelayMillis(void)
{
	SysTick->LOAD = 0x11940;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
}

void DelayMs(unsigned long t)
{
	for(;t>0;t--)
		{
			DelayMillis();
		}
}


void butonanahtar2(void){
		if(GPIOA->IDR & (1<<6)) /// Checking status of PIN ! portA  A9 pini
		{
			durum2=!durum2;
		}
		while(GPIOA->IDR & (1<<6))	
		{
		// DelayMs(1);
		}
	}

int main(void)
{			
	RCC->APB2ENR |= (1<<2); ////  PORT A Aktif
	GPIOA->CRL &= 0xF0FF0FFF; /// A3 ve A6 pini resetlendi
	GPIOA->CRL |= 0x08008000; /// A3 ve A6 pini girisi push pull
	GPIOA->CRH &= 0xFFF0FF0F; /// A9 ve A12 pini resetlendi
	GPIOA->CRH |= 0x00030030; /// A9 ve A12 pini girisi push pull 
GPIOA->ODR |= 0x1000;
GPIOA->ODR |= 0x0200;
systick_init();	
	while(1)
	{
		butonanahtar2();		
		if(GPIOA->IDR & (1<<3)) //a3
		{ 
		 GPIOA->ODR &= ~0x1000;	
			
		}
		else{
		 GPIOA->ODR |= 0x1000;		
		}
		if(durum2==1) //a6
		{
		 GPIOA->ODR |= 0x0200; //led kapali
/////////////////////////////////////////////////////////////////////////////////////////////////////////////15khz
      RCC->CR &= ~0x01000000;//Pll açik
      RCC->CR |=0x00080000;  //CSSC: clock securty system aktif			
	    RCC->CFGR |= ~0x00010000 | 0x00020000 | 0x003C0000;   //pll ayarlarini iptal et
			RCC->CFGR &= 0x00010000 | 0x001C0000;//Pll mul x9 i iptal et
      RCC->CFGR |= 0x00000004;  //HSE system clock olarak seçildi
      RCC->CFGR |= 0x00000001;  //HSE system clock switch olarak seçildi
      RCC->CFGR |= 0x000000F0;	//ahb 512 ye bölündu		
      RCC->CFGR |= 0x00003800;  //apb2 16 ya bölündü
      RCC->CFGR |= 0x00000700;	//apb1 16	ya bölündü	
	}
		else{
		 GPIOA->ODR &=~0x0200;//led açik   72mhz
			RCC->CR |= 0x01000000;//Pll açik
      RCC->CR |=0x00080000;  //CSSC: clock securty system aktif
      RCC->CFGR &= ~0x00000004;  //HSE system clock olarak seçildi
      RCC->CFGR &= ~0x00000001;  //HSE system clock switch olarak seçildi			
      RCC->CFGR &= ~0x000000F0;	 //ahb 512 ye bölündu		
      RCC->CFGR &= ~0x00003800;		//apb2 16 ya bölündü
      RCC->CFGR &= ~0x00000700;  //apb1 16	ya bölündü
      RCC->CFGR |= 0x00000002; //PLL Kaynak saat sinyali olarak ayarlandi   
			  }
  }	
}
