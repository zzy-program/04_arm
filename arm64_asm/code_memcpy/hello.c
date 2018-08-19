#include <stdio.h>

#define CPY_PRE_TEST_LEN 16
#define CPY_LARGE_LEN    1024*1024

extern void *memset(void *s, int c, size_t n);

int main()
{
	char *src_pre_test = "1234567890ABCDE";
	char dst_pre_test[CPY_PRE_TEST_LEN];
	int len = CPY_PRE_TEST_LEN - 1;

	// test memcpy_base function
	memset(dst_pre_test, 0, CPY_PRE_TEST_LEN);
	memcpy_base(dst_pre_test, src_pre_test, len);
	printf("memcpy_base: %s\n", dst_pre_test);
	if(!strcmp(src_pre_test, dst_pre_test)) {
		printf("memcpy_base work!\n");	
	}

	// test memcpy_neon function
	memset(dst_pre_test, 0, CPY_PRE_TEST_LEN);
	memcpy_neon(dst_pre_test, src_pre_test, len);
	printf("memcpy_neon: %s\n", dst_pre_test);
	if(!strcmp(src_pre_test, dst_pre_test)) {
		printf("memcpy_neon work!\n");	
	}

	// todo test performance

	return 0;	
}