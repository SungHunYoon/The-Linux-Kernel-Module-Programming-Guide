# Linux 커널 모듈 프로그래밍 가이드 실습

이 저장소는 [The Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg)의 내용을 기반으로  
커널 모듈 개발을 연습하고 실습 예제를 정리하기 위한 개인 학습 저장소입니다.

## 📘 원문 안내

- 원문 사이트: [https://sysprog21.github.io/lkmpg](https://sysprog21.github.io/lkmpg)
- 원작자: Peter Jay Salzman, Ori Pomerantz
- 유지보수자: [sysprog21](https://github.com/sysprog21/lkmpg)

## 📁 저장소 구성

이 저장소는 각 장별 실습을 디렉터리로 나누어 구성하고 있습니다.

각 디렉터리에는 예제 코드(`.c`), Makefile, 관련 설명이 포함됩니다.

## 🔧 환경 요구사항

- Linux 시스템 (LFS 8.4에서 실습 중)
- 해당 커널 버전에 맞는 커널 헤더 (kernel headers)
- `make`, `gcc`
- root 권한 (모듈 로드/언로드 시 필요)

## 🛠️ 빌드 및 실행 예시

```bash
make
insmod hello.ko
tail -10 /var/log/kern.log
rmmod hello
