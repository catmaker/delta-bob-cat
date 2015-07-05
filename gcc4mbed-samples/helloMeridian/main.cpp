/* Test which brings default HelloWorld project from mbed online compiler
   to be built under GCC.
*/
#include "mbed.h"

DigitalOut myled0(P0_22);
DigitalOut myled1(P1_18);
DigitalOut myled2(P1_19);
DigitalOut myled3(P1_20);



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
    int led = 0;
     
    while(1) {
	turnon(led++);
	if (led > 3) {
	    led = 0;
	    wait(0.2);
	}
        wait(0.2);
    }
}
