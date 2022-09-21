#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "music_frequency.h"
#include "./NUC100Series.h"
#include "./MCU_init.h"
#include "./SYS_init.h"
#include "./LCD.h"
#include "./picture.h"
#include "./Draw2D.h"
#define BUZZER_BEEP_TIME 3
#define BUZZER_BEEP_DELAY 200000
#define BOUNCING_DLY 200000
#define SYSTICK_DLAY_us 1000000
#define SNAKE_MOVEMENT_TM 500000
#define MAX_LENGTH 20
#define BLOCK_SIZE 5

unsigned short int loadMusic[32] = {
	D4, B3, D4, 0, 0, 0, D4, B3,
	D4, A3, D4, 0, 0, 0, D4, A3,
	E4b, B3, E4b, 0, 0, 0, E4b, B3,
	C4, 0 , C4, 0, C4, B3, A3, B3 };


void drawBackground();
void Tutorial();
void Delay(long double);
void Move();
void generateFood();
void drawWall(uint8_t x, uint8_t y);
void PWM_Freq(unsigned int freq, unsigned char duty_cycle);

// Function System
void System_Config(void);
void SPI3_Config(void);
void LCD_start(void);
void LCD_command(unsigned char temp);
void LCD_data(unsigned char temp);
void LCD_clear(void);
void LCD_SetAddress(uint8_t PageAddr, uint8_t ColumnAddr);
void KeyPadEnable(void);
void Timer_Config();
void spawn();
void displayDifficult();
void setDifficulty();
void countingTime();
void printWin();
void printEndGame();
void Buzzer_beep(int beep_time);
void randomize(int arr[], int n);
void swap (int *a, int *b);

uint8_t KeyPadScanning(void);
uint8_t control = 0;
uint8_t game_pad = 0;
typedef enum {menu_screen, tutorial_screen, chooseDifficult, game1, game2, game3, you_win, you_lose} STATES;
typedef enum {EASY, NORMAL, HARD} difficulty;
volatile int game_update;
difficulty game_difficulty;
int count = 0;
volatile int num_food = 2;// maximum food appear on screen 2
volatile int num_wall = 100;
 
typedef enum {UP, DOWN, LEFT, RIGHT} direction; // direction of the snake
volatile direction snake_direction = RIGHT; // By default direction of snake is on the right
void draw_snake(); // Function to draw snake
void snake_grow(); // Function to make a snake grow when it eats food
void draw_object(uint8_t x, uint8_t y, char name);
void undraw_object(uint8_t x, uint8_t y, char name);
void snake_auto_move(); // Function that make snake automatically move
void undraw_snake_tail(); // Function to delete snake tail
void undraw_snake(); // Function to delete snake
volatile int snake_speed = 100000; // Velocity of the snake can be choose in function chooseDifficult();
volatile int snake_length = 3; // Initial length 3 blocks.
uint8_t snake_x=64, snake_y=32, snake_dx=0, snake_dy=0; // Initial position spawn of the snake
uint8_t score = 0; 
void draw_body(uint8_t x, uint8_t y, char name);
uint8_t check_collision();
void obj_init();
void game_init();
void undrawWall(uint8_t x, uint8_t y);
void undraw_background();
void PWM4_Init(void);

volatile int running_time = 229376;
// Create an object
struct Object {
	 char name;
	 int x;
	 int y;
};
// Body part of snake
struct Object Snake0 = {'s', 70, 30};
struct Object Snake1 = {'s', 65, 30};
struct Object Snake2 = {'s', 60, 30};
struct Object temp_tail = {'s', 60, 30};
	
// Food initialize
struct Object Food0 = {'f', 30, 30};
struct Object Food1;
	
// Wall initialize
struct Object Wall1 = { 'w', 0, 0 };
struct Object Wall2 = { 'w', 5, 0 };
struct Object Wall3 = { 'w', 10, 0 };
struct Object Wall4 = { 'w', 15, 0 };
struct Object Wall5 = { 'w', 20, 0 };
struct Object Wall6 = { 'w', 25, 0 };
struct Object Wall7 = { 'w', 30, 0 };
struct Object Wall8 = { 'w', 35, 0 };
struct Object Wall9 = { 'w', 40, 0 };
struct Object Wall10 = { 'w', 45, 0 };
struct Object Wall11 = { 'w', 50, 0 };
struct Object Wall12 = { 'w', 55, 0 };
struct Object Wall13 = { 'w', 60, 0 };
struct Object Wall14 = { 'w', 65, 0 };
struct Object Wall15 = { 'w', 70, 0 };
struct Object Wall16 = { 'w', 75, 0 };
struct Object Wall17 = { 'w', 80, 0 };
struct Object Wall18 = { 'w', 85, 0 };
struct Object Wall19 = { 'w', 90, 0 };
struct Object Wall20 = { 'w', 95, 0 };
struct Object Wall21 = { 'w', 100, 0 };
struct Object Wall22 = { 'w', 105, 0 };
struct Object Wall23 = { 'w', 110, 0 };
struct Object Wall24 = { 'w', 115, 0 };
struct Object Wall25 = { 'w', 120, 0 };
struct Object Wall26 = { 'w', 125, 0 };
struct Object Wall27 = { 'w', 0, 5 };
struct Object Wall28 = { 'w', 0, 10 };
struct Object Wall29 = { 'w', 0, 15 };
struct Object Wall30 = { 'w', 0, 20 };
struct Object Wall31 = { 'w', 0, 25 };
struct Object Wall32 = { 'w', 0, 30 };
struct Object Wall33 = { 'w', 0, 35 };
struct Object Wall34 = { 'w', 0, 40 };
struct Object Wall35 = { 'w', 0, 45 };
struct Object Wall36 = { 'w', 0, 50 };	
struct Object Wall37 = { 'w', 0, 55 };
struct Object Wall38 = { 'w', 0, 55 };
struct Object Wall39 = { 'w', 5, 55 };
struct Object Wall40 = { 'w', 10, 55};
struct Object Wall41 = { 'w', 15, 55 };
struct Object Wall42 = { 'w', 20, 55 };
struct Object Wall43 = { 'w', 25, 55 };
struct Object Wall44 = { 'w', 30, 55 };
struct Object Wall45 = { 'w', 35, 55 };	
struct Object Wall46 = { 'w', 40, 55 };
struct Object Wall47 = { 'w', 45, 55 };
struct Object Wall48 = { 'w', 50, 55 };
struct Object Wall49 = { 'w', 55, 55 };
struct Object Wall50 = { 'w', 60, 55 };
struct Object Wall51 = { 'w', 65, 55 };
struct Object Wall52 = { 'w', 70, 55 };
struct Object Wall53 = { 'w', 75, 55 };
struct Object Wall54 = { 'w', 80, 55 };
struct Object Wall55 = { 'w', 85, 55 };
struct Object Wall56 = { 'w', 90, 55 };
struct Object Wall57 = { 'w', 95, 55 };
struct Object Wall58 = { 'w', 100, 55};
struct Object Wall59 = { 'w', 105, 55};
struct Object Wall60 = { 'w', 110, 55};
struct Object Wall61 = { 'w', 115, 55};
struct Object Wall62 = { 'w', 120, 55};
struct Object Wall63 = { 'w', 120, 0 };
struct Object Wall64 = { 'w', 120, 10 };
struct Object Wall65 = { 'w', 120, 15 };
struct Object Wall66 = { 'w', 120, 20 };
struct Object Wall67 = { 'w', 120, 25 };	
struct Object Wall68 = { 'w', 120, 30 };
struct Object Wall69 = { 'w', 120, 35 };
struct Object Wall70 = { 'w', 120, 40 };
struct Object Wall71 = { 'w', 120, 45 };
struct Object Wall72 = { 'w', 120, 50 };
struct Object Wall73 = { 'w', 120, 55 };
struct Object Wall74 = { 'w', 120, 60 };
struct Object Wall75 = { 'w', 120, 65 };
struct Object Wall76 = { 'w', 120, 5 };

struct Object SnakeBody[50] = {};
struct Object Food[2] = {};
struct Object Wall[100] = {};
// Time variables
volatile int minute = 0;
volatile int second = 0;
char minute_str[] = "00";
char second_str[] = "00";
// Score variables
char score_s[2] = "";
int flag = 0;
int randomX[] = {15,25,30,35,40,45, 50, 55,60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115};
int randomY[] = {10, 15, 20, 25, 30, 35, 40, 45, 50};
int reset = 0;
/*** MAIN Function ***/
int main()
{

		STATES game_state;
		uint8_t difficult_key = 0;
		uint8_t i, key_pressed = 0, game_pad=0;
		uint8_t previousX, previousY;
		uint8_t x_steps = 2; 
		uint8_t y_steps = 2;
		uint8_t x_reset = 64; 
		uint8_t y_reset = 32;
		uint8_t hit = 0;
		int length = 1;
	
		System_Config(); 
		
		//--------------------------------
		//SPI3 initialization
		//--------------------------------
		PWM4_Init();
		SPI3_Config();
		Timer_Config();
		KeyPadEnable();
		//--------------------------------
		//LCD initialization
		//--------------------------------
		LCD_start();
		LCD_clear();
		//--------------------------------
		//LCD static content
		//--------------------------------
		//--------------------------------
		//LCD dynamic content
		//--------------------------------
		uint8_t pressed_key = 0;
		game_state = menu_screen;
		reset_loop:
		while(KeyPadScanning() == 0){
		while (1){
				switch(game_state){
						case menu_screen:	
							resetting:
								draw_LCD(welcome_snake);
								for (i=0;i<5;i++) CLK_SysTickDelay(SYSTICK_DLAY_us);
								LCD_clear();
								game_state = tutorial_screen;
								break;	
						
						case tutorial_screen:  
								Tutorial();
								for (i=0;i<5;i++) CLK_SysTickDelay(SYSTICK_DLAY_us);
								LCD_clear();
								game_state = chooseDifficult;			
								break;
						case chooseDifficult:
							difficulty:
								LCD_clear();
								displayDifficult();
								while(difficult_key == 0) difficult_key = KeyPadScanning();
								switch(difficult_key){
										case 1: game_difficulty = EASY; game_state = game1; setDifficulty(); difficult_key = 0; break;
										case 2: game_difficulty = NORMAL; game_state = game2; setDifficulty(); difficult_key = 0; break;
										case 3: game_difficulty = HARD; game_state = game3; setDifficulty(); difficult_key = 0; break;
									default: difficult_key = 0; goto difficulty;
								}
								break;
						
						// Easy Mode
						case game1:
								
								LCD_clear();
								obj_init();
				
								while(1) {
										start:
										generateFood();
										draw_object(Food0.x, Food0.y, 'f');
										TIMER0->TCMPR = running_time;
										while(!(TIMER0->TDR == running_time)){ // Wait for a certain interval defined by the difficulty selected (Easy/Medium/Hard)		
												Move(); // Allows user to redirect the snake
												if(TIMER0->TDR % 32768 == 0){ // Every one second update the time 
														countingTime();
												}
												if (reset == 1){ // If bool is 1
														undraw_snake();
														//undraw_background();
														undraw_object(Food0.x, Food0.y, 'f');
														snake_direction = RIGHT;
														game_init();
														reset = 0;
														goto reset_loop;		
														break;
												}
												if ((TIMER0->TDR % snake_speed == 0)){ // Timer defines the speed of the snake
														// Create a temporary tail to store the last coordinate of the tail that it moved so later on in snake_grow function we can add this tail
														temp_tail.x = SnakeBody[snake_length - 1].x;
														temp_tail.y = SnakeBody[snake_length - 1].y;
														undraw_snake_tail(); // Undraw the coordinate that the tail went
														snake_auto_move(); // Snake will automatically move depends on the direction of the snake
														draw_snake(); // Display snake on the screen 
														drawBackground(); // Display walls
														int check = check_collision(); // Check collision between snake on itself, walls, food
														if (check == 1){ // If snake eats itself or hit the wall
																undraw_snake();
																undraw_background();
																goto end_of_game;
														}
														else if (check == 2) goto start; // If snake eats food
														if (snake_length == 11){ // If length of the snake is 11 or score 8 then win the round
																goto win_game;
														}
													}
								
												}
												undraw_object(Food0.x, Food0.y, 'f'); // Undraw food after a period of time
									}
									end_of_game:
									undraw_snake();
									undraw_object(Food0.x, Food0.y, 'f');
									game_state = you_lose;
									break;
						
									win_game:
									undraw_snake();
									undraw_object(Food0.x, Food0.y, 'f');
									game_state = you_win;
									break;
						
						// Medium Mode
						case game2:
											
								LCD_clear();
								obj_init();
				
								while(1) {
										start2:
										generateFood();
										draw_object(Food0.x, Food0.y, 'f');
										TIMER0->TCMPR = running_time;
										while(!(TIMER0->TDR == running_time)){ // Wait for a certain interval defined by the difficulty selected (Easy/Medium/Hard)		
												Move(); // Allows user to redirect the snake
												if(TIMER0->TDR % 32768 == 0){ // Every one second update the time 
														countingTime();
												}
												if (reset == 1){ // If bool is 1
														undraw_snake();
														//undraw_background();
														undraw_object(Food0.x, Food0.y, 'f');
														snake_direction = RIGHT;
														game_init();
														reset = 0;
														goto reset_loop;		
														break;
												}
												if ((TIMER0->TDR % snake_speed == 0)){ // Timer defines the speed of the snake
														// Create a temporary tail to store the last coordinate of the tail that it moved so later on in snake_grow function we can add this tail
														temp_tail.x = SnakeBody[snake_length - 1].x;
														temp_tail.y = SnakeBody[snake_length - 1].y;
														undraw_snake_tail(); // Undraw the coordinate that the tail went
														snake_auto_move(); // Snake will automatically move depends on the direction of the snake
														draw_snake(); // Display snake on the screen 
														drawBackground(); // Display walls
														int check = check_collision(); // Check collision between snake on itself, walls, food
														if (check == 1){ // If snake eats itself or hit the wall
																undraw_snake();
																undraw_background();
																goto end_of_game2;
														}
														else if (check == 2) goto start2; // If snake eats food
														if (snake_length == 11){ // If length of the snake is 11 or score 8 then win the round
																goto win_game2;
														}
													}
								
												}
												undraw_object(Food0.x, Food0.y, 'f'); // Undraw food after a period of time
									}
									end_of_game2:
									undraw_snake();
									undraw_object(Food0.x, Food0.y, 'f');
									game_state = you_lose;
									break;
						
									win_game2:
									undraw_snake();
									undraw_object(Food0.x, Food0.y, 'f');
									game_state = you_win;
									break;
						// Hard Mode
						case game3:
											
								LCD_clear();
								obj_init();
				
								while(1) {
										start3:
										generateFood();
										draw_object(Food0.x, Food0.y, 'f');
										TIMER0->TCMPR = running_time;
										while(!(TIMER0->TDR == running_time)){ // Wait for a certain interval defined by the difficulty selected (Easy/Medium/Hard)		
												Move(); // Allows user to redirect the snake
												if(TIMER0->TDR % 32768 == 0){ // Every one second update the time 
														countingTime();
												}
												if (reset == 1){ // If bool is 1
														undraw_snake();
														//undraw_background();
														undraw_object(Food0.x, Food0.y, 'f');
														snake_direction = RIGHT;
														game_init();
														reset = 0;
														goto reset_loop;		
														break;
												}
												if ((TIMER0->TDR % snake_speed == 0)){ // Timer defines the speed of the snake
														// Create a temporary tail to store the last coordinate of the tail that it moved so later on in snake_grow function we can add this tail
														temp_tail.x = SnakeBody[snake_length - 1].x;
														temp_tail.y = SnakeBody[snake_length - 1].y;
														undraw_snake_tail(); // Undraw the coordinate that the tail went
														snake_auto_move(); // Snake will automatically move depends on the direction of the snake
														draw_snake(); // Display snake on the screen 
														drawBackground(); // Display walls
														int check = check_collision(); // Check collision between snake on itself, walls, food
														if (check == 1){ // If snake eats itself or hit the wall
																undraw_snake();
																undraw_background();
																goto end_of_game3;
														}
														else if (check == 2) goto start3; // If snake eats food
														if (snake_length == 11){ // If length of the snake is 11 or score 8 then win the round
																goto win_game3;
														}
													}
								
												}
												undraw_object(Food0.x, Food0.y, 'f'); // Undraw food after a period of time
									}
									end_of_game3:
									undraw_snake();
									undraw_object(Food0.x, Food0.y, 'f');
									game_state = you_lose;
									break;
						
									win_game3:
									undraw_snake();
									undraw_object(Food0.x, Food0.y, 'f');
									game_state = you_win;
									break;
						case you_lose:
						printEndGame(); // Print out score and time
						game_init();
						game_state = menu_screen;
						break;
						
						case you_win:
						printWin(); // Print out score and time
						game_init();
						game_state = menu_screen;
						break;
						
						break;
						default: break;
						}	 
			}
		}
}

void LCD_start(void)
{
		LCD_command(0xE2);
		LCD_command(0xA1);
		LCD_command(0xEB);
		LCD_command(0x81);
		LCD_command(0xA0);
		LCD_command(0xC0);
		LCD_command(0xAF);
}

void LCD_command(unsigned char temp)
{
		SPI3->SSR |= 1ul << 0;
		SPI3->TX[0] = temp;
		SPI3->CNTRL |= 1ul << 0;
		while(SPI3->CNTRL & (1ul << 0));
		SPI3->SSR &= ~(1ul << 0);
}

void LCD_data(unsigned char temp)
{
		SPI3->SSR |= 1ul << 0;
		SPI3->TX[0] = 0x0100+temp;
		SPI3->CNTRL |= 1ul << 0;
		while(SPI3->CNTRL & (1ul << 0));
		SPI3->SSR &= ~(1ul << 0);
}

void LCD_clear(void)
{
		int16_t i;
		LCD_SetAddress(0x0, 0x0);
		for (i = 0; i < 132 *8; i++)
		{
				LCD_data(0x00);
		}
}

void LCD_SetAddress(uint8_t PageAddr, uint8_t ColumnAddr)
{
		LCD_command(0xB0 | PageAddr);
		LCD_command(0x10 | (ColumnAddr>>4)&0xF);
		LCD_command(0x00 | (ColumnAddr & 0xF));
}

void KeyPadEnable(void){
		GPIO_SetMode(PA, BIT0, GPIO_MODE_QUASI);
		GPIO_SetMode(PA, BIT1, GPIO_MODE_QUASI);
		GPIO_SetMode(PA, BIT2, GPIO_MODE_QUASI);
		GPIO_SetMode(PA, BIT3, GPIO_MODE_QUASI);
		GPIO_SetMode(PA, BIT4, GPIO_MODE_QUASI);
		GPIO_SetMode(PA, BIT5, GPIO_MODE_QUASI);
}

uint8_t KeyPadScanning(void){
		PA0=1; PA1=1; PA2=0; PA3=1; PA4=1; PA5=1;
		if (PA3==0) return 1;
		if (PA4==0) return 4;
		if (PA5==0) return 7;
		PA0=1; PA1=0; PA2=1; PA3=1; PA4=1; PA5=1;
		if (PA3==0) return 2;
		if (PA4==0) return 5;
		if (PA5==0) return 8;
		PA0=0; PA1=1; PA2=1; PA3=1; PA4=1; PA5=1;
		if (PA3==0) return 3;
		if (PA4==0) return 6;
		if (PA5==0) return 9;
		return 0;
}

void System_Config (void){
		SYS_UnlockReg(); // Unlock protected registers
		// HXT Clock 
		CLK->PWRCON |= (0x01ul << 0);
		while(!(CLK->CLKSTATUS & (1ul << 0)));
	  // LXT Clock
		CLK->PWRCON |= (0x01ul << 1);
		while(!(CLK->CLKSTATUS & (1ul << 1)));
		//PLL configuration starts
		CLK->PLLCON &= ~(1ul<<19); //0: PLL input is HXT
		CLK->PLLCON &= ~(1ul<<16); //PLL in normal mode
		CLK->PLLCON &= (~(0x01FFul << 0));
		// FB_DV = 48
		CLK->PLLCON |= 48; // 50MHz
		CLK->PLLCON &= ~(1ul<<18); //0: enable PLLOUT
		while(!(CLK->CLKSTATUS & (0x01ul << 2)));
		//PLL configuration ends

		//clock source selection
		// HCLK_S = 010 => PLLFOUT
		CLK->CLKSEL0 &= (~(0x07ul << 0));
		CLK->CLKSEL0 |= (0x02ul << 0);
		//clock frequency division
		CLK->CLKDIV &= (~0x0Ful << 0);
		//enable clock of SPI3
		CLK->APBCLK |= 1ul << 15;
		PB->PMD &= ~(3<<30);
		PB->PMD |= (1<<30);
		PB->DBEN &= ~(1<<15);
		PB->DBEN |= (1<<15);
		PB->IMD &= (~1ul << 15);
		PB->IEN |= (1ul << 15);

		NVIC->ISER[0] |= 1ul << 3;
		NVIC->IP[0] &= (~(3ul << 30));
		
		CLK->CLKSEL0 &= ~(0x07ul << 0);
		CLK->CLKSEL0 |= (0x02ul << 0); 							//select HCLK PLL = 50MHz
		CLK->CLKDIV &= ~(0x0Ful);                  
		CLK->CLKSEL0 |= (0x03ul << 3);
		CLK->CLKSEL2 |= (0x2ul << 4);
		CLK-> APBCLK |=(0x1ul << 22);
		SYS->ALT_MFP |= (0x1ul <<4);
		SYS->GPB_MFP |= (0x01ul << 11);
		SYS_LockReg(); // Lock protected registers
}

void SPI3_Config (void){
		SYS->GPD_MFP |= 1ul << 11; //1: PD11 is configured for alternative function
		SYS->GPD_MFP |= 1ul << 9; //1: PD9 is configured for alternative function
		SYS->GPD_MFP |= 1ul << 8; //1: PD8 is configured for alternative function

		SPI3->CNTRL &= ~(1ul << 23); //0: disable variable clock feature
		SPI3->CNTRL &= ~(1ul << 22); //0: disable two bits transfer mode
		SPI3->CNTRL &= ~(1ul << 18); //0: select Master mode
		SPI3->CNTRL &= ~(1ul << 17); //0: disable SPI interrupt
		SPI3->CNTRL |= 1ul << 11; //1: SPI clock idle high
		SPI3->CNTRL &= ~(1ul << 10); //0: MSB is sent first
		SPI3->CNTRL &= ~(3ul << 8); //00: one transmit/receive word will be executed in one data transfer

		SPI3->CNTRL &= ~(31ul << 3); //Transmit/Receive bit length
		SPI3->CNTRL |= 9ul << 3; //9: 9 bits transmitted/received per data transfer

		SPI3->CNTRL |= (1ul << 2); //1: Transmit at negative edge of SPI CLK
		SPI3->DIVIDER = 0; // SPI clock divider. SPI clock = HCLK / ((DIVIDER+1)*2). HCLK = 50 MHz
}

void Timer_Config(){
		//Timer 0 Config to count time for the game LXT: 32.768kHz
		CLK->CLKSEL1 &= ~(0x07ul << 8);
		CLK->CLKSEL1 |= (0x01ul << 8);
		CLK->APBCLK |= (0x01ul << 2);
		TIMER0->TCSR &= ~(0xFFul << 0);
		//reset Timer 0
		TIMER0->TCSR |= (0x01ul << 26);
		//define Timer 0 operation mode
		TIMER0->TCSR &= ~(0x03ul << 27);
		TIMER0->TCSR |= (0x01ul << 27);
		TIMER0->TCSR &= ~(0x01ul << 24);
		//TDR to be updated continuously while timer bomb_freqer is bomb_freqing
		TIMER0->TCSR |= (0x01ul << 16);
		//TimeOut = 7s --> Counter's TCMPR = 7s / (1/(32768 Hz) = 262144
		TIMER0->TCMPR = 229376;
		//start bomb_freqing
		TIMER0->TCSR |= (0x01ul << 30);
		
		
		// Timer 1 Config to have background music
		CLK->CLKSEL1 &= ~(0x07ul << 12);            // configure for Timer 1, Clock source from HCLK
		CLK->CLKSEL1 |= (0x02ul << 12);							// HCLK -> PLLFOUT = 50MHz
		CLK->APBCLK |= (0x01ul << 3); 
	
		NVIC_EnableIRQ(TMR1_IRQn);	
		TIMER1->TCSR |= (0x01ul << 27); //MODE =01,timer operates in periodic mode
	
		TIMER1->TCSR &= ~(0xFFul << 0); //Clear prescale
		
		TIMER1->TCSR |= (0x01ul << 16);
	
		TIMER1->TCSR &= ~(0x1ul << 24);
	
		TIMER1->TCSR &= ~(0x01ul << 29);
		TIMER1->TCSR |= (0x1ul << 29);
		//  246.1094970703125
		TIMER1->TCMPR = 8064516; 
	
		TIMER1->TCSR |= (0x01ul << 30);
		TIMER1->TCSR &= ~(0x01ul << 26);
}


// Button to reset game
void EINT1_IRQHandler(void){
		//Buzzer_beep(BUZZER_BEEP_TIME);
		reset = 1;
		PB->ISRC |= (1ul << 15);
}

// Function to draw walls
void drawBackground(){
		for (int i = 0; i < 77; i++){
				drawWall(Wall[i].x, Wall[i].y);
		}
}

// Function to print out tutorials
void Tutorial(){
		printS_5x7(1, 0, "Use Keypad to control");
		printS_5x7(1, 8, "2: UP 4: LEFT");
		printS_5x7(1, 16, "6: RIGHT 8: DOWN");
		printS_5x7(1, 32, "Get 8 points to win");
}	

// Function allows user to redirect the snake
void Move(){
	
		game_pad= KeyPadScanning();
		switch(game_pad) {
		// Move up
		case 2: 
				if (snake_direction != DOWN){
				snake_direction = UP;
			}
			break;
		// Move left
		case 4:
				if (snake_direction != RIGHT){
				snake_direction = LEFT;
				}

				break;
		// Move right
		case 6: 
				if (snake_direction != LEFT){
				snake_direction = RIGHT;
				}

				break;
		// Move down
		case 8: 
				if (snake_direction != UP){
				snake_direction = DOWN;
				}
			break;
		// Default
		default: break;
		}
		game_pad = 0;
}

/*** Snake automatically move in direction that user is controlling ***/
void snake_auto_move(){
		if (snake_direction == UP){
				for (int i = snake_length - 1; i > 0; i--){		
						SnakeBody[i].y = SnakeBody[i-1].y;
						SnakeBody[i].x = SnakeBody[i-1].x;
				}
				SnakeBody[0].y -= BLOCK_SIZE;
		}	else if (snake_direction == DOWN){
				for (int i = snake_length - 1; i > 0; i--){		
						SnakeBody[i].y = SnakeBody[i-1].y;
						SnakeBody[i].x = SnakeBody[i-1].x;
			}
			SnakeBody[0].y += BLOCK_SIZE;
		} else if (snake_direction == LEFT){
				for (int i = snake_length - 1; i > 0; i--){		
						SnakeBody[i].x = SnakeBody[i-1].x;
						SnakeBody[i].y = SnakeBody[i-1].y;
				}
			SnakeBody[0].x -= BLOCK_SIZE;
		} else if (snake_direction == RIGHT){
				for (int i = snake_length - 1; i > 0; i--){	
						SnakeBody[i].y = SnakeBody[i-1].y;			
						SnakeBody[i].x = SnakeBody[i-1].x;
		}
		SnakeBody[0].x += BLOCK_SIZE;
	}
	
}

/*** Generate food by finding random element in array randomX and randomY ***/
void generateFood(){
		int food_x, food_y, check = 0;
		randomize(randomX, 20);
		randomize(randomY, 9);
		while (1){
				food_x = randomX[rand() % 20];
				food_y = randomY[rand() % 9];
				for( int i = 0; i < snake_length; i++){
						if( food_x == SnakeBody[i].x && food_y == SnakeBody[i].y){
								check = 1;
								break;
						}
				}
				if (check == 0) break;
	}
	
	Food0.x = food_x;
	Food0.y = food_y;
	
}

// Function to display whole body of the snake
void draw_snake(){
		for (int i = 0; i < snake_length; i++){
				draw_object(SnakeBody[i].x, SnakeBody[i].y, 's');
		}
}

//Function to delete snake tail
void undraw_snake_tail(){
	fill_Rectangle(SnakeBody[snake_length-1].x, SnakeBody[snake_length-1].y, SnakeBody[snake_length-1].x + BLOCK_SIZE - 1, SnakeBody[snake_length-1].y + BLOCK_SIZE - 1, 0, 0);
}

//Function to delete snake tail
void undraw_snake(){
	for (int i = 0; i < snake_length; i++)
			fill_Rectangle(SnakeBody[i].x, SnakeBody[i].y, SnakeBody[i].x + BLOCK_SIZE - 1, SnakeBody[i].y + BLOCK_SIZE - 1, 0, 0);
}

void undraw_background(){
		for (int i = 0; i < 77; i++){
				undrawWall(Wall[i].x, Wall[i].y);
		}
}
// Function display difficulty
void displayDifficult(){
		printS_5x7(15, 15, "1. Easy Mode");
		printS_5x7(15, 23, "2. Medium Mode");
		printS_5x7(15, 31, "3. Hard Mode");
}

// Function to draw object if name == 's' or 'f'
void draw_object(uint8_t x, uint8_t y, char name){
		if (name == 's'){
				fill_Rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, 1, 0);
		} else if (name == 'f'){
				draw_Rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, 1, 0);
		}
}

// Function to undraw food
void undraw_object(uint8_t x, uint8_t y, char name){
		if (name == 'f'){
				draw_Rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, 0, 0);
		}
}

void TMR1_IRQHandler(void){
	static int i=0;
	PWM_Freq(loadMusic[i],90);
	i++;
	if(i >= 32) i=0; 
	TIMER1->TISR |= (0x01ul << 0);
}

// Buzzer beep
void Buzzer_beep(int beep_time){
		int i;
		for(i=0;i<(beep_time*2);i++){
				PB->DOUT ^= (1 << 11);
				CLK_SysTickDelay(BUZZER_BEEP_DELAY);
		}
}

// Insert all Object to Object List
void obj_init(){
		// Snake has 3 body parts
		SnakeBody[0] = Snake0;
		SnakeBody[1] = Snake1;
		SnakeBody[2] = Snake2;

		// Wall list assign
		Wall[0] = Wall1;
		Wall[1] = Wall2;
		Wall[2] = Wall3;
		Wall[3] = Wall4;
		Wall[4] = Wall5;
		Wall[5] = Wall6;
		Wall[6] = Wall7;
		Wall[7] = Wall8;
		Wall[8] = Wall9;
		Wall[9] = Wall10;
		Wall[10] = Wall11;
		Wall[11] = Wall12;
		Wall[12] = Wall13;
		Wall[13] = Wall14;
		Wall[14] = Wall15;
		Wall[15] = Wall16;
		Wall[16] = Wall17;
		Wall[17] = Wall18;
		Wall[18] = Wall19;
		Wall[19] = Wall20;
		Wall[20] = Wall21;
		Wall[21] = Wall22;
		Wall[22] = Wall23;
		Wall[23] = Wall24;
		Wall[26] = Wall27;
		Wall[27] = Wall28;
		Wall[28] = Wall29;
		Wall[29] = Wall30;
		Wall[30] = Wall31;
		Wall[31] = Wall32;
		Wall[32] = Wall33;
		Wall[33] = Wall34;
		Wall[34] = Wall35;
		Wall[35] = Wall36;
		Wall[36] = Wall37;
		Wall[37] = Wall38;
		Wall[38] = Wall39;
		Wall[39] = Wall40;
		Wall[40] = Wall41;
		Wall[41] = Wall42;
		Wall[42] = Wall43;
		Wall[43] = Wall44;
		Wall[44] = Wall45;
		Wall[45] = Wall46;
		Wall[46] = Wall47;
		Wall[47] = Wall48;
		Wall[48] = Wall49;
		Wall[49] = Wall50;
		Wall[50] = Wall51;
		Wall[51] = Wall52;
		Wall[52] = Wall53;
		Wall[53] = Wall54;
		Wall[54] = Wall55;
		Wall[55] = Wall56;
		Wall[56] = Wall57;
		Wall[57] = Wall58;
		Wall[58] = Wall59;
		Wall[59] = Wall60;
		Wall[60] = Wall61;
		Wall[61] = Wall62;
		Wall[62] = Wall63;
		Wall[63] = Wall64;
		Wall[64] = Wall65;
		Wall[65] = Wall66;
		Wall[66] = Wall67;
		Wall[67] = Wall68;
		Wall[68] = Wall69;
		Wall[69] = Wall70;
		Wall[70] = Wall71;
		Wall[71] = Wall72;
		Wall[72] = Wall73;
		Wall[74] = Wall75;
		Wall[75] = Wall76;


}

// Function to assign snake speed for difficult level
void setDifficulty(){
		if (game_difficulty == EASY){
			// 0.244140625
				snake_speed = 8000;
		} else if (game_difficulty == NORMAL){
				snake_speed = 5000;
				
		} else if (game_difficulty == HARD){
				snake_speed = 3000;
				
		}
		TIMER0->TCMPR = running_time;
}

// Function to draw wall
void drawWall(uint8_t x, uint8_t y) {
		draw_Rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, 1, 0);
		draw_Line(x, y, x+ BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, 1, 0);
		draw_Line(x + BLOCK_SIZE - 1, y, x, y + BLOCK_SIZE - 1, 1, 0);
}

// Function to undraw wall
void undrawWall(uint8_t x, uint8_t y) {
		draw_Rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, 0, 0);
		draw_Line(x, y, x+ BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, 0, 0);
		draw_Line(x + BLOCK_SIZE - 1, y, x, y + BLOCK_SIZE - 1, 0, 0);
}

// Function snake grows when it eats food
void snake_grow() {
		if(snake_length <= 13){
				snake_length ++;
				struct Object new_tail ={'s', temp_tail.x, temp_tail.y};
				SnakeBody[snake_length -1] = new_tail;
		}	
}

// Function to check whether snake met these conditions
uint8_t check_collision(){
	
		// Check if snake bites itself
		for (int i = 1; i < snake_length; i++){
				if (SnakeBody[0].x == SnakeBody[i].x && SnakeBody[0].y == SnakeBody[i].y)
					return 1;
		}

		// Check if snake hits the wall
		for (int i = 0; i < num_wall; i++){
				if (SnakeBody[0].x == Wall[i].x && SnakeBody[0].y == Wall[i].y)
					return 1;
		}

		// Check if snake eats food then it will grow up and get score
		if (SnakeBody[0].x == Food0.x && SnakeBody[0].y == Food0.y){
				score++;
				snake_grow();
				TIMER0->TCMPR = running_time;
				return 2;
		}
		return 0;
}

// Iniatilize game
void game_init(){

		obj_init();
	  score = 0;
		
		second = 0;
		minute = 0;
		second_str[0] = '0';
		second_str[1] = '0';
		minute_str[0] = '0';
		minute_str[0] = '0';
		
		snake_speed = 8000;
		snake_length = 3;
}

// Function clock time
void countingTime(){
	second++;
	if(second > 59){
			second = 0;
			minute++;
	}
	if (minute >= 0){
			if (minute < 10){
					minute_str[0] = '0';
			if (minute == 1) 
					minute_str[1] = '1';
			else if (minute == 2)
					minute_str[1] = '2';
			else if (minute == 3)
					minute_str[1] = '3';
			else if (minute == 4)
					minute_str[1] = '4';
			else if (minute == 5)
					minute_str[1] = '5';
			else if (minute == 6)
					minute_str[1] = '6';
			else if (minute == 7)
					minute_str[1] = '7';
			else if (minute == 8)
					minute_str[1] = '8';
			else if (minute == 9)
					minute_str[1] = '9';
			else minute_str[1] = '0';
		}
		else 
			sprintf(minute_str, "%d", minute);
	}
	if (second >= 0){
			if (second < 10){
					second_str[0] = '0';
			if (second == 1) 
					second_str[1] = '1';
			else if (second == 2)
					second_str[1] = '2';
			else if (second == 3)
					second_str[1] = '3';
			else if (second == 4)
					second_str[1] = '4';
			else if (second == 5)
					second_str[1] = '5';
			else if (second == 6)
					second_str[1] = '6';
			else if (second == 7)
					second_str[1] = '7';
			else if (second == 8)
					second_str[1] = '8';
			else if (second == 9)
					second_str[1] = '9';
			else 
					second_str[1] = '0';
		}
			else
					sprintf(second_str, "%d", second);
		}
}

// Function to print out time and score when win
void printWin(){
		LCD_clear();
		printS_5x7(33, 20, "You Win");
		printS_5x7(33, 29, "Time: ");
		printS_5x7(58, 29, minute_str);
		printS_5x7(68, 28, ":");
		printS_5x7(72, 28, second_str);
		sprintf(score_s, "%d", score);
		printS_5x7(33, 40, "Score: ");
		printS_5x7(62, 40, score_s);
	for (uint8_t i=0;i<10;i++) CLK_SysTickDelay(10000000);
}

// Function to print out time and score when lose
void printEndGame(){
		LCD_clear();
		printS_5x7(32,20,"Game Over");
		printS_5x7(33, 29, "Time: ");
		printS_5x7(58, 29, minute_str);
		printS_5x7(68, 28, ":");
		printS_5x7(72, 28, second_str);
		sprintf(score_s, "%d", score);
		printS_5x7(33, 37, "Score: ");
		printS_5x7(62, 35, score_s);
		for (uint8_t i=0;i<10;i++) CLK_SysTickDelay(10000000);
	
}

// A function to generate a random permutation of arr[]
void randomize ( int array[], int n )
{
	
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

// A utility function to swap to integers
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void PWM_Freq(unsigned int freq, unsigned char duty_cycle){
	static unsigned short interval, frequency;
	
	if(freq==0){
		interval=0;
		frequency=0;
	}else{
		frequency = 50000000/2/freq; 
		interval = duty_cycle*frequency/100;  
	}
	PWMB->CMR0 = interval;
	PWMB->CNR0 = frequency;
}

void PWM4_Init(void){
	PWMB->CSR |= (0x04ul << 0);
	PWMB->PPR &= ~(0xFFul <<0);
	PWMB->PPR |= (0x01ul << 0);	
	PWMB->PCR |= (0x01ul << 3);	
	PWMB->CMR0 &= ~(0xFFFFul << 0);
	PWMB->CMR0 |= (0x1ul << 0);
	PWMB->CNR0 &= ~(0xFFFFul << 0);
	PWMB->CNR0 |= (0x3ul << 0);
	
	PWMB->POE |= (0x1ul << 0);
	
	PWMB->PCR |= (0x01ul << 0);
}
