#pragma once

void restart(int);

void restart(int delay_seconds) {
  if (!DEBUG_MODE) {
    delay(delay_seconds * 1000);
    ESP.restart();
  }
}
