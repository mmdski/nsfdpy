#ifndef NSFD_VECTOR_HPP_
#define NSFD_VECTOR_HPP_

namespace nsfd {
struct Vector {
  double x;
  double y;

  Vector() : x{0}, y{0} {}
  Vector(double x = 0, double y = 0) : x{x}, y{y} {}

  Vector& operator+=(const Vector& rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }

  friend Vector operator+(Vector lhs, const Vector& rhs) {
    lhs += rhs;
    return lhs;
  }
};
}  // namespace nsfd

#endif
