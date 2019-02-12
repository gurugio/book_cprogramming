# 샘플 프로젝트: 시리얼 번호 생성 프로그램

## serial 프레임웍 구현

다음 그림과 같이 사용자 ID와 제품번호만 가지고 시리얼을 생성하던 프로그램은 앞으로 더 어떤 정보를 입력받게될지 알 수 없습니다.

![serial2](/serial2.png)

그런데 한가지 공통점이 있습니다. 어떤 정보든 숫자나 알파벳으로 이루어진 문자열이라는 것입니다. 또 각 정보마다 자리수가 미리 정해집니다.

결국 serial이라는 프레임웍은 문자열과 자리수를 입력하는 인터페이스를 제공하게되고, 각 플러그인들은 각자의 문자열과 자리수를 제공하면 됩니다.

![serial3](/serial3.png)

다음은 serial_interface.h의 내용입니다.

```
#include <stdio.h>

struct serial_info {
	char *data;
	int digit;
	int (*put)(struct serial_info *info);
	char *(*get)(struct serial_info *info);
};

#define DEFINE_SERIALINFO(__name, __struct_info)	\
	void register_##__name(void) {					\
		register_info(#__name, &__struct_info);	\
	}

#define REGISTER_SERIALINFO(__name)    \
	void register_##__name(void);	 \
	register_##__name()

int register_info(char *name, struct serial_info *info);
int create_serial(void);
```

각각의 플러그인으로부터 문자열과 자리수를 입력받기위해 struct serial_info 구조체를 정의했습니다.
struct serial_info 구조체에있는 put는 사용자로부터 데이터를 입력받는 함수입니다. 반대로 get함수는 프레임웍이 플러그인으로부터 데이터를 받는 함수입니다.
유닛테스트를 만들때와 마찬가지로 ``DEFINE_``함수와 ``REGISTER_``함수를 만들었습니다. 사용법도 동일합니다.

다음은 시리얼 프로그램의 프레임웍을 구현한 serial_interface.c 파일입니다.

```
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "serial_interface.h"

struct serial_node {
	char *name;
	struct serial_info *info;
	struct serial_node *next;
};

struct serial_node *head;


int register_info(char *name, struct serial_info *info)
{
	struct serial_node *new_node = calloc(1, sizeof(*new_node));

	new_node->name = name;
	new_node->info = info;
	new_node->next = NULL;

	if (head == NULL) {
		head = new_node;
	} else {
		struct serial_node *old = head;
		head = new_node;
		new_node->next = old;
	}

	printf("serial-info [%s] is registered\n", name);
	return 0;
}

int create_serial(void)
{
	struct serial_node *node;
	char *serial_buffer = calloc(64, sizeof(char));
	int digit = 0;

	for (node = head; node != NULL; node = node->next) {
		node->info->put(node->info);
		digit += node->info->digit;
		strcat(serial_buffer, node->info->get(node->info));
	}

	printf("Serial[%d]: [%s]\n", digit, serial_buffer);
	return 0;
}
```

유닛테스트 프레임웍과 거의 동일합니다. 플러그인으로부터 받은 정보들을 리스트로 저장후 시리얼을 생성할 때 플러그인 함수를 호출합니다. 각 플러그인이 제공한 put함수는 사용자로부터 정보를 받고, get함수는 프레임웍에게 정보를 전달합니다.

사용자 ID를 저장하는 플러그인 serialinfo_id.c 소스를 보겠습니다.

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial_interface.h"


#define ID_DIGIT 4

int put_id(struct serial_info *info)
{
	char *id;

	info->digit = ID_DIGIT;

	/* fgets stores the newline and the null characters in the buffer.
	 * So buffer size should be +2.
	 */
	id = calloc(ID_DIGIT + 2, sizeof(char));
	printf("Input User ID[%d-digits]:", info->digit);
	fgets(id, ID_DIGIT + 2, stdin);
	id[strlen(id) - 1] = '\0';

	info->data = id;
	return 0;
}

char *get_id(struct serial_info *info)
{
	return info->data;
}

struct serial_info serialinfo_id= {
	.put = put_id,
	.get = get_id,
};

DEFINE_SERIALINFO(id, serialinfo_id)
```

``put_id``함수는 ``serial_simple.c``에서 만든것과같이 사용자로부터 4자리 숫자를 입력받습니다. 그리고 ``serial_info serialinfo_id`` 객체에 저장합니다. 4자리라는 자리수 또한 ``serial_info`` 객체에 저장됩니다.

다음은 serial_main.c 파일입니다.

```
#include <stdio.h>
#include <string.h>
#include "serial_interface.h"

int main(void)
{
	/* new info should be added at top */
	REGISTER_SERIALINFO(id);
	
	create_serial();
    
	return 0;
}
```

id라는 serial_info 객체를 등록하고, 시리얼 생성 함수를 호출합니다.

거의 모든게 유닛테스트 프레임웍과 동일해서 굳이 자세히 설명하지 않아도 아시겠지요?
반대로 생각하면 유닛테스트 프레임웍이 다양하게 활용될 수 있다는 것도 아실 수 있으실것입니다.

## 연습문제
* ``serialinfo_id.c``은 사용자 ID를 입력받아서 ``serial_info serialinfo_id`` 객체에 저장합니다. 제품 번호를 입력받아서 ``serial_info serialinfo_product``객체에 저장하는 ``serialinfo_product.c``를 만들어보세요. ``serial_main.c`` 파일에도 추가해서 실행해보세요.

## 추가 프로젝트
사용자 ID나 기타 정보를 일일이 입력받는 방식을 여러개의 시리얼을 생성하기에 불편합니다. 다음과 같이 각 줄마다 사용자 ID와 제품번호가 써진 파일을 읽어서 여러개의 시리얼을 생성하는 프로그램을 만들어보세요.

## 추가 프로젝트
시리얼 생성 프로그램과 반대로 시리얼 번호로부터 사용자 ID와 제품번호를 뽑아내는 시리얼 인증 프로그램이 있어야 사용자가 입력한 시리얼이 정상적인 시리얼인지 알 수 있겠지요. 시리얼 인증 프로그램도 만들어보세요. 시리얼 생성 프로그램과 하나의 프로젝트로 만들어야할까요? 아니면 분리된 프로젝트로 만들어야할까요? 만약 분리된 프로젝트로 만들어야한다면, 두 프로젝트가 가진 공통된 데이터 (사용자 ID와 제품번호의 자리수)를 어떻게 공유할 수 있을까요?

## 추가 프로젝트
사실 실제로 실무에서 사용될 프로그램이라면 사용자 ID의 자리수나 제품번호의 자리수 등 시리얼 번호 생성에 들어가야할 정보들이 하나의 설정 파일에 저장되는 경우가 많을 것입니다. 다음처럼 serial.conf 파일을 만듭니다.
```
user-id 4
product-id 8
```
시리얼 생성 프로그램을 serial.conf 파일을 한줄씩 읽어서 각각의 플러그인을 생성하도록 만들어보세요.
그리고 시리얼 생성 프로그램과 인증 프로그램 둘 다 serial.conf 파일을 이용하도록 만들어보세요.

## 추가 프로젝트
사용자 ID와 제품번호를 암호화해서 시리얼 번호를 만들도록 프레임웍을 수정해보세요. 여러가지 암호화 알고리즘 중에서 선택할 수 있도록 플러그인을 만들어서 구현해보세요. openssl 라이브러리를 참고하세요.
