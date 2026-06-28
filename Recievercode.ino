#include <RH_ASK.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Setup: Address 0x27, 16 chars, 2 lines
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RF Driver Setup: Speed 2000, RX on Pin 11, TX on Pin 12
RH_ASK rf_driver(2000, 11, 12);

// Pin Definitions
const int buzzer = 8;
const int IN1 = 7;
const int IN2 = 6;
const int monitorPin = 13; 

// Global Variables for State Management
String lastL1 = "";
String lastL2 = "";

void setup() {
  // Serial monitoring
  Serial.begin(9600);
  
  if (!rf_driver.init()) {
    Serial.println("RF Driver init failed!");
  }

  // Pin Modes
  pinMode(buzzer, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(monitorPin, OUTPUT);

  // LCD Initialization
  lcd.init();
  lcd.backlight();

  // ================= INITIAL DATA / STATE =================
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(buzzer, LOW);

  lcd.setCursor(0, 0);
  lcd.print("SYSTEM READY");
  lcd.setCursor(0, 1);
  lcd.print("MOTOR: ON");
  
  delay(2000); 
  lcd.clear();
}

void loop() {
  // --- REAL-TIME MONITORING ---
  digitalWrite(monitorPin, digitalRead(11));

  uint8_t buf[20];
  uint8_t buflen = sizeof(buf);

  // Check if RF data is received
  if (rf_driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; // String terminate 
    String data = String((char*)buf);
    
    Serial.println("Received: " + data);

    int alcohol = 0;
    int obstacle = 0;

    // Comma-separated data parsing (e.g., "1,0")
    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      alcohol = data.substring(0, commaIndex).toInt();
      obstacle = data.substring(commaIndex + 1).toInt();
    }

    // ================= BUZZER LOGIC =================
    digitalWrite(buzzer, alcohol); 

    // ================= MOTOR LOGIC =================
    if (obstacle == 1) {
      //  motor OFF
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    } else {
      //motor ON
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }

    // ================= LCD DISPLAY LOGIC =================
    String line1 = "";
    String line2 = "";

    if (alcohol == 1 && obstacle == 1) {
      line1 = "ALCOHOL DETECT";
      line2 = "OBSTACLE DETECT";
    } 
    else if (alcohol == 1) {
      line1 = "ALCOHOL DETECT";
      line2 = "WARNING!";
    } 
    else if (obstacle == 1) {
      line1 = "OBSTACLE DETECT";
      line2 = "MOTOR OFF";
    } 
    else {
      line1 = "SYSTEM SAFE";
      line2 = "MOTOR ON";
    }

    // Update LCD only if status changed to avoid flickering
    if (line1 != lastL1 || line2 != lastL2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(line1);
      lcd.setCursor(0, 1);
      lcd.print(line2);

      lastL1 = line1;
      lastL2 = line2;
    }
  }
}