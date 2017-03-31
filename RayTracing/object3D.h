//
//  Object3D.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef Object3D_h
#define Object3D_h

using namespace Geometry;

class Object3D {
public:
    
    virtual bool intersect(Point3D start, Point3D finish, Point3D* crossPoint) {
        return false;
    }
    virtual SDL_Color getColor(const Point3D& point) {
        return color_;
    }
    virtual Point3D getNormal(const Point3D& point) {
        return Point3D(0, 0, 0);
    }
protected:
    Object3D(SDL_Color color) : color_(color) { }
    virtual ~Object3D() { }
private:
    SDL_Color color_;
};

#endif /* Object3D_h */
