#include <reg52.h>

bit Flag;
unsigned int ReData,SenData;

//ir
#define uchar unsigned char
uchar f;

#define Imax 14000
#define Imin 8000
#define Inum1 1450
#define Inum2 700
#define Inum3 3000

unsigned char Im[4]={0x00,0x00,0x00,0x00};
uchar show[2]={0,0};
unsigned long m,Tc;
unsigned char IrOK;


//void delay(void)
//{
//	int i=1000;
//	for(;i>0;i--)
//	;
//}
 void delay(unsigned int i)
{
	unsigned char j;
	for(i; i > 0; i--)
		for(j = 200; j > 0; j--) ;

}

//void uart_init()
//{
//   	// init serial
//	SCON = 0x50;
//	TMOD = 0x21;
//	PCON |= 0x80;
//	TH1 = 0xF3;
//	TL1 = 0xF3;
//	TR1 = 1;
//	ES = 1;
//	EA = 1;
//}
//void IR_init()
//{
//	 	// init ir
//	m = 0;
//	f = 0;
//	EA = 1;
//	TMOD = 0x11;
//	IT1 = 1;
//	EX1 = 1;
//	TH0 = 0;
//	TL0 = 0;
//	TR0 = 1;
//}


void main(void)
{

	 //uart_init();
	 //IR_init();
   	// init serial
	SCON = 0x50;
	TMOD = 0x21;
	PCON |= 0x80;
	TH1 = 0xF3;
	TL1 = 0xF3;
	TR1 = 1;
	ES = 1;
	EA = 1;

	m = 0;
	f = 0;
	EA = 1;
	//TMOD = 0x11;
	IT1 = 1;
	EX1 = 1;
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;

	while(1)
	{
		if(Flag == 1)
		{
			SBUF = SenData;
			while(!TI);
			TI = 0;
			Flag = 0;
			delay(1000);
		}
		if(IrOK == 1)
		{
			SBUF = SenData;
			while(!TI);
			TI = 0;
			IrOK = 0;
			delay(1000);
		}
		
	}
}
void ser_int(void) interrupt 4 using 1
{
	if(RI == 1)
	{
		//process receive data;
		RI = 0;
		ReData = SBUF;
		SenData = ReData;
		Flag = 1;
	}
}

void interir(void) interrupt 2 using 2
{
	Tc=TH0*256+TL0;
	TH0=0;
	TL0=0;
	if((Tc>Imin)&&(Tc<Imax))
	{
		m = 0;
		f = 1;
		return ;
	}
	if(f == 1)
	{
		if(Tc>Inum1 && Tc < Inum3)
		{
			Im[m/8] = Im[m/8]>>1|0x80;
			m++;
		}
		if(Tc>Inum2 && Tc < Inum1)
		{
			Im[m/8] = Im[m/8]>>1;
			m++;
		}
		if(32 == m)
		{
			m = 0;
			f = 0;
			if(Im[2] == ~Im[3])
			{
				IrOK = 1;
				SenData=0x11;
				P1 = 0x00; // show the value received
			}
			else
				IrOK = 0;
		}
	}
}