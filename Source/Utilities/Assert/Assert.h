#pragma once

#include "Lumen/Lumen.h"
#include <assert.h>

#ifdef LUMEN_ASSERTS

	#define LUMEN_ASSERT(condition, message) assert(condition);

	#ifdef _DEBUG
		#define LUMEN_DEBUG_ASSERT(condition, message) assert(condition);
	#else
		#define LUMEN_DEBUG_ASSERT(condition, message)
	#endif

#else
	
	#define LUMEN_ASSERT(condition, message)
	#define LUMEN_DEBUG_ASSERT(condition, message)

#endif