#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <error.h>
#include <errno.h>

#define MAX_WORD_LEN		1024
#define TEXT_FILENAME_BASE	"quotes.txt"
#define NUM_FILENAME_BASE	"num.txt"
#define END_STR			"***"

char * get_text_filename(void)
{
	static char text_filename[FILENAME_MAX] = "";
	if (text_filename[0] == 0) {
		strcpy(text_filename, __FILE__);
		char * p = text_filename + strlen(text_filename);
		while (*p != '/' && p > text_filename)
			--p;
		if (*p == '/')
			++p;
		strcpy(p, TEXT_FILENAME_BASE);
	}
	return text_filename;
}

char * get_num_filename(void)
{
	static char num_filename[FILENAME_MAX] = "";
	if (num_filename[0] == 0) {
		strcpy(num_filename, __FILE__);
		char * p = num_filename + strlen(num_filename);
		while (*p != '/' && p > num_filename)
			--p;
		if (*p == '/')
			++p;
		strcpy(p, NUM_FILENAME_BASE);
	}
	return num_filename;
}

void access_by_char(const char * filename)
{
	int c;
	FILE * fp = fopen(filename, "r");
	if (!fp) {
		/* perror("failed to open file"); */
		/* err(EXIT_FAILURE, "failed to open ``%s''", filename); */
		/* error(EXIT_FAILURE, errno, "failed to open ``%s''", filename); */
		error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
			      "failed to open ``%s''", filename);
		return;
	}
	printf("\nread %s by char:\n", filename);
	while ((c = fgetc(fp)) != EOF)
		printf("<%c>", c);
	printf("%s\n", END_STR);
	fclose(fp);
}

void access_by_word(const char * filename)
{
	char s[MAX_WORD_LEN];
	FILE * fp = fopen(filename, "r");
	if (!fp) {
		error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
			      "failed to open ``%s''", filename);
		return;
	}
	printf("\nread %s by word:\n", filename);
	while (fscanf(fp, "%s", s) == 1)
		printf("<%s>", s);
	printf("%s\n", END_STR);
	fclose(fp);
}

void access_by_line(const char * filename)
{
	char s[LINE_MAX];
	FILE * fp = fopen(filename, "r");
	if (!fp) {
		error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
			      "failed to open ``%s''", filename);
		return;
	}
	printf("\nread %s by line:\n", filename);
	while (fgets(s, LINE_MAX, fp))
		printf("<%s>", s);
	printf("%s\n", END_STR);
	fclose(fp);
}

void access_by_num(const char * filename)
{
	int num;
	FILE * fp = fopen(filename, "r");
	if (!fp) {
		error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__,
			      "failed to open ``%s''", filename);
		return;
	}
	printf("\nread %s by num:\n", filename);
	while (fscanf(fp, "%d", &num) == 1) {
		if ((unsigned int)num == INT_MAX) /* @see http://en.wikipedia.org/wiki/Limits.h */
			perror("parsing error");
		printf("<%d>", num);
	}
	printf("%s\n", END_STR);
	fclose(fp);
}

int main(void)
{
	printf("\n%sAll about File I/O.%s\n", "\033[31m", "\033[0m");
	const char * text_filename = get_text_filename();
	printf("text filename: %s\n", text_filename);
	access_by_char(text_filename);
	access_by_word(text_filename);
	access_by_line(text_filename);

	const char * num_filename = get_num_filename();
	printf("num filename: %s\n", num_filename);
	access_by_num(num_filename);

        return 0;
}
