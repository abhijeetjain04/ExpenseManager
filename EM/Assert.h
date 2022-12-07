#pragma once

#include <assert.h>

#ifdef _DEBUG
	#define DBG_ASSERT(x) assert(x)
#else
	#define DBG_ASSERT(x) 
#endif

