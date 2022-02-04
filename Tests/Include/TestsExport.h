
#ifndef TESTS_API_H
#define TESTS_API_H

#ifdef METADATA_STATIC_DEFINE
#  define TESTS_API
#  define METADATA_NO_EXPORT
#else
#  ifndef TESTS_API
#    ifdef Metadata_EXPORTS
        /* We are building this library */
#      define TESTS_API 
#    else
        /* We are using this library */
#      define TESTS_API 
#    endif
#  endif

#  ifndef METADATA_NO_EXPORT
#    define METADATA_NO_EXPORT 
#  endif
#endif

#ifndef METADATA_DEPRECATED
#  define METADATA_DEPRECATED __declspec(deprecated)
#endif

#ifndef METADATA_DEPRECATED_EXPORT
#  define METADATA_DEPRECATED_EXPORT TESTS_API METADATA_DEPRECATED
#endif

#ifndef METADATA_DEPRECATED_NO_EXPORT
#  define METADATA_DEPRECATED_NO_EXPORT METADATA_NO_EXPORT METADATA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef METADATA_NO_DEPRECATED
#    define METADATA_NO_DEPRECATED
#  endif
#endif

#endif /* TESTS_API_H */
