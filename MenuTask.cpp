#include "MenuTask.h"

MenuTask::MenuTask(NixieDisplay* nixie, Chronodot* rtc, HvSupply* hv) {
  _nixie = nixie;
  _rtc = rtc;
  _hv = hv;
}

void MenuTask::task() {
    if (Serial.available() > 0) {
    if(_mustsetdate) {
      String input = Serial.readString();
      Serial.print(F("Input: "));
      Serial.println(input);
      char* datestr = strtok(input.c_str(), " ");      
      char* timestr = strtok(0, " ");
      if (datestr != 0 && timestr != 0) {
        char* year = strtok(datestr, "-");
        char* month = strtok(0, "-");
        char* day = strtok(0, "-");
        char* hour = strtok(timestr, ":");
        char* minute = strtok(0, ":");
        char* second = strtok(0, ":");
        if(year != 0 && month != 0 && day != 0 && hour != 0 && minute != 0 && second != 0) {
          Serial.print(F("Setting year: "));
          Serial.println(year);
          Serial.print(F("Setting month: "));
          Serial.println(month);
          Serial.print(F("Setting day: "));
          Serial.println(day);
          Serial.print(F("Setting hour: "));
          Serial.println(hour);
          Serial.print(F("Setting minute: "));
          Serial.println(minute);
          Serial.print(F("Setting second: "));
          Serial.println(second);
          _rtc->adjust(DateTime(atoi(year), atoi(month), atoi(day), 
            atoi(hour), atoi(minute), atoi(second), 0, 0));
        } else {
          Serial.println(F("[ERROR] Cannot read input - date or time"));
        }
      } else {
        Serial.println(F("[ERROR] Cannot read input"));
      }
      _mustsetdate = false;
      printMenu();
    } else {
      int in = Serial.read();
      if (in == 49) {
        Serial.println(F("You have entered [1]"));
        printTime();
        printMenu();
      } else if (in == 50) {
        Serial.println(F("You have entered [2] - Enter date/time in this format: 'YYYY-MM-DD HH:MM:SS'"));
        _mustsetdate = true;
      } else if (in == 51) {
        Serial.println(F("You have entered [3] - Running slot machine effect..."));
        _nixie->runSlotMachine();
        Serial.println(F("Done."));
        printMenu();
      } else if (in == 52) {
        Serial.println(F("You have entered [4] - Toggling HV"));
        if(_hv->isOn()) {
          _hv->switchOff();
          Serial.println(F("HV supply is now turned off"));
        } else {
          _hv->switchOn();
          Serial.println(F("HV supply is now turned on - 170V on board!!"));
        }
        printMenu();
      } else {
        Serial.println(F("[ERROR] Whut?"));
        printMenu();
      }
    }
  }
}

void MenuTask::printMenu() {
  Serial.println(F(""));
  Serial.println(F("Menu"));
  Serial.println(F("----"));
  Serial.println(F("  1. Get time"));
  Serial.println(F("  2. Set time"));
  Serial.println(F("  3. Run slot machine effect"));
  Serial.println(F("  4. Toggle HV"));
  Serial.println(F(""));
}

void MenuTask::printTime() {
  DateTime now = _rtc->now();
  Serial.print(F("Current time is:  "));
  if(now.hour() < 10) Serial.print(F("0"));
  Serial.print(now.hour(), DEC);
  Serial.print(F(":"));
  if(now.minute() < 10) Serial.print(F("0"));
  Serial.print(now.minute(), DEC);
  Serial.print(F(":"));
  if(now.second() < 10) Serial.print(F("0"));
  Serial.print(now.second(), DEC);
  Serial.println();
}
