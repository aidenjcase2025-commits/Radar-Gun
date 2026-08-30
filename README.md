Hardware Architecture Overview

HB100 Microwave Transceiver: Operates at 10.525 GHz, beaming out continuous microwave signals and capturing the reflected Doppler shift from moving targets.

LM324 Operational Amplifier: Utilizes a Texas Instruments LM324N (14-PDIP) quad op-amp in a dual-stage non-inverting layout to boost microvolt-level Intermediate Frequency (IF) signals.

2.5V Virtual Ground Biasing: Employs a voltage divider network using 10K resistors to establish a 2.5V DC center floor, preventing signal clipping on a single 5V power supply.

Active Filtering & Signal Processing

High-Pass Filtering: A 100nF ceramic capacitor (C1) blocks DC voltage from the sensor and strips away slow background movements and DC drift under 100 Hz.

AC-Coupling: Electrolytic capacitors (10uF) manage inter-stage coupling, blocking DC bias between op-amp stages while passing the AC Doppler wave.

Target Bandwidth: Designed to isolate frequencies between 1,000 Hz and 4,000 Hz, capturing the precise 31.36 Hz per 1 mph Doppler shift of a baseball.

