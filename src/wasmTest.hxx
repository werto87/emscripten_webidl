#ifndef D36D6A4C_6616_4E85_9BB2_212824D9378A
#define D36D6A4C_6616_4E85_9BB2_212824D9378A
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <vector>

struct Point
{
  int x{};
  int y{};
};

struct SomeArray
{
  std::vector<Point> points{ Point{}, Point{}, Point{}, Point{} };
  Point &
  at (size_t index)
  {
    return points.at (index);
  }
  size_t
  size ()
  {
    return points.size ();
  }
  void
  multiplyPoints (int multiplier)
  {
    std::transform (points.begin (), points.end (), points.begin (), [multiplier] (Point const &point) { return Point{ .x = point.x * multiplier, .y = point.y * multiplier }; });
  };
};

#endif /* D36D6A4C_6616_4E85_9BB2_212824D9378A */
