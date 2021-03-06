/* ctors and dtors arrays -- to be used by runtime system */
/*   to call static constructors and destructors          */
/*                                                        */
/* NOTE: Use a C compiler to compile this file. If you    */
/*       are using GNU C++, be sure to use compile this   */
/*       file using a GNU compiler with the               */
/*       -fdollars-in-identifiers flag.                   */

char __dso_handle = 0;

extern void (*_ctors[])();
void (*_ctors[])() =
    {
    0
    };

extern void (*_dtors[])();
void (*_dtors[])() =
    {
    0
    };

/* build variables */
#ifdef __GNUC__
__asm("	.section \".wrs_build_vars\",\"a\"");
#endif
#ifdef __DCC__
__asm("	.section \".wrs_build_vars\",\"r\"");
#endif
__asm(" .ascii \"tag SMP 0\"");
__asm(" .byte 0");
__asm("	.ascii \"end\"");
__asm("	.byte 0");
__asm("	.previous");

/* OSCB static tables */
#include <oscb.h>

static const uint64_t oscbOSFlags = OSCB_OS_FLAGS_VALUE;
static const uint64_t oscbCapProtocols = OSCB_VX_CAP_PROTOCOLS;
static const uint64_t oscbCapKernel = OSCB_VX_CAP_KERNEL;
static const char oscbOSSig[] = __DATE__ " " __TIME__;
static const char oscbOSBuildPath[] = "";

static const struct OSCB_key_value oscb_keyval_table[] =
    {
    {OSCB_OS_SIGNATURE, (void*) oscbOSSig},
    {OSCB_OS_BUILD_PATH, (void*) oscbOSBuildPath},
    {OSCB_TARGET_SIGNATURE, (void*) &sysProcNum},
    {OSCB_OS_BUILD_HOST, (void*) sysBootHost},
    {OSCB_OS_HEARTBEAT, (void*) &vxAbsTicks},
    {OSCB_OS_FLAGS, (void*) &oscbOSFlags},
#if defined(INCLUDE_LOADER) || defined(INCLUDE_WDB)
    {OSCB_VX_BUILD_VARS, (void*) _wrs_build_vars},
    {OSCB_VX_BUILD_VARS_END, (void*) _wrs_build_vars_end},
#endif
    {OSCB_VX_BOOTLINE, (void*) &sysBootLine},
    {OSCB_VX_EXC_RECORD, (void*) &sysExcMsg},
    {OSCB_VX_CAPABILITY_SET_PROTOCOLS, (void*) &oscbCapProtocols},
    {OSCB_VX_CAPABILITY_SET_KERNEL, (void*) &oscbCapKernel},
#ifdef INCLUDE_WINDVIEW
    {OSCB_VX_SV_LOGS, (void*) &wvCurrentLogList},
#endif
    {OSCB_TERM, (void*) 0}
    };

const struct OSCB_head __oscb_hdr =
    {
    {OSCB_SIGN_0, OSCB_SIGN_1, OSCB_SIGN_2, OSCB_SIGN_3},
    OSCB_ENDIAN,
    OSCB_PRO_MODEL,
    OSCB_VERSION,
    OSCB_VENDOR_ID,
    RUNTIME_NAME,
    RUNTIME_VERSION,
    (void*) &oscb_keyval_table
    };
