
// @author @crackaf

// Include Libraries
#include "Arduino.h"
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <MQUnifiedsensor.h>
#include "ESP8266.h"

// https://jsonplaceholder.typicode.com/todos/1

#define DELAY_TIME_SECONDS 15000 // Delay time in mili seconds
#define PRINT_VALUES 1           // Print values to serial monitor

// Pin Definitions

//////////////////// BMP //////////////////////
// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

// bmp.readTemperature()
// bmp.readPressure()
// bmp.readAltitude()
// bmp.readSealevelPressure()

// you can get a more precise measurement of altitude
// if you know the current sea level pressure which will
// vary with weather and such. If it is 1015 millibars
// that is equal to 101500 Pascals.
// bmp.readAltitude(101500)

///////////////////////////////////////////////

//////////////////// DHT //////////////////////
#define DHT_PIN_DATA 2

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// dht.readHumidity()
// dht.readTemperature(bool isFahrenheit) true = Fahrenheit, false = Celsius
// dht.computeHeatIndex(float, float, bool isFahrenheit) true = Fahrenheit, false = Celsius

///////////////////////////////////////////////

//////////////////// MQ135 //////////////////////

#define placa "Arduino MEGA"
#define Voltage_Resolution 5
#define MQpin A10              // Analog input 0 of your arduino
#define MQtype "MQ-135"        // MQ135
#define ADC_Bit_Resolution 10  // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6 // RS / R0 = 3.6 ppm

//   Exponential regression:
//   GAS      | a      | b
//   CO       | 605.18 | -3.937
//   Alcohol  | 77.255 | -3.18
//   CO2      | 110.47 | -2.862
//   Toluen  | 44.947 | -3.445
//   NH4      | 102.2  | -2.473
//   Aceton  | 34.668 | -3.369

//   MQ135.update(); // Update data, the arduino will read the voltage from the analog pin

//   MQ135.setA(605.18); MQ135.setB(-3.937); // Configure the equation to calculate CO concentration value
//   float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

//   MQ135.setA(77.255); MQ135.setB(-3.18); //Configure the equation to calculate Alcohol concentration value
//   float Alcohol = MQ135.readSensor(); // SSensor will read PPM concentration using the model, a and b values set previously or from the setup

//   MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to calculate CO2 concentration value
//   float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

//   MQ135.setA(44.947); MQ135.setB(-3.445); // Configure the equation to calculate Toluen concentration value
//   float Toluen = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

//   MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configure the equation to calculate NH4 concentration value
//   float NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

//   MQ135.setA(34.668); MQ135.setB(-3.369); // Configure the equation to calculate Aceton concentration value
//   float Aceton = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

///////////////////////////////////////////////

// Global variables and defines

// ====================================================================
// vvvvvvvvvvvvvvvvvvvv ENTER YOUR WI-FI SETTINGS  vvvvvvvvvvvvvvvvvvvv
//
char SSID[] = "Transworld 2";               //  Your network SSID (name)
char PASSWORD[] = "88990012";               // Your network password
const char server[] = "api.thingspeak.com"; // ThingSpeak Serve
const char apiKey[] = "7NPBBG8VC0C93LL7";   // ThingSpeak API Key
const char channelID[] = "1745289";         // ThingSpeak Channel ID
/* Collect data once every 15 seconds and post data to ThingSpeak channel once every 2 minutes */
unsigned long lastConnectionTime = 0; // Track the last connection time
//
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// ====================================================================
int hostPort = 80;
// object initialization
Adafruit_BMP085 bmp180;
DHT dht(DHT_PIN_DATA, DHTTYPE);
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, MQpin, MQtype);
ESP8266 wifi;

// define vars for testing menu
const int timeout = 10000; // define timeout of 10 sec
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup()
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial)
        ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");

    // Initialize I2C device
    if (!bmp180.begin())
        Serial.println("BMP::Could not find a valid BMP085/BMP180 sensor, check wiring!");

    dht.begin();

    // Initialize MQ135
    // Set math model to calculate the PPM concentration and the value of constants
    MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
    MQ135.init();
    Serial.println("MQ135::Calibrating please wait.");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++)
    {
        MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
        calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
        Serial.print(".");
    }
    MQ135.setR0(calcR0 / 10);
    Serial.println("MQ135::Calibration done!.");

    if (isinf(calcR0))
    {
        Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    }
    if (calcR0 == 0)
    {
        Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
    }
    // Initialize WiFi

    wifi.init(SSID, PASSWORD);
}

/*
JSON string maker
{
    "api_key": "XXXXXXXXXXXXXXXX",
    "created_at": "2018-04-23 21:36:20 +0200",
    "field1": "John Johnson",
    "field3": 56,
    "field4": 555,
    "field5": 444,
    "latitude": "",
    "longitude": "",
    "status": "Please check in!"
}
*/
/*
String makeJSON()
{
    String s = String("{") +
               "\"api_key\":\"" + apiKey + "\"" +
               ",\"field1\":" + String(bmp180.readAltitude()) +
               ",\"field2\":" + String(bmp180.readPressure()) +
               ",\"field3\":" + String(bmp180.readSealevelPressure()) +
               ",\"field4\":" + String(bmp180.readTemperature()) +
               ",\"field5\":" + String(dht.readHumidity()) +
               ",\"field6\":" + String(dht.readTemperature());

    // MQ135
    MQ135.update(); // Update data, the arduino will read the voltage from the analog pin

    MQ135.setA(605.18);
    MQ135.setB(-3.937); // Configure the equation to calculate CO concentration value
    s += ",\"field7\":" + String(MQ135.readSensor());

    MQ135.setA(77.255);
    MQ135.setB(-3.18); // Configure the equation to calculate Alcohol concentration value
    s += ",\"field8\":" + String(MQ135.readSensor());

    MQ135.setA(110.47);
    MQ135.setB(-2.862); // Configure the equation to calculate CO2 concentration value
    s += ",\"field9\":" + String(MQ135.readSensor());

    MQ135.setA(44.947);
    MQ135.setB(-3.445); // Configure the equation to calculate Toluen concentration value
    s += ",\"field10\":" + String(MQ135.readSensor());

    MQ135.setA(102.2);
    MQ135.setB(-2.473); // Configure the equation to calculate NH4 concentration value
    s += ",\"field11\":" + String(MQ135.readSensor());

    MQ135.setA(34.668);
    MQ135.setB(-3.369); // Configure the equation to calculate Aceton concentration value
    s += ",\"field12\":" + String(MQ135.readSensor());

    s += "}";

    return s;
}

const char * makeGetReq()
{
    String s = String("GET /update?key=") + apiKey + "&";
    s += "field1=" + String(bmp180.readAltitude()) + "&";
    s += "field2=" + String(bmp180.readPressure()) + "&";
    s += "field3=" + String(bmp180.readSealevelPressure()) + "&";
    s += "field4=" + String(bmp180.readTemperature()) + "&";
    s += "field5=" + String(dht.readHumidity()) + "&";
    s += "field6=" + String(dht.readTemperature());

    // MQ135
    MQ135.update(); // Update data, the arduino will read the voltage from the analog pin

    MQ135.setA(605.18);
    MQ135.setB(-3.937); // Configure the equation to calculate CO concentration value
    s += "&field7=" + String(MQ135.readSensor());

    MQ135.setA(77.255);
    MQ135.setB(-3.18); // Configure the equation to calculate Alcohol concentration value
    s += "&field8=" + String(MQ135.readSensor());

    MQ135.setA(110.47);
    MQ135.setB(-2.862); // Configure the equation to calculate CO2 concentration value
    s += "&field9=" + String(MQ135.readSensor());

    MQ135.setA(44.947);
    MQ135.setB(-3.445); // Configure the equation to calculate Toluen concentration value
    s += "&field10=" + String(MQ135.readSensor());

    MQ135.setA(102.2);
    MQ135.setB(-2.473); // Configure the equation to calculate NH4 concentration value
    s += "&field11=" + String(MQ135.readSensor());

    MQ135.setA(34.668);
    MQ135.setB(-3.369); // Configure the equation to calculate Aceton concentration value
    s += "&field12=" + String(MQ135.readSensor());
    s+="\r\nHost: "+String(server)+"\r\nConnection: close\r\n\r\n";
    Serial.print("makeREQ");
    Serial.println(s);
    return s.c_str();
}
*/

// Updates the ThingSpeakchannel with data
void httpRequest()
{
    uint8_t buffer[1024] = {0};

    // const char *hello = "GET /update?key=7NPBBG8VC0C93LL7&field1=200 HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n";

    // making req
    String s = String("GET /update?key=") + apiKey + "&";
    s += "field1=" + String(dht.readTemperature()) + "&";
    s += "field2=" + String(dht.readHumidity()) + "&";

    // MQ135
    MQ135.update(); // Update data, the arduino will read the voltage from the analog pin

    MQ135.setA(605.18);
    MQ135.setB(-3.937); // Configure the equation to calculate CO concentration value
    double a = MQ135.readSensor();

    MQ135.setA(77.255);
    MQ135.setB(-3.18); // Configure the equation to calculate Alcohol concentration value
    double b = MQ135.readSensor();

    MQ135.setA(110.47);
    MQ135.setB(-2.862); // Configure the equation to calculate CO2 concentration value
    double c = MQ135.readSensor();

    MQ135.setA(44.947);
    MQ135.setB(-3.445); // Configure the equation to calculate Toluen concentration value
    double d = MQ135.readSensor();

    MQ135.setA(102.2);
    MQ135.setB(-2.473); // Configure the equation to calculate NH4 concentration value
    double e = MQ135.readSensor();

    MQ135.setA(34.668);
    MQ135.setB(-3.369); // Configure the equation to calculate Aceton concentration value
    double f = MQ135.readSensor();

    s += "field3=" + String((a + b + c + d + e + f) / 6) + "&";
    s += "field4=" + String(bmp180.readPressure());

    s += "\r\nHost: " + String(server) + "\r\nConnection: close\r\n\r\n";

    const char *hello = s.c_str();
    Serial.print("This is request: ");
    Serial.println(hello);

    if (wifi.createTCP(server, hostPort))
    {
        Serial.print("create tcp ok\r\n");
    }
    else
    {
        Serial.print("create tcp err\r\n");
    }

    wifi.send((const uint8_t *)hello, strlen(hello));
    delay(250);
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);

    if (len > 0)
    {
        Serial.print("Received:[");
        for (uint32_t i = 0; i < len; i++)
        {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
    }

    if (wifi.releaseTCP())
    {
        Serial.print("release tcp ok\r\n");
    }
    else
    {
        Serial.print("release tcp err\r\n");
    }

    lastConnectionTime = millis(); // Update the last conenction time
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop()
{
    if (PRINT_VALUES)
    {
        Serial.println("\n===========================================================\n");

        // BMP180 - Barometric Pressure, Temperature, Altitude Sensor - Test Code
        // Read Altitude from barometric sensor, note that the sensor is 1m accurate
        Serial.println("BMP180::Output");

        Serial.print(F("\tAltitude: "));
        Serial.print(bmp180.readAltitude(), 1);
        Serial.println(F(" [meters]"));

        Serial.print(F("\tReal Altitude: "));
        Serial.print(bmp180.readAltitude(101500), 1);
        Serial.println(F(" [meters]"));

        Serial.print(F("\tPressure: "));
        Serial.print(bmp180.readPressure(), 1);
        Serial.println(F(" [Pa]"));

        Serial.print(F("\tSea Level Pressure: "));
        Serial.print(bmp180.readSealevelPressure(), 1);
        Serial.println(F(" [Pa]"));

        Serial.print(F("\tTemperature: "));
        Serial.print(bmp180.readTemperature(), 1);
        Serial.println(F(" [°C]"));

        Serial.println("");

        // DHT11 Humidity and Temperature Sensor - Test Code
        Serial.println("DHT11::Output");

        Serial.print(F("\tHumidity: "));
        Serial.print(dht.readHumidity());
        Serial.println(F(" [%]"));

        Serial.print(F("\tTemp: "));
        Serial.print(dht.readTemperature());
        Serial.print(F(" [°C]\t"));
        Serial.print(dht.readTemperature(true));
        Serial.println(F(" [°F]"));

        Serial.print(F("\tHeat Index: "));
        Serial.print(dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity(), false));
        Serial.print(F(" [°C]\t"));
        Serial.print(dht.computeHeatIndex(dht.readTemperature(true), dht.readHumidity(), true));
        Serial.println(F(" [°F]"));

        Serial.println("");

        // MQ135 - Carbon Monoxide Sensor - Test Code
        MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
        Serial.println("MQ135::Output");

        MQ135.setA(605.18);
        MQ135.setB(-3.937); // Configure the equation to calculate CO concentration value
        // MQ135CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
        Serial.print(F("\tCO: "));
        Serial.print(MQ135.readSensor());
        Serial.println(F(" [PPM]"));

        MQ135.setA(77.255);
        MQ135.setB(-3.18); // Configure the equation to calculate Alcohol concentration value
        // MQ135Alcohol = MQ135.readSensor(); // SSensor will read PPM concentration using the model, a and b values set previously or from the setup
        Serial.print(F("\tAlcohol: "));
        Serial.print(MQ135.readSensor());
        Serial.println(F(" [PPM]"));

        MQ135.setA(110.47);
        MQ135.setB(-2.862); // Configure the equation to calculate CO2 concentration value
        // MQ135CO2 = MQ135.readSensor() + 400; // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
        Serial.print(F("\tCO2: "));
        Serial.print(MQ135.readSensor() + 400);
        Serial.println(F(" [PPM]"));

        MQ135.setA(44.947);
        MQ135.setB(-3.445); // Configure the equation to calculate Toluen concentration value
        // MQ135Toluen = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
        Serial.print(F("\tToluen: "));
        Serial.print(MQ135.readSensor());
        Serial.println(F(" [PPM]"));

        MQ135.setA(102.2);
        MQ135.setB(-2.473); // Configure the equation to calculate NH4 concentration value
        // MQ135NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
        Serial.print(F("\tNH4: "));
        Serial.print(MQ135.readSensor());
        Serial.println(F(" [PPM]"));

        MQ135.setA(34.668);
        MQ135.setB(-3.369); // Configure the equation to calculate Aceton concentration value
        // MQ135Aceton = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
        Serial.print(F("\tAceton: "));
        Serial.print(MQ135.readSensor());
        Serial.println(F(" [PPM]"));

        Serial.println("");
    }

    // ESP8266 - WiFi - Test Code
    // Logic Level Converter - Bi-Directional - Test Code

    if (millis() - lastConnectionTime >= DELAY_TIME_SECONDS)
    {
        httpRequest();
    }

    delay(DELAY_TIME_SECONDS);
}
