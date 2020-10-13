# Exam_Rank_04

## 문제 번역
- Assignment name  : microshell
- Expected files   : *.c *.h
- Allowed functions: malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup,  dup2, pipe, strcmp, strncmp

---

- 쉘 처럼 작동하는 프로그램을 만들어라!
- 싱대경로, 절대경로로만 작동(cd 제외)
- 파이프와 세미콜론이 쉘 처럼 작동해야 함.
    - 파이프 뒤에 아무것도 안오거나 바로 파이프나 세미콜론이 나오는 케이스들은 고려 안함("| |", "echo 123 |", ";|")
- "cd"는 빌트인 함수(chdir)를 사용
    - 경로나 , '~' 는 구현, '-'는 No
    - 인자가 한개가 아니면 "error: cd: bad arguments" 출력
    - 경로가 잘못 되었으면 "error: cd: cannot change directory to path_to_change" 출력
    - "cd |" 는 절대 테스트 안함.

- 와일드카드, 환경변수 구현 X
- execve, chdir을 제외한 시스템 콜이 리턴 되면 즉시 "error: fatal" 출력하고 프로그램 종료
- execve 실패 시 "error: cannot execute executable_that_failed" 출력
- 수백번의 파이프를 실행 시킬 수 있어야 함.

### 힌트
- execve 에 환경 변수를 넘겨라
- fd를 누수 시키지 마라.


## 개념 복습
### pipe
- https://nroses-taek.tistory.com/139
- https://github.com/LambdaSchool/CS-Wiki/wiki/How-Unix-Pipes-are-Implemented

### dup, dup2
- https://reakwon.tistory.com/104

## 시험 통과!!(201013)
### extern char **environ 사용하면 안됨.
### main 매개변수 char **env 계속 넘겨서 execve의 3번째 매개변수로 전달해야 함.
