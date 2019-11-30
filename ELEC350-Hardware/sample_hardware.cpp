#include "mbed.h"
#include "sample_hardware.hpp"
#include "Networkbits.hpp"

#define RED_DONE 1
#define YELLOW_DONE 2

//Digital outputs
DigitalOut onBoardLED(LED1);
DigitalOut redLED(PE_15);
DigitalOut yellowLED(PB_10);
DigitalOut greenLED(PB_11);

//Inputs
DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(PE_12);
DigitalIn  SW2(PE_14);
//Serial pc(USBTX, USBRX);
AnalogIn adcIn(PA_0);

//Environmental Sensor driver
#ifdef BME
BME280 sensor(PB_9, PB_8);
#else
BMP280 sensor(PB_9, PB_8);
#endif

//LCD Driver (provided via mbed repository)
//RS D9
//E  D8  
//D7,6,4,2 are the 4 bit for d4-7
TextLCD lcd(D9, D8, D7, D6, D4, D2); // rs, e, d4-d7

//SD Card
SDBlockDevice sd(PB_5, D12, D13, D10); // mosi, miso, sclk, cs 

//POWER ON SELF TEST
void post() 
{
    //POWER ON TEST (POT)
    puts("**********STARTING POWER ON SELF TEST (POST)**********\n\r");
    
    //Test LEDs
    puts("ALL LEDs should be blinking\n\r");
    for (unsigned int n=0; n<10; n++) {
        redLED    = 1;
        yellowLED = 1;
        greenLED  = 1;
        wait_ms(100);
        redLED    = 0;
        yellowLED = 0;
        greenLED  = 0;     
        wait_ms(100);         
    } 
    
    //Output the switch states (hold them down to test)
		uint8_t sw1 = SW1.read();
		uint8_t sw2 = SW2.read();
    printf("SW1: %d\tSW2: %d\n\r", sw1, sw2);    
    printf("USER: %d\n\r", onBoardSwitch.read()); 
    
    //Output the ADC
		float ldr = adcIn.read();
    printf("ADC: %f\n\r", ldr);
    
    //Read Sensors (I2C)
    float temp = sensor.getTemperature();
    float pressure = sensor.getPressure();
    #ifdef BME
    float humidity = sensor.getHumidity();
    #endif
   
    //Display in PuTTY
    printf("Temperature: %5.1f\n\r", temp);
    printf("Pressure: %5.1f\n\r", pressure);
    #ifdef BME
    printf("Pressure: %5.1f\n", humidity);
    #endif
    
    //Display on LCD
    redLED = 1;
    lcd.cls();
		lcd.locate(0,0);
    lcd.printf("LCD TEST...\n\r");
    wait_us(500000);
    redLED = 0;
    
    //Network test (if BOTH switches are held down)
		if(sw1 == 1 && sw2 == 1)
		{
			printf("Network Test");
			networktest();
		}
		//puts("IMPORTANT! Network Post Test Not Ran	\n\r");

		
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

    FILE* fp = SD_Open("test.csv");
    
    //Last message before sampling begins
    lcd.cls();
    lcd.printf("READY\n\n");
		printf("READY\n\r");  
			
        //Read environmental sensors
        temp = sensor.getTemperature();
        pressure = sensor.getPressure();
        
        //Write new data to LCD (not fast!)
        lcd.cls();
        lcd.printf("Temp   Pressure\n\r"); 
        lcd.printf("%6.1f \n\r",temp);
        lcd.printf("%.2f\n\r",pressure);
			
				printf("Temp   Pressure\n"); 
        printf("%6.1f \n\r",temp);
        printf("%.2f\n\r",pressure);
        
        //Write to SD (potentially slow)
        fprintf(fp, "%6.1f,%.2f\n\r", temp, pressure);
				
				//Close File
    fclose(fp);

printf("SD Card initalised\n\r");
   
    
    puts("**********POST END**********\n\r");
 
}

void errorCode(ELEC350_ERROR_CODE err)
{
    switch (err) {
      case OK:
        greenLED = 1;
        wait_us(1000000);
        greenLED = 0;
        return;                
      case FATAL:
        while(1) {
            redLED = 1;
            wait_us(100000);
            redLED = 0;
            wait_us(100000);             
        }
    };
}

void SD_Unmount()
{
		//Close down
    sd.deinit();
    printf("Unmounted...\n\r");
    lcd.cls();
    lcd.printf("Unmounted...\n\n");
}

FILE* SD_Open(string _fileName)
{
	  //Open to WRITE
	string fp_string = "/sd/";
	fp_string += _fileName;
    FILE* fp = fopen(fp_string.c_str(),"a");
    if (fp == NULL) {
        error("Could not open file for write\n\r");
        lcd.cls();
        lcd.printf("CANNOT OPEN FILE\n\n");
        errorCode(FATAL);
    }
		return fp;
}
