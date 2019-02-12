# 샘플 프로젝트: 시리얼 번호 생성 프로그램

## 시험 버전

유닛테스트 프레임웍을 만드는 예제로 인터페이스와 플러그인을 분리하는 디자인을 알아봤습니다.
조금 더 실무에 가까운 프로그램을 하나 만들면서 한번 더 연습을 해보겠습니다.

이번에 만들어볼 프로그램을 시리얼 번호 생성 프로그램입니다.
윈도우를 설치할 때나 게임을 설치할 때 알파벳과 숫자로 이루어진 시리얼 번호를 입력해본 경험이 한번씩은 있을겁니다.
우리는 개발자이니까 사용자가 아닌 회사 입장에서 사용자의 ID와 제품 번호를 가지고 시리얼 번호를 만드는 프로그램을 만들어보겠습니다.
생성된 시리얼은 사용자에게 메일로 알려주게되겠지요. 그러니 프로그램은 시리얼 번호를 생성하는 것까지만 동작하도록 만들겠습니다.

다음 그림과 같이 2개의 정보를 입력받아서 암호화한 후 알파벳과 숫자 12자리로된 시리얼을 생성하는 프로그램입니다.

![serial1](/serial1.png)

암호화까지 만드려면 좀더 생각할게 많아지니까 일단은 단순하게 사용자 ID와 제품번호를 그대로 이어붙여서 12자리 시리얼을 만들어보겠습니다.
대략 다음과 같은 순서로 동작하는 프로그램이 되겠네요.
* 사용자ID 입력받기
* 제품번호 입력받기
* 12자리 버퍼 할당
* 사용자ID 복사
* 사용자ID 뒤에 제품번호 복사
* 생성된 시리얼 출력


```
#include <stdio.h>
#include <string.h>

int make_serial(char *serial, int serial_len,
                char *id, int id_len,
                char *product, int product_len)
{
	int i;
    
	if (id_len + product_len != serial_len) {
		printf("Wrong input: id=4-digit product=8-digit\n");
		return 0;
	}
	else
	{
		strncat(serial, id, id_len);
		strncpy(serial + id_len, product, product_len);
	}

	return strlen(serial);
}

int main(void)
{
	char id[32];
	char product[32];
	char serial[13] = {0,};
	int ret;
    
	printf("Input User ID[4-digits]:");
	fgets(id, 32, stdin);
	id[strlen(id) - 1] = '\0';

	printf("Input Product Number[8-digit]:");
	fgets(product, 32, stdin);
	product[strlen(product) - 1] = '\0';

	ret = make_serial(serial, 12,
			  id, strlen(id),
			  product, strlen(product));
	printf("Serial[%d]=[%s]\n", ret, serial);
    
	return 0;
}
```

너무나 간단하지요. 하지만 실무에서는 언제나 프로그램의 스펙이 바뀐다는걸 기억하셔야됩니다. 몇가지 스펙 변경을 요청하는 경우를 상상해볼까요?
* 영업부서에서 연락이왔습니다. 1년만 계약해서 사용해보고싶다는 고객이 있다네요. 시리얼번호에 1년만 사용가능하도록 사용기간에 대한 정보를 추가해야합니다.
* 마케팅에서 연락이 왔습니다. 다른 제품을 구매한 고객이 이 제품도 사용해보고 싶다고하네요. 시리얼번호에 테스트 버전이라는 정보를 추가해서 몇가지 기능만 사용할 수 있도록 만들어달라고 합니다. 시리얼번호에 테스트 버전이라는 정보도 추가해야합니다.
* 영업부서에서 또 연락이 왔습니다. 제품이 너무 많이 팔려서 4자리 고객 아이디로는 부족하다고 합니다. 고객 아이디를 8자리로 바꿔달라고 합니다.

심지어는 이런 요구사항도 들어올 수 있습니다.
* 시리얼번호를 생성할 때 사용한 암호화 알고리즘이 너무 단순해서 시중에 크랙버전이 돌아다닌다고 합니다. 암호화 알고리즘을 바꿔야합니다. 시리얼 번호에 어떤 암호화 알고리즘이 사용되었는지에 대한 정보도 추가해야합니다.

충분히 있을법한 시나리오 아닌가요?

더 큰 문제는 이런 요구사항들, 스펙변경 요청들이 프로그램의 디자인 단계나 초기 개발단계에서 들어오는게 아니라는 것입니다.
제품을 판매하는 도중에 제품이 잘팔리면 잘팔릴수록 더 많은 요구사항들이 들어오고, 더 많은 스펙 변경 요청이 들어온다는 것입니다.
제품이 안팔려서 잊혀지는 것보다는 행복한 일이지만, 프로그램을 자꾸 변경해야하는 문제가 생깁니다.
어쩌면 몇달 후 make_serial함수는 이런 모습이 되있을지 모릅니다.

```
int make_serial(char *serial, int serial_len,
		char *id, int id_len,
		char *product, int product_len,
		char *expire, int expire_len,
		int trial_type,
		int crypt_type)
{
......
}
```

C 언어에서 함수가 최대 몇개의 인자를 가질 수 있는지 확인해보신적이 있나요?
저는 잘 모릅니다.
제가 분명히 아는건 하나의 함수에 인자가 많을 수록 좋은 디자인이 아니라는 것입니다.

## 연습문제
* ``int make_serial(char *serial, int serial_len, char *id, int id_len, char *product, int product_len, char *expire, int expire_len, 	int trial_type, int crypt_type)`` 타입을 가지는 make_serial을 만들어보세요. 암호화 알고리즘을 사용할 필요는 없고, 단순하게 각 정보들을 그대로 버퍼에 복사하면 됩니다. strncpy를 이용할 수도 있고, sprintf를 이용할 수도 있습니다. 반복되는 코드가 보이시나요? 어떻게하면 반복되는 코드를 유연하게 바꿀 수 있을까요?
