#include "HX711.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>  // Hapus jika tidak pakai LCD

// Objek
HX711 scale;
Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Opsional - pakai jika ada LCD

// Pin
#define DOUT  3
#define CLK   2
#define BUZZER_PIN 4
#define SERVO_PIN 5

// Konstanta
const float thresholdKg = 100.0; // Ambang beban dalam gram
const int openPos = 0;   // Posisi servo buka gerbang
const int closePos = 90; // Posisi servo tutup gerbang
float calibration_factor = 1005.0;

void setup() {
  Serial.begin(9600);

  // Load Cell
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);    // Nanti dikalibrasi dulu
  scale.tare();         // Set titik nol

  // Servo & Buzzer
  gateServo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);

  // LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("Sistem Siap");

  // Awal: buka gerbang
  gateServo.write(openPos);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  float weight = scale.get_units(10); // Ambil rata-rata 10 pembacaan

  // Tampilkan berat di Serial dan LCD
  Serial.print("Berat: ");
  Serial.print(weight, 2);
  Serial.println(" gram");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Berat:");
  lcd.setCursor(0, 1);
  lcd.print(weight, 2);
  lcd.print(" gram");

  if (weight > thresholdKg) {
    // Beban terlalu berat
    gateServo.write(closePos);  // Tutup gerbang
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  } else {
    // Beban aman
    gateServo.write(openPos);   // Buka gerbang
    digitalWrite(BUZZER_PIN, LOW);
    delay(200); // Jaga stabilitas
  }
}
