#ifndef COMPILERSPECIFIC_H
#define COMPILERSPECIFIC_H

#if __GNUC__ < 4
	#error Your compiler is too old, you need GCC 4.4 or higher
#elif (__GNUC__ == 4) && (__GNUC_MINOR__ <= 3)
	#error Your compiler is too old, you need GCC 4.4 or higher
#elif (__GNUC__ == 4) && (__GNUC_MINOR__ > 3) && (__GNUC_MINOR__ < 6)
	#define nullptr 0
#endif

#endif // COMPILERSPECIFIC_H
