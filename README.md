# esp8266_telegram_to_screen_and_power_control

[![Phot of esp8266](https://github.com/user-attachments/assets/c3f9e46d-11a7-44ea-9648-0d138639ec5c)](#)

[Test | Протестировать](https://t.me/msg_to_screen_bot)

The `config.h` file must be filled in according to the `config.h.example` example
(файл `config.h` нужно заполнить по образцу `config.h.example`)
```
#pragma once

#define SCREEN_WIDTH      128
#define SCREEN_HEIGHT     64
#define OLED_SDA          12
#define OLED_SCL          14
#define WIFI_SSID         "MyHomeWiFi"
#define WIFI_PASS         "SuperSecret123"
#define BOT_TOKEN         "123456:ABC-DEF..."
#define ADMIN_CHAT_ID     "123"
#define BTN_IN_PC         5       // оптопара (нажатие кнопки питания ПК)
```

BTN_IN_PC is connected via an optocoupler to the computer's power button.
(BTN_IN_PC подключен через оптопару к кнопке питания компьютера.)
