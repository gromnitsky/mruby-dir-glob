#include <string.h>
#include <ctype.h>

#include "fnmatch.h"

#define ISUPPER(c) isupper((unsigned char)(c))

#define downcase(c) (nocase && ISUPPER(c) ? tolower(c) : (c))
#define compare(c1, c2) (((unsigned char)(c1)) - ((unsigned char)(c2)))
#define Next(p) ((p) + 1)
#define Inc(p) (++(p))
#define Compare(p1, p2) (compare(downcase(*(p1)), downcase(*(p2))))

static char *
bracket(const char* p, const char* s, int flags) {
	const int nocase = flags & FNM_CASEFOLD;
	const int escape = !(flags & FNM_NOESCAPE);

	int ok = 0, nope = 0;

	if(*p == '!' || *p == '^') {
		nope = 1;
		p++;
	}

	while(*p != ']') {
		const char *t1 = p;
		if(escape && *t1 == '\\') t1++;
		if(!*t1) return NULL;

		p = Next(t1);
		if(p[0] == '-' && p[1] != ']') {
		const char *t2 = p + 1;
		if(escape && *t2 == '\\') t2++;
		if(!*t2) return NULL;

		p = Next(t2);
		if(!ok && Compare(t1, s) <= 0 && Compare(s, t2) <= 0) ok = 1;
		} else {
		if(!ok && Compare(t1, s) == 0) ok = 1;
		}
	}

	return ok == nope ? NULL : (char *)p + 1;
}

/* If FNM_PATHNAME is set, only path element will be matched. (upto '/' or '\0')
   Otherwise, entire string will be matched.
   End marker itself won't be compared.
   And if function succeeds, *pcur reaches end marker.
*/
#define UNESCAPE(p) (escape && *(p) == '\\' ? (p) + 1 : (p))
#define ISEND(p) (!*(p) || (pathname && *(p) == '/'))
#define RETURN(val) return *pcur = p, *scur = s, (val);

static int
fnmatch_helper(const char** pcur, const char** scur, int flags) {
	const int period = !(flags & FNM_DOTMATCH);
	const int pathname = flags & FNM_PATHNAME;
	const int escape = !(flags & FNM_NOESCAPE);
	const int nocase = flags & FNM_CASEFOLD;

	const char *ptmp = 0;
	const char *stmp = 0;

	const char *p = *pcur;
	const char *s = *scur;

	if(period && *s == '.' && *UNESCAPE(p) != '.') /* leading period */
		RETURN(FNM_NOMATCH);

	while(1) {
		switch(*p) {
		case '*':
			do { p++; } while (*p == '*');
			if(ISEND(UNESCAPE(p))) {
				p = UNESCAPE(p);
				RETURN(0);
			}
			if(ISEND(s)) RETURN(FNM_NOMATCH);

			ptmp = p;
			stmp = s;
			continue;

		case '?':
			if(ISEND(s)) RETURN(FNM_NOMATCH);
			p++;
			Inc(s);
			continue;

		case '[': {
			const char *t;
			if(ISEND(s)) RETURN(FNM_NOMATCH);
			if((t = bracket(p + 1, s, flags)) != 0) {
				p = t;
				Inc(s);
				continue;
			}
			goto failed;
		}
		}

		/* ordinary */
		p = UNESCAPE(p);
		if(ISEND(s)) RETURN(ISEND(p) ? 0 : FNM_NOMATCH);
		if(ISEND(p)) goto failed;
		if(Compare(p, s) != 0) goto failed;

		Inc(p);
		Inc(s);
		continue;

	failed: /* try next '*' position */
		if(ptmp && stmp) {
			p = ptmp;
			Inc(stmp); /* !ISEND(*stmp) */
			s = stmp;
			continue;
		}
		RETURN(FNM_NOMATCH);
	}
}

int
fnmatch(const char* p, const char* s, int flags) {
	const int period = !(flags & FNM_DOTMATCH);
	const int pathname = flags & FNM_PATHNAME;

	const char *ptmp = 0;
	const char *stmp = 0;

	if(pathname) {
	    while(1) {
			if(p[0] == '*' && p[1] == '*' && p[2] == '/') {
				do { p += 3; } while (p[0] == '*' && p[1] == '*' && p[2] == '/');
				ptmp = p;
				stmp = s;
			}
			if(fnmatch_helper(&p, &s, flags)) {
				while(*s && *s != '/') Inc(s);
				if(*p && *s) {
					p++;
					s++;
					continue;
				}
				if(!*p && !*s)
					return 0;
			}
			/* failed : try next recursion */
			if(ptmp && stmp && !(period && *stmp == '.')) {
				while(*stmp && *stmp != '/') Inc(stmp);
				if(*stmp) {
					p = ptmp;
					stmp++;
					s = stmp;
					continue;
				}
			}
			return FNM_NOMATCH;
        }
	} else {
        return fnmatch_helper(&p, &s, flags);
	}
}

/*
 * Local Variables:
 * tab-width: 4
 * End:
 */
