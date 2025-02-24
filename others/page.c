// linux
#include <stdio.h>
#include <unistd.h> /* sysconf(3) */

int main(void)
{
	printf("The page size for this system is %ld bytes.\n",
		sysconf(_SC_PAGESIZE)); /* _SC_PAGE_SIZE is OK too. */

	return 0;
}


// windows
// #include <stdio.h>
// #include <windows.h>

// int main(void)
// {
// 	SYSTEM_INFO si;
// 	GetSystemInfo(&si);

// 	printf("The page size for this system is %u bytes.\n", si.dwPageSize);

// 	return 0;
// }
