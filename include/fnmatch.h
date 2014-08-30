#ifndef FNMATCH_H_81FC5C17_8B73_2C6E_4A94_3F68CCA3B7E1
#define FNMATCH_H_81FC5C17_8B73_2C6E_4A94_3F68CCA3B7E1

// makes '\' ordinary
#define FNM_NOESCAPE	0x01
// wildcard doesn't match '/'
#define FNM_PATHNAME	0x02
// match leading period
#define FNM_DOTMATCH	0x04
// case insensitive
#define FNM_CASEFOLD	0x08

#define FNM_NOMATCH		1

int fnmatch(const char* pattern, const char* string, int flags);

#endif // FNMATCH_H_81FC5C17_8B73_2C6E_4A94_3F68CCA3B7E1
