
#ifndef COLLADA_PARSER_CONFIG_H_
#define COLLADA_PARSER_CONFIG_H_



/* helpers for shared library support */
#if defined _WIN32 || defined __CYGWIN__
	#define COLLADA_PARSER_HELPER_DLL_IMPORT __declspec(dllimport)
	#define COLLADA_PARSER_HELPER_DLL_EXPORT __declspec(dllexport)
	#define COLLADA_PARSER_HELPER_DLL_LOCAL
#else
	#if __GNUC__ >= 4
		#define COLLADA_PARSER_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
		#define COLLADA_PARSER_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
		#define COLLADA_PARSER_HELPER_DLL_LOCAL  __attribute__ ((visibility("hidden")))
	#else
		#define COLLADA_PARSER_HELPER_DLL_IMPORT
		#define COLLADA_PARSER_HELPER_DLL_EXPORT
		#define COLLADA_PARSER_HELPER_DLL_LOCAL
	#endif
#endif



/* define COLLADA_PARSER_API and COLLADA_PARSER_LOCAL */
/* ColladaParser is a shared library */
#ifdef COLLADA_PARSER_DLL
	/* ColladaParser is being built, export it */
	#ifdef COLLADA_PARSER_DLL_EXPORTS
		#define COLLADA_PARSER_API COLLADA_PARSER_HELPER_DLL_EXPORT
	/* ColladaParser is being used, import it */
	#else
		#define COLLADA_PARSER_API COLLADA_PARSER_HELPER_DLL_IMPORT
	#endif /* COLLADA_PARSER_DLL_EXPORTS */
	
	#define COLLADA_PARSER_LOCAL COLLADA_PARSER_HELPER_DLL_LOCAL

/* ColladaParser is not a shared library */
#else
	#define COLLADA_PARSER_API
	#define COLLADA_PARSER_LOCAL
#endif /* COLLADA_PARSER_DLL */



#endif /* COLLADA_PARSER_CONFIG_H_ */
