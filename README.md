# 📂 GoSung_ST

<br>

## 📖 소개 (Introduction)
STM32와 FreeRTOS 기반의 펌웨어부터 ROS2 센서퓨전, 모바일 앱까지, 실외 자율주행 배달 로봇의 핵심 시스템을 개발한 프로젝트입니다.<br>
주요 목표는 교내(인덕대학교) 실외 환경에서 로봇이 안정적으로 주행하고, 다양한 센서 데이터를 융합해 장애물 회피와 경로 생성을 수행할 수 있는 시스템을 구현하는 것입니다.<br>
이 저장소(Repository)는 전체 시스템 중 STM32 펌웨어, 즉 구동부와 센서부의 소스 코드를 담고 있습니다.<br><br>
⚠️ 참고: 본 프로젝트는 전문학사 기간 내 개발되었으며, 프로젝트 종료시점에 하드웨어 반납으로 추가 실험 및 장기간 데이터 수집이 불가능했습니다.<br>
본 프로젝트의 실험 결과는 당시 기록된 로그, 녹화 영상, 시뮬레이션을 기반으로 작성되었습니다.<br>

<br>

## ✨ 주요 기능 (Key Features)
### 제어 및 통신 (Control & Communication)
- FreeRTOS (CMSIS_V1)로 Task를 분리하여 실시간 시스템 구현
- UART/UART_IT 통신 기반 제어부(ROS2)와 데이터 및 명령어 통신 기능
- UART_IT 기반 Bluetooth 앱 원격 제어 기능

### 센서부 (Sensing)
- UART_DMA 기반 IMU/GPS 데이터 파싱 기능
  - IMU - roll, pitch, yaw, ax, ay, az, lx, ly, lz
  - GPS - GNRMC 포맷에서 위도(Latitude), 경도(Longitude) 추출

### 구동부 (Actuation)
- TIM_PWM 기반 BLDC 모터 드라이버 제어 및 양쪽 바퀴 속도 조절 기능
- GPIO_IT 기반 홀센서 속도 제어 및 경사로 정지 기능
- I2C 기반 초음파 기반 장애물 거리에 정지 기능

<br>

## 🛠️ 기술 스택 (Tech Stack)
- **MCU**: `STM32F407VET6`
- **Sensor**: `EBIMU(IMU)`, `BN880Q(GPS)`, `SRF08(Ultrasonic)`, `BLDC MOTOR`, `HC-06(Bluetooth)`, 
- **Language**: `C`
- **Framework/Library**: `STM32CubeIDE`, `HAL Library`, `FreeRTOS`
- **Tools**: `Git`, `GitHub`

<br>

## 👤 기여도 (My Contribution)
본 프로젝트의 펌웨어 파트는 모든 개발 과정을 직접 수행했습니다.
- FreeRTOS Task 구조 설계 및 전체 펌웨어 아키텍처 설계
- IMU/GPS 파싱 모듈 구현 및 센서 통합 코드 작성
- BLDC 제어 루프, 경사로 정지 로직, 초음파 기반 비상정지 기능 구현
- ROS2 <-> STM32 UART 통신 프로토콜 정의 및 구현
- Bluetooth 앱 제어 인터페이스 설계 및 펌웨어 연동

<br>

## 🚀 시작하기 (Getting Started) - 센서부
1. IDE: STM32CubeIDE 1.13.0 사용
2. Board: STM32F407VET6 개발보드 + ST-Link 디버거
3. Build & Flash: `main.c` 빌드 후 보드에 업로드 (코드 내용은 freertos.c에 작성)
4. UART1 - IMU, UART2 - GPS 연결
5. 터미널과 UART4 연결 후, IMU/GPS 데이터 출력 확인 (115200bps 고정)
6. 출력 예상
![Image](https://github.com/user-attachments/assets/271e6d6c-4d00-4eb1-a203-f17ad793658b)

<br>

## 📌 향후 개선 계획 (Future Work)
- 코드 리팩토링 및 모듈별 주석/README 강화
- ROS2 시뮬레이션과 연동하여 자율주행 시나리오 고도화

<br>


## 🔧 참고 자료

### 전체 회로도
<img width="1265" height="701" alt="Image" src="https://github.com/user-attachments/assets/d0e47ba4-425e-4193-8090-d9ac1f3d6b83" />

<br>

### YouTube - 프로젝트 최종 데모 영상
https://www.youtube.com/watch?v=ovQ-Pq1NfFU
