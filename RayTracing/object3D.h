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
    virtual bool isCross(Point3D start, Point3D finish, Point3D* crossPoint, SDL_Color* crossColor) {
        return false;
    }
};

#endif /* Object3D_h */
