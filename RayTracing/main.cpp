//
//  main.cpp
//  RayTracing
//
//  Created by wheeltune on 09.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#include <iostream>
#include <SDL2/sdl.h>
#include <OpenGL/gl3.h>
#include "ray.h"
#include "geometry.h"
#include "objects.h"

using namespace Geometry;

int main(int argc, const char * argv[]) {
    
    int a = 20;
    int& b = a;
    a = 10;
    std::cout << b;
    
    RayTracer rayTracer(
                        Point3D(0, 0, -500),
                        Window(
                               Point3D(-400, -300, 0),
                               Point3D(400, -300, 0),
                               Point3D(-400, 300, 0)
                               )
                        );
    
    if (!rayTracer.start()) {
        return EXIT_FAILURE;
    }
    
    rayTracer.addObject(new Sphere(
                                   Point3D(400, 300, 900),
                                   200,
                                   Material(Vec3(0.25, 0.41, 0.93), Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1), 1)
                                   )
                        );
    
    
    rayTracer.addObject(new Sphere(
                                   Point3D(0, 0, 500),
                                   100,
                                   Material(Vec3(1, 0.55, 0), Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1), 1)
                                   )
                        );

    rayTracer.addObject(new Sphere(
                                   Point3D(-100, 0, 500),
                                   100,
                                   Material(Vec3(0.86, 0.08, 0), Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1), 1)
                                   )
                        );
    
    rayTracer.addObject(new Sphere(
                                   Point3D(100, 0, 500),
                                   100,
                                   Material(Vec3(1, 0.84, 0), Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1), 1)
                                   )
                        );
    
    rayTracer.addObject(new Sphere(
                                   Point3D(-50, 100, 500),
                                   100,
                                   Material(Vec3(0.2, 0.8, 0.2), Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1), 1)
                                   )
                        );
    
    rayTracer.addObject(new Sphere(
                                   Point3D(50, 100, 500),
                                   100,
                                   Material(Vec3(0, 0.75, 1), Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1), 1)
                                   )
                        );
    
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, -400, 0), Point3D(-500, -400, 1000), Point3D(-500, 400, 1000), Point3D(-500, 400, 0) },
                                    Material(Vec3(0.16, 0.73, 0.6), Vec3(0.2, 0.2, 0.2), Vec3(1, 1, 1))
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(500, -400, 0),
                                                  Point3D(500, -400, 1000),
                                                  Point3D(500, 400, 1000),
                                                  Point3D(500, 400, 0) },
                                    Material(Vec3(0.16, 0.73, 0.6), Vec3(0.2, 0.2, 0.2), Vec3(1, 1, 1))
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, -400, 1000), Point3D(500, -400, 1000), Point3D(500, 400, 1000), Point3D(-500, 400, 1000) },
                                    Material(Vec3(0.16, 0.73, 0.6), Vec3(0.2, 0.2, 0.2), Vec3(1, 1, 1))
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, -400, 0), Point3D(-500, -400, 1000), Point3D(500, -400, 1000), Point3D(500, -400, 0) },
                                    Material(Vec3(0.99, 0.99, 0.99), Vec3(0.01, 0.01, 0.01), Vec3(1, 1, 1))
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, 400, 0), Point3D(-500, 400, 1000), Point3D(500, 400, 1000), Point3D(500, 400, 0) },
                                    Material(Vec3(0.79, 0.41, 0.14), Vec3(0.1, 0.1, 0.1), Vec3(1, 1, 1))
                                    )
                        );
    
    rayTracer.addLight(new Light(Point3D(0, -350, 250), LightParams(0, 100000, 1000)));
    rayTracer.addLight(new Light(Point3D(0, -350, 600), LightParams(0, 100000, 1000)));
    
    rayTracer.draw();
    
    while(!rayTracer.shouldClose()) {}
    rayTracer.stop();
    
    return EXIT_SUCCESS;
}
