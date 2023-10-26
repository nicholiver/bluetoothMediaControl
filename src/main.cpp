#include <Arduino.h>
#include "OneButton.h"
#include <BleKeyboard.h>

#define threshold 75
touch_pad_t touchPin;

hw_timer_t * timer = NULL;

OneButton playPauseButton (2,false);
OneButton incNextButton (19,false);
OneButton decPrevButton (18,false);

BleKeyboard bleKeyboard("nicholiver_esp");

int previousTime = 0;
int currentTime = 0;


void playPause();
void stop();
void increaseVolume();
void nextTrack();
void decreaseVolume();
void previousTrack();


void setup() {
  Serial.begin(115200);
  delay(1000);
  playPauseButton.attachClick(playPause);
  playPauseButton.attachLongPressStart(stop);
  incNextButton.attachClick(increaseVolume);
  incNextButton.attachDoubleClick(nextTrack);
  decPrevButton.attachClick(decreaseVolume);
  decPrevButton.attachDoubleClick(previousTrack);
  Serial.println("ready");

  
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

}

void loop() {
  currentTime = millis();

  Serial.println(currentTime);
  playPauseButton.tick();
  incNextButton.tick();
  decPrevButton.tick();
  if(currentTime-previousTime > 10000){
    touchSleepWakeUpEnable(13, threshold);
    Serial.println("going to sleep...");
    esp_deep_sleep_start();
  }
}

void playPause() {
    if(bleKeyboard.isConnected()) {
      Serial.println("Sending Play/Pause media key...");
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
  }
  previousTime = currentTime;
}

void stop(){
  if(bleKeyboard.isConnected()){
  Serial.println("rejecting...");
  bleKeyboard.write(KEY_ESC);
  }
  previousTime = currentTime;
}

void increaseVolume() {
  if(bleKeyboard.isConnected()){
  Serial.println("increasing volume");
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  }
  previousTime = currentTime;
}

void nextTrack() {
  if(bleKeyboard.isConnected()){
  Serial.println("playing next track...");
  bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
  }
  previousTime = currentTime;
}

void decreaseVolume() {
  if(bleKeyboard.isConnected()){
  Serial.println("decreasing volume");
  bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
  }
  previousTime = currentTime;

}

void previousTrack() {
  if(bleKeyboard.isConnected()){
  Serial.println("playing previous track...");
  bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
  }
  previousTime = currentTime;
}
