#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()
#include <string.h>

#define PROC_LOADAVG "/proc/loadavg"
#define PROC_UPTIME "/proc/uptime"

// 1. 시스템 부하(Load Average) 가져오기
void print_load_average() {
    FILE *fp = fopen(PROC_LOADAVG, "r");
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
        printf("[System Load] 1min: %.2f | 5min: %.2f | 15min: %.2f\n", 
               load1, load5, load15);
    }

    fclose(fp);
}

// 2. 시스템 가동 시간(Uptime) 가져오기
void print_uptime() {
    FILE *fp = fopen(PROC_UPTIME, "r");
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

int main() {
    // 화면을 지우는 ANSI 이스케이프 코드
    printf("\033[H\033[J");
    printf("=== MBTop: Linux Process Monitor (Press Ctrl+C to quit) ===\n\n");

    while (1) {
        // 커서를 위로 올려서 덮어쓰기 효과
        printf("\033[3;1H"); // 3번째 줄, 1번째 칸으로 이동
        
        print_uptime();
        print_load_average();
        
        printf("\nRefreshing every 1 second...\n");
        sleep(1); // 1초 대기
    }

    return 0;
}