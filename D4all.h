/* d4all.h   (c)Copyright Sequiter Software Inc., 1988-1997. All rights reserved. */

#ifndef D4ALL_INC
#define D4ALL_INC

/**********************************************************************/
/**********            USER SWITCH SETTINGS AREA            ***********/

/* CodeBase configuration */
/* #define S4CLIENT */
   #define S4STAND_ALONE

/* Index File compatibility options */
#ifndef S4CLIENT
/* #define S4CLIPPER */
   #define S4FOX
/* #define S4MDX     */
#endif

#ifdef S4FOX
   /* FoxPro collating sequence support (select none, some or all) */
   #define S4GENERAL   /* Supports German FoxPro 2.5a and Visual FoxPro with general collating sequences */
   
   /* FoxPro codepage support (select none, some or all) */
/* #define S4CODEPAGE_437 */  /* U.S. MS-DOS CodePage */
   #define S4CODEPAGE_1252  /* WINDOWS ANSI CodePage */
#endif

/* Output selection (alternatives to default) */
/* #define S4CODE_SCREENS */
/* #define S4CONSOLE */

/* Specify Library Type (choose one) */
   #define S4STATIC
/* #define S4DLL     */
/* #define S4DLL_BUILD */

/* Choose Operating System */
/* #define S4DOS        */
/* #define S4OS2        */   /* for OS/2 */
/* #define S4WIN16      */   /* for Windows 3.x */
   #define S4WIN32           /* for Windows NT and Windows 95 */
/* #define S4UNIX       */   /* requires CodeBase Portability version */
/* #define S4MACINTOSH  */   /* requires CodeBase Mac version */
/* #define S4PASCAL_WIN */   /* requires CodePascal version */

/* Selected default communications option */
/* #define S4LOCAL */
/* #define S4SPX */
#define S4WINSOCK


/* Alterable CodeBase Global Defines */
#define DEF4SERVER_ID "S4SERVER"
#define DEF4PROCESS_ID "23165"

/* General Configuration Options */
/* #define S4CB51         */
/* #define S4LOCK_HOOK    */
/* #define S4MAX          */
/* #define S4SAFE         */
/* #define S4TIMEOUT_HOOK */

/* Error Configuration Options */
/* #define E4ANALYZE    */
/* #define E4DEBUG      */
/* #define E4HOOK       */
/* #define E4LINK       */
/* #define E4MISC       */
/* #define E4OFF        */
/* #define E4OFF_STRING */
   #define E4PARM_HIGH
   #define E4PAUSE
/* #define E4STOP       */
   #define E4STOP_CRITICAL
/* #define E4MAC_ALERT 4444 */

#ifndef S4CLIENT
/* Library Reducing Switches */
/* #define S4OFF_INDEX    */
/* #define S4OFF_MEMO     */
/* #define S4OFF_MULTI    */
/* #define S4OFF_OPTIMIZE */
/* #define S4OFF_REPORT   */
/* #define S4OFF_TRAN     */
/* #define S4OFF_WRITE    */
/* #define S4OFF_THREAD   */
#endif

/* Spoken Language Switches */
   #define S4ANSI         
/* #define S4DICTIONARY   */
/* #define S4FINNISH      */
/* #define S4FRENCH       */
/* #define S4GERMAN       */
/* #define S4NORWEGIAN    */
/* #define S4SCANDINAVIAN */
/* #define S4SWEDISH      */

/**********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef S4UNIX
   #include "p4port.h"
#else
   #ifdef S4MACINTOSH
      #include "p4port.h"
   #else
      #define S4WINTEL   /*This will include DOS and OS2 also */
      #include <limits.h>
      #include <io.h>
      #include <stdarg.h>
      #ifdef S4OS2
         #include <os2.h>
         #include <direct.h>
      #else
         #include <dos.h>
      #endif
   #endif
#endif

#include <time.h>

#ifdef S4WIN16
   #include <windows.h>
#else
   #ifdef S4WIN32
      #include <windows.h>
   #endif
#endif

#include "d4defs.h"
#include "d4data.h"
#include "d4declar.h"
#include "d4inline.h"
#include "f4flag.h"
#include "e4expr.h"
#include "s4sort.h"
#include "e4string.h"
#include "e4error.h"

#include "o4opt.h"

#include "c4com.h"
#include "c4trans.h"

#ifdef S4SERVER
   #include "d4server.h"
#endif

#ifdef S4LOCAL
   #include "c4coml.h"
#else
   #ifdef S4SPX
      #include "c4comspx.h"
   #endif
   #ifdef S4WINSOCK
      #include "c4comws.h"
   #endif
#endif

#include "r4relate.h"

#ifdef S4VBX
   #ifndef S4CONTROLS
   #endif
   #include "ctrl4vbx.h"
#else
   #ifdef S4CONTROLS
      #include "ctrl4.h"
   #endif
#endif

#ifndef S4OFF_REPORT
   #include "r4report.h"
#endif

#ifdef S4CODE_SCREENS
   #include "w4.h"
#endif

#define S4VERSION 6300

#endif /* D4ALL_INC */
