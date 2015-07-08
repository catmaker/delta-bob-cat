/* Test which brings default HelloWorld project from mbed online compiler
   to be built under GCC.
*/
#include "mbed.h"

Serial uart0(USBTX, USBRX);

DigitalOut myled0(P0_22);
DigitalOut myled1(P1_18);
DigitalOut myled2(P1_19);
DigitalOut myled3(P1_20);

DigitalOut alphaDir(P2_7);
DigitalOut alphaStep(P2_8);
DigitalOut betaDir(P2_11);
DigitalOut betaStep(P2_12);

DigitalIn  N_sw(P0_25, PullUp);

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


#define ASSERT_DELAY	0.0000
#define ASSERT_PERIOD	0.0000
#define STEP_PERIOD	0.000001
#define TABLE_WIDTH	24576
#define TABLE_Y_DEPTH	24300

int xpos = TABLE_WIDTH + 1000;
int ypos =(-TABLE_Y_DEPTH - 200); // unknown start position until limit switch


int motorMove(int EW, int NS)
{
    int NSEW = 0;
    
    if (N_sw.read() == 0) {
	ypos = 0;
    }
  
    if ((NS >= 1) && (ypos < 0)) {
	NSEW |= 0b1000;
	ypos++;
    } else if ((NS <= -1) && (ypos > -TABLE_Y_DEPTH)) {
	NSEW |= 0b0100;
	ypos--;
    }
    
    if ((EW >= 1) && (xpos < TABLE_WIDTH)) {
	NSEW |= 0b0010;
	xpos++;
    } else if ((EW <= -1) && (xpos > 0)) {
	NSEW |= 0b0001;
	xpos--;
    }
    
    switch(NSEW) {
    case 0b1000: /* north */
	alphaDir = 1; betaDir = 0; wait(ASSERT_DELAY); alphaStep = 1; betaStep = 1;
	break;
    case 0b0100: /* south */
	alphaDir = 0; betaDir = 1; wait(ASSERT_DELAY); alphaStep = 1; betaStep = 1;
	break;
    case 0b0010: /* east */
	alphaDir = 0; betaDir = 0; wait(ASSERT_DELAY); alphaStep = 1; betaStep = 1;
	break;
    case 0b0001: /* west */
	alphaDir = 1; betaDir = 1; wait(ASSERT_DELAY); alphaStep = 1; betaStep = 1;
	break;
    case 0b1010: /* north-east */
	alphaDir = 1; betaDir = 0; wait(ASSERT_DELAY); alphaStep = 0; betaStep = 1;
	break;
    case 0b0110: /* south-east */
	alphaDir = 0; betaDir = 0; wait(ASSERT_DELAY); alphaStep = 1; betaStep = 0;
	break;
    case 0b0101: /* south-west */
	alphaDir = 1; betaDir = 1; wait(ASSERT_DELAY); alphaStep = 0; betaStep = 1;
	break;
    case 0b1001: /* north-west */
	alphaDir = 1; betaDir = 0; wait(ASSERT_DELAY); alphaStep = 1; betaStep = 0;
	break;
    default:     /* unknown or error */
	alphaStep = 0; betaStep = 0;
	break;
    }

    wait(ASSERT_PERIOD);
    alphaStep = 0;
    betaStep = 0;
    
    return 0;
}
 

#if 0
void tableZero(void)
{
    while(1) {
	motorMove(1, 0);
	if (x_sw.read() == 0) {
	    break;
	}
    }
    ypos = 0;
}
#endif

int main() 
{ 
    int led = 0;
    int ch;
    int jx, jy;
     
    uart0.baud (115200);
    printf("\r\n\r\nTESTING Meridian xy-table h-bot motor frame\r\n");

    while(1) {

	if(uart0.readable()) {
            ch = uart0.getc();
	    switch (ch) {
	    case ' ':
		printf("position(x, y) = %05d, %05d\r\n", xpos, ypos);
		break;
	    case 'w':
		xpos = 0;
		break;
	    case 'n':
		ypos = 0;
		break;
	    default:
		break;
	    }
        }

	jx = joy_x.read_u16();
	jy = joy_y.read_u16();

	jx = jx - 0x8000;
	if ((jx > -800) && (jx < 800)) {
	    jx = 0;
	}
	jy = jy - 0x8000;
	if ((jy > -800) && (jy < 800)) {
	    jy = 0;
	}
	    
	motorMove(jx, jy);
	wait(STEP_PERIOD);
	
	

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


