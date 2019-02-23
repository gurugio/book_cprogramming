# Professional C programming: Framework and plugin design in C

C프로그래밍을 실무로 쓴 기간은 12년, 처음 접한 후부터로 따지만 20년정도를 써왔습니다.
그러다보니 시중에 있는 좋은 책들과 강좌들이 C의 기초에 대해서 아주 잘 가르쳐주고있지만, 문법을 배운 후 실무로 개발할 때에 미리 알아두면 좋을 실전적인 노하우들에 대해서는 약간 부족하다는걸 느꼈습니다.
저는 운좋게 좋은 회사에서 좋은 분들께 많이 배웠고, 리눅스 커널을 공부하고 개발하면서 전 세계 개발자들이 C를 어떻게 쓰는지를 배울 수 있었습니다.
그렇게 배운 것들 중에 중요한 것들을 문서로 정리해봤습니다.

I first met the C language 20 years ago. I have been working with C language for 12 years.

학생분들은 일주일이나 보름정도 따라해보시면서 단순히 함수 포인터나 구조체, 매크로등 배웠던 것들이 실제로 실무에 어떻게 사용되는지를 알아가시면 좋을듯합니다.
실무를 시작한지 얼마안된 분들은 가볍게 읽으시면서 본인의 코드나 자사 코드베이스와 비교해보시면 좋을듯합니다.

이 문서의 내용외에도 좋은 기법들이 많이있습니다. 프로젝트 이슈를 통해 알려주시면 문서로 작성해서 추가하겠습니다.

(1st version: Feb. 12 2019)

## 목차

* [goto를 이용한 에러처리](error_handle.md)
* [분기문 갯수 줄이기](long-if.md)
* [자주쓰는 for 루프를 간편하게](foreach.md)
* 인터페이스과 플러그인을 분리하는 프로그래밍
  * [C언어로 C++의 string 객체 만들어보기](cstring.md)
  * [인터페이스과 플러그인은 무엇인가](interface.md)
  * [유닛테스트 프레임웍](unittest.md)
  * [cstring의 유닛테스트](unittest_cstring.md)
  * [테스트 케이스를 간편하게](unittest_cstring2.md)
* 샘플 프로젝트: 시리얼 번호 생성 프로그램
  * [시험 버전](serial_step1.md)
  * [프레임웍 버전](serial_step2.md)

## TODO
