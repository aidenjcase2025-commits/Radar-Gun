#include "arduinoFFT.h"

double vReal[128];
double vImag[128];
arduinoFFT FFT = arduinoFFT();

void setup() {
  Serial.begin(115200);
  Serial.println("Radar Gun Ready! Waiting for pitch...");
}

void loop() {
  double maxPitchSpeed = 0;
  unsigned long throwWindow = millis();

  // Listen aggressively for 2 straight seconds to catch the absolute peak speed
  while (millis() - throwWindow < 2000) {
    
    // --- THE STOPWATCH START ---
    unsigned long startTime = micros();
    
    // 1. DATA ACQUISITION
    for (int i = 0; i < 128; i++) {
      vReal[i] = analogRead(A0) - 512; 
      vImag[i] = 0;
    }

    // --- THE STOPWATCH END ---
    unsigned long endTime = micros();

    // Calculate the EXACT bin width dynamically based on processor speed
    double totalTimeSeconds = (endTime - startTime) / 1000000.0;
    double actualSampleRate = 128.0 / totalTimeSeconds;
    double actualBinWidth = actualSampleRate / 128.0; 

    // 2. FFT MATH
    FFT.Windowing(vReal, 128, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, 128, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, 128);

    // 3. FIND THE FASTEST TARGET (Jacob deGrom Mode)
    double peakMagnitude = 0;
    int peakBin = 0;
    
    // Scan BACKWARDS from the absolute fastest bin (63) down to the slowest (8)
    for (int i = 63; i >= 8; i--) {
      // The instant we find ANY signal taller than the 255 noise wall, lock it!
      if (vReal[i] > 370) {
        peakMagnitude = vReal[i];
        peakBin = i;
        break; // Stop searching! We found the fastest valid object and ignore the torso.
      }
    }

    // 4. CALCULATE CURRENT SPEED
    // Only calculate if we actually found something above the threshold
    if (peakMagnitude > 370) { 
      double peakFrequency = peakBin * actualBinWidth; 
      
      // Using your calibrated divisor
      double currentSpeed = peakFrequency / 30.7; 
      
      if (currentSpeed > maxPitchSpeed) {
        maxPitchSpeed = currentSpeed;
      }
    }
  }

  // 5. PRINT THE PEAK SPEED
  // Only print if we caught a legitimate target above 25 MPH
  if (maxPitchSpeed > 25) { 
    Serial.print("PEAK PITCH SPEED: ");
    Serial.print(maxPitchSpeed);
    Serial.println(" MPH");
    
    delay(3000); // 3-second human pause before it starts listening again
  }
}

