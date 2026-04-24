
#pragma once

#include <GL/gl.h>
#include <vector>
#include <algorithm> // for std::swap

struct Texture {
    GLuint id = 0;
    int width = 0;
    int height = 0;

    Texture(int width = 2000, int height = 2000)
        : width(width), height(height) {}

    ~Texture() {
        destroy();
    }

    Texture(const Texture&) = delete;

    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept
        : id(other.id), width(other.width), height(other.height) {

        other.id = 0;
        other.width = 0;
        other.height = 0;
    }

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            destroy();

            id = other.id;
            width = other.width;
            height = other.height;

            other.id = 0;
            other.width = 0;
            other.height = 0;
        }
        return *this;
    }

    void init() {
        if (id != 0) return;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, id, 0);
    }

    void bind() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void resize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;

        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, id, 0);
    }

    void export_data(std::vector<unsigned char>& pixels) {
        pixels.resize(width * height * 4);

        bind();

        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, pixels.data());

        for (int y = 0; y < height / 2; ++y) {
            for (int x = 0; x < width * 4; ++x) {
                std::swap(
                    pixels[y * width * 4 + x],
                    pixels[(height - 1 - y) * width * 4 + x]
                );
            }
        }
    }

    void destroy() {
        if (id != 0) {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }
};
