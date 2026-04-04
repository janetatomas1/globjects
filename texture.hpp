
#pragma once

#include <GL/glew.h>
#include <vector>

struct Texture {
    GLuint id = 0;
    int width = 0;
    int height = 0;
    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;

    Texture() = default;

    Texture(
        int w,
        int h,
        GLenum internalFmt = GL_RGBA8,
        GLenum fmt = GL_RGBA,
        GLenum t = GL_UNSIGNED_BYTE
    ): width(w), height(h),
          internalFormat(internalFmt),
          format(fmt), type(t)
    {
        glGenTextures(1, &id);
        bind();

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
                     width, height, 0,
                     format, type, nullptr);
        set_default_params();
    }

    void set_default_params() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void bind(GLuint unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void upload(const void* data) {
        bind();
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0, 0, 0,
            width, height,
            format, type, data
        );
    }

    // Download full data
    void download(std::vector<unsigned char> &data) const {
        bind();
        glGetTexImage(
            GL_TEXTURE_2D,
            0,
            format,
            type,
            data.data()
        );
    }

    int channels() const {
        switch (format) {
            case GL_RED: return 1;
            case GL_RG: return 2;
            case GL_RGB: return 3;
            case GL_RGBA: return 4;
            default: return 4;
        }
    }

    void destroy() {
        if (id) {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }
};
