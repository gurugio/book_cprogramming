# Professional C programming tips

# goto를 이용한 에러처리
https://github.com/gurugio/book_cprogramming/blob/master/error_handle.md

# 분기문 갯수 줄이기
https://github.com/gurugio/book_cprogramming/blob/master/long-if.md

# 자주쓰는 for 루프를 간편하게

https://github.com/gurugio/book_cprogramming/blob/master/foreach.md

# 인터페이스과 플러그인을 분리하는 프로그래밍

## C언어로 C++의 string 객체 만들어보기
https://github.com/gurugio/book_cprogramming/blob/master/cstring.md

## 인터페이스과 플러그인은 무엇인가
https://github.com/gurugio/book_cprogramming/blob/master/interface.md

## 유닛테스트 프레임웍

https://github.com/gurugio/book_cprogramming/blob/master/unittest.md


## cstring 의 유닛테스트를 만들어보자.

 프레임웍은 디스크립터의 템플릿을 제공하고 플러그인은 디스크립터를 생성해서 프레임웍으로 전달
 프레임웍은 디스크립터를 확인하여 플러그인을 실행함
 http://gurugio.blogspot.com/2010/05/blog-post_14.html
 리눅스 커널의 register_chrdev에서 장치 파일의 디스크립터 struct char_device_struct 데이터 생성 및 관리 방법과 struct file_operations 을 전달하는 의미 -> 예제
 
## 테스트 인터페이스 만들기
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

## 테스트 플러그인 만들기
http://gurugio.blogspot.com/2010/05/data-driven-design.html

```
if (test_func(data1) != result1) printf("error1");
if (test_func(data2) != result2) printf("error2");
if (test_func(data3) != result3) printf("error3");
```
이런 반복을
```
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
```
이렇게 바꾸자


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


