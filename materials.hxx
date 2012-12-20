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

	Vec3f evalBrdf( const Vec3f& wil, const Vec3f& wol, float prob ) const
	{
		if( wil.z <= 0 && wol.z <= 0)
			return Vec3f(0);

		float a = mDiffuseReflectance.Max();
		float b = mPhongReflectance.Max();
		
		float coeffA = (a / (a + b));
		float coeffB = (b / (a + b));
		
		Vec3f N = Vec3f(0,0,1);
		Vec3f R = 2*Dot(N,wil)*N-wil;

		Vec3f diffuseComponent = mDiffuseReflectance / PI_F;

		Vec3f glossyComponent  = std::pow( Dot(wol, R), mPhongExponent ) * mPhongReflectance *(mPhongExponent+2) / (2*PI_F);

		if (prob < coeffA)
			return diffuseComponent;
		else
			return glossyComponent;

		//return diffuseComponent + glossyComponent;
	}

	
	Vec3f evalBrdf( const Vec3f& wil, const Vec3f& wol ) const
	{
		if( wil.z <= 0 && wol.z <= 0)
			return Vec3f(0);

		Vec3f N = Vec3f(0,0,1);
		Vec3f R = 2*Dot(N,wil)*N-wil;

		Vec3f diffuseComponent = mDiffuseReflectance / PI_F;

		Vec3f glossyComponent  = std::pow( Dot(wol, R), mPhongExponent ) * mPhongReflectance *(mPhongExponent+2) / (2*PI_F);

		return diffuseComponent + glossyComponent;
	}

	
	Vec3f sampleRay(const Vec2f& samples, float prob, Vec3f wol, float* oPdfW) const
	{
		float a = mDiffuseReflectance.Max();
		float b = mPhongReflectance.Max();

		//float a = mDiffuseReflectance.Length();
		//float b = mDiffuseReflectance.Length();

		if ((a+b) == 0)
		{
			*oPdfW = 1e36f;
			return Vec3f(0);
		}
		
		float coeffA = (a / (a + b));
		float coeffB = (b / (a + b));

		Vec3f R = Vec3f(-wol.x, -wol.y, wol.z);
		Frame fr = Frame();
		fr.SetFromZ(R);

		Vec3f wil;
		Vec3f nr;
		if (prob < coeffA)
		{
			wil = SampleCosHemisphereW(samples, oPdfW);
			*oPdfW = (*oPdfW) * coeffA;
			nr = wil;
		}
		else
		{
			wil = SamplePowerCosHemisphereW(samples, mPhongExponent, oPdfW);
			*oPdfW = (*oPdfW) * coeffB;
			nr = fr.ToWorld(wil);
		}
		
		return nr;
	}	

    Vec3f mDiffuseReflectance;
    Vec3f mPhongReflectance;
    float mPhongExponent;
};
