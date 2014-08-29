#include <stdio.h>
#include <stdlib.h>

#include "../fnmatch.h"

void
t(const char *pattern, const char *string, int flags, int expected_result)
{
	int r = fnmatch(pattern, string, flags);
	printf("%-27s %-36s %s %s\n",
		   pattern,
		   string,
		   r == expected_result ? "OK" : "FAILED",
		   r != 0 ? "no match" : "");

	if (r != expected_result) exit(1);
}

int main(int argc, char **argv)
{
	t("cat", "cat", 0, 0);
	t("cat", "ca", 0, FNM_NOMATCH);

	t("c?t", "cat", 0, 0);
	t("c??t", "cat", 0, FNM_NOMATCH);
	t("c*", "cats!", 0, 0);

	t("c*t", "c/a/b/t", 0, 0);

	t("cat", "CAT", 0,            FNM_NOMATCH);
	t("cat", "CAT", FNM_CASEFOLD, 0);

	return 0;
}
