# 컴파일러 설정
CC = gcc
# 컴파일 옵션: -Wall(모든 경고), -Wextra(추가 경고), -g(디버깅 정보 포함)
CFLAGS = -Wall -Wextra -g

# [수정됨] 실행 파일 이름에 .out 확장자를 붙여 관리 편의성 증대
TARGET = mb_top.out

# 소스 파일 목록
SRCS = mb_top.c
# 소스 파일(.c)을 컴파일하여 생성될 오브젝트 파일(.o) 목록 자동 생성
OBJS = $(SRCS:.c=.o)

# 기본 타겟: make만 쳤을 때 실행됨
all: $(TARGET)

# 링크 과정: 오브젝트 파일들을 묶어서 최종 실행 파일 생성
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 컴파일 과정: .c 파일을 .o 파일로 컴파일 (확장자 규칙)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 정리(Clean): 빌드된 파일들을 싹 지움
clean:
	rm -f $(TARGET) $(OBJS) 