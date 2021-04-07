/*
 * GccApplication4.cpp
 *
 * Created: 10.03.2021 17:48:40
 * Author : Admin
 */ 
#define F_CPU 1000000UL //Частота процессора 1 МГц
#include <avr/io.h> //подключение библиотеки портов ввода-вывода
#include <util/delay.h> //библиотека задержек
#include <avr/interrupt.h> //библиотека прерываний
volatile uint16_t desiatki,edinitsi,chislo,i,vibor, sotni; // Объявление беззнаковых 8-битных переменных. volatile-тип при котором компилятор не меняет значения. 
uint8_t seg_table[] = {  
	0b00111111,	 // 0     
	0b00000110,	 // 1
	0b01011011,	 // 2 
	0b01001111,	 // 3
	0b01100110,	 // 4
	0b01101101,	 // 5
	0b01111101,	 // 6
	0b00000111,	 // 7
	0b01111111,	 // 8
	0b01101111,	 // 9
	0b01111001,  // E
	0b01010000,  // R
	0b11010000  // R.
}; // определение массива

uint8_t seg_table_dot[] = {
	0b10111111,	 // 0
	0b10000110,	 // 1
	0b11011011,	 // 2
	0b11001111,	 // 3
	0b11100110,	 // 4
	0b11101101,	 // 5
	0b11111101,	 // 6
	0b10000111,	 // 7
	0b11111111,	 // 8
	0b11101111,	 // 9
	0b11111001,  // E
	0b11010000,  // R
	0b11010000  // R.
}; // определение массива


bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;
int sos = 1;
void znachenie(uint16_t chislo) //Возвращаем значение 
{
sotni = chislo/100;
edinitsi = chislo%10; //Выражаем число в единицах
desiatki = (chislo%100)/10; //Выражаем число в десятках
} 

int plus(int a){
		if(sos == 1){
		a++;
		}
		if(sos == 2){
			a = a + 10;
		}
		if(sos == 3){
			a = a + 100;
		}
		
		
		if (a > 999){
			a = 0;
		}
		znachenie(a);
		_delay_ms(60);
		return a;
}

int minus(int a){
	if(sos == 1){
		a--;
	}
	if(sos == 2){
		a = a - 10;
	}
	if(sos == 3){
		a = a - 100;
	}
	if (a < 0){
		a = 999;
	}
	znachenie(a);
	_delay_ms(60);
	return a;
}
int zero(int a){
	a=a - 1;
	znachenie(a);
	_delay_ms(1000);
	if(a >0){
	zero(a);
//	DDRB = 0b0000;
	}
	else{
		//DDRB = 0b0000;
	return a;
	}
}

void err(uint16_t chislo) //Возвращаем значение
{
	sotni = 10;
	desiatki = 11;
	edinitsi = 12;
	
}
ISR(TIMER0_OVF_vect)
{
	if (vibor == 1) //Вывод единиц
	{
		PORTC = 0b100;
		if(sos == 1){
			PORTD = seg_table_dot[edinitsi];
		}else{
		PORTD = seg_table[edinitsi];}
	};
	if (vibor == 2) //Вывод десяток
	{
		PORTC = 0b10;
		if(sos == 2){
			PORTD = seg_table_dot[desiatki];
		}else{
			PORTD = seg_table[desiatki];
		}
		
	};
		if (vibor == 3) //Вывод сотни
		{
			PORTC = 0b1;
			if(sos == 3){
			PORTD = seg_table_dot[sotni];}
			else{
			PORTD = seg_table[sotni];	
			}
		};
	
	
	if (vibor == 4) // Обнуление выбора
	{
		vibor = 0;
	};
	vibor++; //Увеличение выбора на 1
	};
	
	bool kn1(){
		if(~PINB&(1<<0) && flag1 == 0){
			flag1 = 1;
			
			return true;
		}		if(PINB&(1<<0) && flag1 == 1){
			flag1 = 0;
			return false;
		}
		
	}
	
	
	

int main(void) 
{

	DDRB = 0b000000;
	PORTB = 0b011111;
	

	
	
	DDRD = 0xFF; // Все выводы порта D работают на выход
	DDRC = 0b111;  // Только нулевой и первый работают на выход
	TIMSK |=(1<<0);  //Настройка прерывания по переполнению таймера
	TCNT0 = 0; 
	TCCR0 |= (1<<1); 
	TCCR0 &= ~(1<<0);
	TCCR0 &= ~(1<<2); 
	sei();                         //Разрешение вызова прерываний
	int n = 0; 



    while (1)                     
    {
		znachenie(n);
		
		if(kn1() == true){
			
			n = plus(n);
			_delay_ms(60);
		}
		
		
/*
		if(n == 0){
		PORTB = 0b11111;	
		}else{
			PORTB = 0b01111;	
		}
		*/
/*
		if(~PINB&(1<<0) && flag1 == 0){
		n = plus(n);
		_delay_ms(60);
		flag1 = 1;
		}
		
		if(PINB&(1<<0) && flag1 == 1){
			flag1 = 0;
		}
		
		*/
		if(~PINB&(1<<1) && flag2 == false){
			n = minus(n);
			_delay_ms(60);
			flag2 = 1;
		}

		if(PINB&(1<<1) && flag2 == 1){
			flag2 = 0;
		}
		
		if(~PINB&(1<<2) && flag3 == false){
			 if(n != 0){
			 n = zero(n);}
			 znachenie(n);
			_delay_ms(60);
			//flag3 = 1;
		}

		if(PINB&(1<<2) && flag3 == 1){
			flag3 = 0;
		}
		
		if(~PINB&(1<<4) && flag4 == false){
			
			_delay_ms(500);
		
			if(~PINB&(1<<4) && flag4 == false){
					sos++;
			if(sos > 3){
				sos = 1;
			}else{
						n = plus(n);
						_delay_ms(60);
						
				
			}				
			//znachenie(sos);
			//_delay_ms(1000);	
				_delay_ms(60);
			flag4 = 1;
			}
			
			
			
		
		}

		if(PINB&(1<<4) && flag4 == 1){
			flag4 = 0;
		}
		
		/*
		for (i = 00; i<=1000; i++)  
		{     
		if(i == 1000){
        err(i);
		_delay_ms(10000);
		}
		if(i < 900)	{                  
		znachenie(i);          // вызов функции "значение"
		_delay_ms(10);       //Таймер между действиями, 100 миллисекунд
		}
		if (i >= 900 && i < 1000){
			znachenie(i);          // вызов функции "значение"
			_delay_ms(100);
			
		}

		};
		*/
		}
	}
	
	

