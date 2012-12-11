#pragma once

#include <vector>
#include <cmath>
#include "math.hxx"
#include <random>
#include <limits>
#include <stdio.h>
#include "kuckirrandom.hxx"

int my_extra_super_counter = 0;

class AbstractLight
{
public:

	virtual Vec3f sampleIllumination(const Vec3f& aSurfPt, const Frame& aFrame, Vec3f& oWig, float& oLightDist) const
	{
		return Vec3f(0);
	}
};

//////////////////////////////////////////////////////////////////////////
class AreaLight : public AbstractLight
{
public:

    AreaLight(
        const Vec3f &aP0,
        const Vec3f &aP1,
        const Vec3f &aP2)
    {
	scale = std::numeric_limits<int>::max();

        p0 = aP0;
        e1 = aP1 - aP0;
        e2 = aP2 - aP0;

        Vec3f normal = Cross(e1, e2);
        float len    = normal.Length();
        mInvArea     = 2.f / len;
        mFrame.SetFromZ(normal);
    }

private:
/*	const float next_random_float() const
	{
		int a = my_extra_super_counter;
		a = (a ^ 61) ^ (a >> 16);
		a = a + (a << 3);
		a = a ^ (a >> 4);
		a = a * 0x27d4eb2d;
		a = a ^ (a >> 15);

		my_extra_super_counter = a;

		return ((float) a )/((float) scale);
	
	}*/
public:
	virtual Vec3f sampleIllumination(
		const Vec3f& aSurfPt, 
		const Frame& aFrame, 
		Vec3f& oWig, 
		float& oLightDist) const
	{

		

		//std::random_device r;
		//std::default_random_engine generator(r());
		//std::uniform_real_distribution<float> distribution(0.0,1.0);


		float x = kuckir::kuckir_random::next_random_float(); // distribution(generator);
		float y = kuckir::kuckir_random::next_random_float();//distribution(generator);

		if ( x + y > 1) { x = 1-x; y = 1-y; } //Hmíra kombo

//		printf("%.2f", x);

//		Vec2f c = r.getVec2f();
//		Vec3f x = e1 / ( 2 * c.x);
//		Vec3f y = e2 / ( 2 * c.y);



		Vec3f sampledPt = p0 + x * e1 + y * e2;// + x + y;
		oWig = sampledPt - aSurfPt;

                float distSqr  = oWig.LenSqr();
                oLightDist     = sqrt(distSqr);

                oWig /= oLightDist;

                float cosTheta = Dot(aFrame.mZ, oWig);

                if(cosTheta <= 0)
                        return Vec3f(0);

                return mRadiance * cosTheta / distSqr;


	}	

public:
	int scale;
    Vec3f p0, e1, e2;
    Frame mFrame;
    Vec3f mRadiance;
    float mInvArea;
};

//////////////////////////////////////////////////////////////////////////
class PointLight : public AbstractLight
{
public:

    PointLight(const Vec3f& aPosition)
    {
        mPosition = aPosition;
    }

	virtual Vec3f sampleIllumination(
		const Vec3f& aSurfPt, 
		const Frame& aFrame, 
		Vec3f& oWig, 
		float& oLightDist) const
	{
		oWig           = mPosition - aSurfPt;
		float distSqr  = oWig.LenSqr();
		oLightDist     = sqrt(distSqr);
		
		oWig /= oLightDist;

		float cosTheta = Dot(aFrame.mZ, oWig);

		if(cosTheta <= 0)
			return Vec3f(0);

		return mIntensity * cosTheta / distSqr;
	}

public:

    Vec3f mPosition;
    Vec3f mIntensity;
};


//////////////////////////////////////////////////////////////////////////
class BackgroundLight : public AbstractLight
{
public:
    BackgroundLight()
    {
        mBackgroundColor = Vec3f(135, 206, 250) / Vec3f(255.f);
    }

public:

    Vec3f mBackgroundColor;
};
