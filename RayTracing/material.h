//
//  material.h
//  RayTracing
//
//  Created by wheeltune on 10.04.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:
    Material(Geometry::Vec3 ambient,
             Geometry::Vec3 diffuse,
             Geometry::Vec3 specular,
             long double shine = 1,
             Geometry::Vec3 emit = Geometry::Vec3(0, 0, 0),
             Geometry::Vec3 transparency = Geometry::Vec3(0, 0, 0))
    : ambient_(ambient)
    , diffuse_(diffuse)
    , specular_(specular)
    , shine_(shine)
    , emit_(emit)
    , transparency_(transparency) { }
    
    Geometry::Vec3 ambient() const {
        return ambient_;
    }
    
    Geometry::Vec3 diffuse() const {
        return diffuse_;
    }
    
    Geometry::Vec3 specular() const {
        return specular_;
    }
    
    Geometry::Vec3 emit() const {
        return emit_;
    }
    
    Geometry::Vec3 transparency() const {
        return transparency_;
    }
    
    long double shine() const {
        return shine_;
    }
    
private:
    Geometry::Vec3 ambient_;
    Geometry::Vec3 diffuse_;
    Geometry::Vec3 specular_;
    Geometry::Vec3 emit_;
    Geometry::Vec3 transparency_;
    long double shine_;
};

#endif /* MATERIAL_H */
