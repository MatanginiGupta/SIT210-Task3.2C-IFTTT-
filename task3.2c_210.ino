/****
   Name : Matangini Gupta
   Student ID : 2210994810
   Task 3.2C - Create IFTTT Trigger
   Embedded Systems Development
****/

#include <WiFiNINA.h>
#include <BH1750.h>

// Please enter your sensitive data in the Secret tab
char ssid[] = "jio_4g";          // My network SSID (name)
char pass[] = "23jan2010";      // Network password (use for WPA, or use as a key for WEP)
int status = WL_IDLE_STATUS;           //  Wi-Fi radio's status
WiFiClient client;

char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/BHlight/json/with/key/5dSCxytPQo7E8zEZxW9PxM04XpjSrqYht9RKioRdYR";
 // my EVENT-NAME and KEY
String queryString = "?value1=&value2=";


unsigned long startTime;
unsigned long timerDuration = 20000;  // The timer can be adjusted according to my requirement
bool timerStarted = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Set the LED as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Attempt to connect to My Wi-Fi network:
  while (status != WL_CONNECTED) {
    Serial.print("Wait......Attempting to connect to the network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  // I'm connected now, so print out the data:
  Serial.println("---------------------------------------");
  Serial.println("You're now connected to the network!!!");
  Serial.println("---------------------------------------");
}

void loop() {
  if (!timerStarted) {
    startTime = millis();
    timerStarted = true;
  }
  // Check for user input (in this case, 'M' from Serial monitor)
  if (Serial.read() == 'M') {
    // Attempt to connect to the server
    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to the server");
    
      // Send HTTP request
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // end HTTP header

      // Read and print server response
      while (client.connected()) {
        if (client.available()) {
          // Read an incoming byte from the server and print it to the serial monitor:
          char c = client.read();
          Serial.print(c);
        }
      }
      client.stop();
      Serial.println();
      Serial.println("Disconnected now...");
      delay(10000);
      
      // Calculate elapsed time and check if the timer duration is reached
      unsigned long currentTime = millis();
      unsigned long elapsedTime = currentTime - startTime;
      if (elapsedTime >= timerDuration) {
        // Attempt to connect to the server again after the timer duration is reached
        if (client.connect(HOST_NAME, 80)) {
          Serial.println("Connected to the server");
      
          // Send HTTP request
          client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
          client.println("Host: " + String(HOST_NAME));
          client.println("Connection: close");
          client.println(); // end HTTP header

          // Read and print server response
          while (client.connected()) {
            if (client.available()) {
              // Read an incoming byte from the server and print it to the serial monitor:
              char c = client.read();
              Serial.print(c);
            }
          }
          // Reset the timer 
          timerStarted = false;
        }
        client.stop();
        Serial.println();
        Serial.println("Disconnected");
        }
      }
    }
  }
