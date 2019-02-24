# Error handling

Every program meets error.
Every program has its own plan to handle error gracefully.
It does not matter if the program is big or small, for test or for production.
This chapter introduces an example to read and copy a file, and introduces one techniques to make the error handling simple to make and to look.

First let us think about what the program should do.
* open a source file
* open a target file
* allocate a memory buffer
* read the source file and store the contents in the memory buffer
* write the contents in the memory buffer into the target file
* repeate reading and writing until it meets the end of the source file
* free the buffer memory
* close the source file
* close the target file
* end

해야 할 일들마다 만약 그 일이 실패한다면을 생각해야 어떤 상황에서도 안정적으로 동작하는 프로그램이 될 수 있습니다. 원본 파일을 열다라는 하나의 동작에도 여러가지 에러 상황이 있을 수 있습니다. 원본 파일이 없을 수도 있고, 파일이 다른 사람의 것이라서 열지 못할 수도 있고, 심지어는 시스템에 메모리가 부족하거나 네트워크 연결이 끊겨서 파일을 열지못할 수 도 있습니다.

Whenever the program does something, it could meet an error.
It is simple. The program tries to do this but it can fail. 
It would be the best if every line has its own error handling.
Sometimes one action could meet various errors.
But let us start with the simplest case.
We will only terminate the program when something bad happens.
We will not case what the error is.

Let us write what the program should do with the error handling scenario.
* open a source file
  * If it fails, terminate the program.
* open a target file
  * If it fails, close the source file
  * and terminate the program.
* allocate a memory buffer
  * If it fails, close the target file
  * close the source file
  * and terminate the program.
* read the source file and store the contents in the memory buffer
* write the contents in the memory buffer into the target file
* repeate reading and writing until it meets the end of the source file
* free the buffer memory
* close the target file
* close the source file
* end

총 4가지 경우의 에러가 발생할 수 있습니다. 첫번째 에러는 별도로 처리할게 없이 바로 종료하면 됩니다. 에러가 발생하기 전까지 아무런 일도 하지 않았기 때문입니다. 그런데 두번째 에러부터 추가로 해야할 일이 있습니다. 그 전까지 실행했던 일들을 되돌려놓는 일을 해야합니다. 파일을 열었다면 닫아주어야하고, 메모리를 할당받았다면 해지해야합니다. 두번째 에러는 이전에 열었던 원본 파일을 닫아야하고, 세번째 에러는 원본 파일과 사본 파일 둘다 닫고 종료합니다.

Let us think about 4 error cases.
The first error is failing to open the source file.
We do not need to do something for it becase we have not done anything.
We only terminate the program.

The second error is failing to open the target file.
We should not terminate the program immediately because we have opened the source file.
We have done something. We are responsible for the open file.
Therefore we close the source file and then we terminate the program.

The third error handling also turning back all things we have done before.
We close the source and the target files, and then we terminate the program.

Let me show you an example code.

```c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int file_copy1(char *src, char *dst)
{
	FILE *fd_src, *fd_dst;
	char *buf;
	size_t total = 0;

	printf("copy %s to %s\n", src, dst);

	fd_src = fopen(src, "r");
	if (fd_src == NULL)
		return -1;

	fd_dst = fopen(dst, "w");
	if (fd_dst == NULL) {
		fclose(fd_src);
		return -1;
	}

	buf = calloc(4096, sizeof(*buf));
	if (buf == NULL) {
		fclose(fd_src);
		fclose(fd_dst);
		return -1;
	}

	total = 0;
	do {
		int c;
		c = fread(buf, sizeof(*buf), 4096, fd_src);
		if (c == 0)
			break;
		c = fwrite(buf, sizeof(*buf), c, fd_dst);
		if (c == 0)
			break;
		total += c;
	} while (1);

	free(buf);
	fclose(fd_src);
	fclose(fd_dst);
	return total;
}

int main(int argc, char *argv[])
{
	int c;
	int (*fp)(char *src, char *dst);

	if (argc != 3) {
		printf("usage: ./a.out SRC DEST\n");
		return 1;
	}

	fp = file_copy1;
	c = fp(argv[1], argv[2]);
	if (c < 0)
		printf("failed\n");

	return 0;
}
```

예제 코드를 같이 읽어보겠습니다. main함수는 프로그램에 전달된 인자의 갯수가 3개인지 확인하고, 3개일 경우에만 file_copy1 함수를 호출합니다. main함수에도 2개의 에러처리가 있습니다. 인자의 개수가 잘못된 경우와 file_copy1함수의 결과값이 잘못되었을때 2번 에러처리를 합니다. 이렇게 짧은 코드에도 2개의 에러처리가 들어갑니다. 에러처리를 위한 코드의 양도 printf 두개 return 한개 3라인이나 됩니다. 전체 10라인의 함수에서 3라인이 에러처리코드입니다. 에러처리는 이렇게 아주 짧은 코드에도 반드시 들어갑니다. 에러처리를 어떻게 하느냐에 따라 코드 전체의 구조나 품질이 바뀔 수 있습니다.

다음으로 우리가 파일 복사 함수 file_copy1을 읽어보겠습니다. 첫번째 에러처리는 원본 파일을 열지못했을때의 처리입니다.  마찬가지로 실패하면 -1을 반환하고 함수를 끝냅니다.

두번째 에러처리는 복사본 파일을 생성하지 못했을때의 처리입니다. 이때는 실패하면 -1을 반환하는것뿐 아니라 이전에 열었던 파일도 닫아주어야합니다.

세번째 에러처리는 파일을 읽어서 저장할 버퍼를 할당하지 못했을경우의 처리입니다. 이때는 함수를 끝내기전에 그동안 열었던 두 파일을 닫아야합니다.
 
마지막으로 함수가 끝날때는 버퍼를 해지하고 두 파일을 닫게됩니다.

어떤 패턴을 찾으셨나요? 에러처리는 함수의 뒤로 갈수록 점점 더 길어집니다. 함수를 끝까지 실행하지못하고 중간에 종료해야할 경우, 그 이전까지 실행했던 것들을 되돌리고 함수를 끝내야합니다. 그래야 함수를 호출한 main함수에서 그 함수를 반복해서 호출해도 main함수 내부에서는 아무런 영향도 받지 않게되니까요. 이렇게 어떤 함수를 호출하기전에 호출하기후의 상태가 동일한 함수를 side effect가 없다고합니다. main함수와 file_copy1함수가 서로 orthogonal하다고 말하는 경우도 있습니다. main함수가 file_copy1함수를 호출하기 전과 호출한 후에 main함수가 가지고있는 상태가 변하지않는다는게 중요합니다. 물론 file_copy1함수의 결과값을 저장하는 변수c는 예외로 합니다.
 
에러처리로 다시 돌아가면 함수의 중간에 에러를 만나서 끝날때 그 함수가 수행했던 것들을 되돌리는 작업을 하는게 좋은 좋은 에러처리의 기본이라는 것입니다. 이때 문제가 한가지 생깁니다. 함수에 여러개의 에러처리가 있는 경우 뒤로 갈수록 점점 더 해야할게 많아진다는 것입니다. file_copy1함수는 파일 복사라는 핵심적인 동작을 수행하기 위해 2개의 파일을 열고, 메모리 할당을 하는데, 이 3가지 경우 모두 에러처리가 필요합니다. 첫번째 파일 열기를 실패했을때는 다른 에러처리가 필요없이 에러값을 반환하는 것뿐이지만, 마지막 메모리 할당이 실패했을때는 파일 2개를 닫는 처리가 추가됩니다. 만약 메모리 할당이 성공한 후에 에러를 만나게되면 메모리 할당도 추가된 에러처리가 필요해지겠지요. 아주 간단한 예제라서 가장 복잡한 에러처리도 겨우 3줄뿐이지만, 만약 수십 수백줄 정도되는 함수라면 에러처리가 얼마나 더 길어질지 막막해집니다.

그런데 아주 중요한 패턴이 두개 더 있습니다. 각각의 에러처리를 보면 중복된다는 것입니다. 첫번째 에러처리는 원본 파일을 닫는것인데, 두번째 에러처리는 원본 파일을 닫는게 또 있습니다. 예제 코드에는 없지만 만약 세번째 네번째 에러처리가 더 있다면 원본 파일을 닫는게 계속 반복되겠지요.
 
두번째 중요한 패턴은 함수가 수행한 것들을 복구하는 코드가, 함수의 수행과 반대 순서라는 것입니다. 원본파일열기 -> 복사본파일열기->메모리할당, 이렇게 3개의 처리를 복구하는 코드는 순서를 반대로해서 메모리해지->복사본파일닫기->원본파일닫기가 됩니다.

이 두가지 패턴을 잘 생각해서 file_copy1의 에러처리를 좀더 효율적으로 바꾼게 file_copy2입니다.

```c
int file_copy2(char *src, char *dst)
{
	FILE *fd_src, *fd_dst;
	char *buf;
	size_t total = -1;


	printf("copy %s to %s\n", src, dst);

	fd_src = fopen(src, "r");
	if (fd_src == NULL)
		goto error_src;

	fd_dst = fopen(dst, "w");
	if (fd_dst == NULL)
		goto error_dst;

	buf = calloc(4096, sizeof(*buf));
	if (buf == NULL)
		goto error_buf;

	total = 0;
	do {
		int c;
		c = fread(buf, sizeof(*buf), 4096, fd_src);
		if (c == 0)
			break;
		c = fwrite(buf, sizeof(*buf), c, fd_dst);
		if (c == 0)
			break;
		total += c;
	} while (1);

	free(buf);
error_buf:
	fclose(fd_dst);
error_dst:
	fclose(fd_src);
error_src:
	return total;
}
```

각각의 에러처리에 에러처리 코드를 넣지않고 goto를 이용해서 에러처리 코드가 있는 곳으로 점프시킵니다. 반복되는 코드가 없어지므로 코드가 훨씬 간결해집니다. 코드가 짧으니 읽기 편합니다. 코드를 읽으면서 예외적인 에러 발생에 대해 신경쓰지않고, 먼저 핵심적인 코드에 집중할 수 있으니 더 이해하기도 쉽습니다.

각각의 에러처리를 보면 첫번째 에러처리 코드 error_src라벨이 가장 마지막에 위치합니다. 두번째 에러처리 error_dst라벨은 그 위에, 마지막 에러처리가 가장 위에 위치합니다. 결과적으로 동일한 코드로 에러처리와 정상처리 둘다 할수있게 되었습니다. file_copy1에서 함수 마지막에 메모리해지와 파일닫기 처리를 했었습니다. 사실은 이런 복구처리가 에러처리와 동일합니다. 함수가 중간에 예외의 경우를 만나 중단된 경우에는 복구처리를 중간부터 실행하면 되는 것입니다. 결국 3가지 예외를 처리하기위한 코드가 3개의 라벨을 추가하는 것으로 완성되었습니다. file_copy1의 예외처리 코드중에서 반복되었떤 파일닫기가 없어졌습니다.

우리는 2가지를 알 수 있었습니다.
에러처리는 결국 함수가 원래 하던 일의 부분집합이라는 것
에러처리는 함수가 원래 하던 일의 역순이라는 것

이렇게 2가지 사실을 기억하면서 에러처리를 만들어나가면 많은 중복 코드를 지우고, 더 읽기 쉽게 이해하기 쉬운 코드를 만들 수 있습니다.

## 연습문제
  * file_copy2함수안에 do-while 루프가 있습니다. 이 루프안에 fread, fwrite 함수를 호출하는데요 이 함수들이 에러를 반환했을 경우에는 어떻게 처리해야할까요?
  * file_copy1함수는 모든 에러처리의 반환값이 -1로 동일합니다. 각 에러마다 다른 반환값을 반환하도록 고쳐보세요. 그리고 file_copy2함수와 같이 goto문을 이용해서 에러처리를 한곳으로 모아보세요.
  * 자신이 예전에 만들었던 코드들을 다시 열어보시고, 에러처리가 복잡한 함수가있는지 확인해보세요. 그리고 최대한 에러처리를 간결하게 고쳐보세요.
  * 예제코드를 보면 calloc을 호출할때 sizeof(*buf)라는 코드가 있습니다. 보통 sizeof(char)로 많이 씁니다. 저는 왜 sizeof(*buf)로 썼을까요? 만약 buf의 데이터 타입이 char가 아니라 short이나 int, 아니면 다른 구조체였다면 sizeof 매크로에 무엇을 전달하는게 편리할까요? 유지보수의 관점에서 생각해보세요. 예를 들어 처음에 프로그램을 만들때는 char 타입의 버퍼를 만들어 썼지만, 나중에 유니코드나 utf-8로 작성된 파일을 복사하도록 프로그램의 스펙이 변경되었을 때를 가정해서 프로그램을 고쳐보세요.
  * file_copy2 함수는 사본 파일이 이미 존재할 때 어떻게 동작할까요? 사본 파일이 이미 존재하는지를 확인하고, 만약 존재한다면 file_copy2 함수를 종료하고 에러값을 반환하도록 프로그램을 고쳐보세요.
