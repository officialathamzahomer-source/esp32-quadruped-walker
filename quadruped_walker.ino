#include <ESP32Servo.h>

// Servo pin assignments
#define PIN_FL_HIP   13
#define PIN_FL_KNEE  12

#define PIN_FR_HIP   14
#define PIN_FR_KNEE  27

#define PIN_RL_HIP   26
#define PIN_RL_KNEE  25

#define PIN_RR_HIP   33
#define PIN_RR_KNEE  32

// Servo objects
Servo fl_hip, fl_knee;
Servo fr_hip, fr_knee;
Servo rl_hip, rl_knee;
Servo rr_hip, rr_knee;

// Neutral / Stand angles (Adjust these based on horn mounting calibration)
const int HIP_CENTER  = 90;
const int KNEE_STAND  = 90;

// Gait offsets
const int HIP_SWING   = 25; // Forward/backward swing amplitude
const int KNEE_LIFT   = 35; // Ground clearance lift amplitude
const int STEP_DELAY  = 120; // Speed of motion in ms

void setup() {
  Serial.begin(115200);

  // Allocate timers for ESP32 PWM
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // 50Hz standard servo frequency
  fl_hip.setPeriodHertz(50);
  fl_knee.setPeriodHertz(50);
  fr_hip.setPeriodHertz(50);
  fr_knee.setPeriodHertz(50);
  rl_hip.setPeriodHertz(50);
  rl_knee.setPeriodHertz(50);
  rr_hip.setPeriodHertz(50);
  rr_knee.setPeriodHertz(50);

  // Attach servos (standard pulse width 500us - 2400us for SG90/MG90S)
  fl_hip.attach(PIN_FL_HIP, 500, 2400);
  fl_knee.attach(PIN_FL_KNEE, 500, 2400);

  fr_hip.attach(PIN_FR_HIP, 500, 2400);
  fr_knee.attach(PIN_FR_KNEE, 500, 2400);

  rl_hip.attach(PIN_RL_HIP, 500, 2400);
  rl_knee.attach(PIN_RL_KNEE, 500, 2400);

  rr_hip.attach(PIN_RR_HIP, 500, 2400);
  rr_knee.attach(PIN_RR_KNEE, 500, 2400);

  stand();
  delay(2000);
}

void loop() {
  walkForward();
}

// Set all servos to a default standing posture
void stand() {
  fl_hip.write(HIP_CENTER);
  fl_knee.write(KNEE_STAND);

  fr_hip.write(HIP_CENTER);
  fr_knee.write(KNEE_STAND);

  rl_hip.write(HIP_CENTER);
  rl_knee.write(KNEE_STAND);

  rr_hip.write(HIP_CENTER);
  rr_knee.write(KNEE_STAND);
}

// 4-phase diagonal crawl gait
void walkForward() {
  // --- Phase 1: Lift & swing FL and RR forward ---
  fl_knee.write(KNEE_STAND - KNEE_LIFT);
  rr_knee.write(KNEE_STAND - KNEE_LIFT);
  delay(STEP_DELAY);

  fl_hip.write(HIP_CENTER + HIP_SWING);
  rr_hip.write(HIP_CENTER - HIP_SWING);
  delay(STEP_DELAY);

  // Lower FL and RR to ground
  fl_knee.write(KNEE_STAND);
  rr_knee.write(KNEE_STAND);
  delay(STEP_DELAY);

  // --- Phase 2: Shift body forward (stance phase) ---
  fl_hip.write(HIP_CENTER);
  rr_hip.write(HIP_CENTER);
  fr_hip.write(HIP_CENTER + HIP_SWING);
  rl_hip.write(HIP_CENTER - HIP_SWING);
  delay(STEP_DELAY);

  // --- Phase 3: Lift & swing FR and RL forward ---
  fr_knee.write(KNEE_STAND - KNEE_LIFT);
  rl_knee.write(KNEE_STAND - KNEE_LIFT);
  delay(STEP_DELAY);

  fr_hip.write(HIP_CENTER - HIP_SWING);
  rl_hip.write(HIP_CENTER + HIP_SWING);
  delay(STEP_DELAY);

  // Lower FR and RL to ground
  fr_knee.write(KNEE_STAND);
  rl_knee.write(KNEE_STAND);
  delay(STEP_DELAY);

  // --- Phase 4: Shift body forward (stance phase) ---
  fr_hip.write(HIP_CENTER);
  rl_hip.write(HIP_CENTER);
  fl_hip.write(HIP_CENTER - HIP_SWING);
  rr_hip.write(HIP_CENTER + HIP_SWING);
  delay(STEP_DELAY);
}
