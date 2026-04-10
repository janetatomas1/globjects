
#pragma once

#include <vector>

#include "buffer.hpp"

struct ElementBuffer {
    Buffer buffer;
    GLuint EBO{};
    GLsizei indexCount = 0;

    ElementBuffer(size_t numAttributes)
        : buffer(numAttributes) {
        glGenBuffers(1, &EBO);
    }

    void bind() {
        buffer.bind();
    }

    void unbind() {
        buffer.unbind();
    }

    void upload(
        size_t idx,
        const std::vector<float>& data,
        GLint componentCount
    ) {
        buffer.upload(idx, data, componentCount);
    }

    void upload(
        size_t idx,
        float* data,
        size_t size,
        GLint componentCount
    ) {
        buffer.upload(idx, data, size, componentCount);
    }

    void upload_indices(const std::vector<unsigned int>& indices) {
        buffer.bind();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            indices.data(),
            GL_STATIC_DRAW
        );

        indexCount = static_cast<GLsizei>(indices.size());
    }

    void draw(GLenum mode = GL_TRIANGLES) {
        buffer.bind();
        glDrawElements(mode, indexCount, GL_UNSIGNED_INT, 0);
    }

    void destroy() {
        buffer.destroy();
        glDeleteBuffers(1, &EBO);
    }
};
