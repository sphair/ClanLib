//===============================================================================
//
// LinearParticle Copyright (c) 2006 Wong Chin Foo
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in a
// product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//===============================================================================


#ifndef L_DEFINATION_H
#define L_DEFINATION_H

#include <cstdlib>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cstdio>

//#define L_USING_DOUBLE


#ifdef L_DEBUG_MODE
	#include <iostream>
#endif


#ifdef L_USING_DOUBLE
	#define L_REAL double
	#define L_SIN(x) sin(x)
	#define L_COS(x) cos(x)
	#define L_SQRT(x) sqrt(x)
	#define L_ATAN2(y,x) atan2(y,x)
	#define L_REAL_MIN DBL_MIN

#else
	#define L_REAL float
	#define L_SIN(x) sinf(x)
	#define L_COS(x) cosf(x)
	#define L_SQRT(x) sqrtf(x)
	#define L_ATAN2(y,x) atan2f(y,x)
	#define L_REAL_MIN FLT_MIN
#endif


#define L_PARTICLE_TYPE_LIMIT 8

const L_REAL L_PI = 3.1415926535898f;
const L_REAL L_INVERSED_PI = 0.31830988618379f;
const L_REAL L_2PI = 6.28318530717959f;


#define L_RADIAN_TO_DEGREE(x) (x*57.2957795130823)
#define L_DEGREE_TO_RADIAN(x) (x*0.0174532925199433)


#define L_INFINITE_LIFE INT_MAX


const L_REAL L_ONE_OVER_RAND_MAX_1 = 1 / (L_REAL)RAND_MAX;
const L_REAL L_ONE_OVER_RAND_MAX_2 = 1 / ( L_REAL(RAND_MAX) + 1 );

// 0 <= random value <= 1
#define L_RAND_REAL_1() ( (L_REAL)rand() * L_ONE_OVER_RAND_MAX_1 )

// 0 <= random value < 1
#define L_RAND_REAL_2() ( (L_REAL)rand() * L_ONE_OVER_RAND_MAX_2 )


enum L_BlendingMode
{
	L_NORMAL_BLENDING,
	L_ADDITIVE_BLENDING
};

#define L_SET_NORMAL_BLENDING(x) /*(x).set_blend_func(blend_src_alpha,blend_one_minus_src_alpha)*/
#define L_SET_ADDITIVE_BLENDING(x) /*(x).set_blend_func(blend_src_alpha,blend_one)*/


//A hack to add blending function

namespace
{
   
   class L_Blend
   {
   protected:
      CL_BlendMode blend;
   public:
      L_Blend()
      {
	 blend.enable_blending(true);
	 blend.set_blend_function(cl_blend_src_alpha, cl_blend_one, cl_blend_src_alpha, cl_blend_one);
      }

      const CL_BlendMode &getBlend() const
      {
	 return blend;
      }
   };


   class L_BlendMinusAlpha : public L_Blend
   {
   public:
      L_BlendMinusAlpha()
      {
	 blend.enable_blending(true);
	 blend.set_blend_function(cl_blend_src_alpha, cl_blend_one_minus_src_alpha, cl_blend_src_alpha, cl_blend_one_minus_src_alpha);
      }
   };

   static L_Blend l_blend;
   static L_BlendMinusAlpha l_blendMinusAlpha;
};


template<class T>
inline void L_DrawParticleBlend(CL_GraphicContext &gc,const L_Blend &blend, T *EmitterOrEffectorParticle)	 
{				      
   gc.set_blend_mode(blend.getBlend());	 
   EmitterOrEffectorParticle->draw(gc);		  
   gc.reset_blend_mode();	      
}


template<class T>
inline void L_DrawParticleBlend(CL_GraphicContext &gc,const L_Blend &blend, T &EmitterOrEffectorParticle)	 
{				      
   gc.set_blend_mode(blend.getBlend());	 
   EmitterOrEffectorParticle.draw(gc);		  
   gc.reset_blend_mode();	      
}

template<class T>
inline void L_DrawParticle(CL_GraphicContext &gc, T &EmitterOrEffectorParticle)	 
{				      
   L_DrawParticleBlend(gc, l_blend, EmitterOrEffectorParticle);
}

template<class T>
inline void L_DrawParticleMinusAlpha(CL_GraphicContext &gc, T &EmitterOrEffectorParticle)	 
{				      
   L_DrawParticleBlend(gc, l_blendMinusAlpha, EmitterOrEffectorParticle);
}


#endif
