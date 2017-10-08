//-----------------------------------------------------------------------------
// configuration  macros
//-----------------------------------------------------------------------------

/*
#ifndef LEDA_MULTI_THREAD
#define LEDA_MULTI_THREAD
#endif
*/

#if defined(LEDA_MULTI_THREAD)
#include <LEDA/internal/config-mt.h>
#else
#include <LEDA/internal/config-st.h>
#endif


