# Prebuilt firmware (ESP32-S3, ESP-IDF v6.1)

## Option 1: Browser, nothing to install
1. Open https://espressif.github.io/esptool-js/ in Chrome or Edge.
2. Plug in the drone over USB, set the baud rate to 460800 and click **Connect**. Pick the drone's serial port.
3. Set Flash Address to `0x0` and choose `esp-drone-esp32s3-merged.bin`.
4. Click **Program**. When it finishes, press the RESET button or unplug and replug the drone.

## Option 2: Command line with esptool
```bash
pip install esptool
cd firmware
esptool.py --chip esp32s3 -p PORT -b 460800 write_flash 0x0 esp-drone-esp32s3-merged.bin
```
`PORT` is `/dev/ttyACM0` or `/dev/ttyUSB0` on Linux, `/dev/cu.usbmodem*` on macOS, or `COM3` (or similar) on Windows.

You can also flash the separate images:
```bash
esptool.py --chip esp32s3 -p PORT -b 460800 write_flash --flash_mode dio --flash_freq 80m --flash_size 2MB \
  0x0 bootloader.bin 0x8000 partition-table.bin 0x10000 ESPDrone.bin
```

If it can't connect, hold **BOOT** while you plug in the USB cable, then try again.
