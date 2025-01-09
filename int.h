#if !defined(u8)
    #if (defined(_MSC_VER) || defined(__SYMBIAN32__) && !defined(RGFW_STD_INT))
        typedef unsigned char   u8;
        typedef signed char     i8;
        typedef unsigned short  u16;
        typedef signed short    i16;
        typedef unsigned int    u32;
        typedef signed int      i32;
        typedef unsigned long   u64;
        typedef signed long     i64;
    #else
        #include <stdint.h>
        typedef uint8_t         uint8;
        typedef int8_t          int8;
        typedef uint16_t        uint16;
        typedef int16_t         int16;
        typedef uint32_t        uint32;
        typedef int32_t         int32;
        typedef uint64_t        uint64;
        typedef int64_t         int64;
    #endif
#endif
