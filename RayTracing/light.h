//
//  light_params.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef light_h
#define light_h

#include "light_params.h"
#include "object3d.h"

class Light {
public:
    Light(Geometry::Point3D position, LightParams params) : lightParams_(params), position_(position) { }
    
    Geometry::Vec3 intencityAt(const Geometry::Point3D& point, const Object3D& object, const Geometry::Point3D& origin) {
        Material material = object.material();
        
        Geometry::Point3D n = object.normalAt(point).normalize();
        Geometry::Point3D v = (origin - point).normalize();
        Geometry::Point3D l = (position_ - point).normalize();
        Geometry::Point3D r = 2 * (n * l) * n - l;
        long double cos_r_v = std::max((long double) 0, r * v);
        
        Geometry::Vec3 intensity = lightParams_.intensity(material.ambient(),
                                                          material.diffuse() * (l * n),
                                                          material.specular() * std::pow(cos_r_v, material.shine()),
                                                          (position_ - point).len2());
        
        return intensity;
    }

    
    Geometry::Point3D position() {
        return position_;
    }
private:
    Geometry::Point3D position_;
    LightParams lightParams_;
};

#endif /* light_h */
