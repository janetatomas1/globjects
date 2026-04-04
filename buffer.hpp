#pragma once

#include <GL/glew.h>
#include <vector>

struct Buffer {
    GLuint VAO;
    std::vector<GLuint> VBOs;

    Buffer(size_t numAttributes) {
        glGenVertexArrays(1, &VAO);

        VBOs.resize(numAttributes);
        glGenBuffers(static_cast<GLsizei>(numAttributes), VBOs.data());
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

    void upload(size_t idx,
                const std::vector<float>& data,
                GLint componentCount) {
        bind_buffer(idx);
        glBufferData(GL_ARRAY_BUFFER,
                     data.size() * sizeof(float),
                     data.data(),
                     GL_STATIC_DRAW);

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
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(static_cast<GLsizei>(VBOs.size()), VBOs.data());
    }
};
