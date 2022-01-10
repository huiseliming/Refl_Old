
#ifndef METADATA_API_H
#define METADATA_API_H

#ifdef METADATA_STATIC_DEFINE
#  define METADATA_API
#  define METADATA_NO_EXPORT
#else
#  ifndef METADATA_API
#    ifdef Metadata_EXPORTS
        /* We are building this library */
#      define METADATA_API 
#    else
        /* We are using this library */
#      define METADATA_API 
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
#  define METADATA_DEPRECATED_EXPORT METADATA_API METADATA_DEPRECATED
#endif

#ifndef METADATA_DEPRECATED_NO_EXPORT
#  define METADATA_DEPRECATED_NO_EXPORT METADATA_NO_EXPORT METADATA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef METADATA_NO_DEPRECATED
#    define METADATA_NO_DEPRECATED
#  endif
#endif

#endif /* METADATA_API_H */
