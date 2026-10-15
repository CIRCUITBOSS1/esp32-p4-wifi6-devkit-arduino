# ESP32-P4-WIFI6-DEV-KIT with Arduino IDE

Working Arduino IDE examples for the **Waveshare ESP32-P4-WIFI6-DEV-KIT**: a Wi-Fi web server, audio playback through the onboard codec, and an internet radio.

Most public demos for this board are ESP-IDF. This repo records what actually worked in the **Arduino IDE**, including the errors I hit and how I fixed them.

> Video walkthrough: \*(add your YouTube link here)\*

## Why this repo exists

I bought this board and, for about four months, I couldn't really use it.

The ESP32-P4 is a powerful chip, but it has no Wi-Fi or Bluetooth radio of its own. This board gets wireless from a second chip, the ESP32-C6, which talks to the P4 over SDIO. So "just connect to Wi-Fi" depends on several things lining up at once: the Arduino core's support for the P4, the ESP-Hosted layer, the firmware running on the C6, and the right pins. When I first tried, that whole stack was too young for me to get working. It felt like I had all this power I couldn't use Most of what I could find was written for ESP-IDF or for other boards, and the Arduino IDE path felt like a gap nobody had filled yet.

That changed. With the platform further along, and some patient debugging help from Claude, I went from a board sitting in a drawer to a working web server, sound out of the speaker, and an internet radio playing Afrobeats, all in one sitting. **The moment I got my buzz sound tone from the speaker it was a joyous moment.**

I'm sharing everything, including the errors, because this is a young platform. Library names and APIs change quickly, and I lost time to things nobody had written down. If you also have this board and it's sitting unused, this repo is for you. If you get something working that isn't here, please open an issue or a pull request.

## The board

**Waveshare ESP32-P4-WIFI6-DEV-KIT** (Waveshare SKU 32054, my board is silkscreened Rev1.2).

* ESP32-P4 dual-core RISC-V, 32MB PSRAM in the chip package, 16MB NOR flash
* ESP32-C6 on board for 2.4 GHz Wi-Fi 6 and Bluetooth 5 / BLE, connected over SDIO (the P4 has no radio of its own)
* 100M Ethernet, USB-A ports, MIPI-CSI camera and MIPI-DSI display connectors, TF card slot
* ES8311 audio codec, onboard microphone, speaker header (8 ohm 2W), 3.5 mm headphone jack
* 2x20 GPIO header, RTC battery header, reserved PoE header

Waveshare sells it as a basic kit plus kits A, B and C (with camera and/or DSI display).

**Where to buy / docs**

* Product page and purchase: https://www.waveshare.com/esp32-p4-wifi6-dev-kit.htm
* Official wiki and downloads (schematic, demos): https://docs.waveshare.com/ESP32-P4-WIFI6-DEV-KIT

Prices change, so check the store for the current price. I am not affiliated with Waveshare.

## Examples

|#|Folder|What it does|Status|
|-|-|-|-|
|01|`01\_WebServer\_STA`|Web server on your home Wi-Fi|Worked|
|02|`02\_WebServer\_AP`|Web server on the P4's own Wi-Fi network (SoftAP)|Not tested yet|
|03|`03\_Audio\_Tone`|440 Hz tone through the speaker (ES8311 + amp)|Worked, tune volume|
|04|`04\_Internet\_Radio`|Streams an MP3 internet radio station|Worked, tune volume|

"Worked" means I ran it on my own board. Anything else is untested.

## Setup

1. **Arduino IDE 2.x** with the board package **esp32 by Espressif Systems** (Waveshare lists 3.2.0 or newer).
2. Board: **ESP32P4 Dev Module**.
3. Tools menu:

   * **PSRAM:** enabled
   * **Flash Size:** 16MB
   * **Partition Scheme:** one with about 3 MB for the app. The radio sketch is about 1.38 MB, and the default 1.25 MB app partition fails with "text section exceeds available space".
4. Libraries for the audio examples (03, 04), by Phil Schatzmann. Install from Library Manager or from the GitHub ZIP (Sketch > Include Library > Add .ZIP Library):

   * `arduino-audio-tools`
   * `arduino-audio-driver`
   * `arduino-libhelix`
5. Open a sketch folder, put your Wi-Fi name and password in the sketch, and upload.

**Never commit your real Wi-Fi password.** The sketches use placeholders.

### My versions (fill in)

* esp32 core: \_\_\_
* arduino-audio-tools: \_\_\_
* arduino-audio-driver: \_\_\_
* arduino-libhelix: \_\_\_
* P4 chip revision (`esptool chip\_id`): \_\_\_

## Pins (from the Waveshare schematic)

|Function|GPIO|
|-|-|
|I2C SDA / SCL (codec control, ES8311 at 0x18)|7 / 8|
|I2S MCLK|13|
|I2S BCLK (SCLK)|12|
|I2S WS (LRCK)|10|
|I2S DOUT (P4 to codec, playback)|9|
|I2S DIN (codec to P4, microphone)|11|
|Amplifier enable (active high)|53|
|C6 Wi-Fi link (SDIO, handled by the core)|CLK 18, CMD 19, D0 to D3 14 to 17, reset 54|

## Gotchas I hit

* **Wi-Fi is 2.4 GHz only** (the C6). Open the page with `http://`, not `https://`; some browsers rewrite a bare IP to HTTPS.
* **Slow first pings** on a phone hotspot (15 to 225 ms) improved with `WiFi.setSleep(false)`.
* **Boot log warnings** about the coprocessor firmware version request and the core dump size did not stop Wi-Fi from working for me.
* **"Sketch too big"** is fixed by 16MB flash plus a larger partition scheme.
* **Harsh, distorted radio audio** was clipping at volume 0.9. Start around 0.4 to 0.5.
* **Library names change between versions.** I needed `AudioTools/Communication/AudioHttp.h`, `AudioTools/AudioLibs/I2SCodecStream.h`, and `addNotifyAudioChange`. If something does not compile, check the examples shipped with your installed version.
* **Headphone jack mutes the speaker.** The schematic has an earphone-detect circuit that keeps the amp off while a plug is inserted.
* **Antenna:** the Rev1.2 schematic shows one RF path with two connectors and no RF switch chip, so I found no way to switch antennas in software.
* **Internet radio streams:** plain `http://` MP3 streams work best. AAC and HTTPS streams need extra setup. Stations come and go, so replace dead URLs.

## Ideas / TODO

* Test AP mode (02)
* Microphone echo (mic to speaker) on DIN = GPIO11
* Web page to change station and volume from a phone
* Camera, display, Ethernet
* Software echo cancellation (ESP-SR) for voice while music plays

## Credits

* Waveshare wiki and schematic: https://docs.waveshare.com/ESP32-P4-WIFI6-DEV-KIT
* Audio libraries by Phil Schatzmann (`arduino-audio-tools`, `arduino-audio-driver`, `arduino-libhelix`)
* Station list from the RCAST.NET directory
* Drafted with help from Claude; only the examples marked "Worked" were confirmed on hardware.

## License

MIT, see `LICENSE`.

