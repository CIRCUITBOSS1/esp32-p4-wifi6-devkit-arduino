// 04 - Internet radio: Wi-Fi (C6) -> MP3 decode -> ES8311 codec -> speaker
// Libraries: arduino-audio-tools, arduino-audio-driver, arduino-libhelix
// Tools menu: PSRAM enabled, Flash 16MB, a big-app partition scheme
//   (the default partition gave "text section exceeds available space").
// STATUS: streamed and played on my board. Audio was harsh at volume 0.9
//   (clipping), so the default here is lower - tune it.

#include "AudioBoard.h"
#include "AudioTools.h"
#include "AudioTools/AudioLibs/I2SCodecStream.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/Communication/AudioHttp.h"      // URLStream lives here
#include <WiFi.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// Plain-HTTP MP3 streams work best (HTTPS and AAC need extra setup).
// Afrobeats options from the RCAST.NET directory (stations come and go):
const char* STREAM_URL = "http://kepler.shoutca.st:8201/stream";
// "http://ec2.yesstreaming.net:3130/stream"
// "http://mnlvmradio1.pro-fhi.net:2410/stream"

#define PIN_I2C_SDA   7
#define PIN_I2C_SCL   8
#define PIN_I2S_MCLK  13
#define PIN_I2S_BCLK  12
#define PIN_I2S_WS    10
#define PIN_I2S_DOUT  9
#define PIN_I2S_DIN   11
#define PIN_AMP_EN    53

DriverPins pins;
AudioBoard board(AudioDriverES8311, pins);
I2SCodecStream i2s(board);
URLStream url;                                        // uses existing Wi-Fi
EncodedAudioStream decoder(&i2s, new MP3DecoderHelix());
StreamCopy copier(decoder, url);

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setSleep(false);
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  pins.addI2C(PinFunction::CODEC, PIN_I2C_SCL, PIN_I2C_SDA);
  pins.addI2S(PinFunction::CODEC, PIN_I2S_MCLK, PIN_I2S_BCLK,
              PIN_I2S_WS, PIN_I2S_DOUT, PIN_I2S_DIN);
  pins.addPin(PinFunction::PA, PIN_AMP_EN, PinLogic::Output);

  auto cfg = i2s.defaultConfig(TX_MODE);
  cfg.sample_rate = 44100;
  cfg.channels = 2;
  cfg.bits_per_sample = 16;
  i2s.begin(cfg);
  i2s.setVolume(0.5);          // 0.9 clipped/distorted for me; tune this

  decoder.addNotifyAudioChange(i2s);   // name used by my library version
  decoder.begin();
  url.begin(STREAM_URL, "audio/mp3");  // try "audio/mpeg" if silent

  Serial.println("Streaming...");
}

void loop() {
  copier.copy();
}
