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

int main() 
{ 
    int n, led = 0;
     
    
    alphaDir = 1;
    betaDir = 1;
    for(n = 0; n < 500; n++) {
	alphaStep = 1;
	betaStep = 1;
	wait(0.0001);
	alphaStep = 0;
	betaStep = 0;
	wait(0.00050);
    }
    
    
    while(1) {
	turnon(led++);
	if (led > 3) {
	    led = 0;
	    wait(0.2);
	}
        wait(0.2);
    }
}


