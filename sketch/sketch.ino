#include <Arduino_RouterBridge.h>
#include <IskakINO_LiquidCrystal_I2C.h>
#include <Modulino.h>


ModulinoButtons buttons;

LiquidCrystal_I2C lcd(20, 4);

void showStartup() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("France Info");
  lcd.setCursor(0, 1);
  lcd.print("Volume : 50%");
  lcd.setCursor(0, 3);
  lcd.print("Etat : ON");
}

void updateLCDFromPayload(String payload) {

  int p1 = payload.indexOf('|');
  int p2 = payload.indexOf('|', p1 + 1);

  if (p1 == -1 || p2 == -1) {
    lcd.setCursor(0, 3);
    lcd.print("Erreur parsing     ");
    return;
  }

  String station = payload.substring(0, p1);
  String volume  = payload.substring(p1 + 1, p2);
  String status  = payload.substring(p2 + 1);

  lcd.clear();

  lcd.setCursor(0, 0);
  //lcd.print("Radio : ");
  lcd.print(station);

  lcd.setCursor(0, 1);
  lcd.print("Volume : ");
  lcd.print(volume);
  lcd.print("%");

  lcd.setCursor(0, 3);
  lcd.print("Etat : ");
  lcd.print(status);
}

void setup() {
  Bridge.begin();
  Monitor.begin();
  Modulino.begin();
  buttons.begin();

  lcd.begin();
  lcd.backlight();

  showStartup();
}

void loop() {
  if (buttons.update()) {

    if (buttons.isPressed('A')) {

      RpcCall r = Bridge.call("next_station");

      String payload;

      if (r.result(payload)) {
        updateLCDFromPayload(payload);
      } else {
        lcd.setCursor(0, 3);
        lcd.print("Erreur Bridge      ");
      }
    }


    if (buttons.isPressed('B')) {
      RpcCall r = Bridge.call("next_volume");
      String payload;
      if (r.result(payload)) {
        updateLCDFromPayload(payload);
      }
    }

    if (buttons.isPressed('C')) {
      RpcCall r = Bridge.call("stop_radio");
      String payload;
      if (r.result(payload)) {
        updateLCDFromPayload(payload);
      }
    }
  }
}
