/* 
* last modified: 17.06.2003   
*/

#ifndef VGL_BASIC_H
#define VGL_BASIC_H

#define VGL_NAMESPACE
#define LEDA_SUPPORT

#ifdef  VGL_NAMESPACE
#define VGL_BEGIN_NAMESPACE namespace vgl {
#define VGL_END_NAMESPACE   }
#else
#define VGL_BEGIN_NAMESPACE 
#define VGL_END_NAMESPACE   
#endif

#ifdef  LEDA_SUPPORT
#define LEDA_COLOR_SUPPORT
#define LEDA_D3_TRANSFORM_SUPPORT
#endif

#include <LEDA/system/basic.h>

//----------------------------------------------------------------------------
// compiler defines
//----------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER > 1300)
#define MSC_VER_6
#endif

//----------------------------------------------------------------------------
// error handling
//----------------------------------------------------------------------------

#define VGL_ERROR_HANDLER(ERROR) /*leda::LEDA_EXCEPTION(99,ERROR) */


#endif
