#include <AFMotor.h>

// ---- 연결 맵 ----
// M1: Front-Right (FR)
// M2: Front-Left  (FL)
// M3: Rear-Left   (RL)
// M4: Rear-Right  (RR)
AF_DCMotor M1(1), M2(2), M3(3), M4(4);

// 주행 기본 속도(0~255)
int BASE_SPEED = 200;

// 배선 극성 보정(필요시 -1로 바꾸세요)
int INV1 = 1;  // M1(앞오른쪽)
int INV2 = 1;  // M2(앞왼쪽)
int INV3 = 1;  // M3(뒤왼쪽)
int INV4 = 1;  // M4(뒤오른쪽)

// 내부 유틸: 모터에 -255~255 값으로 명령
void driveMotor(AF_DCMotor &m, int pwm, int inv = 1) {
  int v = pwm * inv;
  int spd = abs(v);
  if (spd > 255) spd = 255;

  if (v > 0) {
    m.setSpeed(spd);
    m.run(FORWARD);
  } else if (v < 0) {
    m.setSpeed(spd);
    m.run(BACKWARD);
  } else {
    m.run(RELEASE);
  }
}

// 네 바퀴 동시 명령 (FR, FL, RL, RR)
void drive4(int fr, int fl, int rl, int rr) {
  driveMotor(M1, fr, INV1);
  driveMotor(M2, fl, INV2);
  driveMotor(M3, rl, INV3);
  driveMotor(M4, rr, INV4);
}

// ---- 동작 프리셋 ----
// 표준 X형 메카넘(앞에서 봤을 때 롤러가 바깥쪽을 향함) 기준

void moveStop() {
  drive4(0, 0, 0, 0);
}

void moveForward(int spd = BASE_SPEED) {
  // 전진: 네 바퀴 모두 + (배선에 따라 INV로 보정)
  drive4(+spd, +spd, +spd, +spd);
}

void moveBackward(int spd = BASE_SPEED) {
  drive4(-spd, -spd, -spd, -spd);
}

void strafeRight(int spd = BASE_SPEED) {
  // 우측 평행이동: FL+, FR-, RL-, RR+
  drive4(-spd, +spd, -spd, +spd);
}

void strafeLeft(int spd = BASE_SPEED) {
  // 좌측 평행이동: FL-, FR+, RL+, RR-
  drive4(+spd, -spd, +spd, -spd);
}

// 대각선 이동(두 바퀴만 구동: 마찰 줄고 간결하게 이동)
void diagFrontRight(int spd = BASE_SPEED) {
  // FL+, RR+ 만 구동
  drive4(0, +spd, 0, +spd);
}

void diagFrontLeft(int spd = BASE_SPEED) {
  // FR+, RL+ 만 구동
  drive4(+spd, 0, +spd, 0);
}

void diagBackRight(int spd = BASE_SPEED) {
  // FL-, RR- 만 구동
  drive4(0, -spd, 0, -spd);
}

void diagBackLeft(int spd = BASE_SPEED) {
  // FR-, RL- 만 구동
  drive4(-spd, 0, -spd, 0);
}

void setup() {
  // 초기 속도 지정(원하면 바꾸세요)
  M1.setSpeed(BASE_SPEED);
  M2.setSpeed(BASE_SPEED);
  M3.setSpeed(BASE_SPEED);
  M4.setSpeed(BASE_SPEED);

  // 시작은 정지
  moveStop();
}

void loop() {
  // 데모 시퀀스: 각 동작 1초씩
  moveForward();     delay(1000);
  moveBackward();    delay(1000);
  strafeRight();     delay(1000);
  strafeLeft();      delay(1000);

  diagFrontRight();  delay(1000);
  diagFrontLeft();   delay(1000);
  diagBackRight();   delay(1000);
  diagBackLeft();    delay(1000);

  moveStop();        delay(1500);
}
