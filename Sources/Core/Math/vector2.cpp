/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magic Software - www.magic-software.com
*/

#include "Core/precomp.h"
#include "API/Core/Math/vector2.h"
#include <math.h>
#include <float.h>

const CL_Vector2 CL_Vector2::ZERO(0.0f,0.0f);
const CL_Vector2 CL_Vector2::UNIT_X(1.0f,0.0f);
const CL_Vector2 CL_Vector2::UNIT_Y(0.0f,1.0f);
float CL_Vector2::FUZZ = 0.0f;

CL_Vector2::CL_Vector2(float fX, float fY)
{
    x = fX;
    y = fY;
}

CL_Vector2::CL_Vector2(float afCoordinate[2])
{
    x = afCoordinate[0];
    y = afCoordinate[1];
}

CL_Vector2::CL_Vector2(const CL_Vector2& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
}

CL_Vector2& CL_Vector2::operator=(const CL_Vector2& rkVector)
{
    x = rkVector.x;
    y = rkVector.y;
    return *this;
}

bool CL_Vector2::operator==(const CL_Vector2& rkVector) const
{
    if ( FUZZ == 0.0f )
        return x == rkVector.x && y == rkVector.y;
    else
        return fabs(x-rkVector.x) <= FUZZ && fabs(y-rkVector.y) <= FUZZ;
}

bool CL_Vector2::operator!=(const CL_Vector2& rkVector) const
{
    if ( FUZZ == 0.0f )
        return x != rkVector.x || y != rkVector.y;
    else
        return fabs(x-rkVector.x) > FUZZ || fabs(y-rkVector.y) > FUZZ;
}

bool CL_Vector2::operator<(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 < uiTest1 )
        return true;
    if ( uiTest0 > uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 < uiTest1;
}

bool CL_Vector2::operator<=(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 < uiTest1 )
        return true;
    if ( uiTest0 > uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 <= uiTest1;
}

bool CL_Vector2::operator>(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 > uiTest1 )
        return true;
    if ( uiTest0 < uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 > uiTest1;
}

bool CL_Vector2::operator>=(const CL_Vector2& rkVector) const
{
    float fXTmp = rkVector.x, fYTmp = rkVector.y;
    if ( FUZZ > 0.0f )
    {
        if ( fabs(x - fXTmp) <= FUZZ )
            fXTmp = x;
        if ( fabs(y - fYTmp) <= FUZZ )
            fYTmp = y;
    }

    // compare y values
    unsigned int uiTest0 = *(unsigned int*)&y;
    unsigned int uiTest1 = *(unsigned int*)&fYTmp;
    if ( uiTest0 > uiTest1 )
        return true;
    if ( uiTest0 < uiTest1 )
        return false;

    // compare x values
    uiTest0 = *(unsigned int*)&x;
    uiTest1 = *(unsigned int*)&fXTmp;
    return uiTest0 >= uiTest1;
}

CL_Vector2 CL_Vector2::operator+(const CL_Vector2& rkVector) const
{
    return CL_Vector2(x+rkVector.x,y+rkVector.y);
}

CL_Vector2 CL_Vector2::operator-(const CL_Vector2& rkVector) const
{
    return CL_Vector2(x-rkVector.x,y-rkVector.y);
}

CL_Vector2 CL_Vector2::operator*(float fScalar) const
{
    return CL_Vector2(fScalar*x,fScalar*y);
}

CL_Vector2 CL_Vector2::operator-() const
{
    return CL_Vector2(-x,-y);
}

/*
CL_Vector2 Mgc::operator* (float fScalar, const CL_Vector2& rkVector)
{
    return CL_Vector2(fScalar*rkVector.x,fScalar*rkVector.y);
}
*/
CL_Vector2 CL_Vector2::operator/(float fScalar) const
{
    CL_Vector2 kQuot;

    if ( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f/fScalar;
        kQuot.x = fInvScalar*x;
        kQuot.y = fInvScalar*y;
        return kQuot;
    }
    else
    {
        return CL_Vector2(FLT_MAX,FLT_MAX);
    }
}

CL_Vector2& CL_Vector2::operator+=(const CL_Vector2& rkVector)
{
    x += rkVector.x;
    y += rkVector.y;
    return *this;
}

CL_Vector2& CL_Vector2::operator-=(const CL_Vector2& rkVector)
{
    x -= rkVector.x;
    y -= rkVector.y;
    return *this;
}

CL_Vector2& CL_Vector2::operator*=(float fScalar)
{
    x *= fScalar;
    y *= fScalar;
    return *this;
}

CL_Vector2& CL_Vector2::operator/=(float fScalar)
{
    if ( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f/fScalar;
        x *= fInvScalar;
        y *= fInvScalar;
    }
    else
    {
        x = FLT_MAX;
        y = FLT_MAX;
    }

    return *this;
}

float CL_Vector2::dot(const CL_Vector2& rkVector) const
{
    return x*rkVector.x + y*rkVector.y;
}

float CL_Vector2::length() const
{
#ifdef __APPLE__
    return sqrt(x*x +y*y);
#else
    return sqrtf(x*x +y*y);
#endif
}

CL_Vector2 CL_Vector2::cross() const
{
    return CL_Vector2(y,-x);
}

CL_Vector2 CL_Vector2::unit_cross() const
{
    CL_Vector2 kCross(y,-x);
    kCross.unitize();
    return kCross;
}

float CL_Vector2::unitize(float fTolerance)
{
    float fLength = length();

    if ( fLength > fTolerance )
    {
        float fInvLength = 1.0f/fLength;
        x *= fInvLength;
        y *= fInvLength;
    }
    else
    {
        fLength = 0.0f;
    }

    return fLength;
}

/*
void CL_Vector2::Orthonormalize (CL_Vector2 akVector[2])	// CL_Vector2 akVector[2]
{
    // If the input vectors are v0 and v1, then the Gram-Schmidt
    // orthonormalization produces vectors u0 and u1 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //
    // where |A| indicates length of vector A and A*B indicates dot
    // product of vectors A and B.

    // compute u0
    akVector[0].Unitize();

    // compute u1
    float fDot0 = akVector[0].Dot(akVector[1]); 
    akVector[1] -= fDot0*akVector[0];
    akVector[1].Unitize();
}
*/
