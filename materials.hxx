#pragma once 

#include "math.hxx"

class Material
{
public:
    Material()
    {
        Reset();
    }

    void Reset()
    {
        mDiffuseReflectance = Vec3f(0);
        mPhongReflectance   = Vec3f(0);
        mPhongExponent      = 1.f;
    }

	Vec3f evalBrdf( const Vec3f& wil, const Vec3f& n, const Vec3f& wol ) const
	{
		if( wil.z <= 0 && wol.z <= 0)
			return Vec3f(0);

		Vec3f diffuseComponent = mDiffuseReflectance / PI_F;

		Vec3f reflected = 2 * Dot(wol, n) * n - wol;

		Vec3f glossyComponent  = (2 + mPhongExponent) * mPhongReflectance * std::pow(Dot(reflected, wil), mPhongExponent) * (0.5f / PI_F) ;

		return diffuseComponent + glossyComponent ;
	}

    Vec3f mDiffuseReflectance;
    Vec3f mPhongReflectance;
    float mPhongExponent;
};
