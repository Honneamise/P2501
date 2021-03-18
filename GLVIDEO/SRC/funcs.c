#include "../glvideo.h"

/**********/
void EnableDebug()
{
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	printf("[%s]: Memory leaks debug enabled\n",__func__);
#endif
}

/**********/
void DisableDebug()
{
#if defined(_MSC_VER) && defined(_DEBUG)
	int flag = 0;

	flag &= ~_CRTDBG_ALLOC_MEM_DF;
	_CrtSetDbgFlag(flag);

	flag &= ~_CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);

	printf("[%s]: Memory leaks debug disabled\n",__func__);
#endif
}

/**********/
void ExitError(char *str, ...)
{
#if defined(_WIN32)
	_flushall();
#else
	flushall();
#endif

	if (str != NULL)
	{
		va_list argp;
		va_start(argp, str);
		vfprintf(stdout, str, argp);
		va_end(argp);
	};

	DisableDebug();
	
	exit(EXIT_FAILURE);
}

/**********/
void* Calloc(size_t count, size_t size)
{
	void* p = calloc(count, size);

	if (p == NULL) { ExitError("[%s]: Allocation failed\n", __func__); };

	return p;
}

/**********/
void Free(void* p)
{
	if (p != NULL) { free(p); };
}

