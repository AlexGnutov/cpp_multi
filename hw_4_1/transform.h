#pragma once
#include"shape.h"


// Класс для выполнения трансформаций.
class Transform
{
public:
    Transform();

    void set_shift(int x, int y, int z) {
        shift_x = x;
        shift_y = y;
        shift_z = z;
    };

    void set_scaleX(int x) {
        scale_x = 1;
    };

    void set_scaleY(int y) {
        scale_y = 1;
    };

    void set_scaleZ(int z) {
        scale_z = 1;
    };

    void set_scale(int s) {
        scale_x *= s;
        scale_y *= s;
        scale_z *= s;
    };

    Line transformed(const Line& shp) {
        Line *l = new Line(shp);
        transform(l);
        Line out(*l);
        delete l;
        return out;
    };

    Sqr transformed(const Sqr& shp) {
        Sqr *q = new Sqr(shp);
        transform(q);
        Sqr out(*q);
        delete q;
        return out;
    };

    Cube transformed(Cube shp) {
        Cube *c = new Cube(shp);
        transform(c);
        Cube out(*c);
        delete c;
        return out;
    };

private:
    void transform(ITransformable *shp) {
        if (shift_x != 0 || shift_y != 0 || shift_z != 0) {
            shp->shift(shift_x, shift_y, shift_z);
        }
        if (scale_x != 1) {
            shp->scaleX(scale_x);
        }
        if (scale_y != 1) {
            shp->scaleY(scale_y);
        }
        if (scale_z != 1) {
            shp->scaleZ(scale_z);
        }
    }


    int shift_x = 0;
    int shift_y = 0;
    int shift_z = 0;

    int scale_x = 1;
    int scale_y = 1;
    int scale_z = 1;

};

