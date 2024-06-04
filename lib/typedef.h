#pragma once
/*********************************************************************
 * This file (typedef.h) 
 *  - Is part of the jmake project, which can be found here: https://github.com/jamesray23114/jmake.
 *  - Is licensed under the MIT License, a copy of which can be found in the LICENSE file.
 *  - Was created on May 11 2024.
 * It:
 * - Contains common definitions of types used throughout the project.
 * - Changes the defitions of the cstdint types to remove the _t suffix.
*********************************************************************/

// == project includes ==
// n/a

// == standard includes ==
#include <cstdint>

// == system includes ==
// n/a

// =============================
// ======== definitions ========
// =============================
#define           true      1
#define           false     0

#define           null      0
#define           nullptr   (void*) 0

typedef unsigned  int       uint;

typedef           int8_t    int8;
typedef           int16_t   int16;
typedef           int32_t   int32;
typedef           int64_t   int64;

typedef           float     f32;
typedef           double    f64;

typedef           uint8_t   uint8;
typedef           uint16_t  uint16;
typedef           uint32_t  uint32;
typedef           uint64_t  uint64;

// =============================
// ====== implementations ======
// =============================
// n/a