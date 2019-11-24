#include "sample_hardware.hpp"
#include "Networkbits.hpp"

// This is a very short demo that demonstrates all the hardware used in the coursework.
// You will need a network connection set up (covered elsewhere). The host PC should have the address 10.0.0.1

//Threads
Thread nwrkThread;
 

int main() {
    //Greeting
    printf("Testing\n\r");    
    
    //Power on self test
    post();
    
    //Initialise the SD card (this needs to move)
    if ( sd.init() != 0) {
        printf("Init failed \n\r");
        lcd.cls();
				lcd.locate(0,0);
        lcd.printf("CANNOT INIT SD");        
        errorCode(FATAL);
    } 
    
    //Create a filing system for SD Card
    FATFileSystem fs("sd", &sd);     

    //Open to WRITE
    FILE* fp = fopen("/sd/test.csv","a");
    if (fp == NULL) {
        error("Could not open file for write\n\r");
        lcd.cls();
        lcd.printf("CANNOT OPEN FILE\n\n");
        errorCode(FATAL);
    }
    
    //Last message before sampling begins
    lcd.cls();
    lcd.printf("READY\n\n");
		printf("READY\n\r");
        
        
    //Press either switch to unmount
    while ((SW1 == 0) && (SW2 == 0)) {
        
        //Base loop delay
        wait(1.0);
        
        //Read environmental sensors
        double temp = sensor.getTemperature();
        double pressure = sensor.getPressure();
        
        //Write new data to LCD (not fast!)
        lcd.cls();
        lcd.printf("Temp   Pressure\n"); 
        lcd.printf("%6.1f ",temp);
        lcd.printf("%.2f\n",pressure);
			
				printf("Temp   Pressure\n"); 
        printf("%6.1f ",temp);
        printf("%.2f\n",pressure);
        
        //Write to SD (potentially slow)
        fprintf(fp, "%6.1f,%.2f\n\r", temp, pressure);
    }
    
    //Close File
    fclose(fp);
    
    //Close down
    sd.deinit();
    printf("Unmounted...\n\r");
    lcd.cls();
    lcd.printf("Unmounted...\n\n");
    
    //Flash to indicate goodness
    while(true) {
        greenLED = 1;
        wait(0.5);
        greenLED = 0;
        wait(0.1);    
    }
}


    
