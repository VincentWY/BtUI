/****************************************************************************/
/****************************************************************************/

#ifdef	BTUI_EXPORT
#define	BTUI_API	__declspec( dllexport ) 
#else
#define	BTUI_API __declspec( dllimport ) 
#endif

#include "CxImageHead/ximage.h"
#include <vector>