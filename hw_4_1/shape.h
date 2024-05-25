#pragma once

#include <cmath>

enum class ShapeType {
    line,
    sqr,
    cube,
    circle,
    cylinder
};

// Класс, описывающий точку.
class Point {
public:
    Point();
    Point(int x, int y, int z = 0);
    int x;
    int y;
    int z;
};

// Интерфейс фигуры.
class IShape {
public:
    ShapeType get_type() const;;

    virtual double get_volume() const;
    virtual double get_square() const;
    virtual double get_height() const;
    virtual double get_radius() const;

protected:
    IShape() {}
    ShapeType type;
};

// Интерфейс преобразования.
class ITransformable {
public:
    virtual void shift(int x, int y, int z = 0) {};
    virtual void scaleX(int x) {};
    virtual void scaleY(int y) {};
    virtual void scaleZ(int z) {};
    virtual void scale(int s) {};
};


// Линия.
class Line : public IShape, public ITransformable {
public:
    Line(Point p1, Point p2);;

    void shift(int x, int y, int z) override;

    void scaleX(int x) override;

    void scaleY(int y) override;

    void scale(int s) override;

    void print();

private:
    Point _p1;
    Point _p2;
};

// Квадрат.
class Sqr : public IShape, public ITransformable {
public:
    Sqr(Point center, int w, int h);

    double get_square() const override;

    void shift(int x, int y, int z) override;

    void scaleX(int x) override;

    void scaleY(int y) override;

    void scale(int s) override;

private:
    Point _center;
    int _w;
    int _h;
};

// Куб.
class Cube : public IShape, public ITransformable {
public:
    Cube(Point center, int w, int h, int z);

    double get_square() const override;
    double get_volume() const override;
    double get_height() const override;

    void shift(int x, int y, int z) override;
    void scaleX(int x) override;
    void scaleY(int y) override;
    void scaleZ(int z) override;
    void scale(int s) override;

private:
    Point _center;
    int _w;
    int _h;
    int _z;
};

// Круг.
class Circle : public IShape {
public:
    Circle(Point center, int r);

    double get_square() const override;
    double get_radius() const override;

private:
    Point center;
    int r;
};

// Цилиндр.
class Cylinder : public IShape {
public:
    Cylinder(Point center, int r, int z);

    double get_volume() const override;
    double get_square() const override;
    double get_height() const override;
    double get_radius() const override;

private:
    Point center;
    int r;
    int z;
};


