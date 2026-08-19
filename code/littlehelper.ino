#include <Bluepad32.h>
#include <Keypad.h>

// ============================================================
// MOTOR PINS
// ============================================================

#define DIR1 2
#define PWM1 15

#define DIR2 32
#define PWM2 33

// ============================================================
// ULTRASONIC + BUZZER
// ============================================================

#define TRIG   19
#define ECHO   21
#define BUZZER 23

// ============================================================
// ESP32 <-> ESP8266 UART2
//
// ESP32 GPIO17 TX2 -> ESP8266 RX GPIO3
// ESP32 GPIO16 RX2 <- ESP8266 TX GPIO1
// ============================================================

#define ESP_RX 16
#define ESP_TX 17

HardwareSerial ESPSerial(2);

// ============================================================
// KEYPAD
//
// Experimentally confirmed electrical mapping:
//
// 1 = GPIO27 <-> GPIO18
// 2 = GPIO14 <-> GPIO18
// 3 = GPIO13 <-> GPIO18
// A = GPIO5  <-> GPIO18
//
// 4 = GPIO27 <-> GPIO4
// 5 = GPIO14 <-> GPIO4
// 6 = GPIO13 <-> GPIO4
// B = GPIO5  <-> GPIO4
//
// 7 = GPIO27 <-> GPIO25
// 8 = GPIO14 <-> GPIO25
// 9 = GPIO13 <-> GPIO25
// C = GPIO5  <-> GPIO25
//
// * = GPIO27 <-> GPIO26
// 0 = GPIO14 <-> GPIO26
// # = GPIO13 <-> GPIO26
// D = GPIO5  <-> GPIO26
// ============================================================

byte rowPins[4] = {
  27,
  14,
  13,
  5
};

byte colPins[4] = {
  18,
  4,
  25,
  26
};

char keys[4][4] = {

  {'1', '4', '7', '*'},

  {'2', '5', '8', '0'},

  {'3', '6', '9', '#'},

  {'A', 'B', 'C', 'D'}
};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  4,
  4
);

// ============================================================
// CONTROLLER
// ============================================================

ControllerPtr myController = nullptr;

// ============================================================
// AUTHENTICATION STATE
// ============================================================

bool cardOK = false;
bool unlocked = false;

String pin = "";

// ============================================================
// SAFETY
// ============================================================

bool obstacleEnabled = true;
bool obstacleActive = false;

bool prevCross = false;
bool prevCircle = false;
bool prevTriangle = false;

// ============================================================
// DRIVE
// ============================================================

int throttle = 0;
int steer = 0;

// ============================================================
// BUZZER
// ============================================================

void beep(int duration) {

  // Don't interrupt the continuous obstacle alarm.
  if (obstacleActive)
    return;

  digitalWrite(BUZZER, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
}

void successBeep() {

  if (obstacleActive)
    return;

  digitalWrite(BUZZER, HIGH);
  delay(120);

  digitalWrite(BUZZER, LOW);
  delay(60);

  digitalWrite(BUZZER, HIGH);
  delay(180);

  digitalWrite(BUZZER, LOW);
}

void errorBeep() {

  if (obstacleActive)
    return;

  digitalWrite(BUZZER, HIGH);
  delay(100);

  digitalWrite(BUZZER, LOW);
  delay(80);

  digitalWrite(BUZZER, HIGH);
  delay(100);

  digitalWrite(BUZZER, LOW);
}

// ============================================================
// MOTORS
// ============================================================

void motor(
  int dirPin,
  int pwmPin,
  int speed
) {

  speed = constrain(
    speed,
    -255,
    255
  );

  digitalWrite(
    dirPin,
    speed >= 0 ? HIGH : LOW
  );

  analogWrite(
    pwmPin,
    abs(speed)
  );
}

void stopMotors() {

  analogWrite(
    PWM1,
    0
  );

  analogWrite(
    PWM2,
    0
  );
}

// ============================================================
// LOCK BOT
// ============================================================

void lockBot() {

  cardOK = false;
  unlocked = false;

  pin = "";

  throttle = 0;
  steer = 0;

  stopMotors();

  ESPSerial.println("LOCK");

  Serial.println();
  Serial.println("================================");
  Serial.println("BOT LOCKED");
  Serial.println("================================");
}

// ============================================================
// CONTROLLER CONNECTED
// ============================================================

void onConnectedController(
  ControllerPtr ctl
) {

  myController = ctl;

  Serial.println(
    "CONTROLLER CONNECTED"
  );
}

void onDisconnectedController(
  ControllerPtr ctl
) {

  if (myController == ctl) {

    myController = nullptr;

    throttle = 0;
    steer = 0;

    stopMotors();

    Serial.println(
      "CONTROLLER DISCONNECTED"
    );
  }
}

// ============================================================
// ESP8266 COMMUNICATION
// ============================================================

void checkESP8266() {

  if (!ESPSerial.available())
    return;

  String command =
    ESPSerial.readStringUntil('\n');

  command.trim();

  if (command.length() == 0)
    return;

  Serial.print(
    "ESP8266: "
  );

  Serial.println(command);

  // ----------------------------------------------------------
  // RFID ACCEPTED
  // ----------------------------------------------------------

  if (command == "CARD_OK") {

    cardOK = true;
    unlocked = false;
    pin = "";

    Serial.println(
      "RFID ACCEPTED"
    );

    successBeep();
  }

  // ----------------------------------------------------------
  // LOCK
  // ----------------------------------------------------------

  else if (command == "LOCK") {

    cardOK = false;
    unlocked = false;
    pin = "";

    throttle = 0;
    steer = 0;

    stopMotors();

    Serial.println(
      "LOCK RECEIVED"
    );
  }

  // ----------------------------------------------------------
  // UNLOCK
  // ----------------------------------------------------------

  else if (command == "UNLOCK") {

    unlocked = true;
    cardOK = true;

    pin = "";

    Serial.println(
      "BOT UNLOCKED"
    );

    successBeep();
  }

  // ----------------------------------------------------------
  // WRONG PIN
  // ----------------------------------------------------------

  else if (command == "PIN_FAIL") {

    pin = "";

    Serial.println(
      "PIN FAILED"
    );

    errorBeep();
  }
}

// ============================================================
// KEYPAD
// ============================================================

void checkKeypad() {

  char key = keypad.getKey();

  if (!key)
    return;

  // Every key gets a short beep.
  beep(60);

  Serial.print(
    "KEY PRESSED: "
  );

  Serial.println(key);

  // ==========================================================
  // *
  // ==========================================================

  if (key == '*') {

    // If currently driving, * locks the bot.
    if (unlocked) {

      Serial.println(
        "STAR -> LOCK"
      );

      lockBot();

      return;
    }

    // RFID accepted -> start PIN entry.
    if (cardOK) {

      pin = "";

      Serial.println(
        "ENTER PIN"
      );

      ESPSerial.println(
        "ENTER_PIN"
      );

      return;
    }

    // No RFID yet.
    Serial.println(
      "NO RFID CARD"
    );

    ESPSerial.println(
      "NEED_CARD"
    );

    return;
  }

  // ==========================================================
  // #
  // ==========================================================

  if (key == '#') {

    if (!cardOK) {

      Serial.println(
        "NO RFID CARD"
      );

      ESPSerial.println(
        "NEED_CARD"
      );

      return;
    }

    if (unlocked) {

      Serial.println(
        "BOT ALREADY UNLOCKED"
      );

      return;
    }

    pin = "";

    Serial.println(
      "PIN CLEARED"
    );

    ESPSerial.println(
      "PIN_CLEAR"
    );

    return;
  }

  // ==========================================================
  // NUMBER
  // ==========================================================

  if (
    key >= '0' &&
    key <= '9'
  ) {

    if (!cardOK) {

      Serial.println(
        "NO RFID CARD"
      );

      ESPSerial.println(
        "NEED_CARD"
      );

      return;
    }

    if (unlocked) {

      Serial.println(
        "BOT ALREADY UNLOCKED"
      );

      return;
    }

    if (pin.length() < 4) {

      pin += key;

      Serial.print(
        "PIN: "
      );

      for (
        unsigned int i = 0;
        i < pin.length();
        i++
      ) {

        Serial.print('*');
      }

      Serial.println();

      ESPSerial.print(
        "PIN_DIGIT:"
      );

      ESPSerial.println(key);
    }

    // Four digits entered.
    if (pin.length() == 4) {

      // ======================================================
      // PIN = 1234
      // ======================================================

      if (pin == "1234") {

        Serial.println(
          "CORRECT PIN"
        );

        unlocked = true;

        pin = "";

        ESPSerial.println(
          "UNLOCK"
        );

        successBeep();
      }

      else {

        Serial.println(
          "WRONG PIN"
        );

        pin = "";

        ESPSerial.println(
          "PIN_FAIL"
        );

        errorBeep();
      }
    }

    return;
  }

  // ==========================================================
  // A / B / C / D
  // ==========================================================

  if (
    key == 'A' ||
    key == 'B' ||
    key == 'C' ||
    key == 'D'
  ) {

    Serial.print(
      "INVALID PIN KEY: "
    );

    Serial.println(key);

    ESPSerial.println(
      "KEY_INVALID"
    );

    return;
  }
}

// ============================================================
// CONTROLLER
// ============================================================

void checkController() {

  if (
    !myController ||
    !myController->isConnected()
  ) {

    throttle = 0;
    steer = 0;

    stopMotors();

    return;
  }

  bool cross =
    myController->a();

  bool circle =
    myController->b();

  bool triangle =
    myController->x();

  // ==========================================================
  // X / CROSS = SAFETY TOGGLE
  // ==========================================================

  if (
    cross &&
    !prevCross
  ) {

    obstacleEnabled =
      !obstacleEnabled;

    if (!obstacleEnabled) {

      obstacleActive = false;

      digitalWrite(
        BUZZER,
        LOW
      );

      Serial.println(
        "SAFETY: OFF"
      );

      ESPSerial.println(
        "SAFE_OFF"
      );
    }

    else {

      Serial.println(
        "SAFETY: ON"
      );

      ESPSerial.println(
        "SAFE_ON"
      );
    }
  }

  // ==========================================================
  // O / CIRCLE = LOCK
  // ==========================================================

  if (
    circle &&
    !prevCircle
  ) {

    Serial.println(
      "CIRCLE -> LOCK"
    );

    lockBot();
  }

  // ==========================================================
  // TRIANGLE = MANUAL BUZZER
  // ==========================================================

  if (
    triangle &&
    !prevTriangle
  ) {

    Serial.println(
      "TRIANGLE -> BEEP"
    );

    beep(200);
  }

  prevCross = cross;
  prevCircle = circle;
  prevTriangle = triangle;

  // ==========================================================
  // DRIVE INPUT
  // ==========================================================

  throttle =
    map(
      myController->axisY(),
      -511,
      512,
      -255,
      255
    );

  steer =
    map(
      myController->axisRX(),
      -511,
      512,
      -255,
      255
    );

  if (
    abs(throttle) < 20
  )
    throttle = 0;

  if (
    abs(steer) < 20
  )
    steer = 0;
}

// ============================================================
// ULTRASONIC
// ============================================================

void checkUltrasonic() {

  static unsigned long lastCheck = 0;

  if (
    millis() - lastCheck < 100
  )
    return;

  lastCheck = millis();

  // Trigger pulse
  digitalWrite(
    TRIG,
    LOW
  );

  delayMicroseconds(2);

  digitalWrite(
    TRIG,
    HIGH
  );

  delayMicroseconds(10);

  digitalWrite(
    TRIG,
    LOW
  );

  unsigned long duration =
    pulseIn(
      ECHO,
      HIGH,
      30000
    );

  int distance =
    duration
      ? duration / 58
      : 0;

  // ==========================================================
  // SAFETY OFF
  // ==========================================================

  if (!obstacleEnabled) {

    obstacleActive = false;

    digitalWrite(
      BUZZER,
      LOW
    );

    return;
  }

  // ==========================================================
  // SAFETY ON
  // ==========================================================

  bool detected =
    distance > 0 &&
    distance < 15;

  bool previousObstacle =
    obstacleActive;

  obstacleActive =
    detected;

  // Continuous obstacle buzzer.
  digitalWrite(
    BUZZER,
    obstacleActive ? HIGH : LOW
  );

  // Only report changes.
  if (
    obstacleActive &&
    !previousObstacle
  ) {

    Serial.print(
      "OBSTACLE: "
    );

    Serial.print(
      distance
    );

    Serial.println(
      " cm"
    );

    ESPSerial.println(
      "OBSTACLE"
    );
  }

  else if (
    !obstacleActive &&
    previousObstacle
  ) {

    Serial.println(
      "OBSTACLE CLEAR"
    );

    ESPSerial.println(
      "CLEAR"
    );
  }
}

// ============================================================
// DRIVE
// ============================================================

void drive() {

  // Must be unlocked.
  if (!unlocked) {

    stopMotors();

    return;
  }

  // Safety obstacle stop.
  if (obstacleActive) {

    stopMotors();

    return;
  }

  int left =
    constrain(
      throttle - steer,
      -255,
      255
    );

  int right =
    constrain(
      throttle + steer,
      -255,
      255
    );

  motor(
    DIR1,
    PWM1,
    left
  );

  motor(
    DIR2,
    PWM2,
    right
  );
}

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  // ----------------------------------------------------------
  // UART2
  // ----------------------------------------------------------

  ESPSerial.begin(
    115200,
    SERIAL_8N1,
    ESP_RX,
    ESP_TX
  );

  // ----------------------------------------------------------
  // MOTOR PINS
  // ----------------------------------------------------------

  pinMode(
    DIR1,
    OUTPUT
  );

  pinMode(
    PWM1,
    OUTPUT
  );

  pinMode(
    DIR2,
    OUTPUT
  );

  pinMode(
    PWM2,
    OUTPUT
  );

  // ----------------------------------------------------------
  // ULTRASONIC
  // ----------------------------------------------------------

  pinMode(
    TRIG,
    OUTPUT
  );

  pinMode(
    ECHO,
    INPUT
  );

  digitalWrite(
    TRIG,
    LOW
  );

  // ----------------------------------------------------------
  // BUZZER
  // ----------------------------------------------------------

  pinMode(
    BUZZER,
    OUTPUT
  );

  digitalWrite(
    BUZZER,
    LOW
  );

  // ----------------------------------------------------------
  // MOTORS OFF
  // ----------------------------------------------------------

  stopMotors();

  // ----------------------------------------------------------
  // BLUEPAD32
  // ----------------------------------------------------------

  BP32.setup(
    &onConnectedController,
    &onDisconnectedController
  );

  Serial.println();
  Serial.println(
    "================================"
  );
  Serial.println(
    "       LITTLE HELPER ESP32"
  );
  Serial.println(
    "================================"
  );
  Serial.println(
    "UART2 RX = GPIO16"
  );
  Serial.println(
    "UART2 TX = GPIO17"
  );
  Serial.println(
    "TRIG = GPIO19"
  );
  Serial.println(
    "ECHO = GPIO21"
  );
  Serial.println(
    "BUZZER = GPIO23"
  );
  Serial.println(
    "KEYPAD READY"
  );
  Serial.println(
    "X = SAFETY ON/OFF"
  );
  Serial.println(
    "O = LOCK"
  );
  Serial.println(
    "TRIANGLE = BEEP"
  );
  Serial.println(
    "================================"
  );
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  BP32.update();

  checkESP8266();

  checkKeypad();

  checkUltrasonic();

  checkController();

  drive();
}