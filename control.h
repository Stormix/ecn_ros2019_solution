#include <algorithm>
#include <math.h>

double absSat(double v, double bound)
{
  return (v < -bound) ? -bound : (bound < v) ? bound : v;
}

struct Cmd {double v, w;};
Cmd command(double x1, double y1,double x2, double y2)
{
  const double d1(sqrt(x1*x1 + y1*y1));
  const double d2(sqrt(x2*x2 + y2*y2));

  const double k1(2/std::min(1., d1));
  double vx = k1*(x1 - 1);
  double vy = k1*(y1 - 1);

  if(d2 < 2)
  {
    const double k2(2);
    vx -= k2 * x2 * (2-d2);
    vy -= k2 * y2 * (2-d2);
  }
  return {absSat(vx, 3), absSat(2*vy, 3)};
}
