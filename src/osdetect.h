#ifndef OS_DETECT_H
#define OS_DETECT_H

#if defined(_WIN32) || defined(_WIN64)
	#define DC_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__) && defined(TARGET_OS_MAC)
	#define DC_PLATFORM_MAC_OS
#elif defined(__linux__)
	#define DC_PLATFORM_LINUX
#else
	#define DC_PLATFORM_OTHER
	#pragma message("This platform is not officially supported.")
#endif

#endif // OS_DETECT_H