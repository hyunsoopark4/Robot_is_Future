# Robot_is_Future
# 🚜 ESP32 Mecanum Wheel Forklift
> **PS4 Controller-based Omnidirectional Forklift Robot**

<img width="427.2" height="427.4" alt="image" src="https://github.com/user-attachments/assets/1d3b0e9b-a108-4caf-898a-6af1b7d61178" />

## 📖 Overview
이 프로젝트는 **ESP32**와 **메카넘 휠(Mecanum Wheel)**을 활용한 무선 조종 지게차입니다. **PS4 컨트롤러(DualShock 4)**를 통해 블루투스로 제어하며, 전후좌우 이동뿐만 아니라 제자리 회전, 대각선 이동 등 옴니지향성 기동이 가능합니다. 

지게차의 리프트 기능은 **스텝모터(Stepper Motor)** 또는 서보모터를 사용하여 정밀하게 높이를 조절할 수 있도록 설계되었습니다.

## ✨ Key Features
* **🎮 PS4 컨트롤러 제어:** `Bluepad32` 라이브러리를 활용한 저지연 블루투스 연결
* **🔄 메카넘 휠 구동:** 4개의 TT 모터 독립 제어를 통한 전방향(Omnidirectional) 이동
* **🏗️ 리프트 메커니즘:** 스텝모터(28BYJ-48)를 활용한 포크 상승/하강 구현
* **🔋 전원 시스템:** 18650 배터리(2S) 및 TB6612FNG 모터 드라이버 사용
* **3️⃣ 3D Printed Chassis:** 직접 설계 및 3D 프린팅된 부품 사용 (`.3mf` 포함)

## 🛠️ Hardware List
| Component | Model / Note | Qty |
| :--- | :--- | :--- |
| **MCU** | ESP32 DevKit V1 | 1 |
| **Motor Driver** | TB6612FNG (Dual Motor Driver) | 2 |
| **Driving Motor** | Arduino TT Motor | 4 |
| **Lift Motor** | 28BYJ-48 Stepper Motor + ULN2003 Driver | 1 |
| **Wheels** | 60mm Mecanum Wheels | 4 |
| **Power** | 18650 Li-ion Battery (2S, 7.4V) | 2 |
| **Regulator** | LTC-3780 | 1 |
| **Controller** | PS4 DualShock 4 | 1 |

## 🔌 Pinout & Wiring
소스 코드(`ESP32_Mecanum_PS4.ino`)에 정의된 핀맵입니다.

### 🚗 Drive Motors (TB6612FNG)
| Motor Position | PWM Pin | IN1 | IN2 | Channel |
| :--- | :--- | :--- | :--- | :--- |
| **Right Front (RF)** | GPIO 32 | GPIO 25 | GPIO 33 | 0 |
| **Right Rear (RR)** | GPIO 13 | GPIO 27 | GPIO 14 | 1 |
| **Left Front (LF)** | GPIO 23 | GPIO 22 | GPIO 21 | 2 |
| **Left Rear (LR)** | GPIO 19 | GPIO 18 | GPIO 5  | 3 |
| **Standby (Common)** | GPIO 26 | - | - | - |

### 🏗️ Lift Motor (Stepper 28BYJ-48)
* **IN1:** GPIO 15
* **IN3:** GPIO 4
* **IN2:** GPIO 2
* **IN4:** GPIO 16
* *(Note: Stepper library uses IN1-IN3-IN2-IN4 sequence)*

## 🎮 Controls (PS4 Mapping)

| Button / Stick | Action | Description |
| :--- | :--- | :--- |
| **Left Stick (X/Y)** | Move | 전후좌우 및 대각선 이동 |
| **Right Stick (X)** | Rotate | 제자리 회전 (Turn Left/Right) |
| **Triangle (△)** | Lift UP | 포크 상승 |
| **Cross (X)** | Lift DOWN | 포크 하강 |

![Controller Mapping] <img width="945.5" height="634" alt="image" src="https://github.com/user-attachments/assets/3148e54c-8c0f-4ad1-a98e-7d30bb5ffc06" />

## 🖨️ 3D Models
이 프로젝트의 기구부는 3D 프린터로 제작되었습니다.
* **File Format:** `.3mf` (Bambu Lab / Prusa Slicer compatible)
* **Included Parts:** Chassis, Wheels mounts, Fork mechanism
> **Preview:**
<img width="1411" height="977" alt="image" src="https://github.com/user-attachments/assets/1a8492d7-8360-4f0a-9b6c-4185832efdc2" />

## 🚀 Upcoming Upgrade: N20 Motor Retrofit
현재 버전은 아두이노 TT 모터(노란색 기어드 모터)를 기반으로 제작되었으나, 더욱 정밀한 제어를 위해 **N20 메탈 기어 모터**로 업그레이드할 예정입니다.

### 🛑 Why Upgrade? (TT Motor Limitations)
* **불규칙한 데드존(Deadzone):** 저가형 TT 모터는 개체마다 기동 시작 PWM 값이 달라(60~90), 저속 주행 시 특정 바퀴만 회전하지 않는 문제가 발생합니다.
* **정밀 제어의 어려움:** 지게차 특성상 미세한 움직임이 필수적인데, TT 모터는 마찰력으로 인해 저속에서 부드러운 제어가 어렵습니다.

### 🛠️ Solution: N20 Motor & Custom Chassis
이를 해결하기 위해 다음과 같은 업그레이드를 진행하고 있습니다.
1.  **Motor Change:** **N20 Gear Motor (6V)**로 교체하여 메탈 기어의 우수한 내구성과 거의 없는 데드존(Deadzone)을 확보합니다.
2.  **Custom Modeling:** 기존 TT 모터용 섀시 대신, N20 모터 규격에 최적화된 **전용 프레임(Chassis)**을 직접 모델링하여 적용합니다.
3.  **Result:** 4개 바퀴의 완벽한 속도 동기화와 부드러운 옴니지향성 주행을 구현합니다.

*(N20 버전의 3D 모델링 파일과 코드는 추후 업데이트될 예정입니다.)*

## 💻 How to Install
1.  Install **Arduino IDE**.
2.  Install **ESP32 Board Manager** (`Espressif Systems`).
3.  Install Required Libraries:
    * [cite_start]`Bluepad32` by Ricardo Quesada [cite: 1]
    * [cite_start]`Stepper` (Built-in) [cite: 1]
4.  Open `ESP32_Mecanum_PS4.ino`.
5.  Select your ESP32 board and Upload.
6.  Open Serial Monitor (115200 baud).
7.  Press the **EN** button on ESP32 to reset and start Bluetooth pairing.
8.  Connect your PS4 Controller (Hold Share + PS button to pair).

