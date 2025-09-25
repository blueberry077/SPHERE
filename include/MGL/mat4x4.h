#ifndef __MAT4X4_H__
#define __MAT4X4_H__

#include <math.h>
#include <string.h>

#ifndef MGLAPI
#define MGLAPI
#endif // MGLAPI

typedef struct float3 {
	float x, y, z;
} float3;

typedef struct float4 {
	float v[4];
} float4;

typedef struct MGLMatrix4x4 {
	float m[4][4];
} MGLMatrix4x4;

MGLAPI void float3_normalize(float3 *v);
MGLAPI void float3_cross(float3 *d, float3 a, float3 b);

MGLAPI MGLMatrix4x4 mat4x4_identity(void);
MGLAPI MGLMatrix4x4 mat4x4_mul(MGLMatrix4x4 a, MGLMatrix4x4 b);
MGLAPI float4 mat4x4_float4_mul(float4 a, MGLMatrix4x4 b);

#endif // __MAT4X4_H__