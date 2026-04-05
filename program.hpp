
#pragma once

#include <GL/glew.h>
#include <GL/glew.h>
#include <string>
#include <stdexcept>

struct Shader {
    GLuint id = 0;

    Shader(const std::string& source, GLenum type) {
        id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        GLint success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(id, 512, nullptr, log);
            throw std::runtime_error(std::string("Shader compile error: ") + log);
        }
    }

    ~Shader() {
        if (id) glDeleteShader(id);
    }
};

struct Program {
    GLuint id = 0;

    Program() = default;

    Program(const std::string& vs_source, const std::string& fs_source) {
        Shader vs(vs_source, GL_VERTEX_SHADER);
        Shader fs(fs_source, GL_FRAGMENT_SHADER);

        id = glCreateProgram();
        glAttachShader(id, vs.id);
        glAttachShader(id, fs.id);
        glLinkProgram(id);

        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            char log[512];
            glGetProgramInfoLog(id, 512, nullptr, log);
            throw std::runtime_error(std::string("Program link error: ") + log);
        }
    }

    void use() const {
        glUseProgram(id);
    }

    GLint get_uniform(const char* name) const {
        return glGetUniformLocation(id, name);
    }

    void destroy() {
        if (id) glDeleteProgram(id);
    }
};
