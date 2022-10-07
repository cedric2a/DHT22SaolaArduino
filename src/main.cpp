

#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp



// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
// or #include "SH1106Wire.h", legacy include: `#include "SH1106.h"`
// For a connection via I2C using brzo_i2c (must be installed) include
// #include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// #include "SH1106Brzo.h"
// For a connection via SPI include
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// #include "SH1106SPi.h"

// Use the corresponding display class:

// Initialize the OLED display using SPI
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi        display(D0, D2, D8);
// or
// SH1106Spi         display(D0, D2);

// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL
// SSD1306Brzo display(0x3c, D3, D5);
// or
// SH1106Brzo  display(0x3c, D3, D5);

// Initialize the OLED display using Wire library
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SH1106Wire display(0x3c, SDA, SCL);


DHTesp dht;

  float humidity ;
  float temperature ;

void Capteur( void *pvParameters ){

  dht.setup(GPIO_NUM_14, DHTesp::DHT22); // Connect DHT sensor to GPIO 14

   for(;;)
   {

  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);

    vTaskDelay( pdMS_TO_TICKS( 2000 ) );
   }

}

void Affichage( void *pvParameters ){

  display.init();
  display.flipScreenVertically();
  display.setContrast(255);
  // Align text vertical/horizontal center
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.setFont(ArialMT_Plain_10);
  display.display();

   for(;;)
   {

  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(display.getWidth() / 2, 16 , "Temp: " + String(temperature,0)+ " °C");
  display.drawString(display.getWidth() / 2, 50, "\nHum: " + String (humidity,0)+" %") ;
  display.display();
  

    vTaskDelay( pdMS_TO_TICKS( 5000 ) );
   }

}

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");

  xTaskCreate(Capteur, "DHT22", 10000, NULL, 1, NULL);
  xTaskCreate(Affichage, "SSD1306", 10000, NULL, 2, NULL);
}

void loop() {
  
}


