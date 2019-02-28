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

Please notice that the main function also has two error handling.
It is very short and does only two things.
It consists of 10 lines and half of them are error handling code.
Eror handling should be everywhere.
That is why the design of error handling is essential.

main function only checks the number of command arguments and call file_copy1 function.
Then file_copy1 function starts.
The first error handling is for the source file opening.
It just returns -1 value and exit if failing to open the source file.

The second error handling is for the target file creating.
It should do something more than retuning -1.
It should close the source file because the main function does not know if the source file is open.

The third error handling is for the buffer memory allocation.
It should not only close the source file but also the target file.

Do you find any pattern?
The error handling code becomes longer.
The last one is much longer than the first one.
Why?
The file_copy1 function should revert every status before returning the main function.
The main function does not know what file_copy1 does.
The main function cannot close the source file because it does not know if file_copy1 open the source file.
The file_copy1 function should clean up what it does until it meets error.
If it does many things and meets error, it should clean up many.
If it does few things and meets error, there would not many things to be cleaned up.

This the very simple function, file_copy1, has 9 lines for error handling.
Longer function has usually more error handling code.
Poor design of error handling will makes functions to difficult to read or modify.
If a function opens 5 files, the error handling of the last file opening should close the 4 files.

But there are two patterns.

The first pattern is the same error handling is repeating.
Closing the source file repeats at the first and the second error handlings.

The another pattern is the order of error handling code is is reverse to the normal code.
file_copy1 does 'opening the source file' -> 'opening the target file' -> 'buffer memory allocation'.
The order of error handling is 'closing the target file' -> 'closing the source file'.
if we add one more error handling after the buffer memory allocation, the order would be 'buffer memory allocation' -> 'closing the target file' -> 'closing the source file'.

Let me show you another code with better error handling.

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

When error happens, it just jumps to error handling code with goto command.
There is no long and repeated error code.
It is not only good for code reading but also code understanding.

Let us see the sequence of the error handling.
The first error handling, closing the source file, is at the last.
The second error handling, closing the target file, is at the second and the last error handling is at the first.
What happens now?
The error handling code is doing not only the error handling.
It is the normal clean-up code that terminate the ffile_copy2 function.

We only reverse the sequence of the clean-up code and add some labels.
But it can handle both of the error case and no-error case.

Please remember two:
* The error handling is sub-set of the cleaning-up.
* The order of error handling is usually the reverse of what it should do.

## Exercises
* The do-while loop in file_copy2 function calls fread and fwrite. What should we do if the fread and fwrite fails? How can we return the error value of fread and fwrite?
* file_copy1 only returns -1 for all error cases.
 

  * file_copy2함수안에 do-while 루프가 있습니다. 이 루프안에 fread, fwrite 함수를 호출하는데요 이 함수들이 에러를 반환했을 경우에는 어떻게 처리해야할까요?
  * file_copy1함수는 모든 에러처리의 반환값이 -1로 동일합니다. 각 에러마다 다른 반환값을 반환하도록 고쳐보세요. 그리고 file_copy2함수와 같이 goto문을 이용해서 에러처리를 한곳으로 모아보세요.
  * 자신이 예전에 만들었던 코드들을 다시 열어보시고, 에러처리가 복잡한 함수가있는지 확인해보세요. 그리고 최대한 에러처리를 간결하게 고쳐보세요.
  * 예제코드를 보면 calloc을 호출할때 sizeof(*buf)라는 코드가 있습니다. 보통 sizeof(char)로 많이 씁니다. 저는 왜 sizeof(*buf)로 썼을까요? 만약 buf의 데이터 타입이 char가 아니라 short이나 int, 아니면 다른 구조체였다면 sizeof 매크로에 무엇을 전달하는게 편리할까요? 유지보수의 관점에서 생각해보세요. 예를 들어 처음에 프로그램을 만들때는 char 타입의 버퍼를 만들어 썼지만, 나중에 유니코드나 utf-8로 작성된 파일을 복사하도록 프로그램의 스펙이 변경되었을 때를 가정해서 프로그램을 고쳐보세요.
  * file_copy2 함수는 사본 파일이 이미 존재할 때 어떻게 동작할까요? 사본 파일이 이미 존재하는지를 확인하고, 만약 존재한다면 file_copy2 함수를 종료하고 에러값을 반환하도록 프로그램을 고쳐보세요.
