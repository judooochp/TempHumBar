#include <TFTv2.h>
#include <SPI.h>

#include <Wire.h>
#include "Adafruit_HTU21DF.h"
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>


/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2013/JUN/17  - Updated altitude calculations (KTOWN)
   2013/FEB/13  - First version (KTOWN)
*/

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

// Declare variables for use in Loop function. Duplicates for second sensor array.
  float bmpTemp;
  float htuTemp;
  float avgTemp;
  float presHg;
  float relHum;
  float bmpTemp2;
  float htuTemp2;
  float avgTemp2;
  float presHg2;
  float relHum2;
  
void setup(void) {
//  Serial.begin(9600);
  pinMode(9,OUTPUT);
  Tft.TFTinit();
  TFT_BL_ON;
 
  /* Initialise the sensor */
  if(!bmp.begin()) {
    /* There was a problem detecting the BMP085 ... check your connections. Will not detect problems with Sensor Array 2. */
    Tft.drawString("Pres Err",5,250,2,ORANGE);
//  print("failure");
    while(1);
  }

// Start Humidity Sensor


  if (!htu.begin()) {
    Tft.drawString("\%RH Err",5,150,2,ORANGE);
    while (1);
  }
  
// Center dividing line to prevent confusion
  Tft.drawRectangle(120,1,1,340,WHITE);
  
}
  
void loop(void) {
  
  digitalWrite(9,LOW); // Set GD4051B selectors to all low, selecting Y0 and therefore sensor array 1
  
/* Get a new pressure sensor event */
  sensors_event_t event;
  bmp.getEvent(&event);

/* Display the results (barometric pressure is measure in hPa, but I've converted it to Inches of Mercury.) */
  if (event.pressure) {
    
   Tft.drawFloat(presHg,1,5,270,4,BLACK); // Remove previous value, which allows for greater refresh rates than  would using "drawRectangle()".
/* Display atmospheric pressue in Inches of Mercury by using an unnecessarily precise conversion factor. */
   presHg = (event.pressure)*0.0295299833;
   Tft.drawFloat(presHg,1,5,270,4,WHITE);

/* First we get the current temperature from the BMP085 */
    bmp.getTemperature(&bmpTemp);
    
    
    Tft.drawFloat(relHum,1,5,180,4,BLACK); // Remove previous value, which allows for greater refresh rates than  would using "drawRectangle()".
    relHum = htu.readHumidity();
    
/* This term is used as compensation similar to the temperature, explained on line 112 */
    relHum -= 0.3;
    Tft.drawFloat(relHum,1,5,180,4,WHITE); // Draw it.
    
    
    Tft.drawFloat(avgTemp,1,5,25,4,BLACK); // Remove previous °F value, which allows for greater refresh rates than  would using "drawRectangle()".
    avgTemp -= 32; // Convert to °C
    avgTemp /= 1.8; // Continue conversion.
    Tft.drawFloat(avgTemp,1,10,100,3,BLACK); // Remove previous value, which allows for greater refresh rates than  would using "drawRectangle()".
    htuTemp = htu.readTemperature(); // Get new temperature in °C
    
    /* The term (+0.5556) is used to comensate for differences of sensor arrays 
      in lieu of calibration to show reliable differences. 
      Based on difference of 1°F between sensors (5/9°C) at time of test.
      Averaging of individual sensors is superfluous. I just felt like it.
      Perhaps it will compensate for differences in time constants, perhaps not.
      Suffer. */
    avgTemp = (htuTemp + bmpTemp) / 2 + 0.5556; 
    Tft.drawFloat(avgTemp,1,10,100,3,WHITE);  // Draw new temp value.
    avgTemp *= 1.8;  // Convert to °F
    avgTemp += 32; // Continute conversion.
    Tft.drawFloat(avgTemp,1,5,25,4,WHITE); // Draw that shit.
  
    }
 

// NOW THE SECOND SENSOR. Explanatory notes removed. Compensation terms are opposite those from first sensor array.

  digitalWrite(9,HIGH); // Set GD4051B selectors HIGH, selecting Y7 and therefore sensor array 2
  
  sensors_event_t event2;
  bmp.getEvent(&event2);

  if (event2.pressure) {
    
    Tft.drawFloat(presHg2,1,135,270,4,BLACK);
    presHg2 = (event2.pressure)*0.0295299833;
    presHg2 += 0.5;
    Tft.drawFloat(presHg2,1,135,270,4,WHITE);

    bmp.getTemperature(&bmpTemp2);    
    
    Tft.drawFloat(relHum2,1,135,180,4,BLACK);
    relHum2 = htu.readHumidity();
    relHum2 += 0.3;
    Tft.drawFloat(relHum2,1,135,180,4,WHITE);
    
    
    Tft.drawFloat(avgTemp2,1,135,25,4,BLACK);
    avgTemp2 -= 32;
    avgTemp2 /= 1.8;
    Tft.drawFloat(avgTemp2,1,145,100,3,BLACK);
    htuTemp2 = htu.readTemperature();
    avgTemp2 = (htuTemp2 + bmpTemp2) / 2 - 0.5556;
    Tft.drawFloat(avgTemp2,1,145,100,3,WHITE);
    avgTemp2 *= 1.8;
    avgTemp2 += 32;
    Tft.drawFloat(avgTemp2,1,135,25,4,WHITE);
    delay(5000);
  
    }
  }
