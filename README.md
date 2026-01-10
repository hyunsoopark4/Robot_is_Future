# Robot_is_Future
# 🚜 ESP32 Mecanum Wheel Forklift
> **PS4 Controller-based Omnidirectional Forklift Robot**

![Project Banner](https://via.placeholder.com/800x400?text=Please+Upload+Your+Robot+Photo+Here)
*(여기에 완성된 로봇 사진을 드래그 앤 드롭하여 넣으세요)*

## 📖 Overview
이 프로젝트는 **ESP32**와 **메카넘 휠(Mecanum Wheel)**을 활용한 무선 조종 지게차입니다. **PS4 컨트롤러(DualShock 4)**를 통해 블루투스로 제어하며, 전후좌우 이동뿐만 아니라 제자리 회전, 대각선 이동 등 옴니지향성 기동이 가능합니다. 

지게차의 리프트 기능은 **스텝모터(Stepper Motor)** 또는 서보모터를 사용하여 정밀하게 높이를 조절할 수 있도록 설계되었습니다.

## ✨ Key Features
* **🎮 PS4 컨트롤러 제어:** `Bluepad32` 라이브러리를 활용한 저지연 블루투스 연결
* **🔄 메카넘 휠 구동:** 4개의 N20 모터 독립 제어를 통한 전방향(Omnidirectional) 이동
* **🏗️ 리프트 메커니즘:** 스텝모터(28BYJ-48)를 활용한 포크 상승/하강 구현
* **🔋 전원 시스템:** 18650 배터리(2S) 및 TB6612FNG 모터 드라이버 사용
* **3️⃣ 3D Printed Chassis:** 직접 설계 및 3D 프린팅된 부품 사용 (`.3mf` 포함)

## 🛠️ Hardware List
| Component | Model / Note | Qty |
| :--- | :--- | :--- |
| **MCU** | ESP32 DevKit V1 | 1 |
| **Motor Driver** | TB6612FNG (Dual Motor Driver) | 2 |
| **Driving Motor** | N20 Gear Motor (6V, 100~150RPM recommended) | 4 |
| **Lift Motor** | 28BYJ-48 Stepper Motor + ULN2003 Driver | 1 |
| **Wheels** | 60mm Mecanum Wheels | 4 |
| **Power** | 18650 Li-ion Battery (2S, 7.4V) | 2 |
| **Regulator** | LM2596 (Buck Converter, 6V output) | 1 |
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

*(아래에 업로드해주신 컨트롤러 매핑 이미지를 넣어주세요)*
![Controller Mapping](https://via.placeholder.com/600x400?text=Upload+Controller+Image+Here)

## 🖨️ 3D Models
이 프로젝트의 기구부는 3D 프린터로 제작되었습니다.
* **File Format:** `.3mf` (Bambu Lab / Prusa Slicer compatible)
* **Included Parts:** Chassis, Wheels mounts, Fork mechanism
* 📂 [Download 3D Files](./Mecanum%20wheel%20forklinft%20for%20N20.3mf)

> **Preview:**
> (GitHub은 STL 파일 뷰어를 지원합니다. 3mf 파일을 STL로 변환하여 업로드하면 인터랙티브 뷰어를 사용할 수 있습니다. 혹은 슬라이서 스크린샷을 여기에 넣으세요.)

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

---
**Developed by [Hyunsoo Park](https://github.com/hyunsoopark4)**
*Make, Code, and Innovate.*
