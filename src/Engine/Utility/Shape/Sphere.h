#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(const Vec3& pos, const float radius) :
		pos_(pos), radius_(radius){}
	virtual bool Intersect(const Shape& other);
	float GetRadius(){return radius_;}
	Vec3 GetPos(){return pos_;}

private:
	float radius_;
	Vec3 pos_;
};