
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>

#define MIC_PIN 34
#define START_PIN 27
#define BUZZER_PIN 25

const uint8_t MAX_SHOTS = 10;
const uint32_t COUNTDOWN_MS = 3000;
const uint32_t BLIND_US = 180000UL;
const uint32_t SLEEP_MS = 300000UL;
const uint32_t BUZZER_MS = 300;
const uint32_t LONG_PRESS_MS = 3000UL;

// Mikrofon hassasiyeti
int threshold = 120;

U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI oled(
  U8G2_R0, 5, 17, 16
);

enum TimerState { IDLE, COUNTDOWN, RUNNING };
TimerState state = IDLE;

uint32_t countdownStart = 0;
uint32_t beepUs = 0;
uint32_t lastShotUs = 0;
uint32_t timeUs = 0;
uint32_t splitUs = 0;

uint32_t lastPressMs = 0;
uint32_t lastDrawMs = 0;
uint32_t lastProbeMs = 0;
uint32_t lastYieldMs = 0;
uint32_t lastActivityMs = 0;
uint32_t buzzerStartMs = 0;

uint32_t buttonDownMs = 0;
bool longPressHandled = false;

uint8_t shots = 0;
float dc = 2048.0f;
int peak = 0;

bool armed = true;
bool micMonitor = false;
bool buzzerActive = false;
bool buttonStable = HIGH;
bool buttonLastRead = HIGH;

// ===== EKRAN =====
void drawScreen(uint32_t shownTime) {
  char buf[28];

  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(0, 10, "SHOT TIMER");

  if (state == COUNTDOWN)
    oled.drawStr(94, 10, "WAIT");
  else if (state == RUNNING)
    oled.drawStr(106, 10, "RUN");

  oled.drawStr(0, 23, "TIME");

  oled.setFont(u8g2_font_logisoso24_tn);
  snprintf(buf, sizeof(buf), "%lu.%03lu",
    (unsigned long)(shownTime / 1000000UL),
    (unsigned long)((shownTime / 1000UL) % 1000UL));
  oled.drawStr(0, 49, buf);

  oled.setFont(u8g2_font_6x10_tf);
  snprintf(buf, sizeof(buf), "SPLIT %lu.%03lu",
    (unsigned long)(splitUs / 1000000UL),
    (unsigned long)((splitUs / 1000UL) % 1000UL));
  oled.drawStr(0, 62, buf);

  snprintf(buf, sizeof(buf), "%u/%u", shots, MAX_SHOTS);
  oled.drawStr(99, 62, buf);
  oled.sendBuffer();
}

// ===== BUZZER =====
void stopBuzzer() {
  digitalWrite(BUZZER_PIN, LOW);
  buzzerActive = false;
}

void updateBuzzer() {
  if (buzzerActive &&
      (uint32_t)(millis() - buzzerStartMs) >= BUZZER_MS) {
    stopBuzzer();
  }
}

// ===== START =====
void startTimer() {
  stopBuzzer();

  shots = 0;
  timeUs = 0;
  splitUs = 0;
  lastShotUs = 0;
  armed = true;

  countdownStart = millis();
  lastActivityMs = countdownStart;
  lastDrawMs = countdownStart;
  state = COUNTDOWN;

  drawScreen(3000000UL);
  Serial.println("START - GERI SAYIM");
}

// ===== OLCUM BASLANGICI =====
void beginMeasurement() {
  digitalWrite(BUZZER_PIN, HIGH);
  buzzerStartMs = millis();
  buzzerActive = true;

  beepUs = micros();
  lastShotUs = beepUs;
  armed = true;
  state = RUNNING;
  lastActivityMs = millis();

  drawScreen(0);
  Serial.println("BEEP - OLCUM BASLADI");
}

// ===== DEEP SLEEP =====
void enterDeepSleep() {
  stopBuzzer();

  Serial.println("DEEP SLEEP BASLIYOR");
  Serial.flush();

  oled.setPowerSave(1);

  rtc_gpio_pullup_en(GPIO_NUM_27);
  rtc_gpio_pulldown_dis(GPIO_NUM_27);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);

  esp_deep_sleep_start();
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  rtc_gpio_deinit(GPIO_NUM_27);

  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  analogReadResolution(12);
  analogSetPinAttenuation(MIC_PIN, ADC_11db);

  SPI.begin(18, -1, 23, 5);
  oled.begin();
  oled.setPowerSave(0);

  // Mikrofon DC kalibrasyonu
  long sum = 0;
  for (int i = 0; i < 500; i++) {
    sum += analogRead(MIC_PIN);
    delayMicroseconds(100);
  }
  dc = sum / 500.0f;

  if (esp_sleep_get_wakeup_cause() ==
      ESP_SLEEP_WAKEUP_EXT0) {

    Serial.println("BUTON ILE UYANILDI");

    while (digitalRead(START_PIN) == LOW) {
      delay(10);
    }
    delay(50);
  }

  buttonLastRead = digitalRead(START_PIN);
  buttonStable = buttonLastRead;

  lastActivityMs = millis();
  drawScreen(0);

  Serial.println("SHOT TIMER HAZIR");
  Serial.println("s: START");
  Serial.println("p: MIC MONITOR");
  Serial.println("+: Hassasiyet artir");
  Serial.println("-: Hassasiyet azalt");
}

// ===== LOOP =====
void loop() {
  uint32_t nowUs = micros();
  uint32_t nowMs = millis();

  updateBuzzer();

  // ===== MIKROFON ALGILAMA =====
  int adc = analogRead(MIC_PIN);

  int amplitude = abs(adc - (int)dc);

  if (amplitude > peak)
    peak = amplitude;

  // DC referans takibi
  if (amplitude < threshold) {
    dc += (adc - dc) / 512.0f;
  }

  // START butonu
  bool reading = digitalRead(START_PIN);

  if (reading != buttonLastRead)
    lastPressMs = nowMs;

  if (nowMs - lastPressMs >= 35 &&
      reading != buttonStable) {

    buttonStable = reading;

    if (buttonStable == LOW) {
      buttonDownMs = nowMs;
      longPressHandled = false;
    } else {
      if (!longPressHandled) {
        startTimer();
      }
    }
  }

  buttonLastRead = reading;

  // 3 saniye basili tutulursa uyku
  if (buttonStable == LOW &&
      !longPressHandled &&
      (uint32_t)(nowMs - buttonDownMs) >= LONG_PRESS_MS) {

    longPressHandled = true;

    while (digitalRead(START_PIN) == LOW) {
      delay(10);
    }

    enterDeepSleep();
  }

  // Seri monitor
  while (Serial.available()) {
    char c = Serial.read();

    if (c == 's' || c == 'S')
      startTimer();
    else if (c == '+') {
      threshold = max(30, threshold - 30);
      Serial.printf("ESIK: %d\n", threshold);
    }
    else if (c == '-') {
      threshold = min(2000, threshold + 30);
      Serial.printf("ESIK: %d\n", threshold);
    }
    else if (c == 'p') {
      micMonitor = !micMonitor;
      Serial.println(micMonitor ? "MIC ON" : "MIC OFF");
    }
  }

  // Geri sayim
  if (state == COUNTDOWN) {
    uint32_t elapsed = nowMs - countdownStart;

    if (elapsed >= COUNTDOWN_MS) {
      beginMeasurement();
    }
    else if (nowMs - lastDrawMs >= 50) {
      lastDrawMs = nowMs;
      drawScreen((COUNTDOWN_MS - elapsed) * 1000UL);
    }
  }

  // Mikrofon yeniden tetikleme
  if (amplitude < threshold / 2) {
    armed = true;
  }

  // Atis algilama
  if (state == RUNNING &&
      shots < MAX_SHOTS &&
      armed &&
      amplitude >= threshold &&
      (uint32_t)(micros() - lastShotUs) >= BLIND_US) {

    uint32_t shotUs = micros();

    lastShotUs = shotUs;
    timeUs = shotUs - beepUs;

    if (shots == 0)
      splitUs = timeUs;

    shots++;
    armed = false;
    lastActivityMs = millis();

    drawScreen(timeUs);

    Serial.printf(
      "ATIS %u | TIME %.3f | SPLIT %.3f\n",
      shots,
      timeUs / 1000000.0f,
      splitUs / 1000000.0f
    );
  }

  // Mikrofon izleme
  if (micMonitor && nowMs - lastProbeMs >= 200) {
    lastProbeMs = nowMs;

    Serial.printf(
      "ADC:%d DC:%d PEAK:%d ESIK:%d ARMED:%d\n",
      adc,
      (int)dc,
      peak,
      threshold,
      armed
    );

    peak = 0;
  }

  if (state == IDLE ||
      (state == RUNNING && shots >= MAX_SHOTS)) {

    if ((uint32_t)(millis() - lastActivityMs) >= SLEEP_MS &&
        digitalRead(START_PIN) == HIGH) {
      enterDeepSleep();
    }
  }

  if (nowMs - lastYieldMs >= 10) {
    lastYieldMs = nowMs;
    delay(1);
  }
}
