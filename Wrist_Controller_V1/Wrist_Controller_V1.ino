//OLED SHIT

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 

#define OLED_RESET     LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//TEMP/HUM SENSOR SHIT
#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT11

//SOUND SENSOR SHIT
int soundSensor = 3;

DHT dht(DHTPIN, DHTTYPE);

//IR RECIVER SHIT
#include <IRremote.h>
int IR_PIN = 9;

IRrecv irrecv(IR_PIN);

decode_results results;




void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); 
  initalize_oled();
  dht.begin(); // initialize the temp/hum sensor
  display.println("dhtsensor_start");
  display.display();
  delay(1000);
  display.println("serial_start");
  display.display();
  delay(1000);
  pinMode (soundSensor, INPUT);
  display.println("startup_complete");
  display.display();
  delay(500);
  display.clearDisplay();
}



void loop() {
  oledstatictext("System Online");
  delay(1000);

//Remote button press selection
if (irrecv.decode(&results)) {

    //in each case instead of those hex number you have to put yours remote keys codes.
    switch(results.value){

      case FF906F: //Red
      display.clearDispplay();
      float tempF = 0;
      float hum = 0;

       display.clearDisplay();
        delay(1000);
        tempF = checktemp();
        hum = checkhum();
        display.print("Temp: ");
        display.println(tempF);
        display.display();      // Show temp on screen
        delay(10);
        display.print("Humidity: ");
        display.println(hum);
        display.display();      // Show temp on screen
        delay(2000);
        display.clearDisplay();
      break;

      case FF10EF:  //Green
      //digitalWrite(3, !digitalRead(3));    
      break;
      
      case FF50AF:  //Blue
      //digitalWrite(4, !digitalRead(4));
      break;
      
      
      case FFD02F:  //White
      //digitalWrite(5, !digitalRead(5));   
      break;
    }
    irrecv.resume(); 
  }
  delay(10);

  
 /* float tempF = 0;
  float hum = 0;

  display.clearDisplay();
  delay(1000);
  tempF = checktemp();
  hum = checkhum();
  display.print("Temp: ");
  display.println(tempF);
  display.display();      // Show temp on screen
  delay(10);
  display.print("Humidity: ");
  display.println(hum);
  display.display();      // Show temp on screen
  delay(2000);
  display.clearDisplay();
  */
}


//FUNCTIONS


//O-LED FUNCTIONS
void oledscrolltext(char *text) {
  display.clearDisplay();

  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(text);
  display.display();      // Show initial text
  delay(100);

  // Scroll right
  display.startscrollright(0x00, 0x0F);
  delay(5000);
  //display.stopscroll();
  //delay(1000);

}
void oledstatictext(char *text)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(text);
  display.display();
  delay(10);
}

void  initalize_oled(void)
{
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("begin_boot_sequence");
  display.display();
  delay(3000);

  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

}
  
//LCD FUNCTIONS

//TEMP SENSOR FUNCTIONS
float checktemp() {
  // wait a few seconds between measurements.
  delay(1000);

  // read temperature as Celsius
  float tempC = dht.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht.readTemperature(true);

  // check if any reads failed
  if (isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {

    Serial.print("Temperature: ");
    Serial.print(tempF);
    Serial.println("°F");

    return tempF;
  }
}
  float checkhum() {
  // wait a few seconds between measurements.
  delay(1000);

  // read humidity
  float humi  = dht.readHumidity();

  // check if any reads failed
  if (isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");
    
    return humi;

  }
  }

//MIC SENSOR FUNCTIONS

//LED FUNCTIONS
