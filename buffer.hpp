#pragma once

#include <GL/gl.h>
#include <vector>

struct Buffer {
    GLuint VAO = 0;
    std::vector<GLuint> VBOs;

    Buffer(size_t numAttributes) {
        glGenVertexArrays(1, &VAO);
        VBOs.resize(numAttributes);
        glGenBuffers(static_cast<GLsizei>(numAttributes), VBOs.data());
    }

    ~Buffer() {
        destroy();
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept
        : VAO(other.VAO), VBOs(std::move(other.VBOs)) {
        other.VAO = 0;
        other.VBOs.clear();
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            destroy();

            VAO = other.VAO;
            VBOs = std::move(other.VBOs);

            other.VAO = 0;
            other.VBOs.clear();
        }
        return *this;
    }

    void bind() {
        glBindVertexArray(VAO);
    }

    void unbind() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void bind_buffer(size_t idx) {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[idx]);
    }

    void upload(
        size_t idx,
        const std::vector<float>& data,
        GLint componentCount
    ) {
        bind_buffer(idx);
        glBufferData(
            GL_ARRAY_BUFFER,
            data.size() * sizeof(float),
            data.data(),
            GL_STATIC_DRAW
        );

        layout(idx, componentCount);
    }

    void upload(
        size_t idx,
        float *data,
        size_t size,
        GLint componentCount
    ) {
        bind_buffer(idx);
        glBufferData(
            GL_ARRAY_BUFFER,
            size * sizeof(float),
            data,
            GL_STATIC_DRAW
        );

        layout(idx, componentCount);
    }

    void layout(size_t idx, GLint componentCount) {
        glVertexAttribPointer(
            static_cast<GLuint>(idx),
            componentCount,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glEnableVertexAttribArray(static_cast<GLuint>(idx));
    }

    void destroy() {
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
            VAO = 0;
        }
        if (!VBOs.empty()) {
            glDeleteBuffers(static_cast<GLsizei>(VBOs.size()), VBOs.data());
            VBOs.clear();
        }
    }
};
