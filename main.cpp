#include "sample_hardware.hpp"
#include "Networkbits.hpp"

// This is a very short demo that demonstrates all the hardware used in the coursework.
// You will need a network connection set up (covered elsewhere). The host PC should have the address 10.0.0.1

//Threads
Thread nwrkThread;
 
//Test git comment
int main() {
    //Greeting
    printf("Testing\n\r");    
    
    //Power on self test
    post();
	
	redLED = 1;
	yellowLED = 1;
	
	while(1)
	{
		wait_us(1000000);
		greenLED = 1;
		float temp = sensor.getTemperature();
		printf("Temp: %6.1f\n\r", temp);
		wait_us(1000000);
		greenLED = 0;
	}       
}


    
