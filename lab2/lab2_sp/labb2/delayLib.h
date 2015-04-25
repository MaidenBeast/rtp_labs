/* delayLib.h - self-calibrating hard delay routines header file */

/*
modification history
--------------------
27Mar96,espin written.

$Id: delayLib.h,v 1.2 1999/10/25 18:22:05 borkhuis Exp $

*/

#ifndef __INCdelayLibh
#define __INCdelayLibh

#if defined(__STDC__) || defined(__cplusplus)
extern void delayUsec (unsigned int u);
extern void delayMsec (unsigned int m);
#else
extern void delayUsec ();
extern void delayMsec ();
#endif /* __STDC__ || __cplusplus */

#endif /* __INCdelayLibh */


