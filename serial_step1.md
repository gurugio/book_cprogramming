# Sample project: serial number generator

## Initial version

Unittest framework shows you the design concept of the framework and plugin.
I would like to show you one more example which are a little bit more practical.

It is a serial number generator.
It is very common to input a serial number when we install a software such like Windows.
Have you ever imagine how Microsoft generates the serial number?

Let us assume that the serial generator gets two data and returns 12-digit alphabet and number.

![serial1](/serial1.png)

We do not consider encryption to keep it simple.
Let us try a simplest example that gets user ID and product number.
And it returns 12-digit serial that is just a combination of the user ID and product number.
It should be work as following.
* request the user ID
* request the product number
* allocate 12-byte buffer
* copy the user ID
* copy the product number after the user ID
* print the serial number

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
