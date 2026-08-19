#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ============================================================
// RFID
// ============================================================

#define RFID_SS   15   // D8
#define RFID_RST  0    // D3

MFRC522 rfid(RFID_SS, RFID_RST);

// ============================================================
// LCD
// ============================================================

#define SDA_PIN 4      // D2
#define SCL_PIN 5      // D1

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ============================================================
// AUTH
// ============================================================

byte allowedUID[4] = {
  0x67, 0x3C, 0xD5, 0xE7
};

const char* PIN_CODE = "1234";

enum State {
  LOCKED,
  CARD_OK,
  UNLOCKED
};

State state = LOCKED;

String lastMessage = "";

// ============================================================
// LCD
// ============================================================

void showMsg(String a, String b = "") {

  String s = a + "|" + b;

  if (s == lastMessage)
    return;

  lastMessage = s;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(a);

  lcd.setCursor(0, 1);
  lcd.print(b);
}

// ============================================================
// LOCK
// ============================================================

void lockBot() {

  state = LOCKED;

  Serial.println("LOCK");

  showMsg(
    "Scan RFID card",
    "to begin"
  );
}

// ============================================================
// RFID
// ============================================================

void checkRFID() {

  if (state != LOCKED)
    return;

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print("UID: ");

  for (
    byte i = 0;
    i < rfid.uid.size;
    i++
  ) {

    if (rfid.uid.uidByte[i] < 0x10)
      Serial.print("0");

    Serial.print(
      rfid.uid.uidByte[i],
      HEX
    );

    if (i < rfid.uid.size - 1)
      Serial.print(":");
  }

  Serial.println();

  bool valid =
    rfid.uid.size == 4;

  for (
    byte i = 0;
    i < 4 && valid;
    i++
  ) {

    if (
      rfid.uid.uidByte[i] !=
      allowedUID[i]
    ) {
      valid = false;
    }
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // ----------------------------------------------------------
  // VALID CARD
  // ----------------------------------------------------------

  if (valid) {

    state = CARD_OK;

    Serial.println("CARD OK");

    showMsg(
      "Card OK",
      "Press * for PIN"
    );

    Serial.println("CARD_OK");
  }

  // ----------------------------------------------------------
  // INVALID CARD
  // ----------------------------------------------------------

  else {

    Serial.println(
      "UNKNOWN CARD"
    );

    showMsg(
      "Unknown card",
      "Try again"
    );

    delay(1200);

    showMsg(
      "Scan RFID card",
      "to begin"
    );
  }
}

// ============================================================
// ESP32 -> ESP8266
// ============================================================

void checkESP32() {

  if (!Serial.available())
    return;

  String command =
    Serial.readStringUntil('\n');

  command.trim();

  // ----------------------------------------------------------
  // LOCK
  // ----------------------------------------------------------

  if (command == "LOCK") {

    state = LOCKED;

    showMsg(
      "Scan RFID card",
      "to begin"
    );
  }

  // ----------------------------------------------------------
  // ENTER PIN
  // ----------------------------------------------------------

  else if (
    command == "ENTER_PIN"
  ) {

    if (state == CARD_OK) {

      showMsg(
        "Enter PIN:",
        ""
      );
    }
  }

  // ----------------------------------------------------------
  // PIN DIGIT
  // ----------------------------------------------------------

  else if (
    command.startsWith("PIN_DIGIT:")
  ) {

    if (state != CARD_OK)
      return;

    String digit =
      command.substring(10);

    showMsg(
      "Enter PIN:",
      "****"
    );
  }

  // ----------------------------------------------------------
  // CLEAR PIN
  // ----------------------------------------------------------

  else if (
    command == "PIN_CLEAR"
  ) {

    if (state == CARD_OK) {

      showMsg(
        "Enter PIN:",
        ""
      );
    }
  }

  // ----------------------------------------------------------
  // WRONG PIN
  // ----------------------------------------------------------

  else if (
    command == "PIN_FAIL"
  ) {

    state = CARD_OK;

    showMsg(
      "Wrong PIN!",
      "Press * to retry"
    );
  }

  // ----------------------------------------------------------
  // UNLOCK
  // ----------------------------------------------------------

  else if (
    command == "UNLOCK"
  ) {

    state = UNLOCKED;

    Serial.println(
      "UNLOCKED"
    );

    showMsg(
      "UNLOCKED",
      "Drive enabled"
    );
  }

  // ----------------------------------------------------------
  // OBSTACLE
  // ----------------------------------------------------------

  else if (
    command == "OBSTACLE"
  ) {

    if (state == UNLOCKED) {

      showMsg(
        "OBSTACLE!",
        "STOPPED"
      );
    }
  }

  // ----------------------------------------------------------
  // CLEAR
  // ----------------------------------------------------------

  else if (
    command == "CLEAR"
  ) {

    if (state == UNLOCKED) {

      showMsg(
        "UNLOCKED",
        "Drive enabled"
      );
    }
  }

  // ----------------------------------------------------------
  // SAFETY OFF
  // ----------------------------------------------------------

  else if (
    command == "SAFE_OFF"
  ) {

    if (state == UNLOCKED) {

      showMsg(
        "UNLOCKED",
        "Safety OFF"
      );
    }
  }

  // ----------------------------------------------------------
  // SAFETY ON
  // ----------------------------------------------------------

  else if (
    command == "SAFE_ON"
  ) {

    if (state == UNLOCKED) {

      showMsg(
        "UNLOCKED",
        "Drive enabled"
      );
    }
  }

  // ----------------------------------------------------------
  // NEED CARD
  // ----------------------------------------------------------

  else if (
    command == "NEED_CARD"
  ) {

    if (state == LOCKED) {

      showMsg(
        "Scan RFID card",
        "to begin"
      );
    }
  }

  // ----------------------------------------------------------
  // INVALID KEY
  // ----------------------------------------------------------

  else if (
    command == "KEY_INVALID"
  ) {

    if (state == CARD_OK) {

      showMsg(
        "Invalid key",
        "Use 0-9"
      );

      delay(700);

      showMsg(
        "Enter PIN:",
        ""
      );
    }
  }
}

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(115200);

  // LCD
  Wire.begin(
    SDA_PIN,
    SCL_PIN
  );

  lcd.init();
  lcd.backlight();

  // RFID
  SPI.begin();

  rfid.PCD_Init();

  delay(100);

  Serial.println();
  Serial.println(
    "================================"
  );
  Serial.println(
    " LITTLE HELPER ESP8266"
  );
  Serial.println(
    "================================"
  );

  rfid.PCD_DumpVersionToSerial();

  lockBot();
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  checkESP32();

  checkRFID();
}