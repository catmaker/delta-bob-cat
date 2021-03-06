/* Test which brings default HelloWorld project from mbed online compiler
   to be built under GCC.
*/
#include "mbed.h"

DigitalOut myled0(P0_22);
DigitalOut myled1(P1_18);
DigitalOut myled2(P1_19);
DigitalOut myled3(P1_20);

DigitalOut alphaDir(P2_7);
DigitalOut alphaStep(P2_8);
DigitalOut betaDir(P2_11);
DigitalOut betaStep(P2_12);

AnalogIn   joy_x(A0);
AnalogIn   joy_y(A1);


void turnon(int i)
{
    myled0 = myled1 = myled2 = myled3 = 0;
    switch(i) {
    case 0:
	myled0 = 1; break;
    case 1:
	myled1 = 1; break;
    case 2:
	myled2 = 1; break;
    case 3:
	myled3 = 1; break;
    default:
	myled0 = myled1 = myled2 = myled3 = 0; break;

    }
}
 
Serial pc(USBTX, USBRX);
 
int main() 
{ 
    int led = 0;
    int x, y;
     
    

    pc.baud (115200);
    printf("\r\n\r\nTESTING ADC\r\n");

    //https://developer.mbed.org/users/aung/notebook/system-clock-frequency
    
    int Fin = 12000000; // 12MHz XTAL
    
    printf("PLL Registers:\r\n");
    printf(" - PLL0CFG = 0x%08X\r\n", LPC_SC->PLL0CFG);
    printf(" - CLKCFG  = 0x%08X\r\n", LPC_SC->CCLKCFG);
    
    int M = (LPC_SC->PLL0CFG & 0xFFFF) + 1;
    int N = (LPC_SC->PLL0CFG >> 16) + 1;
    int CCLKDIV = LPC_SC->CCLKCFG + 1;
 
    printf("Clock Variables:\r\n");
    printf(" - Fin = %d\r\n", Fin);
    printf(" - M   = %d\r\n", M);
    printf(" - N   = %d\r\n", N);
    printf(" - CCLKDIV = %d\r\n", CCLKDIV);
 
    int Fcco = (2 * M * 12000000) / N;
    int CCLK = Fcco / CCLKDIV;
 
    printf("Clock Results:\r\n");    
    printf(" - Fcco = %d\r\n", Fcco);
    printf(" - CCLK = %d\r\n", CCLK);    

    printf("SystemCoreClock = %d Hz\r\n", SystemCoreClock);
 
    while(1) {
	
	turnon(led++);
	if (led > 3) {
	    led = 0;
	    wait(0.2);
	}
	wait(0.2);
	
	x = joy_x.read_u16();
	y = joy_y.read_u16();
	 
        printf("normalized: x = 0x%04X y = 0x%04X \r\n", x, y);
        wait(0.2f);

    }
}


