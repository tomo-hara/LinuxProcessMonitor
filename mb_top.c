#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROC_LOADAVG "/proc/loadavg"
#define PROC_UPTIME "/proc/uptime"
#define PROC_STAT "/proc/stat"

// 터미널 제어용 ANSI 코드
#define ANSI_CLS        "\033[2J"    // 화면 전체 지우기
#define ANSI_HOME       "\033[H"     // 커서를 맨 왼쪽 위(0,0)로 이동
#define ANSI_HIDE_CURSOR "\033[?25l" // 커서 숨기기 (깔끔함 UP)
#define ANSI_SHOW_CURSOR "\033[?25h" // 커서 보이기 (종료 시 복구용)

// CPU 통계 데이터를 담을 구조체
typedef struct cpu_stats{
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
} cpu_stats_t;

// /proc/stat 파일을 읽어서 구조체를 구성하는 함수
void get_cpu_stats(cpu_stats_t* stats) {
    FILE* fp = fopen(PROC_STAT, "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        return;
    }

    char buf[1024];
    if (fgets(buf, sizeof(buf), fp) == NULL) {
        perror("Failed to read /proc/stat");
        fclose(fp);
        return;
    }

    // 문자열 "cpu" 뒤의 숫자들을 파싱
    // 형식: cpu  user nice system idle iowait irq softirq steal ...
    sscanf(buf, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &stats->user, &stats->nice,
           &stats->system, &stats->idle, &stats->iowait, &stats->irq, &stats->softirq,
           &stats->steal);

    fclose(fp);
}

// 1. 시스템 부하(Load Average) 가져오기
void print_load_average() {
    FILE* fp = fopen(PROC_LOADAVG, "r");
    if (fp == NULL) {
        perror("Failed to open /proc/loadavg");
        return;
    }

    double load1, load5, load15;
    // /proc/loadavg 예시: 0.00 0.01 0.05 1/135 12345
    // 앞에서부터 실수형 데이터 3개를 읽어옵니다.
    if (fscanf(fp, "%lf %lf %lf", &load1, &load5, &load15) != 3) {
        fprintf(stderr, "Failed to parse load average\n");
    } else {
        printf("[System Load] 1min: %.2f | 5min: %.2f | 15min: %.2f\n", load1, load5, load15);
    }

    fclose(fp);
}

// 2. 시스템 가동 시간(Uptime) 가져오기
void print_uptime() {
    FILE* fp = fopen(PROC_UPTIME, "r");
    if (fp == NULL) {
        perror("Failed to open /proc/uptime");
        return;
    }

    double uptime_seconds;
    // /proc/uptime 예시: 3600.50 1200.00 (가동시간, 유휴시간)
    if (fscanf(fp, "%lf", &uptime_seconds) != 1) {
        fprintf(stderr, "Failed to parse uptime\n");
    } else {
        // 초 단위를 시:분:초로 변환
        int hours = (int)uptime_seconds / 3600;
        int minutes = ((int)uptime_seconds % 3600) / 60;
        int seconds = (int)uptime_seconds % 60;

        printf("[System Uptime] %02d:%02d:%02d\n", hours, minutes, seconds);
    }

    fclose(fp);
}

// 화면 초기화 함수 (프로그램 시작 시 한 번 호출)
void init_screen() {
    printf("%s", ANSI_CLS);         // 화면 싹 지우기
    printf("%s", ANSI_HIDE_CURSOR); // 커서 숨기기
    fflush(stdout); // 버퍼 비우기 (즉시 반영)
}

int main() {
    init_screen();

    cpu_stats_t prev, curr;

    // 첫 번째 데이터 수집 (기준점)
    get_cpu_stats(&prev);

    while (1) {
        sleep(1);  // 1초 대기 (이 사이에 CPU가 일을 함)

        // 두 번째 데이터 수집
        get_cpu_stats(&curr);

        // --- 계산 로직 시작 ---
        // 1. 이전 시점의 합계와 Idle
        unsigned long long prev_idle = prev.idle + prev.iowait;
        unsigned long long prev_total =
            prev.user + prev.nice + prev.system + prev_idle + prev.irq + prev.softirq + prev.steal;

        // 2. 현재 시점의 합계와 Idle
        unsigned long long curr_idle = curr.idle + curr.iowait;
        unsigned long long curr_total =
            curr.user + curr.nice + curr.system + curr_idle + curr.irq + curr.softirq + curr.steal;

        // 3. 차이(Delta) 계산
        unsigned long long total_diff = curr_total - prev_total;
        unsigned long long idle_diff = curr_idle - prev_idle;

        // 4. 사용률(%) 계산: (전체 차이 - Idle 차이) / 전체 차이
        double cpu_usage = 0.0;
        if (total_diff > 0) {
            cpu_usage = (double)(total_diff - idle_diff) / total_diff * 100.0;
        }
        // --- 계산 로직 끝 ---

        printf("%s", ANSI_HOME);

        printf("=== MBTop: Linux Process Monitor (Press Ctrl+C to quit) ===\n\n");

        print_uptime();
        print_load_average();

        // CPU 바(Bar) 그리기 효과
        printf("[ CPU Usage] %.1f%% [", cpu_usage);
        int bars = (int)(cpu_usage / 5);  // 5% 당 막대기 하나
        for (int i = 0; i < 20; i++) {
            if (i < bars)
                printf("#");
            else
                printf(" ");
        }
        printf("]\n");

        // 현재 화면 내용보다 짧은 내용이 출력될 경우를 대비해
        // 화면 나머지 부분을 지우는 코드
        printf("\033[J"); // 커서 위치부터 화면 끝까지 지우기

        fflush(stdout); // 출력 버퍼 강제 비우기

        // 현재 상태를 '이전 상태'로 저장 (다음 루프를 위해)
        prev = curr;
    }

    return 0;
}