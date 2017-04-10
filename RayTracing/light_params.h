//
//  light_params.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef light_params_h
#define light_params_h

#include <cmath>

#include "vec3.h"

class LightParams {
public:
    LightParams(long double ambient,
                long double diffuse,
                long double specular,
                Geometry::Vec3 kDistance = Geometry::Vec3(0, 0, 1))
    : ambient_(ambient)
    , diffuse_(diffuse)
    , specular_(specular)
    , distance_(kDistance) { }
    
    LightParams(Geometry::Vec3 ambient,
                Geometry::Vec3 diffuse,
                Geometry::Vec3 specular,
                Geometry::Vec3 distance = Geometry::Vec3(0, 0, 1))
    : ambient_(ambient)
    , diffuse_(diffuse)
    , specular_(specular)
    , distance_(distance) { }
    
    Geometry::Vec3 intensity(Geometry::Vec3 ambient,
                   Geometry::Vec3 diffuse,
                   Geometry::Vec3 specular,
                   long double d2) const {
        
        return (ambient_  * ambient +
                diffuse_  * diffuse +
                specular_ * specular) / (distance_[0] +
                                         distance_[1] * sqrt(d2) +
                                         distance_[2] * d2);
    }
    
protected:
    Geometry::Vec3 ambient_;
    Geometry::Vec3 diffuse_;
    Geometry::Vec3 specular_;
    Geometry::Vec3 distance_;
};

#endif /* light_params_h */
