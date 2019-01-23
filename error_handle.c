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
