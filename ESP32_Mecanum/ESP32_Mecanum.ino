/*
 * ESP32-S3 DevKitC-1 및 TB6612 2개용 메카넘 휠 제어 코드
 * * 제공된 Mecanum_wheel.ino 파일의 로직(전진/정지)을 기반으로 재작성되었습니다.
 * 사용자가 지정한 새 핀 매핑을 적용했습니다.
 */

// 전진 시 속도 (0~255)
int speedValue = 200;

// -----------------------------------------------------------------
// 1. 핀 정의 (사용자 지정 매핑 적용)
// -----------------------------------------------------------------

// 공통 Standby 핀
const int STBY = 4; // 사용자가 GPIO 4로 지정

// 모터 1: Front Left (FL) - Driver 1, Channel A
const int PWMA_PIN = 1;  // (구 GPIO 10 -> 검은색 핀 4)
const int AIN1 = 2;  // (구 GPIO 11 -> 검은색 핀 5)
const int AIN2 = 42; // (구 GPIO 12 -> 검은색 핀 6)

// 모터 2: Rear Left (RL) - Driver 1, Channel B
const int PWMB_PIN = 41; // (구 GPIO 13 -> 검은색 핀 7)
const int BIN1 = 40; // (구 GPIO 14 -> 검은색 핀 8)
const int BIN2 = 39; // (구 GPIO 15 -> 검은색 핀 9)

// 모터 3: Front Right (FR) - Driver 2, Channel A
const int PWMC_PIN = 38; // (구 GPIO 16 -> 검은색 핀 10)
const int CIN1 = 37; // (구 GPIO 17 -> 검은색 핀 11)
const int CIN2 = 36; // (구 GPIO 18 -> 검은색 핀 12)

// 모터 4: Rear Right (RR) - Driver 2, Channel B
const int PWMD_PIN = 35; // (구 GPIO 19 -> 검은색 핀 13)
// 🚨 위험! GPIO 0은 부팅 핀입니다. 다른 핀으로 변경하는 것을 강력히 권장합니다.
const int DIN1 = 0;  // (구 GPIO 38 -> 검은색 핀 14)
const int DIN2 = 45; // (구 GPIO 39 -> 검은색 핀 15)

// -----------------------------------------------------------------
// 2. ESP32 LEDC (PWM) 설정
// -----------------------------------------------------------------
const int PWM_FREQ = 5000;    // 5kHz PWM 주파수
const int PWM_RESOLUTION = 8; // 8비트 해상도 (0-255)

// 각 모터에 대한 PWM 채널 할당 (0~7)
const int PWM_CHAN_FL = 0; // Front Left
const int PWM_CHAN_RL = 1; // Rear Left
const int PWM_CHAN_FR = 2; // Front Right
const int PWM_CHAN_RR = 3; // Rear Right


void setup() {
  // [cite: 3] 핀 모드 설정 (OUTPUT)
  pinMode(STBY, OUTPUT);
  
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(CIN1, OUTPUT);
  pinMode(CIN2, OUTPUT);
  pinMode(DIN1, OUTPUT);
  pinMode(DIN2, OUTPUT);
  
  // PWM 핀들은 ledcAttachPin이 자동으로 OUTPUT 설정함 (pinMode 불필요)

  // ESP32 LEDC(PWM) 시스템 설정
  ledcSetup(PWM_CHAN_FL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHAN_RL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHAN_FR, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHAN_RR, PWM_FREQ, PWM_RESOLUTION);

  // PWM 핀을 LEDC 채널에 할당
  ledcAttachPin(PWMA_PIN, PWM_CHAN_FL);
  ledcAttachPin(PWMB_PIN, PWM_CHAN_RL);
  ledcAttachPin(PWMC_PIN, PWM_CHAN_FR);
  ledcAttachPin(PWMD_PIN, PWM_CHAN_RR);

  // 모터 드라이버 활성화
  digitalWrite(STBY, HIGH);
  
  // [cite: 2] (AFMotor 라이브러리 부분을 제외하고 setup 로직 비어있음)
}

void loop() {
  // [cite: 4] 1️⃣ 전진 (3초)
  forward();
  delay(3000);

  // [cite: 4] 2️⃣ 정지 (3초)
  stopMotors();
  delay(3000);
}

// [cite: 5] 전진 함수 (TB6612 로직 적용)
void forward() {
  // 메카넘휠 '전진'은 4개 모터가 모두 같은 방향으로 회전합니다.
  // (만약 반대로 도는 모터가 있다면 해당 모터의 IN1, IN2를 교체)
  
  // FL: 정방향
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWM_CHAN_FL, speedValue);

  // RL: 정방향
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  ledcWrite(PWM_CHAN_RL, speedValue);

  // FR: 정방향
  digitalWrite(CIN1, HIGH);
  digitalWrite(CIN2, LOW);
  ledcWrite(PWM_CHAN_FR, speedValue);

  // RR: 정방향
  digitalWrite(DIN1, HIGH);
  digitalWrite(DIN2, LOW);
  ledcWrite(PWM_CHAN_RR, speedValue);
}

// [cite: 6] 정지 함수
void stopMotors() {
  // PWM을 0으로 설정하여 모터 정지 (Coast)
  ledcWrite(PWM_CHAN_FL, 0);
  ledcWrite(PWM_CHAN_RL, 0);
  ledcWrite(PWM_CHAN_FR, 0);
  ledcWrite(PWM_CHAN_RR, 0);
}