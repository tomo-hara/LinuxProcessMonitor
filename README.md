# MBTop (Linux Process Monitor)

리눅스 터미널에서 **/proc 파일 시스템을 직접 파싱**하여 시스템/프로세스 상태를 **1초 주기로 갱신 표시**하는 초경량 TUI 모니터입니다.

- 목표: `top/htop` 복제가 아니라, **CPU/메모리/프로세스 정보가 어떤 근거로 계산되는지**를 코드로 확인
- (예정) 프로세스 제어: `kill(2)`, `setpriority(2)`(nice) 등

---

## Status (진행 상태)

### Implemented ✅
- `/proc/uptime` 기반 Uptime 표시
- `/proc/loadavg` 기반 Load Average 표시
- `/proc/stat` delta 기반 CPU 사용률 계산 + Bar
- `/proc/meminfo`(MemTotal/MemAvailable) 기반 메모리 사용률 + MB
- `/proc/[pid]/stat` 파싱: PID/PPID/STATE/NAME 출력 (최대 15개)

### In progress / Planned 🛠️
- TUI 인터랙션(키 입력, 선택/스크롤, 정렬/필터)
- 프로세스 상세 보기(`/proc/[pid]/status`, `cmdline`)
- Process Control: `kill(2)` / `setpriority(2)`(nice)
- 화면 갱신 최적화(부분 갱신, flicker 최소화)
- (선택) Signal 기반 IPC 실험

---

## Demo (예시)

```
=== MBTop: Linux Process Monitor (Press Ctrl+C to quit) ===

[System Uptime] 12:34:56
[System Load] 1min: 0.12 | 5min: 0.08 | 15min: 0.05
CPU Usage :  23.4% [####                ]
Mem Usage :  61.0% [############        ] (3120/5120 MB)

[ Process List (Top 15) ]
PID      PPID   STATE  NAME
----------------------------------------
1        0      S      systemd
...
```

> TODO: `/docs/`에 스크린샷 또는 gif 추가

---

## Quick Start

### Requirements
- Linux (`/proc` 사용)
- `gcc`, `make` (외부 라이브러리 필요 없음)

### Build
```bash
make
```

### Run
```bash
./mb_top.out
```

### Clean
```bash
make clean
```

---

## Usage (조작)

- 현재 버전:
  - 종료: `Ctrl + C`

- (예정)
  - 방향키/단축키로 프로세스 선택/스크롤
  - `k`로 kill, `n`으로 nice 조정 등

---

## How it works (Implementation Notes)

### Main loop 개요
1) `/proc`에서 필요한 값 읽기  
2) CPU tick은 **이전 값과 delta**로 사용률 계산  
3) 화면 clear → 렌더링 → sleep(1s) → 반복

### CPU Usage 계산 (/proc/stat)
1. `/proc/stat` 첫 줄의 `cpu` 항목을 읽어 tick 합계를 구합니다.
2. 이전 tick 합과 현재 tick 합의 차이(`total_diff`), idle 차이(`idle_diff`)를 구합니다.
3. 사용률은 다음으로 계산합니다.

```
cpu_usage = (total_diff - idle_diff) / total_diff * 100
```

> 구현 위치: `mb_top.c` 의 `get_cpu_stats()` 및 main loop 계산부

### Memory Usage (/proc/meminfo)
- `MemTotal`, `MemAvailable`을 읽고 `used = total - available` 로 계산합니다.
- 출력은 MB 단위로 변환해 보여줍니다.

> 구현 위치: `get_memory_stats()`

### Process List (/proc/[pid]/stat)
- `/proc`에서 이름이 숫자인 항목만 PID로 간주
- `/proc/[pid]/stat`에서 `pid`, `comm(name)`, `state`, `ppid` 파싱
- 화면 길이 제한을 위해 최대 15개 출력

> 구현 위치: `print_procs()`

---

## TUI (현재 방식 / 개선 방향)

- 현재(가정): ANSI escape 기반 화면 clear/커서 제어로 갱신
- 개선:
  - flicker 줄이기(부분 갱신 / 더블 버퍼링 유사 접근)
  - 필요 시 `ncurses` 도입 여부 검토

---

## Process Control (권한/안전 메모)

- `kill(2)`:
  - 일반적으로 **같은 사용자(UID)의 프로세스**만 종료 가능 (권한 없으면 실패)
- `setpriority(2)`:
  - nice 값을 낮추는(우선순위 올리는) 동작은 권한이 필요할 수 있음

> 구현 시에는 “실패 케이스(EPERM 등)”를 UX로 안내하는 것을 목표로 합니다.

---

## Troubleshooting / Known Issues

### Case 0. Ctrl+C 종료 시 커서가 숨겨진 채 남을 수 있음
무한 루프를 `Ctrl+C(SIGINT)`로 종료하면 `cleanup_screen()`이 호출되지 않아 커서 숨김 상태가 유지될 수 있습니다.

- 임시 복구:
  ```bash
  tput cnorm
  # 또는
  printf '\033[?25h'
  # 또는
  reset
  ```
- 근본 해결(예정):
  - `SIGINT` 핸들러에서 cleanup 수행
  - `atexit()`로 종료 경로 보장

### Case 1. Segmentation Fault (템플릿)
- 증상(Symptom):
- 재현(Reproduction):
- 분석(Investigation):
- 원인(Root Cause):
- 해결(Fix):
- 검증(Validation):

---

## Project Structure

```
.
├── mb_top.c      # /proc 파싱 + 화면 갱신 로직
├── Makefile      # 빌드 스크립트 (TARGET=mb_top.out)
└── README.md
```

---

## Environment (개발 환경)

- Distro: Ubuntu 24.04.3 LTS
- Kernel: Linux 6.8.0-90-generic
- Compiler: gcc 13.3.0

---

## Roadmap

- [ ] PID/CPU/MEM 기준 정렬 & 필터링
- [ ] 키 입력 기반 TUI 인터랙션(선택/스크롤)
- [ ] 선택 프로세스 상세(`/proc/[pid]/status`, `cmdline`)
- [ ] Process Control: `kill(2)` / `setpriority(2)`(Nice)
- [ ] 화면 갱신 최적화(부분 갱신, flicker 최소화)

---

## License
- MIT License
