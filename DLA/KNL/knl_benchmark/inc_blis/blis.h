

#ifndef BLIS_H
#define BLIS_H


// Allow C++ users to include this header file in their source code. However,
// we make the extern "C" conditional on whether we're using a C++ compiler,
// since regular C compilers don't understand the extern "C" construct.
#ifdef __cplusplus
extern "C" {
#endif


// -- BLIS configuration definition --

// NOTE: We include bli_config.h first because there might be something
// defined there that is needed within one of the system headers. A good
// example: posix_memalign() needs _GNU_SOURCE on GNU systems (I think).
// 
// PLEASE DON'T CHANGE THE ORDER IN WHICH HEADERS ARE INCLUDED UNLESS YOU
// KNOW WHAT YOU ARE DOING.

// begin bli_config.h


#ifndef BLIS_CONFIG_H
#define BLIS_CONFIG_H

// Enabled configuration "family" (config_name)
#define BLIS_FAMILY_KNL


// Enabled sub-configurations (config_list)
#define BLIS_CONFIG_KNL


// Enabled kernel sets (kernel_list)
#define BLIS_KERNELS_KNL
#define BLIS_KERNELS_ZEN


#if 1
#define BLIS_ENABLE_OPENMP
#endif

#if 0
#define BLIS_ENABLE_PTHREADS
#endif

#if 1
#define BLIS_ENABLE_PACKBUF_POOLS
#endif

#if 0 == 64
#define BLIS_INT_TYPE_SIZE 64
#elif 0 == 32
#define BLIS_INT_TYPE_SIZE 32
#else
// determine automatically
#endif

#if 32 == 64
#define BLIS_BLAS2BLIS_INT_TYPE_SIZE 64
#elif 32 == 32
#define BLIS_BLAS2BLIS_INT_TYPE_SIZE 32
#else
// determine automatically
#endif

#ifndef BLIS_ENABLE_BLAS2BLIS
#ifndef BLIS_DISABLE_BLAS2BLIS
#if 1
#define BLIS_ENABLE_BLAS2BLIS
#else
#define BLIS_DISABLE_BLAS2BLIS
#endif
#endif
#endif

#ifndef BLIS_ENABLE_CBLAS
#ifndef BLIS_DISABLE_CBLAS
#if 1
#define BLIS_ENABLE_CBLAS
#else
#define BLIS_DISABLE_CBLAS
#endif
#endif
#endif

#if 1
#define BLIS_ENABLE_MEMKIND
#else
#define BLIS_DISABLE_MEMKIND
#endif

#endif
// end bli_config.h
// begin bli_config_macro_defs.h


#ifndef BLIS_CONFIG_MACRO_DEFS_H
#define BLIS_CONFIG_MACRO_DEFS_H


// -- INTEGER PROPERTIES -------------------------------------------------------

// The bit size of the integer type used to track values such as dimensions,
// strides, diagonal offsets. A value of 32 results in BLIS using 32-bit signed
// integers while 64 results in 64-bit integers. Any other value results in use
// of the C99 type "long int". Note that this ONLY affects integers used
// internally within BLIS as well as those exposed in the native BLAS-like BLIS
// interface.
#ifndef BLIS_INT_TYPE_SIZE
#ifdef BLIS_ARCH_64
#define BLIS_INT_TYPE_SIZE               64
#else
#define BLIS_INT_TYPE_SIZE               32
#endif
#endif


// -- FLOATING-POINT PROPERTIES ------------------------------------------------

// Enable use of built-in C99 "float complex" and "double complex" types and
// associated overloaded operations and functions? Disabling results in
// scomplex and dcomplex being defined in terms of simple structs.
// NOTE: AVOID USING THIS FEATURE. IT IS PROBABLY BROKEN.
#ifdef BLIS_ENABLE_C99_COMPLEX
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif


// -- MULTITHREADING -----------------------------------------------------------

// Enable multithreading via POSIX threads.
#ifdef BLIS_ENABLE_PTHREADS
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif

// Enable multithreading via OpenMP.
#ifdef BLIS_ENABLE_OPENMP
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif

// Perform a sanity check to make sure the user doesn't try to enable
// both OpenMP and pthreads.
#if defined ( BLIS_ENABLE_OPENMP ) && \
    defined ( BLIS_ENABLE_PTHREADS )
  #error "BLIS_ENABLE_OPENMP and BLIS_ENABLE_PTHREADS may not be simultaneously defined."
#endif

// Here, we define BLIS_ENABLE_MULTITHREADING if either OpenMP
// or pthreads are enabled. This macro is useful in situations when
// we want to detect use of either OpenMP or pthreads (as opposed
// to neither being used).
#if defined ( BLIS_ENABLE_OPENMP ) || \
    defined ( BLIS_ENABLE_PTHREADS )
  #define BLIS_ENABLE_MULTITHREADING
#endif


// -- MISCELLANEOUS OPTIONS ----------------------------------------------------

// Do NOT require the cross-blocksize constraints. That is, do not enforce
// MC % NR = 0 and NC % MR = 0 in bli_kernel_macro_defs.h. These are ONLY
// needed when implementing trsm_r by allowing the right-hand matrix B to
// be triangular.
#ifndef BLIS_RELAX_MCNR_NCMR_CONSTRAINTS
  #define BLIS_RELAX_MCNR_NCMR_CONSTRAINTS
#endif

// Stay initialized after auto-initialization, unless and until the user
// explicitly calls bli_finalize().
#ifdef BLIS_DISABLE_STAY_AUTO_INITIALIZED
  #undef BLIS_ENABLE_STAY_AUTO_INITIALIZED
#else
  // Default behavior is enabled.
  #undef  BLIS_ENABLE_STAY_AUTO_INITIALIZED // In case user explicitly enabled.
  #define BLIS_ENABLE_STAY_AUTO_INITIALIZED
#endif


// -- BLAS COMPATIBILITY LAYER -------------------------------------------------

// Enable the BLAS compatibility layer?
#ifdef BLIS_DISABLE_BLAS2BLIS
  #undef BLIS_ENABLE_BLAS2BLIS
#else
  // Default behavior is enabled.
  #undef  BLIS_ENABLE_BLAS2BLIS // In case user explicitly enabled.
  #define BLIS_ENABLE_BLAS2BLIS
#endif

// The bit size of the integer type used to track values such as dimensions and
// leading dimensions (ie: column strides) within the BLAS compatibility layer.
// A value of 32 results in the compatibility layer using 32-bit signed integers
// while 64 results in 64-bit integers. Any other value results in use of the
// C99 type "long int". Note that this ONLY affects integers used within the
// BLAS compatibility layer.
#ifndef BLIS_BLAS2BLIS_INT_TYPE_SIZE
#define BLIS_BLAS2BLIS_INT_TYPE_SIZE     32
#endif


// -- CBLAS COMPATIBILITY LAYER ------------------------------------------------

// Enable the CBLAS compatibility layer?
// NOTE: Enabling CBLAS will automatically enable the BLAS compatibility layer
// regardless of whether or not it was explicitly enabled above. Furthermore,
// the CBLAS compatibility layer will use the integer type size definition
// specified above when defining the size of its own integers (regardless of
// whether the BLAS layer was enabled directly or indirectly).
#ifdef BLIS_ENABLE_CBLAS
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif


#endif

// end bli_config_macro_defs.h


// -- System headers --

// begin bli_system.h


#ifndef BLIS_SYSTEM_H
#define BLIS_SYSTEM_H

#include <stdio.h> // skipped
#include <stdlib.h> // skipped
#include <math.h> // skipped
#include <string.h> // skipped
#include <stdarg.h> // skipped
#include <float.h> // skipped
#include <errno.h> // skipped

// Determine if we are on a 64-bit or 32-bit architecture
#if defined(_M_X64) || defined(__x86_64) || defined(__aarch64__) || \
    defined(_ARCH_PPC64)
#define BLIS_ARCH_64
#else
#define BLIS_ARCH_32
#endif

// Determine the target operating system
#if defined(_WIN32) || defined(__CYGWIN__)
#define BLIS_OS_WINDOWS 1
#elif defined(__APPLE__) || defined(__MACH__)
#define BLIS_OS_OSX 1
#elif defined(__ANDROID__)
#define BLIS_OS_ANDROID 1
#elif defined(__linux__)
#define BLIS_OS_LINUX 1
#elif defined(__bgq__)
#define BLIS_OS_BGQ 1
#elif defined(__bg__)
#define BLIS_OS_BGP 1
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
      defined(__bsdi__) || defined(__DragonFly__)
#define BLIS_OS_BSD 1
#elif defined(EMSCRIPTEN)
#define BLIS_OS_EMSCRIPTEN
#else
#error "Cannot determine operating system"
#endif

// A few changes that may be necessary in Windows environments.
#if BLIS_OS_WINDOWS

  // Include Windows header file.
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
#include <windows.h> // skipped

  // Undefine attribute specifiers in Windows.
  #define __attribute__(x)

  // Undefine restrict.
  #define restrict

#endif

// time.h provides clock_gettime().
#if BLIS_OS_WINDOWS
#include <time.h> // skipped
#elif BLIS_OS_OSX
#include <mach/mach_time.h> // skipped
#else
#include <sys/time.h> // skipped
#include <time.h> // skipped
#endif

// hbwmalloc.h provides hbw_malloc() and hbw_free() on systems with
// libmemkind. But disable use of libmemkind if BLIS_DISABLE_MEMKIND
// was explicitly defined.
#ifdef BLIS_DISABLE_MEMKIND
  #undef BLIS_ENABLE_MEMKIND
#endif
#ifdef BLIS_ENABLE_MEMKIND
#include <hbwmalloc.h> // skipped
#endif


#endif
// end bli_system.h


// -- Common BLIS definitions --

// begin bli_type_defs.h


#ifndef BLIS_TYPE_DEFS_H
#define BLIS_TYPE_DEFS_H


//
// -- BLIS basic types ---------------------------------------------------------
//

#ifdef __cplusplus
  // For C++, include stdint.h.
#include <stdint.h> // skipped
#elif __STDC_VERSION__ >= 199901L
  // For C99 (or later), include stdint.h.
#include <stdint.h> // skipped
#else
  // When stdint.h is not available, manually typedef the types we will use.
  #ifdef _WIN32
    typedef          __int32  int32_t;
    typedef unsigned __int32 uint32_t;
    typedef          __int64  int64_t;
    typedef unsigned __int64 uint64_t;
  #else
    #error "Attempting to compile on pre-C99 system without stdint.h."
  #endif
#endif

// -- General-purpose integers --

// Define integer types depending on what size integer was requested.
#if   BLIS_INT_TYPE_SIZE == 32
typedef           int32_t  gint_t;
typedef          uint32_t guint_t;
#elif BLIS_INT_TYPE_SIZE == 64
typedef           int64_t  gint_t;
typedef          uint64_t guint_t;
#else
typedef   signed long int  gint_t;
typedef unsigned long int guint_t;
#endif

// -- Boolean type --

typedef  gint_t  bool_t;


// -- Boolean values --

#ifndef TRUE
  #define TRUE  1
#endif

#ifndef FALSE
  #define FALSE 0
#endif


// -- Special-purpose integers --

// This cpp guard provides a temporary hack to allow libflame
// interoperability with BLIS.
#ifndef _DEFINED_DIM_T
#define _DEFINED_DIM_T
typedef  gint_t  dim_t;      // dimension type
#endif
typedef  gint_t  inc_t;      // increment/stride type
typedef  gint_t  doff_t;     // diagonal offset type
typedef guint_t  siz_t;      // byte size type
typedef guint_t  objbits_t;  // object information bit field

// -- Real types --

// Define the number of floating-point types supported, and the size of the
// largest type.
#define BLIS_NUM_FP_TYPES   4
#define BLIS_MAX_TYPE_SIZE  sizeof(dcomplex)

// There are some places where we need to use sizeof() inside of a C
// preprocessor #if conditional, and so here we define the various sizes
// for those purposes.
#define BLIS_SIZEOF_S      4  // sizeof(float)
#define BLIS_SIZEOF_D      8  // sizeof(double)
#define BLIS_SIZEOF_C      8  // sizeof(scomplex)
#define BLIS_SIZEOF_Z      16 // sizeof(dcomplex)

// -- Complex types --

#ifdef BLIS_ENABLE_C99_COMPLEX

	#if __STDC_VERSION__ >= 199901L
#include <complex.h> // skipped

		// Typedef official complex types to BLIS complex type names.
		typedef  float complex scomplex;
		typedef double complex dcomplex;
	#else
		#error "Configuration requested C99 complex types, but C99 does not appear to be supported."
	#endif

#else // ifndef BLIS_ENABLE_C99_COMPLEX

	// This cpp guard provides a temporary hack to allow libflame
	// interoperability with BLIS.
	#ifndef _DEFINED_SCOMPLEX
	#define _DEFINED_SCOMPLEX
	typedef struct
	{
		float  real;
		float  imag;
	} scomplex;
	#endif

	// This cpp guard provides a temporary hack to allow libflame
	// interoperability with BLIS.
	#ifndef _DEFINED_DCOMPLEX
	#define _DEFINED_DCOMPLEX
	typedef struct
	{
		double real;
		double imag;
	} dcomplex;
	#endif

#endif // BLIS_ENABLE_C99_COMPLEX

// -- Atom type --

// Note: atom types are used to hold "bufferless" scalar object values. Note
// that it needs to be as large as the largest possible scalar value we might
// want to hold. Thus, for now, it is a dcomplex.
typedef dcomplex atom_t;

// -- Fortran-77 types --

// Note: These types are typically only used by BLAS compatibility layer, but
// we must define them even when the compatibility layer isn't being built
// because they also occur in bli_slamch() and bli_dlamch().

// Define f77_int depending on what size of integer was requested.
#if   BLIS_BLAS2BLIS_INT_TYPE_SIZE == 32
typedef int32_t   f77_int;
#elif BLIS_BLAS2BLIS_INT_TYPE_SIZE == 64
typedef int64_t   f77_int;
#else
typedef long int  f77_int;
#endif

typedef char      f77_char;
typedef float     f77_float;
typedef double    f77_double;
typedef scomplex  f77_scomplex;
typedef dcomplex  f77_dcomplex;


//
// -- BLIS info bit field offsets ----------------------------------------------
//



#define BLIS_DATATYPE_SHIFT                0
#define   BLIS_DOMAIN_SHIFT                0
#define   BLIS_PRECISION_SHIFT             1
#define BLIS_CONJTRANS_SHIFT               3
#define   BLIS_TRANS_SHIFT                 3
#define   BLIS_CONJ_SHIFT                  4
#define BLIS_UPLO_SHIFT                    5
#define   BLIS_UPPER_SHIFT                 5
#define   BLIS_DIAG_SHIFT                  6
#define   BLIS_LOWER_SHIFT                 7
#define BLIS_UNIT_DIAG_SHIFT               8
#define BLIS_INVERT_DIAG_SHIFT             9
#define BLIS_TARGET_DT_SHIFT               10
#define BLIS_EXECUTION_DT_SHIFT            13
#define BLIS_PACK_SCHEMA_SHIFT             16
#define   BLIS_PACK_RC_SHIFT               16
#define   BLIS_PACK_PANEL_SHIFT            17
#define   BLIS_PACK_FORMAT_SHIFT           18
#define   BLIS_PACK_SHIFT                  22
#define BLIS_PACK_REV_IF_UPPER_SHIFT       23
#define BLIS_PACK_REV_IF_LOWER_SHIFT       24
#define BLIS_PACK_BUFFER_SHIFT             25
#define BLIS_STRUC_SHIFT                   27

//
// -- BLIS info bit field masks ------------------------------------------------
//

#define BLIS_DATATYPE_BITS                 ( 0x7  << BLIS_DATATYPE_SHIFT )
#define   BLIS_DOMAIN_BIT                  ( 0x1  << BLIS_DOMAIN_SHIFT )
#define   BLIS_PRECISION_BIT               ( 0x1  << BLIS_PRECISION_SHIFT )
#define BLIS_CONJTRANS_BITS                ( 0x3  << BLIS_CONJTRANS_SHIFT )
#define   BLIS_TRANS_BIT                   ( 0x1  << BLIS_TRANS_SHIFT )
#define   BLIS_CONJ_BIT                    ( 0x1  << BLIS_CONJ_SHIFT )
#define BLIS_UPLO_BITS                     ( 0x7  << BLIS_UPLO_SHIFT )
#define   BLIS_UPPER_BIT                   ( 0x1  << BLIS_UPPER_SHIFT )
#define   BLIS_DIAG_BIT                    ( 0x1  << BLIS_DIAG_SHIFT )
#define   BLIS_LOWER_BIT                   ( 0x1  << BLIS_LOWER_SHIFT )
#define BLIS_UNIT_DIAG_BIT                 ( 0x1  << BLIS_UNIT_DIAG_SHIFT )
#define BLIS_INVERT_DIAG_BIT               ( 0x1  << BLIS_INVERT_DIAG_SHIFT )
#define BLIS_TARGET_DT_BITS                ( 0x7  << BLIS_TARGET_DT_SHIFT )
#define BLIS_EXECUTION_DT_BITS             ( 0x7  << BLIS_EXECUTION_DT_SHIFT )
#define BLIS_PACK_SCHEMA_BITS              ( 0x7F << BLIS_PACK_SCHEMA_SHIFT )
#define   BLIS_PACK_RC_BIT                 ( 0x1  << BLIS_PACK_RC_SHIFT )
#define   BLIS_PACK_PANEL_BIT              ( 0x1  << BLIS_PACK_PANEL_SHIFT )
#define   BLIS_PACK_FORMAT_BITS            ( 0xF  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_PACK_BIT                    ( 0x1  << BLIS_PACK_SHIFT )
#define BLIS_PACK_REV_IF_UPPER_BIT         ( 0x1  << BLIS_PACK_REV_IF_UPPER_SHIFT )
#define BLIS_PACK_REV_IF_LOWER_BIT         ( 0x1  << BLIS_PACK_REV_IF_LOWER_SHIFT )
#define BLIS_PACK_BUFFER_BITS              ( 0x3  << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_STRUC_BITS                    ( 0x3  << BLIS_STRUC_SHIFT )


//
// -- BLIS enumerated type value definitions -----------------------------------
//

#define BLIS_BITVAL_REAL                      0x0
#define BLIS_BITVAL_COMPLEX                   BLIS_DOMAIN_BIT
#define BLIS_BITVAL_SINGLE_PREC               0x0
#define BLIS_BITVAL_DOUBLE_PREC               BLIS_PRECISION_BIT
#define   BLIS_BITVAL_FLOAT_TYPE              0x0
#define   BLIS_BITVAL_SCOMPLEX_TYPE           BLIS_DOMAIN_BIT  
#define   BLIS_BITVAL_DOUBLE_TYPE             BLIS_PRECISION_BIT
#define   BLIS_BITVAL_DCOMPLEX_TYPE         ( BLIS_DOMAIN_BIT | BLIS_PRECISION_BIT )
#define   BLIS_BITVAL_INT_TYPE                0x04
#define   BLIS_BITVAL_CONST_TYPE              0x05
#define BLIS_BITVAL_NO_TRANS                  0x0
#define BLIS_BITVAL_TRANS                     BLIS_TRANS_BIT
#define BLIS_BITVAL_NO_CONJ                   0x0
#define BLIS_BITVAL_CONJ                      BLIS_CONJ_BIT
#define BLIS_BITVAL_CONJ_TRANS              ( BLIS_CONJ_BIT | BLIS_TRANS_BIT )
#define BLIS_BITVAL_ZEROS                     0x0 
#define BLIS_BITVAL_UPPER                   ( BLIS_UPPER_BIT | BLIS_DIAG_BIT )
#define BLIS_BITVAL_LOWER                   ( BLIS_LOWER_BIT | BLIS_DIAG_BIT )
#define BLIS_BITVAL_DENSE                     BLIS_UPLO_BITS  
#define BLIS_BITVAL_NONUNIT_DIAG              0x0
#define BLIS_BITVAL_UNIT_DIAG                 BLIS_UNIT_DIAG_BIT
#define BLIS_BITVAL_INVERT_DIAG               BLIS_INVERT_DIAG_BIT
#define BLIS_BITVAL_NOT_PACKED                0x0
#define   BLIS_BITVAL_4MI                   ( 0x1  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_3MI                   ( 0x2  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_4MS                   ( 0x3  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_3MS                   ( 0x4  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_RO                    ( 0x5  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_IO                    ( 0x6  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_RPI                   ( 0x7  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_1E                    ( 0x8  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_1R                    ( 0x9  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_PACKED_UNSPEC         ( BLIS_PACK_BIT                                                            )
#define   BLIS_BITVAL_PACKED_ROWS           ( BLIS_PACK_BIT                                                            )
#define   BLIS_BITVAL_PACKED_COLUMNS        ( BLIS_PACK_BIT                                         | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS     ( BLIS_PACK_BIT                   | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS     ( BLIS_PACK_BIT                   | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_4MI ( BLIS_PACK_BIT | BLIS_BITVAL_4MI | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_4MI ( BLIS_PACK_BIT | BLIS_BITVAL_4MI | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_3MI ( BLIS_PACK_BIT | BLIS_BITVAL_3MI | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_3MI ( BLIS_PACK_BIT | BLIS_BITVAL_3MI | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_4MS ( BLIS_PACK_BIT | BLIS_BITVAL_4MS | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_4MS ( BLIS_PACK_BIT | BLIS_BITVAL_4MS | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_3MS ( BLIS_PACK_BIT | BLIS_BITVAL_3MS | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_3MS ( BLIS_PACK_BIT | BLIS_BITVAL_3MS | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_RO  ( BLIS_PACK_BIT | BLIS_BITVAL_RO  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_RO  ( BLIS_PACK_BIT | BLIS_BITVAL_RO  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_IO  ( BLIS_PACK_BIT | BLIS_BITVAL_IO  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_IO  ( BLIS_PACK_BIT | BLIS_BITVAL_IO  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_RPI ( BLIS_PACK_BIT | BLIS_BITVAL_RPI | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_RPI ( BLIS_PACK_BIT | BLIS_BITVAL_RPI | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_1E  ( BLIS_PACK_BIT | BLIS_BITVAL_1E  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_1E  ( BLIS_PACK_BIT | BLIS_BITVAL_1E  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_1R  ( BLIS_PACK_BIT | BLIS_BITVAL_1R  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_1R  ( BLIS_PACK_BIT | BLIS_BITVAL_1R  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define BLIS_BITVAL_PACK_FWD_IF_UPPER         0x0
#define BLIS_BITVAL_PACK_REV_IF_UPPER         BLIS_PACK_REV_IF_UPPER_BIT
#define BLIS_BITVAL_PACK_FWD_IF_LOWER         0x0
#define BLIS_BITVAL_PACK_REV_IF_LOWER         BLIS_PACK_REV_IF_LOWER_BIT
#define BLIS_BITVAL_BUFFER_FOR_A_BLOCK        0x0
#define BLIS_BITVAL_BUFFER_FOR_B_PANEL      ( 0x1 << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_BITVAL_BUFFER_FOR_C_PANEL      ( 0x2 << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_BITVAL_BUFFER_FOR_GEN_USE      ( 0x3 << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_BITVAL_GENERAL                   0x0
#define BLIS_BITVAL_HERMITIAN               ( 0x1 << BLIS_STRUC_SHIFT )
#define BLIS_BITVAL_SYMMETRIC               ( 0x2 << BLIS_STRUC_SHIFT )
#define BLIS_BITVAL_TRIANGULAR              ( 0x3 << BLIS_STRUC_SHIFT )


//
// -- BLIS enumerated type definitions -----------------------------------------
//

// -- Operational parameter types --

typedef enum
{
	BLIS_NO_TRANSPOSE      = 0x0,
	BLIS_TRANSPOSE         = BLIS_BITVAL_TRANS,
	BLIS_CONJ_NO_TRANSPOSE = BLIS_BITVAL_CONJ,
	BLIS_CONJ_TRANSPOSE    = BLIS_BITVAL_CONJ_TRANS
} trans_t;

typedef enum
{
	BLIS_NO_CONJUGATE      = 0x0,
	BLIS_CONJUGATE         = BLIS_BITVAL_CONJ
} conj_t;

typedef enum
{
	BLIS_ZEROS             = BLIS_BITVAL_ZEROS,
	BLIS_LOWER             = BLIS_BITVAL_LOWER,
	BLIS_UPPER             = BLIS_BITVAL_UPPER,
	BLIS_DENSE             = BLIS_BITVAL_DENSE
} uplo_t;

typedef enum
{
	BLIS_LEFT              = 0x0,
	BLIS_RIGHT
} side_t;

typedef enum
{
	BLIS_NONUNIT_DIAG      = 0x0,
	BLIS_UNIT_DIAG         = BLIS_BITVAL_UNIT_DIAG
} diag_t;

typedef enum
{
	BLIS_NO_INVERT_DIAG    = 0x0,
	BLIS_INVERT_DIAG       = BLIS_BITVAL_INVERT_DIAG
} invdiag_t;

typedef enum
{
	BLIS_GENERAL           = BLIS_BITVAL_GENERAL,
	BLIS_HERMITIAN         = BLIS_BITVAL_HERMITIAN,
	BLIS_SYMMETRIC         = BLIS_BITVAL_SYMMETRIC,
	BLIS_TRIANGULAR        = BLIS_BITVAL_TRIANGULAR
} struc_t;


// -- Data type --

typedef enum
{
	BLIS_FLOAT             = BLIS_BITVAL_FLOAT_TYPE,
	BLIS_DOUBLE            = BLIS_BITVAL_DOUBLE_TYPE,
	BLIS_SCOMPLEX          = BLIS_BITVAL_SCOMPLEX_TYPE,
	BLIS_DCOMPLEX          = BLIS_BITVAL_DCOMPLEX_TYPE,
	BLIS_INT               = BLIS_BITVAL_INT_TYPE,
	BLIS_CONSTANT          = BLIS_BITVAL_CONST_TYPE,
	BLIS_DT_LO             = BLIS_FLOAT,
	BLIS_DT_HI             = BLIS_DCOMPLEX
} num_t;

typedef enum
{
	BLIS_REAL              = BLIS_BITVAL_REAL,
	BLIS_COMPLEX           = BLIS_BITVAL_COMPLEX
} dom_t;

typedef enum
{
	BLIS_SINGLE_PREC       = BLIS_BITVAL_SINGLE_PREC,
	BLIS_DOUBLE_PREC       = BLIS_BITVAL_DOUBLE_PREC
} prec_t;


// -- Pack schema type --

typedef enum
{
	BLIS_NOT_PACKED            = BLIS_BITVAL_NOT_PACKED,
	BLIS_PACKED_UNSPEC         = BLIS_BITVAL_PACKED_UNSPEC,
	BLIS_PACKED_VECTOR         = BLIS_BITVAL_PACKED_UNSPEC,
	BLIS_PACKED_ROWS           = BLIS_BITVAL_PACKED_ROWS,
	BLIS_PACKED_COLUMNS        = BLIS_BITVAL_PACKED_COLUMNS,
	BLIS_PACKED_ROW_PANELS     = BLIS_BITVAL_PACKED_ROW_PANELS,
	BLIS_PACKED_COL_PANELS     = BLIS_BITVAL_PACKED_COL_PANELS,
	BLIS_PACKED_ROW_PANELS_4MI = BLIS_BITVAL_PACKED_ROW_PANELS_4MI,
	BLIS_PACKED_COL_PANELS_4MI = BLIS_BITVAL_PACKED_COL_PANELS_4MI,
	BLIS_PACKED_ROW_PANELS_3MI = BLIS_BITVAL_PACKED_ROW_PANELS_3MI,
	BLIS_PACKED_COL_PANELS_3MI = BLIS_BITVAL_PACKED_COL_PANELS_3MI,
	BLIS_PACKED_ROW_PANELS_4MS = BLIS_BITVAL_PACKED_ROW_PANELS_4MS,
	BLIS_PACKED_COL_PANELS_4MS = BLIS_BITVAL_PACKED_COL_PANELS_4MS,
	BLIS_PACKED_ROW_PANELS_3MS = BLIS_BITVAL_PACKED_ROW_PANELS_3MS,
	BLIS_PACKED_COL_PANELS_3MS = BLIS_BITVAL_PACKED_COL_PANELS_3MS,
	BLIS_PACKED_ROW_PANELS_RO  = BLIS_BITVAL_PACKED_ROW_PANELS_RO,
	BLIS_PACKED_COL_PANELS_RO  = BLIS_BITVAL_PACKED_COL_PANELS_RO,
	BLIS_PACKED_ROW_PANELS_IO  = BLIS_BITVAL_PACKED_ROW_PANELS_IO,
	BLIS_PACKED_COL_PANELS_IO  = BLIS_BITVAL_PACKED_COL_PANELS_IO,
	BLIS_PACKED_ROW_PANELS_RPI = BLIS_BITVAL_PACKED_ROW_PANELS_RPI,
	BLIS_PACKED_COL_PANELS_RPI = BLIS_BITVAL_PACKED_COL_PANELS_RPI,
	BLIS_PACKED_ROW_PANELS_1E  = BLIS_BITVAL_PACKED_ROW_PANELS_1E,
	BLIS_PACKED_COL_PANELS_1E  = BLIS_BITVAL_PACKED_COL_PANELS_1E,
	BLIS_PACKED_ROW_PANELS_1R  = BLIS_BITVAL_PACKED_ROW_PANELS_1R,
	BLIS_PACKED_COL_PANELS_1R  = BLIS_BITVAL_PACKED_COL_PANELS_1R
} pack_t;

// We combine row and column packing into one "type", and we start
// with BLIS_PACKED_ROW_PANELS, _COLUMN_PANELS. We also count the
// schema pair for "4ms" (4m separated), because its bit value has
// been reserved, even though we don't use it.
#define BLIS_NUM_PACK_SCHEMA_TYPES 10


// -- Pack order type --

typedef enum
{
	BLIS_PACK_FWD_IF_UPPER = BLIS_BITVAL_PACK_FWD_IF_UPPER,
	BLIS_PACK_REV_IF_UPPER = BLIS_BITVAL_PACK_REV_IF_UPPER,

	BLIS_PACK_FWD_IF_LOWER = BLIS_BITVAL_PACK_FWD_IF_LOWER,
	BLIS_PACK_REV_IF_LOWER = BLIS_BITVAL_PACK_REV_IF_LOWER
} packord_t;


// -- Pack buffer type --

typedef enum
{
	BLIS_BUFFER_FOR_A_BLOCK = BLIS_BITVAL_BUFFER_FOR_A_BLOCK,
	BLIS_BUFFER_FOR_B_PANEL = BLIS_BITVAL_BUFFER_FOR_B_PANEL,
	BLIS_BUFFER_FOR_C_PANEL = BLIS_BITVAL_BUFFER_FOR_C_PANEL,
	BLIS_BUFFER_FOR_GEN_USE = BLIS_BITVAL_BUFFER_FOR_GEN_USE
} packbuf_t;


// -- Partitioning direction --

typedef enum
{
	BLIS_FWD,
	BLIS_BWD
} dir_t;


// -- Subpartition type --

typedef enum
{
	BLIS_SUBPART0,
	BLIS_SUBPART1,
	BLIS_SUBPART2,
	BLIS_SUBPART1T,
	BLIS_SUBPART1B,
	BLIS_SUBPART1L,
	BLIS_SUBPART1R,
	BLIS_SUBPART00,
	BLIS_SUBPART10,
	BLIS_SUBPART20,
	BLIS_SUBPART01,
	BLIS_SUBPART11,
	BLIS_SUBPART21,
	BLIS_SUBPART02,
	BLIS_SUBPART12,
	BLIS_SUBPART22
} subpart_t;


// -- Matrix dimension type --

typedef enum
{
	BLIS_M = 0,
	BLIS_N = 1
} mdim_t;


// -- Machine parameter types --

typedef enum
{
	BLIS_MACH_EPS = 0,
	BLIS_MACH_SFMIN,
	BLIS_MACH_BASE,
	BLIS_MACH_PREC,
	BLIS_MACH_NDIGMANT,
	BLIS_MACH_RND,
	BLIS_MACH_EMIN,
	BLIS_MACH_RMIN,
	BLIS_MACH_EMAX,
	BLIS_MACH_RMAX,
	BLIS_MACH_EPS2
} machval_t;

#define BLIS_NUM_MACH_PARAMS   11
#define BLIS_MACH_PARAM_FIRST  BLIS_MACH_EPS
#define BLIS_MACH_PARAM_LAST   BLIS_MACH_EPS2


// -- Induced method types --

typedef enum
{
	BLIS_3MH = 0,
	BLIS_3M1,
	BLIS_4MH,
	BLIS_4M1B,
	BLIS_4M1A,
	BLIS_1M,
	BLIS_NAT
} ind_t;

#define BLIS_NUM_IND_METHODS (BLIS_NAT+1)

// These are used in bli_*_oapi.c to construct the ind_t values from
// the induced method substrings that go into function names.
#define bli_3mh  BLIS_3MH
#define bli_3m1  BLIS_3M1
#define bli_4mh  BLIS_4MH
#define bli_4mb  BLIS_4M1B
#define bli_4m1  BLIS_4M1A
#define bli_1m   BLIS_1M
#define bli_nat  BLIS_NAT


// -- Kernel ID types --

typedef enum
{
	BLIS_ADDV_KER  = 0,
	BLIS_AMAXV_KER,
	BLIS_AXPBYV_KER,
	BLIS_AXPYV_KER,
	BLIS_COPYV_KER,
	BLIS_DOTV_KER,
	BLIS_DOTXV_KER,
	BLIS_INVERTV_KER,
	BLIS_SCALV_KER,
	BLIS_SCAL2V_KER,
	BLIS_SETV_KER,
	BLIS_SUBV_KER,
	BLIS_SWAPV_KER,
	BLIS_XPBYV_KER
} l1vkr_t;

#define BLIS_NUM_LEVEL1V_KERS 14


typedef enum
{
	BLIS_AXPY2V_KER = 0,
	BLIS_DOTAXPYV_KER,
	BLIS_AXPYF_KER,
	BLIS_DOTXF_KER,
	BLIS_DOTXAXPYF_KER
} l1fkr_t;

#define BLIS_NUM_LEVEL1F_KERS 5


typedef enum
{
	BLIS_PACKM_0XK_KER  = 0,
	BLIS_PACKM_1XK_KER  = 1,
	BLIS_PACKM_2XK_KER  = 2,
	BLIS_PACKM_3XK_KER  = 3,
	BLIS_PACKM_4XK_KER  = 4,
	BLIS_PACKM_5XK_KER  = 5,
	BLIS_PACKM_6XK_KER  = 6,
	BLIS_PACKM_7XK_KER  = 7,
	BLIS_PACKM_8XK_KER  = 8,
	BLIS_PACKM_9XK_KER  = 9,
	BLIS_PACKM_10XK_KER = 10,
	BLIS_PACKM_11XK_KER = 11,
	BLIS_PACKM_12XK_KER = 12,
	BLIS_PACKM_13XK_KER = 13,
	BLIS_PACKM_14XK_KER = 14,
	BLIS_PACKM_15XK_KER = 15,
	BLIS_PACKM_16XK_KER = 16,
	BLIS_PACKM_17XK_KER = 17,
	BLIS_PACKM_18XK_KER = 18,
	BLIS_PACKM_19XK_KER = 19,
	BLIS_PACKM_20XK_KER = 20,
	BLIS_PACKM_21XK_KER = 21,
	BLIS_PACKM_22XK_KER = 22,
	BLIS_PACKM_23XK_KER = 23,
	BLIS_PACKM_24XK_KER = 24,
	BLIS_PACKM_25XK_KER = 25,
	BLIS_PACKM_26XK_KER = 26,
	BLIS_PACKM_27XK_KER = 27,
	BLIS_PACKM_28XK_KER = 28,
	BLIS_PACKM_29XK_KER = 29,
	BLIS_PACKM_30XK_KER = 30,
	BLIS_PACKM_31XK_KER = 31,

	BLIS_UNPACKM_0XK_KER  = 0,
	BLIS_UNPACKM_1XK_KER  = 1,
	BLIS_UNPACKM_2XK_KER  = 2,
	BLIS_UNPACKM_3XK_KER  = 3,
	BLIS_UNPACKM_4XK_KER  = 4,
	BLIS_UNPACKM_5XK_KER  = 5,
	BLIS_UNPACKM_6XK_KER  = 6,
	BLIS_UNPACKM_7XK_KER  = 7,
	BLIS_UNPACKM_8XK_KER  = 8,
	BLIS_UNPACKM_9XK_KER  = 9,
	BLIS_UNPACKM_10XK_KER = 10,
	BLIS_UNPACKM_11XK_KER = 11,
	BLIS_UNPACKM_12XK_KER = 12,
	BLIS_UNPACKM_13XK_KER = 13,
	BLIS_UNPACKM_14XK_KER = 14,
	BLIS_UNPACKM_15XK_KER = 15,
	BLIS_UNPACKM_16XK_KER = 16,
	BLIS_UNPACKM_17XK_KER = 17,
	BLIS_UNPACKM_18XK_KER = 18,
	BLIS_UNPACKM_19XK_KER = 19,
	BLIS_UNPACKM_20XK_KER = 20,
	BLIS_UNPACKM_21XK_KER = 21,
	BLIS_UNPACKM_22XK_KER = 22,
	BLIS_UNPACKM_23XK_KER = 23,
	BLIS_UNPACKM_24XK_KER = 24,
	BLIS_UNPACKM_25XK_KER = 25,
	BLIS_UNPACKM_26XK_KER = 26,
	BLIS_UNPACKM_27XK_KER = 27,
	BLIS_UNPACKM_28XK_KER = 28,
	BLIS_UNPACKM_29XK_KER = 29,
	BLIS_UNPACKM_30XK_KER = 30,
	BLIS_UNPACKM_31XK_KER = 31

} l1mkr_t;

#define BLIS_NUM_PACKM_KERS   32
#define BLIS_NUM_UNPACKM_KERS 32


typedef enum
{
	BLIS_GEMM_UKR = 0,
	BLIS_GEMMTRSM_L_UKR,
	BLIS_GEMMTRSM_U_UKR,
	BLIS_TRSM_L_UKR,
	BLIS_TRSM_U_UKR
} l3ukr_t;

#define BLIS_NUM_LEVEL3_UKRS 5


typedef enum
{
	BLIS_REFERENCE_UKERNEL = 0,
	BLIS_VIRTUAL_UKERNEL,
	BLIS_OPTIMIZED_UKERNEL,
	BLIS_NOTAPPLIC_UKERNEL
} kimpl_t;

#define BLIS_NUM_UKR_IMPL_TYPES 4


#if 0
typedef enum
{
	BLIS_JC_IDX = 0,
	BLIS_PC_IDX,
	BLIS_IC_IDX,
	BLIS_JR_IDX,
	BLIS_IR_IDX,
	BLIS_PR_IDX
} thridx_t;
#endif

#define BLIS_NUM_LOOPS 6


// -- Operation ID type --

typedef enum
{
//
// NOTE: If/when additional type values are added to this enum,
// you must either:
// - keep the level-3 values (starting with _GEMM) beginning at
//   index 0; or
// - if the value range is moved such that it does not begin at
//   index 0, implement something like a BLIS_OPID_LEVEL3_RANGE_START
//   value that can be subtracted from the opid_t value to map it
//   to a zero-based range.
// This is needed because these level-3 opid_t values are used in
// bli_l3_ind.c to index into arrays.
//
	BLIS_GEMM = 0,
	BLIS_HEMM,
	BLIS_HERK,
	BLIS_HER2K,
	BLIS_SYMM,
	BLIS_SYRK,
	BLIS_SYR2K,
	BLIS_TRMM3,
	BLIS_TRMM,
	BLIS_TRSM,

	BLIS_NOID
} opid_t;

#define BLIS_NUM_LEVEL3_OPS 10


// -- Blocksize ID type --

typedef enum
{
	// NOTE: the level-3 blocksizes MUST be indexed starting at zero.
	// At one point, we made this assumption in bli_cntx_set_blkszs()
	// and friends.

	BLIS_KR = 0,
	BLIS_MR,
	BLIS_NR,
	BLIS_MC,
	BLIS_KC,
	BLIS_NC,
	BLIS_M2, // level-2 blocksize in m dimension
	BLIS_N2, // level-2 blocksize in n dimension
	BLIS_AF, // level-1f axpyf fusing factor
	BLIS_DF, // level-1f dotxf fusing factor
	BLIS_XF, // level-1f dotxaxpyf fusing factor

	BLIS_NO_PART  // used as a placeholder when blocksizes are not applicable.
} bszid_t;

#define BLIS_NUM_BLKSZS 11


// -- Architecture ID type --

// NOTE: This typedef enum must be kept up-to-date with the arch_t
// string array in bli_arch.c. Whenever values are added/inserted
// OR if values are rearranged, be sure to update the string array
// in bli_arch.c.

typedef enum
{
	// Intel
	BLIS_ARCH_SKX = 0,
	BLIS_ARCH_KNL,
	BLIS_ARCH_KNC,
	BLIS_ARCH_HASWELL,
	BLIS_ARCH_SANDYBRIDGE,
	BLIS_ARCH_PENRYN,

	// AMD
	BLIS_ARCH_ZEN,
	BLIS_ARCH_EXCAVATOR,
	BLIS_ARCH_STEAMROLLER,
	BLIS_ARCH_PILEDRIVER,
	BLIS_ARCH_BULLDOZER,

	// ARM
	BLIS_ARCH_CORTEXA57,
	BLIS_ARCH_CORTEXA15,
	BLIS_ARCH_CORTEXA9,

	// IBM/Power
	BLIS_ARCH_POWER7,
	BLIS_ARCH_BGQ,

	// Generic architecture/configuration
	BLIS_ARCH_GENERIC

} arch_t;

#define BLIS_NUM_ARCHS 17


//
// -- BLIS misc. structure types -----------------------------------------------
//

// -- Pool block type --

typedef struct
{
	void* buf_sys;
	void* buf_align;
} pblk_t;

// -- Pool type --

typedef struct
{
	pblk_t* block_ptrs;
	dim_t   block_ptrs_len;

	dim_t   top_index;
	dim_t   num_blocks;

	siz_t   block_size;
	siz_t   align_size;
} pool_t;

// -- Mutex object type --

// begin bli_mutex.h


#ifndef BLIS_MUTEX_H
#define BLIS_MUTEX_H

// Include definitions (mostly mtx_t) specific to the method of
// multithreading.
// begin bli_mutex_single.h


#ifndef BLIS_MUTEX_SINGLE_H
#define BLIS_MUTEX_SINGLE_H

// Define mtx_t for situations when multithreading is disabled.
#ifndef BLIS_ENABLE_MULTITHREADING

// Define mtx_t.

typedef struct mtx_s
{
} mtx_t;

// Define macros to operate on pthread-based mtx_t.

static void bli_mutex_init( mtx_t* m )
{
}

static void bli_mutex_finalize( mtx_t* m )
{
}

static void bli_mutex_lock( mtx_t* m )
{
}

static void bli_mutex_unlock( mtx_t* m )
{
}

#endif

#endif

// end bli_mutex_single.h
// begin bli_mutex_openmp.h


#ifndef BLIS_MUTEX_OPENMP_H
#define BLIS_MUTEX_OPENMP_H

// Define mutex_t for situations when OpenMP multithreading is enabled.
#ifdef BLIS_ENABLE_OPENMP

#include <omp.h> // skipped

// Define mtx_t.

typedef struct mtx_s
{
	omp_lock_t mutex;
} mtx_t;

// Define functions to operate on OpenMP-based mtx_t.

static void bli_mutex_init( mtx_t* m )
{
	omp_init_lock( &(m->mutex) );
}

static void bli_mutex_finalize( mtx_t* m )
{
	omp_destroy_lock( &(m->mutex) );
}

static void bli_mutex_lock( mtx_t* m )
{
	omp_set_lock( &(m->mutex) );
}

static void bli_mutex_unlock( mtx_t* m )
{
	omp_unset_lock( &(m->mutex) );
}

#endif

#endif

// end bli_mutex_openmp.h
// begin bli_mutex_pthreads.h


#ifndef BLIS_MUTEX_PTHREADS_H
#define BLIS_MUTEX_PTHREADS_H

// Define mutex_t for situations when POSIX multithreading is enabled.
#ifdef BLIS_ENABLE_PTHREADS

#include <pthread.h> // skipped

// Define mtx_t.

typedef struct mtx_s
{
	pthread_mutex_t mutex;
} mtx_t;

// Define macros to operate on pthread-based mtx_t.

static void bli_mutex_init( mtx_t* m )
{
	pthread_mutex_init( &(m->mutex), NULL ); \
}

static void bli_mutex_finalize( mtx_t* m )
{
	pthread_mutex_destroy( &(m->mutex) ); \
}

static void bli_mutex_lock( mtx_t* m )
{
	pthread_mutex_lock( &(m->mutex) ); \
}

static void bli_mutex_unlock( mtx_t* m )
{
	pthread_mutex_unlock( &(m->mutex) ); \
}

#endif

#endif

// end bli_mutex_pthreads.h

// Thread mutex prototypes.


#endif

// end bli_mutex.h
// begin bli_malloc.h


// Typedef function pointer types for malloc() and free() substitutes.
typedef void* (*malloc_ft) ( size_t size );
typedef void  (*free_ft)   ( void*  p    );

// -----------------------------------------------------------------------------

void* bli_malloc_pool( size_t size );
void  bli_free_pool( void* p );

void* bli_malloc_intl( size_t size );
void* bli_calloc_intl( size_t size );
void  bli_free_intl( void* p );

void* bli_malloc_user( size_t size );
void  bli_free_user( void* p );

// -----------------------------------------------------------------------------

void* bli_malloc_align( malloc_ft f, size_t size, size_t align_size );
void  bli_free_align( free_ft f, void* p );

void* bli_malloc_noalign( malloc_ft f, size_t size );
void  bli_free_noalign( free_ft f, void* p );

void  bli_malloc_align_check( malloc_ft f, size_t size, size_t align_size );

// end bli_malloc.h

// -- Memory broker object type --

typedef struct membrk_s
{
	pool_t    pools[3];
	mtx_t     mutex;

	malloc_ft malloc_fp;
	free_ft   free_fp;
} membrk_t;

// -- Memory object type --

typedef struct mem_s
{
	pblk_t    pblk;
	packbuf_t buf_type;
	pool_t*   pool;
	membrk_t* membrk;
	siz_t     size;
} mem_t;

// -- Control tree node type --

struct cntl_s
{
	// Basic fields (usually required).
	opid_t         family;
	bszid_t        bszid;
	void*          var_func;
	struct cntl_s* sub_node;

	// Optional fields (needed only by some operations such as packm).
	// NOTE: first field of params must be a uint64_t containing the size
	// of the struct.
	void*          params;

	// Internal fields that track "cached" data.
	mem_t          pack_mem;
};
typedef struct cntl_s cntl_t;


// -- Blocksize object type --

typedef struct blksz_s
{
	// Primary blocksize values.
	dim_t  v[BLIS_NUM_FP_TYPES];

	// Blocksize extensions.
	dim_t  e[BLIS_NUM_FP_TYPES];

} blksz_t;


// -- Function pointer object type --

typedef struct func_s
{
	// Kernel function address.
	void*  ptr[BLIS_NUM_FP_TYPES];

} func_t;


// -- Multi-boolean object type --

typedef struct mbool_s
{
	bool_t  v[BLIS_NUM_FP_TYPES];

} mbool_t;


// -- Auxiliary kernel info type --

// Note: This struct is used by macro-kernels to package together extra
// parameter values that may be of use to the micro-kernel without
// cluttering up the micro-kernel interface itself.

typedef struct
{
	// The pack schemas of A and B.
	pack_t schema_a;
	pack_t schema_b;

	// Pointers to the micro-panels of A and B which will be used by the
	// next call to the micro-kernel.
	void*  a_next;
	void*  b_next;

	// The imaginary strides of A and B.
	inc_t  is_a;
	inc_t  is_b;

} auxinfo_t;


// -- Global scalar constant data struct --

// Note: This struct is used only when statically initializing the
// global scalar constants in bli_const.c.
typedef struct constdata_s
{
	float    s;
	double   d;
	scomplex c;
	dcomplex z;
	gint_t   i;

} constdata_t;


//
// -- BLIS object type definitions ---------------------------------------------
//

typedef struct obj_s
{
	// Basic fields
	struct obj_s* root;

	dim_t         off[2];
	dim_t         dim[2];
	doff_t        diag_off;

	objbits_t     info;
	siz_t         elem_size;

	void*         buffer;
	inc_t         rs;
	inc_t         cs;
	inc_t         is;

	// Bufferless scalar storage
	atom_t        scalar;

	// Pack-related fields
	dim_t         m_padded; // m dimension of matrix, including any padding
	dim_t         n_padded; // n dimension of matrix, including any padding
	inc_t         ps;       // panel stride (distance to next panel)
	inc_t         pd;       // panel dimension (the "width" of a panel:
	                        // usually MR or NR)
	dim_t         m_panel;  // m dimension of a "full" panel
	dim_t         n_panel;  // n dimension of a "full" panel
} obj_t;

// Define these macros here since they must be updated if contents of
// obj_t changes.

#define bli_obj_init_full_shallow_copy_of( a, b ) \
{ \
	(b).root      = (a).root; \
\
	(b).off[0]    = (a).off[0]; \
	(b).off[1]    = (a).off[1]; \
	(b).dim[0]    = (a).dim[0]; \
	(b).dim[1]    = (a).dim[1]; \
	(b).diag_off  = (a).diag_off; \
\
	(b).info      = (a).info; \
	(b).elem_size = (a).elem_size; \
\
	(b).buffer    = (a).buffer; \
	(b).rs        = (a).rs; \
	(b).cs        = (a).cs; \
	(b).is        = (a).is; \
\
	(b).scalar    = (a).scalar; \
\
	 \
	(b).m_padded  = (a).m_padded; \
	(b).n_padded  = (a).n_padded; \
	(b).ps        = (a).ps; \
	(b).pd        = (a).pd; \
	(b).m_panel   = (a).m_panel; \
	(b).n_panel   = (a).n_panel; \
}

#define bli_obj_init_subpart_from( a, b ) \
{ \
	(b).root      = (a).root; \
\
	(b).off[0]    = (a).off[0]; \
	(b).off[1]    = (a).off[1]; \
	 \
	 \
	(b).diag_off  = (a).diag_off; \
\
	(b).info      = (a).info; \
	(b).elem_size = (a).elem_size; \
\
	(b).buffer    = (a).buffer; \
	(b).rs        = (a).rs; \
	(b).cs        = (a).cs; \
	(b).is        = (a).is; \
\
	(b).scalar    = (a).scalar; \
\
	 \
	 \
	(b).m_padded  = (a).m_padded; \
	(b).n_padded  = (a).n_padded; \
	(b).pd        = (a).pd; \
	(b).ps        = (a).ps; \
	(b).m_panel   = (a).m_panel; \
	(b).n_panel   = (a).n_panel; \
}


// -- Context type --

typedef struct cntx_s
{
	blksz_t   blkszs[ BLIS_NUM_BLKSZS ];
	bszid_t   bmults[ BLIS_NUM_BLKSZS ];

	func_t    l3_vir_ukrs[ BLIS_NUM_LEVEL3_UKRS ];
	func_t    l3_nat_ukrs[ BLIS_NUM_LEVEL3_UKRS ];
	mbool_t   l3_nat_ukrs_prefs[ BLIS_NUM_LEVEL3_UKRS ];

	func_t    l1f_kers[ BLIS_NUM_LEVEL1F_KERS ];
	func_t    l1v_kers[ BLIS_NUM_LEVEL1V_KERS ];

	func_t    packm_kers[ BLIS_NUM_PACKM_KERS ];
	func_t    unpackm_kers[ BLIS_NUM_UNPACKM_KERS ];

	ind_t     method;
	pack_t    schema_a_block;
	pack_t    schema_b_panel;
	pack_t    schema_c_panel;

	bool_t    anti_pref;

	dim_t     thrloop[ BLIS_NUM_LOOPS ];

	membrk_t* membrk;
} cntx_t;


// -- Error types --

typedef enum
{
	BLIS_NO_ERROR_CHECKING = 0,
	BLIS_FULL_ERROR_CHECKING
} errlev_t;

typedef enum
{
	// Generic error codes
	BLIS_SUCCESS                               = (  -1),
	BLIS_FAILURE                               = (  -2),

	BLIS_ERROR_CODE_MIN                        = (  -9),

	// General errors
	BLIS_INVALID_ERROR_CHECKING_LEVEL          = ( -10),
	BLIS_UNDEFINED_ERROR_CODE                  = ( -11),
	BLIS_NULL_POINTER                          = ( -12),
	BLIS_NOT_YET_IMPLEMENTED                   = ( -13),

	// Parameter-specific errors
	BLIS_INVALID_SIDE                          = ( -20),
	BLIS_INVALID_UPLO                          = ( -21),
	BLIS_INVALID_TRANS                         = ( -22),
	BLIS_INVALID_CONJ                          = ( -23),
	BLIS_INVALID_DIAG                          = ( -24),
	BLIS_INVALID_MACHVAL                       = ( -25),
	BLIS_EXPECTED_NONUNIT_DIAG                 = ( -26),

	// Datatype-specific errors
	BLIS_INVALID_DATATYPE                      = ( -30),
	BLIS_EXPECTED_FLOATING_POINT_DATATYPE      = ( -31),
	BLIS_EXPECTED_NONINTEGER_DATATYPE          = ( -32),
	BLIS_EXPECTED_NONCONSTANT_DATATYPE         = ( -33),
	BLIS_EXPECTED_REAL_DATATYPE                = ( -34),
	BLIS_EXPECTED_INTEGER_DATATYPE             = ( -35),
	BLIS_INCONSISTENT_DATATYPES                = ( -36),
	BLIS_EXPECTED_REAL_PROJ_OF                 = ( -37),
	BLIS_EXPECTED_REAL_VALUED_OBJECT           = ( -38),

	// Dimension-specific errors
	BLIS_NONCONFORMAL_DIMENSIONS               = ( -40),
	BLIS_EXPECTED_SCALAR_OBJECT                = ( -41),
	BLIS_EXPECTED_VECTOR_OBJECT                = ( -42),
	BLIS_UNEQUAL_VECTOR_LENGTHS                = ( -43),
	BLIS_EXPECTED_SQUARE_OBJECT                = ( -44),
	BLIS_UNEXPECTED_OBJECT_LENGTH              = ( -45),
	BLIS_UNEXPECTED_OBJECT_WIDTH               = ( -46),
	BLIS_UNEXPECTED_VECTOR_DIM                 = ( -47),
	BLIS_UNEXPECTED_DIAG_OFFSET                = ( -48),
	BLIS_NEGATIVE_DIMENSION                    = ( -49),

	// Stride-specific errors
	BLIS_INVALID_ROW_STRIDE                    = ( -50),
	BLIS_INVALID_COL_STRIDE                    = ( -51),
	BLIS_INVALID_DIM_STRIDE_COMBINATION        = ( -52),

	// Structure-specific errors    
	BLIS_EXPECTED_GENERAL_OBJECT               = ( -60),
	BLIS_EXPECTED_HERMITIAN_OBJECT             = ( -61),
	BLIS_EXPECTED_SYMMETRIC_OBJECT             = ( -62),
	BLIS_EXPECTED_TRIANGULAR_OBJECT            = ( -63),

	// Storage-specific errors    
	BLIS_EXPECTED_UPPER_OR_LOWER_OBJECT        = ( -70),

	// Partitioning-specific errors
	BLIS_INVALID_3x1_SUBPART                   = ( -80),
	BLIS_INVALID_1x3_SUBPART                   = ( -81),
	BLIS_INVALID_3x3_SUBPART                   = ( -82),

	// Control tree-specific errors
	BLIS_UNEXPECTED_NULL_CONTROL_TREE          = ( -90),

	// Packing-specific errors
	BLIS_PACK_SCHEMA_NOT_SUPPORTED_FOR_UNPACK  = (-100),

	// Buffer-specific errors 
	BLIS_EXPECTED_NONNULL_OBJECT_BUFFER        = (-110),

	// Memory allocator errors
	BLIS_INVALID_PACKBUF                       = (-120),
	BLIS_EXHAUSTED_CONTIG_MEMORY_POOL          = (-122),
	BLIS_INSUFFICIENT_STACK_BUF_SIZE           = (-123),
	BLIS_ALIGNMENT_NOT_POWER_OF_TWO            = (-124),
	BLIS_ALIGNMENT_NOT_MULT_OF_PTR_SIZE        = (-125),

	// Object-related errors
	BLIS_EXPECTED_OBJECT_ALIAS                 = (-130),

	// Architecture-related errors
	BLIS_INVALID_ARCH_ID                       = (-140),

	// Blocksize-related errors
	BLIS_MC_DEF_NONMULTIPLE_OF_MR              = (-150),
	BLIS_MC_MAX_NONMULTIPLE_OF_MR              = (-151),
	BLIS_NC_DEF_NONMULTIPLE_OF_NR              = (-152),
	BLIS_NC_MAX_NONMULTIPLE_OF_NR              = (-153),
	BLIS_KC_DEF_NONMULTIPLE_OF_KR              = (-154),
	BLIS_KC_MAX_NONMULTIPLE_OF_KR              = (-155),

	BLIS_ERROR_CODE_MAX                        = (-160)
} err_t;

#endif
// end bli_type_defs.h
// begin bli_macro_defs.h


#ifndef BLIS_MACRO_DEFS_H
#define BLIS_MACRO_DEFS_H


// -- Undefine restrict for C++ and C89/90 --

#ifdef __cplusplus
  // Language is C++; define restrict as nothing.
  #ifndef restrict
  #define restrict
  #endif
#elif __STDC_VERSION__ >= 199901L
  // Language is C99 (or later); do nothing since restrict is recognized.
#else
  // Language is pre-C99; define restrict as nothing.
  #ifndef restrict
  #define restrict
  #endif
#endif


// -- Define typeof() operator if using non-GNU compiler --

#ifndef __GNUC__
  #define typeof __typeof__
#else
  #ifndef typeof
  #define typeof __typeof__
  #endif
#endif


// -- BLIS Thread Local Storage Keyword --

// __thread for TLS is supported by GCC, CLANG, ICC, and IBMC.
// There is a small risk here as __GNUC__ can also be defined by some other
// compiler (other than ICC and CLANG which we know define it) that
// doesn't support __thread, as __GNUC__ is not quite unique to GCC.
// But the possibility of someone using such non-main-stream compiler
// for building BLIS is low.
#if defined(__GNUC__) || defined(__clang__) || defined(__ICC) || defined(__IBMC__)
  #define BLIS_THREAD_LOCAL __thread
#else
  #define BLIS_THREAD_LOCAL
#endif


// -- BLIS constructor/destructor function attribute --

// __attribute__((constructor/destructor)) is supported by GCC only.
// There is a small risk here as __GNUC__ can also be defined by some other
// compiler (other than ICC and CLANG which we know define it) that
// doesn't support this, as __GNUC__ is not quite unique to GCC.
// But the possibility of someone using such non-main-stream compiler
// for building BLIS is low.

#if defined(__ICC) || defined(__INTEL_COMPILER)
  // ICC defines __GNUC__ but doesn't support this
  #define BLIS_ATTRIB_CTOR
  #define BLIS_ATTRIB_DTOR
#elif defined(__clang__)
  // CLANG supports __attribute__, but its documentation doesn't
  // mention support for constructor/destructor. Compiling with
  // clang and testing shows that it does support.
  #define BLIS_ATTRIB_CTOR __attribute__((constructor))
  #define BLIS_ATTRIB_DTOR __attribute__((destructor))
#elif defined(__GNUC__)
  #define BLIS_ATTRIB_CTOR __attribute__((constructor))
  #define BLIS_ATTRIB_DTOR __attribute__((destructor))
#else
  #define BLIS_ATTRIB_CTOR
  #define BLIS_ATTRIB_DTOR
#endif


// -- Concatenation macros --

#define BLIS_FUNC_PREFIX_STR       "bli"

// We add an extra layer the definitions of these string-pasting macros
// because sometimes it is needed if, for example, one of the PASTE
// macros is invoked with an "op" argument that is itself a macro.

#define PASTEMAC0_(op)             bli_ ## op
#define PASTEMAC0(op)              PASTEMAC0_(op)

#define PASTEMAC_(ch,op)           bli_ ## ch  ## op
#define PASTEMAC(ch,op)            PASTEMAC_(ch,op)

#define PASTEMAC2_(ch1,ch2,op)     bli_ ## ch1 ## ch2 ## op
#define PASTEMAC2(ch1,ch2,op)      PASTEMAC2_(ch1,ch2,op)

#define PASTEMAC3_(ch1,ch2,ch3,op) bli_ ## ch1 ## ch2 ## ch3 ## op
#define PASTEMAC3(ch1,ch2,ch3,op)  PASTEMAC3_(ch1,ch2,ch3,op)

#define PASTEBLACHK_(op)           bla_ ## op ## _check
#define PASTEBLACHK(op)            PASTEBLACHK_(op)

#define PASTECH_(ch,op)            ch ## op
#define PASTECH(ch,op)             PASTECH_(ch,op)

#define PASTECH2_(ch1,ch2,op)      ch1 ## ch2 ## op
#define PASTECH2(ch1,ch2,op)       PASTECH2_(ch1,ch2,op)

#define PASTECH3_(ch1,ch2,ch3,op)  ch1 ## ch2 ## ch3 ## op
#define PASTECH3(ch1,ch2,ch3,op)   PASTECH3_(ch1,ch2,ch3,op)

#define MKSTR(s1)                  #s1
#define STRINGIFY_INT( s )         MKSTR( s )

// Fortran-77 name-mangling macros.
#define PASTEF770(name)                            name ## _
#define PASTEF77(ch1,name)           ch1        ## name ## _
#define PASTEF772(ch1,ch2,name)      ch1 ## ch2 ## name ## _
#define PASTEF773(ch1,ch2,ch3,name)  ch1 ## ch2 ## ch3 ## name ## _

// -- Include other groups of macros

// begin bli_genarray_macro_defs.h


#ifndef BLIS_GENARRAY_MACRO_DEFS_H
#define BLIS_GENARRAY_MACRO_DEFS_H


// -- Macros to generate function arrays ---------------------------------------

// -- "Smart" one-operand macro --

#define GENARRAY_VFP(ftname,opname) \
\
PASTECH(ftname,_vft) \
PASTECH(opname,_vfp)[BLIS_NUM_FP_TYPES] = \
{ \
	PASTEMAC(s,opname), \
	PASTEMAC(c,opname), \
	PASTEMAC(d,opname), \
	PASTEMAC(z,opname)  \
}

// -- "Smart" two-operand macro --





// -- One-operand macro --

#define GENARRAY(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES] = \
{ \
	PASTEMAC(s,op), \
	PASTEMAC(c,op), \
	PASTEMAC(d,op), \
	PASTEMAC(z,op)  \
}

#define GENARRAY_I(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES+1] = \
{ \
	PASTEMAC(s,op), \
	PASTEMAC(c,op), \
	PASTEMAC(d,op), \
	PASTEMAC(z,op), \
	PASTEMAC(i,op)  \
}





// -- Two-operand macros --


#define GENARRAY2_ALL(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES] = \
{ \
	{ PASTEMAC2(s,s,op), PASTEMAC2(s,c,op), PASTEMAC2(s,d,op), PASTEMAC2(s,z,op) }, \
	{ PASTEMAC2(c,s,op), PASTEMAC2(c,c,op), PASTEMAC2(c,d,op), PASTEMAC2(c,z,op) }, \
	{ PASTEMAC2(d,s,op), PASTEMAC2(d,c,op), PASTEMAC2(d,d,op), PASTEMAC2(d,z,op) }, \
	{ PASTEMAC2(z,s,op), PASTEMAC2(z,c,op), PASTEMAC2(z,d,op), PASTEMAC2(z,z,op) }  \
}


#define GENARRAY2_EXT(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES] = \
{ \
	{ PASTEMAC2(s,s,op), PASTEMAC2(s,c,op), NULL,              NULL,             }, \
	{ PASTEMAC2(c,s,op), PASTEMAC2(c,c,op), NULL,              NULL,             }, \
	{ NULL,              NULL,              PASTEMAC2(d,d,op), PASTEMAC2(d,z,op) }, \
	{ NULL,              NULL,              PASTEMAC2(z,d,op), PASTEMAC2(z,z,op) }  \
}


#define GENARRAY2_MIN(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES] = \
{ \
	{ PASTEMAC2(s,s,op), NULL,              NULL,              NULL,             }, \
	{ NULL,              PASTEMAC2(c,c,op), NULL,              NULL,             }, \
	{ NULL,              NULL,              PASTEMAC2(d,d,op), NULL,             }, \
	{ NULL,              NULL,              NULL,              PASTEMAC2(z,z,op) }  \
}


// -- Three-operand macros --


#define GENARRAY3_ALL(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES] = \
{ \
	{ \
	{ PASTEMAC3(s,s,s,op), PASTEMAC3(s,s,c,op), PASTEMAC3(s,s,d,op), PASTEMAC3(s,s,z,op) }, \
	{ PASTEMAC3(s,c,s,op), PASTEMAC3(s,c,c,op), PASTEMAC3(s,c,d,op), PASTEMAC3(s,c,z,op) }, \
	{ PASTEMAC3(s,d,s,op), PASTEMAC3(s,d,c,op), PASTEMAC3(s,d,d,op), PASTEMAC3(s,d,z,op) }, \
	{ PASTEMAC3(s,z,s,op), PASTEMAC3(s,z,c,op), PASTEMAC3(s,z,d,op), PASTEMAC3(s,z,z,op) }  \
	}, \
	{ \
	{ PASTEMAC3(c,s,s,op), PASTEMAC3(c,s,c,op), PASTEMAC3(c,s,d,op), PASTEMAC3(c,s,z,op) }, \
	{ PASTEMAC3(c,c,s,op), PASTEMAC3(c,c,c,op), PASTEMAC3(c,c,d,op), PASTEMAC3(c,c,z,op) }, \
	{ PASTEMAC3(c,d,s,op), PASTEMAC3(c,d,c,op), PASTEMAC3(c,d,d,op), PASTEMAC3(c,d,z,op) }, \
	{ PASTEMAC3(c,z,s,op), PASTEMAC3(c,z,c,op), PASTEMAC3(c,z,d,op), PASTEMAC3(c,z,z,op) }  \
	}, \
	{ \
	{ PASTEMAC3(d,s,s,op), PASTEMAC3(d,s,c,op), PASTEMAC3(d,s,d,op), PASTEMAC3(d,s,z,op) }, \
	{ PASTEMAC3(d,c,s,op), PASTEMAC3(d,c,c,op), PASTEMAC3(d,c,d,op), PASTEMAC3(d,c,z,op) }, \
	{ PASTEMAC3(d,d,s,op), PASTEMAC3(d,d,c,op), PASTEMAC3(d,d,d,op), PASTEMAC3(d,d,z,op) }, \
	{ PASTEMAC3(d,z,s,op), PASTEMAC3(d,z,c,op), PASTEMAC3(d,z,d,op), PASTEMAC3(d,z,z,op) }  \
	}, \
	{ \
	{ PASTEMAC3(z,s,s,op), PASTEMAC3(z,s,c,op), PASTEMAC3(z,s,d,op), PASTEMAC3(z,s,z,op) }, \
	{ PASTEMAC3(z,c,s,op), PASTEMAC3(z,c,c,op), PASTEMAC3(z,c,d,op), PASTEMAC3(z,c,z,op) }, \
	{ PASTEMAC3(z,d,s,op), PASTEMAC3(z,d,c,op), PASTEMAC3(z,d,d,op), PASTEMAC3(z,d,z,op) }, \
	{ PASTEMAC3(z,z,s,op), PASTEMAC3(z,z,c,op), PASTEMAC3(z,z,d,op), PASTEMAC3(z,z,z,op) }  \
	} \
}


#define GENARRAY3_EXT(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES] = \
{ \
	{ \
	{ PASTEMAC3(s,s,s,op), PASTEMAC3(s,s,c,op), NULL,                NULL,               }, \
	{ PASTEMAC3(s,c,s,op), PASTEMAC3(s,c,c,op), NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }  \
	}, \
	{ \
	{ PASTEMAC3(c,s,s,op), PASTEMAC3(c,s,c,op), NULL,                NULL,               }, \
	{ PASTEMAC3(c,c,s,op), PASTEMAC3(c,c,c,op), NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }  \
	}, \
	{ \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                PASTEMAC3(d,d,d,op), PASTEMAC3(d,d,z,op) }, \
	{ NULL,                NULL,                PASTEMAC3(d,z,d,op), PASTEMAC3(d,z,z,op) }  \
	}, \
	{ \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                PASTEMAC3(z,d,d,op), PASTEMAC3(z,d,z,op) }, \
	{ NULL,                NULL,                PASTEMAC3(z,z,d,op), PASTEMAC3(z,z,z,op) }  \
	} \
}


#define GENARRAY3_MIN(arrayname,op) \
\
arrayname[BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES][BLIS_NUM_FP_TYPES] = \
{ \
	{ \
	{ PASTEMAC3(s,s,s,op), NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }  \
	}, \
	{ \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                PASTEMAC3(c,c,c,op), NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }  \
	}, \
	{ \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                PASTEMAC3(d,d,d,op), NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }  \
	}, \
	{ \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                NULL,               }, \
	{ NULL,                NULL,                NULL,                PASTEMAC3(z,z,z,op) }  \
	} \
}


#endif
// end bli_genarray_macro_defs.h
// begin bli_gentdef_macro_defs.h


#ifndef BLIS_GENTDEF_MACRO_DEFS_H
#define BLIS_GENTDEF_MACRO_DEFS_H

//
// -- MACROS TO INSERT TYPEDEF-GENERATING MACROS -------------------------------
//


// -- function typedef macro (both typed and void) --

#define INSERT_GENTDEF( opname ) \
\
GENTDEF( float,    s, opname, _ft ) \
GENTDEF( double,   d, opname, _ft ) \
GENTDEF( scomplex, c, opname, _ft ) \
GENTDEF( dcomplex, z, opname, _ft ) \
\
GENTDEF( void,     s, opname, _vft ) \
GENTDEF( void,     d, opname, _vft ) \
GENTDEF( void,     c, opname, _vft ) \
GENTDEF( void,     z, opname, _vft ) \
\
GENTDEF( void,      , opname, _vft )

// -- function typedef macro (both typed and void) with real projection --

#define INSERT_GENTDEFR( opname ) \
\
GENTDEFR( float,    float,    s, s, opname, _ft ) \
GENTDEFR( double,   double,   d, d, opname, _ft ) \
GENTDEFR( scomplex, float,    c, s, opname, _ft ) \
GENTDEFR( dcomplex, double,   z, d, opname, _ft ) \
\
GENTDEFR( void,     void,     s, s, opname, _vft ) \
GENTDEFR( void,     void,     d, d, opname, _vft ) \
GENTDEFR( void,     void,     c, s, opname, _vft ) \
GENTDEFR( void,     void,     z, d, opname, _vft ) \
\
GENTDEFR( void,     void,      ,  , opname, _vft )


#endif
// end bli_gentdef_macro_defs.h
// begin bli_gentfunc_macro_defs.h



#ifndef BLIS_GENTFUNC_MACRO_DEFS_H
#define BLIS_GENTFUNC_MACRO_DEFS_H

//
// -- MACROS TO INSERT FUNCTION-GENERATING MACROS ------------------------------
//



// -- Macros for generating BLAS routines --------------------------------------


// -- Basic one-operand macro --


#define INSERT_GENTFUNC_BLAS( blasname, blisname ) \
\
GENTFUNC( float,    s, blasname, blisname ) \
GENTFUNC( double,   d, blasname, blisname ) \
GENTFUNC( scomplex, c, blasname, blisname ) \
GENTFUNC( dcomplex, z, blasname, blisname )


// -- Basic one-operand macro with real domain only --


#define INSERT_GENTFUNCRO_BLAS( blasname, blisname ) \
\
GENTFUNCRO( float,    s, blasname, blisname ) \
GENTFUNCRO( double,   d, blasname, blisname )


// -- Basic one-operand macro with complex domain only and real projection --


#define INSERT_GENTFUNCCO_BLAS( blasname, blisname ) \
\
GENTFUNCCO( scomplex, float,  c, s, blasname, blisname ) \
GENTFUNCCO( dcomplex, double, z, d, blasname, blisname )


// -- Basic one-operand macro with conjugation (used only for dot, ger) --


#define INSERT_GENTFUNCDOT_BLAS( blasname, blisname ) \
\
GENTFUNCDOT( float,    s,  , BLIS_NO_CONJUGATE, blasname, blisname ) \
GENTFUNCDOT( double,   d,  , BLIS_NO_CONJUGATE, blasname, blisname ) \
GENTFUNCDOT( scomplex, c, c, BLIS_CONJUGATE,    blasname, blisname ) \
GENTFUNCDOT( scomplex, c, u, BLIS_NO_CONJUGATE, blasname, blisname ) \
GENTFUNCDOT( dcomplex, z, c, BLIS_CONJUGATE,    blasname, blisname ) \
GENTFUNCDOT( dcomplex, z, u, BLIS_NO_CONJUGATE, blasname, blisname )


// -- Basic one-operand macro with real projection --


#define INSERT_GENTFUNCR_BLAS( rblasname, cblasname, blisname ) \
\
GENTFUNCR( float,    float,  s, s, rblasname, blisname ) \
GENTFUNCR( double,   double, d, d, rblasname, blisname ) \
GENTFUNCR( scomplex, float,  c, s, cblasname, blisname ) \
GENTFUNCR( dcomplex, double, z, d, cblasname, blisname )


// -- Alternate two-operand macro (one char for complex, one for real proj) --


#define INSERT_GENTFUNCR2_BLAS( blasname, blisname ) \
\
GENTFUNCR2( float,    float,  s,  , blasname, blisname ) \
GENTFUNCR2( double,   double, d,  , blasname, blisname ) \
GENTFUNCR2( scomplex, float,  c, s, blasname, blisname ) \
GENTFUNCR2( dcomplex, double, z, d, blasname, blisname )


// -- Extended two-operand macro (used only for scal) --


#define INSERT_GENTFUNCSCAL_BLAS( blasname, blisname ) \
\
GENTFUNCSCAL( float,    float,    s,  , blasname, blisname ) \
GENTFUNCSCAL( double,   double,   d,  , blasname, blisname ) \
GENTFUNCSCAL( scomplex, scomplex, c,  , blasname, blisname ) \
GENTFUNCSCAL( dcomplex, dcomplex, z,  , blasname, blisname ) \
GENTFUNCSCAL( scomplex, float,    c, s, blasname, blisname ) \
GENTFUNCSCAL( dcomplex, double,   z, d, blasname, blisname )




// -- Macros for functions with one operand ------------------------------------


// -- Basic one-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC_BASIC0( tfuncname ) \
\
GENTFUNC( float,    s, tfuncname ) \
GENTFUNC( double,   d, tfuncname ) \
GENTFUNC( scomplex, c, tfuncname ) \
GENTFUNC( dcomplex, z, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC_BASIC( tfuncname, varname ) \
\
GENTFUNC( float,    s, tfuncname, varname ) \
GENTFUNC( double,   d, tfuncname, varname ) \
GENTFUNC( scomplex, c, tfuncname, varname ) \
GENTFUNC( dcomplex, z, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNC_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTFUNC( float,    s, tfuncname, varname1, varname2 ) \
GENTFUNC( double,   d, tfuncname, varname1, varname2 ) \
GENTFUNC( scomplex, c, tfuncname, varname1, varname2 ) \
GENTFUNC( dcomplex, z, tfuncname, varname1, varname2 )

// -- (three auxiliary arguments) --

#define INSERT_GENTFUNC_BASIC3( tfuncname, varname1, varname2, varname3 ) \
\
GENTFUNC( float,    s, tfuncname, varname1, varname2, varname3 ) \
GENTFUNC( double,   d, tfuncname, varname1, varname2, varname3 ) \
GENTFUNC( scomplex, c, tfuncname, varname1, varname2, varname3 ) \
GENTFUNC( dcomplex, z, tfuncname, varname1, varname2, varname3 )

// -- (four auxiliary arguments) --

#define INSERT_GENTFUNC_BASIC4( tfuncname, varname1, varname2, varname3, varname4 ) \
\
GENTFUNC( float,    s, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTFUNC( double,   d, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTFUNC( scomplex, c, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTFUNC( dcomplex, z, tfuncname, varname1, varname2, varname3, varname4 )



// -- Basic one-operand with real projection --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNCR_BASIC0( tfuncname ) \
\
GENTFUNCR( float,    float,  s, s, tfuncname ) \
GENTFUNCR( double,   double, d, d, tfuncname ) \
GENTFUNCR( scomplex, float,  c, s, tfuncname ) \
GENTFUNCR( dcomplex, double, z, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNCR_BASIC( tfuncname, varname ) \
\
GENTFUNCR( float,    float,  s, s, tfuncname, varname ) \
GENTFUNCR( double,   double, d, d, tfuncname, varname ) \
GENTFUNCR( scomplex, float,  c, s, tfuncname, varname ) \
GENTFUNCR( dcomplex, double, z, d, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNCR_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTFUNCR( float,    float,  s, s, tfuncname, varname1, varname2 ) \
GENTFUNCR( double,   double, d, d, tfuncname, varname1, varname2 ) \
GENTFUNCR( scomplex, float,  c, s, tfuncname, varname1, varname2 ) \
GENTFUNCR( dcomplex, double, z, d, tfuncname, varname1, varname2 )

// -- (three auxiliary arguments) --

#define INSERT_GENTFUNCR_BASIC3( tfuncname, varname1, varname2, varname3  ) \
\
GENTFUNCR( float,    float,  s, s, tfuncname, varname1, varname2, varname3 ) \
GENTFUNCR( double,   double, d, d, tfuncname, varname1, varname2, varname3 ) \
GENTFUNCR( scomplex, float,  c, s, tfuncname, varname1, varname2, varname3 ) \
GENTFUNCR( dcomplex, double, z, d, tfuncname, varname1, varname2, varname3 )

// -- (four auxiliary arguments) --

#define INSERT_GENTFUNCR_BASIC4( tfuncname, varname1, varname2, varname3, varname4  ) \
\
GENTFUNCR( float,    float,  s, s, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTFUNCR( double,   double, d, d, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTFUNCR( scomplex, float,  c, s, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTFUNCR( dcomplex, double, z, d, tfuncname, varname1, varname2, varname3, varname4 )



// -- Basic one-operand macro with complex domain only and real projection --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNCCO_BASIC0( tfuncname ) \
\
GENTFUNCCO( scomplex, float,  c, s, tfuncname ) \
GENTFUNCCO( dcomplex, double, z, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNCCO_BASIC( tfuncname, varname ) \
\
GENTFUNCCO( scomplex, float,  c, s, tfuncname, varname ) \
GENTFUNCCO( dcomplex, double, z, d, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNCCO_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTFUNCCO( scomplex, float,  c, s, tfuncname, varname1, varname2 ) \
GENTFUNCCO( dcomplex, double, z, d, tfuncname, varname1, varname2 )

// -- (three auxiliary arguments) --

#define INSERT_GENTFUNCCO_BASIC3( tfuncname, varname1, varname2, varname3 ) \
\
GENTFUNCCO( scomplex, float,  c, s, tfuncname, varname1, varname2, varname3 ) \
GENTFUNCCO( dcomplex, double, z, d, tfuncname, varname1, varname2, varname3 )



// -- Basic one-operand macro with integer instance --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC_BASIC0_I( tfuncname ) \
\
GENTFUNC( float,    s, tfuncname ) \
GENTFUNC( double,   d, tfuncname ) \
GENTFUNC( scomplex, c, tfuncname ) \
GENTFUNC( dcomplex, z, tfuncname ) \
GENTFUNC( gint_t,   i, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC_BASIC_I( tfuncname, varname ) \
\
GENTFUNC( float,    s, tfuncname, varname ) \
GENTFUNC( double,   d, tfuncname, varname ) \
GENTFUNC( scomplex, c, tfuncname, varname ) \
GENTFUNC( dcomplex, z, tfuncname, varname ) \
GENTFUNC( gint_t,   i, tfuncname, varname )



// -- Basic one-operand with integer projection --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNCI_BASIC0( tfuncname ) \
\
GENTFUNCI( float,    gint_t, s, i, tfuncname ) \
GENTFUNCI( double,   gint_t, d, i, tfuncname ) \
GENTFUNCI( scomplex, gint_t, c, i, tfuncname ) \
GENTFUNCI( dcomplex, gint_t, z, i, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNCI_BASIC( tfuncname, varname ) \
\
GENTFUNCI( float,    gint_t, s, i, tfuncname, varname ) \
GENTFUNCI( double,   gint_t, d, i, tfuncname, varname ) \
GENTFUNCI( scomplex, gint_t, c, i, tfuncname, varname ) \
GENTFUNCI( dcomplex, gint_t, z, i, tfuncname, varname )



// -- Basic one-operand with real and integer projections --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNCRI_BASIC0( tfuncname ) \
\
GENTFUNCRI( float,    float,  gint_t, s, s, i, tfuncname ) \
GENTFUNCRI( double,   double, gint_t, d, d, i, tfuncname ) \
GENTFUNCRI( scomplex, float,  gint_t, c, s, i, tfuncname ) \
GENTFUNCRI( dcomplex, double, gint_t, z, d, i, tfuncname )




// -- Macros for functions with two primary operands ---------------------------


// -- Basic two-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC2_BASIC0( tfuncname ) \
\
GENTFUNC2( float,    float,    s, s, tfuncname ) \
GENTFUNC2( double,   double,   d, d, tfuncname ) \
GENTFUNC2( scomplex, scomplex, c, c, tfuncname ) \
GENTFUNC2( dcomplex, dcomplex, z, z, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC2_BASIC( tfuncname, varname ) \
\
GENTFUNC2( float,    float,    s, s, tfuncname, varname ) \
GENTFUNC2( double,   double,   d, d, tfuncname, varname ) \
GENTFUNC2( scomplex, scomplex, c, c, tfuncname, varname ) \
GENTFUNC2( dcomplex, dcomplex, z, z, tfuncname, varname )



// -- Mixed domain two-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC2_MIX_D0( tfuncname ) \
\
GENTFUNC2( float,    scomplex, s, c, tfuncname ) \
GENTFUNC2( scomplex, float,    c, s, tfuncname ) \
\
GENTFUNC2( double,   dcomplex, d, z, tfuncname ) \
GENTFUNC2( dcomplex, double,   z, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC2_MIX_D( tfuncname, varname ) \
\
GENTFUNC2( float,    scomplex, s, c, tfuncname, varname ) \
GENTFUNC2( scomplex, float,    c, s, tfuncname, varname ) \
\
GENTFUNC2( double,   dcomplex, d, z, tfuncname, varname ) \
GENTFUNC2( dcomplex, double,   z, d, tfuncname, varname )



// -- Mixed precision two-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC2_MIX_P0( tfuncname ) \
\
GENTFUNC2( float,    double,   s, d, tfuncname ) \
GENTFUNC2( float,    dcomplex, s, z, tfuncname ) \
\
GENTFUNC2( double,   float,    d, s, tfuncname ) \
GENTFUNC2( double,   scomplex, d, c, tfuncname ) \
\
GENTFUNC2( scomplex, double,   c, d, tfuncname ) \
GENTFUNC2( scomplex, dcomplex, c, z, tfuncname ) \
\
GENTFUNC2( dcomplex, float,    z, s, tfuncname ) \
GENTFUNC2( dcomplex, scomplex, z, c, tfuncname ) \

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC2_MIX_P( tfuncname, varname ) \
\
GENTFUNC2( float,    double,   s, d, tfuncname, varname ) \
GENTFUNC2( float,    dcomplex, s, z, tfuncname, varname ) \
\
GENTFUNC2( double,   float,    d, s, tfuncname, varname ) \
GENTFUNC2( double,   scomplex, d, c, tfuncname, varname ) \
\
GENTFUNC2( scomplex, double,   c, d, tfuncname, varname ) \
GENTFUNC2( scomplex, dcomplex, c, z, tfuncname, varname ) \
\
GENTFUNC2( dcomplex, float,    z, s, tfuncname, varname ) \
GENTFUNC2( dcomplex, scomplex, z, c, tfuncname, varname ) \



// -- Basic two-operand with real projection of first operand --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC2R_BASIC0( tfuncname ) \
\
GENTFUNC2R( float,    float,    float,    s, s, s, tfuncname ) \
GENTFUNC2R( double,   double,   double,   d, d, d, tfuncname ) \
GENTFUNC2R( scomplex, scomplex, float,    c, c, s, tfuncname ) \
GENTFUNC2R( dcomplex, dcomplex, double,   z, z, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC2R_BASIC( tfuncname, varname ) \
\
GENTFUNC2R( float,    float,    float,    s, s, s, tfuncname, varname ) \
GENTFUNC2R( double,   double,   double,   d, d, d, tfuncname, varname ) \
GENTFUNC2R( scomplex, scomplex, float,    c, c, s, tfuncname, varname ) \
GENTFUNC2R( dcomplex, dcomplex, double,   z, z, d, tfuncname, varname )



// -- Mixed domain two-operand with real projection of first operand --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC2R_MIX_D0( tfuncname ) \
\
GENTFUNC2R( float,    scomplex, float,    s, c, s, tfuncname ) \
GENTFUNC2R( scomplex, float,    float,    c, s, s, tfuncname ) \
\
GENTFUNC2R( double,   dcomplex, double,   d, z, d, tfuncname ) \
GENTFUNC2R( dcomplex, double,   double,   z, d, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC2R_MIX_D( tfuncname, varname ) \
\
GENTFUNC2R( float,    scomplex, float,    s, c, s, tfuncname, varname ) \
GENTFUNC2R( scomplex, float,    float,    c, s, s, tfuncname, varname ) \
\
GENTFUNC2R( double,   dcomplex, double,   d, z, d, tfuncname, varname ) \
GENTFUNC2R( dcomplex, double,   double,   z, d, d, tfuncname, varname )



// -- Mixed precision two-operand with real projection of first operand --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC2R_MIX_P0( tfuncname ) \
\
GENTFUNC2R( float,    double,   float,    s, d, s, tfuncname ) \
GENTFUNC2R( float,    dcomplex, float,    s, z, s, tfuncname ) \
\
GENTFUNC2R( double,   float,    double,   d, s, d, tfuncname ) \
GENTFUNC2R( double,   scomplex, double,   d, c, d, tfuncname ) \
\
GENTFUNC2R( scomplex, double,   float,    c, d, s, tfuncname ) \
GENTFUNC2R( scomplex, dcomplex, float,    c, z, s, tfuncname ) \
\
GENTFUNC2R( dcomplex, float,    double,   z, s, d, tfuncname ) \
GENTFUNC2R( dcomplex, scomplex, double,   z, c, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC2R_MIX_P( tfuncname, varname ) \
\
GENTFUNC2R( float,    double,   float,    s, d, s, tfuncname, varname ) \
GENTFUNC2R( float,    dcomplex, float,    s, z, s, tfuncname, varname ) \
\
GENTFUNC2R( double,   float,    double,   d, s, d, tfuncname, varname ) \
GENTFUNC2R( double,   scomplex, double,   d, c, d, tfuncname, varname ) \
\
GENTFUNC2R( scomplex, double,   float,    c, d, s, tfuncname, varname ) \
GENTFUNC2R( scomplex, dcomplex, float,    c, z, s, tfuncname, varname ) \
\
GENTFUNC2R( dcomplex, float,    double,   z, s, d, tfuncname, varname ) \
GENTFUNC2R( dcomplex, scomplex, double,   z, c, d, tfuncname, varname )



// -- Macros for functions with three primary operands -------------------------


// -- Basic three-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC3_BASIC0( tfuncname ) \
\
GENTFUNC3( float,    float,    float,    s, s, s, tfuncname ) \
GENTFUNC3( double,   double,   double,   d, d, d, tfuncname ) \
GENTFUNC3( scomplex, scomplex, scomplex, c, c, c, tfuncname ) \
GENTFUNC3( dcomplex, dcomplex, dcomplex, z, z, z, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC3_BASIC( tfuncname, varname ) \
\
GENTFUNC3( float,    float,    float,    s, s, s, tfuncname, varname ) \
GENTFUNC3( double,   double,   double,   d, d, d, tfuncname, varname ) \
GENTFUNC3( scomplex, scomplex, scomplex, c, c, c, tfuncname, varname ) \
GENTFUNC3( dcomplex, dcomplex, dcomplex, z, z, z, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNC3_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTFUNC3( float,    float,    float,    s, s, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   double,   double,   d, d, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, scomplex, scomplex, c, c, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, dcomplex, dcomplex, z, z, z, tfuncname, varname1, varname2 )



// -- Mixed domain three-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC3_MIX_D0( tfuncname ) \
\
GENTFUNC3( float,    float,    scomplex, s, s, c, tfuncname ) \
GENTFUNC3( float,    scomplex, float,    s, c, s, tfuncname ) \
GENTFUNC3( float,    scomplex, scomplex, s, c, c, tfuncname ) \
\
GENTFUNC3( double,   double,   dcomplex, d, d, z, tfuncname ) \
GENTFUNC3( double,   dcomplex, double,   d, z, d, tfuncname ) \
GENTFUNC3( double,   dcomplex, dcomplex, d, z, z, tfuncname ) \
\
GENTFUNC3( scomplex, float,    float,    c, s, s, tfuncname ) \
GENTFUNC3( scomplex, float,    scomplex, c, s, c, tfuncname ) \
GENTFUNC3( scomplex, scomplex, float,    c, c, s, tfuncname ) \
\
GENTFUNC3( dcomplex, double,   double,   z, d, d, tfuncname ) \
GENTFUNC3( dcomplex, double,   dcomplex, z, d, z, tfuncname ) \
GENTFUNC3( dcomplex, dcomplex, double,   z, z, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC3_MIX_D( tfuncname, varname ) \
\
GENTFUNC3( float,    float,    scomplex, s, s, c, tfuncname, varname ) \
GENTFUNC3( float,    scomplex, float,    s, c, s, tfuncname, varname ) \
GENTFUNC3( float,    scomplex, scomplex, s, c, c, tfuncname, varname ) \
\
GENTFUNC3( double,   double,   dcomplex, d, d, z, tfuncname, varname ) \
GENTFUNC3( double,   dcomplex, double,   d, z, d, tfuncname, varname ) \
GENTFUNC3( double,   dcomplex, dcomplex, d, z, z, tfuncname, varname ) \
\
GENTFUNC3( scomplex, float,    float,    c, s, s, tfuncname, varname ) \
GENTFUNC3( scomplex, float,    scomplex, c, s, c, tfuncname, varname ) \
GENTFUNC3( scomplex, scomplex, float,    c, c, s, tfuncname, varname ) \
\
GENTFUNC3( dcomplex, double,   double,   z, d, d, tfuncname, varname ) \
GENTFUNC3( dcomplex, double,   dcomplex, z, d, z, tfuncname, varname ) \
GENTFUNC3( dcomplex, dcomplex, double,   z, z, d, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNC3_MIX_D2( tfuncname, varname1, varname2 ) \
\
GENTFUNC3( float,    float,    scomplex, s, s, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    scomplex, float,    s, c, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    scomplex, scomplex, s, c, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( double,   double,   dcomplex, d, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   dcomplex, double,   d, z, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   dcomplex, dcomplex, d, z, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( scomplex, float,    float,    c, s, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, float,    scomplex, c, s, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, scomplex, float,    c, c, s, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( dcomplex, double,   double,   z, d, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, double,   dcomplex, z, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, dcomplex, double,   z, z, d, tfuncname, varname1, varname2 )



// -- Mixed precision three-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC3_MIX_P0( tfuncname ) \
\
GENTFUNC3( float,    float,    double,   s, s, d, tfuncname ) \
GENTFUNC3( float,    float,    dcomplex, s, s, z, tfuncname ) \
\
GENTFUNC3( float,    double,   float,    s, d, s, tfuncname ) \
GENTFUNC3( float,    double,   double,   s, d, d, tfuncname ) \
GENTFUNC3( float,    double,   scomplex, s, d, c, tfuncname ) \
GENTFUNC3( float,    double,   dcomplex, s, d, z, tfuncname ) \
\
GENTFUNC3( float,    scomplex, double,   s, c, d, tfuncname ) \
GENTFUNC3( float,    scomplex, dcomplex, s, c, z, tfuncname ) \
\
GENTFUNC3( float,    dcomplex, float,    s, z, s, tfuncname ) \
GENTFUNC3( float,    dcomplex, double,   s, z, d, tfuncname ) \
GENTFUNC3( float,    dcomplex, scomplex, s, z, c, tfuncname ) \
GENTFUNC3( float,    dcomplex, dcomplex, s, z, z, tfuncname ) \
\
\
GENTFUNC3( double,   float,    float,    d, s, s, tfuncname ) \
GENTFUNC3( double,   float,    double,   d, s, d, tfuncname ) \
GENTFUNC3( double,   float,    scomplex, d, s, c, tfuncname ) \
GENTFUNC3( double,   float,    dcomplex, d, s, z, tfuncname ) \
\
GENTFUNC3( double,   double,   float,    d, d, s, tfuncname ) \
GENTFUNC3( double,   double,   scomplex, d, d, c, tfuncname ) \
\
GENTFUNC3( double,   scomplex, float,    d, c, s, tfuncname ) \
GENTFUNC3( double,   scomplex, double,   d, c, d, tfuncname ) \
GENTFUNC3( double,   scomplex, scomplex, d, c, c, tfuncname ) \
GENTFUNC3( double,   scomplex, dcomplex, d, c, z, tfuncname ) \
\
GENTFUNC3( double,   dcomplex, float,    d, z, s, tfuncname ) \
GENTFUNC3( double,   dcomplex, scomplex, d, z, c, tfuncname ) \
\
\
GENTFUNC3( scomplex, float,    double,   c, s, d, tfuncname ) \
GENTFUNC3( scomplex, float,    dcomplex, c, s, z, tfuncname ) \
\
GENTFUNC3( scomplex, double,   float,    c, d, s, tfuncname ) \
GENTFUNC3( scomplex, double,   double,   c, d, d, tfuncname ) \
GENTFUNC3( scomplex, double,   scomplex, c, d, c, tfuncname ) \
GENTFUNC3( scomplex, double,   dcomplex, c, d, z, tfuncname ) \
\
GENTFUNC3( scomplex, scomplex, double,   c, c, d, tfuncname ) \
GENTFUNC3( scomplex, scomplex, dcomplex, c, c, z, tfuncname ) \
\
GENTFUNC3( scomplex, dcomplex, float,    c, z, s, tfuncname ) \
GENTFUNC3( scomplex, dcomplex, double,   c, z, d, tfuncname ) \
GENTFUNC3( scomplex, dcomplex, scomplex, c, z, c, tfuncname ) \
GENTFUNC3( scomplex, dcomplex, dcomplex, c, z, z, tfuncname ) \
\
\
GENTFUNC3( dcomplex, float,    float,    z, s, s, tfuncname ) \
GENTFUNC3( dcomplex, float,    double,   z, s, d, tfuncname ) \
GENTFUNC3( dcomplex, float,    scomplex, z, s, c, tfuncname ) \
GENTFUNC3( dcomplex, float,    dcomplex, z, s, z, tfuncname ) \
\
GENTFUNC3( dcomplex, double,   float,    z, d, s, tfuncname ) \
GENTFUNC3( dcomplex, double,   scomplex, z, d, c, tfuncname ) \
\
GENTFUNC3( dcomplex, scomplex, float,    z, c, s, tfuncname ) \
GENTFUNC3( dcomplex, scomplex, double,   z, c, d, tfuncname ) \
GENTFUNC3( dcomplex, scomplex, scomplex, z, c, c, tfuncname ) \
GENTFUNC3( dcomplex, scomplex, dcomplex, z, c, z, tfuncname ) \
\
GENTFUNC3( dcomplex, dcomplex, float,    z, z, s, tfuncname ) \
GENTFUNC3( dcomplex, dcomplex, scomplex, z, z, c, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC3_MIX_P( tfuncname, varname ) \
\
GENTFUNC3( float,    float,    double,   s, s, d, tfuncname, varname ) \
GENTFUNC3( float,    float,    dcomplex, s, s, z, tfuncname, varname ) \
\
GENTFUNC3( float,    double,   float,    s, d, s, tfuncname, varname ) \
GENTFUNC3( float,    double,   double,   s, d, d, tfuncname, varname ) \
GENTFUNC3( float,    double,   scomplex, s, d, c, tfuncname, varname ) \
GENTFUNC3( float,    double,   dcomplex, s, d, z, tfuncname, varname ) \
\
GENTFUNC3( float,    scomplex, double,   s, c, d, tfuncname, varname ) \
GENTFUNC3( float,    scomplex, dcomplex, s, c, z, tfuncname, varname ) \
\
GENTFUNC3( float,    dcomplex, float,    s, z, s, tfuncname, varname ) \
GENTFUNC3( float,    dcomplex, double,   s, z, d, tfuncname, varname ) \
GENTFUNC3( float,    dcomplex, scomplex, s, z, c, tfuncname, varname ) \
GENTFUNC3( float,    dcomplex, dcomplex, s, z, z, tfuncname, varname ) \
\
\
GENTFUNC3( double,   float,    float,    d, s, s, tfuncname, varname ) \
GENTFUNC3( double,   float,    double,   d, s, d, tfuncname, varname ) \
GENTFUNC3( double,   float,    scomplex, d, s, c, tfuncname, varname ) \
GENTFUNC3( double,   float,    dcomplex, d, s, z, tfuncname, varname ) \
\
GENTFUNC3( double,   double,   float,    d, d, s, tfuncname, varname ) \
GENTFUNC3( double,   double,   scomplex, d, d, c, tfuncname, varname ) \
\
GENTFUNC3( double,   scomplex, float,    d, c, s, tfuncname, varname ) \
GENTFUNC3( double,   scomplex, double,   d, c, d, tfuncname, varname ) \
GENTFUNC3( double,   scomplex, scomplex, d, c, c, tfuncname, varname ) \
GENTFUNC3( double,   scomplex, dcomplex, d, c, z, tfuncname, varname ) \
\
GENTFUNC3( double,   dcomplex, float,    d, z, s, tfuncname, varname ) \
GENTFUNC3( double,   dcomplex, scomplex, d, z, c, tfuncname, varname ) \
\
\
GENTFUNC3( scomplex, float,    double,   c, s, d, tfuncname, varname ) \
GENTFUNC3( scomplex, float,    dcomplex, c, s, z, tfuncname, varname ) \
\
GENTFUNC3( scomplex, double,   float,    c, d, s, tfuncname, varname ) \
GENTFUNC3( scomplex, double,   double,   c, d, d, tfuncname, varname ) \
GENTFUNC3( scomplex, double,   scomplex, c, d, c, tfuncname, varname ) \
GENTFUNC3( scomplex, double,   dcomplex, c, d, z, tfuncname, varname ) \
\
GENTFUNC3( scomplex, scomplex, double,   c, c, d, tfuncname, varname ) \
GENTFUNC3( scomplex, scomplex, dcomplex, c, c, z, tfuncname, varname ) \
\
GENTFUNC3( scomplex, dcomplex, float,    c, z, s, tfuncname, varname ) \
GENTFUNC3( scomplex, dcomplex, double,   c, z, d, tfuncname, varname ) \
GENTFUNC3( scomplex, dcomplex, scomplex, c, z, c, tfuncname, varname ) \
GENTFUNC3( scomplex, dcomplex, dcomplex, c, z, z, tfuncname, varname ) \
\
\
GENTFUNC3( dcomplex, float,    float,    z, s, s, tfuncname, varname ) \
GENTFUNC3( dcomplex, float,    double,   z, s, d, tfuncname, varname ) \
GENTFUNC3( dcomplex, float,    scomplex, z, s, c, tfuncname, varname ) \
GENTFUNC3( dcomplex, float,    dcomplex, z, s, z, tfuncname, varname ) \
\
GENTFUNC3( dcomplex, double,   float,    z, d, s, tfuncname, varname ) \
GENTFUNC3( dcomplex, double,   scomplex, z, d, c, tfuncname, varname ) \
\
GENTFUNC3( dcomplex, scomplex, float,    z, c, s, tfuncname, varname ) \
GENTFUNC3( dcomplex, scomplex, double,   z, c, d, tfuncname, varname ) \
GENTFUNC3( dcomplex, scomplex, scomplex, z, c, c, tfuncname, varname ) \
GENTFUNC3( dcomplex, scomplex, dcomplex, z, c, z, tfuncname, varname ) \
\
GENTFUNC3( dcomplex, dcomplex, float,    z, z, s, tfuncname, varname ) \
GENTFUNC3( dcomplex, dcomplex, scomplex, z, z, c, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNC3_MIX_P2( tfuncname, varname1, varname2 ) \
\
GENTFUNC3( float,    float,    double,   s, s, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    float,    dcomplex, s, s, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( float,    double,   float,    s, d, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    double,   double,   s, d, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    double,   scomplex, s, d, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    double,   dcomplex, s, d, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( float,    scomplex, double,   s, c, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    scomplex, dcomplex, s, c, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( float,    dcomplex, float,    s, z, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    dcomplex, double,   s, z, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    dcomplex, scomplex, s, z, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( float,    dcomplex, dcomplex, s, z, z, tfuncname, varname1, varname2 ) \
\
\
GENTFUNC3( double,   float,    float,    d, s, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   float,    double,   d, s, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   float,    scomplex, d, s, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   float,    dcomplex, d, s, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( double,   double,   float,    d, d, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   double,   scomplex, d, d, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( double,   scomplex, float,    d, c, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   scomplex, double,   d, c, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   scomplex, scomplex, d, c, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   scomplex, dcomplex, d, c, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( double,   dcomplex, float,    d, z, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( double,   dcomplex, scomplex, d, z, c, tfuncname, varname1, varname2 ) \
\
\
GENTFUNC3( scomplex, float,    double,   c, s, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, float,    dcomplex, c, s, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( scomplex, double,   float,    c, d, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, double,   double,   c, d, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, double,   scomplex, c, d, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, double,   dcomplex, c, d, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( scomplex, scomplex, double,   c, c, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, scomplex, dcomplex, c, c, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( scomplex, dcomplex, float,    c, z, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, dcomplex, double,   c, z, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, dcomplex, scomplex, c, z, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( scomplex, dcomplex, dcomplex, c, z, z, tfuncname, varname1, varname2 ) \
\
\
GENTFUNC3( dcomplex, float,    float,    z, s, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, float,    double,   z, s, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, float,    scomplex, z, s, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, float,    dcomplex, z, s, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( dcomplex, double,   float,    z, d, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, double,   scomplex, z, d, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( dcomplex, scomplex, float,    z, c, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, scomplex, double,   z, c, d, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, scomplex, scomplex, z, c, c, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, scomplex, dcomplex, z, c, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3( dcomplex, dcomplex, float,    z, z, s, tfuncname, varname1, varname2 ) \
GENTFUNC3( dcomplex, dcomplex, scomplex, z, z, c, tfuncname, varname1, varname2 )



// -- Basic three-operand with union of operands 1 and 2 --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC3U12_BASIC0( tfuncname ) \
\
GENTFUNC3U12( float,    float,    float,    float,    s, s, s, s, tfuncname ) \
GENTFUNC3U12( double,   double,   double,   double,   d, d, d, d, tfuncname ) \
GENTFUNC3U12( scomplex, scomplex, scomplex, scomplex, c, c, c, c, tfuncname ) \
GENTFUNC3U12( dcomplex, dcomplex, dcomplex, dcomplex, z, z, z, z, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC3U12_BASIC( tfuncname, varname ) \
\
GENTFUNC3U12( float,    float,    float,    float,    s, s, s, s, tfuncname, varname ) \
GENTFUNC3U12( double,   double,   double,   double,   d, d, d, d, tfuncname, varname ) \
GENTFUNC3U12( scomplex, scomplex, scomplex, scomplex, c, c, c, c, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, dcomplex, dcomplex, dcomplex, z, z, z, z, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNC3U12_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( float,    float,    float,    float,    s, s, s, s, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   double,   double,   double,   d, d, d, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, scomplex, scomplex, scomplex, c, c, c, c, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, dcomplex, dcomplex, dcomplex, z, z, z, z, tfuncname, varname1, varname2 )



// -- Mixed domain three-operand with union of operands 1 and 2 --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC3U12_MIX_D0( tfuncname ) \
\
GENTFUNC3U12( float,    float,    scomplex, float,    s, s, c, s, tfuncname ) \
GENTFUNC3U12( float,    scomplex, float,    scomplex, s, c, s, c, tfuncname ) \
GENTFUNC3U12( float,    scomplex, scomplex, scomplex, s, c, c, c, tfuncname ) \
\
GENTFUNC3U12( double,   double,   dcomplex, double,   d, d, z, d, tfuncname ) \
GENTFUNC3U12( double,   dcomplex, double,   dcomplex, d, z, d, z, tfuncname ) \
GENTFUNC3U12( double,   dcomplex, dcomplex, dcomplex, d, z, z, z, tfuncname ) \
\
GENTFUNC3U12( scomplex, float,    float,    scomplex, c, s, s, c, tfuncname ) \
GENTFUNC3U12( scomplex, float,    scomplex, scomplex, c, s, c, c, tfuncname ) \
GENTFUNC3U12( scomplex, scomplex, float,    scomplex, c, c, s, c, tfuncname ) \
\
GENTFUNC3U12( dcomplex, double,   double,   dcomplex, z, d, d, z, tfuncname ) \
GENTFUNC3U12( dcomplex, double,   dcomplex, dcomplex, z, d, z, z, tfuncname ) \
GENTFUNC3U12( dcomplex, dcomplex, double,   dcomplex, z, z, d, z, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC3U12_MIX_D( tfuncname, varname ) \
\
GENTFUNC3U12( float,    float,    scomplex, float,    s, s, c, s, tfuncname, varname ) \
GENTFUNC3U12( float,    scomplex, float,    scomplex, s, c, s, c, tfuncname, varname ) \
GENTFUNC3U12( float,    scomplex, scomplex, scomplex, s, c, c, c, tfuncname, varname ) \
\
GENTFUNC3U12( double,   double,   dcomplex, double,   d, d, z, d, tfuncname, varname ) \
GENTFUNC3U12( double,   dcomplex, double,   dcomplex, d, z, d, z, tfuncname, varname ) \
GENTFUNC3U12( double,   dcomplex, dcomplex, dcomplex, d, z, z, z, tfuncname, varname ) \
\
GENTFUNC3U12( scomplex, float,    float,    scomplex, c, s, s, c, tfuncname, varname ) \
GENTFUNC3U12( scomplex, float,    scomplex, scomplex, c, s, c, c, tfuncname, varname ) \
GENTFUNC3U12( scomplex, scomplex, float,    scomplex, c, c, s, c, tfuncname, varname ) \
\
GENTFUNC3U12( dcomplex, double,   double,   dcomplex, z, d, d, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, double,   dcomplex, dcomplex, z, d, z, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, dcomplex, double,   dcomplex, z, z, d, z, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNC3U12_MIX_D2( tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( float,    float,    scomplex, float,    s, s, c, s, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    scomplex, float,    scomplex, s, c, s, c, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    scomplex, scomplex, scomplex, s, c, c, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( double,   double,   dcomplex, double,   d, d, z, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   dcomplex, double,   dcomplex, d, z, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   dcomplex, dcomplex, dcomplex, d, z, z, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( scomplex, float,    float,    scomplex, c, s, s, c, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, float,    scomplex, scomplex, c, s, c, c, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, scomplex, float,    scomplex, c, c, s, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( dcomplex, double,   double,   dcomplex, z, d, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, double,   dcomplex, dcomplex, z, d, z, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, dcomplex, double,   dcomplex, z, z, d, z, tfuncname, varname1, varname2 )



// -- Mixed precision three-operand with union of operands 1 and 2 --

// -- (no auxiliary arguments) --

#define INSERT_GENTFUNC3U12_MIX_P0( tfuncname ) \
\
GENTFUNC3U12( float,    float,    double,   float,    s, s, d, s, tfuncname ) \
GENTFUNC3U12( float,    float,    dcomplex, float,    s, s, z, s, tfuncname ) \
\
GENTFUNC3U12( float,    double,   float,    double,   s, d, s, d, tfuncname ) \
GENTFUNC3U12( float,    double,   double,   double,   s, d, d, d, tfuncname ) \
GENTFUNC3U12( float,    double,   scomplex, double,   s, d, c, d, tfuncname ) \
GENTFUNC3U12( float,    double,   dcomplex, double,   s, d, z, d, tfuncname ) \
\
GENTFUNC3U12( float,    scomplex, double,   scomplex, s, c, d, c, tfuncname ) \
GENTFUNC3U12( float,    scomplex, dcomplex, scomplex, s, c, z, c, tfuncname ) \
\
GENTFUNC3U12( float,    dcomplex, float,    dcomplex, s, z, s, z, tfuncname ) \
GENTFUNC3U12( float,    dcomplex, double,   dcomplex, s, z, d, z, tfuncname ) \
GENTFUNC3U12( float,    dcomplex, scomplex, dcomplex, s, z, c, z, tfuncname ) \
GENTFUNC3U12( float,    dcomplex, dcomplex, dcomplex, s, z, z, z, tfuncname ) \
\
\
GENTFUNC3U12( double,   float,    float,    double,   d, s, s, d, tfuncname ) \
GENTFUNC3U12( double,   float,    double,   double,   d, s, d, d, tfuncname ) \
GENTFUNC3U12( double,   float,    scomplex, double,   d, s, c, d, tfuncname ) \
GENTFUNC3U12( double,   float,    dcomplex, double,   d, s, z, d, tfuncname ) \
\
GENTFUNC3U12( double,   double,   float,    double,   d, d, s, d, tfuncname ) \
GENTFUNC3U12( double,   double,   scomplex, double,   d, d, c, d, tfuncname ) \
\
GENTFUNC3U12( double,   scomplex, float,    dcomplex, d, c, s, z, tfuncname ) \
GENTFUNC3U12( double,   scomplex, double,   dcomplex, d, c, d, z, tfuncname ) \
GENTFUNC3U12( double,   scomplex, scomplex, dcomplex, d, c, c, z, tfuncname ) \
GENTFUNC3U12( double,   scomplex, dcomplex, dcomplex, d, c, z, z, tfuncname ) \
\
GENTFUNC3U12( double,   dcomplex, float,    dcomplex, d, z, s, z, tfuncname ) \
GENTFUNC3U12( double,   dcomplex, scomplex, dcomplex, d, z, c, z, tfuncname ) \
\
\
GENTFUNC3U12( scomplex, float,    double,   scomplex, c, s, d, c, tfuncname ) \
GENTFUNC3U12( scomplex, float,    dcomplex, scomplex, c, s, z, c, tfuncname ) \
\
GENTFUNC3U12( scomplex, double,   float,    dcomplex, c, d, s, z, tfuncname ) \
GENTFUNC3U12( scomplex, double,   double,   dcomplex, c, d, d, z, tfuncname ) \
GENTFUNC3U12( scomplex, double,   scomplex, dcomplex, c, d, c, z, tfuncname ) \
GENTFUNC3U12( scomplex, double,   dcomplex, dcomplex, c, d, z, z, tfuncname ) \
\
GENTFUNC3U12( scomplex, scomplex, double,   scomplex, c, c, d, c, tfuncname ) \
GENTFUNC3U12( scomplex, scomplex, dcomplex, scomplex, c, c, z, c, tfuncname ) \
\
GENTFUNC3U12( scomplex, dcomplex, float,    dcomplex, c, z, s, z, tfuncname ) \
GENTFUNC3U12( scomplex, dcomplex, double,   dcomplex, c, z, d, z, tfuncname ) \
GENTFUNC3U12( scomplex, dcomplex, scomplex, dcomplex, c, z, c, z, tfuncname ) \
GENTFUNC3U12( scomplex, dcomplex, dcomplex, dcomplex, c, z, z, z, tfuncname ) \
\
\
GENTFUNC3U12( dcomplex, float,    float,    dcomplex, z, s, s, z, tfuncname ) \
GENTFUNC3U12( dcomplex, float,    double,   dcomplex, z, s, d, z, tfuncname ) \
GENTFUNC3U12( dcomplex, float,    scomplex, dcomplex, z, s, c, z, tfuncname ) \
GENTFUNC3U12( dcomplex, float,    dcomplex, dcomplex, z, s, z, z, tfuncname ) \
\
GENTFUNC3U12( dcomplex, double,   float,    dcomplex, z, d, s, z, tfuncname ) \
GENTFUNC3U12( dcomplex, double,   scomplex, dcomplex, z, d, c, z, tfuncname ) \
\
GENTFUNC3U12( dcomplex, scomplex, float,    dcomplex, z, c, s, z, tfuncname ) \
GENTFUNC3U12( dcomplex, scomplex, double,   dcomplex, z, c, d, z, tfuncname ) \
GENTFUNC3U12( dcomplex, scomplex, scomplex, dcomplex, z, c, c, z, tfuncname ) \
GENTFUNC3U12( dcomplex, scomplex, dcomplex, dcomplex, z, c, z, z, tfuncname ) \
\
GENTFUNC3U12( dcomplex, dcomplex, float,    dcomplex, z, z, s, z, tfuncname ) \
GENTFUNC3U12( dcomplex, dcomplex, scomplex, dcomplex, z, z, c, z, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTFUNC3U12_MIX_P( tfuncname, varname ) \
\
GENTFUNC3U12( float,    float,    double,   float,    s, s, d, s, tfuncname, varname ) \
GENTFUNC3U12( float,    float,    dcomplex, float,    s, s, z, s, tfuncname, varname ) \
\
GENTFUNC3U12( float,    double,   float,    double,   s, d, s, d, tfuncname, varname ) \
GENTFUNC3U12( float,    double,   double,   double,   s, d, d, d, tfuncname, varname ) \
GENTFUNC3U12( float,    double,   scomplex, double,   s, d, c, d, tfuncname, varname ) \
GENTFUNC3U12( float,    double,   dcomplex, double,   s, d, z, d, tfuncname, varname ) \
\
GENTFUNC3U12( float,    scomplex, double,   scomplex, s, c, d, c, tfuncname, varname ) \
GENTFUNC3U12( float,    scomplex, dcomplex, scomplex, s, c, z, c, tfuncname, varname ) \
\
GENTFUNC3U12( float,    dcomplex, float,    dcomplex, s, z, s, z, tfuncname, varname ) \
GENTFUNC3U12( float,    dcomplex, double,   dcomplex, s, z, d, z, tfuncname, varname ) \
GENTFUNC3U12( float,    dcomplex, scomplex, dcomplex, s, z, c, z, tfuncname, varname ) \
GENTFUNC3U12( float,    dcomplex, dcomplex, dcomplex, s, z, z, z, tfuncname, varname ) \
\
\
GENTFUNC3U12( double,   float,    float,    double,   d, s, s, d, tfuncname, varname ) \
GENTFUNC3U12( double,   float,    double,   double,   d, s, d, d, tfuncname, varname ) \
GENTFUNC3U12( double,   float,    scomplex, double,   d, s, c, d, tfuncname, varname ) \
GENTFUNC3U12( double,   float,    dcomplex, double,   d, s, z, d, tfuncname, varname ) \
\
GENTFUNC3U12( double,   double,   float,    double,   d, d, s, d, tfuncname, varname ) \
GENTFUNC3U12( double,   double,   scomplex, double,   d, d, c, d, tfuncname, varname ) \
\
GENTFUNC3U12( double,   scomplex, float,    dcomplex, d, c, s, z, tfuncname, varname ) \
GENTFUNC3U12( double,   scomplex, double,   dcomplex, d, c, d, z, tfuncname, varname ) \
GENTFUNC3U12( double,   scomplex, scomplex, dcomplex, d, c, c, z, tfuncname, varname ) \
GENTFUNC3U12( double,   scomplex, dcomplex, dcomplex, d, c, z, z, tfuncname, varname ) \
\
GENTFUNC3U12( double,   dcomplex, float,    dcomplex, d, z, s, z, tfuncname, varname ) \
GENTFUNC3U12( double,   dcomplex, scomplex, dcomplex, d, z, c, z, tfuncname, varname ) \
\
\
GENTFUNC3U12( scomplex, float,    double,   scomplex, c, s, d, c, tfuncname, varname ) \
GENTFUNC3U12( scomplex, float,    dcomplex, scomplex, c, s, z, c, tfuncname, varname ) \
\
GENTFUNC3U12( scomplex, double,   float,    dcomplex, c, d, s, z, tfuncname, varname ) \
GENTFUNC3U12( scomplex, double,   double,   dcomplex, c, d, d, z, tfuncname, varname ) \
GENTFUNC3U12( scomplex, double,   scomplex, dcomplex, c, d, c, z, tfuncname, varname ) \
GENTFUNC3U12( scomplex, double,   dcomplex, dcomplex, c, d, z, z, tfuncname, varname ) \
\
GENTFUNC3U12( scomplex, scomplex, double,   scomplex, c, c, d, c, tfuncname, varname ) \
GENTFUNC3U12( scomplex, scomplex, dcomplex, scomplex, c, c, z, c, tfuncname, varname ) \
\
GENTFUNC3U12( scomplex, dcomplex, float,    dcomplex, c, z, s, z, tfuncname, varname ) \
GENTFUNC3U12( scomplex, dcomplex, double,   dcomplex, c, z, d, z, tfuncname, varname ) \
GENTFUNC3U12( scomplex, dcomplex, scomplex, dcomplex, c, z, c, z, tfuncname, varname ) \
GENTFUNC3U12( scomplex, dcomplex, dcomplex, dcomplex, c, z, z, z, tfuncname, varname ) \
\
\
GENTFUNC3U12( dcomplex, float,    float,    dcomplex, z, s, s, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, float,    double,   dcomplex, z, s, d, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, float,    scomplex, dcomplex, z, s, c, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, float,    dcomplex, dcomplex, z, s, z, z, tfuncname, varname ) \
\
GENTFUNC3U12( dcomplex, double,   float,    dcomplex, z, d, s, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, double,   scomplex, dcomplex, z, d, c, z, tfuncname, varname ) \
\
GENTFUNC3U12( dcomplex, scomplex, float,    dcomplex, z, c, s, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, scomplex, double,   dcomplex, z, c, d, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, scomplex, scomplex, dcomplex, z, c, c, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, scomplex, dcomplex, dcomplex, z, c, z, z, tfuncname, varname ) \
\
GENTFUNC3U12( dcomplex, dcomplex, float,    dcomplex, z, z, s, z, tfuncname, varname ) \
GENTFUNC3U12( dcomplex, dcomplex, scomplex, dcomplex, z, z, c, z, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTFUNC3U12_MIX_P2( tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( float,    float,    double,   float,    s, s, d, s, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    float,    dcomplex, float,    s, s, z, s, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( float,    double,   float,    double,   s, d, s, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    double,   double,   double,   s, d, d, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    double,   scomplex, double,   s, d, c, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    double,   dcomplex, double,   s, d, z, d, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( float,    scomplex, double,   scomplex, s, c, d, c, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    scomplex, dcomplex, scomplex, s, c, z, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( float,    dcomplex, float,    dcomplex, s, z, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    dcomplex, double,   dcomplex, s, z, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    dcomplex, scomplex, dcomplex, s, z, c, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( float,    dcomplex, dcomplex, dcomplex, s, z, z, z, tfuncname, varname1, varname2 ) \
\
\
GENTFUNC3U12( double,   float,    float,    double,   d, s, s, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   float,    double,   double,   d, s, d, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   float,    scomplex, double,   d, s, c, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   float,    dcomplex, double,   d, s, z, d, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( double,   double,   float,    double,   d, d, s, d, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   double,   scomplex, double,   d, d, c, d, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( double,   scomplex, float,    dcomplex, d, c, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   scomplex, double,   dcomplex, d, c, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   scomplex, scomplex, dcomplex, d, c, c, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   scomplex, dcomplex, dcomplex, d, c, z, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( double,   dcomplex, float,    dcomplex, d, z, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( double,   dcomplex, scomplex, dcomplex, d, z, c, z, tfuncname, varname1, varname2 ) \
\
\
GENTFUNC3U12( scomplex, float,    double,   scomplex, c, s, d, c, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, float,    dcomplex, scomplex, c, s, z, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( scomplex, double,   float,    dcomplex, c, d, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, double,   double,   dcomplex, c, d, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, double,   scomplex, dcomplex, c, d, c, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, double,   dcomplex, dcomplex, c, d, z, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( scomplex, scomplex, double,   scomplex, c, c, d, c, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, scomplex, dcomplex, scomplex, c, c, z, c, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( scomplex, dcomplex, float,    dcomplex, c, z, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, dcomplex, double,   dcomplex, c, z, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, dcomplex, scomplex, dcomplex, c, z, c, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( scomplex, dcomplex, dcomplex, dcomplex, c, z, z, z, tfuncname, varname1, varname2 ) \
\
\
GENTFUNC3U12( dcomplex, float,    float,    dcomplex, z, s, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, float,    double,   dcomplex, z, s, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, float,    scomplex, dcomplex, z, s, c, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, float,    dcomplex, dcomplex, z, s, z, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( dcomplex, double,   float,    dcomplex, z, d, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, double,   scomplex, dcomplex, z, d, c, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( dcomplex, scomplex, float,    dcomplex, z, c, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, scomplex, double,   dcomplex, z, c, d, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, scomplex, scomplex, dcomplex, z, c, c, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, scomplex, dcomplex, dcomplex, z, c, z, z, tfuncname, varname1, varname2 ) \
\
GENTFUNC3U12( dcomplex, dcomplex, float,    dcomplex, z, z, s, z, tfuncname, varname1, varname2 ) \
GENTFUNC3U12( dcomplex, dcomplex, scomplex, dcomplex, z, z, c, z, tfuncname, varname1, varname2 )


#endif
// end bli_gentfunc_macro_defs.h
// begin bli_gentprot_macro_defs.h



#ifndef BLIS_GENTPROT_MACRO_DEFS_H
#define BLIS_GENTPROT_MACRO_DEFS_H

//
// -- MACROS TO INSERT PROTOTYPE-GENERATING MACROS -----------------------------
//



// -- Macros for generating BLAS routines --------------------------------------


// -- Basic one-operand macro --


#define INSERT_GENTPROT_BLAS( blasname ) \
\
GENTPROT( float,    s, blasname ) \
GENTPROT( double,   d, blasname ) \
GENTPROT( scomplex, c, blasname ) \
GENTPROT( dcomplex, z, blasname )


// -- Basic one-operand macro with real domain only --


#define INSERT_GENTPROTRO_BLAS( blasname ) \
\
GENTPROTRO( float,    s, blasname ) \
GENTPROTRO( double,   d, blasname )


// -- Basic one-operand macro with complex domain only and real projection --


#define INSERT_GENTPROTCO_BLAS( blasname ) \
\
GENTPROTCO( scomplex, float,  c, s, blasname ) \
GENTPROTCO( dcomplex, double, z, d, blasname )


// -- Basic one-operand macro with conjugation (used only for dot, ger) --


#define INSERT_GENTPROTDOT_BLAS( blasname ) \
\
GENTPROTDOT( float,    s,  , blasname ) \
GENTPROTDOT( double,   d,  , blasname ) \
GENTPROTDOT( scomplex, c, c, blasname ) \
GENTPROTDOT( scomplex, c, u, blasname ) \
GENTPROTDOT( dcomplex, z, c, blasname ) \
GENTPROTDOT( dcomplex, z, u, blasname )


// -- Basic one-operand macro with real projection --


#define INSERT_GENTPROTR_BLAS( rblasname, cblasname ) \
\
GENTPROTR( float,    float,  s, s, rblasname ) \
GENTPROTR( double,   double, d, d, rblasname ) \
GENTPROTR( scomplex, float,  c, s, cblasname ) \
GENTPROTR( dcomplex, double, z, d, cblasname )


// -- Alternate two-operand macro (one char for complex, one for real proj) --


#define INSERT_GENTPROTR2_BLAS( blasname ) \
\
GENTPROTR2( float,    float,   , s, blasname ) \
GENTPROTR2( double,   double,  , d, blasname ) \
GENTPROTR2( scomplex, float,  c, s, blasname ) \
GENTPROTR2( dcomplex, double, z, d, blasname )


// -- Extended two-operand macro (used only for scal) --


#define INSERT_GENTPROTSCAL_BLAS( blasname ) \
\
GENTPROTSCAL( float,    float,     , s, blasname ) \
GENTPROTSCAL( double,   double,    , d, blasname ) \
GENTPROTSCAL( scomplex, scomplex,  , c, blasname ) \
GENTPROTSCAL( dcomplex, dcomplex,  , z, blasname ) \
GENTPROTSCAL( float,    scomplex, s, c, blasname ) \
GENTPROTSCAL( double,   dcomplex, d, z, blasname )




// -- Macros for functions with one operand ------------------------------------


// -- Basic one-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT_BASIC0( tfuncname ) \
\
GENTPROT( float,    s, tfuncname ) \
GENTPROT( double,   d, tfuncname ) \
GENTPROT( scomplex, c, tfuncname ) \
GENTPROT( dcomplex, z, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROT_BASIC( tfuncname, varname ) \
\
GENTPROT( float,    s, tfuncname, varname ) \
GENTPROT( double,   d, tfuncname, varname ) \
GENTPROT( scomplex, c, tfuncname, varname ) \
GENTPROT( dcomplex, z, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTPROT_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTPROT( float,    s, tfuncname, varname1, varname2 ) \
GENTPROT( double,   d, tfuncname, varname1, varname2 ) \
GENTPROT( scomplex, c, tfuncname, varname1, varname2 ) \
GENTPROT( dcomplex, z, tfuncname, varname1, varname2 )

// -- (three auxiliary arguments) --

#define INSERT_GENTPROT_BASIC3( tfuncname, varname1, varname2, varname3 ) \
\
GENTPROT( float,    s, tfuncname, varname1, varname2, varname3 ) \
GENTPROT( double,   d, tfuncname, varname1, varname2, varname3 ) \
GENTPROT( scomplex, c, tfuncname, varname1, varname2, varname3 ) \
GENTPROT( dcomplex, z, tfuncname, varname1, varname2, varname3 )

// -- (four auxiliary arguments) --

#define INSERT_GENTPROT_BASIC4( tfuncname, varname1, varname2, varname3, varname4 ) \
\
GENTPROT( float,    s, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTPROT( double,   d, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTPROT( scomplex, c, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTPROT( dcomplex, z, tfuncname, varname1, varname2, varname3, varname4 )



// -- Basic one-operand with real projection --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROTR_BASIC0( tfuncname ) \
\
GENTPROTR( float,    float,  s, s, tfuncname ) \
GENTPROTR( double,   double, d, d, tfuncname ) \
GENTPROTR( scomplex, float,  c, s, tfuncname ) \
GENTPROTR( dcomplex, double, z, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROTR_BASIC( tfuncname, varname ) \
\
GENTPROTR( float,    float,  s, s, tfuncname, varname ) \
GENTPROTR( double,   double, d, d, tfuncname, varname ) \
GENTPROTR( scomplex, float,  c, s, tfuncname, varname ) \
GENTPROTR( dcomplex, double, z, d, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTPROTR_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTPROTR( float,    float,  s, s, tfuncname, varname1, varname2 ) \
GENTPROTR( double,   double, d, d, tfuncname, varname1, varname2 ) \
GENTPROTR( scomplex, float,  c, s, tfuncname, varname1, varname2 ) \
GENTPROTR( dcomplex, double, z, d, tfuncname, varname1, varname2 )

// -- (three auxiliary arguments) --

#define INSERT_GENTPROTR_BASIC3( tfuncname, varname1, varname2, varname3  ) \
\
GENTPROTR( float,    float,  s, s, tfuncname, varname1, varname2, varname3 ) \
GENTPROTR( double,   double, d, d, tfuncname, varname1, varname2, varname3 ) \
GENTPROTR( scomplex, float,  c, s, tfuncname, varname1, varname2, varname3 ) \
GENTPROTR( dcomplex, double, z, d, tfuncname, varname1, varname2, varname3 )

// -- (four auxiliary arguments) --

#define INSERT_GENTPROTR_BASIC4( tfuncname, varname1, varname2, varname3, varname4  ) \
\
GENTPROTR( float,    float,  s, s, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTPROTR( double,   double, d, d, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTPROTR( scomplex, float,  c, s, tfuncname, varname1, varname2, varname3, varname4 ) \
GENTPROTR( dcomplex, double, z, d, tfuncname, varname1, varname2, varname3, varname4 )



// -- Basic one-operand macro with complex domain only and real projection --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROTCO_BASIC0( tfuncname ) \
\
GENTPROTCO( scomplex, float,  c, s, tfuncname ) \
GENTPROTCO( dcomplex, double, z, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROTCO_BASIC( tfuncname, varname ) \
\
GENTPROTCO( scomplex, float,  c, s, tfuncname, varname ) \
GENTPROTCO( dcomplex, double, z, d, tfuncname, varname )

// -- (two auxiliary arguments) --

#define INSERT_GENTPROTCO_BASIC2( tfuncname, varname1, varname2 ) \
\
GENTPROTCO( scomplex, float,  c, s, tfuncname, varname1, varname2 ) \
GENTPROTCO( dcomplex, double, z, d, tfuncname, varname1, varname2 )



// -- Basic one-operand macro with integer instance --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT_BASIC0_I( funcname ) \
\
GENTPROT( float,    s, funcname ) \
GENTPROT( double,   d, funcname ) \
GENTPROT( scomplex, c, funcname ) \
GENTPROT( dcomplex, z, funcname ) \
GENTPROT( gint_t,   i, funcname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROT_BASIC_I( tfuncname, varname ) \
\
GENTPROT( float,    s, tfuncname, varname ) \
GENTPROT( double,   d, tfuncname, varname ) \
GENTPROT( scomplex, c, tfuncname, varname ) \
GENTPROT( dcomplex, z, tfuncname, varname ) \
GENTPROT( gint_t,   i, tfuncname, varname )



// -- Basic one-operand with integer projection --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROTI_BASIC0( funcname ) \
\
GENTPROTI( float,    gint_t, s, i, funcname ) \
GENTPROTI( double,   gint_t, d, i, funcname ) \
GENTPROTI( scomplex, gint_t, c, i, funcname ) \
GENTPROTI( dcomplex, gint_t, z, i, funcname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROTI_BASIC( tfuncname, varname ) \
\
GENTPROTI( float,    gint_t, s, i, tfuncname, varname ) \
GENTPROTI( double,   gint_t, d, i, tfuncname, varname ) \
GENTPROTI( scomplex, gint_t, c, i, tfuncname, varname ) \
GENTPROTI( dcomplex, gint_t, z, i, tfuncname, varname )



// -- Basic one-operand with real and integer projections --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROTRI_BASIC( funcname ) \
\
GENTPROTRI( float,    float,  gint_t, s, s, i, funcname ) \
GENTPROTRI( double,   double, gint_t, d, d, i, funcname ) \
GENTPROTRI( scomplex, float,  gint_t, c, s, i, funcname ) \
GENTPROTRI( dcomplex, double, gint_t, z, d, i, funcname )




// -- Macros for functions with two primary operands ---------------------------


// -- Basic two-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT2_BASIC0( funcname ) \
\
GENTPROT2( float,    float,    s, s, funcname ) \
GENTPROT2( double,   double,   d, d, funcname ) \
GENTPROT2( scomplex, scomplex, c, c, funcname ) \
GENTPROT2( dcomplex, dcomplex, z, z, funcname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROT2_BASIC( tfuncname, varname ) \
\
GENTPROT2( float,    float,    s, s, tfuncname, varname ) \
GENTPROT2( double,   double,   d, d, tfuncname, varname ) \
GENTPROT2( scomplex, scomplex, c, c, tfuncname, varname ) \
GENTPROT2( dcomplex, dcomplex, z, z, tfuncname, varname )



// -- Mixed domain two-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT2_MIX_D0( funcname ) \
\
GENTPROT2( float,    scomplex, s, c, funcname ) \
GENTPROT2( scomplex, float,    c, s, funcname ) \
\
GENTPROT2( double,   dcomplex, d, z, funcname ) \
GENTPROT2( dcomplex, double,   z, d, funcname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROT2_MIX_D( tfuncname, varname ) \
\
GENTPROT2( float,    scomplex, s, c, tfuncname, varname ) \
GENTPROT2( scomplex, float,    c, s, tfuncname, varname ) \
\
GENTPROT2( double,   dcomplex, d, z, tfuncname, varname ) \
GENTPROT2( dcomplex, double,   z, d, tfuncname, varname )



// -- Mixed precision two-operand macro --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT2_MIX_P0( funcname ) \
\
GENTPROT2( float,    double,   s, d, funcname ) \
GENTPROT2( float,    dcomplex, s, z, funcname ) \
\
GENTPROT2( double,   float,    d, s, funcname ) \
GENTPROT2( double,   scomplex, d, c, funcname ) \
\
GENTPROT2( scomplex, double,   c, d, funcname ) \
GENTPROT2( scomplex, dcomplex, c, z, funcname ) \
\
GENTPROT2( dcomplex, float,    z, s, funcname ) \
GENTPROT2( dcomplex, scomplex, z, c, funcname ) \

// -- (one auxiliary argument) --

#define INSERT_GENTPROT2_MIX_P( tfuncname, varname ) \
\
GENTPROT2( float,    double,   s, d, tfuncname, varname ) \
GENTPROT2( float,    dcomplex, s, z, tfuncname, varname ) \
\
GENTPROT2( double,   float,    d, s, tfuncname, varname ) \
GENTPROT2( double,   scomplex, d, c, tfuncname, varname ) \
\
GENTPROT2( scomplex, double,   c, d, tfuncname, varname ) \
GENTPROT2( scomplex, dcomplex, c, z, tfuncname, varname ) \
\
GENTPROT2( dcomplex, float,    z, s, tfuncname, varname ) \
GENTPROT2( dcomplex, scomplex, z, c, tfuncname, varname ) \



// -- Basic two-operand with real projection of first operand --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT2R_BASIC0( funcname ) \
\
GENTPROT2R( float,    float,    float,    s, s, s, funcname ) \
GENTPROT2R( double,   double,   double,   d, d, d, funcname ) \
GENTPROT2R( scomplex, scomplex, float,    c, c, s, funcname ) \
GENTPROT2R( dcomplex, dcomplex, double,   z, z, d, funcname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROT2R_BASIC( tfuncname, varname ) \
\
GENTPROT2R( float,    float,    float,    s, s, s, tfuncname, varname ) \
GENTPROT2R( double,   double,   double,   d, d, d, tfuncname, varname ) \
GENTPROT2R( scomplex, scomplex, float,    c, c, s, tfuncname, varname ) \
GENTPROT2R( dcomplex, dcomplex, double,   z, z, d, tfuncname, varname )



// -- Mixed domain two-operand with real projection of first operand --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT2R_MIX_D0( tfuncname ) \
\
GENTPROT2R( float,    scomplex, float,    s, c, s, tfuncname ) \
GENTPROT2R( scomplex, float,    float,    c, s, s, tfuncname ) \
\
GENTPROT2R( double,   dcomplex, double,   d, z, d, tfuncname ) \
GENTPROT2R( dcomplex, double,   double,   z, d, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROT2R_MIX_D( tfuncname, varname ) \
\
GENTPROT2R( float,    scomplex, float,    s, c, s, tfuncname, varname ) \
GENTPROT2R( scomplex, float,    float,    c, s, s, tfuncname, varname ) \
\
GENTPROT2R( double,   dcomplex, double,   d, z, d, tfuncname, varname ) \
GENTPROT2R( dcomplex, double,   double,   z, d, d, tfuncname, varname )



// -- Mixed precision two-operand with real projection of first operand --

// -- (no auxiliary arguments) --

#define INSERT_GENTPROT2R_MIX_P0( tfuncname ) \
\
GENTPROT2R( float,    double,   float,    s, d, s, tfuncname ) \
GENTPROT2R( float,    dcomplex, float,    s, z, s, tfuncname ) \
\
GENTPROT2R( double,   float,    double,   d, s, d, tfuncname ) \
GENTPROT2R( double,   scomplex, double,   d, c, d, tfuncname ) \
\
GENTPROT2R( scomplex, double,   float,    c, d, s, tfuncname ) \
GENTPROT2R( scomplex, dcomplex, float,    c, z, s, tfuncname ) \
\
GENTPROT2R( dcomplex, float,    double,   z, s, d, tfuncname ) \
GENTPROT2R( dcomplex, scomplex, double,   z, c, d, tfuncname )

// -- (one auxiliary argument) --

#define INSERT_GENTPROT2R_MIX_P( tfuncname, varname ) \
\
GENTPROT2R( float,    double,   float,    s, d, s, tfuncname, varname ) \
GENTPROT2R( float,    dcomplex, float,    s, z, s, tfuncname, varname ) \
\
GENTPROT2R( double,   float,    double,   d, s, d, tfuncname, varname ) \
GENTPROT2R( double,   scomplex, double,   d, c, d, tfuncname, varname ) \
\
GENTPROT2R( scomplex, double,   float,    c, d, s, tfuncname, varname ) \
GENTPROT2R( scomplex, dcomplex, float,    c, z, s, tfuncname, varname ) \
\
GENTPROT2R( dcomplex, float,    double,   z, s, d, tfuncname, varname ) \
GENTPROT2R( dcomplex, scomplex, double,   z, c, d, tfuncname, varname )



// -- Macros for functions with three primary operands -------------------------


// -- Basic three-operand macro --


#define INSERT_GENTPROT3_BASIC( funcname ) \
\
GENTPROT3( float,    float,    float,    s, s, s, funcname ) \
GENTPROT3( double,   double,   double,   d, d, d, funcname ) \
GENTPROT3( scomplex, scomplex, scomplex, c, c, c, funcname ) \
GENTPROT3( dcomplex, dcomplex, dcomplex, z, z, z, funcname )


// -- Mixed domain three-operand macro --


#define INSERT_GENTPROT3_MIX_D( funcname ) \
\
GENTPROT3( float,    float,    scomplex, s, s, c, funcname ) \
GENTPROT3( float,    scomplex, float,    s, c, s, funcname ) \
GENTPROT3( float,    scomplex, scomplex, s, c, c, funcname ) \
\
GENTPROT3( double,   double,   dcomplex, d, d, z, funcname ) \
GENTPROT3( double,   dcomplex, double,   d, z, d, funcname ) \
GENTPROT3( double,   dcomplex, dcomplex, d, z, z, funcname ) \
\
GENTPROT3( scomplex, float,    float,    c, s, s, funcname ) \
GENTPROT3( scomplex, float,    scomplex, c, s, c, funcname ) \
GENTPROT3( scomplex, scomplex, float,    c, c, s, funcname ) \
\
GENTPROT3( dcomplex, double,   double,   z, d, d, funcname ) \
GENTPROT3( dcomplex, double,   dcomplex, z, d, z, funcname ) \
GENTPROT3( dcomplex, dcomplex, double,   z, z, d, funcname )


// -- Mixed precision three-operand macro --


#define INSERT_GENTPROT3_MIX_P( funcname ) \
\
GENTPROT3( float,    float,    double,   s, s, d, funcname ) \
GENTPROT3( float,    float,    dcomplex, s, s, z, funcname ) \
\
GENTPROT3( float,    double,   float,    s, d, s, funcname ) \
GENTPROT3( float,    double,   double,   s, d, d, funcname ) \
GENTPROT3( float,    double,   scomplex, s, d, c, funcname ) \
GENTPROT3( float,    double,   dcomplex, s, d, z, funcname ) \
\
GENTPROT3( float,    scomplex, double,   s, c, d, funcname ) \
GENTPROT3( float,    scomplex, dcomplex, s, c, z, funcname ) \
\
GENTPROT3( float,    dcomplex, float,    s, z, s, funcname ) \
GENTPROT3( float,    dcomplex, double,   s, z, d, funcname ) \
GENTPROT3( float,    dcomplex, scomplex, s, z, c, funcname ) \
GENTPROT3( float,    dcomplex, dcomplex, s, z, z, funcname ) \
\
\
GENTPROT3( double,   float,    float,    d, s, s, funcname ) \
GENTPROT3( double,   float,    double,   d, s, d, funcname ) \
GENTPROT3( double,   float,    scomplex, d, s, c, funcname ) \
GENTPROT3( double,   float,    dcomplex, d, s, z, funcname ) \
\
GENTPROT3( double,   double,   float,    d, d, s, funcname ) \
GENTPROT3( double,   double,   scomplex, d, d, c, funcname ) \
\
GENTPROT3( double,   scomplex, float,    d, c, s, funcname ) \
GENTPROT3( double,   scomplex, double,   d, c, d, funcname ) \
GENTPROT3( double,   scomplex, scomplex, d, c, c, funcname ) \
GENTPROT3( double,   scomplex, dcomplex, d, c, z, funcname ) \
\
GENTPROT3( double,   dcomplex, float,    d, z, s, funcname ) \
GENTPROT3( double,   dcomplex, scomplex, d, z, c, funcname ) \
\
\
GENTPROT3( scomplex, float,    double,   c, s, d, funcname ) \
GENTPROT3( scomplex, float,    dcomplex, c, s, z, funcname ) \
\
GENTPROT3( scomplex, double,   float,    c, d, s, funcname ) \
GENTPROT3( scomplex, double,   double,   c, d, d, funcname ) \
GENTPROT3( scomplex, double,   scomplex, c, d, c, funcname ) \
GENTPROT3( scomplex, double,   dcomplex, c, d, z, funcname ) \
\
GENTPROT3( scomplex, scomplex, double,   c, c, d, funcname ) \
GENTPROT3( scomplex, scomplex, dcomplex, c, c, z, funcname ) \
\
GENTPROT3( scomplex, dcomplex, float,    c, z, s, funcname ) \
GENTPROT3( scomplex, dcomplex, double,   c, z, d, funcname ) \
GENTPROT3( scomplex, dcomplex, scomplex, c, z, c, funcname ) \
GENTPROT3( scomplex, dcomplex, dcomplex, c, z, z, funcname ) \
\
\
GENTPROT3( dcomplex, float,    float,    z, s, s, funcname ) \
GENTPROT3( dcomplex, float,    double,   z, s, d, funcname ) \
GENTPROT3( dcomplex, float,    scomplex, z, s, c, funcname ) \
GENTPROT3( dcomplex, float,    dcomplex, z, s, z, funcname ) \
\
GENTPROT3( dcomplex, double,   float,    z, d, s, funcname ) \
GENTPROT3( dcomplex, double,   scomplex, z, d, c, funcname ) \
\
GENTPROT3( dcomplex, scomplex, float,    z, c, s, funcname ) \
GENTPROT3( dcomplex, scomplex, double,   z, c, d, funcname ) \
GENTPROT3( dcomplex, scomplex, scomplex, z, c, c, funcname ) \
GENTPROT3( dcomplex, scomplex, dcomplex, z, c, z, funcname ) \
\
GENTPROT3( dcomplex, dcomplex, float,    z, z, s, funcname ) \
GENTPROT3( dcomplex, dcomplex, scomplex, z, z, c, funcname ) \



// -- Basic three-operand with union of operands 1 and 2 --


#define INSERT_GENTPROT3U12_BASIC( funcname ) \
\
GENTPROT3U12( float,    float,    float,    float,    s, s, s, s, funcname ) \
GENTPROT3U12( double,   double,   double,   double,   d, d, d, d, funcname ) \
GENTPROT3U12( scomplex, scomplex, scomplex, scomplex, c, c, c, c, funcname ) \
GENTPROT3U12( dcomplex, dcomplex, dcomplex, dcomplex, z, z, z, z, funcname )


// -- Mixed domain three-operand with union of operands 1 and 2 --


#define INSERT_GENTPROT3U12_MIX_D( funcname ) \
\
GENTPROT3U12( float,    float,    scomplex, float,    s, s, c, s, funcname ) \
GENTPROT3U12( float,    scomplex, float,    scomplex, s, c, s, c, funcname ) \
GENTPROT3U12( float,    scomplex, scomplex, scomplex, s, c, c, c, funcname ) \
\
GENTPROT3U12( double,   double,   dcomplex, double,   d, d, z, d, funcname ) \
GENTPROT3U12( double,   dcomplex, double,   dcomplex, d, z, d, z, funcname ) \
GENTPROT3U12( double,   dcomplex, dcomplex, dcomplex, d, z, z, z, funcname ) \
\
GENTPROT3U12( scomplex, float,    float,    scomplex, c, s, s, c, funcname ) \
GENTPROT3U12( scomplex, float,    scomplex, scomplex, c, s, c, c, funcname ) \
GENTPROT3U12( scomplex, scomplex, float,    scomplex, c, c, s, c, funcname ) \
\
GENTPROT3U12( dcomplex, double,   double,   dcomplex, z, d, d, z, funcname ) \
GENTPROT3U12( dcomplex, double,   dcomplex, dcomplex, z, d, z, z, funcname ) \
GENTPROT3U12( dcomplex, dcomplex, double,   dcomplex, z, z, d, z, funcname )


// -- Mixed precision three-operand with union of operands 1 and 2 --


#define INSERT_GENTPROT3U12_MIX_P( funcname ) \
\
GENTPROT3U12( float,    float,    double,   float,    s, s, d, s, funcname ) \
GENTPROT3U12( float,    float,    dcomplex, float,    s, s, z, s, funcname ) \
\
GENTPROT3U12( float,    double,   float,    double,   s, d, s, d, funcname ) \
GENTPROT3U12( float,    double,   double,   double,   s, d, d, d, funcname ) \
GENTPROT3U12( float,    double,   scomplex, double,   s, d, c, d, funcname ) \
GENTPROT3U12( float,    double,   dcomplex, double,   s, d, z, d, funcname ) \
\
GENTPROT3U12( float,    scomplex, double,   scomplex, s, c, d, c, funcname ) \
GENTPROT3U12( float,    scomplex, dcomplex, scomplex, s, c, z, c, funcname ) \
\
GENTPROT3U12( float,    dcomplex, float,    dcomplex, s, z, s, z, funcname ) \
GENTPROT3U12( float,    dcomplex, double,   dcomplex, s, z, d, z, funcname ) \
GENTPROT3U12( float,    dcomplex, scomplex, dcomplex, s, z, c, z, funcname ) \
GENTPROT3U12( float,    dcomplex, dcomplex, dcomplex, s, z, z, z, funcname ) \
\
\
GENTPROT3U12( double,   float,    float,    double,   d, s, s, d, funcname ) \
GENTPROT3U12( double,   float,    double,   double,   d, s, d, d, funcname ) \
GENTPROT3U12( double,   float,    scomplex, double,   d, s, c, d, funcname ) \
GENTPROT3U12( double,   float,    dcomplex, double,   d, s, z, d, funcname ) \
\
GENTPROT3U12( double,   double,   float,    double,   d, d, s, d, funcname ) \
GENTPROT3U12( double,   double,   scomplex, double,   d, d, c, d, funcname ) \
\
GENTPROT3U12( double,   scomplex, float,    dcomplex, d, c, s, z, funcname ) \
GENTPROT3U12( double,   scomplex, double,   dcomplex, d, c, d, z, funcname ) \
GENTPROT3U12( double,   scomplex, scomplex, dcomplex, d, c, c, z, funcname ) \
GENTPROT3U12( double,   scomplex, dcomplex, dcomplex, d, c, z, z, funcname ) \
\
GENTPROT3U12( double,   dcomplex, float,    dcomplex, d, z, s, z, funcname ) \
GENTPROT3U12( double,   dcomplex, scomplex, dcomplex, d, z, c, z, funcname ) \
\
\
GENTPROT3U12( scomplex, float,    double,   scomplex, c, s, d, c, funcname ) \
GENTPROT3U12( scomplex, float,    dcomplex, scomplex, c, s, z, c, funcname ) \
\
GENTPROT3U12( scomplex, double,   float,    dcomplex, c, d, s, z, funcname ) \
GENTPROT3U12( scomplex, double,   double,   dcomplex, c, d, d, z, funcname ) \
GENTPROT3U12( scomplex, double,   scomplex, dcomplex, c, d, c, z, funcname ) \
GENTPROT3U12( scomplex, double,   dcomplex, dcomplex, c, d, z, z, funcname ) \
\
GENTPROT3U12( scomplex, scomplex, double,   scomplex, c, c, d, c, funcname ) \
GENTPROT3U12( scomplex, scomplex, dcomplex, scomplex, c, c, z, c, funcname ) \
\
GENTPROT3U12( scomplex, dcomplex, float,    dcomplex, c, z, s, z, funcname ) \
GENTPROT3U12( scomplex, dcomplex, double,   dcomplex, c, z, d, z, funcname ) \
GENTPROT3U12( scomplex, dcomplex, scomplex, dcomplex, c, z, c, z, funcname ) \
GENTPROT3U12( scomplex, dcomplex, dcomplex, dcomplex, c, z, z, z, funcname ) \
\
\
GENTPROT3U12( dcomplex, float,    float,    dcomplex, z, s, s, z, funcname ) \
GENTPROT3U12( dcomplex, float,    double,   dcomplex, z, s, d, z, funcname ) \
GENTPROT3U12( dcomplex, float,    scomplex, dcomplex, z, s, c, z, funcname ) \
GENTPROT3U12( dcomplex, float,    dcomplex, dcomplex, z, s, z, z, funcname ) \
\
GENTPROT3U12( dcomplex, double,   float,    dcomplex, z, d, s, z, funcname ) \
GENTPROT3U12( dcomplex, double,   scomplex, dcomplex, z, d, c, z, funcname ) \
\
GENTPROT3U12( dcomplex, scomplex, float,    dcomplex, z, c, s, z, funcname ) \
GENTPROT3U12( dcomplex, scomplex, double,   dcomplex, z, c, d, z, funcname ) \
GENTPROT3U12( dcomplex, scomplex, scomplex, dcomplex, z, c, c, z, funcname ) \
GENTPROT3U12( dcomplex, scomplex, dcomplex, dcomplex, z, c, z, z, funcname ) \
\
GENTPROT3U12( dcomplex, dcomplex, float,    dcomplex, z, z, s, z, funcname ) \
GENTPROT3U12( dcomplex, dcomplex, scomplex, dcomplex, z, z, c, z, funcname )


#endif
// end bli_gentprot_macro_defs.h

// begin bli_obj_macro_defs.h


#ifndef BLIS_OBJ_MACRO_DEFS_H
#define BLIS_OBJ_MACRO_DEFS_H


// -- Object query/modification macros --

// Info query

#define bli_obj_is_float( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) == BLIS_BITVAL_FLOAT_TYPE )

#define bli_obj_is_double( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) == BLIS_BITVAL_DOUBLE_TYPE )

#define bli_obj_is_scomplex( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) == BLIS_BITVAL_SCOMPLEX_TYPE )

#define bli_obj_is_dcomplex( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) == BLIS_BITVAL_DCOMPLEX_TYPE )

#define bli_obj_is_int( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) == BLIS_BITVAL_INT_TYPE )

#define bli_obj_is_const( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) == BLIS_BITVAL_CONST_TYPE )

#define bli_obj_domain( obj ) \
\
	(   (obj).info & BLIS_DOMAIN_BIT )

#define bli_obj_is_real( obj ) \
\
	( ( (obj).info & BLIS_DOMAIN_BIT ) == BLIS_BITVAL_REAL )

#define bli_obj_is_complex( obj ) \
\
	( ( (obj).info & BLIS_DOMAIN_BIT ) == BLIS_BITVAL_COMPLEX )

#define bli_obj_precision( obj ) \
\
	(   (obj).info & BLIS_PRECISION_BIT )

#define bli_obj_is_double_precision( obj ) \
\
	( ( (obj).info & BLIS_PRECISION_BIT ) == BLIS_BITVAL_DOUBLE_PREC )

#define bli_obj_datatype( obj ) \
\
	(   (obj).info & BLIS_DATATYPE_BITS )

#define bli_obj_datatype_proj_to_real( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) & ~BLIS_BITVAL_COMPLEX )

#define bli_obj_datatype_proj_to_complex( obj ) \
\
	( ( (obj).info & BLIS_DATATYPE_BITS ) &  BLIS_BITVAL_COMPLEX )

#define bli_obj_target_datatype( obj ) \
\
	( ( (obj).info & BLIS_TARGET_DT_BITS ) >> BLIS_TARGET_DT_SHIFT )

#define bli_obj_execution_datatype( obj ) \
\
	( ( (obj).info & BLIS_EXECUTION_DT_BITS ) >> BLIS_EXECUTION_DT_SHIFT )

#define bli_obj_conjtrans_status( obj ) \
\
	(   (obj).info & BLIS_CONJTRANS_BITS )

#define bli_obj_onlytrans_status( obj ) \
\
	(   (obj).info & BLIS_TRANS_BIT )

#define bli_obj_has_trans( obj ) \
\
	( ( (obj).info  & BLIS_TRANS_BIT ) == BLIS_BITVAL_TRANS ) \

#define bli_obj_has_notrans( obj ) \
\
	( ( (obj).info  & BLIS_TRANS_BIT ) == BLIS_BITVAL_NO_TRANS ) \

#define bli_obj_conj_status( obj ) \
\
	(   (obj).info & BLIS_CONJ_BIT )

#define bli_obj_has_conj( obj ) \
\
	( ( (obj).info  & BLIS_CONJ_BIT ) == BLIS_BITVAL_CONJ ) \

#define bli_obj_has_noconj( obj ) \
\
	( ( (obj).info  & BLIS_CONJ_BIT ) == BLIS_BITVAL_NO_CONJ ) \

#define bli_obj_uplo( obj ) \
\
	(   (obj).info & BLIS_UPLO_BITS ) \

#define bli_obj_is_upper( obj ) \
\
	( ( (obj).info & BLIS_UPLO_BITS ) == BLIS_BITVAL_UPPER )

#define bli_obj_is_lower( obj ) \
\
	( ( (obj).info & BLIS_UPLO_BITS ) == BLIS_BITVAL_LOWER )

#define bli_obj_is_upper_after_trans( obj ) \
\
	( bli_obj_has_trans( (obj) ) ? bli_obj_is_lower( (obj) ) \
	                             : bli_obj_is_upper( (obj) ) )

#define bli_obj_is_lower_after_trans( obj ) \
\
	( bli_obj_has_trans( (obj) ) ? bli_obj_is_upper( (obj) ) \
	                             : bli_obj_is_lower( (obj) ) )

#define bli_obj_is_upper_or_lower( obj ) \
\
	( bli_obj_is_upper( obj ) || \
	  bli_obj_is_lower( obj ) )

#define bli_obj_is_dense( obj ) \
\
	( ( (obj).info & BLIS_UPLO_BITS ) == BLIS_BITVAL_DENSE )

#define bli_obj_is_zeros( obj ) \
\
	( ( (obj).info & BLIS_UPLO_BITS ) == BLIS_BITVAL_ZEROS )

#define bli_obj_diag( obj ) \
\
	(   (obj).info & BLIS_UNIT_DIAG_BIT )

#define bli_obj_has_nonunit_diag( obj ) \
\
	( ( (obj).info & BLIS_UNIT_DIAG_BIT ) == BLIS_BITVAL_NONUNIT_DIAG )

#define bli_obj_has_unit_diag( obj ) \
\
	( ( (obj).info & BLIS_UNIT_DIAG_BIT ) == BLIS_BITVAL_UNIT_DIAG )

#define bli_obj_has_inverted_diag( obj ) \
\
	( ( (obj).info & BLIS_INVERT_DIAG_BIT ) == BLIS_BITVAL_INVERT_DIAG )

#define bli_obj_is_pack_rev_if_upper( obj ) \
\
	( ( (obj).info & BLIS_PACK_REV_IF_UPPER_BIT ) == BLIS_BITVAL_PACK_REV_IF_UPPER )

#define bli_obj_is_pack_rev_if_lower( obj ) \
\
	( ( (obj).info & BLIS_PACK_REV_IF_LOWER_BIT ) == BLIS_BITVAL_PACK_REV_IF_LOWER )

#define bli_obj_pack_schema( obj ) \
\
	(   (obj).info & BLIS_PACK_SCHEMA_BITS )

#define bli_obj_is_packed( obj ) \
\
	( ( (obj).info & BLIS_PACK_BIT  ) )

#define bli_obj_is_row_packed( obj ) \
\
	( ( (obj).info & BLIS_PACK_RC_BIT  ) == ( BLIS_BITVAL_PACKED_UNSPEC ^ \
	                                          BLIS_BITVAL_PACKED_ROWS    ) )

#define bli_obj_is_col_packed( obj ) \
\
	( ( (obj).info & BLIS_PACK_RC_BIT  ) == ( BLIS_BITVAL_PACKED_UNSPEC ^ \
	                                          BLIS_BITVAL_PACKED_COLUMNS ) )

#define bli_obj_is_panel_packed( obj ) \
\
	( ( (obj).info & BLIS_PACK_PANEL_BIT ) )

#define bli_obj_pack_buffer_type( obj ) \
\
	(   (obj).info & BLIS_PACK_BUFFER_BITS )

#define bli_obj_struc( obj ) \
\
	(   (obj).info & BLIS_STRUC_BITS )

#define bli_obj_is_general( obj ) \
\
	( ( (obj).info & BLIS_STRUC_BITS ) == BLIS_BITVAL_GENERAL )

#define bli_obj_is_hermitian( obj ) \
\
	( ( (obj).info & BLIS_STRUC_BITS ) == BLIS_BITVAL_HERMITIAN )

#define bli_obj_is_symmetric( obj ) \
\
	( ( (obj).info & BLIS_STRUC_BITS ) == BLIS_BITVAL_SYMMETRIC )

#define bli_obj_is_triangular( obj ) \
\
	( ( (obj).info & BLIS_STRUC_BITS ) == BLIS_BITVAL_TRIANGULAR )

#define bli_obj_is_herm_or_symm( obj ) \
\
	( bli_obj_is_hermitian( obj ) || \
	  bli_obj_is_symmetric( obj ) )



// Info modification

#define bli_obj_set_conjtrans( conjtrans, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_CONJTRANS_BITS ) | (conjtrans); \
}

#define bli_obj_set_onlytrans( trans, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_TRANS_BIT ) | (trans); \
}

#define bli_obj_set_conj( conjval, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_CONJ_BIT ) | (conjval); \
}

#define bli_obj_set_uplo( uplo, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_UPLO_BITS ) | (uplo); \
}

#define bli_obj_set_diag( diag, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_UNIT_DIAG_BIT ) | (diag); \
}

#define bli_obj_set_invert_diag( inv_diag, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_INVERT_DIAG_BIT ) | (inv_diag); \
}

#define bli_obj_set_datatype( dt, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_DATATYPE_BITS ) | (dt); \
}

#define bli_obj_set_target_datatype( dt, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_TARGET_DT_BITS ) | ( dt << BLIS_TARGET_DT_SHIFT ); \
}

#define bli_obj_set_execution_datatype( dt, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_EXECUTION_DT_BITS ) | ( dt << BLIS_EXECUTION_DT_SHIFT ); \
}

#define bli_obj_set_pack_schema( pack, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_PACK_SCHEMA_BITS ) | (pack); \
}

#define bli_obj_set_pack_order_if_upper( packordifup, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_PACK_REV_IF_UPPER_BIT ) | (packordifup); \
}

#define bli_obj_set_pack_order_if_lower( packordiflo, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_PACK_REV_IF_LOWER_BIT ) | (packordiflo); \
}

#define bli_obj_set_pack_buffer_type( packbuf, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_PACK_BUFFER_BITS ) | (packbuf); \
}

#define bli_obj_set_struc( struc, obj ) \
{ \
	(obj).info = ( (obj).info & ~BLIS_STRUC_BITS ) | (struc); \
}

#define bli_obj_toggle_trans( obj )\
{ \
	(obj).info = ( (obj).info ^ BLIS_TRANS_BIT ); \
}

#define bli_obj_toggle_conj( obj )\
{ \
	(obj).info = ( (obj).info ^ BLIS_CONJ_BIT ); \
}

#define bli_obj_toggle_uplo( obj ) \
{ \
	(obj).info = ( (obj).info ^ BLIS_LOWER_BIT ) ^ BLIS_UPPER_BIT; \
}

#define bli_obj_toggle_region_ref( obj ) \
{ \
	if      ( bli_obj_is_upper( obj ) ) bli_obj_inc_diag_off( -1, obj ); \
	else if ( bli_obj_is_lower( obj ) ) bli_obj_inc_diag_off(  1, obj ); \
\
	bli_obj_toggle_uplo( obj ); \
}

#define bli_obj_toggle_uplo_if_trans( trans, obj ) \
{ \
	if ( bli_does_trans( trans ) && ( bli_obj_is_upper_or_lower( obj ) ) ) \
	{ \
		bli_obj_toggle_uplo( obj ); \
		bli_obj_negate_diag_offset( obj ); \
	} \
}

#define bli_obj_apply_trans( trans, obj ) \
{ \
	(obj).info = ( (obj).info ^ (trans) ); \
}

#define bli_obj_apply_conj( conjval, obj ) \
{ \
	(obj).info = ( (obj).info ^ (conjval) ); \
}


// Root matrix query

#define bli_obj_root( obj ) \
\
	((obj).root)

#define bli_obj_root_uplo( obj ) \
\
	bli_obj_uplo( *bli_obj_root( obj ) )

#define bli_obj_root_is_general( obj ) \
\
	bli_obj_is_general( *bli_obj_root( obj ) )

#define bli_obj_root_is_hermitian( obj ) \
\
	bli_obj_is_hermitian( *bli_obj_root( obj ) )

#define bli_obj_root_is_symmetric( obj ) \
\
	bli_obj_is_symmetric( *bli_obj_root( obj ) )

#define bli_obj_root_is_triangular( obj ) \
\
	bli_obj_is_triangular( *bli_obj_root( obj ) )

#define bli_obj_root_is_herm_or_symm( obj ) \
\
	( bli_obj_is_hermitian( *bli_obj_root( obj ) ) || \
	  bli_obj_is_symmetric( *bli_obj_root( obj ) ) )

#define bli_obj_root_is_upper( obj ) \
\
	bli_obj_is_upper( *bli_obj_root( obj ) )

#define bli_obj_root_is_lower( obj ) \
\
	bli_obj_is_lower( *bli_obj_root( obj ) )


// Root matrix modification

#define bli_obj_set_as_root( obj )\
{ \
	(obj).root = &(obj); \
}


// Dimension query

#define bli_obj_length( obj ) \
\
	( (obj).dim[BLIS_M] )

#define bli_obj_width( obj ) \
\
	( (obj).dim[BLIS_N] )

#define bli_obj_dim( mdim, obj ) \
\
	( (obj).dim[mdim] )

#define bli_obj_min_dim( obj ) \
\
	( bli_min( bli_obj_length( obj ), \
	           bli_obj_width( obj ) ) )

#define bli_obj_max_dim( obj ) \
\
	( bli_max( bli_obj_length( obj ), \
	           bli_obj_width( obj ) ) )

#define bli_obj_length_after_trans( obj ) \
\
	( bli_obj_has_trans( (obj) ) ? bli_obj_width(  (obj) ) \
	                             : bli_obj_length( (obj) ) )

#define bli_obj_width_after_trans( obj ) \
\
	( bli_obj_has_trans( (obj) ) ? bli_obj_length( (obj) ) \
	                             : bli_obj_width(  (obj) ) )

#define bli_obj_get_dims_after_trans( obj, dim_m, dim_n ) \
{ \
	if ( bli_obj_has_notrans( trans ) ) \
	{ \
		dim_m = bli_obj_length( obj ); \
		dim_n = bli_obj_width( obj ); \
	} \
	else \
	{ \
		dim_m = bli_obj_width( obj ); \
		dim_n = bli_obj_length( obj ); \
	} \
}


// Note: The purpose of these macros is to obtain the length and width
// of the smallest submatrices of an object that could still encompass
// the stored data above (if obj is upper) or below (if obj is lower)
// the diagonal.
#define bli_obj_length_stored( obj ) \
\
	( bli_obj_is_upper( obj ) \
		? bli_min( bli_obj_length( obj ), \
		           bli_obj_width( obj )  - bli_obj_diag_offset( obj ) ) \
		: bli_min( bli_obj_length( obj ), \
		           bli_obj_length( obj ) + bli_obj_diag_offset( obj ) ) \
	)

#define bli_obj_width_stored( obj ) \
\
	( bli_obj_is_lower( obj ) \
		? bli_min( bli_obj_width( obj ), \
		           bli_obj_length( obj ) + bli_obj_diag_offset( obj ) ) \
		: bli_min( bli_obj_width( obj ), \
		           bli_obj_width( obj )  - bli_obj_diag_offset( obj ) ) \
	)

#define bli_obj_length_stored_after_trans( obj ) \
\
	( bli_obj_has_trans( obj ) ? bli_obj_width_stored( obj ) \
	                           : bli_obj_length_stored( obj ) )

#define bli_obj_width_stored_after_trans( obj ) \
\
	( bli_obj_has_trans( obj ) ? bli_obj_length_stored( obj ) \
	                           : bli_obj_width_stored( obj ) )

#define bli_obj_vector_dim( x ) \
\
	( bli_obj_length( x ) == 1 ? bli_obj_width( x ) \
	                           : bli_obj_length( x ) )

#define bli_obj_vector_inc( x ) \
\
	( bli_obj_is_1x1( x ) ? 1 : \
	( bli_obj_length( x ) == 1 ? bli_obj_col_stride( x ) \
	                           : bli_obj_row_stride( x ) ) \
	)

#define bli_obj_is_vector( x ) \
\
	( bli_obj_length( x ) == 1 || \
	  bli_obj_width( x )  == 1 )

#define bli_obj_is_row_vector( x ) \
\
	( bli_obj_length( x ) == 1 )

#define bli_obj_is_col_vector( x ) \
\
	( bli_obj_width( x ) == 1 )

#define bli_obj_has_zero_dim( obj ) \
\
	( bli_obj_length( obj ) == 0 || \
	  bli_obj_width( obj )  == 0 )

#define bli_obj_is_1x1( x ) \
\
	( bli_obj_length( x ) == 1 && \
	  bli_obj_width( x )  == 1 )


// Dimension modification

#define bli_obj_set_length( dim_m, obj ) \
{ \
	(obj).dim[BLIS_M] = dim_m; \
}

#define bli_obj_set_width( dim_n, obj ) \
{ \
	(obj).dim[BLIS_N] = dim_n; \
}

#define bli_obj_set_dim( mdim, dim_val, obj ) \
{ \
	(obj).dim[mdim] = dim_val; \
}

#define bli_obj_set_dims( dim_m, dim_n, obj ) \
{ \
	bli_obj_set_length( dim_m, obj ); \
	bli_obj_set_width( dim_n, obj ); \
}

#define bli_obj_set_dims_with_trans( trans, dim_m, dim_n, obj ) \
{ \
	if ( bli_does_notrans( trans ) ) \
	{ \
		bli_obj_set_length( dim_m, obj ); \
		bli_obj_set_width( dim_n, obj ); \
	} \
	else \
	{ \
		bli_obj_set_length( dim_n, obj ); \
		bli_obj_set_width( dim_m, obj ); \
	} \
}


// Stride/increment query

#define bli_obj_row_stride( obj ) \
\
	( (obj).rs )

#define bli_obj_col_stride( obj ) \
\
	( (obj).cs )

#define bli_obj_imag_stride( obj ) \
\
	( (obj).is )

#define bli_obj_row_stride_mag( obj ) \
\
	( bli_abs( bli_obj_row_stride( obj ) ) )

#define bli_obj_col_stride_mag( obj ) \
\
	( bli_abs( bli_obj_col_stride( obj ) ) )

#define bli_obj_imag_stride_mag( obj ) \
\
	( bli_abs( bli_obj_imag_stride( obj ) ) )

//
// NOTE: The following two macros differ from their non-obj counterparts
// in that they do not identify m x 1 and 1 x n objects as row-stored and
// column-stored, respectively, which is needed when considering packed
// objects. But this is okay, since none of the invocations of these
// "obj" macros are used on packed matrices.
//
#define bli_obj_is_row_stored( obj ) \
\
	( bli_obj_col_stride_mag( obj ) == 1 )

#define bli_obj_is_col_stored( obj ) \
\
	( bli_obj_row_stride_mag( obj ) == 1 )

#define bli_obj_is_gen_stored( obj ) \
\
	( bli_obj_row_stride_mag( obj ) != 1 && \
	  bli_obj_col_stride_mag( obj ) != 1 )

#define bli_obj_is_row_tilted( obj ) \
\
	( bli_obj_col_stride_mag( obj ) < bli_obj_row_stride_mag( obj ) )

#define bli_obj_is_col_tilted( obj ) \
\
	( bli_obj_row_stride_mag( obj ) < bli_obj_col_stride_mag( obj ) )


// Stride/increment modification

#define bli_obj_set_strides( row_stride, col_stride, obj ) \
{ \
	(obj).rs = row_stride; \
	(obj).cs = col_stride; \
}

#define bli_obj_set_imag_stride( imag_stride, obj ) \
{ \
	(obj).is = imag_stride; \
}


// Offset query

#define bli_obj_row_off( obj ) \
\
	( (obj).off[BLIS_M] )

#define bli_obj_col_off( obj ) \
\
	( (obj).off[BLIS_N] )

#define bli_obj_off( mdim, obj ) \
\
	( (obj).off[mdim] )


// Offset modification

#define bli_obj_set_off( mdim, offset, obj ) \
{ \
	(obj).off[mdim] = offset; \
}

#define bli_obj_set_offs( offset_m, offset_n, obj ) \
{ \
	bli_obj_set_off( BLIS_M, offset_m, obj ); \
	bli_obj_set_off( BLIS_N, offset_n, obj ); \
}

#define bli_obj_inc_off( mdim, offset, obj ) \
{ \
	(obj).off[mdim] += offset; \
}

#define bli_obj_inc_offm( offset, obj ) \
{ \
	bli_obj_inc_off( BLIS_M, offset, obj ); \
}

#define bli_obj_inc_offn( offset, obj ) \
{ \
	bli_obj_inc_off( BLIS_N, offset, obj ); \
}

#define bli_obj_inc_offs( offset_m, offset_n, obj ) \
{ \
	bli_obj_inc_off( BLIS_M, offset_m, obj ); \
	bli_obj_inc_off( BLIS_N, offset_n, obj ); \
}



// Diagonal offset query

#define bli_obj_diag_offset( obj ) \
\
	( (obj).diag_off )

#define bli_obj_diag_offset_after_trans( obj ) \
\
	( bli_obj_has_trans( obj ) ? -bli_obj_diag_offset( obj ) \
	                           :  bli_obj_diag_offset( obj ) )

#define bli_obj_has_main_diag( obj ) \
\
	( bli_obj_diag_offset( obj ) == 0 )

#define bli_obj_is_strictly_above_diag( obj ) \
\
	( ( doff_t )bli_obj_length( obj ) <= -bli_obj_diag_offset( obj ) )

#define bli_obj_is_strictly_below_diag( obj ) \
\
	( ( doff_t )bli_obj_width( obj )  <=  bli_obj_diag_offset( obj ) )

#define bli_obj_is_outside_diag( obj ) \
\
	( bli_obj_is_strictly_above_diag( obj ) || \
	  bli_obj_is_strictly_below_diag( obj ) )

#define bli_obj_intersects_diag( obj ) \
\
	( !bli_obj_is_strictly_above_diag( obj ) && \
	  !bli_obj_is_strictly_below_diag( obj ) )

#define bli_obj_is_unstored_subpart( obj ) \
\
	( ( bli_obj_root_is_lower( obj ) && bli_obj_is_strictly_above_diag( obj ) ) || \
	  ( bli_obj_root_is_upper( obj ) && bli_obj_is_strictly_below_diag( obj ) ) )


// Diagonal offset modification

#define bli_obj_set_diag_offset( offset, obj ) \
{ \
	(obj).diag_off  = ( doff_t )(offset); \
}

#define bli_obj_negate_diag_offset( obj ) \
{ \
	(obj).diag_off  = -(obj).diag_off; \
}

#define bli_obj_inc_diag_off( offset, obj ) \
{ \
	(obj).diag_off += ( doff_t )(offset); \
}


// Buffer address query

#define bli_obj_buffer( obj ) \
\
	( (obj).buffer )

// Buffer address modification

#define bli_obj_set_buffer( buf, obj ) \
{ \
	(obj).buffer = buf; \
}


// Bufferless scalar field query

#define bli_obj_internal_scalar_buffer( obj ) \
\
	&( (obj).scalar )

// Bufferless scalar field modification

#define bli_obj_set_internal_scalar( val, obj ) \
{ \
	(obj).scalar = val; \
}

#define bli_obj_copy_internal_scalar( a, b ) \
{ \
	(b).scalar = (a).scalar; \
}

// Element size query

#define bli_obj_elem_size( obj ) \
\
	( (obj).elem_size )

// Element size modification

#define bli_obj_set_elem_size( size, obj ) \
{ \
	(obj).elem_size = size; \
}

// Packed matrix info query

#define bli_obj_padded_length( obj ) \
\
	( (obj).m_padded )

#define bli_obj_padded_width( obj ) \
\
	( (obj).n_padded )

// Packed matrix info modification

#define bli_obj_set_buffer_to_mem( mem_p, obj ) \
{ \
	void* buf = bli_mem_buffer( mem_p ); \
	bli_obj_set_buffer( buf, obj ); \
} \

#define bli_obj_set_padded_length( m0, obj ) \
{ \
    (obj).m_padded = m0; \
}

#define bli_obj_set_padded_width( n0, obj ) \
{ \
    (obj).n_padded = n0; \
}

#define bli_obj_set_padded_dims( m0, n0, obj ) \
{ \
	bli_obj_set_padded_length( m0, obj ); \
	bli_obj_set_padded_width( n0, obj ); \
}


// Packed panel info query

#define bli_obj_panel_length( obj ) \
\
	( (obj).m_panel )

#define bli_obj_panel_width( obj ) \
\
	( (obj).n_panel )

#define bli_obj_panel_dim( obj ) \
\
	( (obj).pd )

#define bli_obj_panel_stride( obj ) \
\
	( (obj).ps )

// Packed panel info modification

#define bli_obj_set_panel_length( m0, obj ) \
{ \
	(obj).m_panel = m0; \
}

#define bli_obj_set_panel_width( n0, obj ) \
{ \
	(obj).n_panel = n0; \
}

#define bli_obj_set_panel_dims( m0, n0, obj ) \
{ \
	bli_obj_set_panel_length( m0, obj ); \
	bli_obj_set_panel_width( n0, obj ); \
}

#define bli_obj_set_panel_dim( panel_dim, obj ) \
{ \
	(obj).pd = panel_dim; \
}

#define bli_obj_set_panel_stride( panel_stride, obj ) \
{ \
	(obj).ps = panel_stride; \
}

 

// -- Miscellaneous object macros --

// Make a full alias (shallow copy)

#define bli_obj_alias_to( a, b ) \
{ \
	bli_obj_init_full_shallow_copy_of( a, b ); \
}

// Check if two objects are aliases of one another

#define bli_obj_is_alias_of( a, b ) \
\
	( (b).buffer == (a).buffer )

// Create an alias with a trans value applied.
// (Note: trans may include a conj component.)

#define bli_obj_alias_with_trans( trans, a, b ) \
{ \
	bli_obj_alias_to( a, b ); \
	bli_obj_apply_trans( trans, b ); \
}

// Create an alias with a conj value applied.

#define bli_obj_alias_with_conj( conja, a, b ) \
{ \
	bli_obj_alias_to( a, b ); \
	bli_obj_apply_conj( conja, b ); \
}


// Initialize object with default properties (info field)

#define bli_obj_set_defaults( obj ) \
{ \
	(obj).info = 0x0; \
	(obj).info = (obj).info | BLIS_BITVAL_DENSE | BLIS_BITVAL_GENERAL; \
}

// Initializor for global scalar constants

#define bli_obj_init_const( buffer0 ) \
{ \
	.root      = NULL, \
\
	.off       = { 0, 0 }, \
	.dim       = { 1, 1 }, \
	.diag_off  = 0, \
\
	.info      = 0x0 | BLIS_BITVAL_CONST_TYPE | \
	                   BLIS_BITVAL_DENSE      | \
	                   BLIS_BITVAL_GENERAL, \
	.elem_size = sizeof( constdata_t ), \
\
	.buffer    = buffer0, \
	.rs        = 1, \
	.cs        = 1, \
	.is        = 1  \
}

#define bli_obj_init_constdata( val ) \
{ \
	.s =           ( float  )val, \
	.d =           ( double )val, \
	.c = { .real = ( float  )val, .imag = 0.0f }, \
	.z = { .real = ( double )val, .imag = 0.0 }, \
	.i =           ( gint_t )val, \
}

// Submatrix/scalar buffer acquisition

#if 0
#define BLIS_CONSTANT_SLOT_SIZE  BLIS_MAX_TYPE_SIZE
#define BLIS_CONSTANT_SIZE       ( 5 * BLIS_CONSTANT_SLOT_SIZE )

#define bli_obj_buffer_for_const( dt, obj ) \
\
	( void* )( \
	           ( ( char* )( bli_obj_buffer( obj ) ) ) + \
                 ( dim_t )( dt * BLIS_CONSTANT_SLOT_SIZE ) \
	         )
#endif
#define bli_obj_buffer_for_const( dt, obj ) \
\
	( dt == BLIS_FLOAT    ? ( void * )&((( constdata_t* )bli_obj_buffer( obj ))->s) : \
	  ( dt == BLIS_DOUBLE   ? ( void * )&((( constdata_t* )bli_obj_buffer( obj ))->d) : \
	    ( dt == BLIS_SCOMPLEX ? ( void * )&((( constdata_t* )bli_obj_buffer( obj ))->c) : \
	      ( dt == BLIS_DCOMPLEX ? ( void * )&((( constdata_t* )bli_obj_buffer( obj ))->z) : \
	                                ( void * )&((( constdata_t* )bli_obj_buffer( obj ))->i)   \
	      ) \
	    ) \
	  ) \
	)

#define bli_obj_buffer_at_off( obj ) \
\
	( void* )( \
	           ( ( char* )( bli_obj_buffer   ( obj ) ) + \
                 ( dim_t )( bli_obj_elem_size( obj ) ) * \
                            ( bli_obj_col_off( obj ) * bli_obj_col_stride( obj ) + \
                              bli_obj_row_off( obj ) * bli_obj_row_stride( obj ) \
                            ) \
               ) \
	         )

#define bli_obj_buffer_for_1x1( dt, obj ) \
\
	( void* )( bli_obj_is_const( obj ) ? ( bli_obj_buffer_for_const( dt, obj ) ) \
	                                   : ( bli_obj_buffer_at_off( obj ) ) \
	         )


// Swap objects

#define bli_obj_swap( a, b ) \
{ \
	obj_t t_; \
	t_ = b; b = a; a = t_; \
}


// Swap object pointers

#define bli_obj_swap_pointers( a, b ) \
{ \
	obj_t* t_; \
	t_ = b; b = a; a = t_; \
}


// If a transposition is needed, induce one: swap dimensions, increments
// and offsets, and then clear the trans bit.

#define bli_obj_induce_trans( obj ) \
{ \
	{ \
		 \
		dim_t  m_        = bli_obj_length( obj ); \
		dim_t  n_        = bli_obj_width( obj ); \
		inc_t  rs_       = bli_obj_row_stride( obj ); \
		inc_t  cs_       = bli_obj_col_stride( obj ); \
		dim_t  offm_     = bli_obj_row_off( obj ); \
		dim_t  offn_     = bli_obj_col_off( obj ); \
		doff_t diag_off_ = bli_obj_diag_offset( obj ); \
\
		bli_obj_set_dims( n_, m_, obj ); \
		bli_obj_set_strides( cs_, rs_, obj ); \
		bli_obj_set_offs( offn_, offm_, obj ); \
		bli_obj_set_diag_offset( -diag_off_, obj ); \
\
		if ( bli_obj_is_upper_or_lower( obj ) ) \
			bli_obj_toggle_uplo( obj ); \
\
		 \
		dim_t  m_padded_ = bli_obj_padded_length( obj ); \
		dim_t  n_padded_ = bli_obj_padded_width( obj ); \
		dim_t  m_panel_  = bli_obj_panel_length( obj ); \
		dim_t  n_panel_  = bli_obj_panel_width( obj ); \
\
		bli_obj_set_padded_dims( n_padded_, m_padded_, obj ); \
		bli_obj_set_panel_dims( n_panel_, m_panel_, obj ); \
\
		 \
	} \
}

// Sometimes we need to "reflect" a partition because the data we want is
// actually stored on the other side of the diagonal. The nuts and bolts of
// this macro look a lot like an induced transposition, except that the row
// and column strides are left unchanged (which, of course, drastically
// changes the effect of the macro).

#define bli_obj_reflect_about_diag( obj ) \
{ \
	{ \
		dim_t  m_        = bli_obj_length( obj ); \
		dim_t  n_        = bli_obj_width( obj ); \
		dim_t  offm_     = bli_obj_row_off( obj ); \
		dim_t  offn_     = bli_obj_col_off( obj ); \
		doff_t diag_off_ = bli_obj_diag_offset( obj ); \
\
		bli_obj_set_dims( n_, m_, obj ); \
		bli_obj_set_offs( offn_, offm_, obj ); \
		bli_obj_set_diag_offset( -diag_off_, obj ); \
\
		bli_obj_toggle_trans( obj ); \
	} \
}

#endif
// end bli_obj_macro_defs.h
// begin bli_param_macro_defs.h


#ifndef BLIS_PARAM_MACRO_DEFS_H
#define BLIS_PARAM_MACRO_DEFS_H


// -- Parameter query macros --

// buffer

#define bli_is_aligned_to( p, size ) \
\
	( ( uintptr_t )(p) % ( uintptr_t )(size) == 0 )

#define bli_is_unaligned_to( p, size ) \
\
	( ( uintptr_t )(p) % ( uintptr_t )(size) != 0 )

#define bli_offset_past_alignment( p, size ) \
\
	( ( uintptr_t )(p) % ( uintptr_t )(size) )


// datatype

#define bli_is_float( dt ) \
\
	( dt == BLIS_FLOAT )

#define bli_is_double( dt ) \
\
	( dt == BLIS_DOUBLE )

#define bli_is_scomplex( dt ) \
\
	( dt == BLIS_SCOMPLEX )

#define bli_is_dcomplex( dt ) \
\
	( dt == BLIS_DCOMPLEX )

#define bli_is_constant( dt ) \
\
	( dt == BLIS_CONSTANT )

#define bli_is_int( dt ) \
\
	( dt == BLIS_INT )

#define bli_is_real( dt ) \
\
    ( bli_is_float( dt ) || \
	  bli_is_double( dt ) )

#define bli_is_complex( dt ) \
\
    ( bli_is_scomplex( dt ) || \
	  bli_is_dcomplex( dt ) )

#define bli_is_single_prec( dt ) \
\
    ( bli_is_float( dt ) || \
	  bli_is_scomplex( dt ) )

#define bli_is_double_prec( dt ) \
\
    ( bli_is_double( dt ) || \
	  bli_is_dcomplex( dt ) )

#define bli_datatype_proj_to_real( dt ) \
\
	( dt & ~BLIS_BITVAL_COMPLEX )

#define bli_datatype_proj_to_complex( dt ) \
\
	( dt &  BLIS_BITVAL_COMPLEX )

#define bli_domain_of_dt( dt ) \
\
	( dt & BLIS_DOMAIN_BIT )


// side

#define bli_is_left( side ) \
\
    ( side == BLIS_LEFT )

#define bli_is_right( side ) \
\
    ( side == BLIS_RIGHT )

#define bli_side_toggled( side ) \
\
	( bli_is_left( side ) ? BLIS_RIGHT : BLIS_LEFT )

#define bli_toggle_side( side ) \
{ \
	side = bli_side_toggled( side ); \
}


// uplo

#define bli_is_lower( uplo ) \
\
    ( uplo == BLIS_LOWER )

#define bli_is_upper( uplo ) \
\
    ( uplo == BLIS_UPPER )

#define bli_is_upper_or_lower( uplo ) \
\
    ( bli_is_upper( uplo ) || \
	  bli_is_lower( uplo ) )

#define bli_is_dense( uplo ) \
\
    ( uplo == BLIS_DENSE )

#define bli_is_zeros( uplo ) \
\
    ( uplo == BLIS_ZEROS )

#define bli_uplo_toggled( uplo ) \
\
	( bli_is_upper_or_lower( uplo ) ? \
	  ( ( uplo ^ BLIS_LOWER_BIT ) ^ BLIS_UPPER_BIT ) : uplo \
	)

#define bli_toggle_uplo( uplo ) \
{ \
	uplo = bli_uplo_toggled( uplo ); \
}

#define bli_set_uplo_with_trans( trans, uplo, uplo_trans ) \
{ \
	if ( bli_does_notrans( trans ) ) { uplo_trans = uplo;                     } \
	else                             { uplo_trans = bli_uplo_toggled( uplo ); } \
}


// structure

#define bli_is_general( struc ) \
\
	( struc == BLIS_GENERAL )

#define bli_is_hermitian( struc ) \
\
	( struc == BLIS_HERMITIAN )

#define bli_is_symmetric( struc ) \
\
	( struc == BLIS_SYMMETRIC )

#define bli_is_triangular( struc ) \
\
	( struc == BLIS_TRIANGULAR )

#define bli_is_herm_or_symm( struc ) \
\
	( bli_is_hermitian( struc ) || \
	  bli_is_symmetric( struc ) )



// conj

#define bli_is_noconj( conjval ) \
\
    ( conjval == BLIS_NO_CONJUGATE )

#define bli_is_conj( conjval ) \
\
    ( conjval == BLIS_CONJUGATE )

#define bli_conj_toggled( conjval ) \
\
	( conjval ^ BLIS_CONJ_BIT )

#define bli_apply_conj( conjapp, conjval )\
\
	( conjval ^ (conjapp) )

#define bli_toggle_conj( conjval ) \
{ \
	conjval = bli_conj_toggled( conjval ); \
}


// trans

#define bli_is_notrans( trans ) \
\
    ( trans == BLIS_NO_TRANSPOSE )

#define bli_is_trans( trans ) \
\
    ( trans == BLIS_TRANSPOSE )

#define bli_is_conjnotrans( trans ) \
\
    ( trans == BLIS_CONJ_NO_TRANSPOSE )

#define bli_is_conjtrans( trans ) \
\
    ( trans == BLIS_CONJ_TRANSPOSE )

#define bli_does_notrans( trans ) \
\
	( (~(trans) & BLIS_TRANS_BIT ) == BLIS_BITVAL_TRANS )

#define bli_does_trans( trans ) \
\
	( (  trans  & BLIS_TRANS_BIT ) == BLIS_BITVAL_TRANS )

#define bli_does_noconj( trans ) \
\
	( (~(trans) & BLIS_CONJ_BIT  ) == BLIS_BITVAL_CONJ )

#define bli_does_conj( trans ) \
\
	( (  trans  & BLIS_CONJ_BIT  ) == BLIS_BITVAL_CONJ )

#define bli_extract_trans( trans ) \
\
	(    trans  & BLIS_TRANS_BIT  )

#define bli_extract_conj( trans ) \
\
	(    trans  & BLIS_CONJ_BIT  )

#define bli_trans_toggled( trans ) \
\
	( trans ^ BLIS_TRANS_BIT )

#define bli_toggle_trans( trans ) \
{ \
	trans = bli_trans_toggled( trans ); \
}


// diag

#define bli_is_nonunit_diag( diag ) \
\
    ( diag == BLIS_NONUNIT_DIAG )

#define bli_is_unit_diag( diag ) \
\
    ( diag == BLIS_UNIT_DIAG )


// dimension-related

#define bli_zero_dim1( n ) \
\
	( (n) == 0 )

#define bli_zero_dim2( m, n ) \
\
	( (m) == 0 || (n) == 0 )

#define bli_zero_dim3( m, n, k ) \
\
	( (m) == 0 || (n) == 0 || (k) == 0 )

#define bli_nonzero_dim( n ) \
\
	( (n) > 0 )

#define bli_vector_dim( m, n ) \
\
	( (m) == 1 ? (n) : (m) )

#define bli_is_vector( m, n ) \
\
	( (m) == 1 || (n) == 1 )

#define bli_is_row_vector( m, n ) \
\
	( (m) == 1 )

#define bli_is_col_vector( m, n ) \
\
	( (n) == 1 )

#define bli_set_dim_with_side( side, m, n, dim ) \
{ \
	if ( bli_is_left( side ) ) { dim = m; } \
	else                       { dim = n; } \
}

#define bli_set_dims_with_trans( trans, m, n, mtrans, ntrans ) \
{ \
	if ( bli_does_notrans( trans ) ) { mtrans = m; ntrans = n; } \
	else                             { mtrans = n; ntrans = m; } \
}

#define bli_set_dims_incs_with_trans( trans, m, n, rs, cs, mt, nt, rst, cst ) \
{ \
	if ( bli_does_notrans( trans ) ) { mt = m; nt = n; rst = rs; cst = cs; } \
	else                             { mt = n; nt = m; rst = cs; cst = rs; } \
}


// blocksize-related

#define bli_determine_blocksize_dim_f( i, dim, b_alg ) \
\
	( bli_min( b_alg, dim - i ) )

#define bli_determine_blocksize_dim_b( i, dim, b_alg ) \
\
	( i == 0 && dim % b_alg != 0 ? dim % b_alg \
	                             : b_alg )


// stride-related

#define bli_vector_inc( trans, m, n, rs, cs ) \
\
	( bli_does_notrans( trans ) ? ( m == 1 ? (cs) : (rs) ) \
                                : ( m == 1 ? (rs) : (cs) ) )

#define bli_is_row_stored( rs, cs ) \
\
	( bli_abs( cs ) == 1 )

#define bli_is_col_stored( rs, cs ) \
\
	( bli_abs( rs ) == 1 )

#define bli_is_row_stored_f( m, n, rs, cs ) \
\
	( cs == 1 && ( rs > 1 || n == 1 ) )

#define bli_is_col_stored_f( m, n, rs, cs ) \
\
	( rs == 1 && ( cs > 1 || m == 1 ) )

#define bli_is_gen_stored( rs, cs ) \
\
	( bli_abs( rs ) != 1 && \
	  bli_abs( cs ) != 1 )

#define bli_is_row_tilted( m, n, rs, cs ) \
\
	( bli_abs( cs ) == bli_abs( rs ) ? n < m \
	                                 : bli_abs( cs ) < bli_abs( rs ) )

#define bli_is_col_tilted( m, n, rs, cs ) \
\
	( bli_abs( rs ) == bli_abs( cs ) ? m < n \
	                                 : bli_abs( rs ) < bli_abs( cs ) )

#define bli_has_nonunit_inc1( inc1 ) \
\
	( inc1 != 1 )

#define bli_has_nonunit_inc2( inc1, inc2 ) \
\
	( inc1 != 1 || inc2 != 1 )

#define bli_has_nonunit_inc3( inc1, inc2, inc3 ) \
\
	( inc1 != 1 || inc2 != 1 || inc3 != 1 )


// diag offset-related

#define bli_negate_diag_offset( diagoff ) \
{ \
	diagoff = -diagoff; \
}

#define bli_shift_diag_offset_to_grow_uplo( uplo, diagoff ) \
{ \
	if      ( bli_is_upper( uplo ) ) diagoff -= 1; \
	else if ( bli_is_lower( uplo ) ) diagoff += 1; \
}

#define bli_shift_diag_offset_to_shrink_uplo( uplo, diagoff ) \
{ \
	if      ( bli_is_upper( uplo ) ) diagoff += 1; \
	else if ( bli_is_lower( uplo ) ) diagoff -= 1; \
}

#define bli_diag_offset_with_trans( trans, diagoff ) \
\
	( bli_does_trans( trans ) ? -diagoff : diagoff )

#define bli_is_strictly_above_diag( diagoff, trans, m, n ) \
\
	( bli_does_trans( trans ) ? ( ( doff_t )n <= -diagoff ) \
	                          : ( ( doff_t )m <= -diagoff ) )

#define bli_is_strictly_below_diag( diagoff, trans, m, n ) \
\
	( bli_does_trans( trans ) ? ( ( doff_t )m <=  diagoff ) \
	                          : ( ( doff_t )n <=  diagoff ) )

#define bli_is_outside_diag( diagoff, trans, m, n ) \
\
	( bli_is_strictly_above_diag( diagoff, trans, m, n ) || \
	  bli_is_strictly_below_diag( diagoff, trans, m, n ) )

#define bli_is_stored_subpart( diagoff, trans, uplo, m, n ) \
\
	( ( bli_is_upper( uplo ) && bli_is_strictly_above_diag( diagoff, trans, m, n ) ) || \
	  ( bli_is_lower( uplo ) && bli_is_strictly_below_diag( diagoff, trans, m, n ) ) )

#define bli_is_unstored_subpart( diagoff, trans, uplo, m, n ) \
\
	( ( bli_is_upper( uplo ) && bli_is_strictly_below_diag( diagoff, trans, m, n ) ) || \
	  ( bli_is_lower( uplo ) && bli_is_strictly_above_diag( diagoff, trans, m, n ) ) )

#define bli_is_strictly_above_diag_n( diagoff, m, n ) \
\
	( ( doff_t )m <= -diagoff ) \

#define bli_is_strictly_below_diag_n( diagoff, m, n ) \
\
	( ( doff_t )n <=  diagoff ) \

#define bli_intersects_diag_n( diagoff, m, n ) \
\
	( !bli_is_strictly_above_diag_n( diagoff, m, n ) && \
	  !bli_is_strictly_below_diag_n( diagoff, m, n ) )

#define bli_is_stored_subpart_n( diagoff, uplo, m, n ) \
\
	( ( bli_is_upper( uplo ) && bli_is_strictly_above_diag_n( diagoff, m, n ) ) || \
	  ( bli_is_lower( uplo ) && bli_is_strictly_below_diag_n( diagoff, m, n ) ) )

#define bli_is_unstored_subpart_n( diagoff, uplo, m, n ) \
\
	( ( bli_is_upper( uplo ) && bli_is_strictly_below_diag_n( diagoff, m, n ) ) || \
	  ( bli_is_lower( uplo ) && bli_is_strictly_above_diag_n( diagoff, m, n ) ) )


// pruning-related

#define bli_prune_unstored_region_top_l( diagoff, m, n, offm_inc ) \
{ \
	offm_inc = 0; \
\
	 \
	if ( diagoff < 0 ) \
	{ \
		m        = m    + diagoff; \
		offm_inc =      - diagoff; \
		diagoff  = 0; \
	} \
}

#define bli_prune_unstored_region_right_l( diagoff, m, n, offn_inc ) \
{ \
	offn_inc = 0; \
\
	 \
	if ( n > diagoff + m ) \
	{ \
		n = diagoff + m; \
	} \
}

#define bli_prune_unstored_region_left_u( diagoff, m, n, offn_inc ) \
{ \
	offn_inc = 0; \
\
	 \
	if ( diagoff > 0 ) \
	{ \
		n        = n    - diagoff; \
		offn_inc =      + diagoff; \
		diagoff  = 0; \
	} \
}

#define bli_prune_unstored_region_bottom_u( diagoff, m, n, offm_inc ) \
{ \
	offm_inc = 0; \
\
	 \
	if ( m > -diagoff + n ) \
	{ \
		m = -diagoff + n; \
	} \
}


// thread range-related

#define bli_rotate180_trapezoid( diagoff, uplo ) \
{ \
	diagoff = n - diagoff - m; \
	bli_toggle_uplo( uplo ); \
}

#define bli_reverse_index_direction( start, end, n ) \
{ \
	dim_t start2_ = n - start; \
	dim_t end2_   = n - end; \
	start = end2_; \
	end   = start2_; \
}

#define bli_reflect_about_diag( diagoff, uplo, m, n ) \
{ \
	bli_swap_dims( m, n ); \
	bli_negate_diag_offset( diagoff ); \
	bli_toggle_uplo( uplo ); \
}


// mdim_t-related

#define bli_is_m_dim( mdim ) \
\
	( mdim == BLIS_M )

#define bli_is_n_dim( mdim ) \
\
	( mdim == BLIS_N )

#define bli_dim_toggled( mdim ) \
\
	( mdim == BLIS_M ? BLIS_N : BLIS_M )

#define bli_toggle_dim( mdim ) \
{ \
	mdim = bli_dim_toggled( mdim ); \
}



// index-related

#define bli_is_edge_f( i1, iter, left ) \
\
	( i1 == iter - 1 && left != 0 )

#define bli_is_not_edge_f( i1, iter, left ) \
\
	( i1 != iter - 1 || left == 0 )

#define bli_is_edge_b( i1, iter, left ) \
\
	( i1 == 0 && left != 0 )

#define bli_is_not_edge_b( i1, iter, left ) \
\
	( i1 != 0 || left == 0 )

#define bli_is_last_iter( i1, niter, tid, nth ) \
\
	( i1 == niter - 1 - ( ( niter - tid - 1 ) % nth ) ) \
	


// packbuf_t-related

#define bli_packbuf_index( buf_type ) \
\
	( ( (buf_type) & BLIS_PACK_BUFFER_BITS ) >> BLIS_PACK_BUFFER_SHIFT )


// pack_t-related

#define bli_is_packed( schema ) \
\
	( ( schema & BLIS_PACK_BIT  ) )

#define bli_is_row_packed( schema ) \
\
	( ( schema & BLIS_PACK_RC_BIT  ) == ( BLIS_BITVAL_PACKED_UNSPEC ^ \
	                                      BLIS_BITVAL_PACKED_ROWS    ) )

#define bli_is_col_packed( schema ) \
\
	( ( schema & BLIS_PACK_RC_BIT  ) == ( BLIS_BITVAL_PACKED_UNSPEC ^ \
	                                      BLIS_BITVAL_PACKED_COLUMNS ) )

#define bli_is_panel_packed( schema ) \
\
	( ( schema & BLIS_PACK_PANEL_BIT ) )

#define bli_is_4mi_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_4MI )

#define bli_is_3mi_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_3MI )

#define bli_is_3ms_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_3MS )

#define bli_is_ro_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_RO )

#define bli_is_io_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_IO )

#define bli_is_rpi_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_RPI )

#define bli_is_rih_packed( schema ) \
\
	( bli_is_ro_packed( schema ) || \
	  bli_is_io_packed( schema ) || \
	  bli_is_rpi_packed( schema ) )

#define bli_is_1r_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_1R )

#define bli_is_1e_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == BLIS_BITVAL_1E )

#define bli_is_1m_packed( schema ) \
\
	( bli_is_1r_packed( schema ) || \
	  bli_is_1e_packed( schema ) )

#define bli_is_nat_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) == 0 )

#define bli_is_ind_packed( schema ) \
\
	( ( schema & BLIS_PACK_FORMAT_BITS ) != 0 )

#define bli_pack_schema_index( schema ) \
\
	( ( (schema) & BLIS_PACK_FORMAT_BITS ) >> BLIS_PACK_FORMAT_SHIFT )



// pointer-related

// p1 = p0 + (num/dem)
#define bli_ptr_add( p1, p0, num, dem ) \
{ \
	p1 = ( typeof( p1 ) ) \
	     ( ( char* )(p0) + ( (   (num) * sizeof( *(p0) ) \
	                         ) / (dem) \
	                       ) \
	     ); \
}

#define bli_is_null( p ) \
\
	( p == NULL )

#define bli_is_nonnull( p ) \
\
	( p != NULL )


// return datatype for char

#define bli_stype ( BLIS_FLOAT    )
#define bli_dtype ( BLIS_DOUBLE   )
#define bli_ctype ( BLIS_SCOMPLEX )
#define bli_ztype ( BLIS_DCOMPLEX )


// return datatype "union" for char pair

#define bli_sstypeunion() ( BLIS_FLOAT    )
#define bli_sdtypeunion() ( BLIS_DOUBLE   )
#define bli_sctypeunion() ( BLIS_SCOMPLEX )
#define bli_sztypeunion() ( BLIS_DCOMPLEX )

#define bli_dstypeunion() ( BLIS_DOUBLE   )
#define bli_ddtypeunion() ( BLIS_DOUBLE   )
#define bli_dctypeunion() ( BLIS_DCOMPLEX )
#define bli_dztypeunion() ( BLIS_DCOMPLEX )

#define bli_cstypeunion() ( BLIS_SCOMPLEX )
#define bli_cdtypeunion() ( BLIS_DCOMPLEX )
#define bli_cctypeunion() ( BLIS_SCOMPLEX )
#define bli_cztypeunion() ( BLIS_DCOMPLEX )

#define bli_zstypeunion() ( BLIS_DCOMPLEX )
#define bli_zdtypeunion() ( BLIS_DCOMPLEX )
#define bli_zctypeunion() ( BLIS_DCOMPLEX )
#define bli_zztypeunion() ( BLIS_DCOMPLEX )


// return default format specifier for char

#define bli_sformatspec() "%9.2e"
#define bli_dformatspec() "%9.2e"
#define bli_cformatspec() "%9.2e + %9.2e "
#define bli_zformatspec() "%9.2e + %9.2e "
#define bli_iformatspec() "%6d"


// set scalar datatype and buffer

#define bli_set_scalar_dt_buffer( obj_scalar, dt_aux, dt_scalar, buf_scalar ) \
{ \
	if ( bli_obj_is_const( *(obj_scalar) ) ) \
	{ \
		dt_scalar  = dt_aux; \
		buf_scalar = bli_obj_buffer_for_1x1( dt_scalar, *(obj_scalar) ); \
	} \
	else \
	{ \
		dt_scalar  = bli_obj_datatype( *(obj_scalar) ); \
		buf_scalar = bli_obj_buffer_at_off( *(obj_scalar) ); \
	} \
}

// set constant datatype and buffer

#define bli_set_const_dt_buffer( obj_scalar, dt_aux, dt_scalar, buf_scalar ) \
{ \
	{ \
		dt_scalar  = dt_aux; \
		buf_scalar = bli_obj_buffer_for_1x1( dt_scalar, *(obj_scalar) ); \
	} \
}


// Set dimensions, increments, effective uplo/diagoff, etc for ONE matrix
// argument.

#define bli_set_dims_incs_uplo_1m( diagoffa, diaga, \
                                   uploa,    m,          n,      rs_a, cs_a, \
                                   uplo_eff, n_elem_max, n_iter, inca, lda, \
                                   ij0, n_shift ) \
{ \
	 \
	ij0     = 0; \
	n_shift = 0; \
\
	 \
	if ( bli_is_unstored_subpart( diagoffa, BLIS_NO_TRANSPOSE, uploa, m, n ) ) \
	{ \
		uplo_eff = BLIS_ZEROS; \
	} \
	else \
	{ \
		doff_t diagoffa_use_ = diagoffa; \
		doff_t diagoff_eff_; \
		dim_t  n_iter_max_; \
\
		if ( bli_is_unit_diag( diaga ) ) \
			bli_shift_diag_offset_to_shrink_uplo( uploa, diagoffa_use_ ); \
\
		 \
		if ( bli_is_stored_subpart( diagoffa_use_, BLIS_NO_TRANSPOSE, uploa, m, n ) ) \
			uploa = BLIS_DENSE; \
\
		n_iter_max_  = n; \
		n_elem_max  = m; \
		inca        = rs_a; \
		lda         = cs_a; \
		uplo_eff    = uploa; \
		diagoff_eff_ = diagoffa_use_; \
\
		if ( bli_is_row_tilted( n_elem_max, n_iter_max_, inca, lda ) ) \
		{ \
			bli_swap_dims( n_iter_max_, n_elem_max ); \
			bli_swap_incs( inca, lda ); \
			bli_toggle_uplo( uplo_eff ); \
			bli_negate_diag_offset( diagoff_eff_ ); \
		} \
\
		if ( bli_is_dense( uplo_eff ) ) \
		{ \
			n_iter = n_iter_max_; \
		} \
		else if ( bli_is_upper( uplo_eff ) ) \
		{ \
			if ( diagoff_eff_ < 0 ) \
			{ \
				ij0        = 0; \
				n_shift    = -diagoff_eff_; \
				n_elem_max = bli_min( n_elem_max, n_shift + bli_min( m, n ) ); \
				n_iter     = n_iter_max_; \
			} \
			else \
			{ \
				ij0        = diagoff_eff_; \
				n_shift    = 0; \
				n_iter     = n_iter_max_ - diagoff_eff_; \
			} \
		} \
		else  \
		{ \
			if ( diagoff_eff_ < 0 ) \
			{ \
				ij0        = -diagoff_eff_; \
				n_shift    = 0; \
				n_elem_max = n_elem_max + diagoff_eff_; \
				n_iter     = bli_min( n_elem_max, bli_min( m, n ) ); \
			} \
			else \
			{ \
				ij0        = 0; \
				n_shift    = diagoff_eff_; \
				n_iter     = bli_min( n_iter_max_, n_shift + bli_min( m, n ) ); \
			} \
		} \
	} \
}

// Set dimensions, increments, effective uplo/diagoff, etc for ONE matrix
// argument (without column-wise stride optimization).

#define bli_set_dims_incs_uplo_1m_noswap( \
          diagoffa, diaga, \
          uploa,    m,          n,      rs_a, cs_a, \
          uplo_eff, n_elem_max, n_iter, inca, lda, \
          ij0, n_shift \
        ) \
{ \
	 \
	ij0     = 0; \
	n_shift = 0; \
\
	 \
	if ( bli_is_unstored_subpart( diagoffa, BLIS_NO_TRANSPOSE, uploa, m, n ) ) \
	{ \
		uplo_eff = BLIS_ZEROS; \
	} \
	else \
	{ \
		doff_t diagoffa_use_ = diagoffa; \
		doff_t diagoff_eff_; \
		dim_t  n_iter_max_; \
\
		if ( bli_is_unit_diag( diaga ) ) \
			bli_shift_diag_offset_to_shrink_uplo( uploa, diagoffa_use_ ); \
\
		 \
		if ( bli_is_stored_subpart( diagoffa_use_, BLIS_NO_TRANSPOSE, uploa, m, n ) ) \
			uploa = BLIS_DENSE; \
\
		n_iter_max_  = n; \
		n_elem_max  = m; \
		inca        = rs_a; \
		lda         = cs_a; \
		uplo_eff    = uploa; \
		diagoff_eff_ = diagoffa_use_; \
\
		if ( bli_is_dense( uplo_eff ) ) \
		{ \
			n_iter = n_iter_max_; \
		} \
		else if ( bli_is_upper( uplo_eff ) ) \
		{ \
			if ( diagoff_eff_ < 0 ) \
			{ \
				ij0        = 0; \
				n_shift    = -diagoff_eff_; \
				n_elem_max = bli_min( n_elem_max, n_shift + bli_min( m, n ) ); \
				n_iter     = n_iter_max_; \
			} \
			else \
			{ \
				ij0        = diagoff_eff_; \
				n_shift    = 0; \
				n_iter     = n_iter_max_ - diagoff_eff_; \
			} \
		} \
		else  \
		{ \
			if ( diagoff_eff_ < 0 ) \
			{ \
				ij0        = -diagoff_eff_; \
				n_shift    = 0; \
				n_elem_max = n_elem_max + diagoff_eff_; \
				n_iter     = bli_min( n_elem_max, bli_min( m, n ) ); \
			} \
			else \
			{ \
				ij0        = 0; \
				n_shift    = diagoff_eff_; \
				n_iter     = bli_min( n_iter_max_, n_shift + bli_min( m, n ) ); \
			} \
		} \
	} \
}

// Set dimensions, increments, effective uplo/diagoff, etc for TWO matrix
// arguments.

#define bli_set_dims_incs_uplo_2m( \
          diagoffa, diaga, transa, \
          uploa,    m,          n,      rs_a, cs_a, rs_b, cs_b, \
          uplo_eff, n_elem_max, n_iter, inca, lda,  incb, ldb, \
          ij0, n_shift \
        ) \
{ \
	 \
	ij0     = 0; \
	n_shift = 0; \
\
	 \
	if ( bli_is_unstored_subpart( diagoffa, transa, uploa, m, n ) ) \
	{ \
		uplo_eff = BLIS_ZEROS; \
	} \
	else \
	{ \
		doff_t diagoffa_use_ = diagoffa; \
		doff_t diagoff_eff_; \
		dim_t  n_iter_max_; \
\
		if ( bli_is_unit_diag( diaga ) ) \
			bli_shift_diag_offset_to_shrink_uplo( uploa, diagoffa_use_ ); \
\
		 \
		if ( bli_is_stored_subpart( diagoffa_use_, transa, uploa, m, n ) ) \
			uploa = BLIS_DENSE; \
\
		n_iter_max_  = n; \
		n_elem_max  = m; \
		inca        = rs_a; \
		lda         = cs_a; \
		incb        = rs_b; \
		ldb         = cs_b; \
		uplo_eff    = uploa; \
		diagoff_eff_ = diagoffa_use_; \
\
		if ( bli_does_trans( transa ) ) \
		{ \
			bli_swap_incs( inca, lda ); \
			bli_toggle_uplo( uplo_eff ); \
			bli_negate_diag_offset( diagoff_eff_ ); \
		} \
\
		if ( bli_is_row_tilted( n_elem_max, n_iter_max_, incb, ldb ) && \
		     bli_is_row_tilted( n_elem_max, n_iter_max_, inca, lda ) ) \
		{ \
			bli_swap_dims( n_iter_max_, n_elem_max ); \
			bli_swap_incs( inca, lda ); \
			bli_swap_incs( incb, ldb ); \
			bli_toggle_uplo( uplo_eff ); \
			bli_negate_diag_offset( diagoff_eff_ ); \
		} \
\
		if ( bli_is_dense( uplo_eff ) ) \
		{ \
			n_iter = n_iter_max_; \
		} \
		else if ( bli_is_upper( uplo_eff ) ) \
		{ \
			if ( diagoff_eff_ < 0 ) \
			{ \
 \
				ij0        = 0; \
				n_shift    = -diagoff_eff_; \
				n_elem_max = bli_min( n_elem_max, n_shift + bli_min( m, n ) ); \
				n_iter     = n_iter_max_; \
			} \
			else \
			{ \
 \
				ij0        = diagoff_eff_; \
				n_shift    = 0; \
				n_iter     = n_iter_max_ - diagoff_eff_; \
			} \
		} \
		else  \
		{ \
			if ( diagoff_eff_ < 0 ) \
			{ \
 \
				ij0        = -diagoff_eff_; \
				n_shift    = 0; \
				n_elem_max = n_elem_max + diagoff_eff_; \
				n_iter     = bli_min( n_elem_max, bli_min( m, n ) ); \
			} \
			else \
			{ \
 \
				ij0        = 0; \
				n_shift    = diagoff_eff_; \
				n_iter     = bli_min( n_iter_max_, n_shift + bli_min( m, n ) ); \
			} \
		} \
	} \
}

// Set dimensions, increments, etc for ONE matrix argument when operating
// on the diagonal.

#define bli_set_dims_incs_1d( diagoffx, \
                              m, n, rs_x, cs_x, \
                              offx, n_elem, incx ) \
{ \
	if ( diagoffx < 0 ) \
	{ \
		n_elem = bli_min( m - ( dim_t )(-diagoffx), n ); \
		offx   = ( dim_t )(-diagoffx) * rs_x; \
	} \
	else \
	{ \
		n_elem = bli_min( n - ( dim_t )( diagoffx), m ); \
		offx   = ( dim_t )( diagoffx) * cs_x; \
	} \
\
	incx = rs_x + cs_x; \
}

// Set dimensions, increments, etc for TWO matrix arguments when operating
// on diagonals.

#define bli_set_dims_incs_2d( diagoffx, transx, \
                              m, n, rs_x, cs_x, rs_y, cs_y, \
                              offx, offy, n_elem, incx, incy ) \
{ \
	doff_t diagoffy_ = bli_diag_offset_with_trans( transx, diagoffx ); \
\
	if ( diagoffx < 0 ) offx = -diagoffx * rs_x; \
	else                offx =  diagoffx * cs_x; \
\
	if ( diagoffy_ < 0 ) \
	{ \
		n_elem = bli_min( m - ( dim_t )(-diagoffy_), n ); \
		offy   = -diagoffy_ * rs_y; \
	} \
	else \
	{ \
		n_elem = bli_min( n - ( dim_t )( diagoffy_), m ); \
		offy   = diagoffy_ * cs_y; \
	} \
\
	incx = rs_x + cs_x; \
	incy = rs_y + cs_y; \
}

// -- Function caller/chooser macros --

#define bli_call_ft_2( dt, fname, o0, o1 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1); \
}
#define bli_call_ft_3( dt, fname, o0, o1, o2 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2); \
}
#define bli_call_ft_3i( dt, fname, o0, o1, o2 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2); \
	else if ( bli_is_int( dt )      ) PASTEMAC(i,fname)(o0,o1,o2); \
}
#define bli_call_ft_4( dt, fname, o0, o1, o2, o3 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3); \
}
#define bli_call_ft_5( dt, fname, o0, o1, o2, o3, o4 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4); \
}
#define bli_call_ft_6( dt, fname, o0, o1, o2, o3, o4, o5 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5); \
}
#define bli_call_ft_7( dt, fname, o0, o1, o2, o3, o4, o5, o6 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6); \
}
#define bli_call_ft_8( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7); \
}
#define bli_call_ft_9( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8); \
}
#define bli_call_ft_10( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8, o9 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9); \
}
#define bli_call_ft_11( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10); \
}
#define bli_call_ft_12( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11); \
}
#define bli_call_ft_13( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12); \
}
#define bli_call_ft_14( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13); \
}
#define bli_call_ft_15( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14); \
}
#define bli_call_ft_20( dt, fname, o0, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10, o11, o12, o13, o14, o15, o16, o17, o18, o19 ) \
{ \
	if      ( bli_is_float( dt )    ) PASTEMAC(s,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14,o15,o16,o17,o18,o19); \
	else if ( bli_is_double( dt )   ) PASTEMAC(d,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14,o15,o16,o17,o18,o19); \
	else if ( bli_is_scomplex( dt ) ) PASTEMAC(c,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14,o15,o16,o17,o18,o19); \
	else if ( bli_is_dcomplex( dt ) ) PASTEMAC(z,fname)(o0,o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12,o13,o14,o15,o16,o17,o18,o19); \
}



#endif
// end bli_param_macro_defs.h
// begin bli_complex_macro_defs.h


#ifndef BLIS_COMPLEX_MACRO_DEFS_H
#define BLIS_COMPLEX_MACRO_DEFS_H


// -- Real and imaginary accessor macros --


#define bli_sreal( x )  ( x )
#define bli_simag( x )  ( 0.0F )
#define bli_dreal( x )  ( x )
#define bli_dimag( x )  ( 0.0 )


#ifndef BLIS_ENABLE_C99_COMPLEX


#define bli_creal( x )  ( (x).real )
#define bli_cimag( x )  ( (x).imag )
#define bli_zreal( x )  ( (x).real )
#define bli_zimag( x )  ( (x).imag )


#else // ifdef BLIS_ENABLE_C99_COMPLEX


#define bli_creal( x )  ( crealf(x) )
#define bli_cimag( x )  ( cimagf(x) )
#define bli_zreal( x )  ( creal(x) )
#define bli_zimag( x )  ( cimag(x) )


#endif // BLIS_ENABLE_C99_COMPLEX


#endif

// end bli_complex_macro_defs.h
// begin bli_scalar_macro_defs.h


#ifndef BLIS_SCALAR_MACRO_DEFS_H
#define BLIS_SCALAR_MACRO_DEFS_H



// -- Assignment/Accessor macros --

// NOTE: This macro is defined first since some of the other scalar macros
// use it to abstract away the method used to assign complex values (ie:
// whether fields of a struct are set directly or whether native C99
// assignment is used).

// begin bli_sets.h


#ifndef BLIS_SETS_H
#define BLIS_SETS_H

// sets

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sssets( xr, xi, y )  { (y) = (xr); }
#define bli_dssets( xr, xi, y )  { (y) = (xr); }
#define bli_cssets( xr, xi, y )  { (y) = (xr); }
#define bli_zssets( xr, xi, y )  { (y) = (xr); }
#define bli_issets( xr, xi, y )  { (y) = (xr); }

#define bli_sdsets( xr, xi, y )  { (y) = (xr); }
#define bli_ddsets( xr, xi, y )  { (y) = (xr); }
#define bli_cdsets( xr, xi, y )  { (y) = (xr); }
#define bli_zdsets( xr, xi, y )  { (y) = (xr); }
#define bli_idsets( xr, xi, y )  { (y) = (xr); }

#ifndef BLIS_ENABLE_C99_COMPLEX 

#define bli_scsets( xr, xi, y ) { bli_creal(y) = (xr); bli_cimag(y) = (xi); }
#define bli_dcsets( xr, xi, y ) { bli_creal(y) = (xr); bli_cimag(y) = (xi); }
#define bli_ccsets( xr, xi, y ) { bli_creal(y) = (xr); bli_cimag(y) = (xi); }
#define bli_zcsets( xr, xi, y ) { bli_creal(y) = (xr); bli_cimag(y) = (xi); }
#define bli_icsets( xr, xi, y ) { bli_creal(y) = (xr); bli_cimag(y) = (xi); }

#define bli_szsets( xr, xi, y ) { bli_zreal(y) = (xr); bli_zimag(y) = (xi); }
#define bli_dzsets( xr, xi, y ) { bli_zreal(y) = (xr); bli_zimag(y) = (xi); }
#define bli_czsets( xr, xi, y ) { bli_zreal(y) = (xr); bli_zimag(y) = (xi); }
#define bli_zzsets( xr, xi, y ) { bli_zreal(y) = (xr); bli_zimag(y) = (xi); }
#define bli_izsets( xr, xi, y ) { bli_zreal(y) = (xr); bli_zimag(y) = (xi); }

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }
#define bli_dcsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }
#define bli_ccsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }
#define bli_zcsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }

#define bli_szsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }
#define bli_dzsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }
#define bli_czsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }
#define bli_zzsets( xr, xi, y )  { (y) = (xr) + (xi) * (I); }

#endif // BLIS_ENABLE_C99_COMPLEX

#define bli_sisets( xr, xi, y ) { (y) = bli_sreal(xr); }
#define bli_disets( xr, xi, y ) { (y) = bli_dreal(xr); }
#define bli_cisets( xr, xi, y ) { (y) = bli_creal(xr); }
#define bli_zisets( xr, xi, y ) { (y) = bli_zreal(xr); }
#define bli_iisets( xr, xi, y ) { (y) =          (xr); }


#define bli_ssets( xr, xi, y )  bli_sssets( xr, xi, y )
#define bli_dsets( xr, xi, y )  bli_ddsets( xr, xi, y )
#define bli_csets( xr, xi, y )  bli_scsets( xr, xi, y )
#define bli_zsets( xr, xi, y )  bli_dzsets( xr, xi, y )
#define bli_isets( xr, xi, y )  bli_disets( xr, xi, y )


#endif

// end bli_sets.h

// NOTE: These macros are not used by other scalar macros, but they are
// related to those defined in bli_sets.h, and so we #include them here.

// begin bli_setrs.h


#ifndef BLIS_SETRS_H
#define BLIS_SETRS_H

// setrs

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sssetrs( xr, y )  { (y) = (xr); }
#define bli_dssetrs( xr, y )  { (y) = (xr); }

#define bli_sdsetrs( xr, y )  { (y) = (xr); }
#define bli_ddsetrs( xr, y )  { (y) = (xr); }

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scsetrs( xr, y )  { bli_creal(y) = (xr); }
#define bli_dcsetrs( xr, y )  { bli_creal(y) = (xr); }

#define bli_szsetrs( xr, y )  { bli_zreal(y) = (xr); }
#define bli_dzsetrs( xr, y )  { bli_zreal(y) = (xr); }

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scsetrs( xr, y )  { (y) = (xr) + bli_cimag(y) * (I); }
#define bli_dcsetrs( xr, y )  { (y) = (xr) + bli_cimag(y) * (I); }

#define bli_szsetrs( xr, y )  { (y) = (xr) + bli_zimag(y) * (I); }
#define bli_dzsetrs( xr, y )  { (y) = (xr) + bli_zimag(y) * (I); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_ssetrs( xr, y )  bli_sssetrs( xr, y )
#define bli_dsetrs( xr, y )  bli_ddsetrs( xr, y )
#define bli_csetrs( xr, y )  bli_scsetrs( xr, y )
#define bli_zsetrs( xr, y )  bli_dzsetrs( xr, y )


#endif

// end bli_setrs.h
// begin bli_setis.h


#ifndef BLIS_SETIS_H
#define BLIS_SETIS_H

// setis

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sssetis( xi, y )  { ; }
#define bli_dssetis( xi, y )  { ; }

#define bli_sdsetis( xi, y )  { ; }
#define bli_ddsetis( xi, y )  { ; }

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scsetis( xi, y )  { bli_cimag(y) = (xi); }
#define bli_dcsetis( xi, y )  { bli_cimag(y) = (xi); }

#define bli_szsetis( xi, y )  { bli_zimag(y) = (xi); }
#define bli_dzsetis( xi, y )  { bli_zimag(y) = (xi); }

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scsetis( xi, y )  { (y) = bli_creal(y) + (xi) * (I); }
#define bli_dcsetis( xi, y )  { (y) = bli_creal(y) + (xi) * (I); }

#define bli_szsetis( xi, y )  { (y) = bli_zreal(y) + (xi) * (I); }
#define bli_dzsetis( xi, y )  { (y) = bli_zreal(y) + (xi) * (I); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_ssetis( xi, y )  bli_sssetis( xi, y )
#define bli_dsetis( xi, y )  bli_ddsetis( xi, y )
#define bli_csetis( xi, y )  bli_scsetis( xi, y )
#define bli_zsetis( xi, y )  bli_dzsetis( xi, y )


#endif

// end bli_setis.h

// NOTE: This macro also needs to be defined early on since it determines
// how real and imaginary components are accessed (ie: whether the fields
// of a struct are read directly or whether native C99 functions are used.)

// begin bli_gets.h


#ifndef BLIS_GETS_H
#define BLIS_GETS_H

// gets

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.


#define bli_ssgets( x, yr, yi )  { (yr) = bli_sreal(x); (yi) = bli_simag(x); }
#define bli_dsgets( x, yr, yi )  { (yr) = bli_dreal(x); (yi) = bli_dimag(x); }
#define bli_csgets( x, yr, yi )  { (yr) = bli_creal(x); (yi) = bli_cimag(x); }
#define bli_zsgets( x, yr, yi )  { (yr) = bli_zreal(x); (yi) = bli_zimag(x); }
#define bli_isgets( x, yr, yi )  { (yr) = ( float )(x); (yi) = 0.0F; }

#define bli_sdgets( x, yr, yi )  { (yr) = bli_sreal(x); (yi) = bli_simag(x); }
#define bli_ddgets( x, yr, yi )  { (yr) = bli_dreal(x); (yi) = bli_dimag(x); }
#define bli_cdgets( x, yr, yi )  { (yr) = bli_creal(x); (yi) = bli_cimag(x); }
#define bli_zdgets( x, yr, yi )  { (yr) = bli_zreal(x); (yi) = bli_zimag(x); }
#define bli_idgets( x, yr, yi )  { (yr) = ( double )(x); (yi) = 0.0; }

#define bli_scgets( x, yr, yi )  { (yr) = bli_sreal(x); (yi) = bli_simag(x); }
#define bli_dcgets( x, yr, yi )  { (yr) = bli_dreal(x); (yi) = bli_dimag(x); }
#define bli_ccgets( x, yr, yi )  { (yr) = bli_creal(x); (yi) = bli_cimag(x); }
#define bli_zcgets( x, yr, yi )  { (yr) = bli_zreal(x); (yi) = bli_zimag(x); }
#define bli_icgets( x, yr, yi )  { (yr) = ( float )(x); (yi) = 0.0F; }

#define bli_szgets( x, yr, yi )  { (yr) = bli_sreal(x); (yi) = bli_simag(x); }
#define bli_dzgets( x, yr, yi )  { (yr) = bli_dreal(x); (yi) = bli_dimag(x); }
#define bli_czgets( x, yr, yi )  { (yr) = bli_creal(x); (yi) = bli_cimag(x); }
#define bli_zzgets( x, yr, yi )  { (yr) = bli_zreal(x); (yi) = bli_zimag(x); }
#define bli_izgets( x, yr, yi )  { (yr) = ( double )(x); (yi) = 0.0; }

#define bli_sigets( x, yr, yi )  { (yr) = bli_sreal(x); (yi) = 0; }
#define bli_digets( x, yr, yi )  { (yr) = bli_dreal(x); (yi) = 0; }
#define bli_cigets( x, yr, yi )  { (yr) = bli_creal(x); (yi) = 0; }
#define bli_zigets( x, yr, yi )  { (yr) = bli_zreal(x); (yi) = 0; }
#define bli_iigets( x, yr, yi )  { (yr) =          (x); (yi) = 0; }


#define bli_sgets( x, yr, yi )  bli_ssgets( x, yr, yi )
#define bli_dgets( x, yr, yi )  bli_ddgets( x, yr, yi )
#define bli_cgets( x, yr, yi )  bli_csgets( x, yr, yi )
#define bli_zgets( x, yr, yi )  bli_zdgets( x, yr, yi )
#define bli_igets( x, yr, yi )  bli_idgets( x, yr, yi )


#endif
// end bli_gets.h


// -- Scalar constant initialization macros --

// begin bli_constants.h


#ifndef BLIS_CONSTANTS_H
#define BLIS_CONSTANTS_H

// return pointers to constants

// 2

#define bli_s2 \
\
	( ( float*    ) bli_obj_buffer_for_const( BLIS_FLOAT,    BLIS_TWO ) )

#define bli_d2 \
\
	( ( double*   ) bli_obj_buffer_for_const( BLIS_DOUBLE,   BLIS_TWO ) )

#define bli_c2 \
\
	( ( scomplex* ) bli_obj_buffer_for_const( BLIS_SCOMPLEX, BLIS_TWO ) )

#define bli_z2 \
\
	( ( dcomplex* ) bli_obj_buffer_for_const( BLIS_DCOMPLEX, BLIS_TWO ) )

#define bli_i2 \
\
	( ( gint_t*   ) bli_obj_buffer_for_const( BLIS_INT,      BLIS_TWO ) )

// 1

#define bli_s1 \
\
	( ( float*    ) bli_obj_buffer_for_const( BLIS_FLOAT,    BLIS_ONE ) )

#define bli_d1 \
\
	( ( double*   ) bli_obj_buffer_for_const( BLIS_DOUBLE,   BLIS_ONE ) )

#define bli_c1 \
\
	( ( scomplex* ) bli_obj_buffer_for_const( BLIS_SCOMPLEX, BLIS_ONE ) )

#define bli_z1 \
\
	( ( dcomplex* ) bli_obj_buffer_for_const( BLIS_DCOMPLEX, BLIS_ONE ) )

#define bli_i1 \
\
	( ( gint_t*   ) bli_obj_buffer_for_const( BLIS_INT,      BLIS_ONE ) )

// 0

#define bli_s0 \
\
	( ( float*    ) bli_obj_buffer_for_const( BLIS_FLOAT,    BLIS_ZERO ) )

#define bli_d0 \
\
	( ( double*   ) bli_obj_buffer_for_const( BLIS_DOUBLE,   BLIS_ZERO ) )

#define bli_c0 \
\
	( ( scomplex* ) bli_obj_buffer_for_const( BLIS_SCOMPLEX, BLIS_ZERO ) )

#define bli_z0 \
\
	( ( dcomplex* ) bli_obj_buffer_for_const( BLIS_DCOMPLEX, BLIS_ZERO ) )

#define bli_i0 \
\
	( ( gint_t*   ) bli_obj_buffer_for_const( BLIS_INT,      BLIS_ZERO ) )

// -1

#define bli_sm1 \
\
	( ( float*    ) bli_obj_buffer_for_const( BLIS_FLOAT,    BLIS_MINUS_ONE ) )

#define bli_dm1 \
\
	( ( double*   ) bli_obj_buffer_for_const( BLIS_DOUBLE,   BLIS_MINUS_ONE ) )

#define bli_cm1 \
\
	( ( scomplex* ) bli_obj_buffer_for_const( BLIS_SCOMPLEX, BLIS_MINUS_ONE ) )

#define bli_zm1 \
\
	( ( dcomplex* ) bli_obj_buffer_for_const( BLIS_DCOMPLEX, BLIS_MINUS_ONE ) )

#define bli_im1 \
\
	( ( gint_t*   ) bli_obj_buffer_for_const( BLIS_INT,      BLIS_MINUS_ONE ) )

// -2

#define bli_sm2 \
\
	( ( float*    ) bli_obj_buffer_for_const( BLIS_FLOAT,    BLIS_MINUS_TWO ) )

#define bli_dm2 \
\
	( ( double*   ) bli_obj_buffer_for_const( BLIS_DOUBLE,   BLIS_MINUS_TWO ) )

#define bli_cm2 \
\
	( ( scomplex* ) bli_obj_buffer_for_const( BLIS_SCOMPLEX, BLIS_MINUS_TWO ) )

#define bli_zm2 \
\
	( ( dcomplex* ) bli_obj_buffer_for_const( BLIS_DCOMPLEX, BLIS_MINUS_TWO ) )

#define bli_im2 \
\
	( ( gint_t*   ) bli_obj_buffer_for_const( BLIS_INT,      BLIS_MINUS_TWO ) )


#endif

// end bli_constants.h


// -- Separated scalar macros (separated real/imaginary values) --

// begin bli_absq2ris.h


#ifndef BLIS_ABSQ2RIS_H
#define BLIS_ABSQ2RIS_H

// absq2ris

#define bli_sabsq2ris( ar, ai, br, bi ) \
{ \
	(br) = (ar) * (ar); \
}

#define bli_dabsq2ris( ar, ai, br, bi ) \
{ \
	(br) = (ar) * (ar); \
}

#define bli_cabsq2ris( ar, ai, br, bi ) \
{ \
	(br) = (ar) * (ar) + (ai) * (ai); \
	(bi) = 0.0F; \
}

#define bli_zabsq2ris( ar, ai, br, bi ) \
{ \
	(br) = (ar) * (ar) + (ai) * (ai); \
	(bi) = 0.0; \
}

#endif

// end bli_absq2ris.h

// begin bli_abval2ris.h


#ifndef BLIS_ABVAL2RIS_H
#define BLIS_ABVAL2RIS_H

// abval2ris

#define bli_sabval2ris( xr, xi, ar, ai ) \
{ \
	(ar)       = fabsf(xr); \
}

#define bli_dabval2ris( xr, xi, ar, ai ) \
{ \
	(ar)       = fabs(xr); \
}

#define bli_cabval2ris( xr, xi, ar, ai ) \
{ \
	float  s   = bli_fmaxabs( (xr), (xi) ); \
	float  mag; \
	if ( s == 0.0F ) mag = 0.0F; \
	else \
	{ \
		mag = sqrtf( s ) * \
		      sqrtf( ( (xr) / s ) * (xr) + \
		             ( (xi) / s ) * (xi) ); \
	} \
	(ar)       = mag; \
	(ai)       = 0.0F; \
}

#define bli_zabval2ris( xr, xi, ar, ai ) \
{ \
	double s   = bli_fmaxabs( (xr), (xi) ); \
	double mag; \
	if ( s == 0.0 ) mag = 0.0; \
	else \
	{ \
		mag = sqrt( s ) * \
		      sqrt( ( (xr) / s ) * (xr) + \
		            ( (xi) / s ) * (xi) ); \
	} \
	(ar)       = mag; \
	(ai)       = 0.0; \
}

#endif
// end bli_abval2ris.h

// begin bli_addris.h


#ifndef BLIS_ADDRIS_H
#define BLIS_ADDRIS_H

// addris

#define bli_saddris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) + (ar); \
}

#define bli_daddris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) + (ar); \
}

#define bli_caddris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) + (ar); \
	(xi) = (xi) + (ai); \
}

#define bli_zaddris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) + (ar); \
	(xi) = (xi) + (ai); \
}

#endif

// end bli_addris.h
// begin bli_addjris.h


#ifndef BLIS_ADDJRIS_H
#define BLIS_ADDJRIS_H

// addjris

#define bli_saddjris( ar, ai, xr, xi )  bli_saddris( (ar), -(ai), (xr), (xi) )
#define bli_daddjris( ar, ai, xr, xi )  bli_daddris( (ar), -(ai), (xr), (xi) )
#define bli_caddjris( ar, ai, xr, xi )  bli_caddris( (ar), -(ai), (xr), (xi) )
#define bli_zaddjris( ar, ai, xr, xi )  bli_zaddris( (ar), -(ai), (xr), (xi) )

#endif

// end bli_addjris.h

// begin bli_add3ris.h


#ifndef BLIS_ADD3RIS_H
#define BLIS_ADD3RIS_H

// add3ris

#define bli_sadd3ris( ar, ai, br, bi, cr, ci ) \
{ \
	(cr) = (ar) + (br); \
}

#define bli_dadd3ris( ar, ai, br, bi, cr, ci ) \
{ \
	(cr) = (ar) + (br); \
}

#define bli_cadd3ris( ar, ai, br, bi, cr, ci ) \
{ \
	(cr) = (ar) + (br); \
	(ci) = (ai) + (bi); \
}

#define bli_zadd3ris( ar, ai, br, bi, cr, ci ) \
{ \
	(cr) = (ar) + (br); \
	(ci) = (ai) + (bi); \
}

#endif

// end bli_add3ris.h

// begin bli_axpbyris.h


#ifndef BLIS_AXPBYRIS_H
#define BLIS_AXPBYRIS_H

// axpbyris

#define bli_saxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (ar) * (xr)               + (br) * (yr); \
}

#define bli_daxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (ar) * (xr)               + (br) * (yr); \
}

#define bli_caxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) - (ai) * (xi) + (br) * (yr) - (bi) * (yi); \
    float  yt_i = (ai) * (xr) + (ar) * (xi) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_sccaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    float  yt_i = (ar) * (xi) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_ccsaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) - (ai) * (xi) + (br) * (yr); \
    float  yt_i = (ai) * (xr) + (ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_cscaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    float  yt_i = (ai) * (xr) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_sscaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    float  yt_i =               (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_cssaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr); \
    float  yt_i = (ai) * (xr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_scsaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr); \
    float  yt_i = (ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
	double yt_r = (ar) * (xr) - (ai) * (xi) + (br) * (yr) - (bi) * (yi); \
	double yt_i = (ai) * (xr) + (ar) * (xi) + (bi) * (yr) + (br) * (yi); \
	(yr) = yt_r; \
	(yi) = yt_i; \
}

#define bli_dzzaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    double yt_i = (ar) * (xi) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zzdaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) - (ai) * (xi) + (br) * (yr); \
    double yt_i = (ai) * (xr) + (ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zdzaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    double yt_i = (ai) * (xr) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_ddzaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    double yt_i =               (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zddaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr); \
    double yt_i = (ai) * (xr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_dzdaxpbyris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr); \
    double yt_i = (ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#endif

// end bli_axpbyris.h
// begin bli_axpbyjris.h


#ifndef BLIS_AXPBYJRIS_H
#define BLIS_AXPBYJRIS_H

// axpbyjris

#define bli_saxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (ar) * (xr)               + (br) * (yr); \
}

#define bli_daxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (ar) * (xr)               + (br) * (yr); \
}

#define bli_caxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (ai) * (xi) + (br) * (yr) - (bi) * (yi); \
    float  yt_i = (ai) * (xr) - (ar) * (xi) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_sccaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r =  (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    float  yt_i = -(ar) * (xi) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_ccsaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (ai) * (xi) + (br) * (yr); \
    float  yt_i = (ai) * (xr) - (ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_cscaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    float  yt_i = (ai) * (xr) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_sscaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    float  yt_i =               (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_cssaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r = (ar) * (xr) + (br) * (yr); \
    float  yt_i = (ai) * (xr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_scsaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    float  yt_r =  (ar) * (xr) + (br) * (yr); \
    float  yt_i = -(ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
	double yt_r = (ar) * (xr) + (ai) * (xi) + (br) * (yr) - (bi) * (yi); \
	double yt_i = (ai) * (xr) - (ar) * (xi) + (bi) * (yr) + (br) * (yi); \
	(yr) = yt_r; \
	(yi) = yt_i; \
}

#define bli_dzzaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r =  (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    double yt_i = -(ar) * (xi) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zzdaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (ai) * (xi) + (br) * (yr); \
    double yt_i = (ai) * (xr) - (ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zdzaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    double yt_i = (ai) * (xr) + (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_ddzaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr) - (bi) * (yi); \
    double yt_i =               (bi) * (yr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_zddaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r = (ar) * (xr) + (br) * (yr); \
    double yt_i = (ai) * (xr) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#define bli_dzdaxpbyjris( ar, ai, xr, xi, br, bi, yr, yi ) \
{ \
    double yt_r =  (ar) * (xr) + (br) * (yr); \
    double yt_i = -(ar) * (xi) + (br) * (yi); \
    (yr) = yt_r; \
    (yi) = yt_i; \
}

#endif

// end bli_axpbyjris.h

// begin bli_axpyris.h


#ifndef BLIS_AXPYRIS_H
#define BLIS_AXPYRIS_H

// axpyris

#define bli_saxpyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr); \
}

#define bli_daxpyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr); \
}

#define bli_caxpyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr) - (ai) * (xi); \
	(yi) += (ai) * (xr) + (ar) * (xi); \
}

#define bli_zaxpyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr) - (ai) * (xi); \
	(yi) += (ai) * (xr) + (ar) * (xi); \
}

#define bli_scaxpyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr); \
	(yi) += (ar) * (xi); \
}

#define bli_dzaxpyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr); \
	(yi) += (ar) * (xi); \
}

#endif

// end bli_axpyris.h
// begin bli_axpyjris.h


#ifndef BLIS_AXPYJRIS_H
#define BLIS_AXPYJRIS_H

// axpyjris

#define bli_saxpyjris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr); \
}

#define bli_daxpyjris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr); \
}

#define bli_caxpyjris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr) + (ai) * (xi); \
	(yi) += (ai) * (xr) - (ar) * (xi); \
}

#define bli_zaxpyjris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) * (xr) + (ai) * (xi); \
	(yi) += (ai) * (xr) - (ar) * (xi); \
}

#define bli_scaxpyjris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) *  (xr); \
	(yi) += (ar) * -(xi); \
}

#define bli_dzaxpyjris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) += (ar) *  (xr); \
	(yi) += (ar) * -(xi); \
}

#endif

// end bli_axpyjris.h

// begin bli_axmyris.h


#ifndef BLIS_AXMYRIS_H
#define BLIS_AXMYRIS_H

// axmyris

#define bli_saxmyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) -= (ar) * (xr); \
}

#define bli_daxmyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) -= (ar) * (xr); \
}

#define bli_caxmyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) -= (ar) * (xr) - (ai) * (xi); \
	(yi) -= (ai) * (xr) + (ar) * (xi); \
}

#define bli_zaxmyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) -= (ar) * (xr) - (ai) * (xi); \
	(yi) -= (ai) * (xr) + (ar) * (xi); \
}

#define bli_scaxmyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) -= (ar) * (xr); \
	(yi) -= (ar) * (xi); \
}

#define bli_dzaxmyris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) -= (ar) * (xr); \
	(yi) -= (ar) * (xi); \
}

#endif

// end bli_axmyris.h

// begin bli_conjris.h


#ifndef BLIS_CONJRIS_H
#define BLIS_CONJRIS_H

// conjris

#define bli_sconjris( xr, xi ) \
{ \
	; \
}

#define bli_dconjris( xr, xi ) \
{ \
	; \
}

#define bli_cconjris( xr, xi ) \
{ \
	(xi) = -(xi); \
}

#define bli_zconjris( xr, xi ) \
{ \
	(xi) = -(xi); \
}

#endif

// end bli_conjris.h

// begin bli_copyris.h


#ifndef BLIS_COPYRIS_H
#define BLIS_COPYRIS_H

// copyris

#define bli_scopyris( ar, ai, br, bi ) \
{ \
	(br) = (ar); \
}

#define bli_dcopyris( ar, ai, br, bi ) \
{ \
	(br) = (ar); \
}

#define bli_ccopyris( ar, ai, br, bi ) \
{ \
	(br) = (ar); \
	(bi) = (ai); \
}

#define bli_zcopyris( ar, ai, br, bi ) \
{ \
	(br) = (ar); \
	(bi) = (ai); \
}

#endif

// end bli_copyris.h
// begin bli_copyjris.h


#ifndef BLIS_COPYJRIS_H
#define BLIS_COPYJRIS_H

// copyjris

#define bli_scopyjris( ar, ai, br, bi )  bli_scopyris( (ar), -(ai), (br), (bi) )
#define bli_dcopyjris( ar, ai, br, bi )  bli_dcopyris( (ar), -(ai), (br), (bi) )
#define bli_ccopyjris( ar, ai, br, bi )  bli_ccopyris( (ar), -(ai), (br), (bi) )
#define bli_zcopyjris( ar, ai, br, bi )  bli_zcopyris( (ar), -(ai), (br), (bi) )

#endif

// end bli_copyjris.h
// begin bli_copycjris.h


#ifndef BLIS_COPYCJRIS_H
#define BLIS_COPYCJRIS_H

// copycjris

#define bli_scopycjris( conj, xr, xi, yr, yi ) \
{ \
	bli_scopyris( (xr), (xi), (yr), (yi) ); \
}

#define bli_dcopycjris( conj, xr, xi, yr, yi ) \
{ \
	bli_dcopyris( (xr), (xi), (yr), (yi) ); \
}

#define bli_ccopycjris( conj, xr, xi, yr, yi ) \
{ \
	(yr) =                          (xr); \
	(yi) = ( bli_is_conj( conj ) ? -(xi) \
	                             :  (xi) ); \
}

#define bli_zcopycjris( conj, xr, xi, yr, yi ) \
{ \
	(yr) =                          (xr); \
	(yi) = ( bli_is_conj( conj ) ? -(xi) \
	                             :  (xi) ); \
}

#define bli_icopycjris( conj, xr, xi, yr, yi ) \
{ \
	bli_icopyris( (xr), (xi), (yr), (yi) ); \
}

#endif
// end bli_copycjris.h

// begin bli_eqris.h


#ifndef BLIS_EQRIS_H
#define BLIS_EQRIS_H


// eqris (passed by value)

#define bli_seqris( ar, ai, br, bi )  ( (ar) == (br) )
#define bli_deqris( ar, ai, br, bi )  ( (ar) == (br) )
#define bli_ceqris( ar, ai, br, bi )  ( (ar) == (br) && (ai) == (bi) )
#define bli_zeqris( ar, ai, br, bi )  ( (ar) == (br) && (ai) == (bi) )
#define bli_ieqris( ar, ai, br, bi )  ( (ar) == (br) )


// eq1ris

#define bli_seq1ris( ar, ai )  bli_seqris( (ar), (ai), 1.0F, 0.0F )
#define bli_deq1ris( ar, ai )  bli_deqris( (ar), (ai), 1.0,  0.0  )
#define bli_ceq1ris( ar, ai )  bli_ceqris( (ar), (ai), 1.0F, 0.0F )
#define bli_zeq1ris( ar, ai )  bli_zeqris( (ar), (ai), 1.0,  0.0  )
#define bli_ieq1ris( ar, ai )  bli_ieqris( (ar), (ai), 1,    0    )


// eq0ris

#define bli_seq0ris( ar, ai )  bli_seqris( (ar), (ai), 0.0F, 0.0F )
#define bli_deq0ris( ar, ai )  bli_deqris( (ar), (ai), 0.0,  0.0  )
#define bli_ceq0ris( ar, ai )  bli_ceqris( (ar), (ai), 0.0F, 0.0F )
#define bli_zeq0ris( ar, ai )  bli_zeqris( (ar), (ai), 0.0,  0.0  )
#define bli_ieq0ris( ar, ai )  bli_ieqris( (ar), (ai), 0,    0    )


// eqm1ris

#define bli_seqm1ris( ar, ai )  bli_seqris( (ar), (ai), -1.0F, 0.0F )
#define bli_deqm1ris( ar, ai )  bli_deqris( (ar), (ai), -1.0,  0.0  )
#define bli_ceqm1ris( ar, ai )  bli_ceqris( (ar), (ai), -1.0F, 0.0F )
#define bli_zeqm1ris( ar, ai )  bli_zeqris( (ar), (ai), -1.0,  0.0  )
#define bli_ieqm1ris( ar, ai )  bli_ieqris( (ar), (ai), -1,    0    )



#endif
// end bli_eqris.h

// begin bli_invertris.h


#ifndef BLIS_INVERTRIS_H
#define BLIS_INVERTRIS_H

// invertris

#define bli_sinvertris( xr, xi ) \
{ \
	(xr) = 1.0F / (xr); \
}

#define bli_dinvertris( xr, xi ) \
{ \
	(xr) = 1.0  / (xr); \
}

#define bli_cinvertris( xr, xi ) \
{ \
	float  s    = bli_fmaxabs( (xr), (xi) ); \
	float  xr_s = (xr) / s; \
	float  xi_s = (xi) / s; \
	float  temp = ( xr_s * (xr) + xi_s * (xi) ); \
	(xr)        =  xr_s / temp; \
	(xi)        = -xi_s / temp; \
}

#define bli_zinvertris( xr, xi ) \
{ \
	double s    = bli_fmaxabs( (xr), (xi) ); \
	double xr_s = (xr) / s; \
	double xi_s = (xi) / s; \
	double temp = ( xr_s * (xr) + xi_s * (xi) ); \
	(xr)        =  xr_s / temp; \
	(xi)        = -xi_s / temp; \
}

#endif
// end bli_invertris.h

// begin bli_invscalris.h


#ifndef BLIS_INVSCALRIS_H
#define BLIS_INVSCALRIS_H

// invscalris

#define bli_sinvscalris( ar, ai, xr, xi ) \
{ \
	(xr) /= (ar); \
}

#define bli_dinvscalris( ar, ai, xr, xi ) \
{ \
	(xr) /= (ar); \
}

#define bli_cinvscalris( ar, ai, xr, xi ) \
{ \
	float  s     = bli_fmaxabs( (ar), (ai) ); \
	float  ar_s  = (ar) / s; \
	float  ai_s  = (ai) / s; \
	float  xrt   = (xr); \
	float  temp  = ( ar_s * (ar) + ai_s * (ai) ); \
	(xr)         = ( (xrt) * ar_s + (xi)  * ai_s ) / temp; \
	(xi)         = ( (xi)  * ar_s - (xrt) * ai_s ) / temp; \
}

#define bli_zinvscalris( ar, ai, xr, xi ) \
{ \
	double s     = bli_fmaxabs( (ar), (ai) ); \
	double ar_s  = (ar) / s; \
	double ai_s  = (ai) / s; \
	double xrt   = (xr); \
	double temp  = ( ar_s * (ar) + ai_s * (ai) ); \
	(xr)         = ( (xrt) * ar_s + (xi)  * ai_s ) / temp; \
	(xi)         = ( (xi)  * ar_s - (xrt) * ai_s ) / temp; \
}

#define bli_scinvscalris( ar, ai, xr, xi ) \
{ \
	(xr) /= (ar); \
	(xi) /= (ar); \
}

#define bli_dzinvscalris( ar, ai, xr, xi ) \
{ \
	(xr) /= (ar); \
	(xi) /= (ar); \
}

#endif
// end bli_invscalris.h
// begin bli_invscaljris.h


#ifndef BLIS_INVSCALJRIS_H
#define BLIS_INVSCALJRIS_H

// invscaljris

#define bli_sinvscaljris( ar, ai, xr, xi )  bli_sinvscalris( (ar), -(ai), (xr), (xi) )
#define bli_dinvscaljris( ar, ai, xr, xi )  bli_dinvscalris( (ar), -(ai), (xr), (xi) )
#define bli_cinvscaljris( ar, ai, xr, xi )  bli_cinvscalris( (ar), -(ai), (xr), (xi) )
#define bli_zinvscaljris( ar, ai, xr, xi )  bli_zinvscalris( (ar), -(ai), (xr), (xi) )

#define bli_scinvscaljris( ar, ai, xr, xi )  bli_scinvscalris( (ar), -(ai), (xr), (xi) )
#define bli_dzinvscaljris( ar, ai, xr, xi )  bli_dzinvscalris( (ar), -(ai), (xr), (xi) )

#endif

// end bli_invscaljris.h

// begin bli_neg2ris.h


#ifndef BLIS_NEG2RIS_H
#define BLIS_NEG2RIS_H

// neg2ris

#define bli_sneg2ris( ar, ai, br, bi ) \
{ \
	(br) = -(ar); \
}

#define bli_dneg2ris( ar, ai, br, bi ) \
{ \
	(br) = -(ar); \
}

#define bli_cneg2ris( ar, ai, br, bi ) \
{ \
	(br) = -(ar); \
	(bi) = -(ai); \
}

#define bli_zneg2ris( ar, ai, br, bi ) \
{ \
	(br) = -(ar); \
	(bi) = -(ai); \
}

#endif

// end bli_neg2ris.h

// begin bli_scalris.h


#ifndef BLIS_SCALRIS_H
#define BLIS_SCALRIS_H

// scalris

#define bli_sscalris( ar, ai, xr, xi ) \
{ \
	(xr)      = (ar) * (xr); \
}

#define bli_dscalris( ar, ai, xr, xi ) \
{ \
	(xr)      = (ar) * (xr); \
}

#define bli_cscalris( ar, ai, xr, xi ) \
{ \
	float  yr = (ar) * (xr) - (ai) * (xi); \
	float  yi = (ai) * (xr) + (ar) * (xi); \
	(xr) = yr; \
	(xi) = yi; \
}

#define bli_zscalris( ar, ai, xr, xi ) \
{ \
	double yr = (ar) * (xr) - (ai) * (xi); \
	double yi = (ai) * (xr) + (ar) * (xi); \
	(xr) = yr; \
	(xi) = yi; \
}

#define bli_scscalris( ar, ai, xr, xi ) \
{ \
	(xr)      = (ar) * (xr); \
	(xi)      = (ar) * (xi); \
}

#define bli_dzscalris( ar, ai, xr, xi ) \
{ \
	(xr)      = (ar) * (xr); \
	(xi)      = (ar) * (xi); \
}

#endif

// end bli_scalris.h
// begin bli_scaljris.h


#ifndef BLIS_SCALJRIS_H
#define BLIS_SCALJRIS_H

// scaljris

#define bli_sscaljris( ar, ai, xr, xi )  bli_sscalris( (ar), -(ai), (xr), (xi) )
#define bli_dscaljris( ar, ai, xr, xi )  bli_dscalris( (ar), -(ai), (xr), (xi) )
#define bli_cscaljris( ar, ai, xr, xi )  bli_cscalris( (ar), -(ai), (xr), (xi) )
#define bli_zscaljris( ar, ai, xr, xi )  bli_zscalris( (ar), -(ai), (xr), (xi) )

#define bli_scscaljris( ar, ai, xr, xi )  bli_scscalris( (ar), -(ai), (xr), (xi) )
#define bli_dzscaljris( ar, ai, xr, xi )  bli_dzscalris( (ar), -(ai), (xr), (xi) )

#endif

// end bli_scaljris.h
// begin bli_scalcjris.h


#ifndef BLIS_SCALCJRIS_H
#define BLIS_SCALCJRIS_H

// scalcjris

#define bli_sscalcjris( conj, ar, ai, xr, xi ) \
{ \
	bli_sscalris( (ar), (ai), (xr), (xi) ); \
}

#define bli_dscalcjris( conj, ar, ai, xr, xi ) \
{ \
	bli_dscalris( (ar), (ai), (xr), (xi) ); \
}

#define bli_cscalcjris( conj, ar, ai, xr, xi ) \
{ \
	if ( bli_is_conj( conj ) ) { bli_cscaljris( (ar), (ai), (xr), (xi) ); } \
	else                       { bli_cscalris(  (ar), (ai), (xr), (xi) ); } \
}

#define bli_zscalcjris( conj, ar, ai, xr, xi ) \
{ \
	if ( bli_is_conj( conj ) ) { bli_zscaljris( (ar), (ai), (xr), (xi) ); } \
	else                       { bli_zscalris(  (ar), (ai), (xr), (xi) ); } \
}

#define bli_iscalcjris( conj, ar, ai, xr, xi ) \
{ \
	bli_iscalris( (ar), (xi), (xr), (xi) ); \
}

#define bli_scscalcjris( conj, ar, ai, xr, xi ) \
{ \
	bli_scscalris( (ar), (ai), (xr), (xi) ); \
}

#define bli_dzscalcjris( conj, ar, ai, xr, xi ) \
{ \
	bli_dzscalris( (ar), (ai), (xr), (xi) ); \
}

#endif
// end bli_scalcjris.h

// begin bli_scal2ris.h


#ifndef BLIS_SCAL2RIS_H
#define BLIS_SCAL2RIS_H

// scal2ris

#define bli_sscal2ris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_dscal2ris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_cscal2ris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr) - (ai) * (xi); \
	(yi) = (ai) * (xr) + (ar) * (xi); \
}

#define bli_zscal2ris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr) - (ai) * (xi); \
	(yi) = (ai) * (xr) + (ar) * (xi); \
}

#define bli_scscal2ris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr); \
	(yi) = (ar) * (xi); \
}

#define bli_dzscal2ris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr); \
	(yi) = (ar) * (xi); \
}

#endif

// end bli_scal2ris.h
// begin bli_scal2jris.h


#ifndef BLIS_SCAL2JRIS_H
#define BLIS_SCAL2JRIS_H

// scal2jris

#define bli_sscal2jris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_dscal2jris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_cscal2jris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr) + (ai) * (xi); \
	(yi) = (ai) * (xr) - (ar) * (xi); \
}

#define bli_zscal2jris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) * (xr) + (ai) * (xi); \
	(yi) = (ai) * (xr) - (ar) * (xi); \
}

#define bli_scscal2jris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) *  (xr); \
	(yi) = (ar) * -(xi); \
}

#define bli_dzscal2jris( ar, ai, xr, xi, yr, yi ) \
{ \
	(yr) = (ar) *  (xr); \
	(yi) = (ar) * -(xi); \
}

#endif

// end bli_scal2jris.h

// begin bli_set0ris.h


#ifndef BLIS_SET0RIS_H
#define BLIS_SET0RIS_H

// set0ris

#define bli_sset0ris( xr, xi ) bli_scopyris( 0.0F, 0.0F, xr, xi )
#define bli_dset0ris( xr, xi ) bli_dcopyris( 0.0 , 0.0 , xr, xi )
#define bli_cset0ris( xr, xi ) bli_ccopyris( 0.0F, 0.0F, xr, xi )
#define bli_zset0ris( xr, xi ) bli_zcopyris( 0.0 , 0.0 , xr, xi )

#endif

// end bli_set0ris.h

// begin bli_sqrt2ris.h


#ifndef BLIS_SQRT2RIS_H
#define BLIS_SQRT2RIS_H

// sqrt2ris

#define bli_ssqrt2ris( xr, xi, ar, ai ) \
{ \
	(ar)       = sqrtf( (xr) ); \
}

#define bli_dsqrt2ris( xr, xi, ar, ai ) \
{ \
	(ar)       = sqrt( (xr) ); \
}

#define bli_csqrt2ris( xr, xi, ar, ai ) \
{ \
	float  s   = bli_fmaxabs( (xr), (xi) ); \
	float  mag; \
	if ( s == 0.0F ) mag = 0.0F; \
	else \
	{ \
		mag = sqrtf( s ) * \
		      sqrtf( ( (xr) / s ) * (xr) + \
		             ( (xi) / s ) * (xi) ); \
	} \
\
	(ar)       = sqrtf( ( mag + (xr) ) / 2.0F ); \
	(ai)       = sqrtf( ( mag - (xi) ) / 2.0F ); \
}

#define bli_zsqrt2ris( xr, xi, ar, ai ) \
{ \
	double s   = bli_fmaxabs( (xr), (xi) ); \
	double mag; \
	if ( s == 0.0 ) mag = 0.0; \
	else \
	{ \
		mag = sqrt( s ) * \
		      sqrt( ( (xr) / s ) * (xr) + \
		            ( (xi) / s ) * (xi) ); \
	} \
\
	(ar)       = sqrt( ( mag + (xr) ) / 2.0 ); \
	(ai)       = sqrt( ( mag - (xi) ) / 2.0 ); \
}

#define bli_scsqrt2ris( xr, xi, ar, ai ) \
{ \
	(ar)       = sqrtf( (xr) ); \
	(ai)       = 0.0F; \
}

#define bli_dzsqrt2ris( xr, xi, ar, ai ) \
{ \
	(ar)       = sqrt( (xr) ); \
	(ai)       = 0.0; \
}

#endif

// end bli_sqrt2ris.h

// begin bli_subris.h


#ifndef BLIS_SUBRIS_H
#define BLIS_SUBRIS_H

// subris

#define bli_ssubris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) - (ar); \
}

#define bli_dsubris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) - (ar); \
}

#define bli_csubris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) - (ar); \
	(xi) = (xi) - (ai); \
}

#define bli_zsubris( ar, ai, xr, xi ) \
{ \
	(xr) = (xr) - (ar); \
	(xi) = (xi) - (ai); \
}

#endif

// end bli_subris.h
// begin bli_subjris.h


#ifndef BLIS_SUBJRIS_H
#define BLIS_SUBJRIS_H

// subjris

#define bli_ssubjris( ar, ai, xr, xi )  bli_ssubris( (ar), -(ai), (xr), (xi) )
#define bli_dsubjris( ar, ai, xr, xi )  bli_dsubris( (ar), -(ai), (xr), (xi) )
#define bli_csubjris( ar, ai, xr, xi )  bli_csubris( (ar), -(ai), (xr), (xi) )
#define bli_zsubjris( ar, ai, xr, xi )  bli_zsubris( (ar), -(ai), (xr), (xi) )

#endif

// end bli_subjris.h

// begin bli_swapris.h


#ifndef BLIS_SWAPRIS_H
#define BLIS_SWAPRIS_H

// swapris

#define bli_sswapris( ar, ai, br, bi ) \
{ \
	float tr, ti; \
\
	bli_scopyris( (br), (bi), (tr), (ti) ); \
	bli_scopyris( (ar), (ai), (br), (bi) ); \
	bli_scopyris( (tr), (ti), (ar), (ai) ); \
}

#define bli_dswapris( ar, ai, br, bi ) \
{ \
	double tr, ti; \
\
	bli_dcopyris( (br), (bi), (tr), (ti) ); \
	bli_dcopyris( (ar), (ai), (br), (bi) ); \
	bli_dcopyris( (tr), (ti), (ar), (ai) ); \
}

#define bli_cswapris( ar, ai, br, bi ) \
{ \
	scomplex tr, ti; \
\
	bli_ccopyris( (br), (bi), (tr), (ti) ); \
	bli_ccopyris( (ar), (ai), (br), (bi) ); \
	bli_ccopyris( (tr), (ti), (ar), (ai) ); \
}

#define bli_zswapris( ar, ai, br, bi ) \
{ \
	dcomplex tr, ti; \
\
	bli_zcopyris( (br), (bi), (tr), (ti) ); \
	bli_zcopyris( (ar), (ai), (br), (bi) ); \
	bli_zcopyris( (tr), (ti), (ar), (ai) ); \
}

#endif

// end bli_swapris.h

// begin bli_xpbyris.h


#ifndef BLIS_XPBYRIS_H
#define BLIS_XPBYRIS_H

// xpbyris

#define bli_sxpbyris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (xr) + (br) * (yr); \
}

#define bli_dxpbyris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (xr) + (br) * (yr); \
}

#define bli_cxpbyris( xr, xi, br, bi, yr, yi ) \
{ \
	float  yt_r = (xr) + (br) * (yr) - (bi) * (yi); \
	float  yt_i = (xi) + (bi) * (yr) + (br) * (yi); \
	(yr) = yt_r; \
	(yi) = yt_i; \
}

#define bli_zxpbyris( xr, xi, br, bi, yr, yi ) \
{ \
	double yt_r = (xr) + (br) * (yr) - (bi) * (yi); \
	double yt_i = (xi) + (bi) * (yr) + (br) * (yi); \
	(yr) = yt_r; \
	(yi) = yt_i; \
}

#define bli_scxpbyris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (xr) + (br) * (yr); \
	(yi)        = (xi) + (br) * (yi); \
}

#define bli_dzxpbyris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (xr) + (br) * (yr); \
	(yi)        = (xi) + (br) * (yi); \
}

#endif

// end bli_xpbyris.h
// begin bli_xpbyjris.h


#ifndef BLIS_XPBYJRIS_H
#define BLIS_XPBYJRIS_H

// xpbyjris

#define bli_sxpbyjris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (xr) + (br) * (yr); \
}

#define bli_dxpbyjris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        = (xr) + (br) * (yr); \
}

#define bli_cxpbyjris( xr, xi, br, bi, yr, yi ) \
{ \
	float  yt_r =  (xr) + (br) * (yr) - (bi) * (yi); \
	float  yt_i = -(xi) + (bi) * (yr) + (br) * (yi); \
	(yr) = yt_r; \
	(yi) = yt_i; \
}

#define bli_zxpbyjris( xr, xi, br, bi, yr, yi ) \
{ \
	double yt_r =  (xr) + (br) * (yr) - (bi) * (yi); \
	double yt_i = -(xi) + (bi) * (yr) + (br) * (yi); \
	(yr) = yt_r; \
	(yi) = yt_i; \
}

#define bli_scxpbyjris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        =  (xr) + (br) * (yr); \
	(yi)        = -(xi) + (br) * (yi); \
}

#define bli_dzxpbyjris( xr, xi, br, bi, yr, yi ) \
{ \
	(yr)        =  (xr) + (br) * (yr); \
	(yi)        = -(xi) + (br) * (yi); \
}

#endif

// end bli_xpbyjris.h

// Inlined scalar macros in loops
// begin bli_scalris_mxn_uplo.h


#ifndef BLIS_SCALRIS_MXN_UPLO_H
#define BLIS_SCALRIS_MXN_UPLO_H

// scalris_mxn_u

#define bli_cscalris_mxn_u( diagoff, m, n, ar, ai, xr, xi, rs_x, cs_x ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	{ \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_cscalris( *(ar), \
			              *(ai), \
			              *((xr) + _i*rs_x + _j*cs_x), \
			              *((xi) + _i*rs_x + _j*cs_x) ); \
		} \
	} \
}

#define bli_zscalris_mxn_u( diagoff, m, n, ar, ai, xr, xi, rs_x, cs_x ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	{ \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_zscalris( *(ar), \
			              *(ai), \
			              *((xr) + _i*rs_x + _j*cs_x), \
			              *((xi) + _i*rs_x + _j*cs_x) ); \
		} \
	} \
}

// scalris_mxn_l

#define bli_cscalris_mxn_l( diagoff, m, n, ar, ai, xr, xi, rs_x, cs_x ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	{ \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_cscalris( *(ar), \
			              *(ai), \
			              *((xr) + _i*rs_x + _j*cs_x), \
			              *((xi) + _i*rs_x + _j*cs_x) ); \
		} \
	} \
}

#define bli_zscalris_mxn_l( diagoff, m, n, ar, ai, xr, xi, rs_x, cs_x ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	{ \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_zscalris( *(ar), \
			              *(ai), \
			              *((xr) + _i*rs_x + _j*cs_x), \
			              *((xi) + _i*rs_x + _j*cs_x) ); \
		} \
	} \
}

#endif
// end bli_scalris_mxn_uplo.h


// -- Conventional scalar macros (paired real/imaginary values) --

// begin bli_absq2s.h


#ifndef BLIS_ABSQR2_H
#define BLIS_ABSQR2_H

// absq2s

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of a.

#define bli_ssabsq2s( x, a )              bli_sabsq2ris( bli_sreal(x), bli_simag(x), bli_sreal(a), 0.0F         )
#define bli_dsabsq2s( x, a )              bli_dabsq2ris( bli_dreal(x), bli_dimag(x), bli_sreal(a), 0.0F         )
#define bli_csabsq2s( x, a ) { float ti;  bli_cabsq2ris( bli_creal(x), bli_cimag(x), bli_sreal(a), ti           ); ( void )ti; }
#define bli_zsabsq2s( x, a ) { float ti;  bli_zabsq2ris( bli_zreal(x), bli_zimag(x), bli_sreal(a), ti           ); ( void )ti; }

#define bli_sdabsq2s( x, a )              bli_sabsq2ris( bli_sreal(x), bli_simag(x), bli_dreal(a), 0.0          )
#define bli_ddabsq2s( x, a )              bli_dabsq2ris( bli_dreal(x), bli_dimag(x), bli_dreal(a), 0.0          )
#define bli_cdabsq2s( x, a ) { double ti; bli_cabsq2ris( bli_creal(x), bli_cimag(x), bli_dreal(a), ti           ); ( void )ti; }
#define bli_zdabsq2s( x, a ) { double ti; bli_zabsq2ris( bli_zreal(x), bli_zimag(x), bli_dreal(a), ti           ); ( void )ti; }

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scabsq2s( x, a )              bli_sabsq2ris( bli_sreal(x), bli_simag(x), bli_creal(a), bli_cimag(a) )
#define bli_dcabsq2s( x, a )              bli_dabsq2ris( bli_dreal(x), bli_dimag(x), bli_creal(a), bli_cimag(a) )
#define bli_ccabsq2s( x, a )              bli_cabsq2ris( bli_creal(x), bli_cimag(x), bli_creal(a), bli_cimag(a) )
#define bli_zcabsq2s( x, a )              bli_zabsq2ris( bli_zreal(x), bli_zimag(x), bli_creal(a), bli_cimag(a) )

#define bli_szabsq2s( x, a )              bli_sabsq2ris( bli_sreal(x), bli_simag(x), bli_zreal(a), bli_zimag(a) )
#define bli_dzabsq2s( x, a )              bli_dabsq2ris( bli_dreal(x), bli_dimag(x), bli_zreal(a), bli_zimag(a) )
#define bli_czabsq2s( x, a )              bli_cabsq2ris( bli_creal(x), bli_cimag(x), bli_zreal(a), bli_zimag(a) )
#define bli_zzabsq2s( x, a )              bli_zabsq2ris( bli_zreal(x), bli_zimag(x), bli_zreal(a), bli_zimag(a) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scabsq2s( x, a )  bli_scsets(          (x) *          (x), 0.0, (a) )
#define bli_dcabsq2s( x, a )  bli_dcsets(          (x) *          (x), 0.0, (a) )
#define bli_ccabsq2s( x, a )  bli_ccsets( bli_creal(x) * bli_creal(x) + \
                                          bli_cimag(x) * bli_cimag(x), 0.0, (a) )
#define bli_zcabsq2s( x, a )  bli_zcsets( bli_zreal(x) * bli_zreal(x) + \
                                          bli_zimag(x) * bli_zimag(x), 0.0, (a) )

#define bli_szabsq2s( x, a )  bli_szsets(          (x) *          (x), 0.0, (a) )
#define bli_dzabsq2s( x, a )  bli_dzsets(          (x) *          (x), 0.0, (a) )
#define bli_czabsq2s( x, a )  bli_czsets( bli_creal(x) * bli_creal(x) + \
                                          bli_cimag(x) * bli_cimag(x), 0.0, (a) )
#define bli_zzabsq2s( x, a )  bli_zzsets( bli_zreal(x) * bli_zreal(x) + \
                                          bli_zimag(x) * bli_zimag(x), 0.0, (a) )

#endif // BLIS_ENABLE_C99_COMPLEX

#define bli_sabsq2s( x, a )  bli_ssabsq2s( x, a )
#define bli_dabsq2s( x, a )  bli_ddabsq2s( x, a )
#define bli_cabsq2s( x, a )  bli_ccabsq2s( x, a )
#define bli_zabsq2s( x, a )  bli_zzabsq2s( x, a )


#endif
// end bli_absq2s.h

// begin bli_abval2s.h


#ifndef BLIS_ABVAL2S_H
#define BLIS_ABVAL2S_H

// abval2s

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of a.

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_ssabval2s( x, a )              bli_sabval2ris( bli_sreal(x), bli_simag(x), bli_sreal(a), 0.0F         )
#define bli_dsabval2s( x, a )              bli_dabval2ris( bli_dreal(x), bli_dimag(x), bli_sreal(a), 0.0F         )
#define bli_csabval2s( x, a ) { float ti;  bli_cabval2ris( bli_creal(x), bli_cimag(x), bli_sreal(a), ti           ); ( void )ti; }
#define bli_zsabval2s( x, a ) { float ti;  bli_zabval2ris( bli_zreal(x), bli_zimag(x), bli_sreal(a), ti           ); ( void )ti; }

#define bli_sdabval2s( x, a )              bli_sabval2ris( bli_sreal(x), bli_simag(x), bli_dreal(a), 0.0          )
#define bli_ddabval2s( x, a )              bli_dabval2ris( bli_dreal(x), bli_dimag(x), bli_dreal(a), 0.0          )
#define bli_cdabval2s( x, a ) { double ti; bli_cabval2ris( bli_creal(x), bli_cimag(x), bli_dreal(a), ti           ); ( void )ti; }
#define bli_zdabval2s( x, a ) { double ti; bli_zabval2ris( bli_zreal(x), bli_zimag(x), bli_dreal(a), ti           ); ( void )ti; }

#define bli_scabval2s( x, a )              bli_sabval2ris( bli_sreal(x), bli_simag(x), bli_creal(a), bli_cimag(a) )
#define bli_dcabval2s( x, a )              bli_dabval2ris( bli_dreal(x), bli_dimag(x), bli_creal(a), bli_cimag(a) )
#define bli_ccabval2s( x, a )              bli_cabval2ris( bli_creal(x), bli_cimag(x), bli_creal(a), bli_cimag(a) )
#define bli_zcabval2s( x, a )              bli_zabval2ris( bli_zreal(x), bli_zimag(x), bli_creal(a), bli_cimag(a) )

#define bli_szabval2s( x, a )              bli_sabval2ris( bli_sreal(x), bli_simag(x), bli_zreal(a), bli_zimag(a) )
#define bli_dzabval2s( x, a )              bli_dabval2ris( bli_dreal(x), bli_dimag(x), bli_zreal(a), bli_zimag(a) )
#define bli_czabval2s( x, a )              bli_cabval2ris( bli_creal(x), bli_cimag(x), bli_zreal(a), bli_zimag(a) )
#define bli_zzabval2s( x, a )              bli_zabval2ris( bli_zreal(x), bli_zimag(x), bli_zreal(a), bli_zimag(a) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_ssabval2s( x, a )  bli_sssets( fabsf(x), 0.0, (a) )
#define bli_dsabval2s( x, a )  bli_dssets( fabs (x), 0.0, (a) )
#define bli_csabval2s( x, a )  bli_cssets( cabsf(x), 0.0, (a) )
#define bli_zsabval2s( x, a )  bli_zssets( cabs (x), 0.0, (a) )

#define bli_sdabval2s( x, a )  bli_sdsets( fabsf(x), 0.0, (a) )
#define bli_ddabval2s( x, a )  bli_ddsets( fabs (x), 0.0, (a) )
#define bli_cdabval2s( x, a )  bli_cdsets( cabsf(x), 0.0, (a) )
#define bli_zdabval2s( x, a )  bli_zdsets( cabs (x), 0.0, (a) )

#define bli_scabval2s( x, a )  bli_scsets( fabsf(x), 0.0, (a) )
#define bli_dcabval2s( x, a )  bli_dcsets( fabs (x), 0.0, (a) )
#define bli_ccabval2s( x, a )  bli_ccsets( cabsf(x), 0.0, (a) )
#define bli_zcabval2s( x, a )  bli_zcsets( cabs (x), 0.0, (a) )

#define bli_szabval2s( x, a )  bli_szsets( fabsf(x), 0.0, (a) )
#define bli_dzabval2s( x, a )  bli_dzsets( fabs (x), 0.0, (a) )
#define bli_czabval2s( x, a )  bli_czsets( cabsf(x), 0.0, (a) )
#define bli_zzabval2s( x, a )  bli_zzsets( cabs (x), 0.0, (a) )

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sabval2s( x, a )  bli_ssabval2s( x, a )
#define bli_dabval2s( x, a )  bli_ddabval2s( x, a )
#define bli_cabval2s( x, a )  bli_ccabval2s( x, a )
#define bli_zabval2s( x, a )  bli_zzabval2s( x, a )


#endif
// end bli_abval2s.h

// begin bli_adds.h


#ifndef BLIS_ADDS_H
#define BLIS_ADDS_H

// adds

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_ssadds( a, y )  bli_saddris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dsadds( a, y )  bli_saddris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_csadds( a, y )  bli_saddris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zsadds( a, y )  bli_saddris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdadds( a, y )  bli_daddris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddadds( a, y )  bli_daddris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdadds( a, y )  bli_daddris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdadds( a, y )  bli_daddris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scadds( a, y )  bli_caddris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcadds( a, y )  bli_caddris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccadds( a, y )  bli_caddris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcadds( a, y )  bli_caddris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szadds( a, y )  bli_zaddris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzadds( a, y )  bli_zaddris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czadds( a, y )  bli_zaddris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzadds( a, y )  bli_zaddris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scadds( a, y )  { (y) += (a); }
#define bli_dcadds( a, y )  { (y) += (a); }
#define bli_ccadds( a, y )  { (y) += (a); }
#define bli_zcadds( a, y )  { (y) += (a); }

#define bli_szadds( a, y )  { (y) += (a); }
#define bli_dzadds( a, y )  { (y) += (a); }
#define bli_czadds( a, y )  { (y) += (a); }
#define bli_zzadds( a, y )  { (y) += (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sadds( a, y )  bli_ssadds( a, y )
#define bli_dadds( a, y )  bli_ddadds( a, y )
#define bli_cadds( a, y )  bli_ccadds( a, y )
#define bli_zadds( a, y )  bli_zzadds( a, y )


#endif

// end bli_adds.h
// begin bli_addjs.h


#ifndef BLIS_ADDJS_H
#define BLIS_ADDJS_H

// addjs

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_ssaddjs( a, y )  bli_saddjris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dsaddjs( a, y )  bli_saddjris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_csaddjs( a, y )  bli_saddjris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zsaddjs( a, y )  bli_saddjris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdaddjs( a, y )  bli_daddjris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddaddjs( a, y )  bli_daddjris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdaddjs( a, y )  bli_daddjris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdaddjs( a, y )  bli_daddjris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scaddjs( a, y )  bli_caddjris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcaddjs( a, y )  bli_caddjris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccaddjs( a, y )  bli_caddjris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcaddjs( a, y )  bli_caddjris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szaddjs( a, y )  bli_zaddjris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzaddjs( a, y )  bli_zaddjris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czaddjs( a, y )  bli_zaddjris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzaddjs( a, y )  bli_zaddjris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scaddjs( a, y )  { (y) +=      (a); }
#define bli_dcaddjs( a, y )  { (y) +=      (a); }
#define bli_ccaddjs( a, y )  { (y) += conjf(a); }
#define bli_zcaddjs( a, y )  { (y) += conj (a); }

#define bli_szaddjs( a, y )  { (y) +=      (a); }
#define bli_dzaddjs( a, y )  { (y) +=      (a); }
#define bli_czaddjs( a, y )  { (y) += conjf(a); }
#define bli_zzaddjs( a, y )  { (y) += conj (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_saddjs( a, y )  bli_ssaddjs( a, y )
#define bli_daddjs( a, y )  bli_ddaddjs( a, y )
#define bli_caddjs( a, y )  bli_ccaddjs( a, y )
#define bli_zaddjs( a, y )  bli_zzaddjs( a, y )


#endif

// end bli_addjs.h

// begin bli_add3s.h


#ifndef BLIS_ADD3S_H
#define BLIS_ADD3S_H

// add3s

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of b.
// - The third char encodes the type of c.


// -- (axy) = (??s) ------------------------------------------------------------

#define bli_sssadd3s( a, b, c )  bli_sadd3ris( bli_sreal(a), bli_simag(a), bli_sreal(b), bli_simag(b), bli_sreal(c), bli_simag(c) )
#define bli_dssadd3s( a, b, c )  bli_sadd3ris( bli_dreal(a), bli_dimag(a), bli_sreal(b), bli_simag(b), bli_sreal(c), bli_simag(c) )
#define bli_cssadd3s( a, b, c )  bli_sadd3ris( bli_creal(a), bli_cimag(a), bli_sreal(b), bli_simag(b), bli_sreal(c), bli_simag(c) )
#define bli_zssadd3s( a, b, c )  bli_sadd3ris( bli_zreal(a), bli_zimag(a), bli_sreal(b), bli_simag(b), bli_sreal(c), bli_simag(c) )

#define bli_sdsadd3s( a, b, c )  bli_sadd3ris( bli_sreal(a), bli_simag(a), bli_dreal(b), bli_dimag(b), bli_sreal(c), bli_simag(c) )
#define bli_ddsadd3s( a, b, c )  bli_sadd3ris( bli_dreal(a), bli_dimag(a), bli_dreal(b), bli_dimag(b), bli_sreal(c), bli_simag(c) )
#define bli_cdsadd3s( a, b, c )  bli_sadd3ris( bli_creal(a), bli_cimag(a), bli_dreal(b), bli_dimag(b), bli_sreal(c), bli_simag(c) )
#define bli_zdsadd3s( a, b, c )  bli_sadd3ris( bli_zreal(a), bli_zimag(a), bli_dreal(b), bli_dimag(b), bli_sreal(c), bli_simag(c) )

#define bli_scsadd3s( a, b, c )  bli_sadd3ris( bli_sreal(a), bli_simag(a), bli_creal(b), bli_cimag(b), bli_sreal(c), bli_simag(c) )
#define bli_dcsadd3s( a, b, c )  bli_sadd3ris( bli_dreal(a), bli_dimag(a), bli_creal(b), bli_cimag(b), bli_sreal(c), bli_simag(c) )
#define bli_ccsadd3s( a, b, c )  bli_sadd3ris( bli_creal(a), bli_cimag(a), bli_creal(b), bli_cimag(b), bli_sreal(c), bli_simag(c) )
#define bli_zcsadd3s( a, b, c )  bli_sadd3ris( bli_zreal(a), bli_zimag(a), bli_creal(b), bli_cimag(b), bli_sreal(c), bli_simag(c) )

#define bli_szsadd3s( a, b, c )  bli_sadd3ris( bli_sreal(a), bli_simag(a), bli_zreal(b), bli_zimag(b), bli_sreal(c), bli_simag(c) )
#define bli_dzsadd3s( a, b, c )  bli_sadd3ris( bli_dreal(a), bli_dimag(a), bli_zreal(b), bli_zimag(b), bli_sreal(c), bli_simag(c) )
#define bli_czsadd3s( a, b, c )  bli_sadd3ris( bli_creal(a), bli_cimag(a), bli_zreal(b), bli_zimag(b), bli_sreal(c), bli_simag(c) )
#define bli_zzsadd3s( a, b, c )  bli_sadd3ris( bli_zreal(a), bli_zimag(a), bli_zreal(b), bli_zimag(b), bli_sreal(c), bli_simag(c) )

// -- (axy) = (??d) ------------------------------------------------------------

#define bli_ssdadd3s( a, b, c )  bli_dadd3ris( bli_sreal(a), bli_simag(a), bli_sreal(b), bli_simag(b), bli_dreal(c), bli_dimag(c) )
#define bli_dsdadd3s( a, b, c )  bli_dadd3ris( bli_dreal(a), bli_dimag(a), bli_sreal(b), bli_simag(b), bli_dreal(c), bli_dimag(c) )
#define bli_csdadd3s( a, b, c )  bli_dadd3ris( bli_creal(a), bli_cimag(a), bli_sreal(b), bli_simag(b), bli_dreal(c), bli_dimag(c) )
#define bli_zsdadd3s( a, b, c )  bli_dadd3ris( bli_zreal(a), bli_zimag(a), bli_sreal(b), bli_simag(b), bli_dreal(c), bli_dimag(c) )

#define bli_sddadd3s( a, b, c )  bli_dadd3ris( bli_sreal(a), bli_simag(a), bli_dreal(b), bli_dimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_dddadd3s( a, b, c )  bli_dadd3ris( bli_dreal(a), bli_dimag(a), bli_dreal(b), bli_dimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_cddadd3s( a, b, c )  bli_dadd3ris( bli_creal(a), bli_cimag(a), bli_dreal(b), bli_dimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_zddadd3s( a, b, c )  bli_dadd3ris( bli_zreal(a), bli_zimag(a), bli_dreal(b), bli_dimag(b), bli_dreal(c), bli_dimag(c) )

#define bli_scdadd3s( a, b, c )  bli_dadd3ris( bli_sreal(a), bli_simag(a), bli_creal(b), bli_cimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_dcdadd3s( a, b, c )  bli_dadd3ris( bli_dreal(a), bli_dimag(a), bli_creal(b), bli_cimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_ccdadd3s( a, b, c )  bli_dadd3ris( bli_creal(a), bli_cimag(a), bli_creal(b), bli_cimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_zcdadd3s( a, b, c )  bli_dadd3ris( bli_zreal(a), bli_zimag(a), bli_creal(b), bli_cimag(b), bli_dreal(c), bli_dimag(c) )

#define bli_szdadd3s( a, b, c )  bli_dadd3ris( bli_sreal(a), bli_simag(a), bli_zreal(b), bli_zimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_dzdadd3s( a, b, c )  bli_dadd3ris( bli_dreal(a), bli_dimag(a), bli_zreal(b), bli_zimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_czdadd3s( a, b, c )  bli_dadd3ris( bli_creal(a), bli_cimag(a), bli_zreal(b), bli_zimag(b), bli_dreal(c), bli_dimag(c) )
#define bli_zzdadd3s( a, b, c )  bli_dadd3ris( bli_zreal(a), bli_zimag(a), bli_zreal(b), bli_zimag(b), bli_dreal(c), bli_dimag(c) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscadd3s( a, b, c )  bli_sadd3ris( bli_sreal(a), bli_simag(a), bli_sreal(b), bli_simag(b), bli_creal(c), bli_cimag(c) )
#define bli_dscadd3s( a, b, c )  bli_sadd3ris( bli_dreal(a), bli_dimag(a), bli_sreal(b), bli_simag(b), bli_creal(c), bli_cimag(c) )
#define bli_cscadd3s( a, b, c )  bli_cadd3ris( bli_creal(a), bli_cimag(a), bli_sreal(b), bli_simag(b), bli_creal(c), bli_cimag(c) )
#define bli_zscadd3s( a, b, c )  bli_cadd3ris( bli_zreal(a), bli_zimag(a), bli_sreal(b), bli_simag(b), bli_creal(c), bli_cimag(c) )

#define bli_sdcadd3s( a, b, c )  bli_sadd3ris( bli_sreal(a), bli_simag(a), bli_dreal(b), bli_dimag(b), bli_creal(c), bli_cimag(c) )
#define bli_ddcadd3s( a, b, c )  bli_sadd3ris( bli_dreal(a), bli_dimag(a), bli_dreal(b), bli_dimag(b), bli_creal(c), bli_cimag(c) )
#define bli_cdcadd3s( a, b, c )  bli_cadd3ris( bli_creal(a), bli_cimag(a), bli_dreal(b), bli_dimag(b), bli_creal(c), bli_cimag(c) )
#define bli_zdcadd3s( a, b, c )  bli_cadd3ris( bli_zreal(a), bli_zimag(a), bli_dreal(b), bli_dimag(b), bli_creal(c), bli_cimag(c) )

#define bli_sccadd3s( a, b, c )  bli_cadd3ris( bli_sreal(a), bli_simag(a), bli_creal(b), bli_cimag(b), bli_creal(c), bli_cimag(c) )
#define bli_dccadd3s( a, b, c )  bli_cadd3ris( bli_dreal(a), bli_dimag(a), bli_creal(b), bli_cimag(b), bli_creal(c), bli_cimag(c) )
#define bli_cccadd3s( a, b, c )  bli_cadd3ris( bli_creal(a), bli_cimag(a), bli_creal(b), bli_cimag(b), bli_creal(c), bli_cimag(c) )
#define bli_zccadd3s( a, b, c )  bli_cadd3ris( bli_zreal(a), bli_zimag(a), bli_creal(b), bli_cimag(b), bli_creal(c), bli_cimag(c) )

#define bli_szcadd3s( a, b, c )  bli_cadd3ris( bli_sreal(a), bli_simag(a), bli_zreal(b), bli_zimag(b), bli_creal(c), bli_cimag(c) )
#define bli_dzcadd3s( a, b, c )  bli_cadd3ris( bli_dreal(a), bli_dimag(a), bli_zreal(b), bli_zimag(b), bli_creal(c), bli_cimag(c) )
#define bli_czcadd3s( a, b, c )  bli_cadd3ris( bli_creal(a), bli_cimag(a), bli_zreal(b), bli_zimag(b), bli_creal(c), bli_cimag(c) )
#define bli_zzcadd3s( a, b, c )  bli_cadd3ris( bli_zreal(a), bli_zimag(a), bli_zreal(b), bli_zimag(b), bli_creal(c), bli_cimag(c) )

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszadd3s( a, b, c )  bli_dadd3ris( bli_sreal(a), bli_simag(a), bli_sreal(b), bli_simag(b), bli_zreal(c), bli_zimag(c) )
#define bli_dszadd3s( a, b, c )  bli_dadd3ris( bli_dreal(a), bli_dimag(a), bli_sreal(b), bli_simag(b), bli_zreal(c), bli_zimag(c) )
#define bli_cszadd3s( a, b, c )  bli_zadd3ris( bli_creal(a), bli_cimag(a), bli_sreal(b), bli_simag(b), bli_zreal(c), bli_zimag(c) )
#define bli_zszadd3s( a, b, c )  bli_zadd3ris( bli_zreal(a), bli_zimag(a), bli_sreal(b), bli_simag(b), bli_zreal(c), bli_zimag(c) )

#define bli_sdzadd3s( a, b, c )  bli_dadd3ris( bli_sreal(a), bli_simag(a), bli_dreal(b), bli_dimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_ddzadd3s( a, b, c )  bli_dadd3ris( bli_dreal(a), bli_dimag(a), bli_dreal(b), bli_dimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_cdzadd3s( a, b, c )  bli_zadd3ris( bli_creal(a), bli_cimag(a), bli_dreal(b), bli_dimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_zdzadd3s( a, b, c )  bli_zadd3ris( bli_zreal(a), bli_zimag(a), bli_dreal(b), bli_dimag(b), bli_zreal(c), bli_zimag(c) )

#define bli_sczadd3s( a, b, c )  bli_zadd3ris( bli_sreal(a), bli_simag(a), bli_creal(b), bli_cimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_dczadd3s( a, b, c )  bli_zadd3ris( bli_dreal(a), bli_dimag(a), bli_creal(b), bli_cimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_cczadd3s( a, b, c )  bli_zadd3ris( bli_creal(a), bli_cimag(a), bli_creal(b), bli_cimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_zczadd3s( a, b, c )  bli_zadd3ris( bli_zreal(a), bli_zimag(a), bli_creal(b), bli_cimag(b), bli_zreal(c), bli_zimag(c) )

#define bli_szzadd3s( a, b, c )  bli_zadd3ris( bli_sreal(a), bli_simag(a), bli_zreal(b), bli_zimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_dzzadd3s( a, b, c )  bli_zadd3ris( bli_dreal(a), bli_dimag(a), bli_zreal(b), bli_zimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_czzadd3s( a, b, c )  bli_zadd3ris( bli_creal(a), bli_cimag(a), bli_zreal(b), bli_zimag(b), bli_zreal(c), bli_zimag(c) )
#define bli_zzzadd3s( a, b, c )  bli_zadd3ris( bli_zreal(a), bli_zimag(a), bli_zreal(b), bli_zimag(b), bli_zreal(c), bli_zimag(c) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_dscadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_cscadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zscadd3s( a, b, c )  { (c) = (a) + (b); }

#define bli_sdcadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_ddcadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_cdcadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zdcadd3s( a, b, c )  { (c) = (a) + (b); }

#define bli_sccadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_dccadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_cccadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zccadd3s( a, b, c )  { (c) = (a) + (b); }

#define bli_szcadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_dzcadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_czcadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zzcadd3s( a, b, c )  { (c) = (a) + (b); }

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_dszadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_cszadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zszadd3s( a, b, c )  { (c) = (a) + (b); }

#define bli_sdzadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_ddzadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_cdzadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zdzadd3s( a, b, c )  { (c) = (a) + (b); }

#define bli_sczadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_dczadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_cczadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zczadd3s( a, b, c )  { (c) = (a) + (b); }

#define bli_szzadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_dzzadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_czzadd3s( a, b, c )  { (c) = (a) + (b); }
#define bli_zzzadd3s( a, b, c )  { (c) = (a) + (b); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sadd3s( a, b, c )  bli_sssadd3s( a, b, c )
#define bli_dadd3s( a, b, c )  bli_dddadd3s( a, b, c )
#define bli_cadd3s( a, b, c )  bli_cccadd3s( a, b, c )
#define bli_zadd3s( a, b, c )  bli_zzzadd3s( a, b, c )


#endif

// end bli_add3s.h

// begin bli_axpbys.h


#ifndef BLIS_AXPBYS_H
#define BLIS_AXPBYS_H

// axpbys

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of x.
// - The third char encodes the type of b.
// - The fourth char encodes the type of y.


// -- (axby) = (???s) ----------------------------------------------------------

#define bli_ssssaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dsssaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_csssaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zsssaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_sdssaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddssaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdssaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdssaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_scssaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dcssaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_ccssaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zcssaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_szssaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzssaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_czssaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzssaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )

#define bli_ssdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dsdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_csdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zsdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sddsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dddsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cddsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zddsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_scdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dcdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ccdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zcdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_szdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzdsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )

#define bli_sscsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dscsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cscsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zscsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sdcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sccsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dccsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cccsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zccsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_szcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzcsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )

#define bli_sszsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dszsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cszsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zszsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sdzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sczsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dczsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cczsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zczsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_szzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzzsaxpbys( a, x, b, y )  bli_saxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )

// -- (axby) = (???d) ----------------------------------------------------------

#define bli_sssdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dssdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cssdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zssdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_scsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dcsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ccsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zcsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzsdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )

#define bli_ssddaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dsddaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_csddaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zsddaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdddaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddddaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdddaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdddaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_scddaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dcddaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ccddaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zcddaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szddaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzddaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czddaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzddaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_sscdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dscdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cscdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zscdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sccdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dccdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cccdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zccdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzcdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_sszdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dszdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cszdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zszdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sczdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dczdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cczdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zczdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzzdaxpbys( a, x, b, y )  bli_daxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axby) = (???c) ----------------------------------------------------------

#define bli_ssscaxpbys( a, x, b, y )    bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dsscaxpbys( a, x, b, y )    bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_csscaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zsscaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_sdscaxpbys( a, x, b, y )    bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddscaxpbys( a, x, b, y )    bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdscaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdscaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_scscaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dcscaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_ccscaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zcscaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_szscaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzscaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_czscaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzscaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )

#define bli_ssdcaxpbys( a, x, b, y )    bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dsdcaxpbys( a, x, b, y )    bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_csdcaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zsdcaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sddcaxpbys( a, x, b, y )    bli_saxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dddcaxpbys( a, x, b, y )    bli_saxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cddcaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zddcaxpbys( a, x, b, y )  bli_cssaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_scdcaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dcdcaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ccdcaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zcdcaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_szdcaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzdcaxpbys( a, x, b, y )  bli_scsaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czdcaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzdcaxpbys( a, x, b, y )  bli_ccsaxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )

#define bli_ssccaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dsccaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_csccaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zsccaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sdccaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddccaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdccaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdccaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_scccaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dcccaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ccccaxpbys( a, x, b, y )    bli_caxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zcccaxpbys( a, x, b, y )    bli_caxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_szccaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzccaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czccaxpbys( a, x, b, y )    bli_caxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzccaxpbys( a, x, b, y )    bli_caxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )

#define bli_sszcaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dszcaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cszcaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zszcaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sdzcaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddzcaxpbys( a, x, b, y )  bli_sscaxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdzcaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdzcaxpbys( a, x, b, y )  bli_cscaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sczcaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dczcaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cczcaxpbys( a, x, b, y )    bli_caxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zczcaxpbys( a, x, b, y )    bli_caxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_szzcaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzzcaxpbys( a, x, b, y )  bli_sccaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czzcaxpbys( a, x, b, y )    bli_caxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzzcaxpbys( a, x, b, y )    bli_caxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )

// -- (axby) = (???z) ----------------------------------------------------------

#define bli_ssszaxpbys( a, x, b, y )    bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dsszaxpbys( a, x, b, y )    bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_csszaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zsszaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sdszaxpbys( a, x, b, y )    bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddszaxpbys( a, x, b, y )    bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdszaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdszaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_scszaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dcszaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ccszaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zcszaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szszaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzszaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czszaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzszaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )

#define bli_ssdzaxpbys( a, x, b, y )    bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dsdzaxpbys( a, x, b, y )    bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_csdzaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zsdzaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sddzaxpbys( a, x, b, y )    bli_daxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dddzaxpbys( a, x, b, y )    bli_daxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cddzaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zddzaxpbys( a, x, b, y )  bli_zddaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_scdzaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dcdzaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ccdzaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zcdzaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szdzaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzdzaxpbys( a, x, b, y )  bli_dzdaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czdzaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzdzaxpbys( a, x, b, y )  bli_zzdaxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_ssczaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dsczaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_csczaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zsczaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sdczaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddczaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdczaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdczaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_scczaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dcczaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ccczaxpbys( a, x, b, y )    bli_zaxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zcczaxpbys( a, x, b, y )    bli_zaxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szczaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzczaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czczaxpbys( a, x, b, y )    bli_zaxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzczaxpbys( a, x, b, y )    bli_zaxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_sszzaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dszzaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cszzaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zszzaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sdzzaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddzzaxpbys( a, x, b, y )  bli_ddzaxpbyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdzzaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdzzaxpbys( a, x, b, y )  bli_zdzaxpbyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sczzaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dczzaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cczzaxpbys( a, x, b, y )    bli_zaxpbyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zczzaxpbys( a, x, b, y )    bli_zaxpbyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szzzaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzzzaxpbys( a, x, b, y )  bli_dzzaxpbyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czzzaxpbys( a, x, b, y )    bli_zaxpbyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzzzaxpbys( a, x, b, y )    bli_zaxpbyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axby) = (???c) ----------------------------------------------------------

#define bli_ssscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dsscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_csscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zsscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sdscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ddscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cdscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zdscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_scscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dcscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ccscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zcscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzscaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

#define bli_ssdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dsdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_csdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zsdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sddcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dddcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cddcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zddcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_scdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dcdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ccdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zcdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzdcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

#define bli_ssccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dsccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_csccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zsccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sdccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ddccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cdccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zdccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_scccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dcccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ccccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zcccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzccaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

#define bli_sszcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dszcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cszcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zszcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sdzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ddzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cdzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zdzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sczcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dczcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cczcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zczcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzzcaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

// -- (axby) = (???z) ----------------------------------------------------------

#define bli_ssszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dsszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_csszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zsszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sdszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ddszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cdszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zdszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_scszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dcszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ccszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zcszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzszaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

#define bli_ssdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dsdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_csdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zsdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sddzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dddzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cddzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zddzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_scdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dcdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ccdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zcdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzdzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

#define bli_ssczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dsczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_csczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zsczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sdczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ddczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cdczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zdczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_scczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dcczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ccczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zcczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzczaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

#define bli_sszzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dszzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cszzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zszzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sdzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_ddzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cdzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zdzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_sczzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dczzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_cczzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zczzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_szzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_dzzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_czzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }
#define bli_zzzzaxpbys( a, x, b, y )  { (y) = (a) * (x) + (b) * (y); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_saxpbys( a, x, b, y )  bli_ssssaxpbys( a, x, b, y )
#define bli_daxpbys( a, x, b, y )  bli_ddddaxpbys( a, x, b, y )
#define bli_caxpbys( a, x, b, y )  bli_ccccaxpbys( a, x, b, y )
#define bli_zaxpbys( a, x, b, y )  bli_zzzzaxpbys( a, x, b, y )


#endif

// end bli_axpbys.h
// begin bli_axpbyjs.h


#ifndef BLIS_AXPBYJS_H
#define BLIS_AXPBYJS_H

// axpbyjs

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of x.
// - The third char encodes the type of b.
// - The fourth char encodes the type of y.


// -- (axby) = (???s) ----------------------------------------------------------

#define bli_ssssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dsssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_csssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zsssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_sdssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_scssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dcssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_ccssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zcssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_szssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_czssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzssaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )

#define bli_ssdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dsdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_csdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zsdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sddsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dddsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cddsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zddsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_scdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dcdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ccdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zcdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_szdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzdsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )

#define bli_sscsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dscsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cscsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zscsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sdcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sccsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dccsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cccsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zccsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_szcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzcsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )

#define bli_sszsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dszsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cszsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zszsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sdzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_sczsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dczsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cczsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zczsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_szzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzzsaxpbyjs( a, x, b, y )  bli_saxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )

// -- (axby) = (???d) ----------------------------------------------------------

#define bli_sssdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dssdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cssdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zssdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_scsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dcsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ccsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zcsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzsdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )

#define bli_ssddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dsddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_csddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zsddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_scddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dcddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ccddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zcddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzddaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_sscdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dscdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cscdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zscdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sccdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dccdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cccdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zccdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzcdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_sszdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dszdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cszdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zszdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sdzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ddzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cdzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zdzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_sczdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dczdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cczdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zczdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_szzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzzdaxpbyjs( a, x, b, y )  bli_daxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axby) = (???c) ----------------------------------------------------------

#define bli_ssscaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dsscaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_csscaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zsscaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_sdscaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddscaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdscaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdscaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_scscaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dcscaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_ccscaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zcscaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_szscaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzscaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_czscaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzscaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )

#define bli_ssdcaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dsdcaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_csdcaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zsdcaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sddcaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dddcaxpbyjs( a, x, b, y )    bli_saxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cddcaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zddcaxpbyjs( a, x, b, y )  bli_cssaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_scdcaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dcdcaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ccdcaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zcdcaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_szdcaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzdcaxpbyjs( a, x, b, y )  bli_scsaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czdcaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzdcaxpbyjs( a, x, b, y )  bli_ccsaxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )

#define bli_ssccaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dsccaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_csccaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zsccaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sdccaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddccaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdccaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdccaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_scccaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dcccaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ccccaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zcccaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_szccaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzccaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czccaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzccaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )

#define bli_sszcaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dszcaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cszcaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zszcaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sdzcaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddzcaxpbyjs( a, x, b, y )  bli_sscaxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdzcaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdzcaxpbyjs( a, x, b, y )  bli_cscaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_sczcaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dczcaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cczcaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zczcaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_szzcaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzzcaxpbyjs( a, x, b, y )  bli_sccaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czzcaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzzcaxpbyjs( a, x, b, y )    bli_caxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )

// -- (axby) = (???z) ----------------------------------------------------------

#define bli_ssszaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dsszaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_csszaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zsszaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sdszaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddszaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdszaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdszaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_scszaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dcszaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ccszaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zcszaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szszaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzszaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czszaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzszaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )

#define bli_ssdzaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dsdzaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_csdzaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zsdzaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sddzaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dddzaxpbyjs( a, x, b, y )    bli_daxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cddzaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zddzaxpbyjs( a, x, b, y )  bli_zddaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_scdzaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dcdzaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ccdzaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zcdzaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szdzaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzdzaxpbyjs( a, x, b, y )  bli_dzdaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czdzaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzdzaxpbyjs( a, x, b, y )  bli_zzdaxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_ssczaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dsczaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_csczaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zsczaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sdczaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddczaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdczaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdczaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_scczaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dcczaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ccczaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zcczaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szczaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzczaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czczaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzczaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_sszzaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dszzaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cszzaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zszzaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sdzzaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddzzaxpbyjs( a, x, b, y )  bli_ddzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdzzaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdzzaxpbyjs( a, x, b, y )  bli_zdzaxpbyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_sczzaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dczzaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cczzaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zczzaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_szzzaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzzzaxpbyjs( a, x, b, y )  bli_dzzaxpbyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czzzaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzzzaxpbyjs( a, x, b, y )    bli_zaxpbyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axby) = (???c) ----------------------------------------------------------

#define bli_ssscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dsscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_csscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zsscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sdscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_ddscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cdscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zdscaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_scscaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dcscaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_ccscaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zcscaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szscaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzscaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czscaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzscaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

#define bli_ssdcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dsdcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_csdcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zsdcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sddcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dddcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cddcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zddcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_scdcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dcdcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_ccdcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zcdcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szdcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzdcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czdcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzdcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

#define bli_ssccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dsccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_csccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zsccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sdccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_ddccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cdccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zdccaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_scccaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dcccaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_ccccaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zcccaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szccaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzccaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czccaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzccaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

#define bli_sszcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dszcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cszcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zszcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sdzcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_ddzcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cdzcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zdzcaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sczcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dczcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_cczcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zczcaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szzcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzzcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czzcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzzcaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

// -- (axby) = (???z) ----------------------------------------------------------

#define bli_ssszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dsszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_csszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zsszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sdszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_ddszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cdszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zdszaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_scszaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dcszaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_ccszaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zcszaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szszaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzszaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czszaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzszaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

#define bli_ssdzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dsdzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_csdzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zsdzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sddzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dddzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cddzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zddzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_scdzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dcdzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_ccdzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zcdzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szdzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzdzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czdzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzdzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

#define bli_ssczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dsczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_csczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zsczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sdczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_ddczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cdczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zdczaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_scczaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dcczaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_ccczaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zcczaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szczaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzczaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czczaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzczaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

#define bli_sszzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_dszzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cszzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zszzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sdzzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_ddzzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_cdzzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_zdzzaxpbyjs( a, x, b, y )  { (y) = (a) *      (x) + (b) * (y); }
#define bli_sczzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_dczzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_cczzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_zczzaxpbyjs( a, x, b, y )  { (y) = (a) * conjf(x) + (b) * (y); }
#define bli_szzzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_dzzzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_czzzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }
#define bli_zzzzaxpbyjs( a, x, b, y )  { (y) = (a) *  conj(x) + (b) * (y); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_saxpbyjs( a, x, b, y )  bli_ssssaxpbyjs( a, x, b, y )
#define bli_daxpbyjs( a, x, b, y )  bli_ddddaxpbyjs( a, x, b, y )
#define bli_caxpbyjs( a, x, b, y )  bli_ccccaxpbyjs( a, x, b, y )
#define bli_zaxpbyjs( a, x, b, y )  bli_zzzzaxpbyjs( a, x, b, y )


#endif

// end bli_axpbyjs.h

// begin bli_axpys.h


#ifndef BLIS_AXPYS_H
#define BLIS_AXPYS_H

// axpys

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of x.
// - The third char encodes the type of y.


// -- (axy) = (??s) ------------------------------------------------------------

#define bli_sssaxpys( a, x, y )  bli_saxpyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dssaxpys( a, x, y )  bli_saxpyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_cssaxpys( a, x, y )  bli_saxpyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_zssaxpys( a, x, y )  bli_saxpyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdsaxpys( a, x, y )  bli_saxpyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ddsaxpys( a, x, y )  bli_saxpyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cdsaxpys( a, x, y )  bli_saxpyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zdsaxpys( a, x, y )  bli_saxpyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )

#define bli_scsaxpys( a, x, y )  bli_saxpyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dcsaxpys( a, x, y )  bli_saxpyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ccsaxpys( a, x, y )  bli_saxpyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zcsaxpys( a, x, y )  bli_saxpyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )

#define bli_szsaxpys( a, x, y )  bli_saxpyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dzsaxpys( a, x, y )  bli_saxpyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_czsaxpys( a, x, y )  bli_saxpyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zzsaxpys( a, x, y )  bli_saxpyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

// -- (axy) = (??d) ------------------------------------------------------------

#define bli_ssdaxpys( a, x, y )  bli_daxpyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dsdaxpys( a, x, y )  bli_daxpyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_csdaxpys( a, x, y )  bli_daxpyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zsdaxpys( a, x, y )  bli_daxpyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )

#define bli_sddaxpys( a, x, y )  bli_daxpyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dddaxpys( a, x, y )  bli_daxpyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cddaxpys( a, x, y )  bli_daxpyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zddaxpys( a, x, y )  bli_daxpyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_scdaxpys( a, x, y )  bli_daxpyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dcdaxpys( a, x, y )  bli_daxpyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ccdaxpys( a, x, y )  bli_daxpyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zcdaxpys( a, x, y )  bli_daxpyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_szdaxpys( a, x, y )  bli_daxpyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dzdaxpys( a, x, y )  bli_daxpyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_czdaxpys( a, x, y )  bli_daxpyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zzdaxpys( a, x, y )  bli_daxpyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscaxpys( a, x, y )  bli_saxpyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dscaxpys( a, x, y )  bli_saxpyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_cscaxpys( a, x, y )  bli_caxpyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_zscaxpys( a, x, y )  bli_caxpyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )

#define bli_sdcaxpys( a, x, y )  bli_saxpyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_ddcaxpys( a, x, y )  bli_saxpyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cdcaxpys( a, x, y )  bli_caxpyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zdcaxpys( a, x, y )  bli_caxpyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )

#define bli_sccaxpys( a, x, y )  bli_scaxpyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccaxpys( a, x, y )  bli_scaxpyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccaxpys( a, x, y )   bli_caxpyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccaxpys( a, x, y )   bli_caxpyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcaxpys( a, x, y )  bli_scaxpyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dzcaxpys( a, x, y )  bli_scaxpyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_czcaxpys( a, x, y )   bli_caxpyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zzcaxpys( a, x, y )   bli_caxpyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszaxpys( a, x, y )  bli_daxpyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dszaxpys( a, x, y )  bli_daxpyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cszaxpys( a, x, y )  bli_zaxpyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zszaxpys( a, x, y )  bli_zaxpyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sdzaxpys( a, x, y )  bli_daxpyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_ddzaxpys( a, x, y )  bli_daxpyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cdzaxpys( a, x, y )  bli_zaxpyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zdzaxpys( a, x, y )  bli_zaxpyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sczaxpys( a, x, y )  bli_dzaxpyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dczaxpys( a, x, y )  bli_dzaxpyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cczaxpys( a, x, y )   bli_zaxpyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zczaxpys( a, x, y )   bli_zaxpyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_szzaxpys( a, x, y )  bli_dzaxpyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzzaxpys( a, x, y )  bli_dzaxpyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czzaxpys( a, x, y )   bli_zaxpyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzzaxpys( a, x, y )   bli_zaxpyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_dscaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_cscaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zscaxpys( a, x, y )  { (y) += (a) * (x); }

#define bli_sdcaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_ddcaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_cdcaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zdcaxpys( a, x, y )  { (y) += (a) * (x); }

#define bli_sccaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_dccaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_cccaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zccaxpys( a, x, y )  { (y) += (a) * (x); }

#define bli_szcaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_dzcaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_czcaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zzcaxpys( a, x, y )  { (y) += (a) * (x); }

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_dszaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_cszaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zszaxpys( a, x, y )  { (y) += (a) * (x); }

#define bli_sdzaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_ddzaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_cdzaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zdzaxpys( a, x, y )  { (y) += (a) * (x); }

#define bli_sczaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_dczaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_cczaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zczaxpys( a, x, y )  { (y) += (a) * (x); }

#define bli_szzaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_dzzaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_czzaxpys( a, x, y )  { (y) += (a) * (x); }
#define bli_zzzaxpys( a, x, y )  { (y) += (a) * (x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_saxpys( a, x, y )  bli_sssaxpys( a, x, y )
#define bli_daxpys( a, x, y )  bli_dddaxpys( a, x, y )
#define bli_caxpys( a, x, y )  bli_cccaxpys( a, x, y )
#define bli_zaxpys( a, x, y )  bli_zzzaxpys( a, x, y )


#endif

// end bli_axpys.h
// begin bli_axpyjs.h


#ifndef BLIS_AXPYJS_H
#define BLIS_AXPYJS_H

// axpyjs

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of x.
// - The third char encodes the type of y.


// -- (axy) = (??s) ------------------------------------------------------------

#define bli_sssaxpyjs( a, x, y )  bli_saxpyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dssaxpyjs( a, x, y )  bli_saxpyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_cssaxpyjs( a, x, y )  bli_saxpyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_zssaxpyjs( a, x, y )  bli_saxpyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdsaxpyjs( a, x, y )  bli_saxpyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ddsaxpyjs( a, x, y )  bli_saxpyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cdsaxpyjs( a, x, y )  bli_saxpyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zdsaxpyjs( a, x, y )  bli_saxpyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )

#define bli_scsaxpyjs( a, x, y )  bli_saxpyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dcsaxpyjs( a, x, y )  bli_saxpyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ccsaxpyjs( a, x, y )  bli_saxpyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zcsaxpyjs( a, x, y )  bli_saxpyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )

#define bli_szsaxpyjs( a, x, y )  bli_saxpyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dzsaxpyjs( a, x, y )  bli_saxpyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_czsaxpyjs( a, x, y )  bli_saxpyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zzsaxpyjs( a, x, y )  bli_saxpyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

// -- (axy) = (??d) ------------------------------------------------------------

#define bli_ssdaxpyjs( a, x, y )  bli_daxpyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dsdaxpyjs( a, x, y )  bli_daxpyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_csdaxpyjs( a, x, y )  bli_daxpyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zsdaxpyjs( a, x, y )  bli_daxpyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )

#define bli_sddaxpyjs( a, x, y )  bli_daxpyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dddaxpyjs( a, x, y )  bli_daxpyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cddaxpyjs( a, x, y )  bli_daxpyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zddaxpyjs( a, x, y )  bli_daxpyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_scdaxpyjs( a, x, y )  bli_daxpyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dcdaxpyjs( a, x, y )  bli_daxpyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ccdaxpyjs( a, x, y )  bli_daxpyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zcdaxpyjs( a, x, y )  bli_daxpyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_szdaxpyjs( a, x, y )  bli_daxpyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dzdaxpyjs( a, x, y )  bli_daxpyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_czdaxpyjs( a, x, y )  bli_daxpyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zzdaxpyjs( a, x, y )  bli_daxpyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscaxpyjs( a, x, y )  bli_saxpyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dscaxpyjs( a, x, y )  bli_saxpyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_cscaxpyjs( a, x, y )  bli_caxpyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_zscaxpyjs( a, x, y )  bli_caxpyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )

#define bli_sdcaxpyjs( a, x, y )  bli_saxpyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_ddcaxpyjs( a, x, y )  bli_saxpyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cdcaxpyjs( a, x, y )  bli_caxpyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zdcaxpyjs( a, x, y )  bli_caxpyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )

#define bli_sccaxpyjs( a, x, y )  bli_scaxpyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccaxpyjs( a, x, y )  bli_scaxpyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccaxpyjs( a, x, y )   bli_caxpyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccaxpyjs( a, x, y )   bli_caxpyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcaxpyjs( a, x, y )  bli_scaxpyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dzcaxpyjs( a, x, y )  bli_scaxpyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_czcaxpyjs( a, x, y )   bli_caxpyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zzcaxpyjs( a, x, y )   bli_caxpyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszaxpyjs( a, x, y )  bli_daxpyjris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dszaxpyjs( a, x, y )  bli_daxpyjris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cszaxpyjs( a, x, y )  bli_zaxpyjris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zszaxpyjs( a, x, y )  bli_zaxpyjris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sdzaxpyjs( a, x, y )  bli_daxpyjris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_ddzaxpyjs( a, x, y )  bli_daxpyjris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cdzaxpyjs( a, x, y )  bli_zaxpyjris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zdzaxpyjs( a, x, y )  bli_zaxpyjris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sczaxpyjs( a, x, y )  bli_dzaxpyjris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dczaxpyjs( a, x, y )  bli_dzaxpyjris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cczaxpyjs( a, x, y )   bli_zaxpyjris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zczaxpyjs( a, x, y )   bli_zaxpyjris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_szzaxpyjs( a, x, y )  bli_dzaxpyjris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzzaxpyjs( a, x, y )  bli_dzaxpyjris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czzaxpyjs( a, x, y )   bli_zaxpyjris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzzaxpyjs( a, x, y )   bli_zaxpyjris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_dscaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_cscaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_zscaxpyjs( a, x, y )  { (y) += (a) * (x); }

#define bli_sdcaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_ddcaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_cdcaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_zdcaxpyjs( a, x, y )  { (y) += (a) * (x); }

#define bli_sccaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }
#define bli_dccaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }
#define bli_cccaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }
#define bli_zccaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }

#define bli_szcaxpyjs( a, x, y )  { (y) += (a) * conj(x); }
#define bli_dzcaxpyjs( a, x, y )  { (y) += (a) * conj(x); }
#define bli_czcaxpyjs( a, x, y )  { (y) += (a) * conj(x); }
#define bli_zzcaxpyjs( a, x, y )  { (y) += (a) * conj(x); }

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_dszaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_cszaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_zszaxpyjs( a, x, y )  { (y) += (a) * (x); }

#define bli_sdzaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_ddzaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_cdzaxpyjs( a, x, y )  { (y) += (a) * (x); }
#define bli_zdzaxpyjs( a, x, y )  { (y) += (a) * (x); }

#define bli_sczaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }
#define bli_dczaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }
#define bli_cczaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }
#define bli_zczaxpyjs( a, x, y )  { (y) += (a) * conjf(x); }

#define bli_szzaxpyjs( a, x, y )  { (y) += (a) * conj(x); }
#define bli_dzzaxpyjs( a, x, y )  { (y) += (a) * conj(x); }
#define bli_czzaxpyjs( a, x, y )  { (y) += (a) * conj(x); }
#define bli_zzzaxpyjs( a, x, y )  { (y) += (a) * conj(x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_saxpyjs( a, x, y )  bli_sssaxpyjs( a, x, y )
#define bli_daxpyjs( a, x, y )  bli_dddaxpyjs( a, x, y )
#define bli_caxpyjs( a, x, y )  bli_cccaxpyjs( a, x, y )
#define bli_zaxpyjs( a, x, y )  bli_zzzaxpyjs( a, x, y )


#endif

// end bli_axpyjs.h

// begin bli_axmys.h


#ifndef BLIS_AXMYS_H
#define BLIS_AXMYS_H

// axmys

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of x.
// - The third char encodes the type of y.


// -- (axy) = (??s) ------------------------------------------------------------

#define bli_sssaxmys( a, x, y )  bli_saxmyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dssaxmys( a, x, y )  bli_saxmyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_cssaxmys( a, x, y )  bli_saxmyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_zssaxmys( a, x, y )  bli_saxmyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdsaxmys( a, x, y )  bli_saxmyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ddsaxmys( a, x, y )  bli_saxmyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cdsaxmys( a, x, y )  bli_saxmyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zdsaxmys( a, x, y )  bli_saxmyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )

#define bli_scsaxmys( a, x, y )  bli_saxmyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dcsaxmys( a, x, y )  bli_saxmyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ccsaxmys( a, x, y )  bli_saxmyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zcsaxmys( a, x, y )  bli_saxmyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )

#define bli_szsaxmys( a, x, y )  bli_saxmyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dzsaxmys( a, x, y )  bli_saxmyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_czsaxmys( a, x, y )  bli_saxmyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zzsaxmys( a, x, y )  bli_saxmyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

// -- (axy) = (??d) ------------------------------------------------------------

#define bli_ssdaxmys( a, x, y )  bli_daxmyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dsdaxmys( a, x, y )  bli_daxmyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_csdaxmys( a, x, y )  bli_daxmyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zsdaxmys( a, x, y )  bli_daxmyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )

#define bli_sddaxmys( a, x, y )  bli_daxmyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dddaxmys( a, x, y )  bli_daxmyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cddaxmys( a, x, y )  bli_daxmyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zddaxmys( a, x, y )  bli_daxmyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_scdaxmys( a, x, y )  bli_daxmyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dcdaxmys( a, x, y )  bli_daxmyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ccdaxmys( a, x, y )  bli_daxmyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zcdaxmys( a, x, y )  bli_daxmyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_szdaxmys( a, x, y )  bli_daxmyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dzdaxmys( a, x, y )  bli_daxmyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_czdaxmys( a, x, y )  bli_daxmyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zzdaxmys( a, x, y )  bli_daxmyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscaxmys( a, x, y )  bli_saxmyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dscaxmys( a, x, y )  bli_saxmyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_cscaxmys( a, x, y )  bli_caxmyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_zscaxmys( a, x, y )  bli_caxmyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )

#define bli_sdcaxmys( a, x, y )  bli_saxmyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_ddcaxmys( a, x, y )  bli_saxmyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cdcaxmys( a, x, y )  bli_caxmyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zdcaxmys( a, x, y )  bli_caxmyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )

#define bli_sccaxmys( a, x, y )  bli_scaxmyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccaxmys( a, x, y )  bli_scaxmyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccaxmys( a, x, y )   bli_caxmyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccaxmys( a, x, y )   bli_caxmyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcaxmys( a, x, y )  bli_scaxmyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dzcaxmys( a, x, y )  bli_scaxmyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_czcaxmys( a, x, y )   bli_caxmyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zzcaxmys( a, x, y )   bli_caxmyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszaxmys( a, x, y )  bli_daxmyris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dszaxmys( a, x, y )  bli_daxmyris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cszaxmys( a, x, y )  bli_zaxmyris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zszaxmys( a, x, y )  bli_zaxmyris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sdzaxmys( a, x, y )  bli_daxmyris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_ddzaxmys( a, x, y )  bli_daxmyris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cdzaxmys( a, x, y )  bli_zaxmyris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zdzaxmys( a, x, y )  bli_zaxmyris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sczaxmys( a, x, y )  bli_dzaxmyris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dczaxmys( a, x, y )  bli_dzaxmyris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cczaxmys( a, x, y )   bli_zaxmyris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zczaxmys( a, x, y )   bli_zaxmyris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_szzaxmys( a, x, y )  bli_dzaxmyris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzzaxmys( a, x, y )  bli_dzaxmyris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czzaxmys( a, x, y )   bli_zaxmyris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzzaxmys( a, x, y )   bli_zaxmyris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_dscaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_cscaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zscaxmys( a, x, y )  { (y) -= (a) * (x); }

#define bli_sdcaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_ddcaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_cdcaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zdcaxmys( a, x, y )  { (y) -= (a) * (x); }

#define bli_sccaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_dccaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_cccaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zccaxmys( a, x, y )  { (y) -= (a) * (x); }

#define bli_szcaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_dzcaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_czcaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zzcaxmys( a, x, y )  { (y) -= (a) * (x); }

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_dszaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_cszaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zszaxmys( a, x, y )  { (y) -= (a) * (x); }

#define bli_sdzaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_ddzaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_cdzaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zdzaxmys( a, x, y )  { (y) -= (a) * (x); }

#define bli_sczaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_dczaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_cczaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zczaxmys( a, x, y )  { (y) -= (a) * (x); }

#define bli_szzaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_dzzaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_czzaxmys( a, x, y )  { (y) -= (a) * (x); }
#define bli_zzzaxmys( a, x, y )  { (y) -= (a) * (x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_saxmys( a, x, y )  bli_sssaxmys( a, x, y )
#define bli_daxmys( a, x, y )  bli_dddaxmys( a, x, y )
#define bli_caxmys( a, x, y )  bli_cccaxmys( a, x, y )
#define bli_zaxmys( a, x, y )  bli_zzzaxmys( a, x, y )


#endif

// end bli_axmys.h

// begin bli_cast.h


#ifndef BLIS_CAST_H
#define BLIS_CAST_H

// cast

// Notes:
// - The first char encodes the type of *ap.
// - The second char encodes the type of b.


#define bli_sscast( ap, b ) \
{ \
	(b) = ( float  )             *(( float*    )(ap)); \
}
#define bli_dscast( ap, b ) \
{ \
	(b) = ( float  )             *(( double*   )(ap)); \
}
#define bli_cscast( ap, b ) \
{ \
	(b) = ( float  )  bli_creal( *(( scomplex* )(ap)) ); \
}
#define bli_zscast( ap, b ) \
{ \
	(b) = ( float  )  bli_zreal( *(( dcomplex* )(ap)) ); \
}


#define bli_sdcast( ap, b ) \
{ \
	(b) = ( double )             *(( float*    )(ap)); \
}
#define bli_ddcast( ap, b ) \
{ \
	(b) = ( double )             *(( double*   )(ap)); \
}
#define bli_cdcast( ap, b ) \
{ \
	(b) = ( double )  bli_creal( *(( scomplex* )(ap)) ); \
}
#define bli_zdcast( ap, b ) \
{ \
	(b) = ( double )  bli_zreal( *(( dcomplex* )(ap)) ); \
}


#ifndef BLIS_ENABLE_C99_COMPLEX


#define bli_sccast( ap, b ) \
{ \
	bli_scsets( bli_sreal( *(( float*    )(ap)) ), \
	                                          0.0, (b) ); \
}
#define bli_dccast( ap, b ) \
{ \
	bli_dcsets( bli_dreal( *(( double*   )(ap)) ), \
	                                          0.0, (b) ); \
}
#define bli_cccast( ap, b ) \
{ \
	bli_ccsets( bli_creal( *(( scomplex* )(ap)) ), \
	            bli_cimag( *(( scomplex* )(ap)) ), (b) ); \
}
#define bli_zccast( ap, b ) \
{ \
	bli_zcsets( bli_zreal( *(( dcomplex* )(ap)) ), \
	            bli_zimag( *(( dcomplex* )(ap)) ), (b) ); \
}


#define bli_szcast( ap, b ) \
{ \
	bli_szsets( bli_sreal( *(( float*    )(ap)) ), \
	                                          0.0, (b) ); \
}
#define bli_dzcast( ap, b ) \
{ \
	bli_dzsets( bli_dreal( *(( double*   )(ap)) ), \
	                                          0.0, (b) ); \
}
#define bli_czcast( ap, b ) \
{ \
	bli_czsets( bli_creal( *(( scomplex* )(ap)) ), \
	            bli_cimag( *(( scomplex* )(ap)) ), (b) ); \
}
#define bli_zzcast( ap, b ) \
{ \
	bli_zzsets( bli_zreal( *(( dcomplex* )(ap)) ), \
	            bli_zimag( *(( dcomplex* )(ap)) ), (b) ); \
}


#else // ifdef BLIS_ENABLE_C99_COMPLEX


#define bli_sccast( ap, b )  { (b) = ( scomplex ) *(( float*    )(ap)); }
#define bli_dccast( ap, b )  { (b) = ( scomplex ) *(( double*   )(ap)); }
#define bli_cccast( ap, b )  { (b) = ( scomplex ) *(( scomplex* )(ap)); }
#define bli_zccast( ap, b )  { (b) = ( scomplex ) *(( dcomplex* )(ap)); }

#define bli_szcast( ap, b )  { (b) = ( dcomplex ) *(( float*    )(ap)); }
#define bli_dzcast( ap, b )  { (b) = ( dcomplex ) *(( double*   )(ap)); }
#define bli_czcast( ap, b )  { (b) = ( dcomplex ) *(( scomplex* )(ap)); }
#define bli_zzcast( ap, b )  { (b) = ( dcomplex ) *(( dcomplex* )(ap)); }


#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_scast( ap, b )  bli_sscast( ap, b )
#define bli_dcast( ap, b )  bli_ddcast( ap, b )
#define bli_ccast( ap, b )  bli_cccast( ap, b )
#define bli_zcast( ap, b )  bli_zzcast( ap, b )

#endif
// end bli_cast.h

// begin bli_conjs.h


#ifndef BLIS_CONJS_H
#define BLIS_CONJS_H

// conjs

#define bli_sconjs( x )  bli_sconjris( bli_sreal(x), bli_simag(x) )
#define bli_dconjs( x )  bli_dconjris( bli_dreal(x), bli_dimag(x) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_cconjs( x )  bli_cconjris( bli_creal(x), bli_cimag(x) )
#define bli_zconjs( x )  bli_zconjris( bli_zreal(x), bli_zimag(x) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_cconjs( x )  { (x) = conjf(x); }
#define bli_zconjs( x )  { (x) = conj (x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#endif

// end bli_conjs.h

// begin bli_copys.h


#ifndef BLIS_COPYS_H
#define BLIS_COPYS_H

// copys

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sscopys( x, y )  bli_scopyris( bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dscopys( x, y )  bli_scopyris( bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cscopys( x, y )  bli_scopyris( bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zscopys( x, y )  bli_scopyris( bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdcopys( x, y )  bli_dcopyris( bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ddcopys( x, y )  bli_dcopyris( bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cdcopys( x, y )  bli_dcopyris( bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zdcopys( x, y )  bli_dcopyris( bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_sccopys( x, y )  bli_ccopyris( bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccopys( x, y )  bli_ccopyris( bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccopys( x, y )  bli_ccopyris( bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccopys( x, y )  bli_ccopyris( bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcopys( x, y )  bli_zcopyris( bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzcopys( x, y )  bli_zcopyris( bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czcopys( x, y )  bli_zcopyris( bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzcopys( x, y )  bli_zcopyris( bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_sccopys( x, y )  { (y) = (x); }
#define bli_dccopys( x, y )  { (y) = (x); }
#define bli_cccopys( x, y )  { (y) = (x); }
#define bli_zccopys( x, y )  { (y) = (x); }

#define bli_szcopys( x, y )  { (y) = (x); }
#define bli_dzcopys( x, y )  { (y) = (x); }
#define bli_czcopys( x, y )  { (y) = (x); }
#define bli_zzcopys( x, y )  { (y) = (x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_iicopys( x, y )  { (y) = ( gint_t ) (x); }


#define bli_scopys( x, y )  bli_sscopys( x, y )
#define bli_dcopys( x, y )  bli_ddcopys( x, y )
#define bli_ccopys( x, y )  bli_cccopys( x, y )
#define bli_zcopys( x, y )  bli_zzcopys( x, y )
#define bli_icopys( x, y )  bli_iicopys( x, y )


#endif

// end bli_copys.h
// begin bli_copyjs.h


#ifndef BLIS_COPYJS_H
#define BLIS_COPYJS_H

// copyjs

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sscopyjs( x, y )  bli_scopyjris( bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dscopyjs( x, y )  bli_scopyjris( bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cscopyjs( x, y )  bli_scopyjris( bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zscopyjs( x, y )  bli_scopyjris( bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdcopyjs( x, y )  bli_dcopyjris( bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ddcopyjs( x, y )  bli_dcopyjris( bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cdcopyjs( x, y )  bli_dcopyjris( bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zdcopyjs( x, y )  bli_dcopyjris( bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_sccopyjs( x, y )  bli_ccopyjris( bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccopyjs( x, y )  bli_ccopyjris( bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccopyjs( x, y )  bli_ccopyjris( bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccopyjs( x, y )  bli_ccopyjris( bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcopyjs( x, y )  bli_zcopyjris( bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzcopyjs( x, y )  bli_zcopyjris( bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czcopyjs( x, y )  bli_zcopyjris( bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzcopyjs( x, y )  bli_zcopyjris( bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_sccopyjs( x, y )  { (y) =      (x); }
#define bli_dccopyjs( x, y )  { (y) =      (x); }
#define bli_cccopyjs( x, y )  { (y) = conjf(x); }
#define bli_zccopyjs( x, y )  { (y) = conj (x); }

#define bli_szcopyjs( x, y )  { (y) =      (x); }
#define bli_dzcopyjs( x, y )  { (y) =      (x); }
#define bli_czcopyjs( x, y )  { (y) = conjf(x); }
#define bli_zzcopyjs( x, y )  { (y) = conj (x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_iicopyjs( x, y )  { (y) = ( gint_t ) (x); }


#define bli_scopyjs( x, y )  bli_sscopyjs( x, y )
#define bli_dcopyjs( x, y )  bli_ddcopyjs( x, y )
#define bli_ccopyjs( x, y )  bli_cccopyjs( x, y )
#define bli_zcopyjs( x, y )  bli_zzcopyjs( x, y )
#define bli_icopyjs( x, y )  bli_iicopyjs( x, y )


#endif

// end bli_copyjs.h
// begin bli_copycjs.h


#ifndef BLIS_COPYCJS_H
#define BLIS_COPYCJS_H

// copycjs

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sscopycjs( conjx, x, y )  bli_scopycjris( conjx, bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dscopycjs( conjx, x, y )  bli_scopycjris( conjx, bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cscopycjs( conjx, x, y )  bli_scopycjris( conjx, bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zscopycjs( conjx, x, y )  bli_scopycjris( conjx, bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdcopycjs( conjx, x, y )  bli_dcopycjris( conjx, bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ddcopycjs( conjx, x, y )  bli_dcopycjris( conjx, bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cdcopycjs( conjx, x, y )  bli_dcopycjris( conjx, bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zdcopycjs( conjx, x, y )  bli_dcopycjris( conjx, bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_sccopycjs( conjx, x, y )  bli_ccopycjris( conjx, bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccopycjs( conjx, x, y )  bli_ccopycjris( conjx, bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccopycjs( conjx, x, y )  bli_ccopycjris( conjx, bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccopycjs( conjx, x, y )  bli_ccopycjris( conjx, bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcopycjs( conjx, x, y )  bli_zcopycjris( conjx, bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzcopycjs( conjx, x, y )  bli_zcopycjris( conjx, bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czcopycjs( conjx, x, y )  bli_zcopycjris( conjx, bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzcopycjs( conjx, x, y )  bli_zcopycjris( conjx, bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_sccopycjs( conjx, x, y )  { (y) = (x); }
#define bli_dccopycjs( conjx, x, y )  { (y) = (x); }
#define bli_cccopycjs( conjx, x, y )  { (y) = ( bli_is_conj( conjx ) ? conjf(x) : (x) ); }
#define bli_zccopycjs( conjx, x, y )  { (y) = ( bli_is_conj( conjx ) ? conj (x) : (x) ); }

#define bli_szcopycjs( conjx, x, y )  { (y) = (x); }
#define bli_dzcopycjs( conjx, x, y )  { (y) = (x); }
#define bli_czcopycjs( conjx, x, y )  { (y) = ( bli_is_conj( conjx ) ? conjf(x) : (x) ); }
#define bli_zzcopycjs( conjx, x, y )  { (y) = ( bli_is_conj( conjx ) ? conj (x) : (x) ); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_iicopycjs( conjx, x, y )  { (y) = ( gint_t ) (x); }


#define bli_scopycjs( conjx, x, y )  bli_sscopycjs( conjx, x, y )
#define bli_dcopycjs( conjx, x, y )  bli_ddcopycjs( conjx, x, y )
#define bli_ccopycjs( conjx, x, y )  bli_cccopycjs( conjx, x, y )
#define bli_zcopycjs( conjx, x, y )  bli_zzcopycjs( conjx, x, y )
#define bli_icopycjs( conjx, x, y )  bli_iicopycjs( conjx, x, y )


#endif

// end bli_copycjs.h

// begin bli_dots.h


#ifndef BLIS_DOTS_H
#define BLIS_DOTS_H

// dots

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.
// - The third char encodes the type of rho.


#define bli_sssdots( x, y, a )  bli_sssaxpys( x, y, a )
#define bli_dssdots( x, y, a )  bli_dssaxpys( x, y, a )
#define bli_cssdots( x, y, a )  bli_cssaxpys( x, y, a )
#define bli_zssdots( x, y, a )  bli_zssaxpys( x, y, a )

#define bli_sdsdots( x, y, a )  bli_sdsaxpys( x, y, a )
#define bli_ddsdots( x, y, a )  bli_ddsaxpys( x, y, a )
#define bli_cdsdots( x, y, a )  bli_cdsaxpys( x, y, a )
#define bli_zdsdots( x, y, a )  bli_zdsaxpys( x, y, a )

#define bli_scsdots( x, y, a )  bli_scsaxpys( x, y, a )
#define bli_dcsdots( x, y, a )  bli_dcsaxpys( x, y, a )
#define bli_ccsdots( x, y, a )  bli_ccsaxpys( x, y, a )
#define bli_zcsdots( x, y, a )  bli_zcsaxpys( x, y, a )

#define bli_szsdots( x, y, a )  bli_szsaxpys( x, y, a )
#define bli_dzsdots( x, y, a )  bli_dzsaxpys( x, y, a )
#define bli_czsdots( x, y, a )  bli_czsaxpys( x, y, a )
#define bli_zzsdots( x, y, a )  bli_zzsaxpys( x, y, a )



#define bli_ssddots( x, y, a )  bli_ssdaxpys( x, y, a )
#define bli_dsddots( x, y, a )  bli_dsdaxpys( x, y, a )
#define bli_csddots( x, y, a )  bli_csdaxpys( x, y, a )
#define bli_zsddots( x, y, a )  bli_zsdaxpys( x, y, a )

#define bli_sdddots( x, y, a )  bli_sddaxpys( x, y, a )
#define bli_ddddots( x, y, a )  bli_dddaxpys( x, y, a )
#define bli_cdddots( x, y, a )  bli_cddaxpys( x, y, a )
#define bli_zdddots( x, y, a )  bli_zddaxpys( x, y, a )

#define bli_scddots( x, y, a )  bli_scdaxpys( x, y, a )
#define bli_dcddots( x, y, a )  bli_dcdaxpys( x, y, a )
#define bli_ccddots( x, y, a )  bli_ccdaxpys( x, y, a )
#define bli_zcddots( x, y, a )  bli_zcdaxpys( x, y, a )

#define bli_szddots( x, y, a )  bli_szdaxpys( x, y, a )
#define bli_dzddots( x, y, a )  bli_dzdaxpys( x, y, a )
#define bli_czddots( x, y, a )  bli_czdaxpys( x, y, a )
#define bli_zzddots( x, y, a )  bli_zzdaxpys( x, y, a )



#define bli_sscdots( x, y, a )  bli_sscaxpys( x, y, a )
#define bli_dscdots( x, y, a )  bli_dscaxpys( x, y, a )
#define bli_cscdots( x, y, a )  bli_cscaxpys( x, y, a )
#define bli_zscdots( x, y, a )  bli_zscaxpys( x, y, a )

#define bli_sdcdots( x, y, a )  bli_sdcaxpys( x, y, a )
#define bli_ddcdots( x, y, a )  bli_ddcaxpys( x, y, a )
#define bli_cdcdots( x, y, a )  bli_cdcaxpys( x, y, a )
#define bli_zdcdots( x, y, a )  bli_zdcaxpys( x, y, a )

#define bli_sccdots( x, y, a )  bli_sccaxpys( x, y, a )
#define bli_dccdots( x, y, a )  bli_dccaxpys( x, y, a )
#define bli_cccdots( x, y, a )  bli_cccaxpys( x, y, a )
#define bli_zccdots( x, y, a )  bli_zccaxpys( x, y, a )

#define bli_szcdots( x, y, a )  bli_szcaxpys( x, y, a )
#define bli_dzcdots( x, y, a )  bli_dzcaxpys( x, y, a )
#define bli_czcdots( x, y, a )  bli_czcaxpys( x, y, a )
#define bli_zzcdots( x, y, a )  bli_zzcaxpys( x, y, a )



#define bli_sszdots( x, y, a )  bli_sszaxpys( x, y, a )
#define bli_dszdots( x, y, a )  bli_dszaxpys( x, y, a )
#define bli_cszdots( x, y, a )  bli_cszaxpys( x, y, a )
#define bli_zszdots( x, y, a )  bli_zszaxpys( x, y, a )

#define bli_sdzdots( x, y, a )  bli_sdzaxpys( x, y, a )
#define bli_ddzdots( x, y, a )  bli_ddzaxpys( x, y, a )
#define bli_cdzdots( x, y, a )  bli_cdzaxpys( x, y, a )
#define bli_zdzdots( x, y, a )  bli_zdzaxpys( x, y, a )

#define bli_sczdots( x, y, a )  bli_sczaxpys( x, y, a )
#define bli_dczdots( x, y, a )  bli_dczaxpys( x, y, a )
#define bli_cczdots( x, y, a )  bli_cczaxpys( x, y, a )
#define bli_zczdots( x, y, a )  bli_zczaxpys( x, y, a )

#define bli_szzdots( x, y, a )  bli_szzaxpys( x, y, a )
#define bli_dzzdots( x, y, a )  bli_dzzaxpys( x, y, a )
#define bli_czzdots( x, y, a )  bli_czzaxpys( x, y, a )
#define bli_zzzdots( x, y, a )  bli_zzzaxpys( x, y, a )



#define bli_sdots( x, y, a )  bli_sssdots( x, y, a )
#define bli_ddots( x, y, a )  bli_ddddots( x, y, a )
#define bli_cdots( x, y, a )  bli_cccdots( x, y, a )
#define bli_zdots( x, y, a )  bli_zzzdots( x, y, a )


#endif

// end bli_dots.h
// begin bli_dotjs.h


#ifndef BLIS_DOTJS_H
#define BLIS_DOTJS_H

// dotjs

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.
// - The third char encodes the type of rho.
// - x is used in conjugated form.


#define bli_sssdotjs( x, y, a )  bli_sssaxpyjs( y, x, a )
#define bli_dssdotjs( x, y, a )  bli_sdsaxpyjs( y, x, a )
#define bli_cssdotjs( x, y, a )  bli_scsaxpyjs( y, x, a )
#define bli_zssdotjs( x, y, a )  bli_szsaxpyjs( y, x, a )

#define bli_sdsdotjs( x, y, a )  bli_dssaxpyjs( y, x, a )
#define bli_ddsdotjs( x, y, a )  bli_ddsaxpyjs( y, x, a )
#define bli_cdsdotjs( x, y, a )  bli_dcsaxpyjs( y, x, a )
#define bli_zdsdotjs( x, y, a )  bli_dzsaxpyjs( y, x, a )

#define bli_scsdotjs( x, y, a )  bli_cssaxpyjs( y, x, a )
#define bli_dcsdotjs( x, y, a )  bli_cdsaxpyjs( y, x, a )
#define bli_ccsdotjs( x, y, a )  bli_ccsaxpyjs( y, x, a )
#define bli_zcsdotjs( x, y, a )  bli_czsaxpyjs( y, x, a )

#define bli_szsdotjs( x, y, a )  bli_zssaxpyjs( y, x, a )
#define bli_dzsdotjs( x, y, a )  bli_zdsaxpyjs( y, x, a )
#define bli_czsdotjs( x, y, a )  bli_zcsaxpyjs( y, x, a )
#define bli_zzsdotjs( x, y, a )  bli_zzsaxpyjs( y, x, a )


#define bli_ssddotjs( x, y, a )  bli_ssdaxpyjs( y, x, a )
#define bli_dsddotjs( x, y, a )  bli_sddaxpyjs( y, x, a )
#define bli_csddotjs( x, y, a )  bli_scdaxpyjs( y, x, a )
#define bli_zsddotjs( x, y, a )  bli_szdaxpyjs( y, x, a )

#define bli_sdddotjs( x, y, a )  bli_dsdaxpyjs( y, x, a )
#define bli_ddddotjs( x, y, a )  bli_dddaxpyjs( y, x, a )
#define bli_cdddotjs( x, y, a )  bli_dcdaxpyjs( y, x, a )
#define bli_zdddotjs( x, y, a )  bli_dzdaxpyjs( y, x, a )

#define bli_scddotjs( x, y, a )  bli_csdaxpyjs( y, x, a )
#define bli_dcddotjs( x, y, a )  bli_cddaxpyjs( y, x, a )
#define bli_ccddotjs( x, y, a )  bli_ccdaxpyjs( y, x, a )
#define bli_zcddotjs( x, y, a )  bli_czdaxpyjs( y, x, a )

#define bli_szddotjs( x, y, a )  bli_zsdaxpyjs( y, x, a )
#define bli_dzddotjs( x, y, a )  bli_zddaxpyjs( y, x, a )
#define bli_czddotjs( x, y, a )  bli_zcdaxpyjs( y, x, a )
#define bli_zzddotjs( x, y, a )  bli_zzdaxpyjs( y, x, a )


#define bli_sscdotjs( x, y, a )  bli_sscaxpyjs( y, x, a )
#define bli_dscdotjs( x, y, a )  bli_sdcaxpyjs( y, x, a )
#define bli_cscdotjs( x, y, a )  bli_sccaxpyjs( y, x, a )
#define bli_zscdotjs( x, y, a )  bli_szcaxpyjs( y, x, a )

#define bli_sdcdotjs( x, y, a )  bli_dscaxpyjs( y, x, a )
#define bli_ddcdotjs( x, y, a )  bli_ddcaxpyjs( y, x, a )
#define bli_cdcdotjs( x, y, a )  bli_dccaxpyjs( y, x, a )
#define bli_zdcdotjs( x, y, a )  bli_dzcaxpyjs( y, x, a )

#define bli_sccdotjs( x, y, a )  bli_cscaxpyjs( y, x, a )
#define bli_dccdotjs( x, y, a )  bli_cdcaxpyjs( y, x, a )
#define bli_cccdotjs( x, y, a )  bli_cccaxpyjs( y, x, a )
#define bli_zccdotjs( x, y, a )  bli_czcaxpyjs( y, x, a )

#define bli_szcdotjs( x, y, a )  bli_zscaxpyjs( y, x, a )
#define bli_dzcdotjs( x, y, a )  bli_zdcaxpyjs( y, x, a )
#define bli_czcdotjs( x, y, a )  bli_zccaxpyjs( y, x, a )
#define bli_zzcdotjs( x, y, a )  bli_zzcaxpyjs( y, x, a )


#define bli_sszdotjs( x, y, a )  bli_sszaxpyjs( y, x, a )
#define bli_dszdotjs( x, y, a )  bli_sdzaxpyjs( y, x, a )
#define bli_cszdotjs( x, y, a )  bli_sczaxpyjs( y, x, a )
#define bli_zszdotjs( x, y, a )  bli_szzaxpyjs( y, x, a )

#define bli_sdzdotjs( x, y, a )  bli_dszaxpyjs( y, x, a )
#define bli_ddzdotjs( x, y, a )  bli_ddzaxpyjs( y, x, a )
#define bli_cdzdotjs( x, y, a )  bli_dczaxpyjs( y, x, a )
#define bli_zdzdotjs( x, y, a )  bli_dzzaxpyjs( y, x, a )

#define bli_sczdotjs( x, y, a )  bli_cszaxpyjs( y, x, a )
#define bli_dczdotjs( x, y, a )  bli_cdzaxpyjs( y, x, a )
#define bli_cczdotjs( x, y, a )  bli_cczaxpyjs( y, x, a )
#define bli_zczdotjs( x, y, a )  bli_czzaxpyjs( y, x, a )

#define bli_szzdotjs( x, y, a )  bli_zszaxpyjs( y, x, a )
#define bli_dzzdotjs( x, y, a )  bli_zdzaxpyjs( y, x, a )
#define bli_czzdotjs( x, y, a )  bli_zczaxpyjs( y, x, a )
#define bli_zzzdotjs( x, y, a )  bli_zzzaxpyjs( y, x, a )





#define bli_sdotjs( x, y, a )  bli_sssdotjs( x, y, a )
#define bli_ddotjs( x, y, a )  bli_ddddotjs( x, y, a )
#define bli_cdotjs( x, y, a )  bli_cccdotjs( x, y, a )
#define bli_zdotjs( x, y, a )  bli_zzzdotjs( x, y, a )


#endif

// end bli_dotjs.h

// begin bli_eq.h


#ifndef BLIS_EQ_H
#define BLIS_EQ_H


// eq (passed by value)

#define bli_seq( a, b )  ( (a) == (b) )
#define bli_deq( a, b )  ( (a) == (b) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_ceq( a, b )  ( ( bli_creal(a) == bli_creal(b) ) && ( bli_cimag(a) == bli_cimag(b) ) )
#define bli_zeq( a, b )  ( ( bli_zreal(a) == bli_zreal(b) ) && ( bli_zimag(a) == bli_zimag(b) ) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_ceq( a, b )  ( (a) == (b) )
#define bli_zeq( a, b )  ( (a) == (b) )

#endif // BLIS_ENABLE_C99_COMPLEX

#define bli_ieq( a, b )  ( (a) == (b) )



// eqtori (passed by value)

#define bli_seqtori( a, br, bi )  ( (a) == (br) )
#define bli_deqtori( a, br, bi )  ( (a) == (br) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_ceqtori( a, br, bi )  ( ( bli_creal(a) == (br) ) && ( bli_cimag(a) == (bi) ) )
#define bli_zeqtori( a, br, bi )  ( ( bli_zreal(a) == (br) ) && ( bli_zimag(a) == (bi) ) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_ceqtori( a, br, bi )  ( (a) == (br) + (bi) * (I) )
#define bli_zeqtori( a, br, bi )  ( (a) == (br) + (bi) * (I) )

#endif // BLIS_ENABLE_C99_COMPLEX



// eqa (passed by address)

#define bli_seqa( a, b )  bli_seq( *(( float*    )(a)), *(( float*    )(b)) )
#define bli_deqa( a, b )  bli_deq( *(( double*   )(a)), *(( double*   )(b)) )
#define bli_ceqa( a, b )  bli_ceq( *(( scomplex* )(a)), *(( scomplex* )(b)) )
#define bli_zeqa( a, b )  bli_zeq( *(( dcomplex* )(a)), *(( dcomplex* )(b)) )
#define bli_ieqa( a, b )  bli_ieq( *(( gint_t*   )(a)), *(( gint_t*   )(b)) )



// eq1

#define bli_seq1( a )  bli_seqtori( (a), 1.0F, 0.0F )
#define bli_deq1( a )  bli_deqtori( (a), 1.0,  0.0  )
#define bli_ceq1( a )  bli_ceqtori( (a), 1.0F, 0.0F )
#define bli_zeq1( a )  bli_zeqtori( (a), 1.0,  0.0  )
#define bli_ieq1( a )  bli_ieq    ( (a), 1          )



// eq0

#define bli_seq0( a )  bli_seqtori( (a), 0.0F, 0.0F )
#define bli_deq0( a )  bli_deqtori( (a), 0.0,  0.0  )
#define bli_ceq0( a )  bli_ceqtori( (a), 0.0F, 0.0F )
#define bli_zeq0( a )  bli_zeqtori( (a), 0.0,  0.0  )
#define bli_ieq0( a )  bli_ieq    ( (a), 0          )



// eqm1

#define bli_seqm1( a )  bli_seqtori( (a), -1.0F, 0.0F )
#define bli_deqm1( a )  bli_deqtori( (a), -1.0,  0.0  )
#define bli_ceqm1( a )  bli_ceqtori( (a), -1.0F, 0.0F )
#define bli_zeqm1( a )  bli_zeqtori( (a), -1.0,  0.0  )
#define bli_ieqm1( a )  bli_ieq    ( (a), -1          )



#endif
// end bli_eq.h

// begin bli_fprints.h


#ifndef BLIS_FPRINTS_H
#define BLIS_FPRINTS_H

// prints

#define bli_sfprints( file, spec, x ) \
{ \
	fprintf( file, spec, (x) ); \
}
#define bli_dfprints( file, spec, x ) \
{ \
	fprintf( file, spec, (x) ); \
}
#define bli_cfprints( file, spec, x ) \
{ \
	fprintf( file, spec, bli_creal(x) ); \
	fprintf( file, " + " ); \
	fprintf( file, spec, bli_cimag(x) ); \
	fprintf( file, " " ); \
}
#define bli_zfprints( file, spec, x ) \
{ \
	fprintf( file, spec, bli_zreal(x) ); \
	fprintf( file, " + " ); \
	fprintf( file, spec, bli_zimag(x) ); \
	fprintf( file, " " ); \
}
#define bli_ifprints( file, spec, x ) \
{ \
	fprintf( file, spec, (x) ); \
}


#endif
// end bli_fprints.h

// begin bli_inverts.h


#ifndef BLIS_INVERTS_H
#define BLIS_INVERTS_H

// inverts

// Notes:
// - The first char encodes the type of x.

#define bli_sinverts( x )  bli_sinvertris( bli_sreal(x), bli_simag(x) )
#define bli_dinverts( x )  bli_dinvertris( bli_dreal(x), bli_dimag(x) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_cinverts( x )  bli_cinvertris( bli_creal(x), bli_cimag(x) )
#define bli_zinverts( x )  bli_zinvertris( bli_zreal(x), bli_zimag(x) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_cinverts( x )  { (x) = 1.0F / (x); }
#define bli_zinverts( x )  { (x) = 1.0  / (x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#endif

// end bli_inverts.h

// begin bli_invscals.h


#ifndef BLIS_INVSCALS_H
#define BLIS_INVSCALS_H

// invscals

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_ssinvscals( a, y )  bli_sinvscalris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dsinvscals( a, y )  bli_sinvscalris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_csinvscals( a, y )  bli_sinvscalris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zsinvscals( a, y )  bli_sinvscalris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdinvscals( a, y )  bli_dinvscalris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddinvscals( a, y )  bli_dinvscalris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdinvscals( a, y )  bli_dinvscalris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdinvscals( a, y )  bli_dinvscalris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scinvscals( a, y )  bli_scinvscalris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcinvscals( a, y )  bli_scinvscalris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccinvscals( a, y )   bli_cinvscalris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcinvscals( a, y )   bli_cinvscalris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szinvscals( a, y )  bli_dzinvscalris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzinvscals( a, y )  bli_dzinvscalris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czinvscals( a, y )   bli_zinvscalris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzinvscals( a, y )   bli_zinvscalris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scinvscals( a, y )  { (y) /= (a); }
#define bli_dcinvscals( a, y )  { (y) /= (a); }
#define bli_ccinvscals( a, y )  { (y) /= (a); }
#define bli_zcinvscals( a, y )  { (y) /= (a); }

#define bli_szinvscals( a, y )  { (y) /= (a); }
#define bli_dzinvscals( a, y )  { (y) /= (a); }
#define bli_czinvscals( a, y )  { (y) /= (a); }
#define bli_zzinvscals( a, y )  { (y) /= (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sinvscals( a, y )  bli_ssinvscals( a, y )
#define bli_dinvscals( a, y )  bli_ddinvscals( a, y )
#define bli_cinvscals( a, y )  bli_ccinvscals( a, y )
#define bli_zinvscals( a, y )  bli_zzinvscals( a, y )


#endif

// end bli_invscals.h
// begin bli_invscaljs.h


#ifndef BLIS_INVSCALJS_H
#define BLIS_INVSCALJS_H

// invscaljs

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_ssinvscaljs( a, y )  bli_sinvscaljris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dsinvscaljs( a, y )  bli_sinvscaljris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_csinvscaljs( a, y )  bli_sinvscaljris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zsinvscaljs( a, y )  bli_sinvscaljris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdinvscaljs( a, y )  bli_dinvscaljris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddinvscaljs( a, y )  bli_dinvscaljris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdinvscaljs( a, y )  bli_dinvscaljris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdinvscaljs( a, y )  bli_dinvscaljris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scinvscaljs( a, y )  bli_scinvscaljris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcinvscaljs( a, y )  bli_scinvscaljris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccinvscaljs( a, y )   bli_cinvscaljris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcinvscaljs( a, y )   bli_cinvscaljris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szinvscaljs( a, y )  bli_dzinvscaljris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzinvscaljs( a, y )  bli_dzinvscaljris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czinvscaljs( a, y )   bli_zinvscaljris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzinvscaljs( a, y )   bli_zinvscaljris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scinvscaljs( a, y )  { (y) /=      (a); }
#define bli_dcinvscaljs( a, y )  { (y) /=      (a); }
#define bli_ccinvscaljs( a, y )  { (y) /= conjf(a); }
#define bli_zcinvscaljs( a, y )  { (y) /= conj (a); }

#define bli_szinvscaljs( a, y )  { (y) /=      (a); }
#define bli_dzinvscaljs( a, y )  { (y) /=      (a); }
#define bli_czinvscaljs( a, y )  { (y) /= conjf(a); }
#define bli_zzinvscaljs( a, y )  { (y) /= conj (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sinvscaljs( a, y )  bli_ssinvscaljs( a, y )
#define bli_dinvscaljs( a, y )  bli_ddinvscaljs( a, y )
#define bli_cinvscaljs( a, y )  bli_ccinvscaljs( a, y )
#define bli_zinvscaljs( a, y )  bli_zzinvscaljs( a, y )


#endif

// end bli_invscaljs.h

// begin bli_neg2s.h


#ifndef BLIS_NEG2S_H
#define BLIS_NEG2S_H

// neg2s

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_ssneg2s( x, y )  bli_sneg2ris( bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dsneg2s( x, y )  bli_sneg2ris( bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_csneg2s( x, y )  bli_sneg2ris( bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zsneg2s( x, y )  bli_sneg2ris( bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdneg2s( x, y )  bli_dneg2ris( bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ddneg2s( x, y )  bli_dneg2ris( bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cdneg2s( x, y )  bli_dneg2ris( bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zdneg2s( x, y )  bli_dneg2ris( bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scneg2s( x, y )  bli_cneg2ris( bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dcneg2s( x, y )  bli_cneg2ris( bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_ccneg2s( x, y )  bli_cneg2ris( bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zcneg2s( x, y )  bli_cneg2ris( bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szneg2s( x, y )  bli_zneg2ris( bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzneg2s( x, y )  bli_zneg2ris( bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czneg2s( x, y )  bli_zneg2ris( bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzneg2s( x, y )  bli_zneg2ris( bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scneg2s( x, y )  { (y) = -(x); }
#define bli_dcneg2s( x, y )  { (y) = -(x); }
#define bli_ccneg2s( x, y )  { (y) = -(x); }
#define bli_zcneg2s( x, y )  { (y) = -(x); }

#define bli_szneg2s( x, y )  { (y) = -(x); }
#define bli_dzneg2s( x, y )  { (y) = -(x); }
#define bli_czneg2s( x, y )  { (y) = -(x); }
#define bli_zzneg2s( x, y )  { (y) = -(x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sneg2s( x, y )  bli_ssneg2s( x, y )
#define bli_dneg2s( x, y )  bli_ddneg2s( x, y )
#define bli_cneg2s( x, y )  bli_ccneg2s( x, y )
#define bli_zneg2s( x, y )  bli_zzneg2s( x, y )


#endif

// end bli_neg2s.h

// begin bli_rands.h


#ifndef BLIS_RANDS_H
#define BLIS_RANDS_H

// rands


#define bli_srands( a ) \
{ \
	(a) = ( float  ) (   ( double ) rand() / \
	                   ( ( double ) RAND_MAX / 2.0 ) \
	                 ) - 1.0F; \
}
#define bli_drands( a ) \
{ \
	(a) = ( double ) (   ( double ) rand() / \
	                   ( ( double ) RAND_MAX / 2.0 ) \
	                 ) - 1.0; \
}
#define bli_crands( a ) \
{ \
	float  ar, ai; \
\
	bli_srands( ar ); \
	bli_srands( ai ); \
\
	bli_csets( ar, ai, (a) ); \
}
#define bli_zrands( a ) \
{ \
	double ar, ai; \
\
	bli_drands( ar ); \
	bli_drands( ai ); \
\
	bli_zsets( ar, ai, (a) ); \
}


#endif

// end bli_rands.h
// begin bli_randnp2s.h


#ifndef BLIS_RANDNP2S_H
#define BLIS_RANDNP2S_H

// randnp2s


#define bli_srandnp2s( a ) \
{ \
	bli_drandnp2s( a ); \
}
#define bli_drandnp2s_prev( a ) \
{ \
	const double m_max  = 3.0; \
	const double m_max2 = m_max + 2.0; \
	double       t; \
	double       r_val; \
\
	 \
\
	 \
	t = ( ( double ) rand() / ( double ) RAND_MAX ) * m_max2; \
\
	 \
	if ( t == m_max2 ) t = t - 1.0; \
\
	 \
	t = floor( t ); \
\
	 \
	if ( t == 0.0 ) r_val = 0.0; \
	else \
	{ \
		 \
\
		double s_exp, s_val; \
\
		 \
		PASTEMAC(d,rands)( s_exp ); \
		PASTEMAC(d,rands)( s_val ); \
\
		 \
		if ( s_exp < 0.0 ) r_val = pow( 2.0, -(t - 1.0) ); \
		else               r_val = pow( 2.0,   t - 1.0  ); \
\
		 \
		if ( s_val < 0.0 ) r_val = -r_val; \
	} \
\
	 \
	r_val = r_val / pow( 2.0, m_max ); \
\
	 \
	 \
	a = r_val; \
}
#define bli_drandnp2s( a ) \
{ \
	const double m_max  = 6.0; \
	const double m_max2 = m_max + 2.0; \
	double       t; \
	double       r_val; \
\
	 \
\
	 \
	t = ( ( double ) rand() / ( double ) RAND_MAX ) * m_max2; \
\
	 \
	if ( t == m_max2 ) t = t - 1.0; \
\
	 \
	t = floor( t ); \
\
	 \
	if ( t == 0.0 ) r_val = 0.0; \
	else \
	{ \
		 \
\
		double s_val; \
\
		 \
		r_val = pow( 2.0, -(t - 1.0) ); \
\
		 \
		PASTEMAC(d,rands)( s_val ); \
\
		 \
		if ( s_val < 0.0 ) r_val = -r_val; \
	} \
\
	 \
	 \
	a = r_val; \
}
#define bli_crandnp2s( a ) \
{ \
	float  ar, ai; \
\
	bli_srandnp2s( ar ); \
	bli_srandnp2s( ai ); \
\
	bli_csets( ar, ai, (a) ); \
}
#define bli_zrandnp2s( a ) \
{ \
	double ar, ai; \
\
	bli_drandnp2s( ar ); \
	bli_drandnp2s( ai ); \
\
	bli_zsets( ar, ai, (a) ); \
}


#endif

// end bli_randnp2s.h

// begin bli_scals.h


#ifndef BLIS_SCALS_H
#define BLIS_SCALS_H

// scals

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_ssscals( a, y )  bli_sscalris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dsscals( a, y )  bli_sscalris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_csscals( a, y )  bli_sscalris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zsscals( a, y )  bli_sscalris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdscals( a, y )  bli_dscalris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddscals( a, y )  bli_dscalris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdscals( a, y )  bli_dscalris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdscals( a, y )  bli_dscalris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scscals( a, y )  bli_scscalris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcscals( a, y )  bli_scscalris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccscals( a, y )   bli_cscalris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcscals( a, y )   bli_cscalris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szscals( a, y )  bli_dzscalris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzscals( a, y )  bli_dzscalris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czscals( a, y )   bli_zscalris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzscals( a, y )   bli_zscalris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scscals( a, y )  { (y) *= (a); }
#define bli_dcscals( a, y )  { (y) *= (a); }
#define bli_ccscals( a, y )  { (y) *= (a); }
#define bli_zcscals( a, y )  { (y) *= (a); }

#define bli_szscals( a, y )  { (y) *= (a); }
#define bli_dzscals( a, y )  { (y) *= (a); }
#define bli_czscals( a, y )  { (y) *= (a); }
#define bli_zzscals( a, y )  { (y) *= (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sscals( a, y )  bli_ssscals( a, y )
#define bli_dscals( a, y )  bli_ddscals( a, y )
#define bli_cscals( a, y )  bli_ccscals( a, y )
#define bli_zscals( a, y )  bli_zzscals( a, y )


#endif

// end bli_scals.h
// begin bli_scaljs.h


#ifndef BLIS_SCALJS_H
#define BLIS_SCALJS_H

// scaljs

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_ssscaljs( a, y )  bli_sscaljris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dsscaljs( a, y )  bli_sscaljris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_csscaljs( a, y )  bli_sscaljris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zsscaljs( a, y )  bli_sscaljris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdscaljs( a, y )  bli_dscaljris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddscaljs( a, y )  bli_dscaljris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdscaljs( a, y )  bli_dscaljris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdscaljs( a, y )  bli_dscaljris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scscaljs( a, y )  bli_scscaljris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcscaljs( a, y )  bli_scscaljris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccscaljs( a, y )   bli_cscaljris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcscaljs( a, y )   bli_cscaljris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szscaljs( a, y )  bli_dzscaljris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzscaljs( a, y )  bli_dzscaljris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czscaljs( a, y )   bli_zscaljris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzscaljs( a, y )   bli_zscaljris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scscaljs( a, y )  { (y) *=      (a); }
#define bli_dcscaljs( a, y )  { (y) *=      (a); }
#define bli_ccscaljs( a, y )  { (y) *= conjf(a); }
#define bli_zcscaljs( a, y )  { (y) *= conj (a); }

#define bli_szscaljs( a, y )  { (y) *=      (a); }
#define bli_dzscaljs( a, y )  { (y) *=      (a); }
#define bli_czscaljs( a, y )  { (y) *= conjf(a); }
#define bli_zzscaljs( a, y )  { (y) *= conj (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sscaljs( a, y )  bli_ssscaljs( a, y )
#define bli_dscaljs( a, y )  bli_ddscaljs( a, y )
#define bli_cscaljs( a, y )  bli_ccscaljs( a, y )
#define bli_zscaljs( a, y )  bli_zzscaljs( a, y )


#endif

// end bli_scaljs.h
// begin bli_scalcjs.h


#ifndef BLIS_SCALCJS_H
#define BLIS_SCALCJS_H

// scalcjs

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_ssscalcjs( conjx, x, y )  bli_sscalcjris( conjx, bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dsscalcjs( conjx, x, y )  bli_sscalcjris( conjx, bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_csscalcjs( conjx, x, y )  bli_sscalcjris( conjx, bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zsscalcjs( conjx, x, y )  bli_sscalcjris( conjx, bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdscalcjs( conjx, x, y )  bli_dscalcjris( conjx, bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ddscalcjs( conjx, x, y )  bli_dscalcjris( conjx, bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cdscalcjs( conjx, x, y )  bli_dscalcjris( conjx, bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zdscalcjs( conjx, x, y )  bli_dscalcjris( conjx, bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scscalcjs( conjx, x, y )  bli_scscalcjris( conjx, bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dcscalcjs( conjx, x, y )  bli_scscalcjris( conjx, bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_ccscalcjs( conjx, x, y )   bli_cscalcjris( conjx, bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zcscalcjs( conjx, x, y )   bli_cscalcjris( conjx, bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szscalcjs( conjx, x, y )  bli_dzscalcjris( conjx, bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzscalcjs( conjx, x, y )  bli_dzscalcjris( conjx, bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czscalcjs( conjx, x, y )   bli_zscalcjris( conjx, bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzscalcjs( conjx, x, y )   bli_zscalcjris( conjx, bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scscalcjs( conjx, x, y )  { (y) *= (x); }
#define bli_dcscalcjs( conjx, x, y )  { (y) *= (x); }
#define bli_ccscalcjs( conjx, x, y )  { (y) *= ( bli_is_conj( conjx ) ? conjf(x) : (x) ); }
#define bli_zcscalcjs( conjx, x, y )  { (y) *= ( bli_is_conj( conjx ) ? conj (x) : (x) ); }

#define bli_szscalcjs( conjx, x, y )  { (y) *= (x); }
#define bli_dzscalcjs( conjx, x, y )  { (y) *= (x); }
#define bli_czscalcjs( conjx, x, y )  { (y) *= ( bli_is_conj( conjx ) ? conjf(x) : (x) ); }
#define bli_zzscalcjs( conjx, x, y )  { (y) *= ( bli_is_conj( conjx ) ? conj (x) : (x) ); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sscalcjs( conjx, x, y )  bli_ssscalcjs( conjx, x, y )
#define bli_dscalcjs( conjx, x, y )  bli_ddscalcjs( conjx, x, y )
#define bli_cscalcjs( conjx, x, y )  bli_ccscalcjs( conjx, x, y )
#define bli_zscalcjs( conjx, x, y )  bli_zzscalcjs( conjx, x, y )


#endif

// end bli_scalcjs.h

// begin bli_scal2s.h


#ifndef BLIS_SCAL2S_H
#define BLIS_SCAL2S_H

// scal2s

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of x.
// - The third char encodes the type of y.


// -- (axy) = (??s) ------------------------------------------------------------

#define bli_sssscal2s( a, x, y )  bli_sscal2ris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dssscal2s( a, x, y )  bli_sscal2ris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_cssscal2s( a, x, y )  bli_sscal2ris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_zssscal2s( a, x, y )  bli_sscal2ris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdsscal2s( a, x, y )  bli_sscal2ris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ddsscal2s( a, x, y )  bli_sscal2ris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cdsscal2s( a, x, y )  bli_sscal2ris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zdsscal2s( a, x, y )  bli_sscal2ris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )

#define bli_scsscal2s( a, x, y )  bli_sscal2ris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dcsscal2s( a, x, y )  bli_sscal2ris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ccsscal2s( a, x, y )  bli_sscal2ris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zcsscal2s( a, x, y )  bli_sscal2ris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )

#define bli_szsscal2s( a, x, y )  bli_sscal2ris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dzsscal2s( a, x, y )  bli_sscal2ris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_czsscal2s( a, x, y )  bli_sscal2ris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zzsscal2s( a, x, y )  bli_sscal2ris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

// -- (axy) = (??d) ------------------------------------------------------------

#define bli_ssdscal2s( a, x, y )  bli_dscal2ris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dsdscal2s( a, x, y )  bli_dscal2ris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_csdscal2s( a, x, y )  bli_dscal2ris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zsdscal2s( a, x, y )  bli_dscal2ris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )

#define bli_sddscal2s( a, x, y )  bli_dscal2ris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dddscal2s( a, x, y )  bli_dscal2ris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cddscal2s( a, x, y )  bli_dscal2ris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zddscal2s( a, x, y )  bli_dscal2ris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_scdscal2s( a, x, y )  bli_dscal2ris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dcdscal2s( a, x, y )  bli_dscal2ris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ccdscal2s( a, x, y )  bli_dscal2ris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zcdscal2s( a, x, y )  bli_dscal2ris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_szdscal2s( a, x, y )  bli_dscal2ris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dzdscal2s( a, x, y )  bli_dscal2ris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_czdscal2s( a, x, y )  bli_dscal2ris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zzdscal2s( a, x, y )  bli_dscal2ris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscscal2s( a, x, y )  bli_sscal2ris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dscscal2s( a, x, y )  bli_sscal2ris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_cscscal2s( a, x, y )  bli_cscal2ris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_zscscal2s( a, x, y )  bli_cscal2ris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )

#define bli_sdcscal2s( a, x, y )  bli_sscal2ris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_ddcscal2s( a, x, y )  bli_sscal2ris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cdcscal2s( a, x, y )  bli_cscal2ris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zdcscal2s( a, x, y )  bli_cscal2ris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )

#define bli_sccscal2s( a, x, y )  bli_scscal2ris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccscal2s( a, x, y )  bli_scscal2ris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccscal2s( a, x, y )   bli_cscal2ris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccscal2s( a, x, y )   bli_cscal2ris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcscal2s( a, x, y )  bli_scscal2ris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dzcscal2s( a, x, y )  bli_scscal2ris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_czcscal2s( a, x, y )   bli_cscal2ris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zzcscal2s( a, x, y )   bli_cscal2ris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszscal2s( a, x, y )  bli_dscal2ris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dszscal2s( a, x, y )  bli_dscal2ris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cszscal2s( a, x, y )  bli_zscal2ris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zszscal2s( a, x, y )  bli_zscal2ris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sdzscal2s( a, x, y )  bli_dscal2ris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_ddzscal2s( a, x, y )  bli_dscal2ris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cdzscal2s( a, x, y )  bli_zscal2ris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zdzscal2s( a, x, y )  bli_zscal2ris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sczscal2s( a, x, y )  bli_dzscal2ris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dczscal2s( a, x, y )  bli_dzscal2ris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cczscal2s( a, x, y )   bli_zscal2ris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zczscal2s( a, x, y )   bli_zscal2ris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_szzscal2s( a, x, y )  bli_dzscal2ris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzzscal2s( a, x, y )  bli_dzscal2ris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czzscal2s( a, x, y )   bli_zscal2ris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzzscal2s( a, x, y )   bli_zscal2ris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_dscscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_cscscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zscscal2s( a, x, y )  { (y) = (a) * (x); }

#define bli_sdcscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_ddcscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_cdcscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zdcscal2s( a, x, y )  { (y) = (a) * (x); }

#define bli_sccscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_dccscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_cccscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zccscal2s( a, x, y )  { (y) = (a) * (x); }

#define bli_szcscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_dzcscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_czcscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zzcscal2s( a, x, y )  { (y) = (a) * (x); }

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_dszscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_cszscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zszscal2s( a, x, y )  { (y) = (a) * (x); }

#define bli_sdzscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_ddzscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_cdzscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zdzscal2s( a, x, y )  { (y) = (a) * (x); }

#define bli_sczscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_dczscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_cczscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zczscal2s( a, x, y )  { (y) = (a) * (x); }

#define bli_szzscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_dzzscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_czzscal2s( a, x, y )  { (y) = (a) * (x); }
#define bli_zzzscal2s( a, x, y )  { (y) = (a) * (x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sscal2s( a, x, y )  bli_sssscal2s( a, x, y )
#define bli_dscal2s( a, x, y )  bli_dddscal2s( a, x, y )
#define bli_cscal2s( a, x, y )  bli_cccscal2s( a, x, y )
#define bli_zscal2s( a, x, y )  bli_zzzscal2s( a, x, y )


#endif

// end bli_scal2s.h
// begin bli_scal2js.h


#ifndef BLIS_SCAL2JS_H
#define BLIS_SCAL2JS_H

// scal2js

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of x.
// - The third char encodes the type of y.


// -- (axy) = (??s) ------------------------------------------------------------

#define bli_sssscal2js( a, x, y )  bli_sscal2jris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_dssscal2js( a, x, y )  bli_sscal2jris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_cssscal2js( a, x, y )  bli_sscal2jris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )
#define bli_zssscal2js( a, x, y )  bli_sscal2jris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_sreal(y), bli_simag(y) )

#define bli_sdsscal2js( a, x, y )  bli_sscal2jris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ddsscal2js( a, x, y )  bli_sscal2jris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_cdsscal2js( a, x, y )  bli_sscal2jris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zdsscal2js( a, x, y )  bli_sscal2jris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_sreal(y), bli_simag(y) )

#define bli_scsscal2js( a, x, y )  bli_sscal2jris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dcsscal2js( a, x, y )  bli_sscal2jris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_ccsscal2js( a, x, y )  bli_sscal2jris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zcsscal2js( a, x, y )  bli_sscal2jris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_sreal(y), bli_simag(y) )

#define bli_szsscal2js( a, x, y )  bli_sscal2jris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_dzsscal2js( a, x, y )  bli_sscal2jris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_czsscal2js( a, x, y )  bli_sscal2jris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )
#define bli_zzsscal2js( a, x, y )  bli_sscal2jris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_sreal(y), bli_simag(y) )

// -- (axy) = (??d) ------------------------------------------------------------

#define bli_ssdscal2js( a, x, y )  bli_dscal2jris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dsdscal2js( a, x, y )  bli_dscal2jris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_csdscal2js( a, x, y )  bli_dscal2jris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zsdscal2js( a, x, y )  bli_dscal2jris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_dreal(y), bli_dimag(y) )

#define bli_sddscal2js( a, x, y )  bli_dscal2jris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dddscal2js( a, x, y )  bli_dscal2jris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_cddscal2js( a, x, y )  bli_dscal2jris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zddscal2js( a, x, y )  bli_dscal2jris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_scdscal2js( a, x, y )  bli_dscal2jris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dcdscal2js( a, x, y )  bli_dscal2jris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_ccdscal2js( a, x, y )  bli_dscal2jris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zcdscal2js( a, x, y )  bli_dscal2jris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_dreal(y), bli_dimag(y) )

#define bli_szdscal2js( a, x, y )  bli_dscal2jris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_dzdscal2js( a, x, y )  bli_dscal2jris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_czdscal2js( a, x, y )  bli_dscal2jris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )
#define bli_zzdscal2js( a, x, y )  bli_dscal2jris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscscal2js( a, x, y )  bli_sscal2jris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_dscscal2js( a, x, y )  bli_sscal2jris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_cscscal2js( a, x, y )  bli_cscal2jris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )
#define bli_zscscal2js( a, x, y )  bli_cscal2jris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_creal(y), bli_cimag(y) )

#define bli_sdcscal2js( a, x, y )  bli_sscal2jris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_ddcscal2js( a, x, y )  bli_sscal2jris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cdcscal2js( a, x, y )  bli_cscal2jris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zdcscal2js( a, x, y )  bli_cscal2jris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_creal(y), bli_cimag(y) )

#define bli_sccscal2js( a, x, y )  bli_scscal2jris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dccscal2js( a, x, y )  bli_scscal2jris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_cccscal2js( a, x, y )   bli_cscal2jris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zccscal2js( a, x, y )   bli_cscal2jris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_creal(y), bli_cimag(y) )

#define bli_szcscal2js( a, x, y )  bli_scscal2jris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_dzcscal2js( a, x, y )  bli_scscal2jris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_czcscal2js( a, x, y )   bli_cscal2jris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )
#define bli_zzcscal2js( a, x, y )   bli_cscal2jris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_creal(y), bli_cimag(y) )

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszscal2js( a, x, y )  bli_dscal2jris( bli_sreal(a), bli_simag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dszscal2js( a, x, y )  bli_dscal2jris( bli_dreal(a), bli_dimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cszscal2js( a, x, y )  bli_zscal2jris( bli_creal(a), bli_cimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zszscal2js( a, x, y )  bli_zscal2jris( bli_zreal(a), bli_zimag(a), bli_sreal(x), bli_simag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sdzscal2js( a, x, y )  bli_dscal2jris( bli_sreal(a), bli_simag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_ddzscal2js( a, x, y )  bli_dscal2jris( bli_dreal(a), bli_dimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cdzscal2js( a, x, y )  bli_zscal2jris( bli_creal(a), bli_cimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zdzscal2js( a, x, y )  bli_zscal2jris( bli_zreal(a), bli_zimag(a), bli_dreal(x), bli_dimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_sczscal2js( a, x, y )  bli_dzscal2jris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dczscal2js( a, x, y )  bli_dzscal2jris( bli_dreal(a), bli_dimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_cczscal2js( a, x, y )   bli_zscal2jris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zczscal2js( a, x, y )   bli_zscal2jris( bli_zreal(a), bli_zimag(a), bli_creal(x), bli_cimag(x), bli_zreal(y), bli_zimag(y) )

#define bli_szzscal2js( a, x, y )  bli_dzscal2jris( bli_sreal(a), bli_simag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_dzzscal2js( a, x, y )  bli_dzscal2jris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_czzscal2js( a, x, y )   bli_zscal2jris( bli_creal(a), bli_cimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )
#define bli_zzzscal2js( a, x, y )   bli_zscal2jris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (axy) = (??c) ------------------------------------------------------------

#define bli_sscscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_dscscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_cscscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_zscscal2js( a, x, y )  { (y) = (a) * (x); }

#define bli_sdcscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_ddcscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_cdcscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_zdcscal2js( a, x, y )  { (y) = (a) * (x); }

#define bli_sccscal2js( a, x, y )  { (y) = (a) * conjf(x); }
#define bli_dccscal2js( a, x, y )  { (y) = (a) * conjf(x); }
#define bli_cccscal2js( a, x, y )  { (y) = (a) * conjf(x); }
#define bli_zccscal2js( a, x, y )  { (y) = (a) * conjf(x); }

#define bli_szcscal2js( a, x, y )  { (y) = (a) * conj(x); }
#define bli_dzcscal2js( a, x, y )  { (y) = (a) * conj(x); }
#define bli_czcscal2js( a, x, y )  { (y) = (a) * conj(x); }
#define bli_zzcscal2js( a, x, y )  { (y) = (a) * conj(x); }

// -- (axy) = (??z) ------------------------------------------------------------

#define bli_sszscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_dszscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_cszscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_zszscal2js( a, x, y )  { (y) = (a) * (x); }

#define bli_sdzscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_ddzscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_cdzscal2js( a, x, y )  { (y) = (a) * (x); }
#define bli_zdzscal2js( a, x, y )  { (y) = (a) * (x); }

#define bli_sczscal2js( a, x, y )  { (y) = (a) * conjf(x); }
#define bli_dczscal2js( a, x, y )  { (y) = (a) * conjf(x); }
#define bli_cczscal2js( a, x, y )  { (y) = (a) * conjf(x); }
#define bli_zczscal2js( a, x, y )  { (y) = (a) * conjf(x); }

#define bli_szzscal2js( a, x, y )  { (y) = (a) * conj(x); }
#define bli_dzzscal2js( a, x, y )  { (y) = (a) * conj(x); }
#define bli_czzscal2js( a, x, y )  { (y) = (a) * conj(x); }
#define bli_zzzscal2js( a, x, y )  { (y) = (a) * conj(x); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sscal2js( a, x, y )  bli_sssscal2js( a, x, y )
#define bli_dscal2js( a, x, y )  bli_dddscal2js( a, x, y )
#define bli_cscal2js( a, x, y )  bli_cccscal2js( a, x, y )
#define bli_zscal2js( a, x, y )  bli_zzzscal2js( a, x, y )


#endif

// end bli_scal2js.h

// begin bli_set0s.h


#ifndef BLIS_SET0S_H
#define BLIS_SET0S_H

#define bli_sset0s( a )  bli_ssets( 0.0F, 0.0F, (a) )
#define bli_dset0s( a )  bli_dsets( 0.0 , 0.0 , (a) )
#define bli_cset0s( a )  bli_csets( 0.0F, 0.0F, (a) )
#define bli_zset0s( a )  bli_zsets( 0.0 , 0.0 , (a) )

#endif

// end bli_set0s.h

// begin bli_set1s.h


#ifndef BLIS_SET1S_H
#define BLIS_SET1S_H

#define bli_sset1s( a )  bli_ssets( 1.0F, 0.0F, (a) )
#define bli_dset1s( a )  bli_dsets( 1.0 , 0.0 , (a) )
#define bli_cset1s( a )  bli_csets( 1.0F, 0.0F, (a) )
#define bli_zset1s( a )  bli_zsets( 1.0 , 0.0 , (a) )

#endif

// end bli_set1s.h

// begin bli_seti0s.h


#ifndef BLIS_SETI0S_H
#define BLIS_SETI0S_H

#define bli_sseti0s( a )  bli_ssetis( 0.0F, (a) )
#define bli_dseti0s( a )  bli_dsetis( 0.0 , (a) )
#define bli_cseti0s( a )  bli_csetis( 0.0F, (a) )
#define bli_zseti0s( a )  bli_zsetis( 0.0 , (a) )

#endif

// end bli_seti0s.h

// begin bli_sqrt2s.h


#ifndef BLIS_SQRT2S_H
#define BLIS_SQRT2S_H

// sqrt2s

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of a.

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_sssqrt2s( x, a )  bli_ssqrt2ris( bli_sreal(x), bli_simag(x), bli_sreal(a), bli_simag(a) )
#define bli_dssqrt2s( x, a )  bli_ssqrt2ris( bli_dreal(x), bli_dimag(x), bli_sreal(a), bli_simag(a) )
#define bli_cssqrt2s( x, a )  bli_ssqrt2ris( bli_creal(x), bli_cimag(x), bli_sreal(a), bli_simag(a) )
#define bli_zssqrt2s( x, a )  bli_ssqrt2ris( bli_zreal(x), bli_zimag(x), bli_sreal(a), bli_simag(a) )

#define bli_sdsqrt2s( x, a )  bli_dsqrt2ris( bli_sreal(x), bli_simag(x), bli_dreal(a), bli_dimag(a) )
#define bli_ddsqrt2s( x, a )  bli_dsqrt2ris( bli_dreal(x), bli_dimag(x), bli_dreal(a), bli_dimag(a) )
#define bli_cdsqrt2s( x, a )  bli_dsqrt2ris( bli_creal(x), bli_cimag(x), bli_dreal(a), bli_dimag(a) )
#define bli_zdsqrt2s( x, a )  bli_dsqrt2ris( bli_zreal(x), bli_zimag(x), bli_dreal(a), bli_dimag(a) )

#define bli_scsqrt2s( x, a )  bli_scsqrt2ris( bli_sreal(x), bli_simag(x), bli_creal(a), bli_cimag(a) )
#define bli_dcsqrt2s( x, a )  bli_scsqrt2ris( bli_dreal(x), bli_dimag(x), bli_creal(a), bli_cimag(a) )
#define bli_ccsqrt2s( x, a )   bli_csqrt2ris( bli_creal(x), bli_cimag(x), bli_creal(a), bli_cimag(a) )
#define bli_zcsqrt2s( x, a )   bli_csqrt2ris( bli_zreal(x), bli_zimag(x), bli_creal(a), bli_cimag(a) )

#define bli_szsqrt2s( x, a )  bli_dzsqrt2ris( bli_sreal(x), bli_simag(x), bli_zreal(a), bli_zimag(a) )
#define bli_dzsqrt2s( x, a )  bli_dzsqrt2ris( bli_dreal(x), bli_dimag(x), bli_zreal(a), bli_zimag(a) )
#define bli_czsqrt2s( x, a )   bli_zsqrt2ris( bli_creal(x), bli_cimag(x), bli_zreal(a), bli_zimag(a) )
#define bli_zzsqrt2s( x, a )   bli_zsqrt2ris( bli_zreal(x), bli_zimag(x), bli_zreal(a), bli_zimag(a) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_sssqrt2s( x, a )  { (a) = ( float    )            sqrtf( (x) )  ; }
#define bli_dssqrt2s( x, a )  { (a) = ( float    )            sqrt ( (x) )  ; }
#define bli_cssqrt2s( x, a )  { (a) = ( float    )bli_creal( csqrtf( (x) ) ); }
#define bli_zssqrt2s( x, a )  { (a) = ( float    )bli_zreal( csqrt ( (x) ) ); }

#define bli_sdsqrt2s( x, a )  { (a) = ( double   )            sqrtf( (x) )  ; }
#define bli_ddsqrt2s( x, a )  { (a) = ( double   )            sqrt ( (x) )  ; }
#define bli_cdsqrt2s( x, a )  { (a) = ( double   )bli_creal( csqrtf( (x) ) ); }
#define bli_zdsqrt2s( x, a )  { (a) = ( double   )bli_zreal( csqrt ( (x) ) ); }

#define bli_scsqrt2s( x, a )  { (a) = ( scomplex )            sqrtf( (x) )  ; }
#define bli_dcsqrt2s( x, a )  { (a) = ( scomplex )            sqrt ( (x) )  ; }
#define bli_ccsqrt2s( x, a )  { (a) = ( scomplex )           csqrtf( (x) )  ; }
#define bli_zcsqrt2s( x, a )  { (a) = ( scomplex )           csqrt ( (x) )  ; }

#define bli_szsqrt2s( x, a )  { (a) = ( dcomplex )            sqrtf( (x) )  ; }
#define bli_dzsqrt2s( x, a )  { (a) = ( dcomplex )            sqrt ( (x) )  ; }
#define bli_czsqrt2s( x, a )  { (a) = ( dcomplex )           csqrtf( (x) )  ; }
#define bli_zzsqrt2s( x, a )  { (a) = ( dcomplex )           csqrt ( (x) )  ; }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_ssqrt2s( x, a )  bli_sssqrt2s( x, a )
#define bli_dsqrt2s( x, a )  bli_ddsqrt2s( x, a )
#define bli_csqrt2s( x, a )  bli_ccsqrt2s( x, a )
#define bli_zsqrt2s( x, a )  bli_zzsqrt2s( x, a )


#endif
// end bli_sqrt2s.h

// begin bli_subs.h


#ifndef BLIS_SUBS_H
#define BLIS_SUBS_H

// subs

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_sssubs( a, y )  bli_ssubris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dssubs( a, y )  bli_ssubris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_cssubs( a, y )  bli_ssubris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zssubs( a, y )  bli_ssubris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdsubs( a, y )  bli_dsubris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddsubs( a, y )  bli_dsubris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdsubs( a, y )  bli_dsubris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdsubs( a, y )  bli_dsubris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scsubs( a, y )  bli_csubris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcsubs( a, y )  bli_csubris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccsubs( a, y )  bli_csubris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcsubs( a, y )  bli_csubris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szsubs( a, y )  bli_zsubris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzsubs( a, y )  bli_zsubris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czsubs( a, y )  bli_zsubris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzsubs( a, y )  bli_zsubris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scsubs( a, y )  { (y) -= (a); }
#define bli_dcsubs( a, y )  { (y) -= (a); }
#define bli_ccsubs( a, y )  { (y) -= (a); }
#define bli_zcsubs( a, y )  { (y) -= (a); }

#define bli_szsubs( a, y )  { (y) -= (a); }
#define bli_dzsubs( a, y )  { (y) -= (a); }
#define bli_czsubs( a, y )  { (y) -= (a); }
#define bli_zzsubs( a, y )  { (y) -= (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_ssubs( a, y )  bli_sssubs( a, y )
#define bli_dsubs( a, y )  bli_ddsubs( a, y )
#define bli_csubs( a, y )  bli_ccsubs( a, y )
#define bli_zsubs( a, y )  bli_zzsubs( a, y )


#endif

// end bli_subs.h
// begin bli_subjs.h


#ifndef BLIS_SUBJS_H
#define BLIS_SUBJS_H

// subjs

// Notes:
// - The first char encodes the type of a.
// - The second char encodes the type of y.

#define bli_sssubjs( a, y )  bli_ssubjris( bli_sreal(a), bli_simag(a), bli_sreal(y), bli_simag(y) )
#define bli_dssubjs( a, y )  bli_ssubjris( bli_dreal(a), bli_dimag(a), bli_sreal(y), bli_simag(y) )
#define bli_cssubjs( a, y )  bli_ssubjris( bli_creal(a), bli_cimag(a), bli_sreal(y), bli_simag(y) )
#define bli_zssubjs( a, y )  bli_ssubjris( bli_zreal(a), bli_zimag(a), bli_sreal(y), bli_simag(y) )

#define bli_sdsubjs( a, y )  bli_dsubjris( bli_sreal(a), bli_simag(a), bli_dreal(y), bli_dimag(y) )
#define bli_ddsubjs( a, y )  bli_dsubjris( bli_dreal(a), bli_dimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_cdsubjs( a, y )  bli_dsubjris( bli_creal(a), bli_cimag(a), bli_dreal(y), bli_dimag(y) )
#define bli_zdsubjs( a, y )  bli_dsubjris( bli_zreal(a), bli_zimag(a), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

#define bli_scsubjs( a, y )  bli_csubjris( bli_sreal(a), bli_simag(a), bli_creal(y), bli_cimag(y) )
#define bli_dcsubjs( a, y )  bli_csubjris( bli_dreal(a), bli_dimag(a), bli_creal(y), bli_cimag(y) )
#define bli_ccsubjs( a, y )  bli_csubjris( bli_creal(a), bli_cimag(a), bli_creal(y), bli_cimag(y) )
#define bli_zcsubjs( a, y )  bli_csubjris( bli_zreal(a), bli_zimag(a), bli_creal(y), bli_cimag(y) )

#define bli_szsubjs( a, y )  bli_zsubjris( bli_sreal(a), bli_simag(a), bli_zreal(y), bli_zimag(y) )
#define bli_dzsubjs( a, y )  bli_zsubjris( bli_dreal(a), bli_dimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_czsubjs( a, y )  bli_zsubjris( bli_creal(a), bli_cimag(a), bli_zreal(y), bli_zimag(y) )
#define bli_zzsubjs( a, y )  bli_zsubjris( bli_zreal(a), bli_zimag(a), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

#define bli_scsubjs( a, y )  { (y) -=      (a); }
#define bli_dcsubjs( a, y )  { (y) -=      (a); }
#define bli_ccsubjs( a, y )  { (y) -= conjf(a); }
#define bli_zcsubjs( a, y )  { (y) -= conj (a); }

#define bli_szsubjs( a, y )  { (y) -=      (a); }
#define bli_dzsubjs( a, y )  { (y) -=      (a); }
#define bli_czsubjs( a, y )  { (y) -= conjf(a); }
#define bli_zzsubjs( a, y )  { (y) -= conj (a); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_ssubjs( a, y )  bli_sssubjs( a, y )
#define bli_dsubjs( a, y )  bli_ddsubjs( a, y )
#define bli_csubjs( a, y )  bli_ccsubjs( a, y )
#define bli_zsubjs( a, y )  bli_zzsubjs( a, y )


#endif

// end bli_subjs.h

// begin bli_swaps.h


#ifndef BLIS_SWAPS_H
#define BLIS_SWAPS_H

// swaps

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.


#define bli_ssswaps( x, y ) \
{ \
	float    w; \
	bli_sscopys( (y), (w) ); \
	bli_sscopys( (x), (y) ); \
	bli_sscopys( (w), (x) ); \
}
#define bli_dsswaps( x, y ) \
{ \
	double   w; \
	bli_sdcopys( (y), (w) ); \
	bli_dscopys( (x), (y) ); \
	bli_ddcopys( (w), (x) ); \
}
#define bli_csswaps( x, y ) \
{ \
	scomplex w; \
	bli_sccopys( (y), (w) ); \
	bli_cscopys( (x), (y) ); \
	bli_cccopys( (w), (x) ); \
}
#define bli_zsswaps( x, y ) \
{ \
	dcomplex w; \
	bli_szcopys( (y), (w) ); \
	bli_zscopys( (x), (y) ); \
	bli_zzcopys( (w), (x) ); \
}


#define bli_sdswaps( x, y ) \
{ \
	float    w; \
	bli_dscopys( (y), (w) ); \
	bli_sdcopys( (x), (y) ); \
	bli_sscopys( (w), (x) ); \
}
#define bli_ddswaps( x, y ) \
{ \
	double   w; \
	bli_ddcopys( (y), (w) ); \
	bli_ddcopys( (x), (y) ); \
	bli_ddcopys( (w), (x) ); \
}
#define bli_cdswaps( x, y ) \
{ \
	scomplex w; \
	bli_dccopys( (y), (w) ); \
	bli_cdcopys( (x), (y) ); \
	bli_cccopys( (w), (x) ); \
}
#define bli_zdswaps( x, y ) \
{ \
	dcomplex w; \
	bli_dzcopys( (y), (w) ); \
	bli_zdcopys( (x), (y) ); \
	bli_zzcopys( (w), (x) ); \
}


#define bli_scswaps( x, y ) \
{ \
	float    w; \
	bli_cscopys( (y), (w) ); \
	bli_sccopys( (x), (y) ); \
	bli_sscopys( (w), (x) ); \
}
#define bli_dcswaps( x, y ) \
{ \
	double   w; \
	bli_cdcopys( (y), (w) ); \
	bli_dccopys( (x), (y) ); \
	bli_ddcopys( (w), (x) ); \
}
#define bli_ccswaps( x, y ) \
{ \
	scomplex w; \
	bli_cccopys( (y), (w) ); \
	bli_cccopys( (x), (y) ); \
	bli_cccopys( (w), (x) ); \
}
#define bli_zcswaps( x, y ) \
{ \
	dcomplex w; \
	bli_czcopys( (y), (w) ); \
	bli_zccopys( (x), (y) ); \
	bli_zzcopys( (w), (x) ); \
}


#define bli_szswaps( x, y ) \
{ \
	float    w; \
	bli_zscopys( (y), (w) ); \
	bli_szcopys( (x), (y) ); \
	bli_sscopys( (w), (x) ); \
}
#define bli_dzswaps( x, y ) \
{ \
	double   w; \
	bli_zdcopys( (y), (w) ); \
	bli_dzcopys( (x), (y) ); \
	bli_ddcopys( (w), (x) ); \
}
#define bli_czswaps( x, y ) \
{ \
	scomplex w; \
	bli_zccopys( (y), (w) ); \
	bli_czcopys( (x), (y) ); \
	bli_cccopys( (w), (x) ); \
}
#define bli_zzswaps( x, y ) \
{ \
	dcomplex w; \
	bli_zzcopys( (y), (w) ); \
	bli_zzcopys( (x), (y) ); \
	bli_zzcopys( (w), (x) ); \
}


#define bli_sswaps( x, y )  bli_ssswaps( x, y )
#define bli_dswaps( x, y )  bli_ddswaps( x, y )
#define bli_cswaps( x, y )  bli_ccswaps( x, y )
#define bli_zswaps( x, y )  bli_zzswaps( x, y )


#endif
// end bli_swaps.h

// begin bli_xpbys.h


#ifndef BLIS_XPBYS_H
#define BLIS_XPBYS_H

// xpbys

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of b.
// - The third char encodes the type of y.


// -- (xby) = (??s) ------------------------------------------------------------

#define bli_sssxpbys( x, b, y )  bli_sxpbyris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dssxpbys( x, b, y )  bli_sxpbyris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_cssxpbys( x, b, y )  bli_sxpbyris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zssxpbys( x, b, y )  bli_sxpbyris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )

#define bli_sdsxpbys( x, b, y )  bli_sxpbyris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddsxpbys( x, b, y )  bli_sxpbyris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdsxpbys( x, b, y )  bli_sxpbyris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdsxpbys( x, b, y )  bli_sxpbyris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )

#define bli_scsxpbys( x, b, y )  bli_sxpbyris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dcsxpbys( x, b, y )  bli_sxpbyris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ccsxpbys( x, b, y )  bli_sxpbyris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zcsxpbys( x, b, y )  bli_sxpbyris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )

#define bli_szsxpbys( x, b, y )  bli_sxpbyris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzsxpbys( x, b, y )  bli_sxpbyris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czsxpbys( x, b, y )  bli_sxpbyris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzsxpbys( x, b, y )  bli_sxpbyris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )

// -- (xby) = (??d) ------------------------------------------------------------

#define bli_ssdxpbys( x, b, y )  bli_dxpbyris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dsdxpbys( x, b, y )  bli_dxpbyris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_csdxpbys( x, b, y )  bli_dxpbyris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zsdxpbys( x, b, y )  bli_dxpbyris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )

#define bli_sddxpbys( x, b, y )  bli_dxpbyris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dddxpbys( x, b, y )  bli_dxpbyris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cddxpbys( x, b, y )  bli_dxpbyris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zddxpbys( x, b, y )  bli_dxpbyris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_scdxpbys( x, b, y )  bli_dxpbyris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dcdxpbys( x, b, y )  bli_dxpbyris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ccdxpbys( x, b, y )  bli_dxpbyris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zcdxpbys( x, b, y )  bli_dxpbyris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_szdxpbys( x, b, y )  bli_dxpbyris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzdxpbys( x, b, y )  bli_dxpbyris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czdxpbys( x, b, y )  bli_dxpbyris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzdxpbys( x, b, y )  bli_dxpbyris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (xby) = (??c) ------------------------------------------------------------

#define bli_sscxpbys( x, b, y )   bli_sxpbyris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dscxpbys( x, b, y )   bli_sxpbyris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_cscxpbys( x, b, y )  bli_scxpbyris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zscxpbys( x, b, y )  bli_scxpbyris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )

#define bli_sdcxpbys( x, b, y )   bli_sxpbyris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddcxpbys( x, b, y )   bli_sxpbyris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdcxpbys( x, b, y )  bli_scxpbyris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdcxpbys( x, b, y )  bli_scxpbyris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )

#define bli_sccxpbys( x, b, y )  bli_cxpbyris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dccxpbys( x, b, y )  bli_cxpbyris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cccxpbys( x, b, y )  bli_cxpbyris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zccxpbys( x, b, y )  bli_cxpbyris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )

#define bli_szcxpbys( x, b, y )  bli_cxpbyris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzcxpbys( x, b, y )  bli_cxpbyris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czcxpbys( x, b, y )  bli_cxpbyris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzcxpbys( x, b, y )  bli_cxpbyris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )

// -- (xby) = (??z) ------------------------------------------------------------

#define bli_sszxpbys( x, b, y )   bli_dxpbyris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dszxpbys( x, b, y )   bli_dxpbyris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cszxpbys( x, b, y )  bli_dzxpbyris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zszxpbys( x, b, y )  bli_dzxpbyris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )

#define bli_sdzxpbys( x, b, y )   bli_dxpbyris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddzxpbys( x, b, y )   bli_dxpbyris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdzxpbys( x, b, y )  bli_dzxpbyris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdzxpbys( x, b, y )  bli_dzxpbyris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_sczxpbys( x, b, y )  bli_zxpbyris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dczxpbys( x, b, y )  bli_zxpbyris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cczxpbys( x, b, y )  bli_zxpbyris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zczxpbys( x, b, y )  bli_zxpbyris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_szzxpbys( x, b, y )  bli_zxpbyris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzzxpbys( x, b, y )  bli_zxpbyris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czzxpbys( x, b, y )  bli_zxpbyris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzzxpbys( x, b, y )  bli_zxpbyris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (xby) = (??c) ------------------------------------------------------------

#define bli_sscxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_dscxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_cscxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zscxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

#define bli_sdcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_ddcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_cdcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zdcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

#define bli_sccxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_dccxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_cccxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zccxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

#define bli_szcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_dzcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_czcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zzcxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

// -- (xby) = (??z) ------------------------------------------------------------

#define bli_sszxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_dszxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_cszxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zszxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

#define bli_sdzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_ddzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_cdzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zdzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

#define bli_sczxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_dczxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_cczxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zczxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

#define bli_szzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_dzzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_czzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }
#define bli_zzzxpbys( x, b, y )  { (y) = (x) + (b) * (y); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sxpbys( x, b, y )  bli_sssxpbys( x, b, y )
#define bli_dxpbys( x, b, y )  bli_dddxpbys( x, b, y )
#define bli_cxpbys( x, b, y )  bli_cccxpbys( x, b, y )
#define bli_zxpbys( x, b, y )  bli_zzzxpbys( x, b, y )


#endif

// end bli_xpbys.h
// begin bli_xpbyjs.h


#ifndef BLIS_XPBYJS_H
#define BLIS_XPBYJS_H

// xpbyjs

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of b.
// - The third char encodes the type of y.


// -- (xby) = (??s) ------------------------------------------------------------

#define bli_sssxpbyjs( x, b, y )  bli_sxpbyjris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_dssxpbyjs( x, b, y )  bli_sxpbyjris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_cssxpbyjs( x, b, y )  bli_sxpbyjris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )
#define bli_zssxpbyjs( x, b, y )  bli_sxpbyjris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_sreal(y), bli_simag(y) )

#define bli_sdsxpbyjs( x, b, y )  bli_sxpbyjris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ddsxpbyjs( x, b, y )  bli_sxpbyjris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_cdsxpbyjs( x, b, y )  bli_sxpbyjris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zdsxpbyjs( x, b, y )  bli_sxpbyjris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_sreal(y), bli_simag(y) )

#define bli_scsxpbyjs( x, b, y )  bli_sxpbyjris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dcsxpbyjs( x, b, y )  bli_sxpbyjris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_ccsxpbyjs( x, b, y )  bli_sxpbyjris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zcsxpbyjs( x, b, y )  bli_sxpbyjris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_sreal(y), bli_simag(y) )

#define bli_szsxpbyjs( x, b, y )  bli_sxpbyjris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_dzsxpbyjs( x, b, y )  bli_sxpbyjris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_czsxpbyjs( x, b, y )  bli_sxpbyjris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )
#define bli_zzsxpbyjs( x, b, y )  bli_sxpbyjris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_sreal(y), bli_simag(y) )

// -- (xby) = (??d) ------------------------------------------------------------

#define bli_ssdxpbyjs( x, b, y )  bli_dxpbyjris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dsdxpbyjs( x, b, y )  bli_dxpbyjris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_csdxpbyjs( x, b, y )  bli_dxpbyjris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zsdxpbyjs( x, b, y )  bli_dxpbyjris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_dreal(y), bli_dimag(y) )

#define bli_sddxpbyjs( x, b, y )  bli_dxpbyjris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dddxpbyjs( x, b, y )  bli_dxpbyjris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_cddxpbyjs( x, b, y )  bli_dxpbyjris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zddxpbyjs( x, b, y )  bli_dxpbyjris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_scdxpbyjs( x, b, y )  bli_dxpbyjris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dcdxpbyjs( x, b, y )  bli_dxpbyjris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_ccdxpbyjs( x, b, y )  bli_dxpbyjris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zcdxpbyjs( x, b, y )  bli_dxpbyjris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_dreal(y), bli_dimag(y) )

#define bli_szdxpbyjs( x, b, y )  bli_dxpbyjris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_dzdxpbyjs( x, b, y )  bli_dxpbyjris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_czdxpbyjs( x, b, y )  bli_dxpbyjris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )
#define bli_zzdxpbyjs( x, b, y )  bli_dxpbyjris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_dreal(y), bli_dimag(y) )

#ifndef BLIS_ENABLE_C99_COMPLEX

// -- (xby) = (??c) ------------------------------------------------------------

#define bli_sscxpbyjs( x, b, y )   bli_sxpbyjris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_dscxpbyjs( x, b, y )   bli_sxpbyjris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_cscxpbyjs( x, b, y )  bli_scxpbyjris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )
#define bli_zscxpbyjs( x, b, y )  bli_scxpbyjris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_creal(y), bli_cimag(y) )

#define bli_sdcxpbyjs( x, b, y )   bli_sxpbyjris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_ddcxpbyjs( x, b, y )   bli_sxpbyjris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cdcxpbyjs( x, b, y )  bli_scxpbyjris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zdcxpbyjs( x, b, y )  bli_scxpbyjris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_creal(y), bli_cimag(y) )

#define bli_sccxpbyjs( x, b, y )  bli_cxpbyjris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dccxpbyjs( x, b, y )  bli_cxpbyjris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_cccxpbyjs( x, b, y )  bli_cxpbyjris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zccxpbyjs( x, b, y )  bli_cxpbyjris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_creal(y), bli_cimag(y) )

#define bli_szcxpbyjs( x, b, y )  bli_cxpbyjris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_dzcxpbyjs( x, b, y )  bli_cxpbyjris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_czcxpbyjs( x, b, y )  bli_cxpbyjris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )
#define bli_zzcxpbyjs( x, b, y )  bli_cxpbyjris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_creal(y), bli_cimag(y) )

// -- (xby) = (??z) ------------------------------------------------------------

#define bli_sszxpbyjs( x, b, y )   bli_dxpbyjris( bli_sreal(x), bli_simag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dszxpbyjs( x, b, y )   bli_dxpbyjris( bli_dreal(x), bli_dimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cszxpbyjs( x, b, y )  bli_dzxpbyjris( bli_creal(x), bli_cimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zszxpbyjs( x, b, y )  bli_dzxpbyjris( bli_zreal(x), bli_zimag(x), bli_sreal(b), bli_simag(b), bli_zreal(y), bli_zimag(y) )

#define bli_sdzxpbyjs( x, b, y )   bli_dxpbyjris( bli_sreal(x), bli_simag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_ddzxpbyjs( x, b, y )   bli_dxpbyjris( bli_dreal(x), bli_dimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cdzxpbyjs( x, b, y )  bli_dzxpbyjris( bli_creal(x), bli_cimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zdzxpbyjs( x, b, y )  bli_dzxpbyjris( bli_zreal(x), bli_zimag(x), bli_dreal(b), bli_dimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_sczxpbyjs( x, b, y )  bli_zxpbyjris( bli_sreal(x), bli_simag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dczxpbyjs( x, b, y )  bli_zxpbyjris( bli_dreal(x), bli_dimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_cczxpbyjs( x, b, y )  bli_zxpbyjris( bli_creal(x), bli_cimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zczxpbyjs( x, b, y )  bli_zxpbyjris( bli_zreal(x), bli_zimag(x), bli_creal(b), bli_cimag(b), bli_zreal(y), bli_zimag(y) )

#define bli_szzxpbyjs( x, b, y )  bli_zxpbyjris( bli_sreal(x), bli_simag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_dzzxpbyjs( x, b, y )  bli_zxpbyjris( bli_dreal(x), bli_dimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_czzxpbyjs( x, b, y )  bli_zxpbyjris( bli_creal(x), bli_cimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )
#define bli_zzzxpbyjs( x, b, y )  bli_zxpbyjris( bli_zreal(x), bli_zimag(x), bli_zreal(b), bli_zimag(b), bli_zreal(y), bli_zimag(y) )

#else // ifdef BLIS_ENABLE_C99_COMPLEX

// -- (xby) = (??c) ------------------------------------------------------------

#define bli_sscxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_dscxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_cscxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zscxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

#define bli_sdcxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_ddcxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_cdcxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zdcxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

#define bli_sccxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_dccxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_cccxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zccxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

#define bli_szcxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_dzcxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_czcxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zzcxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

// -- (xby) = (??z) ------------------------------------------------------------

#define bli_sszxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_dszxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_cszxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zszxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

#define bli_sdzxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_ddzxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_cdzxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zdzxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

#define bli_sczxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_dczxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_cczxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zczxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

#define bli_szzxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_dzzxpbyjs( x, b, y )  { (y) =      (x) + (b) * (y); }
#define bli_czzxpbyjs( x, b, y )  { (y) = conjf(x) + (b) * (y); }
#define bli_zzzxpbyjs( x, b, y )  { (y) =  conj(x) + (b) * (y); }

#endif // BLIS_ENABLE_C99_COMPLEX


#define bli_sxpbyjs( x, b, y )  bli_sssxpbyjs( x, b, y )
#define bli_dxpbyjs( x, b, y )  bli_dddxpbyjs( x, b, y )
#define bli_cxpbyjs( x, b, y )  bli_cccxpbyjs( x, b, y )
#define bli_zxpbyjs( x, b, y )  bli_zzzxpbyjs( x, b, y )


#endif

// end bli_xpbyjs.h

// Inlined scalar macros in loops
// begin bli_adds_mxn.h


#ifndef BLIS_ADDS_MXN_H
#define BLIS_ADDS_MXN_H

// adds_mxn

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_ssadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_ssadds( *(x + _i*rs_x + _j*cs_x), \
	            *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_ddadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_ddadds( *(x + _i*rs_x + _j*cs_x), \
	            *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_ccadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_ccadds( *(x + _i*rs_x + _j*cs_x), \
	            *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_zzadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_zzadds( *(x + _i*rs_x + _j*cs_x), \
	            *(y + _i*rs_y + _j*cs_y) ); \
}


#define bli_sadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ssadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_dadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ddadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_cadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ccadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_zadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_zzadds_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}

#endif
// end bli_adds_mxn.h
// begin bli_adds_mxn_uplo.h


#ifndef BLIS_ADDS_MXN_UPLO_H
#define BLIS_ADDS_MXN_UPLO_H

// adds_mxn_u

#define bli_ssadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
			{ \
				bli_ssadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}

#define bli_ddadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
			{ \
				bli_ddadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}

#define bli_ccadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
			{ \
				bli_ccadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}

#define bli_zzadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
			{ \
				bli_zzadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}

// adds_mxn_l

#define bli_ssadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
			{ \
				bli_ssadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}

#define bli_ddadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
			{ \
				bli_ddadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}

#define bli_ccadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
			{ \
				bli_ccadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}

#define bli_zzadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	{ \
		for ( _i = 0; _i < m; ++_i ) \
		{ \
			if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
			{ \
				bli_zzadds( *(x + _i*rs_x + _j*cs_x), \
				            *(y + _i*rs_y + _j*cs_y) ); \
			} \
		} \
	} \
}


#define bli_sadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ssadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_dadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ddadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_cadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ccadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_zadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_zzadds_mxn_u( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_sadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ssadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_dadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ddadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_cadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ccadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_zadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_zzadds_mxn_l( diagoff, m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}

#endif
// end bli_adds_mxn_uplo.h
// begin bli_copys_mxn.h


#ifndef BLIS_COPYS_MXN_H
#define BLIS_COPYS_MXN_H

// copys_mxn

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sscopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_sscopys( *(x + _i*rs_x + _j*cs_x), \
	             *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_ddcopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_ddcopys( *(x + _i*rs_x + _j*cs_x), \
	             *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_cccopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_cccopys( *(x + _i*rs_x + _j*cs_x), \
	             *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_zzcopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_zzcopys( *(x + _i*rs_x + _j*cs_x), \
	             *(y + _i*rs_y + _j*cs_y) ); \
}


#define bli_scopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_sscopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_dcopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_ddcopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_ccopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_cccopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}
#define bli_zcopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ) \
{ \
	bli_zzcopys_mxn( m, n, x, rs_x, cs_x, y, rs_y, cs_y ); \
}

#endif
// end bli_copys_mxn.h
// begin bli_set0s_mxn.h


#ifndef BLIS_SET0S_MXN_H
#define BLIS_SET0S_MXN_H

// set0s_mxn

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of y.

#define bli_sset0s_mxn( m, n, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_sset0s( *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_dset0s_mxn( m, n, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_dset0s( *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_cset0s_mxn( m, n, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_cset0s( *(y + _i*rs_y + _j*cs_y) ); \
}

#define bli_zset0s_mxn( m, n, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	for ( _j = 0; _j < n; ++_j ) \
	for ( _i = 0; _i < m; ++_i ) \
	bli_zset0s( *(y + _i*rs_y + _j*cs_y) ); \
}



#endif
// end bli_set0s_mxn.h
// begin bli_xpbys_mxn.h


#ifndef BLIS_XPBYS_MXN_H
#define BLIS_XPBYS_MXN_H

// xpbys_mxn

// Notes:
// - The first char encodes the type of x.
// - The second char encodes the type of b.
// - The third char encodes the type of y.

#define bli_sssxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	 \
	if ( bli_seq0( *beta ) ) \
	{ \
		bli_sscopys_mxn( m, n, \
		                 x, rs_x, cs_x, \
		                 y, rs_y, cs_y ); \
	} \
	else \
	{ \
		dim_t _i, _j; \
\
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		bli_sssxpbys( *(x + _i*rs_x + _j*cs_x), \
		              *(beta), \
		              *(y + _i*rs_y + _j*cs_y) ); \
	} \
}

#define bli_dddxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	 \
	if ( bli_deq0( *beta ) ) \
	{ \
		bli_ddcopys_mxn( m, n, \
		                 x, rs_x, cs_x, \
		                 y, rs_y, cs_y ); \
	} \
	else \
	{ \
		dim_t _i, _j; \
\
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		bli_dddxpbys( *(x + _i*rs_x + _j*cs_x), \
		              *(beta), \
		              *(y + _i*rs_y + _j*cs_y) ); \
	} \
}

#define bli_cccxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	 \
	if ( bli_ceq0( *beta ) ) \
	{ \
		bli_cccopys_mxn( m, n, \
		                 x, rs_x, cs_x, \
		                 y, rs_y, cs_y ); \
	} \
	else \
	{ \
		dim_t _i, _j; \
\
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		bli_cccxpbys( *(x + _i*rs_x + _j*cs_x), \
		              *(beta), \
		              *(y + _i*rs_y + _j*cs_y) ); \
	} \
}

#define bli_zzzxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	 \
	if ( bli_zeq0( *beta ) ) \
	{ \
		bli_zzcopys_mxn( m, n, \
		                 x, rs_x, cs_x, \
		                 y, rs_y, cs_y ); \
	} \
	else \
	{ \
		dim_t _i, _j; \
\
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		bli_zzzxpbys( *(x + _i*rs_x + _j*cs_x), \
		              *(beta), \
		              *(y + _i*rs_y + _j*cs_y) ); \
	} \
}


#define bli_sxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_sssxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_dxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_dddxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_cxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_cccxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_zxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_zzzxpbys_mxn( m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}

#endif
// end bli_xpbys_mxn.h
// begin bli_xpbys_mxn_uplo.h


#ifndef BLIS_XPBYS_MXN_UPLO_H
#define BLIS_XPBYS_MXN_UPLO_H

// xpbys_mxn_u

#define bli_sssxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_seq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_sscopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_sssxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}

#define bli_dddxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_deq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_ddcopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_dddxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}

#define bli_cccxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_ceq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_cccopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_cccxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}

#define bli_zzzxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_zeq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_zzcopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i >= diagoff ) \
		{ \
			bli_zzzxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}

// xpbys_mxn_l

#define bli_sssxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_seq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_sscopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_sssxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}

#define bli_dddxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_deq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_ddcopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_dddxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}

#define bli_cccxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_ceq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_cccopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_cccxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}

#define bli_zzzxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_zeq0( *beta ) ) \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_zzcopys( *(x + _i*rs_x + _j*cs_x), \
			             *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
	else \
	{ \
		for ( _j = 0; _j < n; ++_j ) \
		for ( _i = 0; _i < m; ++_i ) \
		if ( (doff_t)_j - (doff_t)_i <= diagoff ) \
		{ \
			bli_zzzxpbys( *(x + _i*rs_x + _j*cs_x), \
			              *(beta), \
			              *(y + _i*rs_y + _j*cs_y) ); \
		} \
	} \
}


#define bli_sxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_sssxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_dxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_dddxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_cxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_cccxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_zxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_zzzxpbys_mxn_u( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_sxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_sssxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_dxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_dddxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_cxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_cccxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}
#define bli_zxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ) \
{\
	bli_zzzxpbys_mxn_l( diagoff, m, n, x, rs_x, cs_x, beta, y, rs_y, cs_y ); \
}

#endif
// end bli_xpbys_mxn_uplo.h


// -- 3m-specific scalar macros --

// begin bli_copyri3s.h


#ifndef BLIS_COPYRI3S_H
#define BLIS_COPYRI3S_H

// copyri3s

#define bli_scopyri3s( ar, ai, br, bi, bri ) \
{ \
	(br) = (ar); \
}

#define bli_dcopyri3s( ar, ai, br, bi, bri ) \
{ \
	(br) = (ar); \
}

#define bli_ccopyri3s( ar, ai, br, bi, bri ) \
{ \
	(br)  = (ar); \
	(bi)  = (ai); \
	(bri) = (ar) + (ai); \
}

#define bli_zcopyri3s( ar, ai, br, bi, bri ) \
{ \
	(br)  = (ar); \
	(bi)  = (ai); \
	(bri) = (ar) + (ai); \
}

#endif

// end bli_copyri3s.h
// begin bli_copyjri3s.h


#ifndef BLIS_COPYJRI3S_H
#define BLIS_COPYJRI3S_H

// copyjri3s

#define bli_scopyjri3s( ar, ai, br, bi, bri )  bli_scopyri3s( (ar), -(ai), (br), (bi), (bri) )
#define bli_dcopyjri3s( ar, ai, br, bi, bri )  bli_dcopyri3s( (ar), -(ai), (br), (bi), (bri) )
#define bli_ccopyjri3s( ar, ai, br, bi, bri )  bli_ccopyri3s( (ar), -(ai), (br), (bi), (bri) )
#define bli_zcopyjri3s( ar, ai, br, bi, bri )  bli_zcopyri3s( (ar), -(ai), (br), (bi), (bri) )

#endif

// end bli_copyjri3s.h

// begin bli_scal2ri3s.h


#ifndef BLIS_SCAL2RI3S_H
#define BLIS_SCAL2RI3S_H

// scal2ri3s

#define bli_sscal2ri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_dscal2ri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_cscal2ri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr) - (ai) * (xi); \
	(yi) = (ai) * (xr) + (ar) * (xi); \
	(yri) = (yr) + (yi); \
}

#define bli_zscal2ri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr) - (ai) * (xi); \
	(yi) = (ai) * (xr) + (ar) * (xi); \
	(yri) = (yr) + (yi); \
}

#define bli_scscal2ri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr); \
	(yi) = (ar) * (xi); \
	(yri) = (yr) + (yi); \
}

#define bli_dzscal2ri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr); \
	(yi) = (ar) * (xi); \
	(yri) = (yr) + (yi); \
}

#endif

// end bli_scal2ri3s.h
// begin bli_scal2jri3s.h


#ifndef BLIS_SCAL2JRI3S_H
#define BLIS_SCAL2JRI3S_H

// scal2jri3s

#define bli_sscal2jri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_dscal2jri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr); \
}

#define bli_cscal2jri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr) + (ai) * (xi); \
	(yi) = (ai) * (xr) - (ar) * (xi); \
	(yri) = (yr) + (yi); \
}

#define bli_zscal2jri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) * (xr) + (ai) * (xi); \
	(yi) = (ai) * (xr) - (ar) * (xi); \
	(yri) = (yr) + (yi); \
}

#define bli_scscal2jri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) *  (xr); \
	(yi) = (ar) * -(xi); \
	(yri) = (yr) + (yi); \
}

#define bli_dzscal2jri3s( ar, ai, xr, xi, yr, yi, yri ) \
{ \
	(yr) = (ar) *  (xr); \
	(yi) = (ar) * -(xi); \
	(yri) = (yr) + (yi); \
}

#endif

// end bli_scal2jri3s.h


// -- 4mh/3mh-specific scalar macros --

// begin bli_scal2rihs_mxn_diag.h


#ifndef BLIS_SCAL2RIHS_MXN_DIAG_H
#define BLIS_SCAL2RIHS_MXN_DIAG_H

// scal2rihs_mxn_diag

#define bli_cscscal2rihs_mxn_diag( schema, m, n, a, x, rs_x, cs_x, y_r, rs_y, cs_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t _i; \
\
	 \
	if ( bli_is_ro_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_scscal2ros( *(x   + _i*rs_x + _i*cs_x), \
			                *(a), \
			                *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else if ( bli_is_io_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_scscal2ios( *(x   + _i*rs_x + _i*cs_x), \
			                *(a), \
			                *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_scscal2rpis( *(x   + _i*rs_x + _i*cs_x), \
			                 *(a), \
			                 *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
}

#define bli_zdzscal2rihs_mxn_diag( schema, m, n, a, x, rs_x, cs_x, y_r, rs_y, cs_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t _i; \
\
	 \
	if ( bli_is_ro_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_dzscal2ros( *(x   + _i*rs_x + _i*cs_x), \
			                *(a), \
			                *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else if ( bli_is_io_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_dzscal2ios( *(x   + _i*rs_x + _i*cs_x), \
			                *(a), \
			                *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_dzscal2rpis( *(x   + _i*rs_x + _i*cs_x), \
			                 *(a), \
			                 *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
}

#endif
// end bli_scal2rihs_mxn_diag.h
// begin bli_scal2rihs_mxn_uplo.h


#ifndef BLIS_SCAL2RIHS_MXN_UPLO_H
#define BLIS_SCAL2RIHS_MXN_UPLO_H

// scal2rihs_mxn_uplo

#define bli_cscal2rihs_mxn_uplo( schema, uplo, conjx, m, a, x, rs_x, cs_x, y_r, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_is_ro_packed( schema ) ) \
	{ \
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_cscal2jros( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_cscal2ros( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_cscal2jros( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_cscal2ros( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
	} \
	else if ( bli_is_io_packed( schema ) ) \
	{ \
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_cscal2jios( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_cscal2ios( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_cscal2jios( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_cscal2ios( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
	} \
	else  \
	{ \
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_cscal2jrpis( *(a), \
					                 *(x   + _i*rs_x + _j*cs_x), \
					                 *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_cscal2rpis( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_cscal2jrpis( *(a), \
					                 *(x   + _i*rs_x + _j*cs_x), \
					                 *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_cscal2rpis( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
	} \
}

#define bli_zscal2rihs_mxn_uplo( schema, uplo, conjx, m, a, x, rs_x, cs_x, y_r, rs_y, cs_y ) \
{ \
	dim_t _i, _j; \
\
	 \
	if ( bli_is_ro_packed( schema ) ) \
	{ \
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_zscal2jros( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_zscal2ros( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_zscal2jros( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_zscal2ros( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
	} \
	else if ( bli_is_io_packed( schema ) ) \
	{ \
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_zscal2jios( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_zscal2ios( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_zscal2jios( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_zscal2ios( *(a), \
					               *(x   + _i*rs_x + _j*cs_x), \
					               *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
	} \
	else  \
	{ \
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_zscal2jrpis( *(a), \
					                 *(x   + _i*rs_x + _j*cs_x), \
					                 *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = _j; _i < m; ++_i ) \
				{ \
					bli_zscal2rpis( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_zscal2jrpis( *(a), \
					                 *(x   + _i*rs_x + _j*cs_x), \
					                 *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( _j = 0; _j < m; ++_j ) \
				for ( _i = 0; _i < _j + 1; ++_i ) \
				{ \
					bli_zscal2rpis( *(a), \
					                *(x   + _i*rs_x + _j*cs_x), \
					                *(y_r + _i*rs_y + _j*cs_y) ); \
				} \
			} \
		} \
	} \
}

#endif
// end bli_scal2rihs_mxn_uplo.h
// begin bli_setrihs_mxn_diag.h


#ifndef BLIS_SETRIHS_MXN_DIAG_H
#define BLIS_SETRIHS_MXN_DIAG_H

// setrihs_mxn_diag

#define bli_csetrihs_mxn_diag( schema, m, n, a, y_r, rs_y, cs_y ) \
{ \
	const float  a_r     = bli_zreal( *a ); \
	const float  a_i     = bli_zimag( *a ); \
	dim_t        min_m_n = bli_min( m, n ); \
	dim_t        _i; \
\
	 \
	if ( bli_is_ro_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_scopys(  (a_r), \
			            *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else if ( bli_is_io_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_scopys(  (a_i), \
			            *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_sadd3s(  (a_r), \
			             (a_i), \
			            *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
}

#define bli_zsetrihs_mxn_diag( schema, m, n, a, y_r, rs_y, cs_y ) \
{ \
	const double a_r     = bli_zreal( *a ); \
	const double a_i     = bli_zimag( *a ); \
	dim_t        min_m_n = bli_min( m, n ); \
	dim_t        _i; \
\
	 \
	if ( bli_is_ro_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_dcopys(  (a_r), \
			            *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else if ( bli_is_io_packed( schema ) ) \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_dcopys(  (a_i), \
			            *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		for ( _i = 0; _i < min_m_n; ++_i ) \
		{ \
			bli_dadd3s(  (a_r), \
			             (a_i), \
			            *(y_r + _i*rs_y + _i*cs_y) ); \
		} \
	} \
}

#endif
// end bli_setrihs_mxn_diag.h

// ro
// begin bli_scal2ros.h


#ifndef BLIS_SCAL2ROS_H
#define BLIS_SCAL2ROS_H

// scal2ros

#define bli_cscal2ros( a, x, yr ) \
{ \
	(yr) = bli_creal(a) * bli_creal(x) - bli_cimag(a) * bli_cimag(x); \
}

#define bli_zscal2ros( a, x, yr ) \
{ \
	(yr) = bli_zreal(a) * bli_zreal(x) - bli_zimag(a) * bli_zimag(x); \
}

#define bli_scscal2ros( a, x, yr ) \
{ \
	(yr) = bli_creal(a) * bli_creal(x); \
}

#define bli_dzscal2ros( a, x, yr ) \
{ \
	(yr) = bli_zreal(a) * bli_zreal(x); \
}


#endif

// end bli_scal2ros.h
// begin bli_scal2jros.h


#ifndef BLIS_SCAL2JROS_H
#define BLIS_SCAL2JROS_H

// scal2jros

#define bli_cscal2jros( a, x, yr ) \
{ \
	(yr) = bli_creal(a) * bli_creal(x) + bli_cimag(a) * bli_cimag(x); \
}

#define bli_zscal2jros( a, x, yr ) \
{ \
	(yr) = bli_zreal(a) * bli_zreal(x) + bli_zimag(a) * bli_zimag(x); \
}

#endif

// end bli_scal2jros.h

// io
// begin bli_scal2ios.h


#ifndef BLIS_SCAL2IOS_H
#define BLIS_SCAL2IOS_H

// scal2ios

#define bli_cscal2ios( a, x, yi ) \
{ \
	(yi) = bli_cimag(a) * bli_creal(x) + bli_creal(a) * bli_cimag(x); \
}

#define bli_zscal2ios( a, x, yi ) \
{ \
	(yi) = bli_zimag(a) * bli_zreal(x) + bli_zreal(a) * bli_zimag(x); \
}

#define bli_scscal2ios( a, x, yi ) \
{ \
	(yi) = bli_creal(a) * bli_cimag(x); \
}

#define bli_dzscal2ios( a, x, yi ) \
{ \
	(yi) = bli_zreal(a) * bli_zimag(x); \
}

#endif

// end bli_scal2ios.h
// begin bli_scal2jios.h


#ifndef BLIS_SCAL2JIOS_H
#define BLIS_SCAL2JIOS_H

// scal2jios

#define bli_cscal2jios( a, x, yi ) \
{ \
	(yi) = bli_cimag(a) * bli_creal(x) - bli_creal(a) * bli_cimag(x); \
}

#define bli_zscal2jios( a, x, yi ) \
{ \
	(yi) = bli_zimag(a) * bli_zreal(x) - bli_zreal(a) * bli_zimag(x); \
}


#endif

// end bli_scal2jios.h

// rpi
// begin bli_scal2rpis.h


#ifndef BLIS_SCAL2RPIS_H
#define BLIS_SCAL2RPIS_H

// scal2rpis

#define bli_cscal2rpis( a, x, yrpi ) \
{ \
	(yrpi) = (bli_creal(a)+bli_cimag(a)) * bli_creal(x) + \
	         (bli_creal(a)-bli_cimag(a)) * bli_cimag(x); \
}

#define bli_zscal2rpis( a, x, yrpi ) \
{ \
	(yrpi) = (bli_zreal(a)+bli_zimag(a)) * bli_zreal(x) + \
	         (bli_zreal(a)-bli_zimag(a)) * bli_zimag(x); \
}

#define bli_scscal2rpis( a, x, yrpi ) \
{ \
	(yrpi) = bli_creal(a) * bli_creal(x) + \
	         bli_creal(a) * bli_cimag(x); \
}

#define bli_dzscal2rpis( a, x, yrpi ) \
{ \
	(yrpi) = bli_zreal(a) * bli_zreal(x) + \
	         bli_zreal(a) * bli_zimag(x); \
}


#endif

// end bli_scal2rpis.h
// begin bli_scal2jrpis.h


#ifndef BLIS_SCAL2JRPIS_H
#define BLIS_SCAL2JRPIS_H

// scal2jrpis

#define bli_cscal2jrpis( a, x, yrpi ) \
{ \
	(yrpi) = (bli_creal(a)+bli_cimag(a)) * bli_creal(x) + \
	         (bli_cimag(a)-bli_creal(a)) * bli_cimag(x); \
}

#define bli_zscal2jrpis( a, x, yrpi ) \
{ \
	(yrpi) = (bli_zreal(a)+bli_zimag(a)) * bli_zreal(x) + \
	         (bli_zimag(a)-bli_zreal(a)) * bli_zimag(x); \
}

#endif

// end bli_scal2jrpis.h


// -- 1m-specific scalar macros --

// begin bli_invert1ms_mxn_diag.h


#ifndef BLIS_INVERT1MS_MXN_DIAG_H
#define BLIS_INVERT1MS_MXN_DIAG_H

// invert1ms_mxn_diag

#define bli_cinvert1ms_mxn_diag( schema, offm, offn, m, n, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		scomplex* restrict y_off_ri = y + (offm  )*rs_y \
		                                + (offn  )*cs_y; \
		scomplex* restrict y_off_ir = y + (offm  )*rs_y \
		                                + (offn  )*cs_y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_cinvert1es( *(y_off_ri + i*rs_y + i*cs_y), \
			                *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		float*    restrict y_cast  = ( float* )y; \
		float*    restrict y_off_r = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2; \
		float*    restrict y_off_i = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2 + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_cinvert1rs( *(y_off_r + i*rs_y2 + i*cs_y2), \
			                *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#define bli_zinvert1ms_mxn_diag( schema, offm, offn, m, n, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		dcomplex* restrict y_off_ri = y + (offm  )*rs_y \
		                                + (offn  )*cs_y; \
		dcomplex* restrict y_off_ir = y + (offm  )*rs_y \
		                                + (offn  )*cs_y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_zinvert1es( *(y_off_ri + i*rs_y + i*cs_y), \
			                *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		double*   restrict y_cast  = ( double* )y; \
		double*   restrict y_off_r = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2; \
		double*   restrict y_off_i = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2 + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_zinvert1rs( *(y_off_r + i*rs_y2 + i*cs_y2), \
			                *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#endif
// end bli_invert1ms_mxn_diag.h

// begin bli_scal1ms_mxn.h


#ifndef BLIS_SCAL1MS_MXN_H
#define BLIS_SCAL1MS_MXN_H

// scal1ms_mxn

#define bli_cscal1ms_mxn( schema, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t i, j; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		scomplex* restrict y_ri = y; \
		scomplex* restrict y_ir = y + ld_y/2; \
\
		for ( j = 0; j < n; ++j ) \
		for ( i = 0; i < m; ++i ) \
		{ \
			bli_cscal1es( *(a), \
			              *(y_ri + i*rs_y + j*cs_y), \
			              *(y_ir + i*rs_y + j*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		float*    restrict y_cast = ( float* )y; \
		float*    restrict y_r    = y_cast; \
		float*    restrict y_i    = y_cast + ld_y; \
\
		for ( j = 0; j < n; ++j ) \
		for ( i = 0; i < m; ++i ) \
		{ \
			bli_cscal1rs( *(a), \
			              *(y_r + i*rs_y2 + j*cs_y2), \
			              *(y_i + i*rs_y2 + j*cs_y2) ); \
		} \
	} \
}

#define bli_zscal1ms_mxn( schema, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t i, j; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		dcomplex* restrict y_ri = y; \
		dcomplex* restrict y_ir = y + ld_y/2; \
\
		for ( j = 0; j < n; ++j ) \
		for ( i = 0; i < m; ++i ) \
		{ \
			bli_zscal1es( *(a), \
			              *(y_ri + i*rs_y + j*cs_y), \
			              *(y_ir + i*rs_y + j*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		double*   restrict y_cast = ( double* )y; \
		double*   restrict y_r    = y_cast; \
		double*   restrict y_i    = y_cast + ld_y; \
\
		for ( j = 0; j < n; ++j ) \
		for ( i = 0; i < m; ++i ) \
		{ \
			bli_zscal1rs( *(a), \
			              *(y_r + i*rs_y2 + j*cs_y2), \
			              *(y_i + i*rs_y2 + j*cs_y2) ); \
		} \
	} \
}

#endif
// end bli_scal1ms_mxn.h

// begin bli_scal21ms_mxn_diag.h


#ifndef BLIS_SCAL21MS_MXN_DIAG_H
#define BLIS_SCAL21MS_MXN_DIAG_H

// scal21ms_mxn_diag

#define bli_cscscal21ms_mxn_diag( schema, m, n, a, x, rs_x, cs_x, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		scomplex* restrict y_off_ri = y; \
		scomplex* restrict y_off_ir = y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_scscal21es( *(x        + i*rs_x + i*cs_x), \
			                *(a), \
			                *(y_off_ri + i*rs_y + i*cs_y), \
			                *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		float*    restrict y_cast  = ( float* )y; \
		float*    restrict y_off_r = y_cast; \
		float*    restrict y_off_i = y_cast + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_scscal21rs( *(x        + i*rs_x + i*cs_x), \
			                *(a), \
			                *(y_off_r + i*rs_y2 + i*cs_y2), \
			                *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#define bli_zdzscal21ms_mxn_diag( schema, m, n, a, x, rs_x, cs_x, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		dcomplex* restrict y_off_ri = y; \
		dcomplex* restrict y_off_ir = y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_dzscal21es( *(x        + i*rs_x + i*cs_x), \
			                *(a), \
			                *(y_off_ri + i*rs_y + i*cs_y), \
			                *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		double*   restrict y_cast  = ( double* )y; \
		double*   restrict y_off_r = y_cast; \
		double*   restrict y_off_i = y_cast + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_dzscal21rs( *(x        + i*rs_x + i*cs_x), \
			                *(a), \
			                *(y_off_r + i*rs_y2 + i*cs_y2), \
			                *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#endif
// end bli_scal21ms_mxn_diag.h
// begin bli_scal21ms_mxn_uplo.h


#ifndef BLIS_SCAL21MS_MXN_UPLO_H
#define BLIS_SCAL21MS_MXN_UPLO_H

// scal21ms_mxn_uplo

#define bli_cscal21ms_mxn_uplo( schema, uplo, conjx, m, a, x, rs_x, cs_x, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t i, j; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		scomplex* restrict y_ri = y; \
		scomplex* restrict y_ir = y + ld_y/2; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_cscal2j1es( *(a), \
					                *(x    + i*rs_x + j*cs_x), \
					                *(y_ri + i*rs_y + j*cs_y), \
					                *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_cscal21es( *(a), \
					               *(x    + i*rs_x + j*cs_x), \
					               *(y_ri + i*rs_y + j*cs_y), \
					               *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_cscal2j1es( *(a), \
					                *(x    + i*rs_x + j*cs_x), \
					                *(y_ri + i*rs_y + j*cs_y), \
					                *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_cscal21es( *(a), \
					               *(x    + i*rs_x + j*cs_x), \
					               *(y_ri + i*rs_y + j*cs_y), \
					               *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		float*    restrict y_cast = ( float* )y; \
		float*    restrict y_r    = y_cast; \
		float*    restrict y_i    = y_cast + ld_y; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_cscal2j1rs( *(a), \
					                *(x   + i*rs_x  + j*cs_x ), \
					                *(y_r + i*rs_y2 + j*cs_y2), \
					                *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_cscal21rs( *(a), \
					               *(x   + i*rs_x  + j*cs_x ), \
					               *(y_r + i*rs_y2 + j*cs_y2), \
					               *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_cscal2j1rs( *(a), \
					                *(x   + i*rs_x  + j*cs_x ), \
					                *(y_r + i*rs_y2 + j*cs_y2), \
					                *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_cscal21rs( *(a), \
					               *(x   + i*rs_x  + j*cs_x ), \
					               *(y_r + i*rs_y2 + j*cs_y2), \
					               *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
		} \
	} \
}

#define bli_zscal21ms_mxn_uplo( schema, uplo, conjx, m, a, x, rs_x, cs_x, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t i, j; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		dcomplex* restrict y_ri = y; \
		dcomplex* restrict y_ir = y + ld_y/2; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_zscal2j1es( *(a), \
					                *(x    + i*rs_x + j*cs_x), \
					                *(y_ri + i*rs_y + j*cs_y), \
					                *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_zscal21es( *(a), \
					               *(x    + i*rs_x + j*cs_x), \
					               *(y_ri + i*rs_y + j*cs_y), \
					               *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_zscal2j1es( *(a), \
					                *(x    + i*rs_x + j*cs_x), \
					                *(y_ri + i*rs_y + j*cs_y), \
					                *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_zscal21es( *(a), \
					               *(x    + i*rs_x + j*cs_x), \
					               *(y_ri + i*rs_y + j*cs_y), \
					               *(y_ir + i*rs_y + j*cs_y) ); \
				} \
			} \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		double*   restrict y_cast = ( double* )y; \
		double*   restrict y_r    = y_cast; \
		double*   restrict y_i    = y_cast + ld_y; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_zscal2j1rs( *(a), \
					                *(x   + i*rs_x  + j*cs_x ), \
					                *(y_r + i*rs_y2 + j*cs_y2), \
					                *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = j; i < m; ++i ) \
				{ \
					bli_zscal21rs( *(a), \
					               *(x   + i*rs_x  + j*cs_x ), \
					               *(y_r + i*rs_y2 + j*cs_y2), \
					               *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
		} \
		else  \
		{ \
			if ( bli_is_conj( conjx ) ) \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_zscal2j1rs( *(a), \
					                *(x   + i*rs_x  + j*cs_x ), \
					                *(y_r + i*rs_y2 + j*cs_y2), \
					                *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
			else  \
			{ \
				for ( j = 0; j < m; ++j ) \
				for ( i = 0; i < j + 1; ++i ) \
				{ \
					bli_zscal21rs( *(a), \
					               *(x   + i*rs_x  + j*cs_x ), \
					               *(y_r + i*rs_y2 + j*cs_y2), \
					               *(y_i + i*rs_y2 + j*cs_y2) ); \
				} \
			} \
		} \
	} \
}

#endif
// end bli_scal21ms_mxn_uplo.h

// begin bli_set1ms_mxn.h


#ifndef BLIS_SET1MS_MXN_H
#define BLIS_SET1MS_MXN_H

// set1ms_mxn

#define bli_cset1ms_mxn( schema, offm, offn, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	inc_t offm_local = offm; \
	inc_t offn_local = offn; \
	dim_t m_local    = m; \
	dim_t n_local    = n; \
	inc_t rs_y1      = rs_y; \
	inc_t cs_y1      = cs_y; \
	inc_t rs_y2      = rs_y; \
	inc_t cs_y2      = cs_y; \
	dim_t i, j; \
\
	 \
	if ( cs_y == 1 ) \
	{ \
		bli_swap_incs( offm_local, offn_local ); \
		bli_swap_dims( m_local, n_local ); \
		bli_swap_incs( rs_y1, cs_y1 ); \
		bli_swap_incs( rs_y2, cs_y2 ); \
	} \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		scomplex* restrict y_off_ri = y + (offm_local  )*rs_y1 \
		                                + (offn_local  )*cs_y1; \
		scomplex* restrict y_off_ir = y + (offm_local  )*rs_y1 \
		                                + (offn_local  )*cs_y1 + ld_y/2; \
\
		for ( j = 0; j < n_local; ++j ) \
		for ( i = 0; i < m_local; ++i ) \
		{ \
			bli_ccopy1es( *(a), \
			              *(y_off_ri + i*rs_y1 + j*cs_y1), \
			              *(y_off_ir + i*rs_y1 + j*cs_y1) ); \
		} \
	} \
	else  \
	{ \
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		float*    restrict y_cast  = ( float* )y; \
		float*    restrict y_off_r = y_cast + (offm_local  )*rs_y2 \
		                                    + (offn_local  )*cs_y2; \
		float*    restrict y_off_i = y_cast + (offm_local  )*rs_y2 \
		                                    + (offn_local  )*cs_y2 + ld_y; \
\
		for ( j = 0; j < n_local; ++j ) \
		for ( i = 0; i < m_local; ++i ) \
		{ \
			bli_ccopy1rs( *(a), \
			              *(y_off_r + i*rs_y2 + j*cs_y2), \
			              *(y_off_i + i*rs_y2 + j*cs_y2) ); \
		} \
	} \
}

#define bli_zset1ms_mxn( schema, offm, offn, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	inc_t offm_local = offm; \
	inc_t offn_local = offn; \
	dim_t m_local    = m; \
	dim_t n_local    = n; \
	inc_t rs_y1      = rs_y; \
	inc_t cs_y1      = cs_y; \
	inc_t rs_y2      = rs_y; \
	inc_t cs_y2      = cs_y; \
	dim_t i, j; \
\
	 \
	if ( cs_y == 1 ) \
	{ \
		bli_swap_incs( offm_local, offn_local ); \
		bli_swap_dims( m_local, n_local ); \
		bli_swap_incs( rs_y1, cs_y1 ); \
		bli_swap_incs( rs_y2, cs_y2 ); \
	} \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		dcomplex* restrict y_off_ri = y + (offm_local  )*rs_y1 \
		                                + (offn_local  )*cs_y1; \
		dcomplex* restrict y_off_ir = y + (offm_local  )*rs_y1 \
		                                + (offn_local  )*cs_y1 + ld_y/2; \
\
		for ( j = 0; j < n_local; ++j ) \
		for ( i = 0; i < m_local; ++i ) \
		{ \
			bli_zcopy1es( *(a), \
			              *(y_off_ri + i*rs_y1 + j*cs_y1), \
			              *(y_off_ir + i*rs_y1 + j*cs_y1) ); \
		} \
	} \
	else  \
	{ \
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		double*   restrict y_cast  = ( double* )y; \
		double*   restrict y_off_r = y_cast + (offm_local  )*rs_y2 \
		                                    + (offn_local  )*cs_y2; \
		double*   restrict y_off_i = y_cast + (offm_local  )*rs_y2 \
		                                    + (offn_local  )*cs_y2 + ld_y; \
\
		for ( j = 0; j < n_local; ++j ) \
		for ( i = 0; i < m_local; ++i ) \
		{ \
			bli_zcopy1rs( *(a), \
			              *(y_off_r + i*rs_y2 + j*cs_y2), \
			              *(y_off_i + i*rs_y2 + j*cs_y2) ); \
		} \
	} \
}

#endif
// end bli_set1ms_mxn.h
// begin bli_set1ms_mxn_diag.h


#ifndef BLIS_SET1MS_MXN_DIAG_H
#define BLIS_SET1MS_MXN_DIAG_H

// set1ms_mxn_diag

#define bli_cset1ms_mxn_diag( schema, offm, offn, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		scomplex* restrict y_off_ri = y + (offm  )*rs_y \
		                                + (offn  )*cs_y; \
		scomplex* restrict y_off_ir = y + (offm  )*rs_y \
		                                + (offn  )*cs_y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_ccopy1es( *(a), \
			              *(y_off_ri + i*rs_y + i*cs_y), \
			              *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		float*    restrict y_cast  = ( float* )y; \
		float*    restrict y_off_r = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2; \
		float*    restrict y_off_i = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2 + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_ccopy1rs( *(a), \
			              *(y_off_r + i*rs_y2 + i*cs_y2), \
			              *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#define bli_zset1ms_mxn_diag( schema, offm, offn, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		dcomplex* restrict y_off_ri = y + (offm  )*rs_y \
		                                + (offn  )*cs_y; \
		dcomplex* restrict y_off_ir = y + (offm  )*rs_y \
		                                + (offn  )*cs_y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_zcopy1es( *(a), \
			              *(y_off_ri + i*rs_y + i*cs_y), \
			              *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		double*   restrict y_cast  = ( double* )y; \
		double*   restrict y_off_r = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2; \
		double*   restrict y_off_i = y_cast + (offm  )*rs_y2 \
		                                    + (offn  )*cs_y2 + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_zcopy1rs( *(a), \
			              *(y_off_r + i*rs_y2 + i*cs_y2), \
			              *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#endif
// end bli_set1ms_mxn_diag.h
// begin bli_set1ms_mxn_uplo.h


#ifndef BLIS_SET1MS_MXN_UPLO_H
#define BLIS_SET1MS_MXN_UPLO_H

// set1ms_mxn_uplo

#define bli_cset1ms_mxn_uplo( schema, diagoff, uplo, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	doff_t diagoff_abs = bli_abs( diagoff ); \
	inc_t  offdiag_inc; \
	dim_t  i, j; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		 \
		if         ( diagoff > 0 )    offdiag_inc = cs_y; \
		else  offdiag_inc = rs_y; \
\
		scomplex* restrict y0   = y + (diagoff_abs  )*offdiag_inc; \
		scomplex* restrict y_ri = y0; \
		scomplex* restrict y_ir = y0 + ld_y/2; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = j; i < m; ++i ) \
			{ \
				bli_ccopy1es( *(a), \
				              *(y_ri + i*rs_y + j*cs_y), \
				              *(y_ir + i*rs_y + j*cs_y) ); \
			} \
		} \
		else  \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = 0; i < j + 1; ++i ) \
			{ \
				bli_ccopy1es( *(a), \
				              *(y_ri + i*rs_y + j*cs_y), \
				              *(y_ir + i*rs_y + j*cs_y) ); \
			} \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		 \
		if         ( diagoff > 0 )    offdiag_inc = cs_y2; \
		else  offdiag_inc = rs_y2; \
\
		float*    restrict y0  = ( float* )y + (diagoff_abs  )*offdiag_inc; \
		float*    restrict y_r = y0; \
		float*    restrict y_i = y0 + ld_y; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = j; i < m; ++i ) \
			{ \
				bli_ccopy1rs( *(a), \
				              *(y_r + i*rs_y2 + j*cs_y2), \
				              *(y_i + i*rs_y2 + j*cs_y2) ); \
			} \
		} \
		else  \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = 0; i < j + 1; ++i ) \
			{ \
				bli_ccopy1rs( *(a), \
				              *(y_r + i*rs_y2 + j*cs_y2), \
				              *(y_i + i*rs_y2 + j*cs_y2) ); \
			} \
		} \
	} \
}

#define bli_zset1ms_mxn_uplo( schema, diagoff, uplo, m, n, a, y, rs_y, cs_y, ld_y ) \
{ \
	doff_t diagoff_abs = bli_abs( diagoff ); \
	inc_t  offdiag_inc; \
	dim_t  i, j; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		 \
		if         ( diagoff > 0 )    offdiag_inc = cs_y; \
		else  offdiag_inc = rs_y; \
\
		dcomplex* restrict y0   = y + (diagoff_abs  )*offdiag_inc; \
		dcomplex* restrict y_ri = y0; \
		dcomplex* restrict y_ir = y0 + ld_y/2; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = j; i < m; ++i ) \
			{ \
				bli_zcopy1es( *(a), \
				              *(y_ri + i*rs_y + j*cs_y), \
				              *(y_ir + i*rs_y + j*cs_y) ); \
			} \
		} \
		else  \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = 0; i < j + 1; ++i ) \
			{ \
				bli_zcopy1es( *(a), \
				              *(y_ri + i*rs_y + j*cs_y), \
				              *(y_ir + i*rs_y + j*cs_y) ); \
			} \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		 \
		if         ( diagoff > 0 )    offdiag_inc = cs_y2; \
		else  offdiag_inc = rs_y2; \
\
		double*   restrict y0  = ( double* )y + (diagoff_abs  )*offdiag_inc; \
		double*   restrict y_r = y0; \
		double*   restrict y_i = y0 + ld_y; \
\
		if ( bli_is_lower( uplo ) ) \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = j; i < m; ++i ) \
			{ \
				bli_zcopy1rs( *(a), \
				              *(y_r + i*rs_y2 + j*cs_y2), \
				              *(y_i + i*rs_y2 + j*cs_y2) ); \
			} \
		} \
		else  \
		{ \
			for ( j = 0; j < n; ++j ) \
			for ( i = 0; i < j + 1; ++i ) \
			{ \
				bli_zcopy1rs( *(a), \
				              *(y_r + i*rs_y2 + j*cs_y2), \
				              *(y_i + i*rs_y2 + j*cs_y2) ); \
			} \
		} \
	} \
}

#endif
// end bli_set1ms_mxn_uplo.h
// begin bli_seti01ms_mxn_diag.h


#ifndef BLIS_SETI01MS_MXN_DIAG_H
#define BLIS_SETI01MS_MXN_DIAG_H

// seti01ms_mxn_diag

#define bli_cseti01ms_mxn_diag( schema, m, n, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		scomplex* restrict y_off_ri = y; \
		scomplex* restrict y_off_ir = y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_cseti0s( *(y_off_ri + i*rs_y + i*cs_y) ); \
			bli_csetr0s( *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		float*    restrict y_cast  = ( float* )y; \
		float*    restrict y_off_i = y_cast + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_sset0s( *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#define bli_zseti01ms_mxn_diag( schema, m, n, y, rs_y, cs_y, ld_y ) \
{ \
	dim_t min_m_n = bli_min( m, n ); \
	dim_t i; \
\
	 \
	if ( bli_is_1e_packed( schema ) ) \
	{ \
		dcomplex* restrict y_off_ri = y; \
		dcomplex* restrict y_off_ir = y + ld_y/2; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_zseti0s( *(y_off_ri + i*rs_y + i*cs_y) ); \
			bli_zsetr0s( *(y_off_ir + i*rs_y + i*cs_y) ); \
		} \
	} \
	else  \
	{ \
		inc_t rs_y2 = rs_y; \
		inc_t cs_y2 = cs_y; \
\
		 \
		if         ( rs_y2 == 1 )    { cs_y2 *= 2; } \
		else  { rs_y2 *= 2; } \
\
		double*   restrict y_cast  = ( double* )y; \
		double*   restrict y_off_i = y_cast + ld_y; \
\
		for ( i = 0; i < min_m_n; ++i ) \
		{ \
			bli_dset0s( *(y_off_i + i*rs_y2 + i*cs_y2) ); \
		} \
	} \
}

#endif
// end bli_seti01ms_mxn_diag.h

// 1e
// begin bli_copy1es.h


#ifndef BLIS_COPY1ES_H
#define BLIS_COPY1ES_H

// copy1es

#define bli_ccopy1es( a, bri, bir ) \
{ \
	bli_ccopyris(  bli_creal(a), bli_cimag(a), bli_creal(bri), bli_cimag(bri) ); \
	bli_ccopyris( -bli_cimag(a), bli_creal(a), bli_creal(bir), bli_cimag(bir) ); \
}

#define bli_zcopy1es( a, bri, bir ) \
{ \
	bli_zcopyris(  bli_zreal(a), bli_zimag(a), bli_zreal(bri), bli_zimag(bri) ); \
	bli_zcopyris( -bli_zimag(a), bli_zreal(a), bli_zreal(bir), bli_zimag(bir) ); \
}

#endif

// end bli_copy1es.h
// begin bli_copyj1es.h


#ifndef BLIS_COPYJ1ES_H
#define BLIS_COPYJ1ES_H

// copyj1es

#define bli_ccopyj1es( a, bri, bir ) \
{ \
	bli_ccopyris( bli_creal(a), -bli_cimag(a), bli_creal(bri), bli_cimag(bri) ); \
	bli_ccopyris( bli_cimag(a),  bli_creal(a), bli_creal(bir), bli_cimag(bir) ); \
}

#define bli_zcopyj1es( a, bri, bir ) \
{ \
	bli_zcopyris( bli_zreal(a), -bli_zimag(a), bli_zreal(bri), bli_zimag(bri) ); \
	bli_zcopyris( bli_zimag(a),  bli_zreal(a), bli_zreal(bir), bli_zimag(bir) ); \
}

#endif

// end bli_copyj1es.h

// begin bli_invert1es.h


#ifndef BLIS_INVERT1ES_H
#define BLIS_INVERT1ES_H

// invert1es

#define bli_cinvert1es( bri, bir ) \
{ \
	bli_cinvertris( bli_creal(bri), bli_cimag(bri) ); \
	bli_ccopyris( bli_creal(bri), -bli_cimag(bri), bli_cimag(bir), bli_creal(bir) ); \
}

#define bli_zinvert1es( bri, bir ) \
{ \
	bli_zinvertris( bli_zreal(bri), bli_zimag(bri) ); \
	bli_zcopyris( bli_zreal(bri), -bli_zimag(bri), bli_zimag(bir), bli_zreal(bir) ); \
}

#endif

// end bli_invert1es.h

// begin bli_scal1es.h


#ifndef BLIS_SCAL1ES_H
#define BLIS_SCAL1ES_H

// scal1es

#define bli_cscal1es( a, yri, yir ) \
{ \
	bli_cscalris(  bli_creal(a),   bli_cimag(a),   bli_creal(yri), bli_cimag(yri) ); \
	bli_ccopyris( -bli_cimag(yri), bli_creal(yri), bli_creal(yir), bli_cimag(yir) ); \
}

#define bli_zscal1es( a, yri, yir ) \
{ \
	bli_zscalris(  bli_zreal(a),   bli_zimag(a),   bli_zreal(yri), bli_zimag(yri) ); \
	bli_zcopyris( -bli_zimag(yri), bli_zreal(yri), bli_zreal(yir), bli_zimag(yir) ); \
}

#endif

// end bli_scal1es.h

// begin bli_scal21es.h


#ifndef BLIS_SCAL21ES_H
#define BLIS_SCAL21ES_H

// scal21es

#define bli_cscal21es( a, x, yri, yir ) \
{ \
	bli_cscal2ris( bli_creal(a), bli_cimag(a),  bli_creal(x), bli_cimag(x), bli_creal(yri), bli_cimag(yri) ); \
	bli_cscal2ris( bli_creal(a), bli_cimag(a), -bli_cimag(x), bli_creal(x), bli_creal(yir), bli_cimag(yir) ); \
}

#define bli_zscal21es( a, x, yri, yir ) \
{ \
	bli_zscal2ris( bli_zreal(a), bli_zimag(a),  bli_zreal(x), bli_zimag(x), bli_zreal(yri), bli_zimag(yri) ); \
	bli_zscal2ris( bli_zreal(a), bli_zimag(a), -bli_zimag(x), bli_zreal(x), bli_zreal(yir), bli_zimag(yir) ); \
}

#define bli_scscal21es( a, x, yri, yir ) \
{ \
	bli_scscal2ris( bli_sreal(a), bli_simag(a),  bli_creal(x), bli_cimag(x), bli_creal(yri), bli_cimag(yri) ); \
	bli_scscal2ris( bli_sreal(a), bli_simag(a), -bli_cimag(x), bli_creal(x), bli_creal(yir), bli_cimag(yir) ); \
}

#define bli_dzscal21es( a, x, yri, yir ) \
{ \
	bli_dzscal2ris( bli_dreal(a), bli_dimag(a),  bli_zreal(x), bli_zimag(x), bli_zreal(yri), bli_zimag(yri) ); \
	bli_dzscal2ris( bli_dreal(a), bli_dimag(a), -bli_zimag(x), bli_zreal(x), bli_zreal(yir), bli_zimag(yir) ); \
}

#endif

// end bli_scal21es.h
// begin bli_scal2j1es.h


#ifndef BLIS_SCAL2J1ES_H
#define BLIS_SCAL2J1ES_H

// scal2j1es

#define bli_cscal2j1es( a, x, yri, yir ) \
{ \
	bli_cscal2ris( bli_creal(a), bli_cimag(a), bli_creal(x), -bli_cimag(x), bli_creal(yri), bli_cimag(yri) ); \
	bli_cscal2ris( bli_creal(a), bli_cimag(a), bli_cimag(x),  bli_creal(x), bli_creal(yir), bli_cimag(yir) ); \
}

#define bli_zscal2j1es( a, x, yri, yir ) \
{ \
	bli_zscal2ris( bli_zreal(a), bli_zimag(a), bli_zreal(x), -bli_zimag(x), bli_zreal(yri), bli_zimag(yri) ); \
	bli_zscal2ris( bli_zreal(a), bli_zimag(a), bli_zimag(x),  bli_zreal(x), bli_zreal(yir), bli_zimag(yir) ); \
}

#define bli_scscal2j1es( a, x, yri, yir ) \
{ \
	bli_scscal2ris( bli_sreal(a), bli_simag(a), bli_creal(x), -bli_cimag(x), bli_creal(yri), bli_cimag(yri) ); \
	bli_scscal2ris( bli_sreal(a), bli_simag(a), bli_cimag(x),  bli_creal(x), bli_creal(yir), bli_cimag(yir) ); \
}

#define bli_dzscal2j1es( a, x, yri, yir ) \
{ \
	bli_dzscal2ris( bli_dreal(a), bli_dimag(a), bli_zreal(x), -bli_zimag(x), bli_zreal(yri), bli_zimag(yri) ); \
	bli_dzscal2ris( bli_dreal(a), bli_dimag(a), bli_zimag(x),  bli_zreal(x), bli_zreal(yir), bli_zimag(yir) ); \
}

#endif

// end bli_scal2j1es.h

// 1r
// begin bli_copy1rs.h


#ifndef BLIS_COPY1RS_H
#define BLIS_COPY1RS_H

// copy1rs

#define bli_ccopy1rs( a, br, bi ) \
{ \
	bli_ccopyris( bli_creal(a), bli_cimag(a), br, bi ); \
}

#define bli_zcopy1rs( a, br, bi ) \
{ \
	bli_zcopyris( bli_zreal(a), bli_zimag(a), br, bi ); \
}

#endif

// end bli_copy1rs.h
// begin bli_copyj1rs.h


#ifndef BLIS_COPYJ1RS_H
#define BLIS_COPYJ1RS_H

// copyj1rs

#define bli_ccopyj1rs( a, br, bi ) \
{ \
	bli_ccopyjris( bli_creal(a), bli_cimag(a), br, bi ); \
}

#define bli_zcopyj1rs( a, br, bi ) \
{ \
	bli_zcopyjris( bli_zreal(a), bli_zimag(a), br, bi ); \
}

#endif

// end bli_copyj1rs.h

// begin bli_invert1rs.h


#ifndef BLIS_INVERT1RS_H
#define BLIS_INVERT1RS_H

// invert1rs

#define bli_cinvert1rs( xr, xi )  bli_cinvertris( xr, xi )
#define bli_zinvert1rs( xr, xi )  bli_zinvertris( xr, xi )

#endif
// end bli_invert1rs.h

// begin bli_scal1rs.h


#ifndef BLIS_SCAL1RS_H
#define BLIS_SCAL1RS_H

// scal1rs

#define bli_cscal1rs( a, yr, yi ) \
{ \
	bli_cscalris( bli_creal(a), bli_cimag(a), yr, yi ); \
}

#define bli_zscal1rs( a, yr, yi ) \
{ \
	bli_zscalris( bli_zreal(a), bli_zimag(a), yr, yi ); \
}

#define bli_scscal1rs( a, yr, yi ) \
{ \
	bli_scscalris( bli_sreal(a), bli_simag(a), yr, yi ); \
}

#define bli_dzscal1rs( a, yr, yi ) \
{ \
	bli_dzscalris( bli_dreal(a), bli_dimag(a), yr, yi ); \
}

#endif

// end bli_scal1rs.h

// begin bli_scal21rs.h


#ifndef BLIS_SCAL21RS_H
#define BLIS_SCAL21RS_H

// scal21rs

#define bli_cscal21rs( a, x, yr, yi ) \
{ \
	bli_cscal2ris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), yr, yi ); \
}

#define bli_zscal21rs( a, x, yr, yi ) \
{ \
	bli_zscal2ris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), yr, yi ); \
}

#define bli_scscal21rs( a, x, yr, yi ) \
{ \
	bli_scscal2ris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), yr, yi ); \
}

#define bli_dzscal21rs( a, x, yr, yi ) \
{ \
	bli_dzscal2ris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), yr, yi ); \
}

#endif

// end bli_scal21rs.h
// begin bli_scal2j1rs.h


#ifndef BLIS_SCAL2J1RS_H
#define BLIS_SCAL2J1RS_H

// scal2j1rs

#define bli_cscal2j1rs( a, x, yr, yi ) \
{ \
	bli_cscal2jris( bli_creal(a), bli_cimag(a), bli_creal(x), bli_cimag(x), yr, yi ); \
}

#define bli_zscal2j1rs( a, x, yr, yi ) \
{ \
	bli_zscal2jris( bli_zreal(a), bli_zimag(a), bli_zreal(x), bli_zimag(x), yr, yi ); \
}

#define bli_scscal2j1rs( a, x, yr, yi ) \
{ \
	bli_scscal2jris( bli_sreal(a), bli_simag(a), bli_creal(x), bli_cimag(x), yr, yi ); \
}

#define bli_dzscal2j1rs( a, x, yr, yi ) \
{ \
	bli_dzscal2jris( bli_dreal(a), bli_dimag(a), bli_zreal(x), bli_zimag(x), yr, yi ); \
}

#endif

// end bli_scal2j1rs.h



// -- Miscellaneous macros --

// min, max, abs

#define bli_min( a, b )  ( (a) < (b) ? (a) : (b) )
#define bli_max( a, b )  ( (a) > (b) ? (a) : (b) )
#define bli_abs( a )     ( (a) <= 0 ? -(a) : (a) )

// fmin, fmax, fabs

#define bli_fmin( a, b ) bli_min( a, b )
#define bli_fmax( a, b ) bli_max( a, b )
#define bli_fabs( a )    ( (a) <= 0.0 ? -(a) : (a) )

// fminabs, fmaxabs

#define bli_fminabs( a, b ) \
\
	bli_fmin( bli_fabs( a ), \
	          bli_fabs( b ) )

#define bli_fmaxabs( a, b ) \
\
	bli_fmax( bli_fabs( a ), \
	          bli_fabs( b ) )

// round

#define bli_round( val ) \
\
	( round( val ) )

// round_to_mult

#define bli_round_to_mult( val, mult ) \
\
	( guint_t )( ( ( ( guint_t )val + \
	                 ( guint_t )mult / 2 \
	               ) / mult \
	             ) * mult \
	           )

// isnan, isinf

#define bli_isinf( a )  isinf( a )
#define bli_isnan( a )  isnan( a )

// is_odd, is_even

#define bli_is_odd( a )  ( a % 2 == 1 )
#define bli_is_even( a ) ( a % 2 == 0 )

// swap_types

#define bli_swap_types( type1, type2 ) \
{ \
	num_t temp = type1; \
	type1 = type2; \
	type2 = temp; \
}

// swap_dims

#define bli_swap_dims( dim1, dim2 ) \
{ \
	dim_t temp = dim1; \
	dim1 = dim2; \
	dim2 = temp; \
}

// swap_incs

#define bli_swap_incs( inc1, inc2 ) \
{ \
	inc_t temp = inc1; \
	inc1 = inc2; \
	inc2 = temp; \
}

// toggle_bool

#define bli_toggle_bool( b ) \
{ \
	if ( b == TRUE ) b = FALSE; \
	else             b = TRUE; \
}


#endif
// end bli_scalar_macro_defs.h
// begin bli_error_macro_defs.h


#ifndef BLIS_ERROR_MACRO_DEFS_H
#define BLIS_ERROR_MACRO_DEFS_H

// -- Error-related macros --

// Used to determine the size of the array of error strings.
#define BLIS_MAX_NUM_ERR_MSGS    200
#define BLIS_MAX_ERR_MSG_LENGTH  200

// Used to insert filenames and line numbers into error-checking code.
#define bli_check_error_code( code ) \
        bli_check_error_code_helper( code, __FILE__, __LINE__ )


#endif

// end bli_error_macro_defs.h
// begin bli_blas_macro_defs.h


#ifndef BLIS_BLAS_MACRO_DEFS_H
#define BLIS_BLAS_MACRO_DEFS_H

// -- Various Fortran compatibility macros --

// Macro to treat negative dimensions as zero.

#define bli_convert_blas_dim1( n_blas, n_blis )\
{ \
	if ( n_blas < 0 ) n_blis = ( dim_t )0; \
	else              n_blis = ( dim_t )n_blas; \
}

// Macro to flip signs of increments if input increments are negative.

#define bli_convert_blas_incv( n, x_blas, incx_blas, \
                                  x_blis, incx_blis ) \
{ \
	if ( incx_blas < 0 ) \
	{ \
		 \
		x_blis    = (x_blas) + (n-1)*(-incx_blas); \
		incx_blis = ( inc_t )(incx_blas); \
	} \
	else \
	{ \
		x_blis    = (x_blas); \
		incx_blis = ( inc_t )(incx_blas); \
	} \
}



#endif

// end bli_blas_macro_defs.h


#endif
// end bli_macro_defs.h


// -- Threading definitions --

// begin bli_thread.h


#ifndef BLIS_THREAD_H
#define BLIS_THREAD_H

// Include thread communicator (thrcomm_t) object definitions and prototypes.
// begin bli_thrcomm.h


#ifndef BLIS_THRCOMM_H
#define BLIS_THRCOMM_H

// Include definitions (mostly thrcomm_t) specific to the method of
// multithreading.
// begin bli_thrcomm_single.h


#ifndef BLIS_THRCOMM_SINGLE_H
#define BLIS_THRCOMM_SINGLE_H

// Define thrcomm_t for situations when multithreading is disabled.
#ifndef BLIS_ENABLE_MULTITHREADING 

//thread communicators may be implementation dependent
#ifdef BLIS_TREE_BARRIER
struct barrier_s
{   
	int               arity;
	int               count;
	struct barrier_s* dad;
	int               signal;
};  
typedef struct barrier_s barrier_t;

struct thrcomm_s
{   
	void*       sent_object;
	dim_t       n_threads;
	barrier_t** barriers;
}; 
#else
struct thrcomm_s
{
	void*   sent_object;
	dim_t   n_threads;

	bool_t  barrier_sense;
	dim_t   barrier_threads_arrived;
};
#endif
typedef struct thrcomm_s thrcomm_t;

#endif

#endif

// end bli_thrcomm_single.h
// begin bli_thrcomm_openmp.h


#ifndef BLIS_THRCOMM_OPENMP_H
#define BLIS_THRCOMM_OPENMP_H

// Define thrcomm_t for situations when OpenMP multithreading is enabled.
#ifdef BLIS_ENABLE_OPENMP

#include <omp.h> // skipped

// Define thrcomm_t for tree barriers and non-tree barriers.
#ifdef BLIS_TREE_BARRIER
struct barrier_s
{   
	int               arity;
	int               count;
	struct barrier_s* dad;
	volatile int      signal;
};  
typedef struct barrier_s barrier_t;

struct thrcomm_s
{   
	void*       sent_object;
	dim_t       n_threads;
	barrier_t** barriers;
}; 
#else
struct thrcomm_s
{
	void*  sent_object;
	dim_t  n_threads;

	//volatile bool_t  barrier_sense;
	bool_t barrier_sense;
	dim_t  barrier_threads_arrived;
};
#endif

typedef struct thrcomm_s thrcomm_t;

// Prototypes specific to tree barriers.
#ifdef BLIS_TREE_BARRIER
barrier_t* bli_thrcomm_tree_barrier_create( int num_threads, int arity, barrier_t** leaves, int leaf_index );
void       bli_thrcomm_tree_barrier_free( barrier_t* barrier );
void       bli_thrcomm_tree_barrier( barrier_t* barack );
#endif

#endif

#endif

// end bli_thrcomm_openmp.h
// begin bli_thrcomm_pthreads.h


#ifndef BLIS_THRCOMM_PTHREADS_H
#define BLIS_THRCOMM_PTHREADS_H

// Define thrcomm_t for situations when POSIX multithreading is enabled.
#ifdef BLIS_ENABLE_PTHREADS 

#include <pthread.h> // skipped

#ifdef BLIS_USE_PTHREAD_BARRIER
struct thrcomm_s
{
	void*             sent_object;
	dim_t             n_threads;

	pthread_barrier_t barrier;
};
#else
struct thrcomm_s
{
	void*  sent_object;
	dim_t  n_threads;

//#ifdef BLIS_USE_PTHREAD_MUTEX
//	pthread_mutex_t mutex;
//#endif

	//volatile bool_t  barrier_sense;
	bool_t barrier_sense;
	dim_t  barrier_threads_arrived;
};
#endif

typedef struct thrcomm_s thrcomm_t;

#endif

#endif

// end bli_thrcomm_pthreads.h


// thrcomm_t query (field only)

static dim_t bli_thrcomm_num_threads( thrcomm_t* comm )
{
	return comm->n_threads;
}


// Thread communicator prototypes.
thrcomm_t* bli_thrcomm_create( dim_t n_threads );
void       bli_thrcomm_free( thrcomm_t* comm );
void       bli_thrcomm_init( thrcomm_t* comm, dim_t n_threads );
void       bli_thrcomm_cleanup( thrcomm_t* comm );
void       bli_thrcomm_barrier( thrcomm_t* comm, dim_t thread_id );
void*      bli_thrcomm_bcast( thrcomm_t* comm, dim_t inside_id, void* to_send );

void       bli_thrcomm_barrier_atomic( thrcomm_t* comm, dim_t t_id );

#endif

// end bli_thrcomm.h

// Include thread info (thrinfo_t) object definitions and prototypes.
// begin bli_thrinfo.h


#ifndef BLIS_THRINFO_H
#define BLIS_THRINFO_H

// Thread info structure definition
struct thrinfo_s
{
	// The thread communicator for the other threads sharing the same work
	// at this level.
	thrcomm_t*         ocomm;

	// Our thread id within the ocomm thread communicator.
	dim_t              ocomm_id;

	// The number of distinct threads used to parallelize the loop.
	dim_t              n_way;

	// What we're working on.
	dim_t              work_id;

	// When freeing, should the communicators in this node be freed? Usually,
	// this is field is true, but when nodes are created that share the same
	// communicators as other nodes (such as with packm nodes), this is set
	// to false.
	bool_t             free_comm;

	struct thrinfo_s*  sub_node;
};
typedef struct thrinfo_s thrinfo_t;

//
// thrinfo_t functions
// NOTE: The naming of these should be made consistent at some point.
// (ie: bli_thrinfo_ vs. bli_thread_)
//

// thrinfo_t query (field only)

static dim_t bli_thread_num_threads( thrinfo_t* t )
{
	return (t->ocomm)->n_threads;
}

static dim_t bli_thread_ocomm_id( thrinfo_t* t )
{
	return t->ocomm_id;
}

static dim_t bli_thread_n_way( thrinfo_t* t )
{
	return t->n_way;
}

static dim_t bli_thread_work_id( thrinfo_t* t )
{
	return t->work_id;
}

static thrcomm_t* bli_thrinfo_ocomm( thrinfo_t* t )
{
	return t->ocomm;
}

static bool_t bli_thrinfo_needs_free_comm( thrinfo_t* t )
{
	return t->free_comm;
}

static thrinfo_t* bli_thrinfo_sub_node( thrinfo_t* t )
{
	return t->sub_node;
}

// thrinfo_t query (complex)

static bool_t bli_thread_am_ochief( thrinfo_t* t )
{
	return t->ocomm_id == 0;
}

// thrinfo_t modification

static void bli_thrinfo_set_sub_node( thrinfo_t* sub_node, thrinfo_t* t )
{
	t->sub_node = sub_node;
}

// other thrinfo_t-related functions

static void* bli_thread_obroadcast( thrinfo_t* t, void* p )
{
	return bli_thrcomm_bcast( t->ocomm, t->ocomm_id, p );
}

static void bli_thread_obarrier( thrinfo_t* t )
{
	bli_thrcomm_barrier( t->ocomm, t->ocomm_id );
}


//
// Prototypes for level-3 thrinfo functions not specific to any operation.
//

thrinfo_t* bli_thrinfo_create
     (
       thrcomm_t* ocomm,
       dim_t      ocomm_id,
       dim_t      n_way,
       dim_t      work_id, 
       bool_t     free_comm,
       thrinfo_t* sub_node
     );

void bli_thrinfo_init
     (
       thrinfo_t* thread,
       thrcomm_t* ocomm,
       dim_t      ocomm_id,
       dim_t      n_way,
       dim_t      work_id, 
       bool_t     free_comm,
       thrinfo_t* sub_node
     );

void bli_thrinfo_init_single
     (
       thrinfo_t* thread
     );

// -----------------------------------------------------------------------------

thrinfo_t* bli_thrinfo_create_for_cntl
     (
       cntx_t*    cntx,
       cntl_t*    cntl_par,
       cntl_t*    cntl_chl,
       thrinfo_t* thread_par
     );

void bli_thrinfo_grow
     (
       cntx_t*    cntx,
       cntl_t*    cntl,
       thrinfo_t* thread
     );

thrinfo_t* bli_thrinfo_rgrow
     (
       cntx_t*    cntx,
       cntl_t*    cntl_par,
       cntl_t*    cntl_cur,
       thrinfo_t* thread_par
     );

#endif
// end bli_thrinfo.h

// Include some operation-specific thrinfo_t prototypes.
// Note that the bli_packm_thrinfo.h must be included before the others!
// begin bli_packm_thrinfo.h


//
// thrinfo_t macros specific to packm.
//

#define packm_thread_my_iter( index, thread ) ( index % thread->n_way == thread->work_id % thread->n_way )

//
// thrinfo_t APIs specific to packm.
//

#if 0
thrinfo_t* bli_packm_thrinfo_create
     (
       thrcomm_t* ocomm,
       dim_t      ocomm_id,
       dim_t      n_way,
       dim_t      work_id,
       thrinfo_t* sub_node
     );
#endif

void bli_packm_thrinfo_init
     (
       thrinfo_t* thread,
       thrcomm_t* ocomm,
       dim_t      ocomm_id,
       dim_t      n_way,
       dim_t      work_id,
       thrinfo_t* sub_node
     );

void bli_packm_thrinfo_init_single
     (
       thrinfo_t* thread
     );

#if 0
void bli_packm_thrinfo_free
     (
       thrinfo_t* thread
     );
#endif

// end bli_packm_thrinfo.h
// begin bli_l3_thrinfo.h


//
// thrinfo_t macros specific to various level-3 operations.
//

// gemm

#define gemm_get_next_a_micropanel( thread, a1, step ) ( a1 + step * thread->n_way )
#define gemm_get_next_b_micropanel( thread, b1, step ) ( b1 + step * thread->n_way )

// herk

#define herk_get_next_a_micropanel( thread, a1, step ) ( a1 + step * thread->n_way )
#define herk_get_next_b_micropanel( thread, b1, step ) ( b1 + step * thread->n_way )

// trmm

#define trmm_r_ir_my_iter( index, thread ) ( index % thread->n_way == thread->work_id % thread->n_way )
#define trmm_r_jr_my_iter( index, thread ) ( index % thread->n_way == thread->work_id % thread->n_way )
#define trmm_l_ir_my_iter( index, thread ) ( index % thread->n_way == thread->work_id % thread->n_way )
#define trmm_l_jr_my_iter( index, thread ) ( index % thread->n_way == thread->work_id % thread->n_way )

// trsm

#define trsm_my_iter( index, thread ) ( index % thread->n_way == thread->work_id % thread->n_way )

//
// thrinfo_t APIs specific to level-3 operations.
//

#if 0
thrinfo_t* bli_l3_thrinfo_create
     (
       thrcomm_t* ocomm,
       dim_t      ocomm_id,
       dim_t      n_way,
       dim_t      work_id,
       thrinfo_t* sub_node
     );
#endif

void bli_l3_thrinfo_init
     (
       thrinfo_t* thread,
       thrcomm_t* ocomm,
       dim_t      ocomm_id,
       dim_t      n_way,
       dim_t      work_id,
       thrinfo_t* sub_node
     );

void bli_l3_thrinfo_init_single
     (
       thrinfo_t* thread
     );

void bli_l3_thrinfo_free
     (
       thrinfo_t* thread
     );

// -----------------------------------------------------------------------------

void bli_l3_thrinfo_create_root
     (
       dim_t       id,
       thrcomm_t*  gl_comm,
       cntx_t*     cntx,
       cntl_t*     cntl,
       thrinfo_t** thread
     );

void bli_l3_thrinfo_print_paths
     (
       thrinfo_t** threads
     );

// -----------------------------------------------------------------------------

#if 0
thrinfo_t** bli_l3_thrinfo_create_roots
     (
       cntx_t* cntx,
       cntl_t* cntl
     );

thrinfo_t** bli_l3_thrinfo_create_full_paths
     (
       cntx_t* cntx
     );
#endif

void bli_l3_thrinfo_free_paths
     (
       thrinfo_t** threads
     );

// end bli_l3_thrinfo.h

// Initialization-related prototypes.
void bli_thread_init( void );
void bli_thread_finalize( void );

// Thread range-related prototypes.
void bli_thread_get_range_sub
     (
       thrinfo_t* thread,
       dim_t      n,
       dim_t      bf,
       bool_t     handle_edge_low,
       dim_t*     start,
       dim_t*     end
     );

#undef  GENPROT
#define GENPROT( opname ) \
\
siz_t PASTEMAC0( opname ) \
     ( \
       dir_t      direct, \
       thrinfo_t* thr, \
       obj_t*     a, \
       obj_t*     b, \
       obj_t*     c, \
       cntl_t*    cntl, \
       cntx_t*    cntx, \
       dim_t*     start, \
       dim_t*     end  \
     );

GENPROT( thread_get_range_mdim )
GENPROT( thread_get_range_ndim )

#undef  GENPROT
#define GENPROT( opname ) \
\
siz_t PASTEMAC0( opname ) \
     ( \
       thrinfo_t* thr, \
       obj_t*     a, \
       blksz_t*   bmult, \
       dim_t*     start, \
       dim_t*     end  \
     );

GENPROT( thread_get_range_l2r )
GENPROT( thread_get_range_r2l )
GENPROT( thread_get_range_t2b )
GENPROT( thread_get_range_b2t )

GENPROT( thread_get_range_weighted_l2r )
GENPROT( thread_get_range_weighted_r2l )
GENPROT( thread_get_range_weighted_t2b )
GENPROT( thread_get_range_weighted_b2t )


dim_t bli_thread_get_range_width_l
     (
       doff_t diagoff_j,
       dim_t  m,
       dim_t  n_j,
       dim_t  j,
       dim_t  n_way,
       dim_t  bf,
       dim_t  bf_left,
       double area_per_thr,
       bool_t handle_edge_low
     );
siz_t bli_find_area_trap_l
     (
       dim_t  m,
       dim_t  n,
       doff_t diagoff
     );
siz_t bli_thread_get_range_weighted_sub
     (
       thrinfo_t* thread,
       doff_t     diagoff,
       uplo_t     uplo,
       dim_t      m,
       dim_t      n,
       dim_t      bf,
       bool_t     handle_edge_low,
       dim_t*     j_start_thr,
       dim_t*     j_end_thr
     );



// Level-3 internal function type
typedef void (*l3int_t)
     (
       obj_t*     alpha,
       obj_t*     a,
       obj_t*     b,
       obj_t*     beta,
       obj_t*     c,
       cntx_t*    cntx,
       cntl_t*    cntl,
       thrinfo_t* thread
     );

// Level-3 thread decorator prototype
void bli_l3_thread_decorator
     (
       l3int_t func,
       opid_t  family,
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );

// -----------------------------------------------------------------------------

// Factorization and partitioning prototypes
typedef struct
{
    dim_t n;
    dim_t sqrt_n;
    dim_t f;
} bli_prime_factors_t;

void bli_prime_factorization(dim_t n, bli_prime_factors_t* factors);

dim_t bli_next_prime_factor(bli_prime_factors_t* factors);

void bli_partition_2x2(dim_t nthread, dim_t work1, dim_t work2, dim_t* nt1, dim_t* nt2);

// -----------------------------------------------------------------------------

dim_t bli_thread_get_env( const char* env, dim_t fallback );

dim_t bli_thread_get_jc_nt( void );
dim_t bli_thread_get_ic_nt( void );
dim_t bli_thread_get_jr_nt( void );
dim_t bli_thread_get_ir_nt( void );
dim_t bli_thread_get_num_threads( void );

void  bli_thread_set_env( const char* env, dim_t value );

void  bli_thread_set_jc_nt( dim_t value );
void  bli_thread_set_ic_nt( dim_t value );
void  bli_thread_set_jr_nt( dim_t value );
void  bli_thread_set_ir_nt( dim_t value );
void  bli_thread_set_num_threads( dim_t value );

// -----------------------------------------------------------------------------

dim_t bli_gcd( dim_t x, dim_t y );
dim_t bli_lcm( dim_t x, dim_t y );
dim_t bli_ipow( dim_t base, dim_t power );

#endif

// end bli_thread.h


// -- Constant definitions --

// begin bli_extern_defs.h


#ifndef BLIS_EXTERN_DEFS_H
#define BLIS_EXTERN_DEFS_H

extern obj_t BLIS_TWO;
extern obj_t BLIS_ONE;
//extern obj_t BLIS_ONE_HALF;
extern obj_t BLIS_ZERO;
//extern obj_t BLIS_MINUS_ONE_HALF;
extern obj_t BLIS_MINUS_ONE;
extern obj_t BLIS_MINUS_TWO;

extern thrcomm_t BLIS_SINGLE_COMM;
extern thrinfo_t BLIS_PACKM_SINGLE_THREADED;
extern thrinfo_t BLIS_GEMM_SINGLE_THREADED;

#endif
// end bli_extern_defs.h


// -- BLIS architecture/kernel definitions --

// begin bli_l1v_ker_prot.h



//
// Define template prototypes for level-1v kernels.
//

#define ADDV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
      ( \
        conj_t           conjx, \
        dim_t            n, \
        ctype*  restrict x, inc_t incx, \
        ctype*  restrict y, inc_t incy, \
        cntx_t* restrict cntx  \
      );


#define AMAXV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t            n, \
       ctype*  restrict x, inc_t incx, \
       dim_t*  restrict index, \
       cntx_t* restrict cntx  \
     ); \


#define AXPBYV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjx, \
       dim_t            n, \
       ctype*  restrict alpha, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict beta, \
       ctype*  restrict y, inc_t incy, \
       cntx_t* restrict cntx  \
     ); \


#define AXPYV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjx, \
       dim_t            n, \
       ctype*  restrict alpha, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       cntx_t* restrict cntx  \
     ); \


#define COPYV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
      ( \
        conj_t           conjx, \
        dim_t            n, \
        ctype*  restrict x, inc_t incx, \
        ctype*  restrict y, inc_t incy, \
        cntx_t* restrict cntx  \
      );


#define DOTV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjx, \
       conj_t           conjy, \
       dim_t            n, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       ctype*  restrict rho, \
       cntx_t* restrict cntx  \
     ); \


#define DOTXV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjx, \
       conj_t           conjy, \
       dim_t            n, \
       ctype*  restrict alpha, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       ctype*  restrict beta, \
       ctype*  restrict rho, \
       cntx_t* restrict cntx  \
     ); \


#define INVERTV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t            n, \
       ctype*  restrict x, inc_t incx, \
       cntx_t* restrict cntx  \
     ); \


#define SCALV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjalpha, \
       dim_t            n, \
       ctype*  restrict alpha, \
       ctype*  restrict x, inc_t incx, \
       cntx_t* restrict cntx  \
     ); \


#define SCAL2V_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjx, \
       dim_t            n, \
       ctype*  restrict alpha, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       cntx_t* restrict cntx  \
     ); \


#define SETV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjalpha, \
       dim_t            n, \
       ctype*  restrict alpha, \
       ctype*  restrict x, inc_t incx, \
       cntx_t* restrict cntx  \
     ); \


#define SUBV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
      ( \
        conj_t           conjx, \
        dim_t            n, \
        ctype*  restrict x, inc_t incx, \
        ctype*  restrict y, inc_t incy, \
        cntx_t* restrict cntx  \
      );


#define SWAPV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t            n, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       cntx_t* restrict cntx  \
     ); \


#define XPBYV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjx, \
       dim_t            n, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict beta, \
       ctype*  restrict y, inc_t incy, \
       cntx_t* restrict cntx  \
     ); \

// end bli_l1v_ker_prot.h
// begin bli_l1f_ker_prot.h



//
// Define template prototypes for level-1f kernels.
//

#define AXPY2V_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjx, \
       conj_t           conjy, \
       dim_t            n, \
       ctype*  restrict alphax, \
       ctype*  restrict alphay, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       ctype*  restrict z, inc_t incz, \
       cntx_t* restrict cntx  \
     );


#define AXPYF_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conja, \
       conj_t           conjx, \
       dim_t            m, \
       dim_t            b_n, \
       ctype*  restrict alpha, \
       ctype*  restrict a, inc_t inca, inc_t lda, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       cntx_t* restrict cntx  \
     );


#define DOTAXPYV_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjxt, \
       conj_t           conjx, \
       conj_t           conjy, \
       dim_t            n, \
       ctype*  restrict alpha, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict y, inc_t incy, \
       ctype*  restrict rho, \
       ctype*  restrict z, inc_t incz, \
       cntx_t* restrict cntx  \
     );


#define DOTXAXPYF_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjat, \
       conj_t           conja, \
       conj_t           conjw, \
       conj_t           conjx, \
       dim_t            m, \
       dim_t            b_n, \
       ctype*  restrict alpha, \
       ctype*  restrict a, inc_t inca, inc_t lda, \
       ctype*  restrict w, inc_t incw, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict beta, \
       ctype*  restrict y, inc_t incy, \
       ctype*  restrict z, inc_t incz, \
       cntx_t* restrict cntx  \
     );


#define DOTXF_KER_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t           conjat, \
       conj_t           conjx, \
       dim_t            m, \
       dim_t            b_n, \
       ctype*  restrict alpha, \
       ctype*  restrict a, inc_t inca, inc_t lda, \
       ctype*  restrict x, inc_t incx, \
       ctype*  restrict beta, \
       ctype*  restrict y, inc_t incy, \
       cntx_t* restrict cntx  \
     );

// end bli_l1f_ker_prot.h
// begin bli_l1m_ker_prot.h



//
// Define template prototypes for level-1m kernels.
//

// native packm kernels

#define PACKM_KER_PROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t           conja, \
       dim_t            n, \
       void*   restrict kappa, \
       void*   restrict a, inc_t inca, inc_t lda, \
       void*   restrict p,             inc_t ldp, \
       cntx_t* restrict cntx  \
     );


// native unpackm kernels

#define UNPACKM_KER_PROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t           conja, \
       dim_t            n, \
       void*   restrict kappa, \
       void*   restrict p,             inc_t ldp, \
       void*   restrict a, inc_t inca, inc_t lda, \
       cntx_t* restrict cntx  \
     );


// 3mis packm kernels

#define PACKM_3MIS_KER_PROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t           conja, \
       dim_t            n, \
       void*   restrict kappa, \
       void*   restrict a, inc_t inca, inc_t lda, \
       void*   restrict p, inc_t is_p, inc_t ldp, \
       cntx_t* restrict cntx  \
     );


// 4mi packm kernels

#define PACKM_4MI_KER_PROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t           conja, \
       dim_t            n, \
       void*   restrict kappa, \
       void*   restrict a, inc_t inca, inc_t lda, \
       void*   restrict p, inc_t is_p, inc_t ldp, \
       cntx_t* restrict cntx  \
     );


// rih packm kernels

#define PACKM_RIH_KER_PROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t           conja, \
       pack_t           schema, \
       dim_t            n, \
       void*   restrict kappa, \
       void*   restrict a, inc_t inca, inc_t lda, \
       void*   restrict p,             inc_t ldp, \
       cntx_t* restrict cntx  \
     );


// 1e/1r packm kernels

#define PACKM_1ER_KER_PROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t           conja, \
       pack_t           schema, \
       dim_t            n, \
       void*   restrict kappa, \
       void*   restrict a, inc_t inca, inc_t lda, \
       void*   restrict p,             inc_t ldp, \
       cntx_t* restrict cntx  \
     );

// end bli_l1m_ker_prot.h
// begin bli_l3_ukr_prot.h


//
// Define template prototypes for level-3 micro-kernels.
//

#define GEMM_UKR_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t               k, \
       ctype*     restrict alpha, \
       ctype*     restrict a, \
       ctype*     restrict b, \
       ctype*     restrict beta, \
       ctype*     restrict c, inc_t rs_c, inc_t cs_c, \
       auxinfo_t* restrict data, \
       cntx_t*    restrict cntx  \
     );


#define GEMMTRSM_UKR_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t               k, \
       ctype*     restrict alpha, \
       ctype*     restrict a1x, \
       ctype*     restrict a11, \
       ctype*     restrict bx1, \
       ctype*     restrict b11, \
       ctype*     restrict c11, inc_t rs_c, inc_t cs_c, \
       auxinfo_t* restrict data, \
       cntx_t*    restrict cntx  \
     );


#define TRSM_UKR_PROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       ctype*     restrict a, \
       ctype*     restrict b, \
       ctype*     restrict c, inc_t rs_c, inc_t cs_c, \
       auxinfo_t* restrict data, \
       cntx_t*    restrict cntx  \
     );

// end bli_l3_ukr_prot.h

// begin bli_arch_config_pre.h


#ifndef BLIS_ARCH_CONFIG_PRE_H
#define BLIS_ARCH_CONFIG_PRE_H


// -- Naming-related kernel definitions ----------------------------------------

// The default suffix appended to reference kernels.
#define BLIS_REF_SUFFIX  _ref

// A suffix used for labeling certain induced method aware functions.
#define BLIS_IND_SUFFIX  _ind

// Add an underscore to the BLIS kernel set string, if it was defined.
#ifdef  BLIS_CNAME
#define BLIS_CNAME_INFIX  PASTECH(_,BLIS_CNAME)
#endif

// -- Prototype-generating macro definitions -----------------------------------

// Prototype-generating macro for bli_cntx_init_<arch>*() functions.
#define CNTX_INIT_PROTS( archname ) \
\
void PASTEMAC(cntx_init_,archname) \
     ( \
       cntx_t* cntx \
     ); \
void PASTEMAC2(cntx_init_,archname,BLIS_REF_SUFFIX) \
     ( \
       cntx_t* cntx \
     ); \
void PASTEMAC2(cntx_init_,archname,BLIS_IND_SUFFIX) \
     ( \
       ind_t   method, \
       num_t   dt, \
       cntx_t* cntx \
     );


#endif

// end bli_arch_config_pre.h
// begin bli_arch_config.h


#ifndef BLIS_ARCH_CONFIG_H
#define BLIS_ARCH_CONFIG_H

//
// -- Context initialization prototypes ----------------------------------------
//

// -- Intel64 architectures --
#ifdef BLIS_CONFIG_SKX
CNTX_INIT_PROTS( skx )
#endif
#ifdef BLIS_CONFIG_KNL
CNTX_INIT_PROTS( knl )
#endif
#ifdef BLIS_CONFIG_KNC
CNTX_INIT_PROTS( knc )
#endif
#ifdef BLIS_CONFIG_HASWELL
CNTX_INIT_PROTS( haswell )
#endif
#ifdef BLIS_CONFIG_SANDYBRIDGE
CNTX_INIT_PROTS( sandybridge )
#endif
#ifdef BLIS_CONFIG_PENRYN
CNTX_INIT_PROTS( penryn )
#endif

// -- AMD64 architectures --

#ifdef BLIS_CONFIG_ZEN
CNTX_INIT_PROTS( zen )
#endif
#ifdef BLIS_CONFIG_EXCAVATOR
CNTX_INIT_PROTS( excavator )
#endif
#ifdef BLIS_CONFIG_STEAMROLLER
CNTX_INIT_PROTS( steamroller )
#endif
#ifdef BLIS_CONFIG_PILEDRIVER
CNTX_INIT_PROTS( piledriver )
#endif
#ifdef BLIS_CONFIG_BULLDOZER
CNTX_INIT_PROTS( bulldozer )
#endif

// -- ARM architectures --

#ifdef BLIS_CONFIG_CORTEXA57
CNTX_INIT_PROTS( cortexa57 )
#endif
#ifdef BLIS_CONFIG_CORTEXA15
CNTX_INIT_PROTS( cortexa15 )
#endif
#ifdef BLIS_CONFIG_CORTEXA9
CNTX_INIT_PROTS( cortexa9 )
#endif

// -- IBM BG/Q --

#ifdef BLIS_CONFIG_POWER7
CNTX_INIT_PROTS( power7 )
#endif
#ifdef BLIS_CONFIG_BGQ
CNTX_INIT_PROTS( bgq )
#endif

// -- Generic --

#ifdef BLIS_CONFIG_GENERIC
CNTX_INIT_PROTS( generic )
#endif


//
// -- Architecture family-specific headers -------------------------------------
//

// -- x86_64 families --

#ifdef BLIS_FAMILY_INTEL64
#include "bli_family_intel64.h" // skipped
#endif
#ifdef BLIS_FAMILY_AMD64
#include "bli_family_amd64.h" // skipped
#endif
#ifdef BLIS_FAMILY_X86_64
#include "bli_family_x86_64.h" // skipped
#endif

// -- Intel64 architectures --
#ifdef BLIS_FAMILY_SKX
#include "bli_family_skx.h" // skipped
#endif
#ifdef BLIS_FAMILY_KNL
// begin bli_family_knl.h


//#ifndef BLIS_FAMILY_H
//#define BLIS_FAMILY_H


// -- THREADING PARAMETERS -----------------------------------------------------

#define BLIS_DEFAULT_M_THREAD_RATIO     4
#define BLIS_DEFAULT_N_THREAD_RATIO     1

#define BLIS_DEFAULT_MR_THREAD_MAX      1
#define BLIS_DEFAULT_NR_THREAD_MAX      1


// -- MEMORY ALLOCATION --------------------------------------------------------

//#define BLIS_TREE_BARRIER
//#define BLIS_TREE_BARRIER_ARITY 4

#define BLIS_SIMD_ALIGN_SIZE             64

#define BLIS_SIMD_SIZE                   64
#define BLIS_SIMD_NUM_REGISTERS          32



//#define BLIS_MALLOC_INTL hbw_malloc
//#define BLIS_FREE_INTL hbw_free


#if 0
// -- LEVEL-3 MICRO-KERNEL CONSTANTS -------------------------------------------

#define BLIS_SGEMM_UKERNEL_PREFERS_CONTIG_ROWS
#define BLIS_SGEMM_UKERNEL             bli_sgemm_opt_30x16_knc
#define BLIS_DEFAULT_MC_S              240
#define BLIS_DEFAULT_KC_S              240
#define BLIS_DEFAULT_NC_S              14400
#define BLIS_DEFAULT_MR_S              30
#define BLIS_DEFAULT_NR_S              16
#define BLIS_PACKDIM_MR_S              32
#define BLIS_PACKDIM_NR_S              16

#if 0

#define BLIS_DGEMM_UKERNEL_PREFERS_CONTIG_ROWS
#define BLIS_DGEMM_UKERNEL             bli_dgemm_opt_30x8_knc
#define BLIS_DEFAULT_MC_D              120
#define BLIS_DEFAULT_KC_D              240
#define BLIS_DEFAULT_NC_D              14400
#define BLIS_DEFAULT_MR_D              30
#define BLIS_DEFAULT_NR_D              8
#define BLIS_PACKDIM_MR_D              32
#define BLIS_PACKDIM_NR_D              8

#elif 0

#define BLIS_DGEMM_UKERNEL_PREFERS_CONTIG_ROWS
#define BLIS_DGEMM_UKERNEL             bli_dgemm_opt_30x8
#define BLIS_DEFAULT_MC_D              120
#define BLIS_DEFAULT_KC_D              240
#define BLIS_DEFAULT_NC_D              14400
#define BLIS_DEFAULT_MR_D              30
#define BLIS_DEFAULT_NR_D              8
#define BLIS_PACKDIM_MR_D              32
#define BLIS_PACKDIM_NR_D              8

#define BLIS_DPACKM_8XK_KERNEL         bli_dpackm_8xk_opt
#define BLIS_DPACKM_30XK_KERNEL        bli_dpackm_30xk_opt

#else

#define BLIS_DGEMM_UKERNEL_PREFERS_CONTIG_ROWS
#define BLIS_DGEMM_UKERNEL             bli_dgemm_opt_24x8
#define BLIS_DEFAULT_MR_D              24
#define BLIS_DEFAULT_NR_D              8
#define BLIS_PACKDIM_MR_D              24
#define BLIS_PACKDIM_NR_D              8
#define BLIS_DEFAULT_MC_D              120
#define BLIS_DEFAULT_KC_D              336
#define BLIS_DEFAULT_NC_D              14400

#define BLIS_DPACKM_8XK_KERNEL         bli_dpackm_8xk_opt
#define BLIS_DPACKM_24XK_KERNEL        bli_dpackm_24xk_opt

#endif

#define BLIS_MAXIMUM_MC_S              (BLIS_DEFAULT_MC_S + BLIS_DEFAULT_MC_S/4)
#define BLIS_MAXIMUM_KC_S              (BLIS_DEFAULT_KC_S + BLIS_DEFAULT_KC_S/4)
#define BLIS_MAXIMUM_NC_S              (BLIS_DEFAULT_NC_S +                   0) 

#define BLIS_MAXIMUM_MC_D              (BLIS_DEFAULT_MC_D + BLIS_DEFAULT_MC_D/4)
#define BLIS_MAXIMUM_KC_D              (BLIS_DEFAULT_KC_D + BLIS_DEFAULT_KC_D/4)
#define BLIS_MAXIMUM_NC_D              (BLIS_DEFAULT_NC_D +                   0)

#endif


//#endif

// end bli_family_knl.h
#endif
#ifdef BLIS_FAMILY_KNC
#include "bli_family_knc.h" // skipped
#endif
#ifdef BLIS_FAMILY_HASWELL
#include "bli_family_haswell.h" // skipped
#endif
#ifdef BLIS_FAMILY_SANDYBRIDGE
#include "bli_family_sandybridge.h" // skipped
#endif
#ifdef BLIS_FAMILY_PENRYN
#include "bli_family_penryn.h" // skipped
#endif

// -- AMD64 architectures --

#ifdef BLIS_FAMILY_ZEN
#include "bli_family_zen.h" // skipped
#endif
#ifdef BLIS_FAMILY_EXCAVATOR
#include "bli_family_excavator.h" // skipped
#endif
#ifdef BLIS_FAMILY_STEAMROLLER
#include "bli_family_steamroller.h" // skipped
#endif
#ifdef BLIS_FAMILY_PILEDRIVER
#include "bli_family_piledriver.h" // skipped
#endif
#ifdef BLIS_FAMILY_BULLDOZER
#include "bli_family_bulldozer.h" // skipped
#endif

// -- ARM architectures --

#ifdef BLIS_FAMILY_CORTEXA57
#include "bli_family_cortexa57.h" // skipped
#endif
#ifdef BLIS_FAMILY_CORTEXA15
#include "bli_family_cortexa15.h" // skipped
#endif
#ifdef BLIS_FAMILY_CORTEXA9
#include "bli_family_cortexa9.h" // skipped
#endif

// -- IBM BG/Q --

#ifdef BLIS_FAMILY_POWER7
#include "bli_family_power7.h" // skipped
#endif
#ifdef BLIS_FAMILY_BGQ
#include "bli_family_bgq.h" // skipped
#endif

// -- Generic --

#ifdef BLIS_FAMILY_GENERIC
#include "bli_family_generic.h" // skipped
#endif


//
// -- kernel set prototypes ----------------------------------------------------
//

// -- Intel64 architectures --
#ifdef BLIS_KERNELS_SKX
#include "bli_kernels_skx.h" // skipped
#endif
#ifdef BLIS_KERNELS_KNL
// begin bli_kernels_knl.h


GEMM_UKR_PROT( double,   s, gemm_knl_asm_24x16 )
GEMM_UKR_PROT( double,   d, gemm_knl_asm_24x8 )

PACKM_KER_PROT( double,   s, packm_knl_asm_24xk )
PACKM_KER_PROT( double,   s, packm_knl_asm_16xk )

PACKM_KER_PROT( double,   d, packm_knl_asm_24xk )
PACKM_KER_PROT( double,   d, packm_knl_asm_8xk )

// unused:
GEMM_UKR_PROT( double,   d, gemm_knl_asm_12x16 )
GEMM_UKR_PROT( double,   d, gemm_knl_asm_30x8 )
GEMM_UKR_PROT( double,   d, gemm_knl_asm_8x24 )

PACKM_KER_PROT( double,   d, packm_knl_asm_30xk )

// end bli_kernels_knl.h
#endif
#ifdef BLIS_KERNELS_KNC
#include "bli_kernels_knc.h" // skipped
#endif
#ifdef BLIS_KERNELS_HASWELL
#include "bli_kernels_haswell.h" // skipped
#endif
#ifdef BLIS_KERNELS_SANDYBRIDGE
#include "bli_kernels_sandybridge.h" // skipped
#endif
#ifdef BLIS_KERNELS_PENRYN
#include "bli_kernels_penryn.h" // skipped
#endif

// -- AMD64 architectures --

#ifdef BLIS_KERNELS_ZEN
// begin bli_kernels_zen.h


// -- level-1v --

// amaxv (intrinsics)
AMAXV_KER_PROT( float,    s, amaxv_zen_int )
AMAXV_KER_PROT( double,   d, amaxv_zen_int )

// axpyv (intrinsics)
AXPYV_KER_PROT( float,    s, axpyv_zen_int )
AXPYV_KER_PROT( double,   d, axpyv_zen_int )

	// axpyv (intrinsics unrolled x10)
	AXPYV_KER_PROT( float,    s, axpyv_zen_int10 )
	AXPYV_KER_PROT( double,   d, axpyv_zen_int10 )

// dotv (intrinsics)
DOTV_KER_PROT( float,    s, dotv_zen_int )
DOTV_KER_PROT( double,   d, dotv_zen_int )

	// dotv (intrinsics, unrolled x10)
	DOTV_KER_PROT( float,    s, dotv_zen_int10 )
	DOTV_KER_PROT( double,   d, dotv_zen_int10 )

// dotxv (intrinsics)
DOTXV_KER_PROT( float,    s, dotxv_zen_int )
DOTXV_KER_PROT( double,   d, dotxv_zen_int )

// scalv (intrinsics)
SCALV_KER_PROT( float,    s, scalv_zen_int )
SCALV_KER_PROT( double,   d, scalv_zen_int )

	// scalv (intrinsics unrolled x10)
	SCALV_KER_PROT( float,    s, scalv_zen_int10 )
	SCALV_KER_PROT( double,   d, scalv_zen_int10 )

// -- level-1f --

// axpyf (intrinsics)
AXPYF_KER_PROT( float,    s, axpyf_zen_int_8 )
AXPYF_KER_PROT( double,   d, axpyf_zen_int_8 )

// dotxf (intrinsics)
DOTXF_KER_PROT( float,    s, dotxf_zen_int_8 )
DOTXF_KER_PROT( double,   d, dotxf_zen_int_8 )

// -- level-3 --

// gemm (asm d6x8)
GEMM_UKR_PROT( float,    s, gemm_zen_asm_6x16 )
GEMM_UKR_PROT( double,   d, gemm_zen_asm_6x8 )
GEMM_UKR_PROT( scomplex, c, gemm_zen_asm_3x8 )
GEMM_UKR_PROT( dcomplex, z, gemm_zen_asm_3x4 )

// gemmtrsm_l (asm d6x8)
GEMMTRSM_UKR_PROT( float,    s, gemmtrsm_l_zen_asm_6x16 )
GEMMTRSM_UKR_PROT( double,   d, gemmtrsm_l_zen_asm_6x8 )

// gemmtrsm_u (asm d6x8)
GEMMTRSM_UKR_PROT( float,    s, gemmtrsm_u_zen_asm_6x16 )
GEMMTRSM_UKR_PROT( double,   d, gemmtrsm_u_zen_asm_6x8 )


// gemm (asm d8x6)
//GEMM_UKR_PROT( float,    s, gemm_zen_asm_16x6 )
//GEMM_UKR_PROT( double,   d, gemm_zen_asm_8x6 )
//GEMM_UKR_PROT( scomplex, c, gemm_zen_asm_8x3 )
//GEMM_UKR_PROT( dcomplex, z, gemm_zen_asm_4x3 )

// end bli_kernels_zen.h
#endif
//#ifdef BLIS_KERNELS_EXCAVATOR
//#include "bli_kernels_excavator.h"
//#endif
//#ifdef BLIS_KERNELS_STEAMROLLER
//#include "bli_kernels_steamroller.h"
//#endif
#ifdef BLIS_KERNELS_PILEDRIVER
#include "bli_kernels_piledriver.h" // skipped
#endif
#ifdef BLIS_KERNELS_BULLDOZER
#include "bli_kernels_bulldozer.h" // skipped
#endif

// -- ARM architectures --

#ifdef BLIS_KERNELS_ARMV8A
#include "bli_kernels_armv8a.h" // skipped
#endif
#ifdef BLIS_KERNELS_ARMV7A
#include "bli_kernels_armv7a.h" // skipped
#endif

// -- IBM BG/Q --

#ifdef BLIS_KERNELS_POWER7
#include "bli_kernels_power7.h" // skipped
#endif
#ifdef BLIS_KERNELS_BGQ
#include "bli_kernels_bgq.h" // skipped
#endif



#endif

// end bli_arch_config.h

// begin bli_kernel_macro_defs.h


#ifndef BLIS_KERNEL_MACRO_DEFS_H
#define BLIS_KERNEL_MACRO_DEFS_H


// -- Define default threading parameters --------------------------------------

#ifndef BLIS_DEFAULT_M_THREAD_RATIO
#define BLIS_DEFAULT_M_THREAD_RATIO 2
#endif

#ifndef BLIS_DEFAULT_N_THREAD_RATIO
#define BLIS_DEFAULT_N_THREAD_RATIO 1
#endif

#ifndef BLIS_DEFAULT_MR_THREAD_MAX
#define BLIS_DEFAULT_MR_THREAD_MAX 1
#endif

#ifndef BLIS_DEFAULT_NR_THREAD_MAX
#define BLIS_DEFAULT_NR_THREAD_MAX 4
#endif


// -- Memory allocation --------------------------------------------------------

// Memory allocation functions. These macros define the three types of
// malloc()-style functions, and their free() counterparts: one for each
// type of memory to be allocated.
// NOTE: ANY ALTERNATIVE TO malloc()/free() USED FOR ANY OF THE FOLLOWING
// THREE PAIRS OF MACROS MUST USE THE SAME FUNCTION PROTOTYPE AS malloc()
// and free():
//
//   void* malloc( size_t size );
//   void  free( void* p );
//

// This allocation function is called to allocate memory for blocks within
// BLIS's internal memory pools.
#ifndef BLIS_MALLOC_POOL
  // If use of libmemkind was enabled at configure-time, the default
  // memory allocation function for memory pools should be hbw_malloc()
  // instead of malloc().
  #ifdef  BLIS_ENABLE_MEMKIND
  #define BLIS_MALLOC_POOL               hbw_malloc
  #else
  #define BLIS_MALLOC_POOL               malloc
  #endif
#endif

#ifndef BLIS_FREE_POOL
  // If use of libmemkind was enabled at configure-time, the default
  // memory deallocation function for memory pools should be hbw_free()
  // instead of free().
  #ifdef  BLIS_ENABLE_MEMKIND
  #define BLIS_FREE_POOL                 hbw_free
  #else
  #define BLIS_FREE_POOL                 free
  #endif
#endif

// This allocation function is called to allocate memory for internally-
// used objects and structures, such as control tree nodes.
#ifndef BLIS_MALLOC_INTL
#define BLIS_MALLOC_INTL                 malloc
#endif

#ifndef BLIS_FREE_INTL
#define BLIS_FREE_INTL                   free
#endif

// This allocation function is called to allocate memory for objects
// created by user-level API functions, such as bli_obj_create().
#ifndef BLIS_MALLOC_USER
#define BLIS_MALLOC_USER                 malloc
#endif

#ifndef BLIS_FREE_USER
#define BLIS_FREE_USER                   free
#endif

// -- Other system-related definitions -----------------------------------------

// Size of a virtual memory page. This is used to align blocks within the
// memory pools.
#ifndef BLIS_PAGE_SIZE
#define BLIS_PAGE_SIZE                   4096
#endif

// The maximum number of named SIMD vector registers available for use.
// When configuring with umbrella configuration families, this should be
// set to the maximum number of registers across all sub-configurations in
// the family.
#ifndef BLIS_SIMD_NUM_REGISTERS
#define BLIS_SIMD_NUM_REGISTERS          32
#endif

// The maximum size (in bytes) of each SIMD vector.
// When configuring with umbrella configuration families, this should be
// set to the maximum SIMD size across all sub-configurations in the family.
#ifndef BLIS_SIMD_SIZE
#define BLIS_SIMD_SIZE                   64
#endif

// Alignment size (in bytes) needed by the instruction set for aligned
// SIMD/vector instructions.
#ifndef BLIS_SIMD_ALIGN_SIZE
#define BLIS_SIMD_ALIGN_SIZE             BLIS_SIMD_SIZE
#endif

// The maximum size in bytes of local stack buffers within macro-kernel
// functions. These buffers are usually used to store a temporary copy
// of a single microtile. The reason we multiply by 2 is to handle induced
// methods, where we use real domain register blocksizes in units of
// complex elements. Specifically, the macro-kernels will need this larger
// micro-tile footprint, even though the virtual micro-kernels will only
// ever be writing to half (real or imaginary part) at a time.
#ifndef BLIS_STACK_BUF_MAX_SIZE
#define BLIS_STACK_BUF_MAX_SIZE          ( BLIS_SIMD_NUM_REGISTERS * \
                                           BLIS_SIMD_SIZE * 2 )
#endif

// Alignment size used to align local stack buffers within macro-kernel
// functions.
#define BLIS_STACK_BUF_ALIGN_SIZE        BLIS_SIMD_ALIGN_SIZE

// Alignment size used when allocating memory via BLIS_MALLOC_USER.
// To disable heap alignment, set this to 1.
#define BLIS_HEAP_ADDR_ALIGN_SIZE        BLIS_SIMD_ALIGN_SIZE

// Alignment size used when sizing leading dimensions of memory allocated
// via BLIS_MALLOC_USER.
#define BLIS_HEAP_STRIDE_ALIGN_SIZE      BLIS_SIMD_ALIGN_SIZE

// Alignment size used when allocating blocks to the internal memory
// pool, via BLIS_MALLOC_POOL.
#define BLIS_POOL_ADDR_ALIGN_SIZE        BLIS_PAGE_SIZE



#endif

// end bli_kernel_macro_defs.h


// -- Base operation prototypes --

// begin bli_init.h


void   bli_init( void );
void   bli_finalize( void );

void   bli_init_auto( void );
void   bli_finalize_auto( void );

void   bli_init_apis( void );
void   bli_finalize_apis( void );

void   bli_init_once( void );
void   bli_finalize_once( void );

// end bli_init.h
// begin bli_const.h


void bli_const_init( void );
void bli_const_finalize( void );

// end bli_const.h
// begin bli_obj.h


// begin bli_obj_check.h


void bli_obj_create_check( num_t  dt,
                           dim_t  m,
                           dim_t  n,
                           inc_t  rs,
                           inc_t  cs,
                           obj_t* obj );

void bli_obj_create_without_buffer_check( num_t  dt,
                                          dim_t  m,
                                          dim_t  n,
                                          obj_t* obj );

void bli_obj_alloc_buffer_check( inc_t  rs,
                                 inc_t  cs,
                                 inc_t  is,
                                 obj_t* obj );

void bli_obj_attach_buffer_check( void*  p,
                                  inc_t  rs,
                                  inc_t  cs,
                                  inc_t  is,
                                  obj_t* obj );

void bli_obj_create_scalar_check( num_t  dt,
                                  obj_t* obj );

void bli_obj_free_check( obj_t* obj );

void bli_obj_create_const_check( double value, obj_t* obj );

void bli_obj_create_const_copy_of_check( obj_t* a, obj_t* b );

void bli_datatype_size_check( num_t dt );

void bli_datatype_string_check( num_t dt );

void bli_datatype_union_check( num_t dt1, num_t dt2 );

void bli_obj_print_check( char* label, obj_t* obj );

// end bli_obj_check.h

void bli_obj_create( num_t  dt,
                     dim_t  m,
                     dim_t  n,
                     inc_t  rs,
                     inc_t  cs,
                     obj_t* obj );

void bli_obj_create_with_attached_buffer( num_t  dt,
                                          dim_t  m,
                                          dim_t  n,
                                          void*  p,
                                          inc_t  rs,
                                          inc_t  cs,
                                          obj_t* obj );

void bli_obj_create_without_buffer( num_t  dt,
                                    dim_t  m,
                                    dim_t  n,
                                    obj_t* obj );

void bli_obj_alloc_buffer( inc_t  rs,
                           inc_t  cs,
                           inc_t  is,
                           obj_t* obj );

void bli_obj_attach_buffer( void*  p,
                            inc_t  rs,
                            inc_t  cs,
                            inc_t  is,
                            obj_t* obj );

void bli_obj_create_1x1( num_t  dt,
                         obj_t* obj );

void bli_obj_create_1x1_with_attached_buffer( num_t  dt,
                                              void*  p,
                                              obj_t* obj );

void bli_obj_create_conf_to( obj_t* s, obj_t* d );

void bli_obj_free( obj_t* obj );

//void bli_obj_create_const( double value, obj_t* obj );

//void bli_obj_create_const_copy_of( obj_t* a, obj_t* b );

void bli_adjust_strides( dim_t  m,
                         dim_t  n,
                         siz_t  elem_size,
                         inc_t* rs,
                         inc_t* cs,
                         inc_t* is );

siz_t bli_datatype_size( num_t dt );
char* bli_datatype_string( num_t dt );

dim_t bli_align_dim_to_mult( dim_t dim, dim_t dim_mult );
dim_t bli_align_dim_to_size( dim_t dim, siz_t elem_size, siz_t align_size );
dim_t bli_align_ptr_to_size( void* p, size_t align_size );

num_t bli_datatype_union( num_t dt1, num_t dt2 );

void bli_obj_print( char* label, obj_t* obj );

// end bli_obj.h
// begin bli_obj_scalar.h


void bli_obj_scalar_init_detached( num_t  dt,
                                   obj_t* beta );

void bli_obj_scalar_init_detached_copy_of( num_t  dt,
                                           conj_t conj,
                                           obj_t* alpha,
                                           obj_t* beta );

void bli_obj_scalar_detach( obj_t* a,
                            obj_t* alpha );

void bli_obj_scalar_attach( conj_t conj,
                            obj_t* alpha,
                            obj_t* a );

void bli_obj_scalar_apply_scalar( obj_t* alpha,
                                  obj_t* a );

void bli_obj_scalar_reset( obj_t* a );

bool_t bli_obj_scalar_has_nonzero_imag( obj_t* a );

bool_t bli_obj_scalar_equals( obj_t* a,
                              obj_t* beta );

// end bli_obj_scalar.h
// begin bli_blksz.h


// blksz_t query

static dim_t bli_blksz_get_def
     (
       num_t    dt,
       blksz_t* b
     )
{
	return b->v[ dt ];
}

static dim_t bli_blksz_get_max
     (
       num_t    dt,
       blksz_t* b
     )
{
	return b->e[ dt ];
}


// blksz_t modification

static void bli_blksz_set_def
     (
       dim_t    val,
       num_t    dt,
       blksz_t* b
     )
{
	b->v[ dt ] = val;
}

static void bli_blksz_set_max
     (
       dim_t    val,
       num_t    dt,
       blksz_t* b
     )
{
	b->e[ dt ] = val;
}

static void bli_blksz_copy
     (
       blksz_t* b_src,
       blksz_t* b_dst
     )
{
	*b_dst = *b_src;
}

static void bli_blksz_copy_if_pos
     (
       blksz_t* b_src,
       blksz_t* b_dst
     )
{
	// Copy the blocksize values over to b_dst one-by-one so that
	// we can skip the ones that are non-positive.

	const dim_t v_s = bli_blksz_get_def( BLIS_FLOAT,    b_src );
	const dim_t v_d = bli_blksz_get_def( BLIS_DOUBLE,   b_src );
	const dim_t v_c = bli_blksz_get_def( BLIS_SCOMPLEX, b_src );
	const dim_t v_z = bli_blksz_get_def( BLIS_DCOMPLEX, b_src );

	const dim_t e_s = bli_blksz_get_max( BLIS_FLOAT,    b_src );
	const dim_t e_d = bli_blksz_get_max( BLIS_DOUBLE,   b_src );
	const dim_t e_c = bli_blksz_get_max( BLIS_SCOMPLEX, b_src );
	const dim_t e_z = bli_blksz_get_max( BLIS_DCOMPLEX, b_src );

	if ( v_s > 0 ) bli_blksz_set_def( v_s, BLIS_FLOAT,    b_dst );
	if ( v_d > 0 ) bli_blksz_set_def( v_d, BLIS_DOUBLE,   b_dst );
	if ( v_c > 0 ) bli_blksz_set_def( v_c, BLIS_SCOMPLEX, b_dst );
	if ( v_z > 0 ) bli_blksz_set_def( v_z, BLIS_DCOMPLEX, b_dst );

	if ( e_s > 0 ) bli_blksz_set_max( e_s, BLIS_FLOAT,    b_dst );
	if ( e_d > 0 ) bli_blksz_set_max( e_d, BLIS_DOUBLE,   b_dst );
	if ( e_c > 0 ) bli_blksz_set_max( e_c, BLIS_SCOMPLEX, b_dst );
	if ( e_z > 0 ) bli_blksz_set_max( e_z, BLIS_DCOMPLEX, b_dst );
}

static void bli_blksz_copy_def_dt
     (
       num_t dt_src, blksz_t* b_src,
       num_t dt_dst, blksz_t* b_dst
     )
{
	const dim_t val = bli_blksz_get_def( dt_src, b_src );

	bli_blksz_set_def( val, dt_dst, b_dst );
}

static void bli_blksz_copy_max_dt
     (
       num_t dt_src, blksz_t* b_src,
       num_t dt_dst, blksz_t* b_dst
     )
{
	const dim_t val = bli_blksz_get_max( dt_src, b_src );

	bli_blksz_set_max( val, dt_dst, b_dst );
}

static void bli_blksz_copy_dt
     (
       num_t dt_src, blksz_t* b_src,
       num_t dt_dst, blksz_t* b_dst
     )
{
	bli_blksz_copy_def_dt( dt_src, b_src, dt_dst, b_dst );
	bli_blksz_copy_max_dt( dt_src, b_src, dt_dst, b_dst );
}

static void bli_blksz_scale_def
     (
       dim_t    num,
       dim_t    den,
       num_t    dt,
       blksz_t* b
     )
{
	const dim_t val = bli_blksz_get_def( dt, b );

	bli_blksz_set_def( ( val * num ) / den, dt, b );
}

static void bli_blksz_scale_max
     (
       dim_t    num,
       dim_t    den,
       num_t    dt,
       blksz_t* b
     )
{
	const dim_t val = bli_blksz_get_max( dt, b );

	bli_blksz_set_max( ( val * num ) / den, dt, b );
}

// -----------------------------------------------------------------------------

blksz_t* bli_blksz_create_ed
     (
       dim_t b_s, dim_t be_s,
       dim_t b_d, dim_t be_d,
       dim_t b_c, dim_t be_c,
       dim_t b_z, dim_t be_z
     );

blksz_t* bli_blksz_create
     (
       dim_t b_s,  dim_t b_d,  dim_t b_c,  dim_t b_z,
       dim_t be_s, dim_t be_d, dim_t be_c, dim_t be_z
     );

void bli_blksz_init_ed
     (
       blksz_t* b,
       dim_t    b_s, dim_t be_s,
       dim_t    b_d, dim_t be_d,
       dim_t    b_c, dim_t be_c,
       dim_t    b_z, dim_t be_z
     );

void bli_blksz_init
     (
       blksz_t* b,
       dim_t b_s,  dim_t b_d,  dim_t b_c,  dim_t b_z,
       dim_t be_s, dim_t be_d, dim_t be_c, dim_t be_z
     );

void bli_blksz_init_easy
     (
       blksz_t* b,
       dim_t b_s,  dim_t b_d,  dim_t b_c,  dim_t b_z
     );

void bli_blksz_free
     (
       blksz_t* b
     );

// -----------------------------------------------------------------------------

#if 0
void bli_blksz_reduce_dt_to
     (
       num_t dt_bm, blksz_t* bmult,
       num_t dt_bs, blksz_t* blksz
     );
#endif

void bli_blksz_reduce_def_to
     (
       num_t dt_bm, blksz_t* bmult,
       num_t dt_bs, blksz_t* blksz
     );

void bli_blksz_reduce_max_to
     (
       num_t dt_bm, blksz_t* bmult,
       num_t dt_bs, blksz_t* blksz
     );
// -----------------------------------------------------------------------------

dim_t bli_determine_blocksize
     (
       dir_t   direct,
       dim_t   i,
       dim_t   dim,
       obj_t*  obj,
       bszid_t bszid,
       cntx_t* cntx
     );

dim_t bli_determine_blocksize_f
     (
       dim_t   i,
       dim_t   dim,
       obj_t*  obj,
       bszid_t bszid,
       cntx_t* cntx
     );

dim_t bli_determine_blocksize_b
     (
       dim_t   i,
       dim_t   dim,
       obj_t*  obj,
       bszid_t bszid,
       cntx_t* cntx
     );

dim_t bli_determine_blocksize_f_sub
     (
       dim_t  i,
       dim_t  dim,
       dim_t  b_alg,
       dim_t  b_max
     );

dim_t bli_determine_blocksize_b_sub
     (
       dim_t  i,
       dim_t  dim,
       dim_t  b_alg,
       dim_t  b_max
     );

// end bli_blksz.h
// begin bli_func.h


// -----------------------------------------------------------------------------

// func_t query

static void* bli_func_get_dt
     (
       num_t   dt,
       func_t* func
     )
{
    return func->ptr[ dt ];
}

// func_t modification

static void bli_func_set_dt
     (
       void*   fp,
       num_t   dt,
       func_t* func
     )
{
    func->ptr[ dt ] = fp;
}

static void bli_func_copy_dt
     (
       num_t dt_src, func_t* func_src,
       num_t dt_dst, func_t* func_dst
     )
{
	void* fp = bli_func_get_dt( dt_src, func_src );

	bli_func_set_dt( fp, dt_dst, func_dst );
}

// -----------------------------------------------------------------------------

func_t* bli_func_create
     (
       void* ptr_s,
       void* ptr_d,
       void* ptr_c,
       void* ptr_z
     );

void bli_func_init
     (
       func_t* f,
       void*   ptr_s,
       void*   ptr_d,
       void*   ptr_c,
       void*   ptr_z
     );

void bli_func_init_null
     (
       func_t* f
     );

void bli_func_free( func_t* f );

// -----------------------------------------------------------------------------

bool_t bli_func_is_null_dt( num_t   dt,
                            func_t* f );
bool_t bli_func_is_null( func_t* f );

// end bli_func.h
// begin bli_mbool.h


// -----------------------------------------------------------------------------

// mbool_t query

static bool_t bli_mbool_get_dt( num_t dt, mbool_t* mb )
{
	return mb->v[ dt ];
}

// mbool_t modification

static void bli_mbool_set_dt( bool_t val, num_t dt, mbool_t* mb )
{
	mb->v[ dt ] = val;
}

// -----------------------------------------------------------------------------

mbool_t* bli_mbool_create
     (
       bool_t b_s,
       bool_t b_d,
       bool_t b_c,
       bool_t b_z
     );

void bli_mbool_init
     (
       mbool_t* b,
       bool_t   b_s,
       bool_t   b_d,
       bool_t   b_c,
       bool_t   b_z
     );

void bli_mbool_free( mbool_t* b );

// end bli_mbool.h
// begin bli_cntx.h


#ifndef BLIS_CNTX_H
#define BLIS_CNTX_H


// Context object type (defined in bli_type_defs.h)



// -----------------------------------------------------------------------------

//
// -- cntx_t query (fields only) -----------------------------------------------
//

static blksz_t* bli_cntx_blkszs_buf( cntx_t* cntx )
{
	return cntx->blkszs;
}
static bszid_t* bli_cntx_bmults_buf( cntx_t* cntx )
{
	return cntx->bmults;
}
static func_t* bli_cntx_l3_vir_ukrs_buf( cntx_t* cntx )
{
	return cntx->l3_vir_ukrs;
}
static func_t* bli_cntx_l3_nat_ukrs_buf( cntx_t* cntx )
{
	return cntx->l3_nat_ukrs;
}
static mbool_t* bli_cntx_l3_nat_ukrs_prefs_buf( cntx_t* cntx )
{
	return cntx->l3_nat_ukrs_prefs;
}
static func_t* bli_cntx_l1f_kers_buf( cntx_t* cntx )
{
	return cntx->l1f_kers;
}
static func_t* bli_cntx_l1v_kers_buf( cntx_t* cntx )
{
	return cntx->l1v_kers;
}
static func_t* bli_cntx_packm_kers_buf( cntx_t* cntx )
{
	return cntx->packm_kers;
}
static func_t* bli_cntx_unpackm_kers_buf( cntx_t* cntx )
{
	return cntx->unpackm_kers;
}
static ind_t bli_cntx_method( cntx_t* cntx )
{
	return cntx->method;
}
static pack_t bli_cntx_schema_a_block( cntx_t* cntx )
{
	return cntx->schema_a_block;
}
static pack_t bli_cntx_schema_b_panel( cntx_t* cntx )
{
	return cntx->schema_b_panel;
}
static pack_t bli_cntx_schema_c_panel( cntx_t* cntx )
{
	return cntx->schema_c_panel;
}
static bool_t bli_cntx_anti_pref( cntx_t* cntx )
{
	return cntx->anti_pref;
}
static dim_t* bli_cntx_thrloop( cntx_t* cntx )
{
	return cntx->thrloop;
}
static membrk_t* bli_cntx_get_membrk( cntx_t* cntx )
{
	return cntx->membrk;
}

// -----------------------------------------------------------------------------

//
// -- cntx_t modification (fields only) ----------------------------------------
//

static void bli_cntx_set_method( ind_t method, cntx_t* cntx )
{
	cntx->method = method;
}
static void bli_cntx_set_schema_a_block( pack_t schema, cntx_t* cntx )
{
	cntx->schema_a_block = schema;
}
static void bli_cntx_set_schema_b_panel( pack_t schema, cntx_t* cntx )
{
	cntx->schema_b_panel = schema;
}
static void bli_cntx_set_schema_c_panel( pack_t schema, cntx_t* cntx )
{
	cntx->schema_c_panel = schema;
}
static void bli_cntx_set_schema_ab_blockpanel( pack_t sa, pack_t sb, cntx_t* cntx )
{
	bli_cntx_set_schema_a_block( sa, cntx );
	bli_cntx_set_schema_b_panel( sb, cntx );
}
static void bli_cntx_set_anti_pref( bool_t anti_pref, cntx_t* cntx )
{
	cntx->anti_pref = anti_pref;
}
static void bli_cntx_set_membrk( membrk_t* membrk, cntx_t* cntx )
{
	cntx->membrk = membrk;
}

// -----------------------------------------------------------------------------

//
// -- cntx_t query (complex) ---------------------------------------------------
//

static blksz_t* bli_cntx_get_blksz( bszid_t bs_id, cntx_t* cntx )
{
	blksz_t* blkszs = bli_cntx_blkszs_buf( cntx );
	blksz_t* blksz  = &blkszs[ bs_id ];

	// Return the address of the blksz_t identified by bs_id.
	return blksz;
}

static dim_t bli_cntx_get_blksz_def_dt( num_t dt, bszid_t bs_id, cntx_t* cntx )
{
	blksz_t* blksz  = bli_cntx_get_blksz( bs_id, cntx );
	dim_t    bs_dt  = bli_blksz_get_def( dt, blksz );

	// Return the main (default) blocksize value for the datatype given.
	return bs_dt;
}

static dim_t bli_cntx_get_blksz_max_dt( num_t dt, bszid_t bs_id, cntx_t* cntx )
{
	blksz_t* blksz  = bli_cntx_get_blksz( bs_id, cntx );
	dim_t    bs_dt  = bli_blksz_get_max( dt, blksz );

	// Return the auxiliary (maximum) blocksize value for the datatype given.
	return bs_dt;
}

static bszid_t bli_cntx_get_bmult_id( bszid_t bs_id, cntx_t* cntx )
{
	bszid_t* restrict bmults = bli_cntx_bmults_buf( cntx );
	bszid_t           bm_id  = bmults[ bs_id ];

	return bm_id;
}

static blksz_t* bli_cntx_get_bmult( bszid_t bs_id, cntx_t* cntx )
{
	bszid_t           bm_id  = bli_cntx_get_bmult_id( bs_id, cntx );
	blksz_t* restrict bmult  = bli_cntx_get_blksz( bm_id, cntx );

	return bmult;
}

static dim_t bli_cntx_get_bmult_dt( num_t dt, bszid_t bs_id, cntx_t* cntx )
{
	blksz_t* bmult  = bli_cntx_get_bmult( bs_id, cntx );
	dim_t    bm_dt  = bli_blksz_get_def( dt, bmult );

	return bm_dt;
}

// -----------------------------------------------------------------------------

static func_t* bli_cntx_get_l3_ukrs( l3ukr_t ukr_id, cntx_t* cntx )
{
	func_t* funcs;

	if ( bli_cntx_method( (cntx) ) != BLIS_NAT )
		funcs = bli_cntx_l3_vir_ukrs_buf( cntx );
	else
		funcs = bli_cntx_l3_nat_ukrs_buf( cntx );

	func_t* func = &funcs[ ukr_id ];

	return func;
}

static void* bli_cntx_get_l3_ukr_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	func_t* func = bli_cntx_get_l3_ukrs( ukr_id, cntx );

	return bli_func_get_dt( dt, func );
}

static func_t* bli_cntx_get_l3_vir_ukrs( l3ukr_t ukr_id, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l3_vir_ukrs_buf( cntx );
	func_t* func  = &funcs[ ukr_id ];

	return func;
}

static void* bli_cntx_get_l3_vir_ukr_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	func_t* func = bli_cntx_get_l3_vir_ukrs( ukr_id, cntx );

	return bli_func_get_dt( dt, func );
}

static func_t* bli_cntx_get_l3_nat_ukrs( l3ukr_t ukr_id, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l3_nat_ukrs_buf( cntx );
	func_t* func  = &funcs[ ukr_id ];

	return func;
}

static void* bli_cntx_get_l3_nat_ukr_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	func_t* func = bli_cntx_get_l3_nat_ukrs( ukr_id, cntx );

	return bli_func_get_dt( dt, func );
}

// -----------------------------------------------------------------------------

static mbool_t* bli_cntx_get_l3_nat_ukr_prefs( l3ukr_t ukr_id, cntx_t* cntx )
{
	mbool_t* mbools = bli_cntx_l3_nat_ukrs_prefs_buf( cntx );
	mbool_t* mbool  = &mbools[ ukr_id ];

	return mbool;
}

static bool_t bli_cntx_get_l3_nat_ukr_prefs_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	mbool_t* mbool = bli_cntx_get_l3_nat_ukr_prefs( ukr_id, cntx );

	return bli_mbool_get_dt( dt, mbool );
}

// -----------------------------------------------------------------------------

static func_t* bli_cntx_get_l1f_kers( l1fkr_t ker_id, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l1f_kers_buf( cntx );
	func_t* func  = &funcs[ ker_id ];

	return func;
}

static void* bli_cntx_get_l1f_ker_dt( num_t dt, l1fkr_t ker_id, cntx_t* cntx )
{
	func_t* func = bli_cntx_get_l1f_kers( ker_id, cntx );

	return bli_func_get_dt( dt, func );
}

// -----------------------------------------------------------------------------

static func_t* bli_cntx_get_l1v_kers( l1vkr_t ker_id, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l1v_kers_buf( cntx );
	func_t* func  = &funcs[ ker_id ];

	return func;
}

static void* bli_cntx_get_l1v_ker_dt( num_t dt, l1vkr_t ker_id, cntx_t* cntx )
{
	func_t* func = bli_cntx_get_l1v_kers( ker_id, cntx );

	return bli_func_get_dt( dt, func );
}

// -----------------------------------------------------------------------------

static func_t* bli_cntx_get_packm_kers( l1mkr_t ker_id, cntx_t* cntx )
{
	func_t* func = NULL;

	// Only index to the requested packm func_t if the packm kernel being
	// requested is one that is explicitly supported.
	if ( 0 <= ( gint_t )ker_id &&
	          ( gint_t )ker_id < BLIS_NUM_PACKM_KERS )
	{
		func_t* funcs = bli_cntx_packm_kers_buf( cntx );

		func = &funcs[ ker_id ];
	}

	return func;
}

static void* bli_cntx_get_packm_ker_dt( num_t dt, l1mkr_t ker_id, cntx_t* cntx )
{
	void* fp = NULL;

	// Only query the context for the packm func_t (and then extract the
	// datatype-specific function pointer) if the packm kernel being
	// requested is one that is explicitly supported.
	if ( 0 <= ( gint_t )ker_id &&
	          ( gint_t )ker_id < BLIS_NUM_PACKM_KERS )
	{
		func_t* func = bli_cntx_get_packm_kers( ker_id, cntx );

		fp = bli_func_get_dt( dt, func );
	}

	return fp;
}

static func_t* bli_cntx_get_unpackm_kers( l1mkr_t ker_id, cntx_t* cntx )
{
	func_t* func = NULL;

	// Only index to the requested unpackm func_t if the unpackm kernel being
	// requested is one that is explicitly supported.
	if ( 0 <= ( gint_t )ker_id &&
	          ( gint_t )ker_id < BLIS_NUM_UNPACKM_KERS )
	{
		func_t* funcs = bli_cntx_unpackm_kers_buf( cntx );

		func = &funcs[ ker_id ];
	}

	return func;
}

static void* bli_cntx_get_unpackm_ker_dt( num_t dt, l1mkr_t ker_id, cntx_t* cntx )
{
	void* fp = NULL;

	// Only query the context for the unpackm func_t (and then extract the
	// datatype-specific function pointer) if the unpackm kernel being
	// requested is one that is explicitly supported.
	if ( 0 <= ( gint_t )ker_id &&
	          ( gint_t )ker_id < BLIS_NUM_UNPACKM_KERS )
	{
		func_t* func = bli_cntx_get_unpackm_kers( ker_id, cntx );

		fp = bli_func_get_dt( dt, func );
	}

	return fp;
}

// -----------------------------------------------------------------------------

static dim_t bli_cntx_jc_way( cntx_t* cntx )
{
	return cntx->thrloop[ BLIS_NC ];
}
static dim_t bli_cntx_pc_way( cntx_t* cntx )
{
	return cntx->thrloop[ BLIS_KC ];
}
static dim_t bli_cntx_ic_way( cntx_t* cntx )
{
	return cntx->thrloop[ BLIS_MC ];
}
static dim_t bli_cntx_jr_way( cntx_t* cntx )
{
	return cntx->thrloop[ BLIS_NR ];
}
static dim_t bli_cntx_ir_way( cntx_t* cntx )
{
	return cntx->thrloop[ BLIS_MR ];
}
static dim_t bli_cntx_pr_way( cntx_t* cntx )
{
	return cntx->thrloop[ BLIS_KR ];
}

static dim_t bli_cntx_way_for_bszid( bszid_t bszid, cntx_t* cntx )
{
	return cntx->thrloop[ bszid ];
}

static dim_t bli_cntx_get_num_threads( cntx_t* cntx )
{
	return bli_cntx_jc_way( cntx ) *
	       bli_cntx_pc_way( cntx ) *
	       bli_cntx_ic_way( cntx ) *
	       bli_cntx_jr_way( cntx ) *
	       bli_cntx_ir_way( cntx );
}

// -----------------------------------------------------------------------------

static bool_t bli_cntx_l3_nat_ukr_prefers_rows_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	bool_t prefs = bli_cntx_get_l3_nat_ukr_prefs_dt( dt, ukr_id, cntx );

	// A ukernel preference of TRUE means the ukernel prefers row storage.
	return prefs == TRUE;
}

static bool_t bli_cntx_l3_nat_ukr_prefers_cols_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	bool_t prefs = bli_cntx_get_l3_nat_ukr_prefs_dt( dt, ukr_id, cntx );

	// A ukernel preference of FALSE means the ukernel prefers column storage.
	return prefs == FALSE;
}

static bool_t bli_cntx_l3_nat_ukr_prefers_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	const num_t  dt    = bli_obj_datatype( *obj );
	const bool_t ukr_prefers_rows
	                   = bli_cntx_l3_nat_ukr_prefers_rows_dt( dt, ukr_id, cntx );
	const bool_t ukr_prefers_cols
	                   = bli_cntx_l3_nat_ukr_prefers_cols_dt( dt, ukr_id, cntx );
	bool_t       r_val = FALSE;

	if      ( bli_obj_is_row_stored( *obj ) && ukr_prefers_rows ) r_val = TRUE;
	else if ( bli_obj_is_col_stored( *obj ) && ukr_prefers_cols ) r_val = TRUE;

	return r_val;
}

static bool_t bli_cntx_l3_nat_ukr_dislikes_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	return !bli_cntx_l3_nat_ukr_prefers_storage_of( obj, ukr_id, cntx );
}

static bool_t bli_cntx_l3_nat_ukr_eff_prefers_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	bool_t r_val = bli_cntx_l3_nat_ukr_prefers_storage_of( obj, ukr_id, cntx );

	// If the anti-preference is set, negate the result.
	if ( bli_cntx_anti_pref( cntx ) ) r_val = !r_val;

	return r_val;
}

static bool_t bli_cntx_l3_nat_ukr_eff_dislikes_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	bool_t r_val = bli_cntx_l3_nat_ukr_dislikes_storage_of( obj, ukr_id, cntx );

	// If the anti-preference is set, negate the result.
	if ( bli_cntx_anti_pref( cntx ) ) r_val = !r_val;

	return r_val;
}

// -----------------------------------------------------------------------------

static bool_t bli_cntx_l3_ukr_prefers_rows_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	// For induced methods, return the ukernel storage preferences of the
	// corresponding real micro-kernel.
	if ( bli_cntx_method( cntx ) != BLIS_NAT )
	    dt = bli_datatype_proj_to_real( dt );

	return bli_cntx_l3_nat_ukr_prefers_rows_dt( dt, ukr_id, cntx );
}

static bool_t bli_cntx_l3_ukr_prefers_cols_dt( num_t dt, l3ukr_t ukr_id, cntx_t* cntx )
{
	// For induced methods, return the ukernel storage preferences of the
	// corresponding real micro-kernel.
	if ( bli_cntx_method( cntx ) != BLIS_NAT )
	    dt = bli_datatype_proj_to_real( dt );

	return bli_cntx_l3_nat_ukr_prefers_cols_dt( dt, ukr_id, cntx );
}

static bool_t bli_cntx_l3_ukr_prefers_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	const num_t  dt    = bli_obj_datatype( *obj );
	const bool_t ukr_prefers_rows
	                   = bli_cntx_l3_ukr_prefers_rows_dt( dt, ukr_id, cntx );
	const bool_t ukr_prefers_cols
	                   = bli_cntx_l3_ukr_prefers_cols_dt( dt, ukr_id, cntx );
	bool_t       r_val = FALSE;

	if      ( bli_obj_is_row_stored( *obj ) && ukr_prefers_rows ) r_val = TRUE;
	else if ( bli_obj_is_col_stored( *obj ) && ukr_prefers_cols ) r_val = TRUE;

	return r_val;
}

static bool_t bli_cntx_l3_ukr_dislikes_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	return !bli_cntx_l3_ukr_prefers_storage_of( obj, ukr_id, cntx );
}

static bool_t bli_cntx_l3_ukr_eff_prefers_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	bool_t r_val = bli_cntx_l3_ukr_prefers_storage_of( obj, ukr_id, cntx );

	// If the anti-preference is set, negate the result.
	if ( bli_cntx_anti_pref( cntx ) ) r_val = !r_val;

	return r_val;
}

static bool_t bli_cntx_l3_ukr_eff_dislikes_storage_of( obj_t* obj, l3ukr_t ukr_id, cntx_t* cntx )
{
	bool_t r_val = bli_cntx_l3_ukr_dislikes_storage_of( obj, ukr_id, cntx );

	// If the anti-preference is set, negate the result.
	if ( bli_cntx_anti_pref( cntx ) ) r_val = !r_val;

	return r_val;
}

// -----------------------------------------------------------------------------

//
// -- cntx_t modification (complex) --------------------------------------------
//

static void bli_cntx_set_blksz( bszid_t bs_id, blksz_t* blksz, bszid_t mult_id, cntx_t* cntx )
{
	blksz_t* blkszs = bli_cntx_blkszs_buf( cntx );
	bszid_t* bmults = bli_cntx_bmults_buf( cntx );

	blkszs[ bs_id ] = *blksz;
	bmults[ bs_id ] = mult_id;
}

static void bli_cntx_set_l3_vir_ukr( l3ukr_t ukr_id, func_t* func, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l3_vir_ukrs_buf( cntx );

	funcs[ ukr_id ] = *func;
}

static void bli_cntx_set_l3_nat_ukr( l3ukr_t ukr_id, func_t* func, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l3_nat_ukrs_buf( cntx );

	funcs[ ukr_id ] = *func;
}

static void bli_cntx_set_l3_nat_ukr_prefs( l3ukr_t ukr_id, mbool_t* prefs, cntx_t* cntx )
{
	mbool_t* mbools = bli_cntx_l3_nat_ukrs_prefs_buf( cntx );

	mbools[ ukr_id ] = *prefs;
}

static void bli_cntx_set_l1f_ker( l1fkr_t ker_id, func_t* func, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l1f_kers_buf( cntx );

	funcs[ ker_id ] = *func;
}

static void bli_cntx_set_l1v_ker( l1vkr_t ker_id, func_t* func, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_l1v_kers_buf( cntx );

	funcs[ ker_id ] = *func;
}

static void bli_cntx_set_packm_ker( l1mkr_t ker_id, func_t* func, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_get_packm_kers( ker_id, cntx );

	funcs[ ker_id ] = *func;
}

static void bli_cntx_set_packm_ker_dt( void* fp, num_t dt, l1mkr_t ker_id, cntx_t* cntx )
{
	func_t* func = bli_cntx_get_packm_ker_dt( dt, ker_id, cntx );

	bli_func_set_dt( fp, dt, func );
}

static void bli_cntx_set_unpackm_ker( l1mkr_t ker_id, func_t* func, cntx_t* cntx )
{
	func_t* funcs = bli_cntx_get_unpackm_kers( ker_id, cntx );

	funcs[ ker_id ] = *func;
}

static void bli_cntx_set_unpackm_ker_dt( void* fp, num_t dt, l1mkr_t ker_id, cntx_t* cntx )
{
	func_t* func = bli_cntx_get_unpackm_ker_dt( dt, ker_id, cntx );

	bli_func_set_dt( fp, dt, func );
}

static void bli_cntx_set_thrloop( dim_t jc, dim_t pc, dim_t ic, dim_t jr, dim_t ir, cntx_t* cntx )
{
	cntx->thrloop[ BLIS_NC ] = jc;
	cntx->thrloop[ BLIS_KC ] = pc;
	cntx->thrloop[ BLIS_MC ] = ic;
	cntx->thrloop[ BLIS_NR ] = jr;
	cntx->thrloop[ BLIS_MR ] = ir;
	cntx->thrloop[ BLIS_KR ] = 1;
}

// -----------------------------------------------------------------------------

// Function prototypes

void  bli_cntx_clear( cntx_t* cntx );

dim_t bli_cntx_get_num_threads_in( cntx_t* cntx, cntl_t* cntl );

void  bli_cntx_set_blkszs( ind_t method, dim_t n_bs, ... );

void  bli_cntx_set_ind_blkszs( ind_t method, dim_t n_bs, ... );

void  bli_cntx_set_l3_nat_ukrs( dim_t n_ukrs, ... );
void  bli_cntx_set_l1f_kers( dim_t n_kers, ... );
void  bli_cntx_set_l1v_kers( dim_t n_kers, ... );
void  bli_cntx_set_packm_kers( dim_t n_kers, ... );

void  bli_cntx_set_thrloop_from_env( opid_t  l3_op,
                                     side_t  side,
                                     cntx_t* cntx,
                                     dim_t m,
                                     dim_t n,
                                     dim_t k );

void  bli_cntx_print( cntx_t* cntx );


#endif

// end bli_cntx.h
// begin bli_gks.h


#ifndef BLIS_GKS_H
#define BLIS_GKS_H

arch_t  bli_arch_query_id( void );

// -----------------------------------------------------------------------------

void    bli_gks_init( void );
void    bli_gks_finalize( void );

void    bli_gks_init_index( void );

cntx_t* bli_gks_lookup_nat_cntx( arch_t id );
cntx_t* bli_gks_lookup_ind_cntx( arch_t id, ind_t ind );
void    bli_gks_register_cntx( arch_t id, void* nat_fp, void* ref_fp, void* ind_fp );

cntx_t* bli_gks_query_cntx( void );
cntx_t* bli_gks_query_nat_cntx( void );
cntx_t* bli_gks_query_cntx_noinit( void );
cntx_t* bli_gks_query_ind_cntx( ind_t ind, num_t dt );

void    bli_gks_init_ref_cntx( cntx_t* cntx );

bool_t  bli_gks_cntx_l3_nat_ukr_is_ref( num_t dt, l3ukr_t ukr_id, cntx_t* cntx );

char*   bli_gks_l3_ukr_impl_string( l3ukr_t ukr, ind_t method, num_t dt );
kimpl_t bli_gks_l3_ukr_impl_type( l3ukr_t ukr, ind_t method, num_t dt );

//char*   bli_gks_l3_ukr_avail_impl_string( l3ukr_t ukr, num_t dt );


#endif

// end bli_gks.h
// begin bli_ind.h


#ifndef BLIS_IND_H
#define BLIS_IND_H

// level-3 induced method management
// begin bli_l3_ind.h


#ifndef BLIS_L3_IND_H
#define BLIS_L3_IND_H

// -----------------------------------------------------------------------------

#undef  GENPROT
#define GENPROT( opname ) \
\
void*  PASTEMAC(opname,ind_get_avail)( num_t dt );


GENPROT( gemm )
GENPROT( hemm )
GENPROT( herk )
GENPROT( her2k )
GENPROT( symm )
GENPROT( syrk )
GENPROT( syr2k )
GENPROT( trmm3 )
GENPROT( trmm )
GENPROT( trsm )

// -----------------------------------------------------------------------------

//bool_t bli_l3_ind_oper_is_avail( opid_t oper, ind_t method, num_t dt );

ind_t  bli_l3_ind_oper_find_avail( opid_t oper, num_t dt );

void   bli_l3_ind_set_enable_dt( ind_t method, num_t dt, bool_t status );

void   bli_l3_ind_oper_enable_only( opid_t oper, ind_t method, num_t dt );
void   bli_l3_ind_oper_set_enable_all( opid_t oper, num_t dt, bool_t status );

void   bli_l3_ind_oper_set_enable( opid_t oper, ind_t method, num_t dt, bool_t status );
bool_t bli_l3_ind_oper_get_enable( opid_t oper, ind_t method, num_t dt );

void*  bli_l3_ind_oper_get_func( opid_t oper, ind_t method );


#endif

// end bli_l3_ind.h

// level-3 object APIs
// begin bli_l3_ind_oapi.h



//
// Generate object-based prototypes for induced methods that work for
// trmm and trsm (ie: two-operand operations).
//
#undef  GENPROT
#define GENPROT( imeth ) \
\
void PASTEMAC(gemm,imeth) (              obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(hemm,imeth) ( side_t side, obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(herk,imeth) (              obj_t* alpha, obj_t* a,           obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(her2k,imeth)(              obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(symm,imeth) ( side_t side, obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(syrk,imeth) (              obj_t* alpha, obj_t* a,           obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(syr2k,imeth)(              obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(trmm3,imeth)( side_t side, obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(trmm,imeth) ( side_t side, obj_t* alpha, obj_t* a, obj_t* b,                        cntx_t* cntx ); \
void PASTEMAC(trsm,imeth) ( side_t side, obj_t* alpha, obj_t* a, obj_t* b,                        cntx_t* cntx );

GENPROT( nat )
GENPROT( ind )
GENPROT( 3m1 )
GENPROT( 4m1 )
GENPROT( 1m )


//
// Generate object-based prototypes for induced methods that do NOT work
// for trmm and trsm (ie: two-operand operations).
//
#undef  GENPROT_NO2OP
#define GENPROT_NO2OP( imeth ) \
\
void PASTEMAC(gemm,imeth) (              obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(hemm,imeth) ( side_t side, obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(herk,imeth) (              obj_t* alpha, obj_t* a,           obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(her2k,imeth)(              obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(symm,imeth) ( side_t side, obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(syrk,imeth) (              obj_t* alpha, obj_t* a,           obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(syr2k,imeth)(              obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx ); \
void PASTEMAC(trmm3,imeth)( side_t side, obj_t* alpha, obj_t* a, obj_t* b, obj_t* beta, obj_t* c, cntx_t* cntx );

GENPROT_NO2OP( 3mh )
GENPROT_NO2OP( 4mh )
GENPROT_NO2OP( 4mb )


//
// Generate object-based prototypes for 1m methods that specify an algorithm
// (e.g., block-panel or panel-block).
//



//GENPROT( 1m, bp )
//GENPROT( 1m, pb )

// end bli_l3_ind_oapi.h

// level-3 typed APIs
// begin bli_l3_ind_tapi.h



#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       trans_t transa, \
       trans_t transb, \
       dim_t   m, \
       dim_t   n, \
       dim_t   k, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( gemm3mh )
INSERT_GENTPROT_BASIC0( gemm3m1 )
INSERT_GENTPROT_BASIC0( gemm4mh )
INSERT_GENTPROT_BASIC0( gemm4mb )
INSERT_GENTPROT_BASIC0( gemm4m1 )
INSERT_GENTPROT_BASIC0( gemm1m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       conj_t  conja, \
       trans_t transb, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( hemm3mh )
INSERT_GENTPROT_BASIC0( hemm3m1 )
INSERT_GENTPROT_BASIC0( hemm4mh )
INSERT_GENTPROT_BASIC0( hemm4m1 )
INSERT_GENTPROT_BASIC0( hemm1m )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t   uploc, \
       trans_t  transa, \
       trans_t  transb, \
       dim_t    m, \
       dim_t    k, \
       ctype*   alpha, \
       ctype*   a, inc_t rs_a, inc_t cs_a, \
       ctype*   b, inc_t rs_b, inc_t cs_b, \
       ctype_r* beta, \
       ctype*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( her2k3mh )
INSERT_GENTPROTR_BASIC0( her2k3m1 )
INSERT_GENTPROTR_BASIC0( her2k4mh )
INSERT_GENTPROTR_BASIC0( her2k4m1 )
INSERT_GENTPROTR_BASIC0( her2k1m )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t   uploc, \
       trans_t  transa, \
       dim_t    m, \
       dim_t    k, \
       ctype_r* alpha, \
       ctype*   a, inc_t rs_a, inc_t cs_a, \
       ctype_r* beta, \
       ctype*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( herk3mh )
INSERT_GENTPROTR_BASIC0( herk3m1 )
INSERT_GENTPROTR_BASIC0( herk4mh )
INSERT_GENTPROTR_BASIC0( herk4m1 )
INSERT_GENTPROTR_BASIC0( herk1m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       conj_t  conja, \
       trans_t transb, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( symm3mh )
INSERT_GENTPROT_BASIC0( symm3m1 )
INSERT_GENTPROT_BASIC0( symm4mh )
INSERT_GENTPROT_BASIC0( symm4m1 )
INSERT_GENTPROT_BASIC0( symm1m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploc, \
       trans_t transa, \
       trans_t transb, \
       dim_t   m, \
       dim_t   k, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( syr2k3mh )
INSERT_GENTPROT_BASIC0( syr2k3m1 )
INSERT_GENTPROT_BASIC0( syr2k4mh )
INSERT_GENTPROT_BASIC0( syr2k4m1 )
INSERT_GENTPROT_BASIC0( syr2k1m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploc, \
       trans_t transa, \
       dim_t   m, \
       dim_t   k, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( syrk3mh )
INSERT_GENTPROT_BASIC0( syrk3m1 )
INSERT_GENTPROT_BASIC0( syrk4mh )
INSERT_GENTPROT_BASIC0( syrk4m1 )
INSERT_GENTPROT_BASIC0( syrk1m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       trans_t transb, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trmm33mh )
INSERT_GENTPROT_BASIC0( trmm33m1 )
INSERT_GENTPROT_BASIC0( trmm34mh )
INSERT_GENTPROT_BASIC0( trmm34m1 )
INSERT_GENTPROT_BASIC0( trmm31m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trmm3m1 )
INSERT_GENTPROT_BASIC0( trmm4m1 )
INSERT_GENTPROT_BASIC0( trmm1m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trsm3m1 )
INSERT_GENTPROT_BASIC0( trsm4m1 )
INSERT_GENTPROT_BASIC0( trsm1m )

// end bli_l3_ind_tapi.h

// level-3 misc. optimizations
// begin bli_l3_ind_opt.h


#ifndef BLIS_L3_IND_OPT_H
#define BLIS_L3_IND_OPT_H

#define bli_l3_ind_recast_1m_params( dt_exec, schema_a, c, \
                                     m, n, k, \
                                     pd_a, ps_a, \
                                     pd_b, ps_b, \
                                     rs_c, cs_c ) \
{ \
	obj_t beta; \
\
	 \
	bli_obj_scalar_detach( c, &beta ); \
\
	 \
	if ( bli_obj_imag_equals( &beta, &BLIS_ZERO ) && \
	     !bli_is_gen_stored( rs_c, cs_c ) ) \
	{ \
		dt_exec = bli_datatype_proj_to_real( dt_exec ); \
\
		if ( bli_is_1e_packed( schema_a ) ) \
		{ \
			m    *= 2; \
			n    *= 1; \
			k    *= 2; \
			pd_a *= 2; ps_a *= 2; \
			pd_b *= 1; ps_b *= 2; \
			rs_c *= 1; cs_c *= 2; \
		} \
		else  \
		{ \
			m    *= 1; \
			n    *= 2; \
			k    *= 2; \
			pd_a *= 1; ps_a *= 2; \
			pd_b *= 2; ps_b *= 2; \
			rs_c *= 2; cs_c *= 1; \
		} \
	} \
}

#endif
// end bli_l3_ind_opt.h

// level-3 cntx initialization
// begin bli_cntx_ind_stage.h


void  bli_cntx_ind_stage( ind_t method, dim_t stage, cntx_t* cntx );

void  bli_cntx_3mh_stage( dim_t stage, cntx_t* cntx );
void  bli_cntx_3m1_stage( dim_t stage, cntx_t* cntx );
void  bli_cntx_4mh_stage( dim_t stage, cntx_t* cntx );
void  bli_cntx_4mb_stage( dim_t stage, cntx_t* cntx );
void  bli_cntx_4m1_stage( dim_t stage, cntx_t* cntx );
void  bli_cntx_1m_stage( dim_t stage, cntx_t* cntx );
void  bli_cntx_nat_stage( dim_t stage, cntx_t* cntx );

// end bli_cntx_ind_stage.h


void   bli_ind_init( void );
void   bli_ind_finalize( void );

void   bli_ind_enable( ind_t method );
void   bli_ind_disable( ind_t method );
void   bli_ind_disable_all( void );

void   bli_ind_enable_dt( ind_t method, num_t dt );
void   bli_ind_disable_dt( ind_t method, num_t dt );
void   bli_ind_disable_all_dt( num_t dt );

void   bli_ind_oper_enable_only( opid_t oper, ind_t method, num_t dt );

bool_t bli_ind_oper_is_impl( opid_t oper, ind_t method );
//bool_t bli_ind_oper_has_avail( opid_t oper, num_t dt );
void*  bli_ind_oper_get_avail( opid_t oper, num_t dt );
ind_t  bli_ind_oper_find_avail( opid_t oper, num_t dt );
char*  bli_ind_oper_get_avail_impl_string( opid_t oper, num_t dt );

char*  bli_ind_get_impl_string( ind_t method );
num_t  bli_ind_map_cdt_to_index( num_t dt );


#endif

// end bli_ind.h
// begin bli_membrk.h


#ifndef BLIS_MEMBRK_H
#define BLIS_MEMBRK_H

// membrk query

static pool_t* bli_membrk_pool( dim_t pool_index, membrk_t* membrk )
{
	return &(membrk->pools[ pool_index ]);
}

static mtx_t* bli_membrk_mutex( membrk_t* membrk )
{
	return &(membrk->mutex);
}

static void* bli_membrk_malloc_fp( membrk_t* membrk )
{
	return membrk->malloc_fp;
}

static void* bli_membrk_free_fp( membrk_t* membrk )
{
	return membrk->free_fp;
}

// membrk modification

static void bli_membrk_set_malloc_fp( void* malloc_fp, membrk_t* membrk )
{
	membrk->malloc_fp = malloc_fp;
}

static void bli_membrk_set_free_fp( void* free_fp, membrk_t* membrk )
{
	membrk->free_fp = free_fp;
}

// membrk action

static void bli_membrk_lock( membrk_t* membrk )
{
	bli_mutex_lock( &(membrk->mutex) );
}

static void bli_membrk_unlock( membrk_t* membrk )
{
	bli_mutex_unlock( &(membrk->mutex) );
}

static void* bli_membrk_malloc( size_t size, membrk_t* membrk )
{
	// Call the malloc()-style function in membrk.
	return ((membrk)->malloc_fp)( size );
}

static void bli_membrk_free( void* p, membrk_t* membrk )
{
	// Call the free()-style function in membrk.
	((membrk)->free_fp)( p );
}


// -----------------------------------------------------------------------------

void bli_membrk_init
     (
       cntx_t*   cntx,
       membrk_t* membrk
     );
void bli_membrk_finalize
     (
       membrk_t* membrk
     );

void bli_membrk_acquire_m
     (
       membrk_t* membrk,
       siz_t     req_size,
       packbuf_t buf_type,
       mem_t*    mem
     );

void bli_membrk_acquire_v
     (
       membrk_t* membrk,
       siz_t     req_size,
       mem_t*    mem
     );

void bli_membrk_release
     (
       mem_t* mem
     );

siz_t bli_membrk_pool_size
     (
       membrk_t* membrk,
       packbuf_t buf_type
     );

// ----------------------------------------------------------------------------

void bli_membrk_init_pools
     (
       cntx_t*   cntx,
       membrk_t* membrk
     );
void bli_membrk_finalize_pools
     (
       membrk_t* membrk
     );

void bli_membrk_compute_pool_block_sizes
     (
       siz_t*  bs_a,
       siz_t*  bs_b,
       siz_t*  bs_c,
       cntx_t* cntx
     );
void bli_membrk_compute_pool_block_sizes_dt
     (
       num_t   dt,
       siz_t*  bs_a,
       siz_t*  bs_b,
       siz_t*  bs_c,
       cntx_t* cntx
     );

#endif

// end bli_membrk.h
// begin bli_pool.h


#ifndef BLIS_POOL_H
#define BLIS_POOL_H

// -- Pool block type --



// -- Pool type --




// Pool block query

static void* bli_pblk_buf_sys( pblk_t* pblk )
{
    return pblk->buf_sys;
}

static void* bli_pblk_buf_align( pblk_t* pblk )
{
    return pblk->buf_align;
}

// Pool block modification

static void bli_pblk_set_buf_sys( void* buf_sys, pblk_t* pblk )
{
    pblk->buf_sys = buf_sys;
}

static void bli_pblk_set_buf_align( void* buf_align, pblk_t* pblk )
{
    pblk->buf_align = buf_align;
}

static void bli_pblk_clear( pblk_t* pblk )
{
	bli_pblk_set_buf_sys( NULL, pblk );
	bli_pblk_set_buf_align( NULL, pblk );
}


// Pool entry query

static pblk_t* bli_pool_block_ptrs( pool_t* pool )
{
	return pool->block_ptrs;
}

static dim_t bli_pool_block_ptrs_len( pool_t* pool )
{
	return pool->block_ptrs_len;
}

static dim_t bli_pool_num_blocks( pool_t* pool )
{
	return pool->num_blocks;
}

static siz_t bli_pool_block_size( pool_t* pool )
{
	return pool->block_size;
}

static siz_t bli_pool_align_size( pool_t* pool )
{
	return pool->align_size;
}

static dim_t bli_pool_top_index( pool_t* pool )
{
	return pool->top_index;
}

static bool_t bli_pool_is_exhausted( pool_t* pool )
{
	return bli_pool_top_index( pool ) == bli_pool_num_blocks( pool );
}

// Pool entry modification

static void bli_pool_set_block_ptrs( pblk_t* block_ptrs, pool_t* pool ) \
{
    pool->block_ptrs = block_ptrs;
}

static void bli_pool_set_block_ptrs_len( dim_t block_ptrs_len, pool_t* pool ) \
{
    pool->block_ptrs_len = block_ptrs_len;
}

static void bli_pool_set_num_blocks( dim_t num_blocks, pool_t* pool ) \
{
    pool->num_blocks = num_blocks;
}

static void bli_pool_set_block_size( siz_t block_size, pool_t* pool ) \
{
    pool->block_size = block_size;
}

static void bli_pool_set_align_size( siz_t align_size, pool_t* pool ) \
{
    pool->align_size = align_size;
}

static void bli_pool_set_top_index( dim_t top_index, pool_t* pool ) \
{
    pool->top_index = top_index;
}

// -----------------------------------------------------------------------------

void bli_pool_init( dim_t   num_blocks,
                    siz_t   block_size,
                    siz_t   align_size,
                    pool_t* pool );
void bli_pool_finalize( pool_t* pool );
void bli_pool_reinit( dim_t   num_blocks_new,
                      siz_t   block_size_new,
                      siz_t   align_size_new,
                      pool_t* pool );

void bli_pool_checkout_block( siz_t req_size, pblk_t* block, pool_t* pool );
void bli_pool_checkin_block( pblk_t* block, pool_t* pool );

void bli_pool_grow( dim_t num_blocks_add, pool_t* pool );
void bli_pool_shrink( dim_t num_blocks_sub, pool_t* pool );

void bli_pool_alloc_block( siz_t   block_size,
                           siz_t   align_size,
                           pblk_t* block );
void bli_pool_free_block( pblk_t* block );

void bli_pool_print( pool_t* pool );
void bli_pblk_print( pblk_t* pblk );

#endif

// end bli_pool.h
// begin bli_memsys.h


#ifndef BLIS_MEMSYS_H
#define BLIS_MEMSYS_H

// -----------------------------------------------------------------------------

membrk_t* bli_memsys_global_membrk( void );

// -----------------------------------------------------------------------------

void bli_memsys_init( void );
void bli_memsys_finalize( void );


#endif

// end bli_memsys.h
// begin bli_mem.h


#ifndef BLIS_MEM_H
#define BLIS_MEM_H


// Mem entry query

static pblk_t* bli_mem_pblk( mem_t* mem )
{
	return &(mem->pblk);
}

static void* bli_mem_buffer( mem_t* mem )
{
	return bli_pblk_buf_align( bli_mem_pblk( mem ) );
}

static void* bli_mem_buf_sys( mem_t* mem )
{
	return bli_pblk_buf_sys( bli_mem_pblk( mem ) );
}

static packbuf_t bli_mem_buf_type( mem_t* mem )
{
	return mem->buf_type;
}

static pool_t* bli_mem_pool( mem_t* mem )
{
	return mem->pool;
}

static membrk_t* bli_mem_membrk( mem_t* mem )
{
	return mem->membrk;
}

static siz_t bli_mem_size( mem_t* mem )
{
	return mem->size;
}

static bool_t bli_mem_is_alloc( mem_t* mem )
{
	return bli_mem_buffer( mem ) != NULL;
}

static bool_t bli_mem_is_unalloc( mem_t* mem )
{
	return bli_mem_buffer( mem ) == NULL;
}


// Mem entry modification

static void bli_mem_set_pblk( pblk_t* pblk, mem_t* mem )
{
	mem->pblk = *pblk;
}

static void bli_mem_set_buffer( void* buf, mem_t* mem )
{
	bli_pblk_set_buf_align( buf, &(mem->pblk) );
}

static void bli_mem_set_buf_sys( void* buf, mem_t* mem )
{
	bli_pblk_set_buf_sys( buf, &(mem->pblk) );
}

static void bli_mem_set_buf_type( packbuf_t buf_type, mem_t* mem )
{
	mem->buf_type = buf_type;
}

static void bli_mem_set_pool( pool_t* pool, mem_t* mem )
{
	mem->pool = pool;
}

static void bli_mem_set_membrk( membrk_t* membrk, mem_t* mem )
{
	mem->membrk = membrk;
}

static void bli_mem_set_size( siz_t size, mem_t* mem )
{
	mem->size = size;
}

static void bli_mem_clear( mem_t* mem )
{
	bli_mem_set_buffer( NULL, mem );
	bli_mem_set_buf_sys( NULL, mem );
	bli_mem_set_pool( NULL, mem );
	bli_mem_set_size( 0, mem );
	bli_mem_set_membrk( NULL, mem );
}


#endif 
// end bli_mem.h
// begin bli_part.h


// begin bli_part_check.h


void bli_acquire_mpart_t2b_check( subpart_t  requested_part,
                                      dim_t  i,
                                      dim_t  b,
                                      obj_t* obj,
                                      obj_t* sub_obj );

void bli_acquire_mpart_l2r_check( subpart_t  requested_part,
                                      dim_t  j,
                                      dim_t  b,
                                      obj_t* obj,
                                      obj_t* sub_obj );

void bli_acquire_mpart_tl2br_check( subpart_t  requested_part,
                                        dim_t  ij,
                                        dim_t  b,
                                        obj_t* obj,
                                        obj_t* sub_obj );

// end bli_part_check.h

// -- Matrix partitioning ------------------------------------------------------

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0( opname ) \
     ( \
       dir_t     direct, \
       subpart_t req_part, \
       dim_t     i, \
       dim_t     b, \
       obj_t*    obj, \
       obj_t*    sub_obj \
     );

GENPROT( acquire_mpart_mdim )
GENPROT( acquire_mpart_ndim )

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0( opname ) \
     ( \
       subpart_t req_part, \
       dim_t     i, \
       dim_t     b, \
       obj_t*    obj, \
       obj_t*    sub_obj \
     );

GENPROT( acquire_mpart_t2b )
GENPROT( acquire_mpart_b2t )
GENPROT( acquire_mpart_l2r )
GENPROT( acquire_mpart_r2l )
GENPROT( acquire_mpart_tl2br )
GENPROT( acquire_mpart_br2tl )

// -- Vector partitioning ------------------------------------------------------

GENPROT( acquire_vpart_f2b )
GENPROT( acquire_vpart_b2f )

// -- Scalar acquisition -------------------------------------------------------

void bli_acquire_mij
     (
       dim_t     i,
       dim_t     j,
       obj_t*    obj,
       obj_t*    sub_obj
     );

void bli_acquire_vi
     (
       dim_t     i,
       obj_t*    obj,
       obj_t*    sub_obj
     );

// end bli_part.h
// begin bli_prune.h


void bli_prune_unref_mparts( obj_t* p, mdim_t mdim_p,
                             obj_t* s, mdim_t mdim_s );
// end bli_prune.h
// begin bli_query.h


bool_t bli_obj_equals( obj_t* a,
                       obj_t* b );

bool_t bli_obj_imag_equals( obj_t* a,
                            obj_t* b );
// end bli_query.h
// begin bli_auxinfo.h


#ifndef BLIS_AUXINFO_MACRO_DEFS_H
#define BLIS_AUXINFO_MACRO_DEFS_H


// auxinfo_t field query

static pack_t bli_auxinfo_schema_a( auxinfo_t* ai )
{
	return ai->schema_a;
}
static pack_t bli_auxinfo_schema_b( auxinfo_t* ai )
{
	return ai->schema_b;
}

static void* bli_auxinfo_next_a( auxinfo_t* ai )
{
	return ai->a_next;
}
static void* bli_auxinfo_next_b( auxinfo_t* ai )
{
	return ai->a_next;
}

static inc_t bli_auxinfo_is_a( auxinfo_t* ai )
{
	return ai->is_a;
}
static inc_t bli_auxinfo_is_b( auxinfo_t* ai )
{
	return ai->is_b;
}


// auxinfo_t field modification

static void bli_auxinfo_set_schema_a( pack_t schema, auxinfo_t* ai )
{
	ai->schema_a = schema;
}
static void bli_auxinfo_set_schema_b( pack_t schema, auxinfo_t* ai )
{
	ai->schema_b = schema;
}

static void bli_auxinfo_set_next_a( void* p, auxinfo_t* ai )
{
	ai->a_next = p;
}
static void bli_auxinfo_set_next_b( void* p, auxinfo_t* ai )
{
	ai->b_next = p;
}
static void bli_auxinfo_set_next_ab( void* ap, void* bp, auxinfo_t* ai )
{
	ai->a_next = ap;
	ai->b_next = bp;
}

static void bli_auxinfo_set_is_a( inc_t is, auxinfo_t* ai )
{
	ai->is_a = is;
}
static void bli_auxinfo_set_is_b( inc_t is, auxinfo_t* ai )
{
	ai->is_b = is;
}

#endif 

// end bli_auxinfo.h
// begin bli_param_map.h



// --- BLIS to BLAS/LAPACK mappings --------------------------------------------

void bli_param_map_blis_to_netlib_side( side_t side, char* blas_side );
void bli_param_map_blis_to_netlib_uplo( uplo_t uplo, char* blas_uplo );
void bli_param_map_blis_to_netlib_trans( trans_t trans, char* blas_trans );
void bli_param_map_blis_to_netlib_diag( diag_t diag, char* blas_diag );
void bli_param_map_blis_to_netlib_machval( machval_t machval, char* blas_machval );


// --- BLAS/LAPACK to BLIS mappings --------------------------------------------

void bli_param_map_netlib_to_blis_side( char side, side_t* blis_side );
void bli_param_map_netlib_to_blis_uplo( char uplo, uplo_t* blis_uplo );
void bli_param_map_netlib_to_blis_trans( char trans, trans_t* blis_trans );
void bli_param_map_netlib_to_blis_diag( char diag, diag_t* blis_diag );


// --- BLIS char to BLIS mappings ----------------------------------------------

void bli_param_map_char_to_blis_side( char side, side_t* blis_side );
void bli_param_map_char_to_blis_uplo( char uplo, uplo_t* blis_uplo );
void bli_param_map_char_to_blis_trans( char trans, trans_t* blis_trans );
void bli_param_map_char_to_blis_conj( char conj, conj_t* blis_conj );
void bli_param_map_char_to_blis_diag( char diag, diag_t* blis_diag );


// --- BLIS to BLIS char mappings ----------------------------------------------

void bli_param_map_blis_to_char_side( side_t blis_side, char* side );
void bli_param_map_blis_to_char_uplo( uplo_t blis_uplo, char* uplo );
void bli_param_map_blis_to_char_trans( trans_t blis_trans, char* trans );
void bli_param_map_blis_to_char_conj( conj_t blis_conj, char* conj );
void bli_param_map_blis_to_char_diag( diag_t blis_diag, char* diag );

// end bli_param_map.h
// begin bli_clock.h


double bli_clock( void );
double bli_clock_min_diff( double time_min, double time_start );
double bli_clock_helper( void );

// end bli_clock.h
// begin bli_check.h



err_t bli_check_error_code_helper( gint_t code, char* file, guint_t line );

err_t bli_check_valid_error_level( errlev_t level );

err_t bli_check_null_pointer( void* ptr );

err_t bli_check_valid_side( side_t side );
err_t bli_check_valid_uplo( uplo_t uplo );
err_t bli_check_valid_trans( trans_t trans );
err_t bli_check_valid_diag( diag_t diag );
err_t bli_check_nonunit_diag( obj_t* a );

err_t bli_check_valid_datatype( num_t dt );
err_t bli_check_object_valid_datatype( obj_t* a );
err_t bli_check_noninteger_datatype( num_t dt );
err_t bli_check_noninteger_object( obj_t* a );
err_t bli_check_nonconstant_datatype( num_t dt );
err_t bli_check_nonconstant_object( obj_t* a );
err_t bli_check_floating_datatype( num_t dt );
err_t bli_check_floating_object( obj_t* a );
err_t bli_check_real_datatype( num_t dt );
err_t bli_check_real_object( obj_t* a );
err_t bli_check_integer_datatype( num_t dt );
err_t bli_check_integer_object( obj_t* a );
err_t bli_check_consistent_datatypes( num_t dt_a, num_t dt_b );
err_t bli_check_consistent_object_datatypes( obj_t* a, obj_t* b );
err_t bli_check_datatype_real_proj_of( num_t dt_c, num_t dt_r );
err_t bli_check_object_real_proj_of( obj_t* c, obj_t* r );
err_t bli_check_real_valued_object( obj_t* a );

err_t bli_check_conformal_dims( obj_t* a, obj_t* b );
err_t bli_check_level3_dims( obj_t* a, obj_t* b, obj_t* c );
err_t bli_check_scalar_object( obj_t* a );
err_t bli_check_vector_object( obj_t* a );
err_t bli_check_matrix_object( obj_t* a );
err_t bli_check_equal_vector_lengths( obj_t* x, obj_t* y );
err_t bli_check_square_object( obj_t* a );
err_t bli_check_object_length_equals( obj_t* a, dim_t m );
err_t bli_check_object_width_equals( obj_t* a, dim_t n );
err_t bli_check_vector_dim_equals( obj_t* a, dim_t n );
err_t bli_check_object_diag_offset_equals( obj_t* a, doff_t offset );

err_t bli_check_matrix_strides( dim_t m, dim_t n, inc_t rs, inc_t cs, inc_t is );

err_t bli_check_general_object( obj_t* a );
err_t bli_check_hermitian_object( obj_t* a );
err_t bli_check_symmetric_object( obj_t* a );
err_t bli_check_triangular_object( obj_t* a );
err_t bli_check_object_struc( obj_t* a, struc_t struc );

err_t bli_check_upper_or_lower_object( obj_t* a );

err_t bli_check_valid_3x1_subpart( subpart_t part );
err_t bli_check_valid_1x3_subpart( subpart_t part );
err_t bli_check_valid_3x3_subpart( subpart_t part );

err_t bli_check_valid_cntl( void* cntl );

err_t bli_check_packm_schema_on_unpack( obj_t* a );
err_t bli_check_packv_schema_on_unpack( obj_t* a );

err_t bli_check_object_buffer( obj_t* a );

err_t bli_check_valid_packbuf( packbuf_t buf_type );
err_t bli_check_if_exhausted_pool( pool_t* pool );
err_t bli_check_sufficient_stack_buf_size( num_t dt, cntx_t* cntx );
err_t bli_check_alignment_is_power_of_two( size_t align_size );
err_t bli_check_alignment_is_mult_of_ptr_size( size_t align_size );

err_t bli_check_object_alias_of( obj_t* a, obj_t* b );

err_t bli_check_valid_arch_id( arch_t id );

err_t bli_check_valid_mc_mod_mult( blksz_t* mc, blksz_t* mr );
err_t bli_check_valid_nc_mod_mult( blksz_t* nc, blksz_t* nr );
err_t bli_check_valid_kc_mod_mult( blksz_t* kc, blksz_t* kr );

// end bli_check.h
// begin bli_error.h



void     bli_error_init( void );
void     bli_error_finalize( void );

void     bli_error_init_msgs( void );
void     bli_print_msg( char* str, char* file, guint_t line );
void     bli_abort( void );

errlev_t bli_error_checking_level( void );
void     bli_error_checking_level_set( errlev_t new_level );

bool_t   bli_error_checking_is_enabled( void );

char*    bli_error_string_for_code( gint_t code );


// end bli_error.h
// begin bli_f2c.h
// f2c.h  --  Standard Fortran to C header file
//  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."
//  - From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition)

#ifndef BLIS_F2C_H
#define BLIS_F2C_H

typedef f77_int bla_integer;
typedef f77_char bla_character;
//typedef char *address;
//typedef short int shortint;
typedef float bla_real;
typedef double bla_double;
typedef scomplex bla_scomplex;
typedef dcomplex bla_dcomplex;
typedef f77_int bla_logical;
//typedef short int shortlogical;
//typedef char logical1;
//typedef char integer1;
#ifdef INTEGER_STAR_8                // Adjust for integer*8.
typedef long long longint;           // system-dependent
typedef unsigned long long ulongint; // system-dependent
#define qbit_clear(a,b)	((a) & ~((ulongint)1 << (b)))
#define qbit_set(a,b)	((a) |  ((ulongint)1 << (b)))
#endif

#ifndef TRUE_
#define TRUE_ (1)
#endif

#ifndef FALSE_
#define FALSE_ (0)
#endif

// Extern is for use with -E
#ifndef Extern
#define Extern extern
#endif

// I/O stuff

#ifdef f2c_i2
// for -i2
typedef short flag;
typedef short ftnlen;
typedef short ftnint;
#else
typedef long int flag;
typedef long int ftnlen;
typedef long int ftnint;
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef f2c_abs
  #define f2c_abs(x) ((x) >= 0 ? (x) : -(x))
#endif
#ifndef f2c_dabs
  #define f2c_dabs(x) (doublereal)f2c_abs(x)
#endif
#ifndef f2c_min
  #define f2c_min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef f2c_max
  #define f2c_max(a,b) ((a) >= (b) ? (a) : (b))
#endif
#ifndef f2c_dmin
  #define f2c_dmin(a,b) (doublereal)f2c_min(a,b)
#endif
#ifndef f2c_dmax
  #define f2c_dmax(a,b) (doublereal)f2c_max(a,b)
#endif

#ifndef bit_test
  #define bit_test(a,b)  ((a) >> (b) & 1)
#endif

#ifndef bit_clear
  #define bit_clear(a,b) ((a) & ~((uinteger)1 << (b)))
#endif

#ifndef bit_set
  #define bit_set(a,b)   ((a) |  ((uinteger)1 << (b)))
#endif

// undef any lower-case symbols that your C compiler predefines, e.g.:

#ifndef Skip_f2c_Undefs
#undef cray
#undef gcos
#undef mc68010
#undef mc68020
#undef mips
#undef pdp11
#undef sgi
#undef sparc
#undef sun
#undef sun2
#undef sun3
#undef sun4
#undef u370
#undef u3b
#undef u3b2
#undef u3b5
#undef unix
#undef vax
#endif

#endif
// end bli_f2c.h
// begin bli_machval.h


// begin bli_lsame.h


bla_logical bli_lsame( bla_character* ca, bla_character* cb, ftnlen ca_len, ftnlen cb_len );
// end bli_lsame.h
// begin bli_slamch.h


bla_real bli_slamch( bla_character* cmach, ftnlen cmach_len );
// end bli_slamch.h
// begin bli_dlamch.h


bla_double bli_dlamch( bla_character* cmach, ftnlen cmach_len );
// end bli_dlamch.h

//
// Prototype object-based interface.
//
void bli_machval( machval_t mval,
                  obj_t*    v );


//
// Prototype BLAS-like interfaces.
//
#undef  GENTPROTR
#define GENTPROTR( ctype_v, ctype_vr, chv, chvr, opname ) \
\
void PASTEMAC(chv,opname) \
     ( \
       machval_t mval, \
       void*     v     \
     );

INSERT_GENTPROTR_BASIC0( machval )

// end bli_machval.h
// begin bli_getopt.h


typedef struct getopt_s
{
	char* optarg;
	int   optind;
	int   opterr;
	int   optopt;
} getopt_t;

void bli_getopt_init_state( int opterr, getopt_t* state );

int bli_getopt( int argc, char** const argv, const char* optstring, getopt_t* state );

// end bli_getopt.h
// begin bli_opid.h


static bool_t bli_opid_is_level3( opid_t opid )
{
	return ( BLIS_GEMM <= opid && opid <= BLIS_TRSM );
}

// end bli_opid.h
// begin bli_cntl.h






// -- Control tree prototypes --

cntl_t* bli_cntl_create_node
     (
       opid_t  family,
       bszid_t bszid,
       void*   var_func,
       void*   params,
       cntl_t* sub_node
     );

void bli_cntl_free_node
     (
       cntl_t* cntl
     );

void bli_cntl_clear_node
     (
       cntl_t* cntl
     );

// -----------------------------------------------------------------------------

void bli_cntl_free
     (
       cntl_t* cntl,
       thrinfo_t* thread
     );

void bli_cntl_free_w_thrinfo
     (
       cntl_t* cntl,
       thrinfo_t* thread
     );

void bli_cntl_free_wo_thrinfo
     (
       cntl_t* cntl
     );

cntl_t* bli_cntl_copy
     (
       cntl_t* cntl
     );

void bli_cntl_mark_family
     (
       opid_t  family,
       cntl_t* cntl
     );

// -----------------------------------------------------------------------------

// cntl_t query (fields only)

static opid_t bli_cntl_family( cntl_t* cntl )
{
	return cntl->family;
}

static bszid_t bli_cntl_bszid( cntl_t* cntl )
{
	return cntl->bszid;
}

static void* bli_cntl_var_func( cntl_t* cntl )
{
	return cntl->var_func;
}

static cntl_t* bli_cntl_sub_node( cntl_t* cntl )
{
	return cntl->sub_node;
}

static void* bli_cntl_params( cntl_t* cntl )
{
	return cntl->params;
}

static uint64_t bli_cntl_params_size( cntl_t* cntl )
{
	// The first 64 bytes is always the size of the params structure.
	return *( ( uint64_t* )(cntl->params) );
}

static mem_t* bli_cntl_pack_mem( cntl_t* cntl )
{
	return &(cntl->pack_mem);
}

// cntl_t query (complex)

static bool_t bli_cntl_is_leaf( cntl_t* cntl )
{
	return bli_cntl_sub_node( cntl ) == NULL;
}

static bool_t bli_cntl_does_part( cntl_t* cntl )
{
	return bli_cntl_bszid( cntl ) != BLIS_NO_PART;
}

// cntl_t modification

static void bli_cntl_set_family( opid_t family, cntl_t* cntl )
{
	cntl->family = family;
}

static void bli_cntl_set_bszid( bszid_t bszid, cntl_t* cntl )
{
	cntl->bszid = bszid;
}

static void bli_cntl_set_var_func( void* var_func, cntl_t* cntl )
{
	cntl->var_func = var_func;
}

static void bli_cntl_set_sub_node( cntl_t* sub_node, cntl_t* cntl )
{
	cntl->sub_node = sub_node;
}

static void bli_cntl_set_params( void* params, cntl_t* cntl )
{
	cntl->params = params;
}

static void bli_cntl_set_pack_mem( mem_t* pack_mem, cntl_t* cntl )
{
	cntl->pack_mem = *pack_mem;
}

// end bli_cntl.h
// begin bli_info.h



// -- General library information ----------------------------------------------

char* bli_info_get_version_str( void );
char* bli_info_get_int_type_size_str( void );


// -- General configuration-related --------------------------------------------

gint_t bli_info_get_int_type_size( void );
gint_t bli_info_get_num_fp_types( void );
gint_t bli_info_get_max_type_size( void );
gint_t bli_info_get_page_size( void );
gint_t bli_info_get_simd_num_registers( void );
gint_t bli_info_get_simd_size( void );
gint_t bli_info_get_simd_align_size( void );
gint_t bli_info_get_stack_buf_max_size( void );
gint_t bli_info_get_stack_buf_align_size( void );
gint_t bli_info_get_heap_addr_align_size( void );
gint_t bli_info_get_heap_stride_align_size( void );
gint_t bli_info_get_pool_addr_align_size( void );
gint_t bli_info_get_enable_stay_auto_init( void );
gint_t bli_info_get_enable_blas2blis( void );
gint_t bli_info_get_enable_cblas( void );
gint_t bli_info_get_blas2blis_int_type_size( void );
gint_t bli_info_get_enable_packbuf_pools( void );


// -- Kernel implementation-related --------------------------------------------


// -- Level-3 kernel definitions --

char* bli_info_get_gemm_ukr_impl_string( ind_t method, num_t dt );
char* bli_info_get_gemmtrsm_l_ukr_impl_string( ind_t method, num_t dt );
char* bli_info_get_gemmtrsm_u_ukr_impl_string( ind_t method, num_t dt );
char* bli_info_get_trsm_l_ukr_impl_string( ind_t method, num_t dt );
char* bli_info_get_trsm_u_ukr_impl_string( ind_t method, num_t dt );


// -- BLIS implementation query (level-3) --------------------------------------

char* bli_info_get_gemm_impl_string( num_t dt );
char* bli_info_get_hemm_impl_string( num_t dt );
char* bli_info_get_herk_impl_string( num_t dt );
char* bli_info_get_her2k_impl_string( num_t dt );
char* bli_info_get_symm_impl_string( num_t dt );
char* bli_info_get_syrk_impl_string( num_t dt );
char* bli_info_get_syr2k_impl_string( num_t dt );
char* bli_info_get_trmm_impl_string( num_t dt );
char* bli_info_get_trmm3_impl_string( num_t dt );
char* bli_info_get_trsm_impl_string( num_t dt );

// end bli_info.h
// begin bli_arch.h


#ifndef BLIS_ARCH_H
#define BLIS_ARCH_H

arch_t  bli_arch_query_id( void );
char*   bli_arch_string( arch_t id );


#endif

// end bli_arch.h
// begin bli_cpuid.h


#if 0
  // Used only during standalone testing of ARM support.
  #define FALSE 0
  #define TRUE  1
  typedef enum
  {
	BLIS_ARCH_CORTEXA57 = 10,
	BLIS_ARCH_CORTEXA15 = 11,
	BLIS_ARCH_CORTEXA9  = 12,
	BLIS_ARCH_GENERIC   = 13
  } arch_t;
  typedef uint64_t bool_t;
  #define bli_abort abort
#endif

#ifndef BLIS_CPUID_H
#define BLIS_CPUID_H

arch_t   bli_cpuid_query_id( void );

// Intel
bool_t   bli_cpuid_is_skx( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_knl( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_haswell( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_sandybridge( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_penryn( uint32_t family, uint32_t model, uint32_t features );

// AMD
bool_t   bli_cpuid_is_zen( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_excavator( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_steamroller( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_piledriver( uint32_t family, uint32_t model, uint32_t features );
bool_t   bli_cpuid_is_bulldozer( uint32_t family, uint32_t model, uint32_t features );

// ARM
bool_t   bli_cpuid_is_cortexa57( uint32_t model, uint32_t part, uint32_t features );
bool_t   bli_cpuid_is_cortexa15( uint32_t model, uint32_t part, uint32_t features );
bool_t   bli_cpuid_is_cortexa9( uint32_t model, uint32_t part, uint32_t features );

uint32_t bli_cpuid_query( uint32_t* family, uint32_t* model, uint32_t* features );

// -----------------------------------------------------------------------------

//
// This section of the file was based off of cpuid.hpp from TBLIS [1].
//
// [1] https://github.com/devinamatthews/tblis
//



static bool_t bli_cpuid_has_features( uint32_t have, uint32_t want )
{
    return ( have & want ) == want;
}

// -----------------------------------------------------------------------------

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)

#include "cpuid.h" // skipped

void get_cpu_name( char *cpu_name );
int  vpu_count( void );


enum
{
	VENDOR_INTEL = 0,
	VENDOR_AMD,
	VENDOR_UNKNOWN
};
enum
{
	FEATURE_SSE3     = 0x0001,
	FEATURE_SSSE3    = 0x0002,
	FEATURE_SSE41    = 0x0004,
	FEATURE_SSE42    = 0x0008,
	FEATURE_AVX      = 0x0010,
	FEATURE_AVX2     = 0x0020,
	FEATURE_FMA3     = 0x0040,
	FEATURE_FMA4     = 0x0080,
	FEATURE_AVX512F  = 0x0100,
	FEATURE_AVX512DQ = 0x0200,
	FEATURE_AVX512PF = 0x0400,
	FEATURE_AVX512ER = 0x0800,
	FEATURE_AVX512CD = 0x1000,
	FEATURE_AVX512BW = 0x2000,
	FEATURE_AVX512VL = 0x4000
};

#elif defined(__aarch64__) || defined(__arm__) || defined(_M_ARM)

enum
{
	VENDOR_ARM = 0,
	VENDOR_UNKNOWN
};
enum
{
	MODEL_ARMV7 = 0,
	MODEL_ARMV8,
	MODEL_UNKNOWN
};
enum
{
	FEATURE_NEON = 0x1
};

#endif



#endif

// end bli_cpuid.h
// begin bli_string.h


void bli_string_mkupper( char* s );
// end bli_string.h
// begin bli_setgetij.h


err_t bli_setijm
     (
       double  ar,
       double  ai,
       dim_t   i,
       dim_t   j,
       obj_t*  b
     );

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       double         ar, \
       double         ai, \
       dim_t          i, \
       dim_t          j, \
       void* restrict b, inc_t rs, inc_t cs  \
     );

INSERT_GENTPROT_BASIC0( setijm )

// -----------------------------------------------------------------------------

err_t bli_getijm
      (
        dim_t   i,
        dim_t   j,
        obj_t*  b,
        double* ar,
        double* ai
      );

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t          i, \
       dim_t          j, \
       void* restrict b, inc_t rs, inc_t cs, \
       double*        ar, \
       double*        ai  \
     );

INSERT_GENTPROT_BASIC0( getijm )

// end bli_setgetij.h


// -- Level-0 operations --

// begin bli_l0.h


// begin bli_l0_check.h



//
// Prototype object-based check functions.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  chi, \
       obj_t*  psi  \
     );

GENTPROT( addsc )
GENTPROT( copysc )
GENTPROT( divsc )
GENTPROT( mulsc )
GENTPROT( sqrtsc )
GENTPROT( subsc )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  chi  \
     );

GENTPROT( invertsc )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  chi, \
       obj_t*  absq  \
     );

GENTPROT( absqsc )
GENTPROT( normfsc )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  chi, \
       double* zeta_r, \
       double* zeta_i  \
     );

GENTPROT( getsc )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       double  zeta_r, \
       double  zeta_i, \
       obj_t*  chi  \
     );

GENTPROT( setsc )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  chi, \
       obj_t*  zeta_r, \
       obj_t*  zeta_i  \
     );

GENTPROT( unzipsc )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  zeta_r, \
       obj_t*  zeta_i, \
       obj_t*  chi  \
     );

GENTPROT( zipsc )


// -----------------------------------------------------------------------------

void bli_l0_xsc_check
     (
       obj_t*  chi
     );

void bli_l0_xxsc_check
     (
       obj_t*  chi,
       obj_t*  psi 
     );

void bli_l0_xx2sc_check
     (
       obj_t*  chi,
       obj_t*  norm 
     );
// end bli_l0_check.h

// begin bli_l0_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  chi, \
       obj_t*  absq  \
     );

GENPROT( absqsc )
GENPROT( normfsc )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  chi, \
       obj_t*  psi  \
     );

GENPROT( addsc )
GENPROT( divsc )
GENPROT( mulsc )
GENPROT( sqrtsc )
GENPROT( subsc )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  chi  \
     );

GENPROT( invertsc )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  chi, \
       double* zeta_r, \
       double* zeta_i  \
     );

GENPROT( getsc )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       double  zeta_r, \
       double  zeta_i, \
       obj_t*  chi  \
     );

GENPROT( setsc )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  chi, \
       obj_t*  zeta_r, \
       obj_t*  zeta_i  \
     );

GENPROT( unzipsc )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  zeta_r, \
       obj_t*  zeta_i, \
       obj_t*  chi  \
     );

GENPROT( zipsc )







// end bli_l0_oapi.h
// begin bli_l0_tapi.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t  conjchi, \
       ctype*  chi, \
       ctype*  psi  \
     );

INSERT_GENTPROT_BASIC0( addsc )
INSERT_GENTPROT_BASIC0( divsc )
INSERT_GENTPROT_BASIC0( mulsc )
INSERT_GENTPROT_BASIC0( subsc )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t  conjchi, \
       ctype*  chi  \
     );

INSERT_GENTPROT_BASIC0( invertsc )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       ctype*   chi, \
       ctype_r* absq  \
     );

INSERT_GENTPROTR_BASIC0( absqsc )
INSERT_GENTPROTR_BASIC0( normfsc )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       ctype*  chi, \
       ctype*  psi  \
     );

INSERT_GENTPROT_BASIC0( sqrtsc )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       ctype*  chi, \
       double* zeta_r, \
       double* zeta_i  \
     );

INSERT_GENTPROT_BASIC0( getsc )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       double  zeta_r, \
       double  zeta_i, \
       ctype*  chi  \
     );

INSERT_GENTPROT_BASIC0( setsc )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       ctype*   chi, \
       ctype_r* zeta_r, \
       ctype_r* zeta_i  \
     );

INSERT_GENTPROTR_BASIC0( unzipsc )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       ctype_r* zeta_r, \
       ctype_r* zeta_i, \
       ctype*   chi  \
     );

INSERT_GENTPROTR_BASIC0( zipsc )

// -----------------------------------------------------------------------------

void bli_igetsc
     (
       dim_t*  chi,
       double* zeta_r,
       double* zeta_i
     );

void bli_isetsc
     (
       double  zeta_r,
       double  zeta_i,
       dim_t*  chi
     );

// end bli_l0_tapi.h

// copysc
// begin bli_copysc.h



//
// Prototype object-based interfaces.
//

#undef  GENFRONT
#define GENFRONT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  chi, \
       obj_t*  psi  \
     );
GENFRONT( copysc )


//
// Define BLAS-like interfaces with heterogeneous-typed operands.
//

#undef  GENTPROT2
#define GENTPROT2( ctype_x, ctype_y, chx, chy, varname ) \
\
void PASTEMAC2(chx,chy,varname) \
     ( \
       conj_t conjchi, \
       void*  chi, \
       void*  psi \
     );

INSERT_GENTPROT2_BASIC0( copysc )
INSERT_GENTPROT2_MIX_D0( copysc )
INSERT_GENTPROT2_MIX_P0( copysc )

// end bli_copysc.h
// end bli_l0.h


// -- Level-1v operations --

// begin bli_l1v.h


// begin bli_l1v_check.h



//
// Prototype object-based check functions.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
     );

GENTPROT( addv )
GENTPROT( copyv )
GENTPROT( subv )
GENTPROT( swapv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  index  \
     );

GENTPROT( amaxv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
     );

GENTPROT( axpbyv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
     );

GENTPROT( axpyv )
GENTPROT( scal2v )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  rho  \
     );

GENTPROT( dotv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  beta, \
       obj_t*  rho  \
     );

GENTPROT( dotxv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x  \
     );

GENTPROT( invertv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
     );

GENTPROT( scalv )
GENTPROT( setv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
     );

GENTPROT( xpbyv )



// -----------------------------------------------------------------------------

void bli_l1v_xy_check
     (
       obj_t*  x,
       obj_t*  y 
     );

void bli_l1v_axy_check
     (
       obj_t*  alpha,
       obj_t*  x,
       obj_t*  y 
     );

void bli_l1v_xby_check
     (
       obj_t*  x,
       obj_t*  beta,
       obj_t*  y
     );

void bli_l1v_axby_check
     (
       obj_t*  alpha,
       obj_t*  x,
       obj_t*  beta,
       obj_t*  y
     );

void bli_l1v_dot_check
     (
       obj_t*  alpha,
       obj_t*  x,
       obj_t*  y,
       obj_t*  beta,
       obj_t*  rho 
     );

void bli_l1v_x_check
     (
       obj_t*  x 
     );

void bli_l1v_ax_check
     (
       obj_t*  alpha,
       obj_t*  x 
     );

void bli_l1v_xi_check
     (
       obj_t*  x,
       obj_t*  index
     );

// end bli_l1v_check.h

// begin bli_l1v_ft.h


#ifndef BLIS_L1V_FT_H
#define BLIS_L1V_FT_H


//
// -- Level-1v function types --------------------------------------------------
//

// addv, copyv, subv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjx, \
       dim_t           n, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( addv )
INSERT_GENTDEF( copyv )
INSERT_GENTDEF( subv )

// amaxv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       dim_t           n, \
       ctype* restrict x, inc_t incx, \
       dim_t* restrict index, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( amaxv )

// axpbyv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjx, \
       dim_t           n, \
       ctype* restrict alpha, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict beta, \
       ctype* restrict y, inc_t incy, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( axpbyv )

// axpyv, scal2v

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjx, \
       dim_t           n, \
       ctype* restrict alpha, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( axpyv )
INSERT_GENTDEF( scal2v )

// dotv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjx, \
       conj_t          conjy, \
       dim_t           n, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       ctype* restrict rho, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( dotv )

// dotxv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjx, \
       conj_t          conjy, \
       dim_t           n, \
       ctype* restrict alpha, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       ctype* restrict beta, \
       ctype* restrict rho, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( dotxv )

// invertv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       dim_t           n, \
       ctype* restrict x, inc_t incx, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( invertv )

// scalv, setv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjalpha, \
       dim_t           n, \
       ctype* restrict alpha, \
       ctype* restrict x, inc_t incx, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( scalv )
INSERT_GENTDEF( setv )

// swapv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       dim_t           n, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( swapv )

// xpybv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjx, \
       dim_t           n, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict beta, \
       ctype* restrict y, inc_t incy, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( xpbyv )


#endif

// end bli_l1v_ft.h

// Prototype object APIs with and without contexts.
// begin bli_oapi_w_cntx.h



// This file defines macros used to allow the _oapi.c files to
// produce object APIs that contain context parameters.

// Define the macro to add a suffix to the object function names.
// We use "ex" for "expert".
#undef  EX_SUF
#define EX_SUF _ex

// Define the macro to add cntx_t* arguments to function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM   ,cntx_t* cntx

// Define the macro to omit the cntx_t declaration block, since it is
// not needed when cntx_t's are passed in through the API.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL

// end bli_oapi_w_cntx.h
// begin bli_l1v_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( addv )
GENTPROT( copyv )
GENTPROT( subv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  index  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( amaxv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( axpbyv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( axpyv )
GENTPROT( scal2v )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  rho  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( dotv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  beta, \
       obj_t*  rho  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( dotxv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( invertv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( scalv )
GENTPROT( setv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( swapv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( xpbyv )

// end bli_l1v_oapi.h
// begin bli_oapi_wo_cntx.h


// This file defines macros used to allow the _oapi.c files to
// produce object APIs that omit context parameters.

// Define the macro to remove the function name suffix.
#undef  EX_SUF
#define EX_SUF

// Define the macro to omit cntx_t* arguments from function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM

// Define the macro to declare a local cntx_t pointer that is initialized
// to NULL.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL   cntx_t* cntx = NULL;

// end bli_oapi_wo_cntx.h
// begin bli_l1v_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( addv )
GENTPROT( copyv )
GENTPROT( subv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  index  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( amaxv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( axpbyv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( axpyv )
GENTPROT( scal2v )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  rho  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( dotv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  beta, \
       obj_t*  rho  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( dotxv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( invertv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( scalv )
GENTPROT( setv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( swapv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( xpbyv )

// end bli_l1v_oapi.h

// begin bli_l1v_tapi.h



//
// Generate prototypes for level-1v operations.
//

#undef  addv_ker_name
#define addv_ker_name       addv

#undef  amaxv_ker_name
#define amaxv_ker_name      amaxv

#undef  axpbyv_ker_name
#define axpbyv_ker_name     axpbyv

#undef  axpyv_ker_name
#define axpyv_ker_name      axpyv

#undef  copyv_ker_name
#define copyv_ker_name      copyv

#undef  dotv_ker_name
#define dotv_ker_name       dotv

#undef  dotxv_ker_name
#define dotxv_ker_name      dotxv

#undef  invertv_ker_name
#define invertv_ker_name    invertv

#undef  scalv_ker_name
#define scalv_ker_name      scalv

#undef  scal2v_ker_name
#define scal2v_ker_name     scal2v

#undef  setv_ker_name
#define setv_ker_name       setv

#undef  subv_ker_name
#define subv_ker_name       subv

#undef  swapv_ker_name
#define swapv_ker_name      swapv

#undef  xpbyv_ker_name
#define xpbyv_ker_name      xpbyv


// Include the level-1v kernel API template.

// begin bli_l1v_ker.h



//
// Define template prototypes for level-1v kernels.
//

// Note: Instead of defining function prototype macro templates and then
// instantiating those macros to define the individual function prototypes,
// we simply alias the official operations' prototypes as defined in
// bli_l1v_ker_prot.h.

#undef  GENTPROT
#define GENTPROT ADDV_KER_PROT

INSERT_GENTPROT_BASIC0( addv_ker_name )


#undef  GENTPROT
#define GENTPROT AMAXV_KER_PROT

INSERT_GENTPROT_BASIC0( amaxv_ker_name )


#undef  GENTPROT
#define GENTPROT AXPBYV_KER_PROT

INSERT_GENTPROT_BASIC0( axpbyv_ker_name )


#undef  GENTPROT
#define GENTPROT AXPYV_KER_PROT

INSERT_GENTPROT_BASIC0( axpyv_ker_name )


#undef  GENTPROT
#define GENTPROT COPYV_KER_PROT

INSERT_GENTPROT_BASIC0( copyv_ker_name )


#undef  GENTPROT
#define GENTPROT DOTV_KER_PROT

INSERT_GENTPROT_BASIC0( dotv_ker_name )


#undef  GENTPROT
#define GENTPROT DOTXV_KER_PROT

INSERT_GENTPROT_BASIC0( dotxv_ker_name )


#undef  GENTPROT
#define GENTPROT INVERTV_KER_PROT

INSERT_GENTPROT_BASIC0( invertv_ker_name )


#undef  GENTPROT
#define GENTPROT SCALV_KER_PROT

INSERT_GENTPROT_BASIC0( scalv_ker_name )


#undef  GENTPROT
#define GENTPROT SCAL2V_KER_PROT

INSERT_GENTPROT_BASIC0( scal2v_ker_name )


#undef  GENTPROT
#define GENTPROT SETV_KER_PROT

INSERT_GENTPROT_BASIC0( setv_ker_name )


#undef  GENTPROT
#define GENTPROT SUBV_KER_PROT

INSERT_GENTPROT_BASIC0( subv_ker_name )


#undef  GENTPROT
#define GENTPROT SWAPV_KER_PROT

INSERT_GENTPROT_BASIC0( swapv_ker_name )


#undef  GENTPROT
#define GENTPROT XPBYV_KER_PROT

INSERT_GENTPROT_BASIC0( xpbyv_ker_name )

// end bli_l1v_ker.h

// end bli_l1v_tapi.h

// Pack-related
// NOTE: packv and unpackv are temporarily disabled.
//#include "bli_packv.h"
//#include "bli_unpackv.h"

// Other
// NOTE: scalv control tree code is temporarily disabled.
//#include "bli_scalv_cntl.h"
//#include "bli_scalv_int.h"

// Reference kernel headers
//#include "bli_l1v_ref.h"

// end bli_l1v.h


// -- Level-1d operations --

// begin bli_l1d.h


// begin bli_l1d_check.h



//
// Prototype object-based check functions.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
    );

GENTPROT( addd )
GENTPROT( copyd )
GENTPROT( subd )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
    );

GENTPROT( axpyd )
GENTPROT( scal2d )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x  \
    );

GENTPROT( invertd )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
    );

GENTPROT( scald )
GENTPROT( setd )
GENTPROT( setid )


// -----------------------------------------------------------------------------

void bli_l1d_xy_check
     (
       obj_t*  x,
       obj_t*  y 
     );

void bli_l1d_axy_check
     (
       obj_t*  alpha,
       obj_t*  x,
       obj_t*  y 
     );

void bli_l1d_x_check
     (
       obj_t*  x 
     );

void bli_l1d_ax_check
     (
       obj_t*  alpha,
       obj_t*  x 
     );

// end bli_l1d_check.h

// Prototype object APIs with and without contexts.
// begin bli_oapi_w_cntx.h



// This file defines macros used to allow the _oapi.c files to
// produce object APIs that contain context parameters.

// Define the macro to add a suffix to the object function names.
// We use "ex" for "expert".
#undef  EX_SUF
#define EX_SUF _ex

// Define the macro to add cntx_t* arguments to function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM   ,cntx_t* cntx

// Define the macro to omit the cntx_t declaration block, since it is
// not needed when cntx_t's are passed in through the API.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL

// end bli_oapi_w_cntx.h
// begin bli_l1d_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( addd )
GENTPROT( copyd )
GENTPROT( subd )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( axpyd )
GENTPROT( scal2d )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( invertd )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( scald )
GENTPROT( setd )
GENTPROT( setid )

// end bli_l1d_oapi.h
// begin bli_oapi_wo_cntx.h


// This file defines macros used to allow the _oapi.c files to
// produce object APIs that omit context parameters.

// Define the macro to remove the function name suffix.
#undef  EX_SUF
#define EX_SUF

// Define the macro to omit cntx_t* arguments from function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM

// Define the macro to declare a local cntx_t pointer that is initialized
// to NULL.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL   cntx_t* cntx = NULL;

// end bli_oapi_wo_cntx.h
// begin bli_l1d_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( addd )
GENTPROT( copyd )
GENTPROT( subd )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( axpyd )
GENTPROT( scal2d )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( invertd )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     ); 

GENTPROT( scald )
GENTPROT( setd )
GENTPROT( setid )

// end bli_l1d_oapi.h

// begin bli_l1d_tapi.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t  diagoffx, \
       diag_t  diagx, \
       trans_t transx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       ctype*  y, inc_t rs_y, inc_t cs_y, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( addd )
INSERT_GENTPROT_BASIC0( copyd )
INSERT_GENTPROT_BASIC0( subd )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t  diagoffx, \
       diag_t  diagx, \
       trans_t transx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       ctype*  y, inc_t rs_y, inc_t cs_y, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( axpyd )
INSERT_GENTPROT_BASIC0( scal2d )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t  diagoffx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( invertd )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t  conjalpha, \
       doff_t  diagoffx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( scald )
INSERT_GENTPROT_BASIC0( setd )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t   diagoffx, \
       dim_t    m, \
       dim_t    n, \
       ctype_r* alpha, \
       ctype*   x, inc_t rs_x, inc_t cs_x, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( setid )

// end bli_l1d_tapi.h

// end bli_l1d.h


// -- Level-1f operations --

// begin bli_l1f.h


// begin bli_l1f_check.h



//
// Prototype object-based check functions.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alphax, \
       obj_t*  alphay, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  z  \
    );

GENTPROT( axpy2v )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  y  \
    );

GENTPROT( axpyf )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  xt, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  rho, \
       obj_t*  z  \
    );

GENTPROT( dotaxpyv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  at, \
       obj_t*  a, \
       obj_t*  w, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y, \
       obj_t*  z  \
    );

GENTPROT( dotxaxpyf )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
    );

GENTPROT( dotxf )

// end bli_l1f_check.h

// begin bli_l1f_ft.h


#ifndef BLIS_L1F_FT_H
#define BLIS_L1F_FT_H


//
// -- Level-1f function types --------------------------------------------------
//

// axpy2v

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjx, \
       conj_t          conjy, \
       dim_t           n, \
       ctype* restrict alpha1, \
       ctype* restrict alpha2, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       ctype* restrict z, inc_t incz, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( axpy2v )

// axpyf

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conja, \
       conj_t          conjx, \
       dim_t           m, \
       dim_t           b_n, \
       ctype* restrict alpha, \
       ctype* restrict a, inc_t inca, inc_t lda, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( axpyf )

// dotaxpyv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjxt, \
       conj_t          conjx, \
       conj_t          conjy, \
       dim_t           m, \
       ctype* restrict alpha, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict y, inc_t incy, \
       ctype* restrict rho, \
       ctype* restrict z, inc_t incz, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( dotaxpyv )

// dotxf

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjat, \
       conj_t          conjx, \
       dim_t           m, \
       dim_t           b_n, \
       ctype* restrict alpha, \
       ctype* restrict a, inc_t inca, inc_t lda, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict beta, \
       ctype* restrict y, inc_t incy, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( dotxf )

// dotxaxpyf

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t          conjat, \
       conj_t          conja, \
       conj_t          conjw, \
       conj_t          conjx, \
       dim_t           m, \
       dim_t           b_n, \
       ctype* restrict alpha, \
       ctype* restrict a, inc_t inca, inc_t lda, \
       ctype* restrict w, inc_t incw, \
       ctype* restrict x, inc_t incx, \
       ctype* restrict beta, \
       ctype* restrict y, inc_t incy, \
       ctype* restrict z, inc_t incz, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( dotxaxpyf )



#endif

// end bli_l1f_ft.h

// Prototype object APIs with and without contexts.
// begin bli_oapi_w_cntx.h



// This file defines macros used to allow the _oapi.c files to
// produce object APIs that contain context parameters.

// Define the macro to add a suffix to the object function names.
// We use "ex" for "expert".
#undef  EX_SUF
#define EX_SUF _ex

// Define the macro to add cntx_t* arguments to function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM   ,cntx_t* cntx

// Define the macro to omit the cntx_t declaration block, since it is
// not needed when cntx_t's are passed in through the API.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL

// end bli_oapi_w_cntx.h
// begin bli_l1f_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alphax, \
       obj_t*  alphay, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  z  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( axpy2v )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( axpyf )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  xt, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  rho, \
       obj_t*  z  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( dotaxpyv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  at, \
       obj_t*  a, \
       obj_t*  w, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y, \
       obj_t*  z  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( dotxaxpyf )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( dotxf )

// end bli_l1f_oapi.h
// begin bli_oapi_wo_cntx.h


// This file defines macros used to allow the _oapi.c files to
// produce object APIs that omit context parameters.

// Define the macro to remove the function name suffix.
#undef  EX_SUF
#define EX_SUF

// Define the macro to omit cntx_t* arguments from function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM

// Define the macro to declare a local cntx_t pointer that is initialized
// to NULL.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL   cntx_t* cntx = NULL;

// end bli_oapi_wo_cntx.h
// begin bli_l1f_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alphax, \
       obj_t*  alphay, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  z  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( axpy2v )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( axpyf )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  xt, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  rho, \
       obj_t*  z  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( dotaxpyv )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  at, \
       obj_t*  a, \
       obj_t*  w, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y, \
       obj_t*  z  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( dotxaxpyf )


#undef  GENTPROT
#define GENTPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENTPROT( dotxf )

// end bli_l1f_oapi.h

// begin bli_l1f_tapi.h



//
// Generate prototypes for level-1f operations.
//

#undef  axpy2v_ker_name
#define axpy2v_ker_name      axpy2v

#undef  dotaxpyv_ker_name
#define dotaxpyv_ker_name    dotaxpyv

#undef  axpyf_ker_name
#define axpyf_ker_name       axpyf

#undef  dotxf_ker_name
#define dotxf_ker_name       dotxf

#undef  dotxaxpyf_ker_name
#define dotxaxpyf_ker_name   dotxaxpyf

// Include the level-1f kernel API template.

// begin bli_l1f_ker.h



//
// Define template prototypes for level-1f kernels.
//

// Note: Instead of defining function prototype macro templates and then
// instantiating those macros to define the individual function prototypes,
// we simply alias the official operations' prototypes as defined in
// bli_l1f_ker_prot.h.

#undef  GENTPROT
#define GENTPROT AXPY2V_KER_PROT

INSERT_GENTPROT_BASIC0( axpy2v_ker_name )


#undef  GENTPROT
#define GENTPROT AXPYF_KER_PROT

INSERT_GENTPROT_BASIC0( axpyf_ker_name )


#undef  GENTPROT
#define GENTPROT DOTAXPYV_KER_PROT

INSERT_GENTPROT_BASIC0( dotaxpyv_ker_name )


#undef  GENTPROT
#define GENTPROT DOTXAXPYF_KER_PROT

INSERT_GENTPROT_BASIC0( dotxaxpyf_ker_name )


#undef  GENTPROT
#define GENTPROT DOTXF_KER_PROT

INSERT_GENTPROT_BASIC0( dotxf_ker_name )

// end bli_l1f_ker.h

// end bli_l1f_tapi.h

// Reference kernel headers
//#include "bli_l1f_ref.h"

// end bli_l1f.h


// -- Level-1m operations --

// begin bli_l1m.h


// begin bli_l1m_check.h



//
// Prototype object-based check functions.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
    );

GENPROT( addm )
GENPROT( copym )
GENPROT( subm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
    );

GENPROT( axpym )
GENPROT( scal2m )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
    );

GENPROT( scalm )
GENPROT( setm )


// -----------------------------------------------------------------------------

void bli_l1m_xy_check
     (
       obj_t*  x,
       obj_t*  y 
     );

void bli_l1m_axy_check
     (
       obj_t*  alpha,
       obj_t*  x,
       obj_t*  y 
     );

void bli_l1m_ax_check
     (
       obj_t*  alpha,
       obj_t*  x 
     );

// end bli_l1m_check.h

// begin bli_l1m_ft.h


#ifndef BLIS_L1M_FT_H
#define BLIS_L1M_FT_H


//
// -- Level-1m function types --------------------------------------------------
//

// packm

// NOTE: This is the function type for the structure-aware "kernel".

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, \
       cntx_t*         cntx  \
     );

INSERT_GENTDEF( packm )


// NOTE: the following macros generate packm kernel function type definitions
// that are "ctyped" and void-typed, for each of the floating-point datatypes.
// However, we will only make use of the void-typed definitions because the
// functions such as bli_?packm_cxk() (currently) use arrays of function
// pointers to store and access the function pointers for various unrolling
// (register blocksize) values, and therefore they must all be of the same
// type (hence the use of void* for kappa, a, and p).

// packm_ker

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t           conja, \
       dim_t            n, \
       ctype*  restrict kappa, \
       ctype*  restrict a, inc_t inca, inc_t lda, \
       ctype*  restrict p,             inc_t ldp, \
       cntx_t* restrict cntx  \
     );

INSERT_GENTDEF( packm_cxk_ker )

// unpackm_ker

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t           conjp, \
       dim_t            n, \
       ctype*  restrict kappa, \
       ctype*  restrict p,             inc_t ldp, \
       ctype*  restrict a, inc_t inca, inc_t lda, \
       cntx_t* restrict cntx  \
     );

INSERT_GENTDEF( unpackm_cxk_ker )

// packm_3mis_ker
// packm_4mi_ker

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t           conja, \
       dim_t            n, \
       ctype*  restrict kappa, \
       ctype*  restrict a, inc_t inca, inc_t lda, \
       ctype*  restrict p, inc_t is_p, inc_t ldp, \
       cntx_t* restrict cntx  \
     );

INSERT_GENTDEF( packm_cxk_3mis_ker )
INSERT_GENTDEF( packm_cxk_4mi_ker )


// packm_rih_ker
// packm_1er_ker

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t           conja, \
       pack_t           schema, \
       dim_t            n, \
       ctype*  restrict kappa, \
       ctype*  restrict a, inc_t inca, inc_t lda, \
       ctype*  restrict p,             inc_t ldp, \
       cntx_t* restrict cntx  \
     );

INSERT_GENTDEF( packm_cxk_rih_ker )
INSERT_GENTDEF( packm_cxk_1er_ker )





#endif

// end bli_l1m_ft.h
// begin bli_l1m_voft.h


#ifndef BLIS_L1M_VAR_OFT_H
#define BLIS_L1M_VAR_OFT_H


//
// -- Level-3 variant function types -------------------------------------------
//

#undef  GENTDEF
#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_voft)) \
( \
  obj_t*  a, \
  obj_t*  p, \
  cntx_t* cntx, \
  cntl_t* cntl, \
  thrinfo_t* thread  \
);

GENTDEF( packm )


#undef  GENTDEF
#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_voft)) \
( \
  obj_t*  p, \
  obj_t*  a, \
  cntx_t* cntx, \
  cntl_t* cntl, \
  thrinfo_t* thread  \
);

GENTDEF( unpackm )



#endif

// end bli_l1m_voft.h

// Prototype object APIs with and without contexts.
// begin bli_oapi_w_cntx.h



// This file defines macros used to allow the _oapi.c files to
// produce object APIs that contain context parameters.

// Define the macro to add a suffix to the object function names.
// We use "ex" for "expert".
#undef  EX_SUF
#define EX_SUF _ex

// Define the macro to add cntx_t* arguments to function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM   ,cntx_t* cntx

// Define the macro to omit the cntx_t declaration block, since it is
// not needed when cntx_t's are passed in through the API.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL

// end bli_oapi_w_cntx.h
// begin bli_l1m_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( addm )
GENPROT( copym )
GENPROT( subm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( axpym )
GENPROT( scal2m )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( scalm )
GENPROT( setm )

// end bli_l1m_oapi.h
// begin bli_oapi_wo_cntx.h


// This file defines macros used to allow the _oapi.c files to
// produce object APIs that omit context parameters.

// Define the macro to remove the function name suffix.
#undef  EX_SUF
#define EX_SUF

// Define the macro to omit cntx_t* arguments from function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM

// Define the macro to declare a local cntx_t pointer that is initialized
// to NULL.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL   cntx_t* cntx = NULL;

// end bli_oapi_wo_cntx.h
// begin bli_l1m_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( addm )
GENPROT( copym )
GENPROT( subm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( axpym )
GENPROT( scal2m )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( scalm )
GENPROT( setm )

// end bli_l1m_oapi.h

// begin bli_l1m_tapi.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t  diagoffx, \
       diag_t  diagx, \
       uplo_t  uplox, \
       trans_t transx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       ctype*  y, inc_t rs_y, inc_t cs_y, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( addm )
INSERT_GENTPROT_BASIC0( copym )
INSERT_GENTPROT_BASIC0( subm )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t  diagoffx, \
       diag_t  diagx, \
       uplo_t  uplox, \
       trans_t transx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       ctype*  y, inc_t rs_y, inc_t cs_y, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( axpym )
INSERT_GENTPROT_BASIC0( scal2m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t  conjalpha, \
       doff_t  diagoffx, \
       diag_t  diagx, \
       uplo_t  uplox, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( scalm )
INSERT_GENTPROT_BASIC0( setm )

// end bli_l1m_tapi.h
// begin bli_l1m_unb_var1.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC2(ch,opname,_unb_var1) \
     ( \
       doff_t  diagoffx, \
       diag_t  diagx, \
       uplo_t  uplox, \
       trans_t transx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       ctype*  y, inc_t rs_y, inc_t cs_y, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( addm )
INSERT_GENTPROT_BASIC0( copym )
INSERT_GENTPROT_BASIC0( subm )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC2(ch,opname,_unb_var1) \
     ( \
       doff_t  diagoffx, \
       diag_t  diagx, \
       uplo_t  uplox, \
       trans_t transx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       ctype*  y, inc_t rs_y, inc_t cs_y, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( axpym )
INSERT_GENTPROT_BASIC0( scal2m )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC2(ch,opname,_unb_var1) \
     ( \
       conj_t  conjalpha, \
       doff_t  diagoffx, \
       diag_t  diagx, \
       uplo_t  uplox, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( scalm )
INSERT_GENTPROT_BASIC0( setm )

// end bli_l1m_unb_var1.h

// Pack-related
// begin bli_packm.h


// begin bli_packm_cntl.h


struct packm_params_s
{
	uint64_t    size; // size field must be present and come first.
	packm_voft  var_func;
	bszid_t     bmid_m;
	bszid_t     bmid_n;
	bool_t      does_invert_diag;
	bool_t      rev_iter_if_upper;
	bool_t      rev_iter_if_lower;
	pack_t      pack_schema;
	packbuf_t   pack_buf_type;
};
typedef struct packm_params_s packm_params_t;

#define bli_cntl_packm_params_var_func( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->var_func )

#define bli_cntl_packm_params_bmid_m( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->bmid_m )

#define bli_cntl_packm_params_bmid_n( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->bmid_n )

#define bli_cntl_packm_params_does_invert_diag( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->does_invert_diag )

#define bli_cntl_packm_params_rev_iter_if_upper( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->rev_iter_if_upper )

#define bli_cntl_packm_params_rev_iter_if_lower( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->rev_iter_if_lower )

#define bli_cntl_packm_params_pack_schema( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->pack_schema )

#define bli_cntl_packm_params_pack_buf_type( cntl ) \
\
	( ( (packm_params_t*)(cntl)->params )->pack_buf_type )

// -----------------------------------------------------------------------------

cntl_t* bli_packm_cntl_create_node
     (
       void*     var_func,
       void*     packm_var_func,
       bszid_t   bmid_m,
       bszid_t   bmid_n,
       bool_t    does_invert_diag,
       bool_t    rev_iter_if_upper,
       bool_t    rev_iter_if_lower,
       pack_t    pack_schema,
       packbuf_t pack_buf_type,
       cntl_t*   sub_node
     );

// end bli_packm_cntl.h
// begin bli_packm_check.h


void bli_packm_init_check
     (
       obj_t*  a,
       obj_t*  p,
       cntx_t* cntx
     );

void bli_packm_int_check
     (
       obj_t*  a,
       obj_t*  p,
       cntx_t* cntx
     );

// end bli_packm_check.h
// begin bli_packm_init.h


siz_t bli_packm_init
     (
       obj_t*  a,
       obj_t*  p,
       cntx_t* cntx,
       cntl_t* cntl
     );

siz_t bli_packm_init_pack
     (
       invdiag_t invert_diag,
       pack_t    schema,
       packord_t pack_ord_if_up,
       packord_t pack_ord_if_lo,
       bszid_t   bmult_id_m,
       bszid_t   bmult_id_n,
       obj_t*    a,
       obj_t*    p,
       cntx_t*   cntx
     );

// end bli_packm_init.h
// begin bli_packm_int.h


void bli_packm_int
     (
       obj_t*  a,
       obj_t*  p,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );
// end bli_packm_int.h

// begin bli_packm_part.h


// -- Matrix partitioning ------------------------------------------------------

void bli_packm_acquire_mpart_t2b( subpart_t requested_part,
                                  dim_t     i,
                                  dim_t     b,
                                  obj_t*    obj,
                                  obj_t*    sub_obj );

void bli_packm_acquire_mpart_l2r( subpart_t requested_part,
                                  dim_t     j,
                                  dim_t     b,
                                  obj_t*    obj,
                                  obj_t*    sub_obj );

void bli_packm_acquire_mpart_tl2br( subpart_t requested_part,
                                    dim_t     ij,
                                    dim_t     b,
                                    obj_t*    obj,
                                    obj_t*    sub_obj );

dim_t bli_packm_offset_to_panel_for( dim_t offmn, obj_t* p );

// end bli_packm_part.h

// begin bli_packm_unb_var1.h


void bli_packm_unb_var1
     (
       obj_t*  c,
       obj_t*  p,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );


#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t strucc, \
       doff_t  diagoffc, \
       diag_t  diagc, \
       uplo_t  uploc, \
       trans_t transc, \
       dim_t   m, \
       dim_t   n, \
       dim_t   m_max, \
       dim_t   n_max, \
       void*   kappa, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       void*   p, inc_t rs_p, inc_t cs_p, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( packm_unb_var1 )

// end bli_packm_unb_var1.h

// begin bli_packm_blk_var1.h


void bli_packm_blk_var1
     (
       obj_t*   c,
       obj_t*   p,
       cntx_t*  cntx,
       cntl_t*  cntl,
       thrinfo_t* t
     );


#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t strucc, \
       doff_t  diagoffc, \
       diag_t  diagc, \
       uplo_t  uploc, \
       trans_t transc, \
       pack_t  schema, \
       bool_t  invdiag, \
       bool_t  revifup, \
       bool_t  reviflo, \
       dim_t   m, \
       dim_t   n, \
       dim_t   m_max, \
       dim_t   n_max, \
       void*   kappa, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       void*   p, inc_t rs_p, inc_t cs_p, \
                  inc_t is_p, \
                  dim_t pd_p, inc_t ps_p, \
       void*   packm_ker, \
       cntx_t* cntx, \
       thrinfo_t* thread  \
     );

INSERT_GENTPROT_BASIC0( packm_blk_var1 )

// end bli_packm_blk_var1.h

// begin bli_packm_struc_cxk.h


#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffp, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROT_BASIC0( packm_struc_cxk )



#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                                      inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROT_BASIC0( packm_herm_cxk )



#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                                      inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROT_BASIC0( packm_tri_cxk )

// end bli_packm_struc_cxk.h
// begin bli_packm_struc_cxk_4mi.h


#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffp, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_struc_cxk_4mi )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_herm_cxk_4mi )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_tri_cxk_4mi )

// end bli_packm_struc_cxk_4mi.h
// begin bli_packm_struc_cxk_3mis.h


#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffp, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_struc_cxk_3mis )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_herm_cxk_3mis )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_tri_cxk_3mis )

// end bli_packm_struc_cxk_3mis.h
// begin bli_packm_struc_cxk_rih.h


#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffp, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_struc_cxk_rih )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                                      inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_herm_cxk_rih )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                                      inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_tri_cxk_rih )

// end bli_packm_struc_cxk_rih.h
// begin bli_packm_struc_cxk_1er.h


#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffp, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                          inc_t is_p, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_struc_cxk_1er )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                                      inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_herm_cxk_1er )



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t         strucc, \
       doff_t          diagoffc, \
       diag_t          diagc, \
       uplo_t          uploc, \
       conj_t          conjc, \
       pack_t          schema, \
       bool_t          invdiag, \
       dim_t           m_panel, \
       dim_t           n_panel, \
       dim_t           m_panel_max, \
       dim_t           n_panel_max, \
       dim_t           panel_dim, \
       dim_t           panel_len, \
       ctype* restrict kappa, \
       ctype* restrict c, inc_t rs_c, inc_t cs_c, \
                          inc_t incc, inc_t ldc, \
       ctype* restrict p, inc_t rs_p, inc_t cs_p, \
                                      inc_t ldp, \
       cntx_t*         cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_tri_cxk_1er )

// end bli_packm_struc_cxk_1er.h

// begin bli_packm_cxk.h



#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t  conja, \
       dim_t   panel_dim, \
       dim_t   panel_len, \
       ctype*  kappa, \
       ctype*  a, inc_t inca, inc_t lda, \
       ctype*  p,             inc_t ldp, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( packm_cxk )

// end bli_packm_cxk.h
// begin bli_packm_cxk_4mi.h



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t  conja, \
       dim_t   panel_dim, \
       dim_t   panel_len, \
       ctype*  kappa, \
       ctype*  a, inc_t inca, inc_t lda, \
       ctype*  p, inc_t is_p, inc_t ldp, \
       cntx_t* cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_cxk_4mi )

// end bli_packm_cxk_4mi.h
// begin bli_packm_cxk_3mis.h



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t  conja, \
       dim_t   panel_dim, \
       dim_t   panel_len, \
       ctype*  kappa, \
       ctype*  a, inc_t inca, inc_t lda, \
       ctype*  p, inc_t is_p, inc_t ldp, \
       cntx_t* cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_cxk_3mis )

// end bli_packm_cxk_3mis.h
// begin bli_packm_cxk_rih.h



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t  conja, \
       pack_t  schema, \
       dim_t   panel_dim, \
       dim_t   panel_len, \
       ctype*  kappa, \
       ctype*  a, inc_t inca, inc_t lda, \
       ctype*  p,             inc_t ldp, \
       cntx_t* cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_cxk_rih )

// end bli_packm_cxk_rih.h
// begin bli_packm_cxk_1er.h



#undef  GENTPROTCO
#define GENTPROTCO( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t  conja, \
       pack_t  schema, \
       dim_t   panel_dim, \
       dim_t   panel_len, \
       ctype*  kappa, \
       ctype*  a, inc_t inca, inc_t lda, \
       ctype*  p,             inc_t ldp, \
       cntx_t* cntx  \
     );

INSERT_GENTPROTCO_BASIC0( packm_cxk_1er )

// end bli_packm_cxk_1er.h

// end bli_packm.h
// begin bli_unpackm.h


// begin bli_unpackm_cntl.h


struct unpackm_params_s
{
	uint64_t      size; // size field must be present and come first.
	unpackm_voft  var_func;
};
typedef struct unpackm_params_s unpackm_params_t;

#define bli_cntl_unpackm_params_var_func( cntl ) \
\
	( ( (unpackm_params_t*)(cntl)->params )->var_func )

// -----------------------------------------------------------------------------

cntl_t* bli_unpackm_cntl_create_node
     (
       void*     var_func,
       void*     unpackm_var_func,
       cntl_t*   sub_node
     );

// end bli_unpackm_cntl.h
// begin bli_unpackm_check.h


void bli_unpackm_int_check
     (
       obj_t*  p,
       obj_t*  a,
       cntx_t* cntx
     );

// end bli_unpackm_check.h
// begin bli_unpackm_int.h


void bli_unpackm_int
     (
       obj_t*  p,
       obj_t*  a,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );

// end bli_unpackm_int.h

// begin bli_unpackm_unb_var1.h


void bli_unpackm_unb_var1
     (
       obj_t*  p,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       doff_t  diagoffp, \
       uplo_t  uplop, \
       trans_t transp, \
       dim_t   m, \
       dim_t   n, \
       void*   p, inc_t rs_p, inc_t cs_p, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( unpackm_unb_var1 )

// end bli_unpackm_unb_var1.h

// begin bli_unpackm_blk_var1.h


void bli_unpackm_blk_var1
     (
       obj_t*  p,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );


#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       struc_t strucc, \
       doff_t  diagoffc, \
       diag_t  diagc, \
       uplo_t  uploc, \
       trans_t transc, \
       dim_t   m, \
       dim_t   n, \
       dim_t   m_panel, \
       dim_t   n_panel, \
       void*   p, inc_t rs_p, inc_t cs_p, \
                  dim_t pd_p, inc_t ps_p, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( unpackm_blk_var1 )

// end bli_unpackm_blk_var1.h

// begin bli_unpackm_cxk.h



#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t  conjp, \
       dim_t   panel_dim, \
       dim_t   panel_len, \
       ctype*  kappa, \
       ctype*  p,             inc_t ldp, \
       ctype*  a, inc_t inca, inc_t lda, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( unpackm_cxk )

// end bli_unpackm_cxk.h
// end bli_unpackm.h

// Other
// begin bli_scalm.h


// begin bli_scalm_cntl.h



cntl_t* bli_scalm_cntl_create_node
     (
       void*   var_func,
       cntl_t* sub_node
     );
// end bli_scalm_cntl.h

// end bli_scalm.h

// end bli_l1m.h


// -- Level-2 operations --

// begin bli_l2.h


// begin bli_l2_check.h



//
// Prototype object-based check functions.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
    );

GENPROT( gemv )
GENPROT( hemv )
GENPROT( symv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  a  \
    );

GENPROT( ger )
GENPROT( her2 )
GENPROT( syr2 )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  a  \
    );

GENPROT( her )
GENPROT( syr )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x  \
    );

GENPROT( trmv )
GENPROT( trsv )


// -----------------------------------------------------------------------------

void bli_xxmv_check
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  x,
       obj_t*  beta,
       obj_t*  y 
     );

void bli_xxr_check
     (
       obj_t*  alpha,
       obj_t*  x,
       obj_t*  y,
       obj_t*  a 
     );
// end bli_l2_check.h

// begin bli_l2_ft.h


#ifndef BLIS_L2_FT_H
#define BLIS_L2_FT_H


//
// -- Level-2 function types ---------------------------------------------------
//

// gemv

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       trans_t transa, \
       conj_t  conjx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       ctype*  beta, \
       ctype*  y, inc_t incy, \
       cntx_t* cntx  \
     );

INSERT_GENTDEF( gemv )

// ger

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       conj_t  conjx, \
       conj_t  conjy, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t incx, \
       ctype*  y, inc_t incy, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       cntx_t* cntx  \
     );

INSERT_GENTDEF( ger )

// hemv (and symv)

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       uplo_t  uploa, \
       conj_t  conja, \
       conj_t  conjx, \
       conj_t  conjh, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       ctype*  beta, \
       ctype*  y, inc_t incy, \
       cntx_t* cntx  \
     );

INSERT_GENTDEF( hemv )

// her (and syr)

#undef  GENTDEFR
#define GENTDEFR( ctype, ctype_r, ch, chr, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       uplo_t   uploa, \
       conj_t   conjx, \
       conj_t   conjh, \
       dim_t    m, \
       ctype*   alpha,  \
       ctype*   x, inc_t incx, \
       ctype*   a, inc_t rs_a, inc_t cs_a, \
       cntx_t*  cntx  \
     );

INSERT_GENTDEFR( her )

// her2 (and syr2)

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       uplo_t  uploa, \
       conj_t  conjx, \
       conj_t  conjy, \
       conj_t  conjh, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  x, inc_t incx, \
       ctype*  y, inc_t incy, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       cntx_t* cntx  \
     );

INSERT_GENTDEF( her2 )

// trmv (and trsv)

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       cntx_t* cntx  \
     );

INSERT_GENTDEF( trmv )


#endif
// end bli_l2_ft.h

// Prototype object APIs with and without contexts.
// begin bli_oapi_w_cntx.h



// This file defines macros used to allow the _oapi.c files to
// produce object APIs that contain context parameters.

// Define the macro to add a suffix to the object function names.
// We use "ex" for "expert".
#undef  EX_SUF
#define EX_SUF _ex

// Define the macro to add cntx_t* arguments to function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM   ,cntx_t* cntx

// Define the macro to omit the cntx_t declaration block, since it is
// not needed when cntx_t's are passed in through the API.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL

// end bli_oapi_w_cntx.h
// begin bli_l2_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( gemv )
GENPROT( hemv )
GENPROT( symv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  a  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( ger )
GENPROT( her2 )
GENPROT( syr2 )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  a  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( her )
GENPROT( syr )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( trmv )
GENPROT( trsv )

// end bli_l2_oapi.h
// begin bli_oapi_wo_cntx.h


// This file defines macros used to allow the _oapi.c files to
// produce object APIs that omit context parameters.

// Define the macro to remove the function name suffix.
#undef  EX_SUF
#define EX_SUF

// Define the macro to omit cntx_t* arguments from function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM

// Define the macro to declare a local cntx_t pointer that is initialized
// to NULL.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL   cntx_t* cntx = NULL;

// end bli_oapi_wo_cntx.h
// begin bli_l2_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( gemv )
GENPROT( hemv )
GENPROT( symv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  a  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( ger )
GENPROT( her2 )
GENPROT( syr2 )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  a  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( her )
GENPROT( syr )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( trmv )
GENPROT( trsv )

// end bli_l2_oapi.h

// begin bli_l2_tapi.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       trans_t transa, \
       conj_t  conjx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       ctype*  beta, \
       ctype*  y, inc_t incy, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( gemv )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       conj_t  conjx, \
       conj_t  conjy, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t incx, \
       ctype*  y, inc_t incy, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( ger )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploa, \
       conj_t  conja, \
       conj_t  conjx, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       ctype*  beta, \
       ctype*  y, inc_t incy, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( hemv )
INSERT_GENTPROT_BASIC0( symv )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t   uploa, \
       conj_t   conjx, \
       dim_t    m, \
       ctype_r* alpha, \
       ctype*   x, inc_t incx, \
       ctype*   a, inc_t rs_a, inc_t cs_a, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( her )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t   uploa, \
       conj_t   conjx, \
       dim_t    m, \
       ctype*   alpha, \
       ctype*   x, inc_t incx, \
       ctype*   a, inc_t rs_a, inc_t cs_a, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROT_BASIC0( syr )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploa, \
       conj_t  conjx, \
       conj_t  conjy, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  x, inc_t incx, \
       ctype*  y, inc_t incy, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( her2 )
INSERT_GENTPROT_BASIC0( syr2 )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trmv )
INSERT_GENTPROT_BASIC0( trsv )
// end bli_l2_tapi.h

// Operation-specific headers
// begin bli_gemv.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_gemv_cntl.h"
//#include "bli_gemv_front.h"
//#include "bli_gemv_int.h"

// begin bli_gemv_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y, \
       cntx_t* cntx, \
       cntl_t* cntl  \
     );

GENPROT( gemv_blk_var1 )
GENPROT( gemv_blk_var2 )

GENPROT( gemv_unb_var1 )
GENPROT( gemv_unb_var2 )

GENPROT( gemv_unf_var1 )
GENPROT( gemv_unf_var2 )


//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       trans_t transa, \
       conj_t  conjx, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       ctype*  beta, \
       ctype*  y, inc_t incy, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( gemv_unb_var1 )
INSERT_GENTPROT_BASIC0( gemv_unb_var2 )

INSERT_GENTPROT_BASIC0( gemv_unf_var1 )
INSERT_GENTPROT_BASIC0( gemv_unf_var2 )

// end bli_gemv_var.h

// end bli_gemv.h
// begin bli_ger.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_ger_cntl.h"
//#include "bli_ger_front.h"
//#include "bli_ger_int.h"

// begin bli_ger_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  a, \
       cntx_t* cntx, \
       cntl_t* cntl  \
     );

GENPROT( ger_blk_var1 )
GENPROT( ger_blk_var2 )

GENPROT( ger_unb_var1 )
GENPROT( ger_unb_var2 )


//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       conj_t  conjx, \
       conj_t  conjy, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  x, inc_t incx, \
       ctype*  y, inc_t incy, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( ger_unb_var1 )
INSERT_GENTPROT_BASIC0( ger_unb_var2 )

// end bli_ger_var.h
// end bli_ger.h
// begin bli_hemv.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_hemv_cntl.h"
//#include "bli_hemv_front.h"
//#include "bli_hemv_int.h"

// begin bli_hemv_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       conj_t  conjh, \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       obj_t*  beta, \
       obj_t*  y, \
       cntx_t* cntx, \
       cntl_t* cntl  \
     );

GENPROT( hemv_blk_var1 )
GENPROT( hemv_blk_var2 )
GENPROT( hemv_blk_var3 )
GENPROT( hemv_blk_var4 )

GENPROT( hemv_unb_var1 )
GENPROT( hemv_unb_var2 )
GENPROT( hemv_unb_var3 )
GENPROT( hemv_unb_var4 )

GENPROT( hemv_unf_var1 )
GENPROT( hemv_unf_var3 )
GENPROT( hemv_unf_var1a )
GENPROT( hemv_unf_var3a )


//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       uplo_t  uplo, \
       conj_t  conja, \
       conj_t  conjx, \
       conj_t  conjh, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       ctype*  beta, \
       ctype*  y, inc_t incy, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( hemv_unb_var1 )
INSERT_GENTPROT_BASIC0( hemv_unb_var2 )
INSERT_GENTPROT_BASIC0( hemv_unb_var3 )
INSERT_GENTPROT_BASIC0( hemv_unb_var4 )

INSERT_GENTPROT_BASIC0( hemv_unf_var1 )
INSERT_GENTPROT_BASIC0( hemv_unf_var3 )
INSERT_GENTPROT_BASIC0( hemv_unf_var1a )
INSERT_GENTPROT_BASIC0( hemv_unf_var3a )

// end bli_hemv_var.h

// end bli_hemv.h
// begin bli_her.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_her_cntl.h"
//#include "bli_her_front.h"
//#include "bli_her_int.h"

// begin bli_her_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       conj_t  conjh, \
       obj_t*  alpha, \
       obj_t*  x, \
       obj_t*  c, \
       cntx_t* cntx, \
       cntl_t* cntl  \
     );

GENPROT( her_blk_var1 )
GENPROT( her_blk_var2 )

GENPROT( her_unb_var1 )
GENPROT( her_unb_var2 )


//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       uplo_t  uplo, \
       conj_t  conjx, \
       conj_t  conjh, \
       dim_t   m, \
       ctype*  alpha,  \
       ctype*  x, inc_t incx, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROTR_BASIC0( her_unb_var1 )
INSERT_GENTPROTR_BASIC0( her_unb_var2 )

// end bli_her_var.h
// end bli_her.h
// begin bli_her2.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_her2_cntl.h"
//#include "bli_her2_front.h"
//#include "bli_her2_int.h"

// begin bli_her2_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       conj_t  conjh, \
       obj_t*  alpha, \
       obj_t*  alpha_conj, \
       obj_t*  x, \
       obj_t*  y, \
       obj_t*  c, \
       cntx_t* cntx, \
       cntl_t* cntl  \
     );

GENPROT( her2_blk_var1 )
GENPROT( her2_blk_var2 )
GENPROT( her2_blk_var3 )
GENPROT( her2_blk_var4 )

GENPROT( her2_unb_var1 )
GENPROT( her2_unb_var2 )
GENPROT( her2_unb_var3 )
GENPROT( her2_unb_var4 )

GENPROT( her2_unf_var1 )
GENPROT( her2_unf_var4 )


//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       uplo_t  uplo, \
       conj_t  conjx, \
       conj_t  conjy, \
       conj_t  conjh, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  x, inc_t incx, \
       ctype*  y, inc_t incy, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( her2_unb_var1 )
INSERT_GENTPROT_BASIC0( her2_unb_var2 )
INSERT_GENTPROT_BASIC0( her2_unb_var3 )
INSERT_GENTPROT_BASIC0( her2_unb_var4 )

INSERT_GENTPROT_BASIC0( her2_unf_var1 )
INSERT_GENTPROT_BASIC0( her2_unf_var4 )

// end bli_her2_var.h
// end bli_her2.h
// begin bli_symv.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_symv_front.h"

// end bli_symv.h
// begin bli_syr.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_syr_front.h"

// end bli_syr.h
// begin bli_syr2.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_syr2_front.h"

// end bli_syr2.h
// begin bli_trmv.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_trmv_cntl.h"
//#include "bli_trmv_front.h"
//#include "bli_trmv_int.h"

// begin bli_trmv_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       cntx_t* cntx, \
       cntl_t* cntl  \
     );

GENPROT( trmv_l_blk_var1 )
GENPROT( trmv_l_blk_var2 )
GENPROT( trmv_u_blk_var1 )
GENPROT( trmv_u_blk_var2 )

GENPROT( trmv_unb_var1 )
GENPROT( trmv_unb_var2 )

GENPROT( trmv_unf_var1 )
GENPROT( trmv_unf_var2 )


//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trmv_unb_var1 )
INSERT_GENTPROT_BASIC0( trmv_unb_var2 )

INSERT_GENTPROT_BASIC0( trmv_unf_var1 )
INSERT_GENTPROT_BASIC0( trmv_unf_var2 )

// end bli_trmv_var.h

// end bli_trmv.h
// begin bli_trsv.h


// NOTE: level-2 control tree code is temporarily disabled.
//#include "bli_trsv_cntl.h"
//#include "bli_trsv_front.h"
//#include "bli_trsv_int.h"

// begin bli_trsv_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  x, \
       cntx_t* cntx, \
       cntl_t* cntl  \
     );

GENPROT( trsv_l_blk_var1 )
GENPROT( trsv_l_blk_var2 )
GENPROT( trsv_u_blk_var1 )
GENPROT( trsv_u_blk_var2 )

GENPROT( trsv_unb_var1 )
GENPROT( trsv_unb_var2 )

GENPROT( trsv_unf_var1 )
GENPROT( trsv_unf_var2 )


//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       dim_t   m, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  x, inc_t incx, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trsv_unb_var1 )
INSERT_GENTPROT_BASIC0( trsv_unb_var2 )

INSERT_GENTPROT_BASIC0( trsv_unf_var1 )
INSERT_GENTPROT_BASIC0( trsv_unf_var2 )

// end bli_trsv_var.h

// end bli_trsv.h
// end bli_l2.h


// -- Level-3 operations --

// begin bli_l3.h


// begin bli_l3_cntl.h



//
// Prototype conditional control tree creation functions.
//

void bli_l3_cntl_create_if
     (
       opid_t   family,
       obj_t*   a,
       obj_t*   b,
       obj_t*   c,
       cntl_t*  cntl_orig,
       cntl_t** cntl_use
     );

void bli_l3_cntl_free_if
     (
       obj_t*  a,
       obj_t*  b,
       obj_t*  c,
       cntl_t* cntl_orig,
       cntl_t* cntl_use,
       thrinfo_t* thread
     );

// end bli_l3_cntl.h
// begin bli_l3_check.h



//
// Prototype object-based check functions.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  beta, \
       obj_t*  c, \
       cntx_t* cntx  \
    );

GENPROT( gemm )
GENPROT( her2k )
GENPROT( syr2k )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       side_t  side, \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  beta, \
       obj_t*  c, \
       cntx_t* cntx  \
    );

GENPROT( hemm )
GENPROT( symm )
GENPROT( trmm )
GENPROT( trsm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  beta, \
       obj_t*  c, \
       cntx_t* cntx  \
    );

GENPROT( herk )
GENPROT( syrk )


// -----------------------------------------------------------------------------

void bli_gemm_basic_check
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx
     );

void bli_hemm_basic_check
     (
       side_t  side,
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx
     );

void bli_herk_basic_check
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  ah,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx
     );

void bli_her2k_basic_check
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  bh,
       obj_t*  b,
       obj_t*  ah,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx
     );

void bli_l3_basic_check
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx
     );
// end bli_l3_check.h

// begin bli_l3_ft.h


#ifndef BLIS_L3_FT_H
#define BLIS_L3_FT_H


//
// -- Level-3 micro-kernel types -----------------------------------------------
//

// gemm

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       dim_t               k, \
       ctype*     restrict alpha, \
       ctype*     restrict a, \
       ctype*     restrict b, \
       ctype*     restrict beta, \
       ctype*     restrict c, inc_t rs_c, inc_t cs_c, \
       auxinfo_t* restrict data, \
       cntx_t*    restrict cntx  \
     );

INSERT_GENTDEF( gemm_ukr )


// trsm_[lu]

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       ctype*     restrict a, \
       ctype*     restrict b, \
       ctype*     restrict c, inc_t rs_c, inc_t cs_c, \
       auxinfo_t* restrict data, \
       cntx_t*    restrict cntx  \
     );

INSERT_GENTDEF( trsm_ukr )


// gemmtrsm_[lu]

#undef  GENTDEF
#define GENTDEF( ctype, ch, opname, tsuf ) \
\
typedef void (*PASTECH2(ch,opname,tsuf)) \
     ( \
       dim_t               k, \
       ctype*     restrict alpha, \
       ctype*     restrict a1x, \
       ctype*     restrict a11, \
       ctype*     restrict bx1, \
       ctype*     restrict b11, \
       ctype*     restrict c11, inc_t rs_c, inc_t cs_c, \
       auxinfo_t* restrict data, \
       cntx_t*    restrict cntx  \
     );

INSERT_GENTDEF( gemmtrsm_ukr )






#endif

// end bli_l3_ft.h
// begin bli_l3_oft.h


#ifndef BLIS_L3_OFT_H
#define BLIS_L3_OFT_H


//
// -- Level-3 object function types --------------------------------------------
//

// gemm

#undef  GENTDEF
#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_oft)) \
( \
  obj_t*  alpha, \
  obj_t*  a, \
  obj_t*  b, \
  obj_t*  beta, \
  obj_t*  c, \
  cntx_t* cntx  \
);

GENTDEF( gemm )
GENTDEF( her2k )
GENTDEF( syr2k )


// hemm, symm, trmm3

#undef  GENTDEF
#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_oft)) \
( \
  side_t  side, \
  obj_t*  alpha, \
  obj_t*  a, \
  obj_t*  b, \
  obj_t*  beta, \
  obj_t*  c, \
  cntx_t* cntx  \
);

GENTDEF( hemm )
GENTDEF( symm )
GENTDEF( trmm3 )


// herk, syrk

#undef  GENTDEF
#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_oft)) \
( \
  obj_t*  alpha, \
  obj_t*  a, \
  obj_t*  beta, \
  obj_t*  c, \
  cntx_t* cntx  \
);

GENTDEF( herk )
GENTDEF( syrk )


// trmm, trsm

#undef  GENTDEF
#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_oft)) \
( \
  side_t  side, \
  obj_t*  alpha, \
  obj_t*  a, \
  obj_t*  b, \
  cntx_t* cntx  \
);

GENTDEF( trmm )
GENTDEF( trsm )



#endif

// end bli_l3_oft.h
// begin bli_l3_voft.h


#ifndef BLIS_L3_VAR_OFT_H
#define BLIS_L3_VAR_OFT_H


//
// -- Level-3 variant function types -------------------------------------------
//

#undef  GENTDEF
#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_voft)) \
( \
  obj_t*  a, \
  obj_t*  b, \
  obj_t*  c, \
  cntx_t* cntx, \
  cntl_t* cntl, \
  thrinfo_t* thread  \
);

GENTDEF( gemm )


#define GENTDEF( opname ) \
\
typedef void (*PASTECH(opname,_voft)) \
( \
  obj_t*  a, \
  obj_t*  b, \
  obj_t*  c, \
  cntx_t* cntx, \
  cntl_t* cntl, \
  thrinfo_t* thread  \
);

GENTDEF( trsm )



#endif

// end bli_l3_voft.h

// begin bli_l3_blocksize.h


dim_t bli_l3_determine_kc
      (
        dir_t   direct,
        dim_t   i,
        dim_t   dim,
        obj_t*  a,
        obj_t*  b,
        bszid_t bszid,
        cntx_t* cntx,
        cntl_t* cntl
      );


#undef  GENPROT
#define GENPROT( opname ) \
\
dim_t PASTEMAC0(opname) \
      ( \
         dir_t   direct, \
         dim_t   i, \
         dim_t   dim, \
         obj_t*  a, \
         obj_t*  b, \
         bszid_t bszid, \
         cntx_t* cntx  \
      );

GENPROT( gemm_determine_kc )
GENPROT( herk_determine_kc )
GENPROT( trmm_determine_kc )
GENPROT( trsm_determine_kc )


#undef  GENPROT
#define GENPROT( opname ) \
\
dim_t PASTEMAC0(opname) \
      ( \
         dim_t   i, \
         dim_t   dim, \
         obj_t*  a, \
         obj_t*  b, \
         bszid_t bszid, \
         cntx_t* cntx  \
      );

GENPROT( gemm_determine_kc_f )
GENPROT( gemm_determine_kc_b )

GENPROT( herk_determine_kc_f )
GENPROT( herk_determine_kc_b )

GENPROT( trmm_determine_kc_f )
GENPROT( trmm_determine_kc_b )

GENPROT( trsm_determine_kc_f )
GENPROT( trsm_determine_kc_b )

// end bli_l3_blocksize.h
// begin bli_l3_direct.h


dir_t bli_l3_direct
     (
       obj_t*  a,
       obj_t*  b,
       obj_t*  c,
       cntl_t* cntl
     );

// -----------------------------------------------------------------------------

#undef  GENPROT
#define GENPROT( opname ) \
\
dir_t PASTEMAC0(opname) \
      ( \
         obj_t*  a, \
         obj_t*  b, \
         obj_t*  c  \
      );

GENPROT( gemm_direct )
GENPROT( herk_direct )
GENPROT( trmm_direct )
GENPROT( trsm_direct )

// end bli_l3_direct.h
// begin bli_l3_prune.h



#undef  GENPROT
#define GENPROT( dim ) \
\
void PASTEMAC(l3_prune_unref_mparts_,dim) \
     ( \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  c, \
       cntl_t* cntl  \
     );

GENPROT( m )
GENPROT( n )
GENPROT( k )

// -----------------------------------------------------------------------------

#undef  GENPROT
#define GENPROT( opname, dim ) \
\
void PASTEMAC2(opname,_prune_unref_mparts_,dim) \
     ( \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  c  \
     );

GENPROT( gemm, m )
GENPROT( gemm, n )
GENPROT( gemm, k )

GENPROT( herk, m )
GENPROT( herk, n )
GENPROT( herk, k )

GENPROT( trmm, m )
GENPROT( trmm, n )
GENPROT( trmm, k )

GENPROT( trsm, m )
GENPROT( trsm, n )
GENPROT( trsm, k )

// end bli_l3_prune.h
// begin bli_l3_packm.h


void bli_l3_packm
     (
       obj_t*  x,
       obj_t*  x_pack,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );

// end bli_l3_packm.h

// Prototype object APIs with and without contexts.
// begin bli_oapi_w_cntx.h



// This file defines macros used to allow the _oapi.c files to
// produce object APIs that contain context parameters.

// Define the macro to add a suffix to the object function names.
// We use "ex" for "expert".
#undef  EX_SUF
#define EX_SUF _ex

// Define the macro to add cntx_t* arguments to function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM   ,cntx_t* cntx

// Define the macro to omit the cntx_t declaration block, since it is
// not needed when cntx_t's are passed in through the API.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL

// end bli_oapi_w_cntx.h
// begin bli_l3_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  beta, \
       obj_t*  c  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( gemm )
GENPROT( her2k )
GENPROT( syr2k )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       side_t  side, \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  beta, \
       obj_t*  c  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( hemm )
GENPROT( symm )
GENPROT( trmm3 )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  beta, \
       obj_t*  c  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( herk )
GENPROT( syrk )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       side_t  side, \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( trmm )
GENPROT( trsm )

// end bli_l3_oapi.h
// begin bli_oapi_wo_cntx.h


// This file defines macros used to allow the _oapi.c files to
// produce object APIs that omit context parameters.

// Define the macro to remove the function name suffix.
#undef  EX_SUF
#define EX_SUF

// Define the macro to omit cntx_t* arguments from function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM

// Define the macro to declare a local cntx_t pointer that is initialized
// to NULL.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL   cntx_t* cntx = NULL;

// end bli_oapi_wo_cntx.h
// begin bli_l3_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  beta, \
       obj_t*  c  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( gemm )
GENPROT( her2k )
GENPROT( syr2k )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       side_t  side, \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  beta, \
       obj_t*  c  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( hemm )
GENPROT( symm )
GENPROT( trmm3 )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  beta, \
       obj_t*  c  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( herk )
GENPROT( syrk )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       side_t  side, \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( trmm )
GENPROT( trsm )

// end bli_l3_oapi.h

// begin bli_l3_tapi.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       trans_t transa, \
       trans_t transb, \
       dim_t   m, \
       dim_t   n, \
       dim_t   k, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( gemm )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       conj_t  conja, \
       trans_t transb, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( hemm )
INSERT_GENTPROT_BASIC0( symm )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t   uploc, \
       trans_t  transa, \
       dim_t    m, \
       dim_t    k, \
       ctype_r* alpha, \
       ctype*   a, inc_t rs_a, inc_t cs_a, \
       ctype_r* beta, \
       ctype*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( herk )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t   uploc, \
       trans_t  transa, \
       trans_t  transb, \
       dim_t    m, \
       dim_t    k, \
       ctype*   alpha, \
       ctype*   a, inc_t rs_a, inc_t cs_a, \
       ctype*   b, inc_t rs_b, inc_t cs_b, \
       ctype_r* beta, \
       ctype*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( her2k )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploc, \
       trans_t transa, \
       dim_t   m, \
       dim_t   k, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( syrk )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploc, \
       trans_t transa, \
       trans_t transb, \
       dim_t   m, \
       dim_t   k, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( syr2k )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       trans_t transb, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       ctype*  beta, \
       ctype*  c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trmm3 )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       side_t  side, \
       uplo_t  uploa, \
       trans_t transa, \
       diag_t  diaga, \
       dim_t   m, \
       dim_t   n, \
       ctype*  alpha, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       ctype*  b, inc_t rs_b, inc_t cs_b, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( trmm )
INSERT_GENTPROT_BASIC0( trsm )

// end bli_l3_tapi.h

// begin bli_l3_ukr_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  alpha, \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  beta, \
       obj_t*  c, \
       cntx_t* cntx  \
     );

GENPROT( gemm_ukernel )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  c, \
       cntx_t* cntx  \
     );

GENPROT( trsm_ukernel )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  alpha, \
       obj_t*  a1x, \
       obj_t*  a11, \
       obj_t*  bx1, \
       obj_t*  b11, \
       obj_t*  c11, \
       cntx_t* cntx  \
     );

GENPROT( gemmtrsm_ukernel )

// end bli_l3_ukr_oapi.h
// begin bli_l3_ukr_tapi.h



//
// Generate prototypes for level-3 micro-kernel wrappers.
//

#undef  gemm_ukr_name
#define gemm_ukr_name       gemm_ukernel

#undef  gemmtrsm_l_ukr_name
#define gemmtrsm_l_ukr_name gemmtrsm_l_ukernel
#undef  gemmtrsm_u_ukr_name
#define gemmtrsm_u_ukr_name gemmtrsm_u_ukernel

#undef  trsm_l_ukr_name
#define trsm_l_ukr_name     trsm_l_ukernel
#undef  trsm_u_ukr_name
#define trsm_u_ukr_name     trsm_u_ukernel

// Include the level-3 micro-kernel API template.

// begin bli_l3_ukr.h


//
// Define template prototypes for level-3 micro-kernels.
//

// Note: Instead of defining function prototype macro templates and then
// instantiating those macros to define the individual function prototypes,
// we simply alias the official operations' prototypes as defined in
// bli_l3_ukr_prot.h.

#undef  GENTPROT
#define GENTPROT GEMM_UKR_PROT

INSERT_GENTPROT_BASIC0( gemm_ukr_name )


#undef  GENTPROT
#define GENTPROT GEMMTRSM_UKR_PROT

INSERT_GENTPROT_BASIC0( gemmtrsm_l_ukr_name )
INSERT_GENTPROT_BASIC0( gemmtrsm_u_ukr_name )


#undef  GENTPROT
#define GENTPROT TRSM_UKR_PROT

INSERT_GENTPROT_BASIC0( trsm_l_ukr_name )
INSERT_GENTPROT_BASIC0( trsm_u_ukr_name )

// end bli_l3_ukr.h

// end bli_l3_ukr_tapi.h

// Prototype reference micro-kernels.
//#include "bli_l3_ref.h"

// Operation-specific headers
// begin bli_gemm.h


// begin bli_gemm_cntl.h


cntl_t* bli_gemm_cntl_create
     (
       opid_t family
     );

// -----------------------------------------------------------------------------

cntl_t* bli_gemmbp_cntl_create
     (
       opid_t family
     );

cntl_t* bli_gemmpb_cntl_create
     (
       opid_t family
     );

// -----------------------------------------------------------------------------

void bli_gemm_cntl_free
     (
       cntl_t* cntl,
       thrinfo_t* thread
     );

// -----------------------------------------------------------------------------

cntl_t* bli_gemm_cntl_create_node
     (
       opid_t  family,
       bszid_t bszid,
       void*   var_func,
       cntl_t* sub_node
     );

// end bli_gemm_cntl.h
// begin bli_gemm_front.h


void bli_gemm_front
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );

err_t bli_gemm_small
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_gemm_front.h
// begin bli_gemm_int.h


void bli_gemm_int
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );

// end bli_gemm_int.h

// begin bli_gemm_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  c, \
       cntx_t* cntx, \
       cntl_t* cntl, \
       thrinfo_t* thread  \
     );

GENPROT( gemm_blk_var1 )
GENPROT( gemm_blk_var2 )
GENPROT( gemm_blk_var3 )
GENPROT( gemm_packa )
GENPROT( gemm_packb )

GENPROT( gemm_ker_var1 )
GENPROT( gemm_ker_var2 )

// Headers for induced algorithms:
GENPROT( gemm4mb_ker_var2 ) // 4m1b


//
// Prototype BLAS-like interfaces with void pointer operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       pack_t  schema_a, \
       pack_t  schema_b, \
       dim_t   m, \
       dim_t   n, \
       dim_t   k, \
       void*   alpha, \
       void*   a, inc_t cs_a, inc_t is_a, \
                  dim_t pd_a, inc_t ps_a, \
       void*   b, inc_t rs_b, inc_t is_b, \
                  dim_t pd_b, inc_t ps_b, \
       void*   beta, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx, \
       thrinfo_t* thread  \
     );

INSERT_GENTPROT_BASIC0( gemm_ker_var2 )

// Headers for induced algorithms:
INSERT_GENTPROT_BASIC0( gemm4mb_ker_var2 ) // 4m1b

// end bli_gemm_var.h

// end bli_gemm.h
// begin bli_hemm.h


// begin bli_hemm_front.h


void bli_hemm_front
     (
       side_t  side,
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_hemm_front.h

// end bli_hemm.h
// begin bli_herk.h


// begin bli_herk_front.h


void bli_herk_front
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_herk_front.h

// begin bli_herk_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  a, \
       obj_t*  ah, \
       obj_t*  c, \
       cntx_t* cntx, \
       cntl_t* cntl, \
       thrinfo_t* thread  \
     );

//GENPROT( herk_blk_var1 )
//GENPROT( herk_blk_var2 )
//GENPROT( herk_blk_var3 )

GENPROT( herk_x_ker_var2 )
GENPROT( herk_l_ker_var2 )
GENPROT( herk_u_ker_var2 )
//GENPROT( herk_packa )
//GENPROT( herk_packb )


//
// Prototype BLAS-like interfaces with void pointer operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       doff_t  diagoffc, \
       pack_t  schema_a, \
       pack_t  schema_b, \
       dim_t   m, \
       dim_t   n, \
       dim_t   k, \
       void*   alpha, \
       void*   a, inc_t cs_a, inc_t is_a, \
                  dim_t pd_a, inc_t ps_a, \
       void*   b, inc_t rs_b, inc_t is_b, \
                  dim_t pd_b, inc_t ps_b, \
       void*   beta, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx, \
       thrinfo_t* thread  \
     );

INSERT_GENTPROT_BASIC0( herk_l_ker_var2 )
INSERT_GENTPROT_BASIC0( herk_u_ker_var2 )

// end bli_herk_var.h

// end bli_herk.h
// begin bli_her2k.h


// begin bli_her2k_front.h


void bli_her2k_front
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_her2k_front.h

// end bli_her2k.h
// begin bli_symm.h


// begin bli_symm_front.h


void bli_symm_front
     (
       side_t  side,
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_symm_front.h

// end bli_symm.h
// begin bli_syrk.h


// begin bli_syrk_front.h


void bli_syrk_front
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_syrk_front.h

// end bli_syrk.h
// begin bli_syr2k.h


// begin bli_syr2k_front.h


void bli_syr2k_front
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_syr2k_front.h

// end bli_syr2k.h
// begin bli_trmm.h


// begin bli_trmm_front.h


void bli_trmm_front
     (
       side_t  side,
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_trmm_front.h

// begin bli_trmm_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  c, \
       cntx_t* cntx, \
       cntl_t* cntl, \
       thrinfo_t* thread  \
     );

//GENPROT( trmm_blk_var1 )
//GENPROT( trmm_blk_var2 )
//GENPROT( trmm_blk_var3 )

GENPROT( trmm_xx_ker_var2 )
GENPROT( trmm_ll_ker_var2 )
GENPROT( trmm_lu_ker_var2 )
GENPROT( trmm_rl_ker_var2 )
GENPROT( trmm_ru_ker_var2 )


//
// Prototype BLAS-like interfaces with void pointer operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       doff_t  diagoff, \
       pack_t  schema_a, \
       pack_t  schema_b, \
       dim_t   m, \
       dim_t   n, \
       dim_t   k, \
       void*   alpha, \
       void*   a, inc_t cs_a, \
                  dim_t pd_a, inc_t ps_a, \
       void*   b, inc_t rs_b, \
                  dim_t pd_b, inc_t ps_b, \
       void*   beta, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx, \
       thrinfo_t* thread  \
     );

INSERT_GENTPROT_BASIC0( trmm_ll_ker_var2 )
INSERT_GENTPROT_BASIC0( trmm_lu_ker_var2 )
INSERT_GENTPROT_BASIC0( trmm_rl_ker_var2 )
INSERT_GENTPROT_BASIC0( trmm_ru_ker_var2 )

// end bli_trmm_var.h

// end bli_trmm.h
// begin bli_trmm3.h


// begin bli_trmm3_front.h


void bli_trmm3_front
     (
       side_t  side,
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_trmm3_front.h

// end bli_trmm3.h
// begin bli_trsm.h


// begin bli_trsm_cntl.h


cntl_t* bli_trsm_cntl_create
     (
       side_t side
     );

cntl_t* bli_trsm_l_cntl_create
     (
       void
     );

cntl_t* bli_trsm_r_cntl_create
     (
       void
     );

void bli_trsm_cntl_free
     (
       cntl_t* cntl,
       thrinfo_t* thread
     );

// -----------------------------------------------------------------------------

cntl_t* bli_trsm_cntl_create_node
     (
       opid_t  family,
       bszid_t bszid,
       void*   var_func, 
       cntl_t* sub_node
     );

// end bli_trsm_cntl.h
// begin bli_trsm_front.h


void bli_trsm_front
     (
       side_t  side,
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       cntx_t* cntx,
       cntl_t* cntl
     );
// end bli_trsm_front.h
// begin bli_trsm_int.h


void bli_trsm_int
     (
       obj_t*  alpha,
       obj_t*  a,
       obj_t*  b,
       obj_t*  beta,
       obj_t*  c,
       cntx_t* cntx,
       cntl_t* cntl,
       thrinfo_t* thread
     );

// end bli_trsm_int.h

// begin bli_trsm_var.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC0(opname) \
     ( \
       obj_t*  a, \
       obj_t*  b, \
       obj_t*  c, \
       cntx_t* cntx, \
       cntl_t* cntl, \
       thrinfo_t* thread  \
     );

GENPROT( trsm_blk_var1 )
GENPROT( trsm_blk_var2 )
GENPROT( trsm_blk_var3 )
GENPROT( trsm_packa )
GENPROT( trsm_packb )

GENPROT( trsm_xx_ker_var2 )
GENPROT( trsm_ll_ker_var2 )
GENPROT( trsm_lu_ker_var2 )
GENPROT( trsm_rl_ker_var2 )
GENPROT( trsm_ru_ker_var2 )


//
// Prototype BLAS-like interfaces with void pointer operands.
//

#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       doff_t  diagoff, \
       pack_t  schema_a, \
       pack_t  schema_b, \
       dim_t   m, \
       dim_t   n, \
       dim_t   k, \
       void*   alpha1, \
       void*   a, inc_t cs_a, \
                  dim_t pd_a, inc_t ps_a, \
       void*   b, inc_t rs_b, \
                  dim_t pd_b, inc_t ps_b, \
       void*   alpha2, \
       void*   c, inc_t rs_c, inc_t cs_c, \
       cntx_t* cntx, \
       thrinfo_t* thread  \
     );

INSERT_GENTPROT_BASIC0( trsm_ll_ker_var2 )
INSERT_GENTPROT_BASIC0( trsm_lu_ker_var2 )
INSERT_GENTPROT_BASIC0( trsm_rl_ker_var2 )
INSERT_GENTPROT_BASIC0( trsm_ru_ker_var2 )

// end bli_trsm_var.h

// end bli_trsm.h

// end bli_l3.h


// -- Utility operations --

// begin bli_util.h


// begin bli_util_check.h



//
// Prototype object-based check functions.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  asum  \
     );

GENPROT( asumv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x  \
     );

GENPROT( mkherm )
GENPROT( mksymm )
GENPROT( mktrim )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  norm  \
     );

GENPROT( norm1v )
GENPROT( normfv )
GENPROT( normiv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  norm  \
     );

GENPROT( norm1m )
GENPROT( normfm )
GENPROT( normim )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       FILE*  file, \
       char*  s1, \
       obj_t* x, \
       char*  format, \
       char*  s2  \
     );

GENPROT( fprintv )
GENPROT( fprintm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x  \
     );

GENPROT( randv )
GENPROT( randnv )
GENPROT( randm )
GENPROT( randnm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,_check) \
     ( \
       obj_t*  x, \
       obj_t*  scale, \
       obj_t*  sumsq  \
     );

GENPROT( sumsqv )


// -----------------------------------------------------------------------------

void bli_utilv_xi_check
     (
       obj_t*  x,
       obj_t*  index
     );

void bli_utilv_xa_check
     (
       obj_t*  x,
       obj_t*  asum
     );

void bli_utilm_mkhst_check
     (
       obj_t*  a
     );

void bli_utilv_norm_check
     (
       obj_t*  x,
       obj_t*  norm
     );

void bli_utilm_norm_check
     (
       obj_t*  x,
       obj_t*  norm
     );

void bli_utilm_fprint_check
     (
       FILE*  file,
       char*  s1,
       obj_t* x,
       char*  format,
       char*  s2
     );

void bli_utilm_rand_check
     (
       obj_t* x
     );

void bli_utilv_sumsqv_check
     (
       obj_t*  x,
       obj_t*  scale,
       obj_t*  sumsq
     );

// end bli_util_check.h

// Prototype object APIs with and without contexts.
// begin bli_oapi_w_cntx.h



// This file defines macros used to allow the _oapi.c files to
// produce object APIs that contain context parameters.

// Define the macro to add a suffix to the object function names.
// We use "ex" for "expert".
#undef  EX_SUF
#define EX_SUF _ex

// Define the macro to add cntx_t* arguments to function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM   ,cntx_t* cntx

// Define the macro to omit the cntx_t declaration block, since it is
// not needed when cntx_t's are passed in through the API.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL

// end bli_oapi_w_cntx.h
// begin bli_util_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  asum  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( asumv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  a  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( mkherm )
GENPROT( mksymm )
GENPROT( mktrim )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  norm  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( norm1v )
GENPROT( normfv )
GENPROT( normiv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  norm  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( norm1m )
GENPROT( normfm )
GENPROT( normim )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       FILE*   file, \
       char*   s1, \
       obj_t*  x, \
       char*   format, \
       char*   s2  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( fprintv )
GENPROT( fprintm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       char*   s1, \
       obj_t*  x, \
       char*   format, \
       char*   s2  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( printv )
GENPROT( printm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( randv )
GENPROT( randnv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( randm )
GENPROT( randnm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  scale, \
       obj_t*  sumsq  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( sumsqv )

// end bli_util_oapi.h
// begin bli_oapi_wo_cntx.h


// This file defines macros used to allow the _oapi.c files to
// produce object APIs that omit context parameters.

// Define the macro to remove the function name suffix.
#undef  EX_SUF
#define EX_SUF

// Define the macro to omit cntx_t* arguments from function signatures
// and prototypes.
#undef  BLIS_OAPI_CNTX_PARAM
#define BLIS_OAPI_CNTX_PARAM

// Define the macro to declare a local cntx_t pointer that is initialized
// to NULL.
#undef  BLIS_OAPI_CNTX_DECL
#define BLIS_OAPI_CNTX_DECL   cntx_t* cntx = NULL;

// end bli_oapi_wo_cntx.h
// begin bli_util_oapi.h



//
// Prototype object-based interfaces.
//

#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  asum  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( asumv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  a  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( mkherm )
GENPROT( mksymm )
GENPROT( mktrim )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  norm  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( norm1v )
GENPROT( normfv )
GENPROT( normiv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  norm  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( norm1m )
GENPROT( normfm )
GENPROT( normim )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       FILE*   file, \
       char*   s1, \
       obj_t*  x, \
       char*   format, \
       char*   s2  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( fprintv )
GENPROT( fprintm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       char*   s1, \
       obj_t*  x, \
       char*   format, \
       char*   s2  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( printv )
GENPROT( printm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( randv )
GENPROT( randnv )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( randm )
GENPROT( randnm )


#undef  GENPROT
#define GENPROT( opname ) \
\
void PASTEMAC(opname,EX_SUF) \
     ( \
       obj_t*  x, \
       obj_t*  scale, \
       obj_t*  sumsq  \
       BLIS_OAPI_CNTX_PARAM  \
     );

GENPROT( sumsqv )

// end bli_util_oapi.h

// begin bli_util_tapi.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t    n, \
       ctype*   x, inc_t incx, \
       ctype_r* asum, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( asumv )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       uplo_t  uploa, \
       dim_t   m, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( mkherm )
INSERT_GENTPROT_BASIC0( mksymm )
INSERT_GENTPROT_BASIC0( mktrim )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t    n, \
       ctype*   x, inc_t incx, \
       ctype_r* norm, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( norm1v )
INSERT_GENTPROTR_BASIC0( normfv )
INSERT_GENTPROTR_BASIC0( normiv )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t   diagoffx, \
       diag_t   diagx, \
       uplo_t   uplox, \
       dim_t    m, \
       dim_t    n, \
       ctype*   x, inc_t rs_x, inc_t cs_x, \
       ctype_r* norm, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( norm1m )
INSERT_GENTPROTR_BASIC0( normfm )
INSERT_GENTPROTR_BASIC0( normim )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       char*  s1, \
       dim_t  n, \
       void*  x, inc_t incx, \
       char*  format, \
       char*  s2  \
     );

INSERT_GENTPROT_BASIC0_I( printv )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       char*  s1, \
       dim_t  m, \
       dim_t  n, \
       void*  x, inc_t rs_x, inc_t cs_x, \
       char*  format, \
       char*  s2  \
     );

INSERT_GENTPROT_BASIC0_I( printm )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t    n, \
       ctype*   x, inc_t incx, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROT_BASIC0( randv )
INSERT_GENTPROT_BASIC0( randnv )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       doff_t  diagoffx, \
       uplo_t  uplox, \
       dim_t   m, \
       dim_t   n, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( randm )
INSERT_GENTPROT_BASIC0( randnm )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       dim_t    n, \
       ctype*   x, inc_t incx, \
       ctype_r* scale, \
       ctype_r* sumsq, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( sumsqv )


// end bli_util_tapi.h
// begin bli_util_unb_var1.h



//
// Prototype BLAS-like interfaces with typed operands.
//

#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       dim_t    n, \
       ctype*   x, inc_t incx, \
       ctype_r* asum, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( asumv_unb_var1 )


#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       uplo_t  uploa, \
       dim_t   m, \
       ctype*  a, inc_t rs_a, inc_t cs_a, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( mkherm_unb_var1 )
INSERT_GENTPROT_BASIC0( mksymm_unb_var1 )
INSERT_GENTPROT_BASIC0( mktrim_unb_var1 )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       dim_t    n, \
       ctype*   x, inc_t incx, \
       ctype_r* norm, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( norm1v_unb_var1 )
INSERT_GENTPROTR_BASIC0( normfv_unb_var1 )
INSERT_GENTPROTR_BASIC0( normiv_unb_var1 )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       doff_t   diagoffx, \
       diag_t   diagx, \
       uplo_t   uplox, \
       dim_t    m, \
       dim_t    n, \
       ctype*   x, inc_t rs_x, inc_t cs_x, \
       ctype_r* norm, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( norm1m_unb_var1 )
INSERT_GENTPROTR_BASIC0( normfm_unb_var1 )
INSERT_GENTPROTR_BASIC0( normim_unb_var1 )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       FILE*  file, \
       char*  s1, \
       dim_t  n, \
       ctype* x, inc_t incx, \
       char*  format, \
       char*  s2  \
     );

INSERT_GENTPROT_BASIC0_I( fprintv )


#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname) \
     ( \
       FILE*  file, \
       char*  s1, \
       dim_t  m, \
       dim_t  n, \
       ctype* x, inc_t rs_x, inc_t cs_x, \
       char*  format, \
       char*  s2  \
     );

INSERT_GENTPROT_BASIC0_I( fprintm )


#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       dim_t   n, \
       ctype*  x, inc_t incx, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( randv_unb_var1 )
INSERT_GENTPROT_BASIC0( randnv_unb_var1 )


#undef  GENTPROT
#define GENTPROT( ctype, ch, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       doff_t  diagoffx, \
       uplo_t  uplox, \
       dim_t   m, \
       dim_t   n, \
       ctype*  x, inc_t rs_x, inc_t cs_x, \
       cntx_t* cntx  \
     );

INSERT_GENTPROT_BASIC0( randm_unb_var1 )
INSERT_GENTPROT_BASIC0( randnm_unb_var1 )


#undef  GENTPROTR
#define GENTPROTR( ctype, ctype_r, ch, chr, varname ) \
\
void PASTEMAC(ch,varname) \
     ( \
       dim_t    n, \
       ctype*   x, inc_t incx, \
       ctype_r* scale, \
       ctype_r* sumsq, \
       cntx_t*  cntx  \
     );

INSERT_GENTPROTR_BASIC0( sumsqv_unb_var1 )

// end bli_util_unb_var1.h

// end bli_util.h


// -- BLAS compatibility layer --

// begin bli_blas.h


#ifdef BLIS_ENABLE_CBLAS

// If the BLAS compatibility layer was not explicitly enabled, we must
// enable it here.
#ifndef BLIS_ENABLE_BLAS2BLIS
#define BLIS_ENABLE_BLAS2BLIS
#endif

#endif // BLIS_ENABLE_CBLAS

#ifdef BLIS_ENABLE_BLAS2BLIS


// -- System headers needed by BLAS compatibility layer --

#include <ctype.h> // skipped


// -- Constants --

#define BLIS_MAX_BLAS_FUNC_STR_LENGTH (7+1)


// -- Utility macros --

// begin bla_r_sign.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_r_sign(const bla_real *a, const bla_real *b);

#endif

// end bla_r_sign.h
// begin bla_d_sign.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_d_sign(const bla_double *a, const bla_double *b);

#endif

// end bla_d_sign.h

// begin bla_r_cnjg.h


#ifdef BLIS_ENABLE_BLAS2BLIS

void bla_r_cnjg(bla_scomplex *dest, const bla_scomplex *src);

#endif

// end bla_r_cnjg.h
// begin bla_d_cnjg.h


#ifdef BLIS_ENABLE_BLAS2BLIS

void bla_d_cnjg(bla_dcomplex *dest, const bla_dcomplex *src);

#endif

// end bla_d_cnjg.h

// begin bla_r_imag.h


#ifdef BLIS_ENABLE_BLAS2BLIS

bla_real bla_r_imag(const bla_scomplex *z);

#endif

// end bla_r_imag.h
// begin bla_d_imag.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_d_imag(const bla_dcomplex *z);

#endif

// end bla_d_imag.h

// begin bla_c_div.h


#ifdef BLIS_ENABLE_BLAS2BLIS

void bla_c_div(bla_scomplex *cp, const bla_scomplex *ap, const bla_scomplex *bp);

#endif

// end bla_c_div.h
// begin bla_z_div.h


#ifdef BLIS_ENABLE_BLAS2BLIS

void bla_z_div(bla_dcomplex *cp, const bla_dcomplex *ap, const bla_dcomplex *bp);

#endif

// end bla_z_div.h

// begin bla_f__cabs.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_f__cabs(double real, double imag);

#endif

// end bla_f__cabs.h
// begin bla_r_abs.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_r_abs(const bla_real *x);

#endif

// end bla_r_abs.h
// begin bla_d_abs.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_d_abs(const bla_double *x);

#endif

// end bla_d_abs.h
// begin bla_c_abs.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_c_abs(const bla_scomplex *z);

#endif

// end bla_c_abs.h
// begin bla_z_abs.h


#ifdef BLIS_ENABLE_BLAS2BLIS

double bla_z_abs(const bla_dcomplex *z);

#endif

// end bla_z_abs.h

// begin bla_lsame.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#ifdef LAPACK_ILP64
long PASTEF770(lsame)(const char *ca, const char *cb, long ca_len, long cb_len);
#else
int PASTEF770(lsame)(const char *ca, const char *cb, int ca_len, int cb_len);
#endif

#endif
// end bla_lsame.h
// begin bla_xerbla.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF770(xerbla)(const bla_character *srname, const bla_integer *info, ftnlen srname_len);

#endif
// end bla_xerbla.h


// -- Level-0 BLAS prototypes --

// begin bla_cabs1.h


#ifdef BLIS_ENABLE_BLAS2BLIS

bla_real   PASTEF77(s,cabs1)(bla_scomplex *z);
bla_double PASTEF77(d,cabs1)(bla_dcomplex *z);

#endif
// end bla_cabs1.h


// -- Level-1 BLAS prototypes --

// begin bla_amax.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype_x, chx, blasname ) \
\
f77_int PASTEF772(i,chx,blasname) \
     ( \
       const f77_int* n, \
       const ftype_x* x, const f77_int* incx  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( amax )
#endif

// end bla_amax.h
// begin bla_asum.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTR2
#define GENTPROTR2( ftype_x, ftype_r, chx, chr, blasname ) \
\
ftype_r PASTEF772(chr,chx,blasname) \
     ( \
       const f77_int* n, \
       const ftype_x* x, const f77_int* incx  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTR2_BLAS( asum )
#endif

// end bla_asum.h
// begin bla_axpy.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_int* n, \
       const ftype*   alpha, \
       const ftype*   x, const f77_int* incx, \
             ftype*   y, const f77_int* incy \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( axpy )
#endif

// end bla_axpy.h
// begin bla_copy.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_int* n, \
       const ftype*   x, const f77_int* incx, \
             ftype*   y, const f77_int* incy \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( copy )
#endif

// end bla_copy.h
// begin bla_dot.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTDOT
#define GENTPROTDOT( ftype, ch, chc, blasname ) \
\
ftype PASTEF772(ch,blasname,chc) \
     ( \
       const f77_int* n, \
       const ftype*   x, const f77_int* incx, \
       const ftype*   y, const f77_int* incy  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTDOT_BLAS( dot )


// -- "Black sheep" dot product function prototypes --

float PASTEF77(sd,sdot)
     (
       const f77_int* n,
       const float*   sb,
       const float*   x, const f77_int* incx,
       const float*   y, const f77_int* incy
     );

double PASTEF77(d,sdot)
     (
         const f77_int* n,
         const float*   x, const f77_int* incx,
         const float*   y, const f77_int* incy
     );
#endif
// end bla_dot.h
// begin bla_nrm2.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTR2
#define GENTPROTR2( ftype_x, ftype_r, chx, chr, blasname ) \
\
ftype_r PASTEF772(chr,chx,blasname) \
     ( \
       const f77_int* n, \
       const ftype_x* x, const f77_int* incx  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTR2_BLAS( nrm2 )
#endif

// end bla_nrm2.h
// begin bla_rot.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(s,rot)(const bla_integer *n, bla_real *sx, const bla_integer *incx, bla_real *sy, const bla_integer *incy, const bla_real *c__, const bla_real *s);
int PASTEF77(d,rot)(const bla_integer *n, bla_double *dx, const bla_integer *incx, bla_double *dy, const bla_integer *incy, const bla_double *c__, const bla_double *s);
int PASTEF77(cs,rot)(const bla_integer *n, bla_scomplex *cx, const bla_integer *incx, bla_scomplex *cy, const bla_integer *incy, const bla_real *c__, const bla_real *s);
int PASTEF77(zd,rot)(const bla_integer *n, bla_dcomplex *zx, const bla_integer *incx, bla_dcomplex *zy, const bla_integer *incy, const bla_double *c__, const bla_double *s);

#endif
// end bla_rot.h
// begin bla_rotg.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(s,rotg)(bla_real *sa, bla_real *sb, bla_real *c__, bla_real *s);
int PASTEF77(d,rotg)(bla_double *da, bla_double *db, bla_double *c__, bla_double *s);
int PASTEF77(c,rotg)(bla_scomplex *ca, bla_scomplex *cb, bla_real *c__, bla_scomplex *s);
int PASTEF77(z,rotg)(bla_dcomplex *ca, bla_dcomplex *cb, bla_double *c__, bla_dcomplex *s);

#endif
// end bla_rotg.h
// begin bla_rotm.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(s,rotm)(const bla_integer *n, bla_real *sx, const bla_integer *incx, bla_real *sy, const bla_integer *incy, const bla_real *sparam);
int PASTEF77(d,rotm)(const bla_integer *n, bla_double *dx, const bla_integer *incx, bla_double *dy, const bla_integer *incy, const bla_double *dparam);

#endif
// end bla_rotm.h
// begin bla_rotmg.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(s,rotmg)(bla_real *sd1, bla_real *sd2, bla_real *sx1, const bla_real *sy1, bla_real *sparam);
int PASTEF77(d,rotmg)(bla_double *dd1, bla_double *dd2, bla_double *dx1, const bla_double *dy1, bla_double *dparam);

#endif
// end bla_rotmg.h
// begin bla_scal.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTSCAL
#define GENTPROTSCAL( ftype_a, ftype_x, cha, chx, blasname ) \
\
void PASTEF772(chx,cha,blasname) \
     ( \
       const f77_int* n, \
       const ftype_a* alpha, \
       ftype_x* x, const f77_int* incx  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTSCAL_BLAS( scal )
#endif

// end bla_scal.h
// begin bla_swap.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_int* n, \
       ftype*   x, const f77_int* incx, \
       ftype*   y, const f77_int* incy \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( swap )
#endif

// end bla_swap.h

// begin f77_amax_sub.h



//
// Prototype CBLAS subroutine wrapper interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype_x, chx, blasname ) \
\
void PASTEF773(i,chx,blasname,sub) \
     ( \
       const f77_int* n, \
       const ftype_x* x, const f77_int* incx, \
             f77_int* rval  \
     );

#ifdef BLIS_ENABLE_CBLAS
INSERT_GENTPROT_BLAS( amax )
#endif
// end f77_amax_sub.h
// begin f77_asum_sub.h



//
// Prototype CBLAS subroutine wrapper interfaces.
//
#undef  GENTPROTR2
#define GENTPROTR2( ftype_x, ftype_r, chx, chr, blasname ) \
\
void PASTEF773(chr,chx,blasname,sub) \
     ( \
       const f77_int* n, \
       const ftype_x* x, const f77_int* incx, \
             ftype_r* rval  \
     );

#ifdef BLIS_ENABLE_CBLAS
INSERT_GENTPROTR2_BLAS( asum )
#endif
// end f77_asum_sub.h
// begin f77_dot_sub.h



//
// Prototype CBLAS subroutine wrapper interfaces.
//
#undef  GENTPROTDOT
#define GENTPROTDOT( ftype, ch, chc, blasname ) \
\
void PASTEF773(ch,blasname,chc,sub) \
     ( \
       const f77_int* n, \
       const ftype*   x, const f77_int* incx, \
       const ftype*   y, const f77_int* incy, \
             ftype*   rval  \
     );

#ifdef BLIS_ENABLE_CBLAS
INSERT_GENTPROTDOT_BLAS( dot )


// -- "Black sheep" dot product function prototypes --

void PASTEF772(sds,dot,sub)
     (
       const f77_int* n,
       const float*  sb,
       const float*   x, const f77_int* incx,
       const float*   y, const f77_int* incy,
             float*   rval
     );

void PASTEF772(ds,dot,sub)
     (
       const f77_int* n,
       const float*   x, const f77_int* incx,
       const float*   y, const f77_int* incy,
             double*  rval
     );
#endif
// end f77_dot_sub.h
// begin f77_nrm2_sub.h



//
// Prototype CBLAS subroutine wrapper interfaces.
//
#undef  GENTPROTR2
#define GENTPROTR2( ftype_x, ftype_r, chx, chr, blasname ) \
\
void PASTEF773(chr,chx,blasname,sub) \
     ( \
       const f77_int* n, \
       const ftype_x* x, const f77_int* incx, \
             ftype_r* rval  \
     );

#ifdef BLIS_ENABLE_CBLAS
INSERT_GENTPROTR2_BLAS( nrm2 )
#endif
// end f77_nrm2_sub.h


// -- Level-2 BLAS prototypes --

// dense

// begin bla_gemv.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* transa, \
       const f77_int*  m, \
       const f77_int*  n, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    x, const f77_int* incx, \
       const ftype*    beta, \
             ftype*    y, const f77_int* incy  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( gemv )
#endif

// end bla_gemv.h
// begin bla_ger.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTDOT
#define GENTPROTDOT( ftype, chxy, chc, blasname ) \
\
void PASTEF772(chxy,blasname,chc) \
     ( \
       const f77_int* m, \
       const f77_int* n, \
       const ftype*   alpha, \
       const ftype*   x, const f77_int* incx, \
       const ftype*   y, const f77_int* incy, \
             ftype*   a, const f77_int* lda  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTDOT_BLAS( ger )
#endif

// end bla_ger.h
// begin bla_hemv.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTCO
#define GENTPROTCO( ftype, ftype_r, ch, chr, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_int*  m, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    x, const f77_int* incx, \
       const ftype*    beta, \
             ftype*    y, const f77_int* incy  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTCO_BLAS( hemv )
#endif

// end bla_hemv.h
// begin bla_her.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTCO
#define GENTPROTCO( ftype, ftype_r, ch, chr, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_int*  m, \
       const ftype_r*  alpha, \
       const ftype*    x, const f77_int* incx, \
             ftype*    a, const f77_int* lda  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTCO_BLAS( her )
#endif

// end bla_her.h
// begin bla_her2.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTCO
#define GENTPROTCO( ftype, ftype_r, ch, chr, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_int*  m, \
       const ftype*    alpha, \
       const ftype*    x, const f77_int* incx, \
       const ftype*    y, const f77_int* incy, \
             ftype*    a, const f77_int* lda  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTCO_BLAS( her2 )
#endif

// end bla_her2.h
// begin bla_symv.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTRO
#define GENTPROTRO( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_int*  m, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    x, const f77_int* incx, \
       const ftype*    beta, \
             ftype*    y, const f77_int* incy  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTRO_BLAS( symv )
#endif

// end bla_symv.h
// begin bla_syr.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTRO
#define GENTPROTRO( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_int*  m, \
       const ftype*    alpha, \
       const ftype*    x, const f77_int* incx, \
             ftype*    a, const f77_int* lda  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTRO_BLAS( syr )
#endif

// end bla_syr.h
// begin bla_syr2.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTRO
#define GENTPROTRO( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_int*  m, \
       const ftype*    alpha, \
       const ftype*    x, const f77_int* incx, \
       const ftype*    y, const f77_int* incy, \
             ftype*    a, const f77_int* lda  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTRO_BLAS( syr2 )
#endif

// end bla_syr2.h
// begin bla_trmv.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_char* transa, \
       const f77_char* diaga, \
       const f77_int*  m, \
       const ftype*    a, const f77_int* lda, \
             ftype*    x, const f77_int* incx  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( trmv )
#endif

// end bla_trmv.h
// begin bla_trsv.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploa, \
       const f77_char* transa, \
       const f77_char* diaga, \
       const f77_int*  m, \
       const ftype*    a, const f77_int* lda, \
             ftype*    x, const f77_int* incx  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( trsv )
#endif

// end bla_trsv.h

// begin bla_gemv_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_gemv_check( dt_str, op_str, transa, m, n, lda, incx, incy ) \
{ \
	f77_int info = 0; \
	f77_int nota, ta, conja; \
\
	nota  = PASTEF770(lsame)( transa, "N", (ftnlen)1, (ftnlen)1 ); \
	ta    = PASTEF770(lsame)( transa, "T", (ftnlen)1, (ftnlen)1 ); \
	conja = PASTEF770(lsame)( transa, "C", (ftnlen)1, (ftnlen)1 ); \
\
	if      ( !nota && !ta && !conja ) \
		info = 1; \
	else if ( *m < 0 ) \
		info = 2; \
	else if ( *n < 0 ) \
		info = 3; \
	else if ( *lda < bli_max( 1, *m ) ) \
		info = 6; \
	else if ( *incx == 0 ) \
		info = 8; \
	else if ( *incy == 0 ) \
		info = 11; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_gemv_check.h
// begin bla_ger_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_ger_check( dt_str, op_str, conj_str, m, n, incx, incy, lda ) \
{ \
	f77_int info = 0; \
\
	if      ( *m < 0 ) \
		info = 1; \
	else if ( *n < 0 ) \
		info = 2; \
	else if ( *incx == 0 ) \
		info = 5; \
	else if ( *incy == 0 ) \
		info = 7; \
	else if ( *lda < bli_max( 1, *m ) ) \
		info = 9; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		 \
		sprintf( func_str, "%s%s%-2s", dt_str, op_str, conj_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_ger_check.h
// begin bla_hemv_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_hemv_check( dt_str, op_str, uploa, m, lda, incx, incy ) \
{ \
	f77_int info = 0; \
	f77_int lower, upper; \
\
	lower = PASTEF770(lsame)( uploa, "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa, "U", (ftnlen)1, (ftnlen)1 ); \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( *m < 0 ) \
		info = 2; \
	else if ( *lda < bli_max( 1, *m ) ) \
		info = 5; \
	else if ( *incx == 0 ) \
		info = 7; \
	else if ( *incy == 0 ) \
		info = 10; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_hemv_check.h
// begin bla_her_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_her_check( dt_str, op_str, uploa, m, incx, lda ) \
{ \
	f77_int info = 0; \
	f77_int lower, upper; \
\
	lower = PASTEF770(lsame)( uploa, "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa, "U", (ftnlen)1, (ftnlen)1 ); \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( *m < 0 ) \
		info = 2; \
	else if ( *incx == 0 ) \
		info = 5; \
	else if ( *lda < bli_max( 1, *m ) ) \
		info = 7; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_her_check.h
// begin bla_her2_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_her2_check( dt_str, op_str, uploa, m, incx, incy, lda ) \
{ \
	f77_int info = 0; \
	f77_int lower, upper; \
\
	lower = PASTEF770(lsame)( uploa, "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa, "U", (ftnlen)1, (ftnlen)1 ); \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( *m < 0 ) \
		info = 2; \
	else if ( *incx == 0 ) \
		info = 5; \
	else if ( *incy == 0 ) \
		info = 7; \
	else if ( *lda < bli_max( 1, *m ) ) \
		info = 9; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_her2_check.h
// begin bla_symv_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_symv_check bla_hemv_check

#endif
// end bla_symv_check.h
// begin bla_syr_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_syr_check bla_her_check

#endif
// end bla_syr_check.h
// begin bla_syr2_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_syr2_check bla_her2_check

#endif
// end bla_syr2_check.h
// begin bla_trmv_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_trmv_check( dt_str, op_str, uploa, transa, diaga, m, lda, incx ) \
{ \
	f77_int info = 0; \
	f77_int lower, upper; \
	f77_int nota, ta, conja; \
	f77_int unita, nonua; \
\
	lower = PASTEF770(lsame)( uploa,  "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa,  "U", (ftnlen)1, (ftnlen)1 ); \
	nota  = PASTEF770(lsame)( transa, "N", (ftnlen)1, (ftnlen)1 ); \
	ta    = PASTEF770(lsame)( transa, "T", (ftnlen)1, (ftnlen)1 ); \
	conja = PASTEF770(lsame)( transa, "C", (ftnlen)1, (ftnlen)1 ); \
	unita = PASTEF770(lsame)( diaga,  "U", (ftnlen)1, (ftnlen)1 ); \
	nonua = PASTEF770(lsame)( diaga,  "N", (ftnlen)1, (ftnlen)1 ); \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( !nota && !ta && !conja ) \
		info = 2; \
	else if ( !unita && !nonua ) \
		info = 3; \
	else if ( *m < 0 ) \
		info = 4; \
	else if ( *lda < bli_max( 1, *m ) ) \
		info = 6; \
	else if ( *incx == 0 ) \
		info = 8; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_trmv_check.h
// begin bla_trsv_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_trsv_check bla_trmv_check

#endif
// end bla_trsv_check.h

// packed

// begin bla_hpmv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,hpmv)(const bla_character *uplo, const bla_integer *n, const bla_scomplex *alpha, const bla_scomplex *ap, const bla_scomplex *x, const bla_integer *incx, const bla_scomplex *beta, bla_scomplex *y, const bla_integer *incy);
int PASTEF77(z,hpmv)(const bla_character *uplo, const bla_integer *n, const bla_dcomplex *alpha, const bla_dcomplex *ap, const bla_dcomplex *x, const bla_integer *incx, const bla_dcomplex *beta, bla_dcomplex *y, const bla_integer *incy);

#endif
// end bla_hpmv.h
// begin bla_hpr.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,hpr)(const bla_character *uplo, const bla_integer *n, const bla_real *alpha, const bla_scomplex *x, const bla_integer *incx, bla_scomplex *ap);
int PASTEF77(z,hpr)(const bla_character *uplo, const bla_integer *n, const bla_double *alpha, const bla_dcomplex *x, const bla_integer *incx, bla_dcomplex *ap);

#endif
// end bla_hpr.h
// begin bla_hpr2.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,hpr2)(const bla_character *uplo, const bla_integer *n, const bla_scomplex *alpha, const bla_scomplex *x, const bla_integer *incx, const bla_scomplex *y, const bla_integer *incy, bla_scomplex *ap);
int PASTEF77(z,hpr2)(const bla_character *uplo, const bla_integer *n, const bla_dcomplex *alpha, const bla_dcomplex *x, const bla_integer *incx, const bla_dcomplex *y, const bla_integer *incy, bla_dcomplex *ap);

#endif
// end bla_hpr2.h
// begin bla_spmv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(d,spmv)(const bla_character *uplo, const bla_integer *n, const bla_double *alpha, const bla_double *ap, const bla_double *x, const bla_integer *incx, const bla_double *beta, bla_double *y, const bla_integer *incy);
int PASTEF77(s,spmv)(const bla_character *uplo, const bla_integer *n, const bla_real *alpha, const bla_real *ap, const bla_real *x, const bla_integer *incx, const bla_real *beta, bla_real *y, const bla_integer *incy);

#endif
// end bla_spmv.h
// begin bla_spr.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(d,spr)(const bla_character *uplo, const bla_integer *n, const bla_double *alpha, const bla_double *x, const bla_integer *incx, bla_double *ap);
int PASTEF77(s,spr)(const bla_character *uplo, const bla_integer *n, const bla_real *alpha, const bla_real *x, const bla_integer *incx, bla_real *ap);

#endif
// end bla_spr.h
// begin bla_spr2.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(d,spr2)(const bla_character *uplo, const bla_integer *n, const bla_double *alpha, const bla_double *x, const bla_integer *incx, const bla_double *y, const bla_integer *incy, bla_double *ap);
int PASTEF77(s,spr2)(const bla_character *uplo, const bla_integer *n, const bla_real *alpha, const bla_real *x, const bla_integer *incx, const bla_real *y, const bla_integer *incy, bla_real *ap);

#endif
// end bla_spr2.h
// begin bla_tpmv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,tpmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_scomplex *ap, bla_scomplex *x, const bla_integer *incx);
int PASTEF77(d,tpmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_double *ap, bla_double *x, const bla_integer *incx);
int PASTEF77(s,tpmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_real *ap, bla_real *x, const bla_integer *incx);
int PASTEF77(z,tpmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_dcomplex *ap, bla_dcomplex *x, const bla_integer *incx);

#endif
// end bla_tpmv.h
// begin bla_tpsv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,tpsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_scomplex *ap, bla_scomplex *x, const bla_integer *incx);
int PASTEF77(d,tpsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_double *ap, bla_double *x, const bla_integer *incx);
int PASTEF77(s,tpsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_real *ap, bla_real *x, const bla_integer *incx);
int PASTEF77(z,tpsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_dcomplex *ap, bla_dcomplex *x, const bla_integer *incx);

#endif
// end bla_tpsv.h

// banded

// begin bla_gbmv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,gbmv)(const bla_character *trans, const bla_integer *m, const bla_integer *n, const bla_integer *kl, const bla_integer *ku, const bla_scomplex *alpha, const bla_scomplex *a, const bla_integer *lda, const bla_scomplex *x, const bla_integer *incx, const bla_scomplex *beta, bla_scomplex *y, const bla_integer *incy);
int PASTEF77(d,gbmv)(const bla_character *trans, const bla_integer *m, const bla_integer *n, const bla_integer *kl, const bla_integer *ku, const bla_double *alpha, const bla_double *a, const bla_integer *lda, const bla_double *x, const bla_integer *incx, const bla_double *beta, bla_double *y, const bla_integer *incy);
int PASTEF77(s,gbmv)(const bla_character *trans, const bla_integer *m, const bla_integer *n, const bla_integer *kl, const bla_integer *ku, const bla_real *alpha, const bla_real *a, const bla_integer *lda, const bla_real *x, const bla_integer * incx, const bla_real *beta, bla_real *y, const bla_integer *incy);
int PASTEF77(z,gbmv)(const bla_character *trans, const bla_integer *m, const bla_integer *n, const bla_integer *kl, const bla_integer *ku, const bla_dcomplex *alpha, const bla_dcomplex *a, const bla_integer *lda, const bla_dcomplex *x, const bla_integer *incx, const bla_dcomplex *beta, bla_dcomplex * y, const bla_integer *incy);

#endif
// end bla_gbmv.h
// begin bla_hbmv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,hbmv)(const bla_character *uplo, const bla_integer *n, const bla_integer *k, const bla_scomplex *alpha, const bla_scomplex *a, const bla_integer *lda, const bla_scomplex *x, const bla_integer *incx, const bla_scomplex *beta, bla_scomplex *y, const bla_integer *incy);
int PASTEF77(z,hbmv)(const bla_character *uplo, const bla_integer *n, const bla_integer *k, const bla_dcomplex *alpha, const bla_dcomplex *a, const bla_integer *lda, const bla_dcomplex *x, const bla_integer *incx, const bla_dcomplex *beta, bla_dcomplex *y, const bla_integer *incy);

#endif
// end bla_hbmv.h
// begin bla_sbmv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(d,sbmv)(const bla_character *uplo, const bla_integer *n, const bla_integer *k, const bla_double *alpha, const bla_double *a, const bla_integer *lda, const bla_double *x, const bla_integer *incx, const bla_double *beta, bla_double *y, const bla_integer *incy);
int PASTEF77(s,sbmv)(const bla_character *uplo, const bla_integer *n, const bla_integer *k, const bla_real *alpha, const bla_real *a, const bla_integer *lda, const bla_real *x, const bla_integer *incx, const bla_real *beta, bla_real *y, const bla_integer *incy);

#endif
// end bla_sbmv.h
// begin bla_tbmv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,tbmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_scomplex *a, const bla_integer *lda, bla_scomplex *x, const bla_integer *incx);
int PASTEF77(d,tbmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_double *a, const bla_integer *lda, bla_double *x, const bla_integer *incx);
int PASTEF77(s,tbmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_real *a, const bla_integer *lda, bla_real *x, const bla_integer *incx);
int PASTEF77(z,tbmv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_dcomplex *a, const bla_integer *lda, bla_dcomplex *x, const bla_integer *incx);

#endif
// end bla_tbmv.h
// begin bla_tbsv.h


#ifdef BLIS_ENABLE_BLAS2BLIS

int PASTEF77(c,tbsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_scomplex *a, const bla_integer *lda, bla_scomplex *x, const bla_integer *incx);
int PASTEF77(d,tbsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_double *a, const bla_integer *lda, bla_double *x, const bla_integer *incx);
int PASTEF77(s,tbsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_real *a, const bla_integer *lda, bla_real *x, const bla_integer *incx);
int PASTEF77(z,tbsv)(const bla_character *uplo, const bla_character *trans, const bla_character *diag, const bla_integer *n, const bla_integer *k, const bla_dcomplex *a, const bla_integer *lda, bla_dcomplex *x, const bla_integer *incx);

#endif
// end bla_tbsv.h


// -- Level-3 BLAS prototypes --

// begin bla_gemm.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* transa, \
       const f77_char* transb, \
       const f77_int*  m, \
       const f77_int*  n, \
       const f77_int*  k, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    b, const f77_int* ldb, \
       const ftype*    beta, \
             ftype*    c, const f77_int* ldc  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( gemm )
#endif

// end bla_gemm.h
// begin bla_hemm.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTCO
#define GENTPROTCO( ftype, ftype_r, ch, chr, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* side, \
       const f77_char* uploa, \
       const f77_int*  m, \
       const f77_int*  n, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    b, const f77_int* ldb, \
       const ftype*    beta, \
             ftype*    c, const f77_int* ldc  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTCO_BLAS( hemm )
#endif

// end bla_hemm.h
// begin bla_herk.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTCO
#define GENTPROTCO( ftype, ftype_r, ch, chr, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploc, \
       const f77_char* transa, \
       const f77_int*  m, \
       const f77_int*  k, \
       const ftype_r*  alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype_r*  beta, \
             ftype*    c, const f77_int* ldc  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTCO_BLAS( herk )
#endif

// end bla_herk.h
// begin bla_her2k.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROTCO
#define GENTPROTCO( ftype, ftype_r, ch, chr, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploc, \
       const f77_char* transa, \
       const f77_int*  m, \
       const f77_int*  k, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    b, const f77_int* ldb, \
       const ftype_r*  beta, \
             ftype*    c, const f77_int* ldc  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROTCO_BLAS( her2k )
#endif

// end bla_her2k.h
// begin bla_symm.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* side, \
       const f77_char* uploa, \
       const f77_int*  m, \
       const f77_int*  n, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    b, const f77_int* ldb, \
       const ftype*    beta, \
             ftype*    c, const f77_int* ldc  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( symm )
#endif

// end bla_symm.h
// begin bla_syrk.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploc, \
       const f77_char* transa, \
       const f77_int*  m, \
       const f77_int*  k, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    beta, \
             ftype*    c, const f77_int* ldc  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( syrk )
#endif

// end bla_syrk.h
// begin bla_syr2k.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* uploc, \
       const f77_char* transa, \
       const f77_int*  m, \
       const f77_int*  k, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
       const ftype*    b, const f77_int* ldb, \
       const ftype*    beta, \
             ftype*    c, const f77_int* ldc  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( syr2k )
#endif

// end bla_syr2k.h
// begin bla_trmm.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* side, \
       const f77_char* uploa, \
       const f77_char* transa, \
       const f77_char* diaga, \
       const f77_int*  m, \
       const f77_int*  n, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
             ftype*    b, const f77_int* ldb  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( trmm )
#endif

// end bla_trmm.h
// begin bla_trsm.h



//
// Prototype BLAS-to-BLIS interfaces.
//
#undef  GENTPROT
#define GENTPROT( ftype, ch, blasname ) \
\
void PASTEF77(ch,blasname) \
     ( \
       const f77_char* side, \
       const f77_char* uploa, \
       const f77_char* transa, \
       const f77_char* diaga, \
       const f77_int*  m, \
       const f77_int*  n, \
       const ftype*    alpha, \
       const ftype*    a, const f77_int* lda, \
             ftype*    b, const f77_int* ldb  \
     );

#ifdef BLIS_ENABLE_BLAS2BLIS
INSERT_GENTPROT_BLAS( trsm )
#endif

// end bla_trsm.h

// begin bla_gemm_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_gemm_check( dt_str, op_str, transa, transb, m, n, k, lda, ldb, ldc ) \
{ \
	f77_int info = 0; \
	f77_int nota,  notb; \
	f77_int conja, conjb; \
	f77_int ta,    tb; \
	f77_int nrowa, nrowb; \
\
	nota  = PASTEF770(lsame)( transa, "N", (ftnlen)1, (ftnlen)1 ); \
	notb  = PASTEF770(lsame)( transb, "N", (ftnlen)1, (ftnlen)1 ); \
	conja = PASTEF770(lsame)( transa, "C", (ftnlen)1, (ftnlen)1 ); \
	conjb = PASTEF770(lsame)( transb, "C", (ftnlen)1, (ftnlen)1 ); \
	ta    = PASTEF770(lsame)( transa, "T", (ftnlen)1, (ftnlen)1 ); \
	tb    = PASTEF770(lsame)( transb, "T", (ftnlen)1, (ftnlen)1 ); \
\
	if ( nota ) { nrowa = *m; } \
	else        { nrowa = *k; } \
	if ( notb ) { nrowb = *k; } \
	else        { nrowb = *n; } \
\
	if      ( !nota && !conja && !ta ) \
		info = 1; \
	else if ( !notb && !conjb && !tb ) \
		info = 2; \
	else if ( *m < 0 ) \
		info = 3; \
	else if ( *n < 0 ) \
		info = 4; \
	else if ( *k < 0 ) \
		info = 5; \
	else if ( *lda < bli_max( 1, nrowa ) ) \
		info = 8; \
	else if ( *ldb < bli_max( 1, nrowb ) ) \
		info = 10; \
	else if ( *ldc < bli_max( 1, *m    ) ) \
		info = 13; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_gemm_check.h
// begin bla_hemm_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_hemm_check( dt_str, op_str, sidea, uploa, m, n, lda, ldb, ldc ) \
{ \
	f77_int info = 0; \
	f77_int left, right; \
	f77_int lower, upper; \
	f77_int nrowa; \
\
	left  = PASTEF770(lsame)( sidea, "L", (ftnlen)1, (ftnlen)1 ); \
	right = PASTEF770(lsame)( sidea, "R", (ftnlen)1, (ftnlen)1 ); \
	lower = PASTEF770(lsame)( uploa, "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa, "U", (ftnlen)1, (ftnlen)1 ); \
\
	if ( left ) { nrowa = *m; } \
	else        { nrowa = *n; } \
\
	if      ( !left && !right ) \
		info = 1; \
	else if ( !lower && !upper ) \
		info = 2; \
	else if ( *m < 0 ) \
		info = 3; \
	else if ( *n < 0 ) \
		info = 4; \
	else if ( *lda < bli_max( 1, nrowa ) ) \
		info = 7; \
	else if ( *ldb < bli_max( 1, *m    ) ) \
		info = 9; \
	else if ( *ldc < bli_max( 1, *m    ) ) \
		info = 12; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_hemm_check.h
// begin bla_herk_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_herk_check( dt_str, op_str, uploa, transa, m, k, lda, ldc ) \
{ \
	f77_int info = 0; \
	f77_int nota, conja; \
	f77_int lower, upper; \
	f77_int nrowa; \
\
	nota  = PASTEF770(lsame)( transa, "N", (ftnlen)1, (ftnlen)1 ); \
	conja = PASTEF770(lsame)( transa, "C", (ftnlen)1, (ftnlen)1 ); \
	lower = PASTEF770(lsame)( uploc,  "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploc,  "U", (ftnlen)1, (ftnlen)1 ); \
\
	if ( nota ) { nrowa = *m; } \
	else        { nrowa = *k; } \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( !nota && !conja ) \
		info = 2; \
	else if ( *m < 0 ) \
		info = 3; \
	else if ( *k < 0 ) \
		info = 4; \
	else if ( *lda < bli_max( 1, nrowa ) ) \
		info = 7; \
	else if ( *ldc < bli_max( 1, *m    ) ) \
		info = 10; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_herk_check.h
// begin bla_her2k_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_her2k_check( dt_str, op_str, uploa, trans, m, k, lda, ldb, ldc ) \
{ \
	f77_int info = 0; \
	f77_int nota, conja; \
	f77_int lower, upper; \
	f77_int nrowa; \
\
	nota  = PASTEF770(lsame)( trans, "N", (ftnlen)1, (ftnlen)1 ); \
	conja = PASTEF770(lsame)( trans, "C", (ftnlen)1, (ftnlen)1 ); \
	lower = PASTEF770(lsame)( uploa, "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa, "U", (ftnlen)1, (ftnlen)1 ); \
\
	if ( nota ) { nrowa = *m; } \
	else        { nrowa = *k; } \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( !nota && !conja ) \
		info = 2; \
	else if ( *m < 0 ) \
		info = 3; \
	else if ( *k < 0 ) \
		info = 4; \
	else if ( *lda < bli_max( 1, nrowa ) ) \
		info = 7; \
	else if ( *ldb < bli_max( 1, nrowa ) ) \
		info = 9; \
	else if ( *ldc < bli_max( 1, *m    ) ) \
		info = 12; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_her2k_check.h
// begin bla_symm_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_symm_check bla_hemm_check

#endif
// end bla_symm_check.h
// begin bla_syrk_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_syrk_check( dt_str, op_str, uploa, transa, m, k, lda, ldc ) \
{ \
	f77_int info = 0; \
	f77_int is_r; \
	f77_int nota, ta, cta; \
	f77_int lower, upper; \
	f77_int nrowa; \
\
	static char* dt_cst = dt_str; \
\
	is_r  = ( dt_cst[0] == 's' || dt_cst[0] == 'd' ); \
	nota  = PASTEF770(lsame)( transa, "N", (ftnlen)1, (ftnlen)1 ); \
	ta    = PASTEF770(lsame)( transa, "T", (ftnlen)1, (ftnlen)1 ); \
	cta   = PASTEF770(lsame)( transa, "C", (ftnlen)1, (ftnlen)1 ); \
	lower = PASTEF770(lsame)( uploc,  "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploc,  "U", (ftnlen)1, (ftnlen)1 ); \
\
	if ( nota ) { nrowa = *m; } \
	else        { nrowa = *k; } \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( !nota && !ta && (is_r ? !cta : 1) ) \
		info = 2; \
	else if ( *m < 0 ) \
		info = 3; \
	else if ( *k < 0 ) \
		info = 4; \
	else if ( *lda < bli_max( 1, nrowa ) ) \
		info = 7; \
	else if ( *ldc < bli_max( 1, *m    ) ) \
		info = 10; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_syrk_check.h
// begin bla_syr2k_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_syr2k_check( dt_str, op_str, uploa, trans, m, k, lda, ldb, ldc ) \
{ \
	f77_int info = 0; \
	f77_int is_r; \
	f77_int nota, ta, cta; \
	f77_int lower, upper; \
	f77_int nrowa; \
\
	static char* dt_cst = dt_str; \
\
	is_r  = ( dt_cst[0] == 's' || dt_cst[0] == 'd' ); \
	nota  = PASTEF770(lsame)( trans, "N", (ftnlen)1, (ftnlen)1 ); \
	ta    = PASTEF770(lsame)( trans, "T", (ftnlen)1, (ftnlen)1 ); \
	cta   = PASTEF770(lsame)( trans, "C", (ftnlen)1, (ftnlen)1 ); \
	lower = PASTEF770(lsame)( uploa, "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa, "U", (ftnlen)1, (ftnlen)1 ); \
\
	if ( nota ) { nrowa = *m; } \
	else        { nrowa = *k; } \
\
	if      ( !lower && !upper ) \
		info = 1; \
	else if ( !nota && !ta && (is_r ? !cta : 1) ) \
		info = 2; \
	else if ( *m < 0 ) \
		info = 3; \
	else if ( *k < 0 ) \
		info = 4; \
	else if ( *lda < bli_max( 1, nrowa ) ) \
		info = 7; \
	else if ( *ldb < bli_max( 1, nrowa ) ) \
		info = 9; \
	else if ( *ldc < bli_max( 1, *m    ) ) \
		info = 12; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_syr2k_check.h
// begin bla_trmm_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_trmm_check( dt_str, op_str, sidea, uploa, transa, diaga, m, n, lda, ldb ) \
{ \
	f77_int info = 0; \
	f77_int left, right; \
	f77_int lower, upper; \
	f77_int nota, ta, conja; \
	f77_int unita, nonua; \
	f77_int nrowa; \
\
	left  = PASTEF770(lsame)( sidea,  "L", (ftnlen)1, (ftnlen)1 ); \
	right = PASTEF770(lsame)( sidea,  "R", (ftnlen)1, (ftnlen)1 ); \
	lower = PASTEF770(lsame)( uploa,  "L", (ftnlen)1, (ftnlen)1 ); \
	upper = PASTEF770(lsame)( uploa,  "U", (ftnlen)1, (ftnlen)1 ); \
	nota  = PASTEF770(lsame)( transa, "N", (ftnlen)1, (ftnlen)1 ); \
	ta    = PASTEF770(lsame)( transa, "T", (ftnlen)1, (ftnlen)1 ); \
	conja = PASTEF770(lsame)( transa, "C", (ftnlen)1, (ftnlen)1 ); \
	unita = PASTEF770(lsame)( diaga,  "U", (ftnlen)1, (ftnlen)1 ); \
	nonua = PASTEF770(lsame)( diaga,  "N", (ftnlen)1, (ftnlen)1 ); \
\
	if ( left ) { nrowa = *m; } \
	else        { nrowa = *n; } \
\
	if      ( !left && !right ) \
		info = 1; \
	else if ( !lower && !upper ) \
		info = 2; \
	else if ( !nota && !ta && !conja ) \
		info = 3; \
	else if ( !unita && !nonua ) \
		info = 4; \
	else if ( *m < 0 ) \
		info = 5; \
	else if ( *n < 0 ) \
		info = 6; \
	else if ( *lda < bli_max( 1, nrowa ) ) \
		info = 9; \
	else if ( *ldb < bli_max( 1, *m    ) ) \
		info = 11; \
\
	if ( info != 0 ) \
	{ \
		char func_str[ BLIS_MAX_BLAS_FUNC_STR_LENGTH ]; \
\
		sprintf( func_str, "%s%-5s", dt_str, op_str ); \
\
		bli_string_mkupper( func_str ); \
\
		PASTEF770(xerbla)( func_str, &info, (ftnlen)6 ); \
\
		return; \
	} \
}

#endif
// end bla_trmm_check.h
// begin bla_trsm_check.h


#ifdef BLIS_ENABLE_BLAS2BLIS

#define bla_trsm_check bla_trmm_check

#endif
// end bla_trsm_check.h


#endif // BLIS_ENABLE_BLAS2BLIS
// end bli_blas.h


// -- CBLAS compatibility layer --

// begin bli_cblas.h


#ifndef BLIS_CBLAS_H
#define BLIS_CBLAS_H

#ifdef BLIS_ENABLE_CBLAS


// Undefine these macros so that no internal conversion is done by CBLAS.
// The function signatures have been modified to use the proper integer types
// directly.
#undef F77_INT
#undef F77_CHAR

// Include the main CBLAS header so that including this header file
// (probably via blis.h) allows applications to access CBLAS
// prototypes and definitions.
// begin cblas.h
#ifndef CBLAS_H
#define CBLAS_H
#include <stddef.h> // skipped

// We need to #include "bli_type_defs.h" in order to pull in the
// definition of f77_int. But in order to #include that header, we
// also need to pull in the headers that precede it in blis.h.
// begin bli_system.h


#ifndef BLIS_SYSTEM_H
#define BLIS_SYSTEM_H

#include <stdio.h> // skipped
#include <stdlib.h> // skipped
#include <math.h> // skipped
#include <string.h> // skipped
#include <stdarg.h> // skipped
#include <float.h> // skipped
#include <errno.h> // skipped

// Determine if we are on a 64-bit or 32-bit architecture
#if defined(_M_X64) || defined(__x86_64) || defined(__aarch64__) || \
    defined(_ARCH_PPC64)
#define BLIS_ARCH_64
#else
#define BLIS_ARCH_32
#endif

// Determine the target operating system
#if defined(_WIN32) || defined(__CYGWIN__)
#define BLIS_OS_WINDOWS 1
#elif defined(__APPLE__) || defined(__MACH__)
#define BLIS_OS_OSX 1
#elif defined(__ANDROID__)
#define BLIS_OS_ANDROID 1
#elif defined(__linux__)
#define BLIS_OS_LINUX 1
#elif defined(__bgq__)
#define BLIS_OS_BGQ 1
#elif defined(__bg__)
#define BLIS_OS_BGP 1
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
      defined(__bsdi__) || defined(__DragonFly__)
#define BLIS_OS_BSD 1
#elif defined(EMSCRIPTEN)
#define BLIS_OS_EMSCRIPTEN
#else
#error "Cannot determine operating system"
#endif

// A few changes that may be necessary in Windows environments.
#if BLIS_OS_WINDOWS

  // Include Windows header file.
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
#include <windows.h> // skipped

  // Undefine attribute specifiers in Windows.
  #define __attribute__(x)

  // Undefine restrict.
  #define restrict

#endif

// time.h provides clock_gettime().
#if BLIS_OS_WINDOWS
#include <time.h> // skipped
#elif BLIS_OS_OSX
#include <mach/mach_time.h> // skipped
#else
#include <sys/time.h> // skipped
#include <time.h> // skipped
#endif

// hbwmalloc.h provides hbw_malloc() and hbw_free() on systems with
// libmemkind. But disable use of libmemkind if BLIS_DISABLE_MEMKIND
// was explicitly defined.
#ifdef BLIS_DISABLE_MEMKIND
  #undef BLIS_ENABLE_MEMKIND
#endif
#ifdef BLIS_ENABLE_MEMKIND
#include <hbwmalloc.h> // skipped
#endif


#endif
// end bli_system.h
// begin bli_config.h


#ifndef BLIS_CONFIG_H
#define BLIS_CONFIG_H

// Enabled configuration "family" (config_name)
#define BLIS_FAMILY_KNL


// Enabled sub-configurations (config_list)
#define BLIS_CONFIG_KNL


// Enabled kernel sets (kernel_list)
#define BLIS_KERNELS_KNL
#define BLIS_KERNELS_ZEN


#if 1
#define BLIS_ENABLE_OPENMP
#endif

#if 0
#define BLIS_ENABLE_PTHREADS
#endif

#if 1
#define BLIS_ENABLE_PACKBUF_POOLS
#endif

#if 0 == 64
#define BLIS_INT_TYPE_SIZE 64
#elif 0 == 32
#define BLIS_INT_TYPE_SIZE 32
#else
// determine automatically
#endif

#if 32 == 64
#define BLIS_BLAS2BLIS_INT_TYPE_SIZE 64
#elif 32 == 32
#define BLIS_BLAS2BLIS_INT_TYPE_SIZE 32
#else
// determine automatically
#endif

#ifndef BLIS_ENABLE_BLAS2BLIS
#ifndef BLIS_DISABLE_BLAS2BLIS
#if 1
#define BLIS_ENABLE_BLAS2BLIS
#else
#define BLIS_DISABLE_BLAS2BLIS
#endif
#endif
#endif

#ifndef BLIS_ENABLE_CBLAS
#ifndef BLIS_DISABLE_CBLAS
#if 1
#define BLIS_ENABLE_CBLAS
#else
#define BLIS_DISABLE_CBLAS
#endif
#endif
#endif

#if 1
#define BLIS_ENABLE_MEMKIND
#else
#define BLIS_DISABLE_MEMKIND
#endif

#endif
// end bli_config.h
// begin bli_config_macro_defs.h


#ifndef BLIS_CONFIG_MACRO_DEFS_H
#define BLIS_CONFIG_MACRO_DEFS_H


// -- INTEGER PROPERTIES -------------------------------------------------------

// The bit size of the integer type used to track values such as dimensions,
// strides, diagonal offsets. A value of 32 results in BLIS using 32-bit signed
// integers while 64 results in 64-bit integers. Any other value results in use
// of the C99 type "long int". Note that this ONLY affects integers used
// internally within BLIS as well as those exposed in the native BLAS-like BLIS
// interface.
#ifndef BLIS_INT_TYPE_SIZE
#ifdef BLIS_ARCH_64
#define BLIS_INT_TYPE_SIZE               64
#else
#define BLIS_INT_TYPE_SIZE               32
#endif
#endif


// -- FLOATING-POINT PROPERTIES ------------------------------------------------

// Enable use of built-in C99 "float complex" and "double complex" types and
// associated overloaded operations and functions? Disabling results in
// scomplex and dcomplex being defined in terms of simple structs.
// NOTE: AVOID USING THIS FEATURE. IT IS PROBABLY BROKEN.
#ifdef BLIS_ENABLE_C99_COMPLEX
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif


// -- MULTITHREADING -----------------------------------------------------------

// Enable multithreading via POSIX threads.
#ifdef BLIS_ENABLE_PTHREADS
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif

// Enable multithreading via OpenMP.
#ifdef BLIS_ENABLE_OPENMP
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif

// Perform a sanity check to make sure the user doesn't try to enable
// both OpenMP and pthreads.
#if defined ( BLIS_ENABLE_OPENMP ) && \
    defined ( BLIS_ENABLE_PTHREADS )
  #error "BLIS_ENABLE_OPENMP and BLIS_ENABLE_PTHREADS may not be simultaneously defined."
#endif

// Here, we define BLIS_ENABLE_MULTITHREADING if either OpenMP
// or pthreads are enabled. This macro is useful in situations when
// we want to detect use of either OpenMP or pthreads (as opposed
// to neither being used).
#if defined ( BLIS_ENABLE_OPENMP ) || \
    defined ( BLIS_ENABLE_PTHREADS )
  #define BLIS_ENABLE_MULTITHREADING
#endif


// -- MISCELLANEOUS OPTIONS ----------------------------------------------------

// Do NOT require the cross-blocksize constraints. That is, do not enforce
// MC % NR = 0 and NC % MR = 0 in bli_kernel_macro_defs.h. These are ONLY
// needed when implementing trsm_r by allowing the right-hand matrix B to
// be triangular.
#ifndef BLIS_RELAX_MCNR_NCMR_CONSTRAINTS
  #define BLIS_RELAX_MCNR_NCMR_CONSTRAINTS
#endif

// Stay initialized after auto-initialization, unless and until the user
// explicitly calls bli_finalize().
#ifdef BLIS_DISABLE_STAY_AUTO_INITIALIZED
  #undef BLIS_ENABLE_STAY_AUTO_INITIALIZED
#else
  // Default behavior is enabled.
  #undef  BLIS_ENABLE_STAY_AUTO_INITIALIZED // In case user explicitly enabled.
  #define BLIS_ENABLE_STAY_AUTO_INITIALIZED
#endif


// -- BLAS COMPATIBILITY LAYER -------------------------------------------------

// Enable the BLAS compatibility layer?
#ifdef BLIS_DISABLE_BLAS2BLIS
  #undef BLIS_ENABLE_BLAS2BLIS
#else
  // Default behavior is enabled.
  #undef  BLIS_ENABLE_BLAS2BLIS // In case user explicitly enabled.
  #define BLIS_ENABLE_BLAS2BLIS
#endif

// The bit size of the integer type used to track values such as dimensions and
// leading dimensions (ie: column strides) within the BLAS compatibility layer.
// A value of 32 results in the compatibility layer using 32-bit signed integers
// while 64 results in 64-bit integers. Any other value results in use of the
// C99 type "long int". Note that this ONLY affects integers used within the
// BLAS compatibility layer.
#ifndef BLIS_BLAS2BLIS_INT_TYPE_SIZE
#define BLIS_BLAS2BLIS_INT_TYPE_SIZE     32
#endif


// -- CBLAS COMPATIBILITY LAYER ------------------------------------------------

// Enable the CBLAS compatibility layer?
// NOTE: Enabling CBLAS will automatically enable the BLAS compatibility layer
// regardless of whether or not it was explicitly enabled above. Furthermore,
// the CBLAS compatibility layer will use the integer type size definition
// specified above when defining the size of its own integers (regardless of
// whether the BLAS layer was enabled directly or indirectly).
#ifdef BLIS_ENABLE_CBLAS
  // No additional definitions needed.
#else
  // Default behavior is disabled.
#endif


#endif

// end bli_config_macro_defs.h
// begin bli_type_defs.h


#ifndef BLIS_TYPE_DEFS_H
#define BLIS_TYPE_DEFS_H


//
// -- BLIS basic types ---------------------------------------------------------
//

#ifdef __cplusplus
  // For C++, include stdint.h.
#include <stdint.h> // skipped
#elif __STDC_VERSION__ >= 199901L
  // For C99 (or later), include stdint.h.
#include <stdint.h> // skipped
#else
  // When stdint.h is not available, manually typedef the types we will use.
  #ifdef _WIN32
    typedef          __int32  int32_t;
    typedef unsigned __int32 uint32_t;
    typedef          __int64  int64_t;
    typedef unsigned __int64 uint64_t;
  #else
    #error "Attempting to compile on pre-C99 system without stdint.h."
  #endif
#endif

// -- General-purpose integers --

// Define integer types depending on what size integer was requested.
#if   BLIS_INT_TYPE_SIZE == 32
typedef           int32_t  gint_t;
typedef          uint32_t guint_t;
#elif BLIS_INT_TYPE_SIZE == 64
typedef           int64_t  gint_t;
typedef          uint64_t guint_t;
#else
typedef   signed long int  gint_t;
typedef unsigned long int guint_t;
#endif

// -- Boolean type --

typedef  gint_t  bool_t;


// -- Boolean values --

#ifndef TRUE
  #define TRUE  1
#endif

#ifndef FALSE
  #define FALSE 0
#endif


// -- Special-purpose integers --

// This cpp guard provides a temporary hack to allow libflame
// interoperability with BLIS.
#ifndef _DEFINED_DIM_T
#define _DEFINED_DIM_T
typedef  gint_t  dim_t;      // dimension type
#endif
typedef  gint_t  inc_t;      // increment/stride type
typedef  gint_t  doff_t;     // diagonal offset type
typedef guint_t  siz_t;      // byte size type
typedef guint_t  objbits_t;  // object information bit field

// -- Real types --

// Define the number of floating-point types supported, and the size of the
// largest type.
#define BLIS_NUM_FP_TYPES   4
#define BLIS_MAX_TYPE_SIZE  sizeof(dcomplex)

// There are some places where we need to use sizeof() inside of a C
// preprocessor #if conditional, and so here we define the various sizes
// for those purposes.
#define BLIS_SIZEOF_S      4  // sizeof(float)
#define BLIS_SIZEOF_D      8  // sizeof(double)
#define BLIS_SIZEOF_C      8  // sizeof(scomplex)
#define BLIS_SIZEOF_Z      16 // sizeof(dcomplex)

// -- Complex types --

#ifdef BLIS_ENABLE_C99_COMPLEX

	#if __STDC_VERSION__ >= 199901L
#include <complex.h> // skipped

		// Typedef official complex types to BLIS complex type names.
		typedef  float complex scomplex;
		typedef double complex dcomplex;
	#else
		#error "Configuration requested C99 complex types, but C99 does not appear to be supported."
	#endif

#else // ifndef BLIS_ENABLE_C99_COMPLEX

	// This cpp guard provides a temporary hack to allow libflame
	// interoperability with BLIS.
	#ifndef _DEFINED_SCOMPLEX
	#define _DEFINED_SCOMPLEX
	typedef struct
	{
		float  real;
		float  imag;
	} scomplex;
	#endif

	// This cpp guard provides a temporary hack to allow libflame
	// interoperability with BLIS.
	#ifndef _DEFINED_DCOMPLEX
	#define _DEFINED_DCOMPLEX
	typedef struct
	{
		double real;
		double imag;
	} dcomplex;
	#endif

#endif // BLIS_ENABLE_C99_COMPLEX

// -- Atom type --

// Note: atom types are used to hold "bufferless" scalar object values. Note
// that it needs to be as large as the largest possible scalar value we might
// want to hold. Thus, for now, it is a dcomplex.
typedef dcomplex atom_t;

// -- Fortran-77 types --

// Note: These types are typically only used by BLAS compatibility layer, but
// we must define them even when the compatibility layer isn't being built
// because they also occur in bli_slamch() and bli_dlamch().

// Define f77_int depending on what size of integer was requested.
#if   BLIS_BLAS2BLIS_INT_TYPE_SIZE == 32
typedef int32_t   f77_int;
#elif BLIS_BLAS2BLIS_INT_TYPE_SIZE == 64
typedef int64_t   f77_int;
#else
typedef long int  f77_int;
#endif

typedef char      f77_char;
typedef float     f77_float;
typedef double    f77_double;
typedef scomplex  f77_scomplex;
typedef dcomplex  f77_dcomplex;


//
// -- BLIS info bit field offsets ----------------------------------------------
//



#define BLIS_DATATYPE_SHIFT                0
#define   BLIS_DOMAIN_SHIFT                0
#define   BLIS_PRECISION_SHIFT             1
#define BLIS_CONJTRANS_SHIFT               3
#define   BLIS_TRANS_SHIFT                 3
#define   BLIS_CONJ_SHIFT                  4
#define BLIS_UPLO_SHIFT                    5
#define   BLIS_UPPER_SHIFT                 5
#define   BLIS_DIAG_SHIFT                  6
#define   BLIS_LOWER_SHIFT                 7
#define BLIS_UNIT_DIAG_SHIFT               8
#define BLIS_INVERT_DIAG_SHIFT             9
#define BLIS_TARGET_DT_SHIFT               10
#define BLIS_EXECUTION_DT_SHIFT            13
#define BLIS_PACK_SCHEMA_SHIFT             16
#define   BLIS_PACK_RC_SHIFT               16
#define   BLIS_PACK_PANEL_SHIFT            17
#define   BLIS_PACK_FORMAT_SHIFT           18
#define   BLIS_PACK_SHIFT                  22
#define BLIS_PACK_REV_IF_UPPER_SHIFT       23
#define BLIS_PACK_REV_IF_LOWER_SHIFT       24
#define BLIS_PACK_BUFFER_SHIFT             25
#define BLIS_STRUC_SHIFT                   27

//
// -- BLIS info bit field masks ------------------------------------------------
//

#define BLIS_DATATYPE_BITS                 ( 0x7  << BLIS_DATATYPE_SHIFT )
#define   BLIS_DOMAIN_BIT                  ( 0x1  << BLIS_DOMAIN_SHIFT )
#define   BLIS_PRECISION_BIT               ( 0x1  << BLIS_PRECISION_SHIFT )
#define BLIS_CONJTRANS_BITS                ( 0x3  << BLIS_CONJTRANS_SHIFT )
#define   BLIS_TRANS_BIT                   ( 0x1  << BLIS_TRANS_SHIFT )
#define   BLIS_CONJ_BIT                    ( 0x1  << BLIS_CONJ_SHIFT )
#define BLIS_UPLO_BITS                     ( 0x7  << BLIS_UPLO_SHIFT )
#define   BLIS_UPPER_BIT                   ( 0x1  << BLIS_UPPER_SHIFT )
#define   BLIS_DIAG_BIT                    ( 0x1  << BLIS_DIAG_SHIFT )
#define   BLIS_LOWER_BIT                   ( 0x1  << BLIS_LOWER_SHIFT )
#define BLIS_UNIT_DIAG_BIT                 ( 0x1  << BLIS_UNIT_DIAG_SHIFT )
#define BLIS_INVERT_DIAG_BIT               ( 0x1  << BLIS_INVERT_DIAG_SHIFT )
#define BLIS_TARGET_DT_BITS                ( 0x7  << BLIS_TARGET_DT_SHIFT )
#define BLIS_EXECUTION_DT_BITS             ( 0x7  << BLIS_EXECUTION_DT_SHIFT )
#define BLIS_PACK_SCHEMA_BITS              ( 0x7F << BLIS_PACK_SCHEMA_SHIFT )
#define   BLIS_PACK_RC_BIT                 ( 0x1  << BLIS_PACK_RC_SHIFT )
#define   BLIS_PACK_PANEL_BIT              ( 0x1  << BLIS_PACK_PANEL_SHIFT )
#define   BLIS_PACK_FORMAT_BITS            ( 0xF  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_PACK_BIT                    ( 0x1  << BLIS_PACK_SHIFT )
#define BLIS_PACK_REV_IF_UPPER_BIT         ( 0x1  << BLIS_PACK_REV_IF_UPPER_SHIFT )
#define BLIS_PACK_REV_IF_LOWER_BIT         ( 0x1  << BLIS_PACK_REV_IF_LOWER_SHIFT )
#define BLIS_PACK_BUFFER_BITS              ( 0x3  << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_STRUC_BITS                    ( 0x3  << BLIS_STRUC_SHIFT )


//
// -- BLIS enumerated type value definitions -----------------------------------
//

#define BLIS_BITVAL_REAL                      0x0
#define BLIS_BITVAL_COMPLEX                   BLIS_DOMAIN_BIT
#define BLIS_BITVAL_SINGLE_PREC               0x0
#define BLIS_BITVAL_DOUBLE_PREC               BLIS_PRECISION_BIT
#define   BLIS_BITVAL_FLOAT_TYPE              0x0
#define   BLIS_BITVAL_SCOMPLEX_TYPE           BLIS_DOMAIN_BIT  
#define   BLIS_BITVAL_DOUBLE_TYPE             BLIS_PRECISION_BIT
#define   BLIS_BITVAL_DCOMPLEX_TYPE         ( BLIS_DOMAIN_BIT | BLIS_PRECISION_BIT )
#define   BLIS_BITVAL_INT_TYPE                0x04
#define   BLIS_BITVAL_CONST_TYPE              0x05
#define BLIS_BITVAL_NO_TRANS                  0x0
#define BLIS_BITVAL_TRANS                     BLIS_TRANS_BIT
#define BLIS_BITVAL_NO_CONJ                   0x0
#define BLIS_BITVAL_CONJ                      BLIS_CONJ_BIT
#define BLIS_BITVAL_CONJ_TRANS              ( BLIS_CONJ_BIT | BLIS_TRANS_BIT )
#define BLIS_BITVAL_ZEROS                     0x0 
#define BLIS_BITVAL_UPPER                   ( BLIS_UPPER_BIT | BLIS_DIAG_BIT )
#define BLIS_BITVAL_LOWER                   ( BLIS_LOWER_BIT | BLIS_DIAG_BIT )
#define BLIS_BITVAL_DENSE                     BLIS_UPLO_BITS  
#define BLIS_BITVAL_NONUNIT_DIAG              0x0
#define BLIS_BITVAL_UNIT_DIAG                 BLIS_UNIT_DIAG_BIT
#define BLIS_BITVAL_INVERT_DIAG               BLIS_INVERT_DIAG_BIT
#define BLIS_BITVAL_NOT_PACKED                0x0
#define   BLIS_BITVAL_4MI                   ( 0x1  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_3MI                   ( 0x2  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_4MS                   ( 0x3  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_3MS                   ( 0x4  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_RO                    ( 0x5  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_IO                    ( 0x6  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_RPI                   ( 0x7  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_1E                    ( 0x8  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_1R                    ( 0x9  << BLIS_PACK_FORMAT_SHIFT )
#define   BLIS_BITVAL_PACKED_UNSPEC         ( BLIS_PACK_BIT                                                            )
#define   BLIS_BITVAL_PACKED_ROWS           ( BLIS_PACK_BIT                                                            )
#define   BLIS_BITVAL_PACKED_COLUMNS        ( BLIS_PACK_BIT                                         | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS     ( BLIS_PACK_BIT                   | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS     ( BLIS_PACK_BIT                   | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_4MI ( BLIS_PACK_BIT | BLIS_BITVAL_4MI | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_4MI ( BLIS_PACK_BIT | BLIS_BITVAL_4MI | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_3MI ( BLIS_PACK_BIT | BLIS_BITVAL_3MI | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_3MI ( BLIS_PACK_BIT | BLIS_BITVAL_3MI | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_4MS ( BLIS_PACK_BIT | BLIS_BITVAL_4MS | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_4MS ( BLIS_PACK_BIT | BLIS_BITVAL_4MS | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_3MS ( BLIS_PACK_BIT | BLIS_BITVAL_3MS | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_3MS ( BLIS_PACK_BIT | BLIS_BITVAL_3MS | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_RO  ( BLIS_PACK_BIT | BLIS_BITVAL_RO  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_RO  ( BLIS_PACK_BIT | BLIS_BITVAL_RO  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_IO  ( BLIS_PACK_BIT | BLIS_BITVAL_IO  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_IO  ( BLIS_PACK_BIT | BLIS_BITVAL_IO  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_RPI ( BLIS_PACK_BIT | BLIS_BITVAL_RPI | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_RPI ( BLIS_PACK_BIT | BLIS_BITVAL_RPI | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_1E  ( BLIS_PACK_BIT | BLIS_BITVAL_1E  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_1E  ( BLIS_PACK_BIT | BLIS_BITVAL_1E  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define   BLIS_BITVAL_PACKED_ROW_PANELS_1R  ( BLIS_PACK_BIT | BLIS_BITVAL_1R  | BLIS_PACK_PANEL_BIT                    )
#define   BLIS_BITVAL_PACKED_COL_PANELS_1R  ( BLIS_PACK_BIT | BLIS_BITVAL_1R  | BLIS_PACK_PANEL_BIT | BLIS_PACK_RC_BIT )
#define BLIS_BITVAL_PACK_FWD_IF_UPPER         0x0
#define BLIS_BITVAL_PACK_REV_IF_UPPER         BLIS_PACK_REV_IF_UPPER_BIT
#define BLIS_BITVAL_PACK_FWD_IF_LOWER         0x0
#define BLIS_BITVAL_PACK_REV_IF_LOWER         BLIS_PACK_REV_IF_LOWER_BIT
#define BLIS_BITVAL_BUFFER_FOR_A_BLOCK        0x0
#define BLIS_BITVAL_BUFFER_FOR_B_PANEL      ( 0x1 << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_BITVAL_BUFFER_FOR_C_PANEL      ( 0x2 << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_BITVAL_BUFFER_FOR_GEN_USE      ( 0x3 << BLIS_PACK_BUFFER_SHIFT )
#define BLIS_BITVAL_GENERAL                   0x0
#define BLIS_BITVAL_HERMITIAN               ( 0x1 << BLIS_STRUC_SHIFT )
#define BLIS_BITVAL_SYMMETRIC               ( 0x2 << BLIS_STRUC_SHIFT )
#define BLIS_BITVAL_TRIANGULAR              ( 0x3 << BLIS_STRUC_SHIFT )


//
// -- BLIS enumerated type definitions -----------------------------------------
//

// -- Operational parameter types --

typedef enum
{
	BLIS_NO_TRANSPOSE      = 0x0,
	BLIS_TRANSPOSE         = BLIS_BITVAL_TRANS,
	BLIS_CONJ_NO_TRANSPOSE = BLIS_BITVAL_CONJ,
	BLIS_CONJ_TRANSPOSE    = BLIS_BITVAL_CONJ_TRANS
} trans_t;

typedef enum
{
	BLIS_NO_CONJUGATE      = 0x0,
	BLIS_CONJUGATE         = BLIS_BITVAL_CONJ
} conj_t;

typedef enum
{
	BLIS_ZEROS             = BLIS_BITVAL_ZEROS,
	BLIS_LOWER             = BLIS_BITVAL_LOWER,
	BLIS_UPPER             = BLIS_BITVAL_UPPER,
	BLIS_DENSE             = BLIS_BITVAL_DENSE
} uplo_t;

typedef enum
{
	BLIS_LEFT              = 0x0,
	BLIS_RIGHT
} side_t;

typedef enum
{
	BLIS_NONUNIT_DIAG      = 0x0,
	BLIS_UNIT_DIAG         = BLIS_BITVAL_UNIT_DIAG
} diag_t;

typedef enum
{
	BLIS_NO_INVERT_DIAG    = 0x0,
	BLIS_INVERT_DIAG       = BLIS_BITVAL_INVERT_DIAG
} invdiag_t;

typedef enum
{
	BLIS_GENERAL           = BLIS_BITVAL_GENERAL,
	BLIS_HERMITIAN         = BLIS_BITVAL_HERMITIAN,
	BLIS_SYMMETRIC         = BLIS_BITVAL_SYMMETRIC,
	BLIS_TRIANGULAR        = BLIS_BITVAL_TRIANGULAR
} struc_t;


// -- Data type --

typedef enum
{
	BLIS_FLOAT             = BLIS_BITVAL_FLOAT_TYPE,
	BLIS_DOUBLE            = BLIS_BITVAL_DOUBLE_TYPE,
	BLIS_SCOMPLEX          = BLIS_BITVAL_SCOMPLEX_TYPE,
	BLIS_DCOMPLEX          = BLIS_BITVAL_DCOMPLEX_TYPE,
	BLIS_INT               = BLIS_BITVAL_INT_TYPE,
	BLIS_CONSTANT          = BLIS_BITVAL_CONST_TYPE,
	BLIS_DT_LO             = BLIS_FLOAT,
	BLIS_DT_HI             = BLIS_DCOMPLEX
} num_t;

typedef enum
{
	BLIS_REAL              = BLIS_BITVAL_REAL,
	BLIS_COMPLEX           = BLIS_BITVAL_COMPLEX
} dom_t;

typedef enum
{
	BLIS_SINGLE_PREC       = BLIS_BITVAL_SINGLE_PREC,
	BLIS_DOUBLE_PREC       = BLIS_BITVAL_DOUBLE_PREC
} prec_t;


// -- Pack schema type --

typedef enum
{
	BLIS_NOT_PACKED            = BLIS_BITVAL_NOT_PACKED,
	BLIS_PACKED_UNSPEC         = BLIS_BITVAL_PACKED_UNSPEC,
	BLIS_PACKED_VECTOR         = BLIS_BITVAL_PACKED_UNSPEC,
	BLIS_PACKED_ROWS           = BLIS_BITVAL_PACKED_ROWS,
	BLIS_PACKED_COLUMNS        = BLIS_BITVAL_PACKED_COLUMNS,
	BLIS_PACKED_ROW_PANELS     = BLIS_BITVAL_PACKED_ROW_PANELS,
	BLIS_PACKED_COL_PANELS     = BLIS_BITVAL_PACKED_COL_PANELS,
	BLIS_PACKED_ROW_PANELS_4MI = BLIS_BITVAL_PACKED_ROW_PANELS_4MI,
	BLIS_PACKED_COL_PANELS_4MI = BLIS_BITVAL_PACKED_COL_PANELS_4MI,
	BLIS_PACKED_ROW_PANELS_3MI = BLIS_BITVAL_PACKED_ROW_PANELS_3MI,
	BLIS_PACKED_COL_PANELS_3MI = BLIS_BITVAL_PACKED_COL_PANELS_3MI,
	BLIS_PACKED_ROW_PANELS_4MS = BLIS_BITVAL_PACKED_ROW_PANELS_4MS,
	BLIS_PACKED_COL_PANELS_4MS = BLIS_BITVAL_PACKED_COL_PANELS_4MS,
	BLIS_PACKED_ROW_PANELS_3MS = BLIS_BITVAL_PACKED_ROW_PANELS_3MS,
	BLIS_PACKED_COL_PANELS_3MS = BLIS_BITVAL_PACKED_COL_PANELS_3MS,
	BLIS_PACKED_ROW_PANELS_RO  = BLIS_BITVAL_PACKED_ROW_PANELS_RO,
	BLIS_PACKED_COL_PANELS_RO  = BLIS_BITVAL_PACKED_COL_PANELS_RO,
	BLIS_PACKED_ROW_PANELS_IO  = BLIS_BITVAL_PACKED_ROW_PANELS_IO,
	BLIS_PACKED_COL_PANELS_IO  = BLIS_BITVAL_PACKED_COL_PANELS_IO,
	BLIS_PACKED_ROW_PANELS_RPI = BLIS_BITVAL_PACKED_ROW_PANELS_RPI,
	BLIS_PACKED_COL_PANELS_RPI = BLIS_BITVAL_PACKED_COL_PANELS_RPI,
	BLIS_PACKED_ROW_PANELS_1E  = BLIS_BITVAL_PACKED_ROW_PANELS_1E,
	BLIS_PACKED_COL_PANELS_1E  = BLIS_BITVAL_PACKED_COL_PANELS_1E,
	BLIS_PACKED_ROW_PANELS_1R  = BLIS_BITVAL_PACKED_ROW_PANELS_1R,
	BLIS_PACKED_COL_PANELS_1R  = BLIS_BITVAL_PACKED_COL_PANELS_1R
} pack_t;

// We combine row and column packing into one "type", and we start
// with BLIS_PACKED_ROW_PANELS, _COLUMN_PANELS. We also count the
// schema pair for "4ms" (4m separated), because its bit value has
// been reserved, even though we don't use it.
#define BLIS_NUM_PACK_SCHEMA_TYPES 10


// -- Pack order type --

typedef enum
{
	BLIS_PACK_FWD_IF_UPPER = BLIS_BITVAL_PACK_FWD_IF_UPPER,
	BLIS_PACK_REV_IF_UPPER = BLIS_BITVAL_PACK_REV_IF_UPPER,

	BLIS_PACK_FWD_IF_LOWER = BLIS_BITVAL_PACK_FWD_IF_LOWER,
	BLIS_PACK_REV_IF_LOWER = BLIS_BITVAL_PACK_REV_IF_LOWER
} packord_t;


// -- Pack buffer type --

typedef enum
{
	BLIS_BUFFER_FOR_A_BLOCK = BLIS_BITVAL_BUFFER_FOR_A_BLOCK,
	BLIS_BUFFER_FOR_B_PANEL = BLIS_BITVAL_BUFFER_FOR_B_PANEL,
	BLIS_BUFFER_FOR_C_PANEL = BLIS_BITVAL_BUFFER_FOR_C_PANEL,
	BLIS_BUFFER_FOR_GEN_USE = BLIS_BITVAL_BUFFER_FOR_GEN_USE
} packbuf_t;


// -- Partitioning direction --

typedef enum
{
	BLIS_FWD,
	BLIS_BWD
} dir_t;


// -- Subpartition type --

typedef enum
{
	BLIS_SUBPART0,
	BLIS_SUBPART1,
	BLIS_SUBPART2,
	BLIS_SUBPART1T,
	BLIS_SUBPART1B,
	BLIS_SUBPART1L,
	BLIS_SUBPART1R,
	BLIS_SUBPART00,
	BLIS_SUBPART10,
	BLIS_SUBPART20,
	BLIS_SUBPART01,
	BLIS_SUBPART11,
	BLIS_SUBPART21,
	BLIS_SUBPART02,
	BLIS_SUBPART12,
	BLIS_SUBPART22
} subpart_t;


// -- Matrix dimension type --

typedef enum
{
	BLIS_M = 0,
	BLIS_N = 1
} mdim_t;


// -- Machine parameter types --

typedef enum
{
	BLIS_MACH_EPS = 0,
	BLIS_MACH_SFMIN,
	BLIS_MACH_BASE,
	BLIS_MACH_PREC,
	BLIS_MACH_NDIGMANT,
	BLIS_MACH_RND,
	BLIS_MACH_EMIN,
	BLIS_MACH_RMIN,
	BLIS_MACH_EMAX,
	BLIS_MACH_RMAX,
	BLIS_MACH_EPS2
} machval_t;

#define BLIS_NUM_MACH_PARAMS   11
#define BLIS_MACH_PARAM_FIRST  BLIS_MACH_EPS
#define BLIS_MACH_PARAM_LAST   BLIS_MACH_EPS2


// -- Induced method types --

typedef enum
{
	BLIS_3MH = 0,
	BLIS_3M1,
	BLIS_4MH,
	BLIS_4M1B,
	BLIS_4M1A,
	BLIS_1M,
	BLIS_NAT
} ind_t;

#define BLIS_NUM_IND_METHODS (BLIS_NAT+1)

// These are used in bli_*_oapi.c to construct the ind_t values from
// the induced method substrings that go into function names.
#define bli_3mh  BLIS_3MH
#define bli_3m1  BLIS_3M1
#define bli_4mh  BLIS_4MH
#define bli_4mb  BLIS_4M1B
#define bli_4m1  BLIS_4M1A
#define bli_1m   BLIS_1M
#define bli_nat  BLIS_NAT


// -- Kernel ID types --

typedef enum
{
	BLIS_ADDV_KER  = 0,
	BLIS_AMAXV_KER,
	BLIS_AXPBYV_KER,
	BLIS_AXPYV_KER,
	BLIS_COPYV_KER,
	BLIS_DOTV_KER,
	BLIS_DOTXV_KER,
	BLIS_INVERTV_KER,
	BLIS_SCALV_KER,
	BLIS_SCAL2V_KER,
	BLIS_SETV_KER,
	BLIS_SUBV_KER,
	BLIS_SWAPV_KER,
	BLIS_XPBYV_KER
} l1vkr_t;

#define BLIS_NUM_LEVEL1V_KERS 14


typedef enum
{
	BLIS_AXPY2V_KER = 0,
	BLIS_DOTAXPYV_KER,
	BLIS_AXPYF_KER,
	BLIS_DOTXF_KER,
	BLIS_DOTXAXPYF_KER
} l1fkr_t;

#define BLIS_NUM_LEVEL1F_KERS 5


typedef enum
{
	BLIS_PACKM_0XK_KER  = 0,
	BLIS_PACKM_1XK_KER  = 1,
	BLIS_PACKM_2XK_KER  = 2,
	BLIS_PACKM_3XK_KER  = 3,
	BLIS_PACKM_4XK_KER  = 4,
	BLIS_PACKM_5XK_KER  = 5,
	BLIS_PACKM_6XK_KER  = 6,
	BLIS_PACKM_7XK_KER  = 7,
	BLIS_PACKM_8XK_KER  = 8,
	BLIS_PACKM_9XK_KER  = 9,
	BLIS_PACKM_10XK_KER = 10,
	BLIS_PACKM_11XK_KER = 11,
	BLIS_PACKM_12XK_KER = 12,
	BLIS_PACKM_13XK_KER = 13,
	BLIS_PACKM_14XK_KER = 14,
	BLIS_PACKM_15XK_KER = 15,
	BLIS_PACKM_16XK_KER = 16,
	BLIS_PACKM_17XK_KER = 17,
	BLIS_PACKM_18XK_KER = 18,
	BLIS_PACKM_19XK_KER = 19,
	BLIS_PACKM_20XK_KER = 20,
	BLIS_PACKM_21XK_KER = 21,
	BLIS_PACKM_22XK_KER = 22,
	BLIS_PACKM_23XK_KER = 23,
	BLIS_PACKM_24XK_KER = 24,
	BLIS_PACKM_25XK_KER = 25,
	BLIS_PACKM_26XK_KER = 26,
	BLIS_PACKM_27XK_KER = 27,
	BLIS_PACKM_28XK_KER = 28,
	BLIS_PACKM_29XK_KER = 29,
	BLIS_PACKM_30XK_KER = 30,
	BLIS_PACKM_31XK_KER = 31,

	BLIS_UNPACKM_0XK_KER  = 0,
	BLIS_UNPACKM_1XK_KER  = 1,
	BLIS_UNPACKM_2XK_KER  = 2,
	BLIS_UNPACKM_3XK_KER  = 3,
	BLIS_UNPACKM_4XK_KER  = 4,
	BLIS_UNPACKM_5XK_KER  = 5,
	BLIS_UNPACKM_6XK_KER  = 6,
	BLIS_UNPACKM_7XK_KER  = 7,
	BLIS_UNPACKM_8XK_KER  = 8,
	BLIS_UNPACKM_9XK_KER  = 9,
	BLIS_UNPACKM_10XK_KER = 10,
	BLIS_UNPACKM_11XK_KER = 11,
	BLIS_UNPACKM_12XK_KER = 12,
	BLIS_UNPACKM_13XK_KER = 13,
	BLIS_UNPACKM_14XK_KER = 14,
	BLIS_UNPACKM_15XK_KER = 15,
	BLIS_UNPACKM_16XK_KER = 16,
	BLIS_UNPACKM_17XK_KER = 17,
	BLIS_UNPACKM_18XK_KER = 18,
	BLIS_UNPACKM_19XK_KER = 19,
	BLIS_UNPACKM_20XK_KER = 20,
	BLIS_UNPACKM_21XK_KER = 21,
	BLIS_UNPACKM_22XK_KER = 22,
	BLIS_UNPACKM_23XK_KER = 23,
	BLIS_UNPACKM_24XK_KER = 24,
	BLIS_UNPACKM_25XK_KER = 25,
	BLIS_UNPACKM_26XK_KER = 26,
	BLIS_UNPACKM_27XK_KER = 27,
	BLIS_UNPACKM_28XK_KER = 28,
	BLIS_UNPACKM_29XK_KER = 29,
	BLIS_UNPACKM_30XK_KER = 30,
	BLIS_UNPACKM_31XK_KER = 31

} l1mkr_t;

#define BLIS_NUM_PACKM_KERS   32
#define BLIS_NUM_UNPACKM_KERS 32


typedef enum
{
	BLIS_GEMM_UKR = 0,
	BLIS_GEMMTRSM_L_UKR,
	BLIS_GEMMTRSM_U_UKR,
	BLIS_TRSM_L_UKR,
	BLIS_TRSM_U_UKR
} l3ukr_t;

#define BLIS_NUM_LEVEL3_UKRS 5


typedef enum
{
	BLIS_REFERENCE_UKERNEL = 0,
	BLIS_VIRTUAL_UKERNEL,
	BLIS_OPTIMIZED_UKERNEL,
	BLIS_NOTAPPLIC_UKERNEL
} kimpl_t;

#define BLIS_NUM_UKR_IMPL_TYPES 4


#if 0
typedef enum
{
	BLIS_JC_IDX = 0,
	BLIS_PC_IDX,
	BLIS_IC_IDX,
	BLIS_JR_IDX,
	BLIS_IR_IDX,
	BLIS_PR_IDX
} thridx_t;
#endif

#define BLIS_NUM_LOOPS 6


// -- Operation ID type --

typedef enum
{
//
// NOTE: If/when additional type values are added to this enum,
// you must either:
// - keep the level-3 values (starting with _GEMM) beginning at
//   index 0; or
// - if the value range is moved such that it does not begin at
//   index 0, implement something like a BLIS_OPID_LEVEL3_RANGE_START
//   value that can be subtracted from the opid_t value to map it
//   to a zero-based range.
// This is needed because these level-3 opid_t values are used in
// bli_l3_ind.c to index into arrays.
//
	BLIS_GEMM = 0,
	BLIS_HEMM,
	BLIS_HERK,
	BLIS_HER2K,
	BLIS_SYMM,
	BLIS_SYRK,
	BLIS_SYR2K,
	BLIS_TRMM3,
	BLIS_TRMM,
	BLIS_TRSM,

	BLIS_NOID
} opid_t;

#define BLIS_NUM_LEVEL3_OPS 10


// -- Blocksize ID type --

typedef enum
{
	// NOTE: the level-3 blocksizes MUST be indexed starting at zero.
	// At one point, we made this assumption in bli_cntx_set_blkszs()
	// and friends.

	BLIS_KR = 0,
	BLIS_MR,
	BLIS_NR,
	BLIS_MC,
	BLIS_KC,
	BLIS_NC,
	BLIS_M2, // level-2 blocksize in m dimension
	BLIS_N2, // level-2 blocksize in n dimension
	BLIS_AF, // level-1f axpyf fusing factor
	BLIS_DF, // level-1f dotxf fusing factor
	BLIS_XF, // level-1f dotxaxpyf fusing factor

	BLIS_NO_PART  // used as a placeholder when blocksizes are not applicable.
} bszid_t;

#define BLIS_NUM_BLKSZS 11


// -- Architecture ID type --

// NOTE: This typedef enum must be kept up-to-date with the arch_t
// string array in bli_arch.c. Whenever values are added/inserted
// OR if values are rearranged, be sure to update the string array
// in bli_arch.c.

typedef enum
{
	// Intel
	BLIS_ARCH_SKX = 0,
	BLIS_ARCH_KNL,
	BLIS_ARCH_KNC,
	BLIS_ARCH_HASWELL,
	BLIS_ARCH_SANDYBRIDGE,
	BLIS_ARCH_PENRYN,

	// AMD
	BLIS_ARCH_ZEN,
	BLIS_ARCH_EXCAVATOR,
	BLIS_ARCH_STEAMROLLER,
	BLIS_ARCH_PILEDRIVER,
	BLIS_ARCH_BULLDOZER,

	// ARM
	BLIS_ARCH_CORTEXA57,
	BLIS_ARCH_CORTEXA15,
	BLIS_ARCH_CORTEXA9,

	// IBM/Power
	BLIS_ARCH_POWER7,
	BLIS_ARCH_BGQ,

	// Generic architecture/configuration
	BLIS_ARCH_GENERIC

} arch_t;

#define BLIS_NUM_ARCHS 17


//
// -- BLIS misc. structure types -----------------------------------------------
//

// -- Pool block type --

typedef struct
{
	void* buf_sys;
	void* buf_align;
} pblk_t;

// -- Pool type --

typedef struct
{
	pblk_t* block_ptrs;
	dim_t   block_ptrs_len;

	dim_t   top_index;
	dim_t   num_blocks;

	siz_t   block_size;
	siz_t   align_size;
} pool_t;

// -- Mutex object type --

// begin bli_mutex.h


#ifndef BLIS_MUTEX_H
#define BLIS_MUTEX_H

// Include definitions (mostly mtx_t) specific to the method of
// multithreading.
// begin bli_mutex_single.h


#ifndef BLIS_MUTEX_SINGLE_H
#define BLIS_MUTEX_SINGLE_H

// Define mtx_t for situations when multithreading is disabled.
#ifndef BLIS_ENABLE_MULTITHREADING

// Define mtx_t.

typedef struct mtx_s
{
} mtx_t;

// Define macros to operate on pthread-based mtx_t.

static void bli_mutex_init( mtx_t* m )
{
}

static void bli_mutex_finalize( mtx_t* m )
{
}

static void bli_mutex_lock( mtx_t* m )
{
}

static void bli_mutex_unlock( mtx_t* m )
{
}

#endif

#endif

// end bli_mutex_single.h
// begin bli_mutex_openmp.h


#ifndef BLIS_MUTEX_OPENMP_H
#define BLIS_MUTEX_OPENMP_H

// Define mutex_t for situations when OpenMP multithreading is enabled.
#ifdef BLIS_ENABLE_OPENMP

#include <omp.h> // skipped

// Define mtx_t.

typedef struct mtx_s
{
	omp_lock_t mutex;
} mtx_t;

// Define functions to operate on OpenMP-based mtx_t.

static void bli_mutex_init( mtx_t* m )
{
	omp_init_lock( &(m->mutex) );
}

static void bli_mutex_finalize( mtx_t* m )
{
	omp_destroy_lock( &(m->mutex) );
}

static void bli_mutex_lock( mtx_t* m )
{
	omp_set_lock( &(m->mutex) );
}

static void bli_mutex_unlock( mtx_t* m )
{
	omp_unset_lock( &(m->mutex) );
}

#endif

#endif

// end bli_mutex_openmp.h
// begin bli_mutex_pthreads.h


#ifndef BLIS_MUTEX_PTHREADS_H
#define BLIS_MUTEX_PTHREADS_H

// Define mutex_t for situations when POSIX multithreading is enabled.
#ifdef BLIS_ENABLE_PTHREADS

#include <pthread.h> // skipped

// Define mtx_t.

typedef struct mtx_s
{
	pthread_mutex_t mutex;
} mtx_t;

// Define macros to operate on pthread-based mtx_t.

static void bli_mutex_init( mtx_t* m )
{
	pthread_mutex_init( &(m->mutex), NULL ); \
}

static void bli_mutex_finalize( mtx_t* m )
{
	pthread_mutex_destroy( &(m->mutex) ); \
}

static void bli_mutex_lock( mtx_t* m )
{
	pthread_mutex_lock( &(m->mutex) ); \
}

static void bli_mutex_unlock( mtx_t* m )
{
	pthread_mutex_unlock( &(m->mutex) ); \
}

#endif

#endif

// end bli_mutex_pthreads.h

// Thread mutex prototypes.


#endif

// end bli_mutex.h
// begin bli_malloc.h


// Typedef function pointer types for malloc() and free() substitutes.
typedef void* (*malloc_ft) ( size_t size );
typedef void  (*free_ft)   ( void*  p    );

// -----------------------------------------------------------------------------

void* bli_malloc_pool( size_t size );
void  bli_free_pool( void* p );

void* bli_malloc_intl( size_t size );
void* bli_calloc_intl( size_t size );
void  bli_free_intl( void* p );

void* bli_malloc_user( size_t size );
void  bli_free_user( void* p );

// -----------------------------------------------------------------------------

void* bli_malloc_align( malloc_ft f, size_t size, size_t align_size );
void  bli_free_align( free_ft f, void* p );

void* bli_malloc_noalign( malloc_ft f, size_t size );
void  bli_free_noalign( free_ft f, void* p );

void  bli_malloc_align_check( malloc_ft f, size_t size, size_t align_size );

// end bli_malloc.h

// -- Memory broker object type --

typedef struct membrk_s
{
	pool_t    pools[3];
	mtx_t     mutex;

	malloc_ft malloc_fp;
	free_ft   free_fp;
} membrk_t;

// -- Memory object type --

typedef struct mem_s
{
	pblk_t    pblk;
	packbuf_t buf_type;
	pool_t*   pool;
	membrk_t* membrk;
	siz_t     size;
} mem_t;

// -- Control tree node type --

struct cntl_s
{
	// Basic fields (usually required).
	opid_t         family;
	bszid_t        bszid;
	void*          var_func;
	struct cntl_s* sub_node;

	// Optional fields (needed only by some operations such as packm).
	// NOTE: first field of params must be a uint64_t containing the size
	// of the struct.
	void*          params;

	// Internal fields that track "cached" data.
	mem_t          pack_mem;
};
typedef struct cntl_s cntl_t;


// -- Blocksize object type --

typedef struct blksz_s
{
	// Primary blocksize values.
	dim_t  v[BLIS_NUM_FP_TYPES];

	// Blocksize extensions.
	dim_t  e[BLIS_NUM_FP_TYPES];

} blksz_t;


// -- Function pointer object type --

typedef struct func_s
{
	// Kernel function address.
	void*  ptr[BLIS_NUM_FP_TYPES];

} func_t;


// -- Multi-boolean object type --

typedef struct mbool_s
{
	bool_t  v[BLIS_NUM_FP_TYPES];

} mbool_t;


// -- Auxiliary kernel info type --

// Note: This struct is used by macro-kernels to package together extra
// parameter values that may be of use to the micro-kernel without
// cluttering up the micro-kernel interface itself.

typedef struct
{
	// The pack schemas of A and B.
	pack_t schema_a;
	pack_t schema_b;

	// Pointers to the micro-panels of A and B which will be used by the
	// next call to the micro-kernel.
	void*  a_next;
	void*  b_next;

	// The imaginary strides of A and B.
	inc_t  is_a;
	inc_t  is_b;

} auxinfo_t;


// -- Global scalar constant data struct --

// Note: This struct is used only when statically initializing the
// global scalar constants in bli_const.c.
typedef struct constdata_s
{
	float    s;
	double   d;
	scomplex c;
	dcomplex z;
	gint_t   i;

} constdata_t;


//
// -- BLIS object type definitions ---------------------------------------------
//

typedef struct obj_s
{
	// Basic fields
	struct obj_s* root;

	dim_t         off[2];
	dim_t         dim[2];
	doff_t        diag_off;

	objbits_t     info;
	siz_t         elem_size;

	void*         buffer;
	inc_t         rs;
	inc_t         cs;
	inc_t         is;

	// Bufferless scalar storage
	atom_t        scalar;

	// Pack-related fields
	dim_t         m_padded; // m dimension of matrix, including any padding
	dim_t         n_padded; // n dimension of matrix, including any padding
	inc_t         ps;       // panel stride (distance to next panel)
	inc_t         pd;       // panel dimension (the "width" of a panel:
	                        // usually MR or NR)
	dim_t         m_panel;  // m dimension of a "full" panel
	dim_t         n_panel;  // n dimension of a "full" panel
} obj_t;

// Define these macros here since they must be updated if contents of
// obj_t changes.

#define bli_obj_init_full_shallow_copy_of( a, b ) \
{ \
	(b).root      = (a).root; \
\
	(b).off[0]    = (a).off[0]; \
	(b).off[1]    = (a).off[1]; \
	(b).dim[0]    = (a).dim[0]; \
	(b).dim[1]    = (a).dim[1]; \
	(b).diag_off  = (a).diag_off; \
\
	(b).info      = (a).info; \
	(b).elem_size = (a).elem_size; \
\
	(b).buffer    = (a).buffer; \
	(b).rs        = (a).rs; \
	(b).cs        = (a).cs; \
	(b).is        = (a).is; \
\
	(b).scalar    = (a).scalar; \
\
	 \
	(b).m_padded  = (a).m_padded; \
	(b).n_padded  = (a).n_padded; \
	(b).ps        = (a).ps; \
	(b).pd        = (a).pd; \
	(b).m_panel   = (a).m_panel; \
	(b).n_panel   = (a).n_panel; \
}

#define bli_obj_init_subpart_from( a, b ) \
{ \
	(b).root      = (a).root; \
\
	(b).off[0]    = (a).off[0]; \
	(b).off[1]    = (a).off[1]; \
	 \
	 \
	(b).diag_off  = (a).diag_off; \
\
	(b).info      = (a).info; \
	(b).elem_size = (a).elem_size; \
\
	(b).buffer    = (a).buffer; \
	(b).rs        = (a).rs; \
	(b).cs        = (a).cs; \
	(b).is        = (a).is; \
\
	(b).scalar    = (a).scalar; \
\
	 \
	 \
	(b).m_padded  = (a).m_padded; \
	(b).n_padded  = (a).n_padded; \
	(b).pd        = (a).pd; \
	(b).ps        = (a).ps; \
	(b).m_panel   = (a).m_panel; \
	(b).n_panel   = (a).n_panel; \
}


// -- Context type --

typedef struct cntx_s
{
	blksz_t   blkszs[ BLIS_NUM_BLKSZS ];
	bszid_t   bmults[ BLIS_NUM_BLKSZS ];

	func_t    l3_vir_ukrs[ BLIS_NUM_LEVEL3_UKRS ];
	func_t    l3_nat_ukrs[ BLIS_NUM_LEVEL3_UKRS ];
	mbool_t   l3_nat_ukrs_prefs[ BLIS_NUM_LEVEL3_UKRS ];

	func_t    l1f_kers[ BLIS_NUM_LEVEL1F_KERS ];
	func_t    l1v_kers[ BLIS_NUM_LEVEL1V_KERS ];

	func_t    packm_kers[ BLIS_NUM_PACKM_KERS ];
	func_t    unpackm_kers[ BLIS_NUM_UNPACKM_KERS ];

	ind_t     method;
	pack_t    schema_a_block;
	pack_t    schema_b_panel;
	pack_t    schema_c_panel;

	bool_t    anti_pref;

	dim_t     thrloop[ BLIS_NUM_LOOPS ];

	membrk_t* membrk;
} cntx_t;


// -- Error types --

typedef enum
{
	BLIS_NO_ERROR_CHECKING = 0,
	BLIS_FULL_ERROR_CHECKING
} errlev_t;

typedef enum
{
	// Generic error codes
	BLIS_SUCCESS                               = (  -1),
	BLIS_FAILURE                               = (  -2),

	BLIS_ERROR_CODE_MIN                        = (  -9),

	// General errors
	BLIS_INVALID_ERROR_CHECKING_LEVEL          = ( -10),
	BLIS_UNDEFINED_ERROR_CODE                  = ( -11),
	BLIS_NULL_POINTER                          = ( -12),
	BLIS_NOT_YET_IMPLEMENTED                   = ( -13),

	// Parameter-specific errors
	BLIS_INVALID_SIDE                          = ( -20),
	BLIS_INVALID_UPLO                          = ( -21),
	BLIS_INVALID_TRANS                         = ( -22),
	BLIS_INVALID_CONJ                          = ( -23),
	BLIS_INVALID_DIAG                          = ( -24),
	BLIS_INVALID_MACHVAL                       = ( -25),
	BLIS_EXPECTED_NONUNIT_DIAG                 = ( -26),

	// Datatype-specific errors
	BLIS_INVALID_DATATYPE                      = ( -30),
	BLIS_EXPECTED_FLOATING_POINT_DATATYPE      = ( -31),
	BLIS_EXPECTED_NONINTEGER_DATATYPE          = ( -32),
	BLIS_EXPECTED_NONCONSTANT_DATATYPE         = ( -33),
	BLIS_EXPECTED_REAL_DATATYPE                = ( -34),
	BLIS_EXPECTED_INTEGER_DATATYPE             = ( -35),
	BLIS_INCONSISTENT_DATATYPES                = ( -36),
	BLIS_EXPECTED_REAL_PROJ_OF                 = ( -37),
	BLIS_EXPECTED_REAL_VALUED_OBJECT           = ( -38),

	// Dimension-specific errors
	BLIS_NONCONFORMAL_DIMENSIONS               = ( -40),
	BLIS_EXPECTED_SCALAR_OBJECT                = ( -41),
	BLIS_EXPECTED_VECTOR_OBJECT                = ( -42),
	BLIS_UNEQUAL_VECTOR_LENGTHS                = ( -43),
	BLIS_EXPECTED_SQUARE_OBJECT                = ( -44),
	BLIS_UNEXPECTED_OBJECT_LENGTH              = ( -45),
	BLIS_UNEXPECTED_OBJECT_WIDTH               = ( -46),
	BLIS_UNEXPECTED_VECTOR_DIM                 = ( -47),
	BLIS_UNEXPECTED_DIAG_OFFSET                = ( -48),
	BLIS_NEGATIVE_DIMENSION                    = ( -49),

	// Stride-specific errors
	BLIS_INVALID_ROW_STRIDE                    = ( -50),
	BLIS_INVALID_COL_STRIDE                    = ( -51),
	BLIS_INVALID_DIM_STRIDE_COMBINATION        = ( -52),

	// Structure-specific errors    
	BLIS_EXPECTED_GENERAL_OBJECT               = ( -60),
	BLIS_EXPECTED_HERMITIAN_OBJECT             = ( -61),
	BLIS_EXPECTED_SYMMETRIC_OBJECT             = ( -62),
	BLIS_EXPECTED_TRIANGULAR_OBJECT            = ( -63),

	// Storage-specific errors    
	BLIS_EXPECTED_UPPER_OR_LOWER_OBJECT        = ( -70),

	// Partitioning-specific errors
	BLIS_INVALID_3x1_SUBPART                   = ( -80),
	BLIS_INVALID_1x3_SUBPART                   = ( -81),
	BLIS_INVALID_3x3_SUBPART                   = ( -82),

	// Control tree-specific errors
	BLIS_UNEXPECTED_NULL_CONTROL_TREE          = ( -90),

	// Packing-specific errors
	BLIS_PACK_SCHEMA_NOT_SUPPORTED_FOR_UNPACK  = (-100),

	// Buffer-specific errors 
	BLIS_EXPECTED_NONNULL_OBJECT_BUFFER        = (-110),

	// Memory allocator errors
	BLIS_INVALID_PACKBUF                       = (-120),
	BLIS_EXHAUSTED_CONTIG_MEMORY_POOL          = (-122),
	BLIS_INSUFFICIENT_STACK_BUF_SIZE           = (-123),
	BLIS_ALIGNMENT_NOT_POWER_OF_TWO            = (-124),
	BLIS_ALIGNMENT_NOT_MULT_OF_PTR_SIZE        = (-125),

	// Object-related errors
	BLIS_EXPECTED_OBJECT_ALIAS                 = (-130),

	// Architecture-related errors
	BLIS_INVALID_ARCH_ID                       = (-140),

	// Blocksize-related errors
	BLIS_MC_DEF_NONMULTIPLE_OF_MR              = (-150),
	BLIS_MC_MAX_NONMULTIPLE_OF_MR              = (-151),
	BLIS_NC_DEF_NONMULTIPLE_OF_NR              = (-152),
	BLIS_NC_MAX_NONMULTIPLE_OF_NR              = (-153),
	BLIS_KC_DEF_NONMULTIPLE_OF_KR              = (-154),
	BLIS_KC_MAX_NONMULTIPLE_OF_KR              = (-155),

	BLIS_ERROR_CODE_MAX                        = (-160)
} err_t;

#endif
// end bli_type_defs.h


enum CBLAS_ORDER {CblasRowMajor=101, CblasColMajor=102};
enum CBLAS_TRANSPOSE {CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113};
enum CBLAS_UPLO {CblasUpper=121, CblasLower=122};
enum CBLAS_DIAG {CblasNonUnit=131, CblasUnit=132};
enum CBLAS_SIDE {CblasLeft=141, CblasRight=142};

#ifdef __cplusplus
extern "C" {
#endif


float  cblas_sdsdot(f77_int N, float alpha, const float *X,
                    f77_int incX, const float *Y, f77_int incY);
double cblas_dsdot(f77_int N, const float *X, f77_int incX, const float *Y,
                   f77_int incY);
float  cblas_sdot(f77_int N, const float  *X, f77_int incX,
                  const float  *Y, f77_int incY);
double cblas_ddot(f77_int N, const double *X, f77_int incX,
                  const double *Y, f77_int incY);


void   cblas_cdotu_sub(f77_int N, const void *X, f77_int incX,
                       const void *Y, f77_int incY, void *dotu);
void   cblas_cdotc_sub(f77_int N, const void *X, f77_int incX,
                       const void *Y, f77_int incY, void *dotc);

void   cblas_zdotu_sub(f77_int N, const void *X, f77_int incX,
                       const void *Y, f77_int incY, void *dotu);
void   cblas_zdotc_sub(f77_int N, const void *X, f77_int incX,
                       const void *Y, f77_int incY, void *dotc);



float  cblas_snrm2(f77_int N, const float *X, f77_int incX);
float  cblas_sasum(f77_int N, const float *X, f77_int incX);

double cblas_dnrm2(f77_int N, const double *X, f77_int incX);
double cblas_dasum(f77_int N, const double *X, f77_int incX);

float  cblas_scnrm2(f77_int N, const void *X, f77_int incX);
float  cblas_scasum(f77_int N, const void *X, f77_int incX);

double cblas_dznrm2(f77_int N, const void *X, f77_int incX);
double cblas_dzasum(f77_int N, const void *X, f77_int incX);



f77_int cblas_isamax(f77_int N, const float  *X, f77_int incX);
f77_int cblas_idamax(f77_int N, const double *X, f77_int incX);
f77_int cblas_icamax(f77_int N, const void   *X, f77_int incX);
f77_int cblas_izamax(f77_int N, const void   *X, f77_int incX);




void cblas_sswap(f77_int N, float *X, f77_int incX,
                 float *Y, f77_int incY);
void cblas_scopy(f77_int N, const float *X, f77_int incX,
                 float *Y, f77_int incY);
void cblas_saxpy(f77_int N, float alpha, const float *X,
                 f77_int incX, float *Y, f77_int incY);

void cblas_dswap(f77_int N, double *X, f77_int incX,
                 double *Y, f77_int incY);
void cblas_dcopy(f77_int N, const double *X, f77_int incX,
                 double *Y, f77_int incY);
void cblas_daxpy(f77_int N, double alpha, const double *X,
                 f77_int incX, double *Y, f77_int incY);

void cblas_cswap(f77_int N, void *X, f77_int incX,
                 void *Y, f77_int incY);
void cblas_ccopy(f77_int N, const void *X, f77_int incX,
                 void *Y, f77_int incY);
void cblas_caxpy(f77_int N, const void *alpha, const void *X,
                 f77_int incX, void *Y, f77_int incY);

void cblas_zswap(f77_int N, void *X, f77_int incX,
                 void *Y, f77_int incY);
void cblas_zcopy(f77_int N, const void *X, f77_int incX,
                 void *Y, f77_int incY);
void cblas_zaxpy(f77_int N, const void *alpha, const void *X,
                 f77_int incX, void *Y, f77_int incY);



void cblas_srotg(float *a, float *b, float *c, float *s);
void cblas_srotmg(float *d1, float *d2, float *b1, const float b2, float *P);
void cblas_srot(f77_int N, float *X, f77_int incX,
                float *Y, f77_int incY, const float c, const float s);
void cblas_srotm(f77_int N, float *X, f77_int incX,
                float *Y, f77_int incY, const float *P);

void cblas_drotg(double *a, double *b, double *c, double *s);
void cblas_drotmg(double *d1, double *d2, double *b1, const double b2, double *P);
void cblas_drot(f77_int N, double *X, f77_int incX,
                double *Y, f77_int incY, const double c, const double  s);
void cblas_drotm(f77_int N, double *X, f77_int incX,
                double *Y, f77_int incY, const double *P);



void cblas_sscal(f77_int N, float alpha, float *X, f77_int incX);
void cblas_dscal(f77_int N, double alpha, double *X, f77_int incX);
void cblas_cscal(f77_int N, const void *alpha, void *X, f77_int incX);
void cblas_zscal(f77_int N, const void *alpha, void *X, f77_int incX);
void cblas_csscal(f77_int N, float alpha, void *X, f77_int incX);
void cblas_zdscal(f77_int N, double alpha, void *X, f77_int incX);




void cblas_sgemv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 float alpha, const float *A, f77_int lda,
                 const float *X, f77_int incX, float beta,
                 float *Y, f77_int incY);
void cblas_sgbmv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 f77_int KL, f77_int KU, float alpha,
                 const float *A, f77_int lda, const float *X,
                 f77_int incX, float beta, float *Y, f77_int incY);
void cblas_strmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const float *A, f77_int lda,
                 float *X, f77_int incX);
void cblas_stbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const float *A, f77_int lda,
                 float *X, f77_int incX);
void cblas_stpmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const float *Ap, float *X, f77_int incX);
void cblas_strsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const float *A, f77_int lda, float *X,
                 f77_int incX);
void cblas_stbsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const float *A, f77_int lda,
                 float *X, f77_int incX);
void cblas_stpsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const float *Ap, float *X, f77_int incX);

void cblas_dgemv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 double alpha, const double *A, f77_int lda,
                 const double *X, f77_int incX, double beta,
                 double *Y, f77_int incY);
void cblas_dgbmv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 f77_int KL, f77_int KU, double alpha,
                 const double *A, f77_int lda, const double *X,
                 f77_int incX, double beta, double *Y, f77_int incY);
void cblas_dtrmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const double *A, f77_int lda,
                 double *X, f77_int incX);
void cblas_dtbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const double *A, f77_int lda,
                 double *X, f77_int incX);
void cblas_dtpmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const double *Ap, double *X, f77_int incX);
void cblas_dtrsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const double *A, f77_int lda, double *X,
                 f77_int incX);
void cblas_dtbsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const double *A, f77_int lda,
                 double *X, f77_int incX);
void cblas_dtpsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const double *Ap, double *X, f77_int incX);

void cblas_cgemv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 const void *X, f77_int incX, const void *beta,
                 void *Y, f77_int incY);
void cblas_cgbmv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 f77_int KL, f77_int KU, const void *alpha,
                 const void *A, f77_int lda, const void *X,
                 f77_int incX, const void *beta, void *Y, f77_int incY);
void cblas_ctrmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *A, f77_int lda,
                 void *X, f77_int incX);
void cblas_ctbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const void *A, f77_int lda,
                 void *X, f77_int incX);
void cblas_ctpmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *Ap, void *X, f77_int incX);
void cblas_ctrsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *A, f77_int lda, void *X,
                 f77_int incX);
void cblas_ctbsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const void *A, f77_int lda,
                 void *X, f77_int incX);
void cblas_ctpsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *Ap, void *X, f77_int incX);

void cblas_zgemv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 const void *X, f77_int incX, const void *beta,
                 void *Y, f77_int incY);
void cblas_zgbmv(enum CBLAS_ORDER order,
                 enum CBLAS_TRANSPOSE TransA, f77_int M, f77_int N,
                 f77_int KL, f77_int KU, const void *alpha,
                 const void *A, f77_int lda, const void *X,
                 f77_int incX, const void *beta, void *Y, f77_int incY);
void cblas_ztrmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *A, f77_int lda,
                 void *X, f77_int incX);
void cblas_ztbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const void *A, f77_int lda,
                 void *X, f77_int incX);
void cblas_ztpmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *Ap, void *X, f77_int incX);
void cblas_ztrsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *A, f77_int lda, void *X,
                 f77_int incX);
void cblas_ztbsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, f77_int K, const void *A, f77_int lda,
                 void *X, f77_int incX);
void cblas_ztpsv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE TransA, enum CBLAS_DIAG Diag,
                 f77_int N, const void *Ap, void *X, f77_int incX);



void cblas_ssymv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, float alpha, const float *A,
                 f77_int lda, const float *X, f77_int incX,
                 float beta, float *Y, f77_int incY);
void cblas_ssbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, f77_int K, float alpha, const float *A,
                 f77_int lda, const float *X, f77_int incX,
                 float beta, float *Y, f77_int incY);
void cblas_sspmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, float alpha, const float *Ap,
                 const float *X, f77_int incX,
                 float beta, float *Y, f77_int incY);
void cblas_sger(enum CBLAS_ORDER order, f77_int M, f77_int N,
                float alpha, const float *X, f77_int incX,
                const float *Y, f77_int incY, float *A, f77_int lda);
void cblas_ssyr(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, float alpha, const float *X,
                f77_int incX, float *A, f77_int lda);
void cblas_sspr(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, float alpha, const float *X,
                f77_int incX, float *Ap);
void cblas_ssyr2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, float alpha, const float *X,
                f77_int incX, const float *Y, f77_int incY, float *A,
                f77_int lda);
void cblas_sspr2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, float alpha, const float *X,
                f77_int incX, const float *Y, f77_int incY, float *A);

void cblas_dsymv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, double alpha, const double *A,
                 f77_int lda, const double *X, f77_int incX,
                 double beta, double *Y, f77_int incY);
void cblas_dsbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, f77_int K, double alpha, const double *A,
                 f77_int lda, const double *X, f77_int incX,
                 double beta, double *Y, f77_int incY);
void cblas_dspmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, double alpha, const double *Ap,
                 const double *X, f77_int incX,
                 double beta, double *Y, f77_int incY);
void cblas_dger(enum CBLAS_ORDER order, f77_int M, f77_int N,
                double alpha, const double *X, f77_int incX,
                const double *Y, f77_int incY, double *A, f77_int lda);
void cblas_dsyr(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, double alpha, const double *X,
                f77_int incX, double *A, f77_int lda);
void cblas_dspr(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, double alpha, const double *X,
                f77_int incX, double *Ap);
void cblas_dsyr2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, double alpha, const double *X,
                f77_int incX, const double *Y, f77_int incY, double *A,
                f77_int lda);
void cblas_dspr2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, double alpha, const double *X,
                f77_int incX, const double *Y, f77_int incY, double *A);



void cblas_chemv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, const void *alpha, const void *A,
                 f77_int lda, const void *X, f77_int incX,
                 const void *beta, void *Y, f77_int incY);
void cblas_chbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, f77_int K, const void *alpha, const void *A,
                 f77_int lda, const void *X, f77_int incX,
                 const void *beta, void *Y, f77_int incY);
void cblas_chpmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, const void *alpha, const void *Ap,
                 const void *X, f77_int incX,
                 const void *beta, void *Y, f77_int incY);
void cblas_cgeru(enum CBLAS_ORDER order, f77_int M, f77_int N,
                 const void *alpha, const void *X, f77_int incX,
                 const void *Y, f77_int incY, void *A, f77_int lda);
void cblas_cgerc(enum CBLAS_ORDER order, f77_int M, f77_int N,
                 const void *alpha, const void *X, f77_int incX,
                 const void *Y, f77_int incY, void *A, f77_int lda);
void cblas_cher(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, float alpha, const void *X, f77_int incX,
                void *A, f77_int lda);
void cblas_chpr(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, float alpha, const void *X,
                f77_int incX, void *A);
void cblas_cher2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo, f77_int N,
                const void *alpha, const void *X, f77_int incX,
                const void *Y, f77_int incY, void *A, f77_int lda);
void cblas_chpr2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo, f77_int N,
                const void *alpha, const void *X, f77_int incX,
                const void *Y, f77_int incY, void *Ap);

void cblas_zhemv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, const void *alpha, const void *A,
                 f77_int lda, const void *X, f77_int incX,
                 const void *beta, void *Y, f77_int incY);
void cblas_zhbmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, f77_int K, const void *alpha, const void *A,
                 f77_int lda, const void *X, f77_int incX,
                 const void *beta, void *Y, f77_int incY);
void cblas_zhpmv(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                 f77_int N, const void *alpha, const void *Ap,
                 const void *X, f77_int incX,
                 const void *beta, void *Y, f77_int incY);
void cblas_zgeru(enum CBLAS_ORDER order, f77_int M, f77_int N,
                 const void *alpha, const void *X, f77_int incX,
                 const void *Y, f77_int incY, void *A, f77_int lda);
void cblas_zgerc(enum CBLAS_ORDER order, f77_int M, f77_int N,
                 const void *alpha, const void *X, f77_int incX,
                 const void *Y, f77_int incY, void *A, f77_int lda);
void cblas_zher(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, double alpha, const void *X, f77_int incX,
                void *A, f77_int lda);
void cblas_zhpr(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo,
                f77_int N, double alpha, const void *X,
                f77_int incX, void *A);
void cblas_zher2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo, f77_int N,
                const void *alpha, const void *X, f77_int incX,
                const void *Y, f77_int incY, void *A, f77_int lda);
void cblas_zhpr2(enum CBLAS_ORDER order, enum CBLAS_UPLO Uplo, f77_int N,
                const void *alpha, const void *X, f77_int incX,
                const void *Y, f77_int incY, void *Ap);




void cblas_sgemm(enum CBLAS_ORDER Order, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_TRANSPOSE TransB, f77_int M, f77_int N,
                 f77_int K, float alpha, const float *A,
                 f77_int lda, const float *B, f77_int ldb,
                 float beta, float *C, f77_int ldc);
void cblas_ssymm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, f77_int M, f77_int N,
                 float alpha, const float *A, f77_int lda,
                 const float *B, f77_int ldb, float beta,
                 float *C, f77_int ldc);
void cblas_ssyrk(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                 float alpha, const float *A, f77_int lda,
                 float beta, float *C, f77_int ldc);
void cblas_ssyr2k(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                  enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                  float alpha, const float *A, f77_int lda,
                  const float *B, f77_int ldb, float beta,
                  float *C, f77_int ldc);
void cblas_strmm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 float alpha, const float *A, f77_int lda,
                 float *B, f77_int ldb);
void cblas_strsm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 float alpha, const float *A, f77_int lda,
                 float *B, f77_int ldb);

void cblas_dgemm(enum CBLAS_ORDER Order, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_TRANSPOSE TransB, f77_int M, f77_int N,
                 f77_int K, double alpha, const double *A,
                 f77_int lda, const double *B, f77_int ldb,
                 double beta, double *C, f77_int ldc);
void cblas_dsymm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, f77_int M, f77_int N,
                 double alpha, const double *A, f77_int lda,
                 const double *B, f77_int ldb, double beta,
                 double *C, f77_int ldc);
void cblas_dsyrk(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                 double alpha, const double *A, f77_int lda,
                 double beta, double *C, f77_int ldc);
void cblas_dsyr2k(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                  enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                  double alpha, const double *A, f77_int lda,
                  const double *B, f77_int ldb, double beta,
                  double *C, f77_int ldc);
void cblas_dtrmm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 double alpha, const double *A, f77_int lda,
                 double *B, f77_int ldb);
void cblas_dtrsm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 double alpha, const double *A, f77_int lda,
                 double *B, f77_int ldb);

void cblas_cgemm(enum CBLAS_ORDER Order, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_TRANSPOSE TransB, f77_int M, f77_int N,
                 f77_int K, const void *alpha, const void *A,
                 f77_int lda, const void *B, f77_int ldb,
                 const void *beta, void *C, f77_int ldc);
void cblas_csymm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 const void *B, f77_int ldb, const void *beta,
                 void *C, f77_int ldc);
void cblas_csyrk(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                 const void *alpha, const void *A, f77_int lda,
                 const void *beta, void *C, f77_int ldc);
void cblas_csyr2k(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                  enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                  const void *alpha, const void *A, f77_int lda,
                  const void *B, f77_int ldb, const void *beta,
                  void *C, f77_int ldc);
void cblas_ctrmm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 void *B, f77_int ldb);
void cblas_ctrsm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 void *B, f77_int ldb);

void cblas_zgemm(enum CBLAS_ORDER Order, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_TRANSPOSE TransB, f77_int M, f77_int N,
                 f77_int K, const void *alpha, const void *A,
                 f77_int lda, const void *B, f77_int ldb,
                 const void *beta, void *C, f77_int ldc);
void cblas_zsymm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 const void *B, f77_int ldb, const void *beta,
                 void *C, f77_int ldc);
void cblas_zsyrk(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                 const void *alpha, const void *A, f77_int lda,
                 const void *beta, void *C, f77_int ldc);
void cblas_zsyr2k(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                  enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                  const void *alpha, const void *A, f77_int lda,
                  const void *B, f77_int ldb, const void *beta,
                  void *C, f77_int ldc);
void cblas_ztrmm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 void *B, f77_int ldb);
void cblas_ztrsm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, enum CBLAS_TRANSPOSE TransA,
                 enum CBLAS_DIAG Diag, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 void *B, f77_int ldb);



void cblas_chemm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 const void *B, f77_int ldb, const void *beta,
                 void *C, f77_int ldc);
void cblas_cherk(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                 float alpha, const void *A, f77_int lda,
                 float beta, void *C, f77_int ldc);
void cblas_cher2k(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                  enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                  const void *alpha, const void *A, f77_int lda,
                  const void *B, f77_int ldb, float beta,
                  void *C, f77_int ldc);

void cblas_zhemm(enum CBLAS_ORDER Order, enum CBLAS_SIDE Side,
                 enum CBLAS_UPLO Uplo, f77_int M, f77_int N,
                 const void *alpha, const void *A, f77_int lda,
                 const void *B, f77_int ldb, const void *beta,
                 void *C, f77_int ldc);
void cblas_zherk(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                 enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                 double alpha, const void *A, f77_int lda,
                 double beta, void *C, f77_int ldc);
void cblas_zher2k(enum CBLAS_ORDER Order, enum CBLAS_UPLO Uplo,
                  enum CBLAS_TRANSPOSE Trans, f77_int N, f77_int K,
                  const void *alpha, const void *A, f77_int lda,
                  const void *B, f77_int ldb, double beta,
                  void *C, f77_int ldc);

void cblas_xerbla(f77_int p, const char *rout, const char *form, ...);

#ifdef __cplusplus
}
#endif
#endif
// end cblas.h


#endif // BLIS_ENABLE_CBLAS

#endif
// end bli_cblas.h


// End extern "C" construct block.
#ifdef __cplusplus
}
#endif

#endif

