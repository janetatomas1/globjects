
#include <GL/gl.h>
#include <vector>

struct Texture {
    GLuint id;
    int width = 0;
    int height = 0;

    Texture(int width = 2000, int height= 2000) {
        this->width = width;
        this->height = height;
    }

    void init() {
       	glGenTextures(1, &id);
    	glBindTexture(GL_TEXTURE_2D, id);
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void resize(int width, int height) {
        this->width = width;
        this->height = height;
       	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    }

    void export_data(std::vector<unsigned char> &pixels) {
        bind();
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

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
        glDeleteTextures(1, &id);
    }

    ~Texture() {
        if(id != 0) {
            glDeleteTextures(1, &id);
        }
    }
};
