#include <stdio.h>
#include <stdlib.h>

#include "../fnmatch.h"

#define OK 0

void
t(const char *pattern, const char *string, int flags, int expected_result)
{
	int r = fnmatch(pattern, string, flags);
	printf("%-25s %-34s %s %#x %s\n",
		   pattern,
		   string,
		   r == expected_result ? "OK" : "FAILED",
		   flags,
		   r != 0 ? "no match" : "");

	if (r != expected_result) exit(1);
}

int main(int argc, char **argv)
{
	t("cat", "cat", 0, OK);
	t("cat", "ca", 0, FNM_NOMATCH);

	t("c?t", "cat", 0, OK);
	t("c??t", "cat", 0, FNM_NOMATCH);
	t("c*", "cats!", 0, OK);

	t("c*t", "c/a/b/t", 0, OK);

	t("cat", "CAT", 0,            FNM_NOMATCH);
	t("cat", "CAT", FNM_CASEFOLD, OK);

	t("?", "/", 0, OK);
	t("?", "/", FNM_PATHNAME, FNM_NOMATCH);

	t("\\?", "?", 0, OK);
	t("\\a", "a", 0, OK);
	t("\\a", "a", FNM_NOESCAPE, FNM_NOMATCH);

	t("*", ".profile", 0, FNM_NOMATCH);
	t(".*", ".profile", 0, OK);
	t("*", ".profile", FNM_DOTMATCH, OK);

	t("*.rb", "lib/song/karaoke.rb", 0, OK);

	return 0;
}
