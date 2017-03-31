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

using namespace Geometry;

int main(int argc, const char * argv[]) {
    
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
                                   SDL_Color{65, 105, 237, 255}
                                   )
                        );
    
    
    rayTracer.addObject(new Sphere(
                                   Point3D(0, 0, 500),
                                   100,
                                   SDL_Color{255, 140, 0, 255}
                                   )
                        );
    
    rayTracer.addObject(new Sphere(
                                   Point3D(-100, 0, 500),
                                   100,
                                   SDL_Color{220, 20, 0, 255}
                                   )
                        );
    
    rayTracer.addObject(new Sphere(
                                   Point3D(100, 0, 500),
                                   100,
                                   SDL_Color{255, 215, 0, 255}
                                   )
                        );
    
    rayTracer.addObject(new Sphere(
                                   Point3D(-50, 100, 500),
                                   100,
                                   SDL_Color{50, 205, 50, 255}
                                   )
                        );
    
    rayTracer.addObject(new Sphere(
                                   Point3D(50, 100, 500),
                                   100,
                                   SDL_Color{0, 191, 255, 255}
                                   )
                        );
    
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, -400, 0), Point3D(-500, -400, 1000), Point3D(-500, 400, 1000), Point3D(-500, 400, 0) },
                                    SDL_Color{42, 187, 155, 255}
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(500, -400, 0),
                                                  Point3D(500, -400, 1000),
                                                  Point3D(500, 400, 1000),
                                                  Point3D(500, 400, 0) },
                                    SDL_Color{42, 187, 155, 255}
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, -400, 1000), Point3D(500, -400, 1000), Point3D(500, 400, 1000), Point3D(-500, 400, 1000) },
                                    SDL_Color{42, 187, 155, 255}
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, -400, 0), Point3D(-500, -400, 1000), Point3D(500, -400, 1000), Point3D(500, -400, 0) },
                                    SDL_Color{242, 241, 239, 255}
                                    )
                        );
    rayTracer.addObject(new Quadrangle(
                                    (Point3D[4]){ Point3D(-500, 400, 0), Point3D(-500, 400, 1000), Point3D(500, 400, 1000), Point3D(500, 400, 0) },
                                    SDL_Color{202, 105, 36, 255}
                                    )
                        );
    
    rayTracer.addLight(new DotLight(Point3D(0, -350, 250), 1000000));
    rayTracer.addLight(new DotLight(Point3D(0, -350, 750), 1000000));
    
    rayTracer.draw();
    
    while(!rayTracer.shouldClose()) {}
    rayTracer.stop();
    
    return EXIT_SUCCESS;
}
