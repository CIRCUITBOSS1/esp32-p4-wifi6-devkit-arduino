// 03 - Play a 440 Hz tone through the onboard ES8311 codec + NS4150B amp
// Libraries: arduino-audio-tools, arduino-audio-driver
// STATUS: speaker played the tone on my board (first version was too quiet;
// amplitude and volume below are raised - tune them, headphones need less).

#include "AudioBoard.h"
#include "AudioTools.h"
#include "AudioTools/AudioLibs/I2SCodecStream.h"

// Pins from Waveshare's wiki/schematic
#define PIN_I2C_SDA   7
#define PIN_I2C_SCL   8
#define PIN_I2S_MCLK  13
#define PIN_I2S_BCLK  12
#define PIN_I2S_WS    10
#define PIN_I2S_DOUT  9     // P4 -> codec (playback)
#define PIN_I2S_DIN   11    // codec -> P4 (microphone)
#define PIN_AMP_EN    53    // amplifier enable, active high

AudioInfo info(44100, 2, 16);
SineWaveGenerator<int16_t> sineWave(16000);      // raise toward 24000 for louder
GeneratedSoundStream<int16_t> sound(sineWave);

DriverPins pins;
AudioBoard board(AudioDriverES8311, pins);
I2SCodecStream out(board);
StreamCopy copier(out, sound);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  pins.addI2C(PinFunction::CODEC, PIN_I2C_SCL, PIN_I2C_SDA);
  pins.addI2S(PinFunction::CODEC, PIN_I2S_MCLK, PIN_I2S_BCLK,
              PIN_I2S_WS, PIN_I2S_DOUT, PIN_I2S_DIN);
  pins.addPin(PinFunction::PA, PIN_AMP_EN, PinLogic::Output);

  auto cfg = out.defaultConfig(TX_MODE);
  cfg.copyFrom(info);
  if (!out.begin(cfg)) {
    Serial.println("Codec init failed");
    while (true) delay(1000);
  }
  out.setVolume(0.7);

  sineWave.begin(info, 440);
  Serial.println("Playing 440 Hz tone");
}

void loop() {
  copier.copy();
}
