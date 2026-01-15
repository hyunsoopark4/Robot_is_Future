#include <Bluepad32.h>
#include <Stepper.h>             // 스텝모터 라이브러리 추가
#include "soc/soc.h"             
#include "soc/rtc_cntl_reg.h"    

// ================================================================
// 1. 메카넘휠 핀 정의 (기존 유지)
// ================================================================
const int STBY_PIN = 26;
// 오른쪽
const int RF_PWM_PIN = 32; const int RF_IN1_PIN = 25; const int RF_IN2_PIN = 33;
const int RR_PWM_PIN = 13; const int RR_IN1_PIN = 27; const int RR_IN2_PIN = 14;
// 왼쪽
const int LF_PWM_PIN = 23; const int LF_IN1_PIN = 22; const int LF_IN2_PIN = 21;
const int LR_PWM_PIN = 19; const int LR_IN1_PIN = 18; const int LR_IN2_PIN = 5;

// ================================================================
// 2. 스텝모터 핀 정의 (새로 추가!)
// ================================================================
// 28BYJ-48 모터는 1바퀴에 2048 스텝
const int STEPS_PER_REV = 2048; 

// 남는 핀 할당 (IN1, IN3, IN2, IN4 순서 중요!)
// ULN2003 드라이버에 연결할 핀들: 15, 2, 4, 16
Stepper forkStepper(STEPS_PER_REV, 15, 2, 4, 16); 

// ================================================================
// 3. 설정 변수
// ================================================================
const int PWM_FREQ = 5000;
const int PWM_RES = 8;
const int CH_RF = 0; const int CH_RR = 1; 
const int CH_LF = 2; const int CH_LR = 3; 
const int DEADZONE = 30;

unsigned long lastDebugTime = 0;
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// ================================================================
// 4. 모터 구동 함수들
// ================================================================
void driveMotor(int channel, int in1, int in2, int speed) {
  speed = constrain(speed, -255, 255);
  if (speed > 0) {
    digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
    ledcWrite(channel, speed);
  } else if (speed < 0) {
    digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
    ledcWrite(channel, abs(speed));
  } else {
    digitalWrite(in1, LOW); digitalWrite(in2, LOW);
    ledcWrite(channel, 0);
  }
}

// 지게차 포크 제어 함수 (스텝모터)
void processForkLift(ControllerPtr ctl) {
  // 스텝모터 속도 설정 (RPM). 너무 빠르면 탈조나서 안 돔. 10~15 추천.
  forkStepper.setSpeed(10); 

  // 세모(Triangle) 버튼: 0x0008 -> 올리기
  if (ctl->buttons() & 0x0008) {
    // 루프가 도는 동안 조금씩 움직임 (블로킹 방지를 위해 작은 값)
    forkStepper.step(50); 
    if (millis() - lastDebugTime > 500) Serial.println(">> 포크 상승 중 (UP)");
  }
  
  // 엑스(Cross) 버튼: 0x0001 -> 내리기
  if (ctl->buttons() & 0x0001) {
    forkStepper.step(-50); // 반대 방향
    if (millis() - lastDebugTime > 500) Serial.println(">> 포크 하강 중 (DOWN)");
  }
  
  // 버튼 안 누르면? step() 함수 호출 안 하니까 자동으로 멈춰있음 (Hold 토크 발생)
  // 단, 스텝모터 드라이버는 전기를 계속 먹고 있음.
}

void processMecanumMovement(ControllerPtr ctl) {
  int lx = ctl->axisX();
  int ly = ctl->axisY();
  int rx = ctl->axisRX(); 

  if (abs(lx) < DEADZONE) lx = 0;
  if (abs(ly) < DEADZONE) ly = 0;
  if (abs(rx) < DEADZONE) rx = 0;

  int y = map(ly, -512, 512, 255, -255); 
  int x = map(lx, -512, 512, -255, 255); 
  int turn = map(rx, -512, 512, -255, 255);

  int fl = y + x + turn;
  int fr = y - x - turn;
  int rl = y - x + turn;
  int rr = y + x - turn;

  driveMotor(CH_LF, LF_IN1_PIN, LF_IN2_PIN, fl);
  driveMotor(CH_RF, RF_IN1_PIN, RF_IN2_PIN, fr);
  driveMotor(CH_LR, LR_IN1_PIN, LR_IN2_PIN, rl);
  driveMotor(CH_RR, RR_IN1_PIN, RR_IN2_PIN, rr);
}

// ================================================================
// 5. Bluepad32 콜백
// ================================================================
void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller connected, index=%d\n", i);
      myControllers[i] = ctl;
      return;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      driveMotor(CH_RF, RF_IN1_PIN, RF_IN2_PIN, 0);
      driveMotor(CH_RR, RR_IN1_PIN, RR_IN2_PIN, 0);
      driveMotor(CH_LF, LF_IN1_PIN, LF_IN2_PIN, 0);
      driveMotor(CH_LR, LR_IN1_PIN, LR_IN2_PIN, 0);
      return;
    }
  }
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
        processMecanumMovement(myController); // 바퀴 제어
        processForkLift(myController);        // 지게차 포크 제어
      }
    }
  }
}

// ================================================================
// 6. Setup & Loop
// ================================================================
void setup() {
  Serial.begin(115200);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  pinMode(STBY_PIN, OUTPUT);
  // 메카넘 핀 설정
  pinMode(RF_IN1_PIN, OUTPUT); pinMode(RF_IN2_PIN, OUTPUT);
  pinMode(RR_IN1_PIN, OUTPUT); pinMode(RR_IN2_PIN, OUTPUT);
  pinMode(LF_IN1_PIN, OUTPUT); pinMode(LF_IN2_PIN, OUTPUT);
  pinMode(LR_IN1_PIN, OUTPUT); pinMode(LR_IN2_PIN, OUTPUT);

  // 메카넘 PWM 설정
  ledcSetup(CH_RF, PWM_FREQ, PWM_RES);
  ledcSetup(CH_RR, PWM_FREQ, PWM_RES);
  ledcSetup(CH_LF, PWM_FREQ, PWM_RES);
  ledcSetup(CH_LR, PWM_FREQ, PWM_RES);
  ledcAttachPin(RF_PWM_PIN, CH_RF);
  ledcAttachPin(RR_PWM_PIN, CH_RR);
  ledcAttachPin(LF_PWM_PIN, CH_LF);
  ledcAttachPin(LR_PWM_PIN, CH_LR);

  digitalWrite(STBY_PIN, HIGH);

  // 스텝모터는 Stepper 라이브러리가 알아서 핀 설정 해줌. 
  // 단, 충돌 방지를 위해 명시적으로 초기화해주고 싶으면 아래 주석 해제
  /*
  pinMode(15, OUTPUT); pinMode(4, OUTPUT); 
  pinMode(2, OUTPUT); pinMode(16, OUTPUT);
  */

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
  
  Serial.println("Ready! Mecanum + Forklift Mode");
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated) processControllers();
  // 스텝모터가 부드럽게 돌려면 delay가 너무 길면 안됨. 짧게 조정.
  delay(1); 
}
