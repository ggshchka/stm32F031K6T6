#define STACK_TOP (0x20000000 + 0x800)

#define RCC(_offset)   *((unsigned long*)(0x40021000 + (_offset)))
#define GPIOB(_offset) *((unsigned long*)(0x48000400 + (_offset)))
#define TIM3(_offset)  *((unsigned long*)(0x40000400 + (_offset)))

#define NVIC(_offset)  *((unsigned long*)(0xE000E100 + (_offset)))
#define ISER1  (0x00)

#define AHB  (0x14)
#define APB1 (0x1C)
#define MODER   (0x00)
#define ODR     (0x14)
#define OTYPER  (0x04)


#define CR1     (0x00)
#define DIER    (0x0C)
#define SR      (0x10)
#define PSC     (0x28)
#define ARR     (0x2C)

#define  RCC_APB1ENR_TIM3EN (0x2)
#define  TIM_CR1_CEN        (0x0001)
#define  TIM_DIER_UIE       (0x0001)
#define  TIM_SR_UIF         (0x0001)

int flg = 0;

void emptyHandl();
void handler();
void timStart();
int main();

unsigned long* vector_table[] __attribute__((section(".vector_table"))) = {
		(unsigned long*)STACK_TOP,
		(unsigned long*)main,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,  //4 000C

		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,  //8 001C

		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,  //12 002C

		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,  //16 003C

		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,  //20 004C

		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,  //24 005C

		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,  //28 006C

		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,
		(unsigned long*)emptyHandl,	 //32 007C

		(unsigned long*)handler,

};

void emptyHandl(){
}

void timStart(){
	RCC(APB1) |= RCC_APB1ENR_TIM3EN;
	for (int i = 0; i < 4; ++i);
	NVIC(ISER1) |= (1 << 16);
	TIM3(DIER) |= TIM_DIER_UIE;
	TIM3(ARR) = 2000;
	TIM3(PSC) = 8000;
	for (int i = 0; i < 4; ++i);
	TIM3(CR1) |= TIM_CR1_CEN;
}

void handler(){
	TIM3(SR) &= ~TIM_SR_UIF;
	if (flg == 0)
	{
		GPIOB(ODR) |= (1 << 3);
		flg = 1;
	}
	else if(flg == 1)
	{
		GPIOB(ODR) &= ~(1 << 3);
		flg = 0;
	}
}

int main()
{
	RCC(AHB) |= (1 << 18);
	for (int i = 0; i < 4; ++i);
	GPIOB(MODER) |= (1 << 6);
	timStart();
	while(1){}

	return 0;
}
