#pragma once

#include <vector>

#include "buffer.hpp"

struct ElementBuffer {
    Buffer buffer;
    GLuint EBO = 0;
    GLsizei indexCount = 0;

    ElementBuffer(size_t numAttributes)
        : buffer(numAttributes) {
        glGenBuffers(1, &EBO);
    }

    ~ElementBuffer() {
        destroy();
    }

    ElementBuffer(const ElementBuffer&) = delete;
    ElementBuffer& operator=(const ElementBuffer&) = delete;

    ElementBuffer(ElementBuffer&& other) noexcept
        : buffer(std::move(other.buffer)),
          EBO(other.EBO),
          indexCount(other.indexCount) {
        other.EBO = 0;
        other.indexCount = 0;
    }

    ElementBuffer& operator=(ElementBuffer&& other) noexcept {
        if (this != &other) {
            destroy();

            buffer = std::move(other.buffer);
            EBO = other.EBO;
            indexCount = other.indexCount;

            other.EBO = 0;
            other.indexCount = 0;
        }
        return *this;
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

    void destroy() {
        buffer.destroy();
        if (EBO != 0) {
            glDeleteBuffers(1, &EBO);
            EBO = 0;
        }
    }
};
