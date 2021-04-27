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
volatile uint16_t desiatki,edinitsi,chislo,i,vibor, sotni, pre=6, cout = 0; // Объявление беззнаковых 8-битных переменных. volatile-тип при котором компилятор не меняет значения. 
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

uint8_t seg_game[] = {
	0b00000000,	 // 0
	0b00000001,	 // 1 // вверх
	0b01000000,	 // 2  // выделяем для самой птицо (1-3) // середина
	0b00001000,	 // 3 // низ
	0b01100011,	 // 4 // верхняя дуга
	0b00001001,	 // 5 // чисто преграды (без пцица)(4-6)
	0b01011100,	 // 6 // не верхняя дуга
	0b01101011,	 // 7 // верхняя и с пиццой
	0b01001001,	 // 8 // преграды с пиццой (7-9)
	0b01011101,	 // 9 // нижняя с птицей
	0b01111001,  // E
	0b01010000,  // R
	0b11010000  // R.
}; // определение массива

bool isgame = false;
bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;
bool flag5 = false;
//sbool timer = false;
int sos = 1,sec = 0, min = 0, change = 0, step = 0;;
int sdfghj= 0;
bool timer = false;
int n = 0; 
bool lox = false;
int speed = 1;
	
void znachenie(uint16_t chislo) //Возвращаем значение 
{
	if(isgame == false){
sec = chislo % 60;
min =  (chislo / 60);

if(min > 9){
	min = 0;
}


sotni = min;
desiatki = sec/10;   // вернуть обратно по возможности
edinitsi = sec%10;
change = 1;
}
if(isgame == true){
	edinitsi = chislo%10;
	desiatki = chislo/10%10;
	sotni = chislo/100;
}
} 

void znachenie_ch(){
	edinitsi = cout%10;
	desiatki = cout/10%10;
	sotni = cout/100;
}


void dot(){//ебать оно работает

	PORTB = 0b11111;	
sos = 4;
_delay_ms(500);
sos = 3;
_delay_ms(500);

				
}

int plus(int a){
		if(sos == 1){
		a++;
		}
		if(sos == 2){
			a = a + 10;
		/*if(a/10%10 >= 6){
			a = 0;
		}*/
		}
		if(sos == 3){
			//a = a + 100;
			a = a + 60;
			//min ++;
			change = 0;
		}
	
		
		/*if (a > 999){
			a = 0;
		}*/
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

			a = a - 60;
			//min --;
			change = 0;
	}
	/*if (a < 0){
		a = 999;
	}*/
	znachenie(a);
	_delay_ms(60);
	return a;
}

void button(){
	
	if(~PINB&(1<<2) && flag5 == false){
		if(timer == true){
			timer = false;
			_delay_ms(1000);
			flag5 = true;
			return;
		}
		
		if(timer == false){
			timer = true;
			_delay_ms(1000);
			return;
		}
		if(PINB&(1<<2) && flag5 == true){
			flag5 = false;
		}
		
		
	}
	
}

int plus_game(int a){
	a = a + 100;
	if (a > 399){
		a = a-100;
	}
	
	
	znachenie(a);
	_delay_ms(60);
	return a;
}


int minus_game(int a){
	a = a - 100;
	if (a < 99){
		a = a+ 100;
	}
	znachenie(a);
	_delay_ms(60);
	return a;
}

int zero(int a){

	znachenie(a);

		a =a -1;
		dot();

	
	if(a >0){
		/*
		if(~PINB&(1<<2)){
			if(timer == true){
			timer = false;
			_delay_ms(1000);
			zero(a);
			}
		if(timer == false){
			timer = true;
			_delay_ms(1000);
			zero(a);
		}
		}*/
	
		
		
	zero(a);
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
	{PORTC = 0b100;
		if(isgame == true){
					if(lox == false){
						PORTD = seg_game[edinitsi];
					}else{
						PORTD = seg_table[edinitsi];
					}
			
		}
		
		
		
		
		if(isgame == false){
		
		if(sos == 1){
			PORTD = seg_table_dot[edinitsi];
		}else{
		PORTD = seg_table[edinitsi];}}
	};
	
	
	if (vibor == 2) //Вывод десяток
	{
		PORTC = 0b10;
		if(isgame == true){
			if(lox == false){
			PORTD = seg_game[desiatki];}
			if(lox == true){
				PORTD = seg_table[desiatki];
			}
		}
		
		if(isgame == false){
		if(sos == 2){
			PORTD = seg_table_dot[desiatki];
		}else{
			PORTD = seg_table[desiatki];
		}
		}
		
	};
	
	
		if (vibor == 3) //Вывод сотни
		{
			PORTC = 0b1;
			
			if(isgame == true){
					if(lox == false){
					PORTD = seg_game[sotni];	}
					if (lox == true){
						PORTD = seg_table[sotni];
					}
			}
			
			if(isgame == false){
			if(sos == 3){
			PORTD = seg_table_dot[sotni];
			}
			else{
			PORTD = seg_table[sotni];	
			}
			}
		};
	
	
	if (vibor == 4) // Обнуление выбора
	{
		vibor = 0;
	};
	vibor++; //Увеличение выбора на 1
	};
	
	
	
	int move(int n){
	step ++;
	if(speed == 1){_delay_ms(100);}
	if(speed == 2){_delay_ms(75);}
	if(speed == 3){_delay_ms(50);}
	if(speed == 4){_delay_ms(25);}
	if(speed == 5){_delay_ms(13);}
	 // йа определяю скорость, кчаю
	
	if (step == 10){
		n =n  + pre;
		return(n);
	}
	
	if (step == 20){
		n = n - pre;
		n =n  + pre * 10;
		return(n);
	//	step = 0;
	}

	if (step == 30){
		n = n - pre * 10;
		n =n  + pre * 100;
		//step = 0;
		/*
		if(sos + n/100 == 7){
			n = sos * 100;
			_delay_ms(1000);
		}*/
		return(n);
		
	}
	
	
}
	
	
	void game(){
		if(cout == 2){
			speed = 2;
		}
		if(cout == 15){
			speed = 3;
		}
		if(cout == 20){
			speed = 4;
		}
		
		if(cout == 25){
			speed = 5;
		}
		
		if (n <= 399){
			sos = n/100;
		}
		
		znachenie(n);
		
		if(~PINB&(1<<0) && flag1 == 0){
			n = plus_game(n);
			_delay_ms(60);
			flag1 = 1;
			change = 0;
		}
		
		if(PINB&(1<<0) && flag1 == 1){
			flag1 = 0;
		}
		

		if(~PINB&(1<<1) && flag2 == false){
			n = minus_game(n);
			_delay_ms(60);
			flag2 = 1;
		}

		if(PINB&(1<<1) && flag2 == 1){
			flag2 = 0;
		}
		
		n =	move(n);

		if(step == 30){

			if( (n/100) == 7){
				if(sos == 1){
					znachenie(900);
				}
				
				if(sos == 2){
					znachenie(800);
				}
				if(sos == 3){
					znachenie(700);
				}
				if(speed == 1){_delay_ms(1000);}
				if(speed == 2){_delay_ms(750);}
				if(speed == 3){_delay_ms(500);}
				if(speed == 4){_delay_ms(250);}
				if(speed == 5){_delay_ms(150);}
				//znachenie(n);
				//_delay_ms(1000);
				step = 0;
				n = sos*100;
				//znachenie(n);
				//	_delay_ms(speed1 * 10); // йа тоже скорость
				cout ++;
				while(1){
					pre = vibor + 3;
					if(pre>=4 && pre <= 6){
					break; //ex pre = 4;
				}else{
					_delay_ms(1);
				}
				}
				
				
				
				
				
				}else{
				lox = true;
				znachenie_ch();
				_delay_ms(10);
				while(1){}
			}

		}

		
	}
	
	void setup(){
		n  = 100;
		sos = 0;
		isgame = true;
		sos = 0; sec = 0; min = 0; change = 0; step = 0;
		while(1){
			game();
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




    while (1)                     
    {
		
	if(flag1 == 1 && flag2 == 1){
		setup();		
	}
		
		PORTB = 0b10111;	// женя пошёл нахуй (никончук) osu!ждаю	
		if (n > 599){
			n = 0;
		}
	if (n < 0){
		n = 599;
		//n = 0;
	}
		znachenie(n);
		

	
		
/*
		if(n != 0){
		
		PORTB = 0b01111;	
		}else{
				PORTB = 0b11111;
		}
		*/

		if(~PINB&(1<<0) && flag1 == 0){
		n = plus(n);
		_delay_ms(60);
		flag1 = 1;
		change = 0;
		}
		
		if(PINB&(1<<0) && flag1 == 1){
			flag1 = 0;
		}
		
		


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
				 sos = 3;
				//timer = true;
			 n = zero(n);}
			
			 znachenie(n);
			_delay_ms(60);
			//flag3 = 1;
		}

		if(PINB&(1<<2) && flag3 == 1){
			flag3 = 0;
		}
		
		if(~PINB&(1<<4) && flag4 == false){
			_delay_ms(60);				
				if(sos <= 3){
					sos = sos + 1;	
				}
				
			if(sos > 3){
				sos = 1;	
			}
			//znachenie(sos);
					
			flag4 = 1;
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
