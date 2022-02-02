/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

DS3231 clock;

bool century = false;
bool h12Flag;
bool pmFlag;

File myFile;
File myFile_att;


SoftwareSerial mySerial(10, 11);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

String student_name;
String registration_number;
int fee_balance = 0;

#define CLOCK_PIN 2
#define REG_PIN 3
#define ATT_PIN 4

bool log_data = false;

enum State {
  clock_,
  reg,
  att
};

State screen_state = clock_;

void registration(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("REGISTRATION MODE");
  lcd.setCursor(0, 1);
  lcd.print("Use serial port");

  Serial.println("Enter your name.");
  while (Serial.available() == 0) {}
  student_name = Serial.readString();

  Serial.println("Enter your Registration Number.");
  while (Serial.available() == 0) {}
  registration_number = Serial.readString();

  Serial.println("Enter your Fee Balance.");
  while (Serial.available() == 0) {}
  fee_balance = Serial.parseInt();

  student_name = student_name.substring(0, student_name.length() - 1);
  registration_number = registration_number.substring(0, registration_number.length() - 1);

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("*******DETAILS**************");
  Serial.print("Name : ");
  Serial.println(student_name);
  Serial.print("Registration Number : ");
  Serial.println(registration_number);
  Serial.print("Fee Balance : ");
  Serial.println(fee_balance);
  Serial.println("*********************************");
  Serial.println("");
  Serial.println("");
  Serial.println("");



  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);

  while (!getFingerprintEnroll() ) {};
  Serial.flush();


  String payload = "{\"name\":\"" + String(student_name) + "\",\"registration\":\"" + String(registration_number) + "\",\"fee_balance\":\"" + String(fee_balance) + "\",\"fingerprint_id\":\"" + String(id) + "\"}\n";
  myFile = SD.open("list.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(payload);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  screen_state = clock_;
}


void mark_register(int fingerprint_id) {
  int id;
  int fee_balance;
  String Name;
  String registration;
  // re-open the file for reading:
  myFile = SD.open("list.txt");
  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      String sd_content = myFile.readStringUntil('\n');
      StaticJsonDocument<200> doc;
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, sd_content);
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      }
      else {
        int id_ = doc["fingerprint_id"];
        int fee_balance_ = doc["fee_balance"];
        String Name_ = doc["name"];
        String registration_ = doc["registration"];
        id = id_;
        fee_balance = fee_balance_;
        Name = Name_;
        registration = registration_;
        if(id == fingerprint_id){
          log_data = true;
          break;
        }
        Serial.println("File opened successfully for reading");
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  myFile.close();
  /**********************************/
  if (log_data) {
    if (id == fingerprint_id) {
      if (fee_balance < 1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cleared for exams");

        //*********Time******************
        String timestamp = String(clock.getDate()) + "/" + String(clock.getMonth(century)) + "/" + String(clock.getYear()) + " " + String(clock.getHour(h12Flag, pmFlag)) + ":" + String(clock.getMinute());
        //********************************
        String attendance_details = String(Name) + "," + String(registration) + "," + String(timestamp) + "\n";
        Serial.print("Found You -> ");
        Serial.print("Time : ");
        Serial.print(attendance_details);
        Serial.print("  ");
        Serial.println("");
        /****************************/
        lcd.setCursor(0, 1);
        lcd.print(Name);
        /******************************/
         myFile.close();
        /* Write to class attendance list */
        myFile_att = SD.open("att.txt", FILE_WRITE);
        // if the file opened okay, write to it:
        if (myFile_att) {
          Serial.print("Writing to attendance.txt...");
          myFile_att.println(attendance_details);
          // close the file:
          myFile_att.close();
          Serial.println("done.");
        } else {
          // if the file didn't open, print an error:
          Serial.println("error opening attendance.txt");
        }
        screen_state = clock_;
        delay(5000);
        /**********************************************/
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please clear fees");
        lcd.setCursor(0, 1);
        lcd.print(Name);
        screen_state = clock_;
        delay(5000);
      }
    }
    log_data = false;
  }
  /************************************/
}

void show_time() {
  String timestamp = String(clock.getDate()) + "/" + String(clock.getMonth(century)) + "/" + String(clock.getYear()) + " " + String(clock.getHour(h12Flag, pmFlag)) + ":" + String(clock.getMinute());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CURRENT TIME");
  lcd.setCursor(14, 0);
  lcd.print(String(clock.getSecond()));
  lcd.setCursor(0, 1);
  lcd.print(timestamp);
  delay(1000);

}
void setup()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  Wire.begin();

  lcd.init();
  lcd.backlight();

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");



  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }

  pinMode(CLOCK_PIN, INPUT_PULLUP);
  pinMode(REG_PIN, INPUT_PULLUP);
  pinMode(ATT_PIN, INPUT_PULLUP);


  lcd.setCursor(0, 0);
  lcd.print("AUTHENTICATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM for EXAMS");

  delay(5000);

}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop() {

  if (digitalRead(CLOCK_PIN) == LOW) {
    Serial.println("PRESSED CLOCK BUTTON");
    screen_state = clock_;
  }
  else if (digitalRead(REG_PIN) == LOW) {
    Serial.println("PRESSED REGISTRATION BUTTON");
    screen_state = reg;
  }
  else if (digitalRead(ATT_PIN) == LOW) {
    Serial.println("PRESSED ATTENDANCE BUTTON");
    screen_state = att;
  }

  switch (screen_state) {
    case clock_:
      show_time();
      break;
    case reg:
      registration();
      break;
    case att:
      getFingerprintID();
      break;
    default:
      break;
  }


  delay(100);
}


uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}

uint8_t getFingerprintID() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ATTENDANCE MODE");

  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      lcd.setCursor(0, 1);
      lcd.print("PLACE FINGER");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);


  mark_register(int(finger.fingerID)); //A valid fingerprint has been abtained , Do something
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
