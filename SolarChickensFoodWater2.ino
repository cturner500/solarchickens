#include "config.h"
//#define WIFI_SSID       "CamAndAnnie"
//#define WIFI_PASS       "roycerenee"
//#include "AdafruitIO_WiFi.h"
//#define IO_USERNAME    "cturner50"
//#define IO_KEY         "4f60ce0e52835ccda04e15e970f4d9c92841ed92"
//AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);



//#include <ESP8266WiFi.h>
/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <HX711.h> //for feed scale

//Changing to GPIO Pins:

const int trigPin1 = 16; //D0;
const int echoPin1 = 5;  //D1;
const int trigPin2 = 14; //D5;
const int echoPin2 = 12; //D6;

const int MAX_DISTANCE = 200;
const double WATERER_HEIGHT = 65; //distance (cm) from top to bottom of waterer
const double FEED_WEIGHT_FULL = 85; 
const int SCALE_DOUT_PIN = 13; //D7
const int SCALE_SCK_PIN = 15; //D8
const double kiloconv = 2.20462/1000; //for g to pound conv.



// defines variables
long duration1;
long distance1;
long duration2;
long distance2;
long wtr1 = 0;
long wtr2 = 0;
float weight =  0;
float feedpct = 0;

//set up scale
HX711 scale(SCALE_DOUT_PIN, SCALE_SCK_PIN);

// set up the feeds
AdafruitIO_Feed *waternumberone = io.feed("waternumberone");
AdafruitIO_Feed *waternumbertwo = io.feed("waternumbertwo");
AdafruitIO_Feed *feedlevel = io.feed("feedlevel");

void setup() {
  
  // start the serial connection
  Serial.begin(115200);
  
  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  
  //set up scale
  scale.set_scale(24600.0/980);// <- (weight shown w/o calibration for bottle of water div by actual weight (g)
 
  //scale.tare();
  Serial.println("ENDING SETUP()");

}

void loop() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input



   io.run();
    Serial.println("STARTING LOOP()");
    // Clears the trigPin1
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration1 = pulseIn(echoPin1, HIGH, 26000);
    // Calculating the distance
    distance1= duration1/58;

    delay(2000); //delay two seconds
    
     // Clears the trigPin2
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration2 = pulseIn(echoPin2, HIGH);
    // Calculating the distance
    distance2= duration2/58;

     
    Serial.print("Distance1: ");
    Serial.println(distance1);
    Serial.print("Distance2: ");
    Serial.println(distance2);
    
    //Water Height divided by Waterer Height equals pct full
    Serial.println("HERE ARE OUR MEASUREMENTS:");
    wtr1 = ((WATERER_HEIGHT - (distance1-15))/WATERER_HEIGHT)*100;//calibrated
    
    Serial.print("Water One Height: ");
    Serial.print(wtr1);
    Serial.println("%");
    
    //Water Height divided by Waterer Height equals pct full
    wtr2 = ((WATERER_HEIGHT - (distance2-15)) / WATERER_HEIGHT) * 100;//calibrated
    Serial.print("Water Two Height: ");
    Serial.print(wtr2); 
    Serial.println("%");

//    //feed weight
  
    weight = scale.get_units(5);
  
    Serial.println(String(weight*kiloconv, 5));
    Serial.println(String(weight, 5));
    feedpct = ((weight*kiloconv)/FEED_WEIGHT_FULL)*100;
    Serial.print("Weight Pct: ");
    Serial.println(feedpct);
  
//    feedwt = scale.get_units(5);
//    Serial.print("Feed Weight: ");
//    Serial.print(feedwt); 
//    Serial.println("g");
//    //feed pct
//    feedpct = feedwt/FEED_WEIGHT_FULL*100;
//    Serial.print("Feed pct: ");
//    Serial.print(feedpct); 
//    Serial.println("%");
//
    
  // save to Adafruit IO
  waternumberone ->save(wtr1);
  waternumbertwo ->save(wtr2);
  feedlevel -> save(feedpct);

  // wait 30 seconds (5000 milliseconds == 5 seconds)
  delay(30000);
  Serial.println("ENDING LOOP()");

}

