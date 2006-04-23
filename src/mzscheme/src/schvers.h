
/* For unusual compilation modes, we change the version string to
   avoid confusing .zo mismatches: */
#ifdef MZSCHEME_SOMETHING_OMITTED
# define _MZ_SPECIAL_TAG "-special"
#else
# define _MZ_SPECIAL_TAG ""
#endif


#define MZSCHEME_VERSION_MAJOR 301
#define MZSCHEME_VERSION_MINOR 13

#define MZSCHEME_VERSION "301.13" _MZ_SPECIAL_TAG
