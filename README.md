# Linux Process Monitor

리눅스 터미널에서 **프로세스 상태를 실시간으로 모니터링**하고,  **프로세스 제어(Kill / Nice 변경)** 까지 수행할 수 있는 **C/C++ 기반 TUI(Terminal UI)** 프로그램입니다.

> **Appeal Point**  
> - 

---

## Problem (문제 정의)
> **왜 이 프로젝트를 시작했는가? 어떤 기술적 호기심/문제를 해결하려 했는가?**  

- `top/htop` 수준의 “보이는 결과”가 아니라, **/proc을 직접 파싱하고 OS 관점에서 데이터가 어떻게 만들어지는지** 이해
- **signal, nice, kill 같은 프로세스 제어 기능을 실제 제품 수준의 UX(TUI)로 묶는 경험**

---

## Demo (예시)
- 상단: CPU / Memory 요약
- 하단: 프로세스 리스트(정렬/선택) + 상세 정보
- 단축키로 Kill / Nice 변경

*(스크린샷이나 gif `/docs/`에 추가 예정)*

---

## Features

### 1) Data Gathering 

### 2) TUI Development

### 3) Process Control

### 4) Signal 기반 IPC (희망)
- **Signal 전송 기반 IPC** 구성  
  - 

---

## Troubleshooting (분석 과정)
> 개발하면서 마주친 장애/버그를 “증상 → 재현 → 원인 규명 → 해결 → 검증” 순서로 기록합니다.  
> 아래 항목은 진행하면서 발생한 **실제 로그/명령/스택트레이스/커밋 링크**입니다.

### Case 1. Segmentation Fault
- **증상(Symptom)**  
- **재현(Reproduction)**  
- **분석(Investigation)**
- **원인(Root Cause)** 
- **해결(Fix)** 
- **검증(Validation)**  

---

## Optimization (성능 최적화)

### Memory Leak / Resource Leak

### Latency / Refresh Cost

---

## Environment (개발 환경)
> 재현 가능한 포트폴리오를 위해 개발/테스트 환경을 명시합니다.

- Distro: **Ubuntu 24.04.3 LTS**
- Kernel: **Linux 6.8.0-90-generic**
- Compiler: gcc 13.3.0

---

## Build & Run

### Dependencies

### Build
```bash
