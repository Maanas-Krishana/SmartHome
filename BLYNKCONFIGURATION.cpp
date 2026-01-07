// Get these from the Blynk Web Dashboard
#define BLYNK_TEMPLATE_ID "yourid"
#define BLYNK_TEMPLATE_NAME "Name"
#define BLYNK_AUTH_TOKEN "Yourtoken"

#define BLYNK_PRINT Serial // Enables debug prints

// --- LIBRARIES ---
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <time.h>

// --- Wi-Fi Credentials ---
char ssid[] = "name";     // <-- PASTE YOUR WIFI NAME HERE
char password[] = "password"; // <-- PASTE YOUR WIFI PASSWORD HERE

// --- NTP CONFIGURATION (for India Standard Time) ---
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

// --- HARDWARE CONFIGURATION ---
const int RELAY_PIN_1 = 26; // Light
const int RELAY_PIN_2 = 27; // Socket
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

// --- INITIALIZE COMPONENTS ---
BlynkTimer timer;

// --- CUSTOM TIMER VARIABLES ---
int light_start_hour = -1, light_start_minute = -1;
int light_end_hour = -1, light_end_minute = -1;
bool light_timer_active = false;


// --- BLYNK CONNECTION HANDLER (For Stability) ---

BLYNK_CONNECTED() {
  // When the device connects or reconnects, ask the server for the latest state of all widgets.
  Blynk.syncAll();
  Serial.println("Blynk connected. Syncing state with server.");
}


// --- BLYNK VIRTUAL PIN HANDLERS (App to Device) ---


// Called when the "Light" button (V1) is pressed
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  digitalWrite(RELAY_PIN_1, (pinValue == 1) ? RELAY_ON : RELAY_OFF);
  Serial.printf("Blynk: Light command received. State: %s\n", (pinValue == 1) ? "ON" : "OFF");

  // --- DEMONSTRATION MODE ---
  // Mock the power reading based on the light's state.
  if (pinValue == 1) {
    Blynk.virtualWrite(V3, 9.0); // Send 9.0 Watts to the app when ON
    Blynk.virtualWrite(V4, millis() / 3600000.0); // Also send a dummy energy value
  } else {
    Blynk.virtualWrite(V3, 0.0); // Send 0.0 Watts to the app when OFF
  }
}

// Called when the "Socket" button (V2) is pressed
BLYNK_WRITE(V2) {
  int pinValue = param.asInt();
  digitalWrite(RELAY_PIN_2, (pinValue == 1) ? RELAY_ON : RELAY_OFF);
  Serial.printf("Blynk: Socket command received. State: %s\n", (pinValue == 1) ? "ON" : "OFF");
}

// --- TIMER HANDLERS ---

// Called when the "Light ON Time" widget (V5) sends a time
BLYNK_WRITE(V5) {
  TimeInputParam t(param);
  if (t.hasStartTime()) {
    light_start_hour = t.getStartHour();
    light_start_minute = t.getStartMinute();
    Serial.printf("Timer Start Time Received: %02d:%02d\n", light_start_hour, light_start_minute);
  }
}

// Called when the "Light OFF Time" widget (V6) sends a time
BLYNK_WRITE(V6) {
  TimeInputParam t(param);
  if (t.hasStartTime()) {
    light_end_hour = t.getStartHour();
    light_end_minute = t.getStartMinute();
    Serial.printf("Timer End Time Received: %02d:%02d\n", light_end_hour, light_end_minute);
  }
}

// Called when the "Set Light Timer" button (V7) is pressed
BLYNK_WRITE(V7) {
  if (param.asInt() == 1) {
    // Check that BOTH times have been set before activating
    if (light_start_hour != -1 && light_end_hour != -1) {
      light_timer_active = true;
      Serial.println("Custom Timer has been ACTIVATED.");
    } else {
      Serial.println("Timer activation failed: Please set BOTH start and end times first.");
      light_timer_active = false;
    }
    Blynk.virtualWrite(V7, 0); // Reset the button in the app
  }
}


// --- PERIODIC FUNCTIONS ---

// This function checks if the current time matches the scheduled times
void checkCustomTimer() {
  if (!light_timer_active) return; // Do nothing if timer is not active

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to get NTP time for timer check.");
    return;
  }

  int current_hour = timeinfo.tm_hour;
  int current_minute = timeinfo.tm_min;

  // Check for ON time
  if (current_hour == light_start_hour && current_minute == light_start_minute) {
    if (digitalRead(RELAY_PIN_1) == RELAY_OFF) { // Only act if the state is different
      digitalWrite(RELAY_PIN_1, RELAY_ON);
      Blynk.virtualWrite(V1, 1); // IMPORTANT: Update the button widget in the app
      Serial.println("Custom Timer: Light has been turned ON.");
    }
  }

  // Check for OFF time
  if (current_hour == light_end_hour && current_minute == light_end_minute) {
    if (digitalRead(RELAY_PIN_1) == RELAY_ON) { // Only act if the state is different
      digitalWrite(RELAY_PIN_1, RELAY_OFF);
      Blynk.virtualWrite(V1, 0); // IMPORTANT: Update the button widget in the app
      Serial.println("Custom Timer: Light has been turned OFF.");
    }
  }
}


// --- SETUP FUNCTION  ---

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting Smart Controller (v12.0 Demonstration Mode)...");

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_1, RELAY_OFF);
  digitalWrite(RELAY_PIN_2, RELAY_OFF);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  timer.setInterval(30000L, checkCustomTimer);
}


// --- MAIN LOOP ---

void loop() {
  Blynk.run();
  timer.run();
}