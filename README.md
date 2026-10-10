# ESP-Drone: Max Imagination Workshop (Mumbai)

ESP32-S3 drone firmware, ported to **ESP-IDF v6.1**.

## Quick start: flash and fly
1. **Flash** the prebuilt firmware. See [`firmware/README.md`](firmware/README.md). You can flash from a browser with nothing installed.
2. **Power on** the drone on a flat surface and keep it still while it calibrates.
3. **Connect** your phone to Wi-Fi `ESP-DRONE_XXXXXXXXXXXX` with password `12345678`.
4. **Fly** with the ESP-Drone app ([Android](https://github.com/EspressifApps/ESP-Drone-Android) / [iOS](https://github.com/EspressifApps/ESP-Drone-iOS)). Raise the throttle gently.

## Build from source
Requires [ESP-IDF v6.1](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/).
```bash
git clone https://github.com/harshith-cmd/esp-drone-max-imagination.git
cd esp-drone-max-imagination
idf.py build
idf.py -p PORT flash monitor
```
`sdkconfig` already holds the board configuration (target esp32s3, pins, motors), so you don't need to run `set-target`.

**Pins:** motors M1-M4 on GPIO 10, 3, 2 and 1 · LEDs on GPIO 4 (blue), 5 (green) and 6 (red) · IMU on I2C0 (SDA 8, SCL 9) · battery ADC on GPIO 7.

The Wi-Fi name and password are set in `components/drivers/general/wifi/wifi_esp32.c`.

### Changes from upstream (IDF v5.0 to v6.1)
- Rewrote the battery ADC driver for `esp_adc/adc_oneshot`, because the legacy `driver/adc.h` was removed in v6.
- Added the `esp_driver_*` component dependencies that v6 needs after splitting the old `driver` component.
- Changed `ESP_IF_WIFI_AP` to `WIFI_IF_AP`, and fixed a GCC 15 build error.

---


## ESP-Drone

* [中文](./README_cn.md)

### Introduction

**ESP-Drone** is an open source solution based on Espressif ESP32/ESP32-S2/ESP32-S3 Wi-Fi chip, which can be controlled by a mobile APP or gamepad over **Wi-Fi** connection. ESP-Drone comes with **simple hardware**, **clear and extensible code architecture**, and therefore this project can be used in **STEAM education** and other fields. The main code is ported from **Crazyflie** open source project with **GPL3.0** protocol.

> Currently support ESP32、ESP32S2、ESP32S3, please using ESP-IDF [release/v4.4](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32s2/get-started/index.html) branch as your develop environment

![ESP-Drone](./docs/_static/espdrone_s2_v1_2_2.png)

For more information, please check the sections below:
* **Getting Started**: [Getting Started](https://docs.espressif.com/projects/espressif-esp-drone/zh_CN/latest/gettingstarted.html)
* **Hardware Schematic**：[Hardware](https://docs.espressif.com/projects/espressif-esp-drone/zh_CN/latest/_static/ESP32_S2_Drone_V1_2/SCH_Mainboard_ESP32_S2_Drone_V1_2.pdf)
* **iOS APP Source code**: [ESP-Drone-iOS](https://github.com/EspressifApps/ESP-Drone-iOS)
* **Android APP Source code**: [ESP-Drone-Android](https://github.com/EspressifApps/ESP-Drone-Android)

### Features

1. Stabilize Mode
2. Height-hold Mode
3. Position-hold Mode
4. APP Control
5. CFclient Supported

Note: to implement Height-hold/Position-hold mode, extension boards are needed. For more information, see Hardware Reference. 

### Third Party Copyrighted Code

Additional third party copyrighted code is included under the following licenses.

| Component | License | Origin |Commit ID |
| :---:  | :---: | :---: |:---: |
| core/crazyflie | GPL3.0  |[Crazyflie](https://github.com/bitcraze/crazyflie-firmware) |tag_2021_01 b448553|
| lib/dsp_lib |  | [esp32-lin](https://github.com/whyengineer/esp32-lin/tree/master/components/dsp_lib) |6fa39f4c|

### Support Policy

From December 2022, we will offer limited support on this project, but Pull Request is still welcomed!

### THANKS

1. Thanks to Bitcraze for the great [Crazyflie project](https://www.bitcraze.io/%20).
2. Thanks to Espressif for the powerful [ESP-IDF framework](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html).
3. Thanks to WhyEngineer for the useful [ESP-DSP lib](https://github.com/whyengineer/esp32-lin/tree/master/components/dsp_lib).

