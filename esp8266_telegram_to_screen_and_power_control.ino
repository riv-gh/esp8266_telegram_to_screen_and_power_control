#include "config.h"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
String botToken = BOT_TOKEN;
String adminChatId = ADMIN_CHAT_ID;

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

unsigned long lastCheck = 0;


void pressPower() {
  digitalWrite(BTN_IN_PC, HIGH);
  delay(500);
  digitalWrite(BTN_IN_PC, LOW);
}

// --------------------- OLED ERROR ---------------------
void showError(const String &msg) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("ERROR:");
  display.println(msg);
  display.display();
}


// --------------------- TRANSLIT ---------------------
String translit(const String &src) {
  String out = "";
  for (int i = 0; i < src.length(); i++) {
    unsigned char c = src[i];

    if (c == 0xD0 || c == 0xD1) {
      unsigned char c2 = src[i + 1];
      i++;

      uint16_t code = (c << 8) | c2;

      switch (code) {
        case 0xD090: out += "A"; break;
        case 0xD091: out += "B"; break;
        case 0xD092: out += "V"; break;
        case 0xD093: out += "G"; break;
        case 0xD094: out += "D"; break;
        case 0xD095: out += "E"; break;
        case 0xD081: out += "Yo"; break;
        case 0xD096: out += "Zh"; break;
        case 0xD097: out += "Z"; break;
        case 0xD098: out += "I"; break;
        case 0xD099: out += "Y"; break;
        case 0xD09A: out += "K"; break;
        case 0xD09B: out += "L"; break;
        case 0xD09C: out += "M"; break;
        case 0xD09D: out += "N"; break;
        case 0xD09E: out += "O"; break;
        case 0xD09F: out += "P"; break;
        case 0xD0A0: out += "R"; break;
        case 0xD0A1: out += "S"; break;
        case 0xD0A2: out += "T"; break;
        case 0xD0A3: out += "U"; break;
        case 0xD0A4: out += "F"; break;
        case 0xD0A5: out += "Kh"; break;
        case 0xD0A6: out += "Ts"; break;
        case 0xD0A7: out += "Ch"; break;
        case 0xD0A8: out += "Sh"; break;
        case 0xD0A9: out += "Sch"; break;
        case 0xD0AA: out += ""; break;
        case 0xD0AB: out += "Y"; break;
        case 0xD0AC: out += ""; break;
        case 0xD0AD: out += "E"; break;
        case 0xD0AE: out += "Yu"; break;
        case 0xD0AF: out += "Ya"; break;

        case 0xD0B0: out += "a"; break;
        case 0xD0B1: out += "b"; break;
        case 0xD0B2: out += "v"; break;
        case 0xD0B3: out += "g"; break;
        case 0xD0B4: out += "d"; break;
        case 0xD0B5: out += "e"; break;
        case 0xD191: out += "yo"; break;
        case 0xD0B6: out += "zh"; break;
        case 0xD0B7: out += "z"; break;
        case 0xD0B8: out += "i"; break;
        case 0xD0B9: out += "y"; break;
        case 0xD0BA: out += "k"; break;
        case 0xD0BB: out += "l"; break;
        case 0xD0BC: out += "m"; break;
        case 0xD0BD: out += "n"; break;
        case 0xD0BE: out += "o"; break;
        case 0xD0BF: out += "p"; break;
        case 0xD180: out += "r"; break;
        case 0xD181: out += "s"; break;
        case 0xD182: out += "t"; break;
        case 0xD183: out += "u"; break;
        case 0xD184: out += "f"; break;
        case 0xD185: out += "kh"; break;
        case 0xD186: out += "ts"; break;
        case 0xD187: out += "ch"; break;
        case 0xD188: out += "sh"; break;
        case 0xD189: out += "sch"; break;
        case 0xD18A: out += ""; break;
        case 0xD18B: out += "y"; break;
        case 0xD18C: out += ""; break;
        case 0xD18D: out += "e"; break;
        case 0xD18E: out += "yu"; break;
        case 0xD18F: out += "ya"; break;

        default:
          out += "?";
      }
    } else {
      out += (char)c;
    }
  }
  return out;
}


// --------------------- WIFI RECONNECT ---------------------
bool reconnectWiFi() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi");
  display.println("reconnect...");
  display.display();

  WiFi.disconnect();
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    if (millis() - start > 15000) {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("WiFi FAIL");
      display.display();
      return false;
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi OK");
  display.display();
  return true;
}


// --------------------- SETUP ---------------------
void setup() {
  Serial.begin(115200);

  pinMode(BTN_IN_PC, OUTPUT);
  digitalWrite(BTN_IN_PC, LOW);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting");
  display.println("WiFi...");
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);

  WiFi.begin(ssid, password);
  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    if (millis() - startAttempt > 15000) {
      showError("WiFi timeout");
      return;
    }
  }

  client.setInsecure();
  client.setTimeout(1500);


  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi OK");
  display.println("Bot ready");
  display.display();
}


// --------------------- LOOP ---------------------
void loop() {

  // ---- WiFi контроль ----
  if (WiFi.status() != WL_CONNECTED) {
    if (!reconnectWiFi()) {
      delay(2000);
      return;
    }
  }

  // ---- Telegram ----
  if (millis() - lastCheck > 5000) {
    int numNew = bot.getUpdates(bot.last_message_received + 1);

    if (numNew < 0) {
      showError("Telegram error");
      client.stop();
      delay(500);
      client.connect("api.telegram.org", 443);
      lastCheck = millis();
      return;
    }

    while (numNew) {
      for (int i = 0; i < numNew; i++) {

        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;
        String oled_text = translit(text);
        String username = bot.messages[i].from_name;
        String oled_username = translit(username);
        if (oled_username.length() > 9) {
          oled_username = oled_username.substring(0, 9);
        }

        bot.sendMessage(chat_id, "Displayed on the OLED screen:\n\n" "```\n" + oled_username + ":\n" + oled_text + "\n```", "Markdown" );

        display.clearDisplay();
        display.setCursor(0, 0);

        if (username.length() > 0) display.print(oled_username);
        else display.print("<unknown>");

        display.println(":");

        if (text.length() > 0) display.println(oled_text);
        else display.print("<unsupported>");

        display.display();

        if (chat_id == adminChatId) {

            // ---------- /start ----------
            if (text == "/start") {
              bot.sendMessage(chat_id,
                "Админ-режим активен.\n"
                "Доступные команды:\n"
                "/power — нажать кнопку питания\n"
                "/screen_off — очистить экран\n"
                "/status — статус ESP\n"
                "/message_<id> <text> — отправить сообщение пользователю",
                ""
              );
            }

            // ---------- /power ----------
            if (text == "/power") {
              bot.sendMessage(chat_id, "Нажимаю кнопку питания", "");
              pressPower();
            }

            // ---------- /power_off ----------
            if (text == "/screen_off") {
              display.clearDisplay();
              display.display();
              bot.sendMessage(chat_id, "Экран очищен", "");
            }

            // ---------- /status ----------
            if (text == "/status") {
              bot.sendMessage(chat_id, "ESP работает. IP: " + WiFi.localIP().toString(), "");
            }

            // ---------- /message_<id> <text> ----------
            if (text.startsWith("/message_")) {
              int spaceIndex = text.indexOf(' ');
              if (spaceIndex > 0) {
                String cmd = text.substring(0, spaceIndex);
                String msg = text.substring(spaceIndex + 1);

                String targetId = cmd.substring(9); // после "/message_"

                bot.sendMessage(targetId, msg, "");
                bot.sendMessage(chat_id, "Отправлено пользователю " + targetId, "");
              } else {
                bot.sendMessage(chat_id, "Формат: /message_<id> <text>", "");
              }
            }

        }
        // else {
        //     // ---------- обычный пользователь ----------
        //     if (text == "/start") {
        //       bot.sendMessage(chat_id,
        //         "Привет! Это бот удалённого управления.\n"
        //         "Ваши сообщения пересылаются администратору.",
        //         ""
        //       );
        //     } else {
        //       bot.sendMessage(adminChatId, "" + username + " [" + chat_id + "]:\n" + text, "");
        //     }
        // }
        else {
          bot.sendMessage(adminChatId, "" + username + " [" + chat_id + "]:\n" + text, "");
        }

      }


      numNew = bot.getUpdates(bot.last_message_received + 1);
    }

    lastCheck = millis();
  }
}
