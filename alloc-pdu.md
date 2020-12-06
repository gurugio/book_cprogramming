# PDU (Protocol Data Unit) 사용

PDU의 정의에 대해서는 https://en.wikipedia.org/wiki/Protocol_data_unit 를 참고하세요.
간단히 말하면 모듈이나 계층간에 통신하는데 사용되는 데이터를 말한다고 합니다.
그런데 사실 무슨 말인지 명확하지않습니다.
PDU를 언제 어떻게 사용하는지 예제를 하나 만들어보겠습니다.

## 보통 모듈간의 인터페이스

client 모듈과 server 모듈이 있다고 생각해봅시다.
아주 흔한 경우인데 클라이언트는 서버에게 콜백함수를 전달하고, 서버는 자기 할 일을 하다가 클라이언트의 처리가 필요할 때 콜백함수를 호출합니다.
그럴때 서버는 클라이언트와 서버가 공통으로 사용한 프로토콜 데이터를 할당합니다.

아래와 같은 예제 코드를 생각해보겠습니다.

client
```
int call_other(commdata *data, cli_data *priv)
{
......
}

int fcallback(commdata *data)
{
	int ret;
	cli_data *priv = malloc(sizeof(*priv));
	ret = call_other(data, priv);
	free(priv);
	return ret;
}

int main(void)
{
	register(fcallback);
}
```

server
```
int (*fp)(commondata *);

void register(int (*f)(commondata *));
{
	fp = f;
}

int xfer_data(int x)
{
	int ret;
	commondata *data = malloc(sizeof(*data));
	data->x = x;
	ret = fp(data);
	free(data);
	return ret;
}
```

클라이언트는 서버에게 fcallback이라는 함수를 등록합니다.
서버는 필요에 따라 fcallback를 호출합니다.
그리고 두 모듈간에 공통으로 사용하는 commondata라는 데이터가 존재합니다.

콜백함수가 호출될 때 서버는 commondata타입의 객체를 생성해서 전달합니다.
commondata객체에는 서버가 클라이언트에 전달해야할 정보들이 저장될 것입니다.
클라이언트는 commondata에있는 정보를 받아서 처리를 하는데, 여기서 중요한 것은 클라이언트 자신이 필요로하는 cli_data라는 객체를 생성한다는 것입니다.

전체적으로 두번의 메모리 할당이 실행됩니다.
서버가 한번, 클라이언트가 한번 메모리 할당을 실행합니다.
사실 메모리 할당을 두번 실행하는건 보통의 경우에 전혀 문제가 아닙니다.
하지만 예를 들어 리눅스 커널의 블럭IO 레이어나 네트워크 레이어같이 최대한의 성능을 내야하는 모듈에 메모리 할당이 한번 추가된다면 어떨까요?
단순히 성능만의 문제가 아니라 메모리 할당이 실패했을 경우의 처리도 복잡해집니다.
메모리 할당은 그야말로 시스템에서 가장 성능을 중요시하는 코드입니다.
시스템에 부하가 적을 때 메모리 할당 한번 늘어나는 것은 전혀 문제될게 없습니다만, 만약 시스템이 바쁜 상황이라면 프로세스가 잠들 수도 있고 프로그램의 성능이 순간적으로 떨어질 수도 있습니다.

## PDU를 사용하는 인터페이스

그럼 메모리 할당을 한번 줄이는 방법을 알아보겠습니다.
간단합니다.
서버에게 클라이언트가 사용할 데이터의 크기를 전달해서 메모리 할당을 좀더 크게 하면 됩니다.

client
```
int call_other(commdata *data, cli_data *priv)
{
......
}

int fcallback(commondata *data)
{
	cli_data *priv = (cli_data *)(data + 1);
	return call_other(data, priv);
}

int main(void)
{
	register(fcallback, sizeof(cli_data));
}
```

server
```
int (*fp)(commondata *);
size_t pdu_size;

void register(int (*f)(commondata *), size_t s);
{
	fp = f;
	pdu_size = s;
}

int xfer_data(int x)
{
	int ret;
	commondata *data = malloc(sizeof(*data) + pdu_size);
	data->x = x;
	ret = fp(data);
	free(data);
	return ret;
}
```

서버가 하나의 큰 메모리 덩이리를 할당하고, 시작부분은 commondata로 사용하고, commondata의 바로 뒷 부분은 cli_data로 사용합니다.
서버가 메모리 할당과 해지를 다 처리해주므로 클라이언트는 메모리 할당/해지를 신경쓰지 않습니다.
그러면서도 클라이언트에서만 사용하는 데이터를 만들 수 있습니다.
클라이언트에 메모리 할당과 해지가 없어지므로, 메모리 할당이 실패했을때를 신경쓸 필요도없고, 메모리 해지를 빼놓을 일도 없고 클라이언트를 개발하기에 편리해집니다.

한가지 더 추가할게 있다면 `(cli_data *)(data + 1)` 이 코드에서 실수하기가 쉽습니다.
`(cli_data *)data + 1` 이렇게라고 쓰면 데이터가 깨집니다.
그러니 commondata 구조체에 `char priv[]`를 추가하는 방법이 흔하게 사용됩니다.

common header
```
struct commondata {
	int x;
	char priv[];
};
```

client
```
int call_other(commdata *data, cli_data *priv)
{
......
}

int fcallback(commondata *data)
{
	cli_data *priv = (cli_data *)data->priv;
	return call_other(data, priv);
}

int main(void)
{
	register(fcallback, sizeof(cli_data));
}
```

commondata객체의 priv 필드는 크기가 0인 배열입니다.
data->priv는 결국 commondata객체의 마지막에 위치한 배열의 시작 위치를 가르킵니다.
따라서 클라이언트가 요청한 데이터의 시작 부분과 같습니다.


server
```
int (*fp)(commondata *);
size_t pdu_size;

void register(int (*f)(commondata *), size_t s);
{
	fp = f;
	pdu_size = s;
}

int xfer_data(int x)
{
	int ret;
	commondata *data = malloc(sizeof(*data) + pdu_size);
	memset(data, sizeof(*data) + pdu_size, 0);
	data->x = x;
	ret = fp(data);
	free(data);
	return ret;
}
```

주의할 점은 아래와 같이 `void *priv`라고 쓰면 안됩니다.
```
struct commondata {
	int x;
	char *priv;
};
```

data->priv 값이 0이므로 priv 의 값이 0이 됩니다.

## 참고
https://en.wikipedia.org/wiki/Protocol_data_unit


