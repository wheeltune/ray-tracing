//
//  sphere3d.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef sphere3d_h
#define sphere3d_h

namespace Geometry {
    struct Sphere3D {
        Point3D center;
        long double r;
        
        Sphere3D() { }
        Sphere3D(Point3D center, long double r);
    };
    
    Sphere3D::Sphere3D(Point3D center, long double r) : center(center), r(r) { }
}

#endif /* sphere3d_h */
