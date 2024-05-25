#include"shape.h"

#include <iostream>

ShapeType IShape::get_type() const { return type; }

double IShape::get_volume() const { return 0.0; }

double IShape::get_square() const { return 0.0; }

double IShape::get_height() const { return 0.0; }

double IShape::get_radius() const { return 0.0; }

Line::Line(Point p1, Point p2):_p1(p1),_p2(p2) {
    type = ShapeType::line;
}

void Line::shift(int x, int y, int z) {
    _p1.x += x;
    _p1.y += y;
    _p2.x += x;
    _p2.y += y;
}

void Line::scaleX(int x) {
    _p1.x *= x;
    _p2.x *= x;
}

void Line::scaleY(int y) {
    _p1.y *= y;
    _p2.y *= y;
}

void Line::scale(int s) {
    _p1.x *= s;
    _p1.y *= s;
    _p2.x *= s;
    _p2.y *= s;
}

void Line::print()
{
    std::cout << _p1.x << ":" << _p1.y << " -> " << _p2.x << ":" << _p2.y << std::endl;
}

Sqr::Sqr(Point center, int w, int h):_center(center),_w(w),_h(h){
    type = ShapeType::sqr;
}

double Sqr::get_square() const { return _w * _h; }

void Sqr::shift(int x, int y, int z) {
    _center.x += x;
    _center.y += y;
}

void Sqr::scaleX(int x) {
    _w *= x;
}

void Sqr::scaleY(int y) {
    _h *= y;
}

void Sqr::scale(int s) {
    _w *= s;
    _h *= s;
}

Cube::Cube(Point center, int w, int h, int z)
    :_w(w),_h(h),_z(z) {
    type = ShapeType::cube;
}

double Cube::get_square() const { return 2 * _w * _h + 2 * _z * _w + 2 * _z * _h; }

double Cube::get_volume() const { return _w * _h * _z; }

double Cube::get_height() const { return _z; }

void Cube::shift(int x, int y, int z) {
    _center.x += x;
    _center.y += y;
    _center.z += z;
}

void Cube::scaleX(int x) {
    _w *= x;
}

void Cube::scaleY(int y) {
    _h *= y;
}

void Cube::scaleZ(int z) {
    _z *= z;
}

void Cube::scale(int s) {
    _w *= s;
    _h *= s;
    _z *= s;
}

Circle::Circle(Point center, int r):center(center), r(r) {
    type = ShapeType::circle;
}

double Circle::get_square() const { return M_PI * std::pow(r,2); }

double Circle::get_radius() const { return r; }

Cylinder::Cylinder(Point center, int r, int z):center(center), r(r), z(z) {
    type = ShapeType::cylinder;
}

double Cylinder::get_volume() const { return M_PI * std::pow(r,2) * z; }

double Cylinder::get_square() const { return 2 * M_PI * std::pow(r,2) + 2 * M_PI * r * z; }

double Cylinder::get_height() const { return z; }

double Cylinder::get_radius() const { return r; }

Point::Point()
{

}

Point::Point(int x, int y, int z):x(x), y(y), z(z){}
