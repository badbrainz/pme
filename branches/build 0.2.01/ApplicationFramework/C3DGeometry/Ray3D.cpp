#include "Ray3D.h"

Ray3D::Ray3D()
{}

Ray3D::Ray3D(const Tuple3f &Po, const Tuple3f &Pd)
{
  origin = Po;
  SetDestination(Pd);
}

void Ray3D::Set(const Tuple3f &Po, const Tuple3f &Pd)
{
  SetOrigin(Po);
  SetDestination(Pd);
}

void Ray3D::SetOrigin(const Tuple3f &Po)
{
  origin = Po;
}

void Ray3D::SetDestination(const Tuple3f &Pd)
{
  destination = Pd;
}

void Ray3D::NormalizeDirection(void)
{
  destination.normalize();
}

const Tuple3f &Ray3D::GetOrigin(void) const
{
  return origin;
}

const Tuple3f &Ray3D::GetDirection(void) const
{
  Tuple3f direction(destination);
  return direction.normalize();
}

const Tuple3f &Ray3D::GetDestination(void) const
{
  return destination;
}

float Ray3D::GetLength(void)
{
  return (origin-destination).getLength();
}

Ray3D::~Ray3D()
{}