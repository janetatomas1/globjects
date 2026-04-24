
#pragma once

#include <GL/gl.h>

struct Framebuffer {
    GLuint fbo = 0;
    GLuint rbo = 0;

    int width = 0;
    int height = 0;

    Framebuffer(int width = 2000, int height = 2000)
        : width(width), height(height) {}

    ~Framebuffer() {
        destroy();
    }

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&& other) noexcept
        : fbo(other.fbo), rbo(other.rbo),
          width(other.width), height(other.height) {
        other.fbo = 0;
        other.rbo = 0;
        other.width = 0;
        other.height = 0;
    }

    Framebuffer& operator=(Framebuffer&& other) noexcept {
        if (this != &other) {
            destroy();

            fbo = other.fbo;
            rbo = other.rbo;
            width = other.width;
            height = other.height;

            other.fbo = 0;
            other.rbo = 0;
            other.width = 0;
            other.height = 0;
        }
        return *this;
    }

    void init() {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    void resize(int width, int height) {
        this->width = width;
        this->height = height;

        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void destroy() {
        if (rbo != 0) {
            glDeleteRenderbuffers(1, &rbo);
            rbo = 0;
        }
        if (fbo != 0) {
            glDeleteFramebuffers(1, &fbo);
            fbo = 0;
        }
    }
};
