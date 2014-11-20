#if !defined(_MSC_VER)
  #include <stdint.h>
#else
    // Define C99 equivalent types.
    // Since MSVC doesn't include these headers, we have to write our own
    // version to provide a compatibility layer between MSVC and the stdint.h
    // headers.
    typedef signed char         int8_t;
    typedef signed short        int16_t;
    typedef signed int          int32_t;
    typedef signed long long    int64_t;
    typedef unsigned char       uint8_t;
    typedef unsigned short      uint16_t;
    typedef unsigned int        uint32_t;
    typedef unsigned long long  uint64_t;
#endif