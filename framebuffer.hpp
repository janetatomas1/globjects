#pragma once

#include <GL/glew.h>
#include <cstdio>

struct Framebuffer {
    GLuint fbo = 0;
    GLuint rbo = 0;
    int width = 0;
    int height = 0;

    Framebuffer(int w, int h) : width(w), height(h) {
        glGenFramebuffers(1, &fbo);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool check() {
        bind();
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    void clear() {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void destroy() {
        if (rbo) glDeleteRenderbuffers(1, &rbo);
        if (fbo) glDeleteFramebuffers(1, &fbo);

        fbo = rbo = 0;
    }
};
