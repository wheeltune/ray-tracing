//
//  Window.h
//  RayTracing
//
//  Created by wheeltune on 10.03.17.
//  Copyright © 2017 wheeltune. All rights reserved.
//

#ifndef Window_h
#define Window_h
#include <SDL2/sdl.h>
#include "geometry.h"

using namespace Geometry;

class Window {
public:
    Window(
           Point3D leftTop,
           Point3D rightTop,
           Point3D leftBottom
           ) : leftTop_(leftTop),
               rightTop_(rightTop),
               leftBottom_(leftBottom),
               rightBottom_(leftBottom + rightTop - leftTop)
    {
        SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    }
    ~Window() {
        SDL_Quit();
    }
    
    bool open() {
        // Check if window is opened
        if (window_ != NULL) {
            printf("Window is already opened\n");
            return false;
        }
        
        // Create an application window with the following settings:
        window_ = SDL_CreateWindow(
                                  "Ray Tracing",                        // window title
                                  SDL_WINDOWPOS_UNDEFINED,              // initial x position
                                  SDL_WINDOWPOS_UNDEFINED,              // initial y position
                                  getPixelWidth(),                               // width, in pixels
                                  getPixelHeight(),                              // height, in pixels
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  // flags - see below
                                  );
        
        // Check on errors
        if (window_ == NULL) {
            printf("Could not create window: %s\n", SDL_GetError());
            return false;
        }
        
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
        
        begin();
        end();
        
        return true;
    }
    
    void close() {
        SDL_DestroyWindow(window_);
        window_ = NULL;
    }
    
    Point3D getPixelPoint(int x, int y) {
        int width = getPixelWidth(), height = getPixelHeight();
        Point3D top = (leftTop_ * (width - x) + rightTop_ * x) / width;
        Point3D bottom = (leftBottom_ * (width - x) + rightBottom_ * x) / width;
        
        return (top * (height - y) + bottom * y) / height;
    }
    
    int getPixelWidth() {
        return (int)(leftTop_ - rightTop_).len();
    }
    
    int getPixelHeight() {
        return (int)(leftTop_ - leftBottom_).len();
    }
    
    bool getEvent(SDL_Event* event) {
        SDL_PumpEvents();
        return SDL_PollEvent(event);
    }
    
    void setPixelColor(int x, int y, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(renderer_, x, y);
    }
    
    void flush() {
        SDL_RenderPresent(renderer_);
    }
    
    void begin() {
        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
        SDL_RenderClear(renderer_);
    }
    
    void end() {
        flush();
    }
    
private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    Point3D leftTop_, rightTop_, leftBottom_, rightBottom_;
};

#endif /* Window_h */
