
# goto를 이용한 에러처리
https://github.com/gurugio/book_cprogramming/blob/master/error_handle.md

# 분기문 갯수 줄이기
https://github.com/gurugio/book_cprogramming/blob/master/long-if.md

# 자주쓰는 for 루프를 간편하게

https://github.com/gurugio/book_cprogramming/blob/master/foreach.md

# 테스트 프레임웍 만들기
테스트케이스마다 개별 파일 정의
시작함수, 종료함수, 실행함수 만들기
각 함수를 매크로로 등록
```
INITTEST_INIT(init_testcase1)
INITTEST_FINAL(final_testcase1)
INITTEST_RUN(run_testcase1)
```
테스트 프레임웍에서 모든 등록함수를 실행
```
struct register_testcase
{
	int (*init)(void *);
	int (*final)(void *);
	int (*run)(void *);
}

for_each_testcase(...)
	case->init(...);
	case->run(...);
	case->final(...);
```

# 테스트 케이스 만들기
http://gurugio.blogspot.com/2010/05/data-driven-design.html

if (test_func(data1) != result1) printf("error1");
if (test_func(data2) != result2) printf("error2");
if (test_func(data3) != result3) printf("error3");
이런 반복을

struct test_data_array
{
int input_data;
int result_data;
char *error_msg;
} test_data_array[] =
{
{data1, result1, "1st test failed"},
{data2, result2, "2nd test failed"},
{data2, result3, "3rd test failed"}
};



for (i = 0; i < test_count; i++)
{
result = test_func(test_data_array[i].input_data)
if (result != test_data_array[i].result_data) printf("%s\n", test_data_array[i].error_msg);
}
이렇게 바꾸자

# 객체지향 흉내내기
구조체에 값,함수포인터 저장
void *이용해서 상속흉내내기

```
struct aaa {
	int val;
	int (*func)(int);
};
struct bbb {
	struct aaa parent;
	int val;
	int (*func)(int);
};
int process(void *ptr)
{
	ptr은 aaa의 주소도 되고 bbb의 주소도된다
	상황에 따라 aaa포인터로 써도된다
}
```

C언어로 C++의 string 객체 만들어보기
구조체안에 문자열 길이,버퍼 포인터, 버퍼 크기 등의 정보 기록
동적 선언,정적 선언, 초기화 등의 매크로 작성

# 프레임웍과 플러그인을 분리하는 프로그래밍
 프레임웍은 디스크립터의 템플릿을 제공하고 플러그인은 디스크립터를 생성해서 프레임웍으로 전달
 프레임웍은 디스크립터를 확인하여 플러그인을 실행함
 http://gurugio.blogspot.com/2010/05/blog-post_14.html
 리눅스 커널의 register_chrdev에서 장치 파일의 디스크립터 struct char_device_struct 데이터 생성 및 관리 방법과 struct file_operations 을 전달하는 의미 -> 예제

# 샘플 프로젝트
https://github.com/gurugio/calib_book/tree/master/ch03
시리얼번호 생성 및 인증 프로그램

XXXXX-XXXXX-XXXXX-XXXXX-XXXXX
이런 형식의 시리얼을 만들기 위해 사용자ID,제품번호,사용기간 등등의 정보를 입력
시리얼번호에 들어갈 정보마다 타입,핸들러 등등 지정
만약 시리얼에 들어갈 정보가 바뀌면?
어떻게하면 데이터와 코드를 분리할 수 있을까?
시리얼번호의 자리수나 형태가 바뀌면?

# 참고자료
https://svn.apache.org/repos/asf/harmony/enhanced/java/trunk/drlvm/vm/port/doc/PortReadme.htm
http://apr.apache.org/docs/apr/1.4/modules.html: 여기나온 에러처리, 메모리관리 등등 소스를 참고해서 예제만들기


# 추가 아이디어들

+ 데이터와 코드를 구분하는 프로그래밍
 http://gurugio.blogspot.com/2010/05/data-driven-design.html
 예제를 뭘로 할지 결정해야함

+ 함수 내부에서 에러를 처리하는 방법과 플랫폼 공통적인 에러값 정의
 http://library.gnome.org/devel/glib/stable/glib-Error-Reporting.html 참고할만한가?
 함수 실패시 점프해서 경우마다 다른 에러처리 루틴을 만들기 (ACP_TEST_RAISE 등)
 에러 처리 루틴을 함수 끝에 모아놓으면 함수의 가독성이 좋아짐
 시스템의 에러 값과 제품의 고유한 에러 값 정의 및 에러 검출 루틴 (acpErrorString)
 쓰레드별 변수를 만들어서 에러가 발생했을 때 에러가 발생한 파일이름/라인번호 등을 저장하기
 linux-kernel-2.6.24의 fs/char_dev.c 파일 중, 최신 커널 소스는 다음과 같이 에러 처리 루틴을 함수 끝부분에 모아놓음 -> 예제

+ C언어로 객체를 흉내내기
 gtk에서 어떻게 하는지, 리눅스 커널에서는 어떻게 하는지 조사. gtk는 객체 흉내를 잘 낸 소스라고함
 동일한 기능의 모듈을 상황에 따라 골라서 사용하기
 동일하거나 유사한 데이터구조인데 약간씩 속성이 다른 경우 각각의 인스턴스를 만들어서 사용하기
 링크드 리스트에서 속성에 따라 더블,서클,락프리 등의 종류별 인스턴스 얻기

+ C언어로 C++의 string 객체 만들기
 구조체안에 문자열 길이,버퍼 포인터, 버퍼 크기 등의 정보 기록
 동적 선언,정적 선언, 초기화 등의 매크로 작성
 acpStr

+ 언어와 인코딩에맞게 사용자 메세지 관리
 언어별,character-set별로 에러 메세지 관리
 에러 메시지는 뭐가 잘못되었는지와 에러를 없애기 위해서 어떻게 해야하는지를 써야한다. no log anchor file이 에러면 make DB file with isql이런 메시지도 같이 나와야 한다.
 메세지 테이블 만드는 방법
 에러 레벨 관리
 로그나 에러 처리 루틴에서 활용하는 예제
 ace

+ 쓰레드별로 에러 번호 관리하기
 TLS만들어서 에러 번호 저장하고 쓰레드별로 에러 번호 확인하는 방법
 쓰레드 여러개 만들어서 각 쓰레드별로 다른 에러 값 반환한 후,
 쓰레드별로 에러 번호 확인해보는 예제 작성

+ 유닛테스트 개발
 무엇을, 어떻게 테스트해야하나
 유닛 테스트에 필요한 매크로/라이브러리 (콜스택 출력)
 테스트 파일을 직접 실행하는게 아니라 utilUGuard를 만들어서 에러 발생시 에러가 발생한 지점과 상황을 알려주기 (콜스택 출력 이용)
 네트워크로 소스를 동기화해서 여러 플랫폼에서 동시에 빌드하고 테스트해서 결과를 모으기
 actTest,utilUGuard

+ 외부 툴을 사용하지 않고 간단하게 메모리 릭을 찾아내는 방법
 http://gurugio.blogspot.com/2010/03/refer-defensive-programming-for-red-hat.html
 이중에서 몇가지 예제 만들기
 efence, duma 조사 - 해당되는 것들인가?

+ 코딩 규칙을 만들고 자동화된 툴로 검사하는 방법
 함수,변수종류,매크로 등의 naming prefix 규칙
 소스 코드 검사 툴 사용하기
 Makefile 에서 실행하도록 적용하기

+ 최신 라이브러리
 다른 최신 C책 조사, 리눅스 시스템 프로그래밍 책 조사할것
 getopt, getopt_long
 stdint.h
