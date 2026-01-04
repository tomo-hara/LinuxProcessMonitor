#include <stdio.h>
#include <unistd.h> // getpid 함수를 사용하기 위해
#include <stddef.h>

int main() {
    // 1. 기본 출력 테스트
    printf("Hello World!\n");
    
    // 2. 시스템 정보 조회 (현재 프로세스 ID)
    pid_t pid = getpid();
    
    printf("Current Process ID (PID): %d\n", pid);
    printf("Running on ARM64 Linux Architecture.\n");

    return 0;
}