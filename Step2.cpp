// This #include statement was automatically added by the Spark IDE.
//This will allow for future support of the LED matrixes
#include "BCG_LED_Library/BCG_LED_Library.h"
//These next 2 lines are required by spark
#include "application.h"
#include <math.h>

//now we will declare your team name
int core_id = 1; //change to your spark team ID (1 - 30)
char *teamName = "TeamName"; //max length 20 characters!!!

//now you need to ask your hardware team what PIN your thermostat is on 
//change XX in the code below to the pin number, e.g., D6, etc
//(if your hardware team tells you thermostat is connected to D6 code would be as follows)
//#define THERM_PIN   D6
#define THERM_PIN   xx

//DO NOT CHANGE THESE NEXT 4 LINES
//These store the current state of the program
int currentTemperature = 0;
char logResult[200] = "";
bool thermostatStatus = false;
char statusString[100] = "";
char *bcgStatus = "step2";

//run by the Spark Core upon start - configures all PINs & web interface
void setup()
{
    Wire.begin();
    
    //allow temperature (room & setting) to be read from web interface
    Spark.variable("current_temp", &currentTemperature, INT);
   
    //Variable that Google docs will read to track team progress
    Spark.variable("log_data", &logResult, STRING);
    
    Serial.begin(9600);
    
    //themometer pin recieves voltage (INPUT)
    pinMode(THERM_PIN, INPUT);
    
}



void loop()
{
    //counter to allow us to only check the temp every so often
    static int wait = 0;
    
    //DO NOT CHANGE
    static int firstTimeOn = 1;
    
    int tempReading = 0;
    double voltage = 0.0;
    
    
    //only check & update temp every so often
    if (!wait)
    {
        wait = 1000;
        
        //TEMPERATE READING
        //read temp raw data
        tempReading = analogRead(THERM_PIN);
        voltage = (tempReading * 3.3) / 4095;
        //get the temp as degrees C
        double doubleTemp = (voltage - 0.5) * 100;
        //convert c to f
        //doubleTemp = ((doubleTemp * 9) / 5) + 32;
        // cast to int (e.g., change 75.44 to 75)
        currentTemperature = round(doubleTemp);
    }
    
    //logging result for Google
    //TODO: Check if Google doc can read this partial set of strings
    sprintf(logResult, "{\"id\":%d,\"team\":\"%s\",\"c_tmp\":%d,\"bcg_status\":\"%s\",\"status\":\"%s\"}", core_id, teamName, currentTemperature, bcgStatus, thermostatStatus ? "on" : "off");
       if (firstTimeOn == 1) {
        sprintf(statusString, "{\"id\":%d,\"team\":\"%s\",\"bcg_status\":\"%s\"}", core_id, teamName, bcgStatus);
        Spark.publish("bcg-status",statusString);
        firstTimeOn = 0;
    }
    
    --wait;
}

