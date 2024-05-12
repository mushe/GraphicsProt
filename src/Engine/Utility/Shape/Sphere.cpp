#include "Sphere.h"

bool Sphere::Intersect(const Shape& other) 
{

  auto otherSphere = dynamic_cast<const Sphere*>(&other);
  if(otherSphere) {
    return length((otherSphere->pos_ - pos_)) < otherSphere->radius_ + radius_;
  }

  return false;
}