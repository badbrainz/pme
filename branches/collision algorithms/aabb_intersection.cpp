bool ClipSegment(float min, float max, float a, float b, float d, float& t0, float& t1)
{
  const float threshold = 10;

  if (fabsf(d) < threshold)
  {
    if (d > 0.0f)
      return !(b < min || a > max);
    else
      return !(a < min || b > max);
  }

  float u0, u1;

  u0 = (min - a) / (d);
  u1 = (max - a) / (d);

  if (u0 > u1) 
  {
    float u2 = u1;
    u1 = u0;
    u0 = u2;
  }

  if (u1 < t0 || u0 > t1)
    return false;

  t0 = u0 > t0 ? u0 : t0;
  t1 = u0 < t0 ? u0 : t0;

  if (t1 < t0)
    return false;

  return true; 
}

bool check(const Tuple3f &p1, const Tuple3f &p2)
{
  Tuple3f Near(p1);
  Tuple3f Far(p2);
  Tuple3f Min(boundsdescriptor.getMinEndAABB());
  Tuple3f Max(boundsdescriptor.getMaxEndAABB());

  float t0 = 0, t1 = 5;

  Far -= p1;

  if (!ClipSegment(Min.x, Max.x, p1.x, p2.x, Far.x, t0, t1) ||
      !ClipSegment(Min.y, Max.y, p1.y, p2.y, Far.y, t0, t1) ||
      !ClipSegment(Min.z, Max.z, p1.z, p2.z, Far.z, t0, t1))
    return false;

  /*p1 = Near + Far * t0;
  p2 = Near + Far * t1;*/

  return true;
}

float fscaleFactor = 100;

bool intersect(const Ray &ray) 
{
  Tuple3f ro = ray.GetOrigin();
  Tuple3f rd = ray.GetDestination();
  Tuple3f e  = boundsdescriptor.getExtents();
  Tuple3f d  = (rd - ro) * fscaleFactor;
  Tuple3f c  = ro + d - boundsdescriptor.getCenterAABB();
  Tuple3f a(fabsf(d.x), fabsf(d.y), fabsf(d.z));

  if (fabsf(c.x) > e.x + a.x)
    return false;
  if (fabsf(c.y) > e.y + a.y)
    return false;
  if (fabsf(c.z) > e.z + a.z)
    return false;

  if (fabsf(d.y * c.z - d.z * c.y) > (e.y * a.z) + (e.z * a.y))
    return false;
  if (fabsf(d.z * c.x - d.x * c.z) > (e.z * a.x) + (e.x * a.z))
    return false;
  if (fabsf(d.x * c.y - d.y * c.x) > (e.x * a.y) + (e.y * a.x))
    return false;

  return true;
}

bool intersect(const Tuple3f &start, const Tuple3f &end, const BoundsDescriptor &box, float *time)
{
  float st,et,fst = 0,fet = 1;
  const float *bmin = &box.getMinEndAABB().x;
  const float *bmax = &box.getMaxEndAABB().x;
  const float *si = &start.x;
  const float *ei = &end.x;

  for (int i = 0; i < 3; i++)
  {
    if (*si < *ei) 
    {
      if (*si > *bmax || *ei < *bmin)
        return false
      float di = *ei - *si;
      st = (*si < *bmin)? (*bmin - *si) / di: 0;
      et = (*ei > *bmax)? (*bmax - *si) / di: 1;
    }
    else
    {
      if (*ei > *bmax || *si < *bmin)
        return false;
      float di = *ei - *si;
      st = (*si > *bmax)? (*bmax - *si) / di: 0;
      et = (*ei < *bmin)? (*bmin - *si) / di: 1;
    }

    if (st > fst) fst = st;
    if (et < fet) fet = et;
    if (fet < fst)
      return false;
    bmin++; bmax++;
    si++; ei++;
  }

  *time = fst;
  return true;
}

AABBIntersection(const BoundsDescriptor &boundsdescriptor, const Ray &ray)
{
  Tuple3f ro = ray.GetOrigin();
  Tuple3f rayDelta = ray.GetDirection() * ray.GetLength();
  bool inside = true;
  float xt, yt, zt;
  
  float minX = boundsdescriptor.getMinEndAABB().x,
        minY = boundsdescriptor.getMinEndAABB().y,
        minZ = boundsdescriptor.getMinEndAABB().z;
  
  float maxX = boundsdescriptor.getMaxEndAABB().x,
        maxY = boundsdescriptor.getMaxEndAABB().y,
        maxZ = boundsdescriptor.getMaxEndAABB().z;
  
  if(ro.x < minX)
  {
    xt = minX - ro.x;
    
    if(xt > rayDelta.x)
      return false;
    
    xt /= rayDelta.x; 
    inside = false;
  } 
  else if(ro.x > minX)
  {
    xt = maxX - ro.x;
    
    if(xt < rayDelta.x)
      return false;
    
    xt /= rayDelta.x;
    inside = false;
  } 
  else
  {
    xt = -1.0f; 
  }

  if(ro.y < minY)
  {
    yt = minY - ro.y;
    
    if(yt > rayDelta.y)
      return false;

    yt /= rayDelta.y;
    inside = false;
  } 
  else if(ro.y > maxY)
  {
    yt = maxY - ro.y;
    
    if(yt < rayDelta.y)
      return false;
    
    yt /= rayDelta.y;
    inside = false;
  } 
  else
  {
    yt = -1.0f;
  }

  if(ro.z < minZ)
  {
    zt = minZ - ro.z;
    
    if(zt > rayDelta.z)
      return false;
    
    zt /= rayDelta.z;
    inside = false;
  } 
  else if(ro.z > maxZ)
  {
    zt = maxZ - ro.z;
    
    if(zt < rayDelta.z)
      return false;
    
    zt /= rayDelta.z;
    inside = false;
  } 
  else
  {
    zt = -1.0f;
  }

  if(inside)
    return true;

  float t = xt;
  
  if(yt > t)
    t = yt;
  
  if(zt > t)
    t = zt;

  if(t == xt)
  {
    float y = ro.y + rayDelta.y * t;
    float z = ro.z + rayDelta.z * t;
  
    if(y < minY || y > maxY)
      return false;
    else if(z < minZ || z > maxZ)
      return false;
  }
  else if(t == yt)
  {
    float x = ro.x + rayDelta.x * t;
    float z = ro.z + rayDelta.z * t;

    if(x < minX || x > maxX)
      return false;
    else if(z < minZ || z > maxZ) 
      return false;
  }
  else
  {
    float x = ro.x + rayDelta.x * t;
    float y = ro.y + rayDelta.y * t;

    if(x < minX || x > maxX)
      return false;
    else if(y < minY || y > maxY)
      return false;
  }

  return true;
}