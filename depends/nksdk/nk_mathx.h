#ifndef NK_MATHX_H__ /*///////////////////////////////////////////////////////*/
#define NK_MATHX_H__

#include "nk_math.h"

#if !defined(NK_HAS_CPP)
#error nk_mathx requires C++ in order to be used
#endif

NKAPI NKFORCEINLINE nkVec2& operator+=(nkVec2& a, const nkVec2& b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}
NKAPI NKFORCEINLINE nkVec2& operator-=(nkVec2& a, const nkVec2& b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}
NKAPI NKFORCEINLINE nkVec2& operator/=(nkVec2& a, const nkVec2& b)
{
    a.x /= b.x;
    a.y /= b.y;
    return a;
}
NKAPI NKFORCEINLINE nkVec2& operator*=(nkVec2& a, const nkVec2& b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

NKAPI NKFORCEINLINE nkVec2& operator+=(nkVec2& a, const nkF32& b)
{
    a.x += b;
    a.y += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2& operator-=(nkVec2& a, const nkF32& b)
{
    a.x -= b;
    a.y -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2& operator/=(nkVec2& a, const nkF32& b)
{
    a.x /= b;
    a.y /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2& operator*=(nkVec2& a, const nkF32& b)
{
    a.x *= b;
    a.y *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec2 operator+(nkVec2 a, const nkVec2& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2 operator-(nkVec2 a, const nkVec2& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2 operator/(nkVec2 a, const nkVec2& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2 operator*(nkVec2 a, const nkVec2& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec2 operator+(nkVec2 a, const nkF32& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2 operator-(nkVec2 a, const nkF32& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2 operator/(nkVec2 a, const nkF32& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec2 operator*(nkVec2 a, const nkF32& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkBool operator==(const nkVec2& a, const nkVec2& b)
{
    return (a.x == b.x && a.y == b.y);
}
NKAPI NKFORCEINLINE nkBool operator!=(const nkVec2& a, const nkVec2& b)
{
    return !(a == b);
}

NKAPI NKFORCEINLINE nkVec3& operator+=(nkVec3& a, const nkVec3& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}
NKAPI NKFORCEINLINE nkVec3& operator-=(nkVec3& a, const nkVec3& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}
NKAPI NKFORCEINLINE nkVec3& operator/=(nkVec3& a, const nkVec3& b)
{
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
    return a;
}
NKAPI NKFORCEINLINE nkVec3& operator*=(nkVec3& a, const nkVec3& b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    return a;
}

NKAPI NKFORCEINLINE nkVec3& operator+=(nkVec3& a, const nkF32& b)
{
    a.x += b;
    a.y += b;
    a.z += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3& operator-=(nkVec3& a, const nkF32& b)
{
    a.x -= b;
    a.y -= b;
    a.z -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3& operator/=(nkVec3& a, const nkF32& b)
{
    a.x /= b;
    a.y /= b;
    a.z /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3& operator*=(nkVec3& a, const nkF32& b)
{
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec3 operator+(nkVec3 a, const nkVec3& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3 operator-(nkVec3 a, const nkVec3& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3 operator/(nkVec3 a, const nkVec3& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3 operator*(nkVec3 a, const nkVec3& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec3 operator+(nkVec3 a, const nkF32& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3 operator-(nkVec3 a, const nkF32& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3 operator/(nkVec3 a, const nkF32& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec3 operator*(nkVec3 a, const nkF32& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkBool operator==(const nkVec3& a, const nkVec3& b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}
NKAPI NKFORCEINLINE nkBool operator!=(const nkVec3& a, const nkVec3& b)
{
    return !(a == b);
}

NKAPI NKFORCEINLINE nkVec4& operator+=(nkVec4& a, const nkVec4& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}
NKAPI NKFORCEINLINE nkVec4& operator-=(nkVec4& a, const nkVec4& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    a.w -= b.w;
    return a;
}
NKAPI NKFORCEINLINE nkVec4& operator/=(nkVec4& a, const nkVec4& b)
{
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
    a.w /= b.w;
    return a;
}
NKAPI NKFORCEINLINE nkVec4& operator*=(nkVec4& a, const nkVec4& b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
    a.w *= b.w;
    return a;
}

NKAPI NKFORCEINLINE nkVec4& operator+=(nkVec4& a, const nkF32& b)
{
    a.x += b;
    a.y += b;
    a.z += b;
    a.w += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4& operator-=(nkVec4& a, const nkF32& b)
{
    a.x -= b;
    a.y -= b;
    a.z -= b;
    a.w -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4& operator/=(nkVec4& a, const nkF32& b)
{
    a.x /= b;
    a.y /= b;
    a.z /= b;
    a.w /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4& operator*=(nkVec4& a, const nkF32& b)
{
    a.x *= b;
    a.y *= b;
    a.z *= b;
    a.w *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec4 operator+(nkVec4 a, const nkVec4& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4 operator-(nkVec4 a, const nkVec4& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4 operator/(nkVec4 a, const nkVec4& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4 operator*(nkVec4 a, const nkVec4& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec4 operator+(nkVec4 a, const nkF32& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4 operator-(nkVec4 a, const nkF32& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4 operator/(nkVec4 a, const nkF32& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkVec4 operator*(nkVec4 a, const nkF32& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkBool operator==(const nkVec4& a, const nkVec4& b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
}
NKAPI NKFORCEINLINE nkBool operator!=(const nkVec4& a, const nkVec4& b)
{
    return !(a == b);
}

NKAPI NKFORCEINLINE nkMat2& operator+=(nkMat2& a, const nkMat2& b)
{
    for(nkS32 c=0; c<2; ++c)
        for(nkS32 r=0; r<2; ++r)
            a.x[c][r] += b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat2& operator-=(nkMat2& a, const nkMat2& b)
{
    for(nkS32 c=0; c<2; ++c)
        for(nkS32 r=0; r<2; ++r)
            a.x[c][r] -= b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat2& operator/=(nkMat2& a, const nkMat2& b)
{
    for(nkS32 c=0; c<2; ++c)
        for(nkS32 r=0; r<2; ++r)
            a.x[c][r] /= b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat2& operator*=(nkMat2& a, const nkMat2& b)
{
    nkMat2 m = NK_ZERO_MEM;
    for(nkS32 c=0; c<2; ++c)
        for(nkS32 r0=0; r0<2; ++r0)
            for(nkS32 r1=0; r1<2; ++r1)
                m.x[c][r0] += a.x[c][r1] * b.x[r1][r0];
    a = m;
    return a;
}

NKAPI NKFORCEINLINE nkMat2 operator+(nkMat2 a, const nkMat2& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkMat2 operator-(nkMat2 a, const nkMat2& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkMat2 operator/(nkMat2 a, const nkMat2& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkMat2 operator*(nkMat2 a, const nkMat2& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec2 operator*(nkMat2 a, const nkVec2& b)
{
    nkVec2 v = NK_ZERO_MEM;
    for(nkS32 r=0; r<2; ++r)
        for(nkS32 c=0; c<2; ++c)
            v.raw[r] += a.x[c][r] * b.raw[c];
    return v;
}

NKAPI NKFORCEINLINE nkBool operator==(const nkMat2& a, const nkMat2& b)
{
    for(nkS32 r=0; r<2; ++r)
        for(nkS32 c=0; c<2; ++c)
            if(a.x[c][r] != b.x[c][r])
                return NK_FALSE;
    return NK_TRUE;
}
NKAPI NKFORCEINLINE nkBool operator!=(const nkMat2& a, const nkMat2& b)
{
    return !(a == b);
}

NKAPI NKFORCEINLINE nkMat3& operator+=(nkMat3& a, const nkMat3& b)
{
    for(nkS32 c=0; c<3; ++c)
        for(nkS32 r=0; r<3; ++r)
            a.x[c][r] += b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat3& operator-=(nkMat3& a, const nkMat3& b)
{
    for(nkS32 c=0; c<3; ++c)
        for(nkS32 r=0; r<3; ++r)
            a.x[c][r] -= b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat3& operator/=(nkMat3& a, const nkMat3& b)
{
    for(nkS32 c=0; c<3; ++c)
        for(nkS32 r=0; r<3; ++r)
            a.x[c][r] /= b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat3& operator*=(nkMat3& a, const nkMat3& b)
{
    nkMat3 m = NK_ZERO_MEM;
    for(nkS32 c=0; c<3; ++c)
        for(nkS32 r0=0; r0<3; ++r0)
            for(nkS32 r1=0; r1<3; ++r1)
                m.x[c][r0] += a.x[c][r1] * b.x[r1][r0];
    a = m;
    return a;
}

NKAPI NKFORCEINLINE nkMat3 operator+(nkMat3 a, const nkMat3& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkMat3 operator-(nkMat3 a, const nkMat3& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkMat3 operator/(nkMat3 a, const nkMat3& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkMat3 operator*(nkMat3 a, const nkMat3& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec3 operator*(nkMat3 a, const nkVec3& b)
{
    nkVec3 v = NK_ZERO_MEM;
    for(nkS32 r=0; r<3; ++r)
        for(nkS32 c=0; c<3; ++c)
            v.raw[r] += a.x[c][r] * b.raw[c];
    return v;
}

NKAPI NKFORCEINLINE nkBool operator==(const nkMat3& a, const nkMat3& b)
{
    for(nkS32 r=0; r<3; ++r)
        for(nkS32 c=0; c<3; ++c)
            if(a.x[c][r] != b.x[c][r])
                return NK_FALSE;
    return NK_TRUE;
}
NKAPI NKFORCEINLINE nkBool operator!=(const nkMat3& a, const nkMat3& b)
{
    return !(a == b);
}

NKAPI NKFORCEINLINE nkMat4& operator+=(nkMat4& a, const nkMat4& b)
{
    for(nkS32 c=0; c<4; ++c)
        for(nkS32 r=0; r<4; ++r)
            a.x[c][r] += b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat4& operator-=(nkMat4& a, const nkMat4& b)
{
    for(nkS32 c=0; c<4; ++c)
        for(nkS32 r=0; r<4; ++r)
            a.x[c][r] -= b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat4& operator/=(nkMat4& a, const nkMat4& b)
{
    for(nkS32 c=0; c<4; ++c)
        for(nkS32 r=0; r<4; ++r)
            a.x[c][r] /= b.x[c][r];
    return a;
}
NKAPI NKFORCEINLINE nkMat4& operator*=(nkMat4& a, const nkMat4& b)
{
    nkMat4 m = NK_ZERO_MEM;
    for(nkS32 c=0; c<4; ++c)
        for(nkS32 r0=0; r0<4; ++r0)
            for(nkS32 r1=0; r1<4; ++r1)
                m.x[c][r0] += a.x[c][r1] * b.x[r1][r0];
    a = m;
    return a;
}

NKAPI NKFORCEINLINE nkMat4 operator+(nkMat4 a, const nkMat4& b)
{
    a += b;
    return a;
}
NKAPI NKFORCEINLINE nkMat4 operator-(nkMat4 a, const nkMat4& b)
{
    a -= b;
    return a;
}
NKAPI NKFORCEINLINE nkMat4 operator/(nkMat4 a, const nkMat4& b)
{
    a /= b;
    return a;
}
NKAPI NKFORCEINLINE nkMat4 operator*(nkMat4 a, const nkMat4& b)
{
    a *= b;
    return a;
}

NKAPI NKFORCEINLINE nkVec4 operator*(nkMat4 a, const nkVec4& b)
{
    nkVec4 v = NK_ZERO_MEM;
    for(nkS32 r=0; r<4; ++r)
        for(nkS32 c=0; c<4; ++c)
            v.raw[r] += a.x[c][r] * b.raw[c];
    return v;
}

NKAPI NKFORCEINLINE nkBool operator==(const nkMat4& a, const nkMat4& b)
{
    for(nkS32 r=0; r<4; ++r)
        for(nkS32 c=0; c<4; ++c)
            if(a.x[c][r] != b.x[c][r])
                return NK_FALSE;
    return NK_TRUE;
}
NKAPI NKFORCEINLINE nkBool operator!=(const nkMat4& a, const nkMat4& b)
{
    return !(a == b);
}

#endif /* NK_MATHX_H__ ///////////////////////////////////////////////////////*/

/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2022 Joshua Robertson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
*******************************************************************************/
