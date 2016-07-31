// Copyright 2016 Phil Homan

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#if KATO_SLOW
// TODO(casey): Complete assertion macro - don't worry everyone!
#define ASSERT(Expression) \
  if (!(Expression)) {     \
    *(int *)0 = 0;         \
  }
#else
#define ASSERT(Expression)
#endif

#define ARRAY_COUNT(Array) (sizeof(Array) / sizeof((Array)[0]))

#define KILOBYTES(x) ((x)*1024LL)
#define MEGABYTES(x) (KILOBYTES(x) * 1024LL)
#define GIGABYTES(x) (MEGABYTES(x) * 1024LL)
#define TERABYTES(x) (GIGABYTES(x) * 1024LL)

#endif  // COMMON_MACROS_H_
