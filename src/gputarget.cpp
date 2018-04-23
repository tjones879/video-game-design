#include "inc/gputarget.hpp"
#include <vector>
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

GPU_Image *generateFBO(NVGcontext *_vg, const float _w, const float _h,
                       void (*draw)(NVGcontext *, const float, const float, const float, const float))
{
    GPU_FlushBlitBuffer();
    NVGLUframebuffer *fb = nvgluCreateFramebuffer(_vg, _w, _h, NVG_IMAGE_NODELETE);
    nvgluBindFramebuffer(fb);
    glViewport(0, 0, _w, _h);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    nvgBeginFrame(_vg, _w, _h, 1.0f);
    draw(_vg, 0, 0, _w, _h);
    nvgEndFrame(_vg);
    nvgluBindFramebuffer(0);
    GPU_ResetRendererState();
    GPU_Image *return_image = GPU_CreateImageUsingTexture(fb->texture, false);
    nvgluDeleteFramebuffer(fb);
    return return_image;
}

void drawNVG(NVGcontext *vg, const float x, const float y, const float w, const float h) {
    nvgBeginPath(vg);
    nvgRect(vg, 100,100, 120,30);
    nvgCircle(vg, 120,120, 5);
    nvgPathWinding(vg, NVG_HOLE);   // Mark circle as a hole.
    nvgFillColor(vg, nvgRGBA(255,192,0,255));
    nvgFill(vg);
}

GPUTarget::GPUTarget(SDL_Window **window)
{
    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
    screen = GPU_Init(640, 480, GPU_DEFAULT_INIT_FLAGS);
    *window = SDL_GetWindowFromID(screen->context->windowID);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
}

GPUTarget::~GPUTarget()
{
    GPU_Quit();
}

SDL_Window *GPUTarget::getWindow()
{
    SDL_Window *window = SDL_GetWindowFromID(screen->context->windowID);
    return window;
}

GPUTarget::operator GPU_Target *()
{
    return screen;
}

void GPUTarget::createGUI(const uint16_t screen_w,
                          const uint16_t screen_h)
{
    GPU_Rect fbo_simple_rect = { 65.0f, 10.0f, 50.0f, 50.0f };
    auto fbo_simple = generateFBO(vg, fbo_simple_rect.w, fbo_simple_rect.h, drawNVG);

    GPU_BlitRectX(fbo_simple, NULL, screen, &fbo_simple_rect, 0, 0, 0, GPU_FLIP_VERTICAL);
    GPU_FlushBlitBuffer();
    nvgBeginFrame(vg, screen_w, screen_h, 1);
    drawNVG(vg, 10, 10, fbo_simple_rect.w, fbo_simple_rect.h);
    nvgEndFrame(vg);
    GPU_ResetRendererState();
}
