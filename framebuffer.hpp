
struct Framebuffer {
    GLuint fbo;
    GLuint rbo;

    int width = 0;
    int height = 0;

    Framebuffer(int width = 2000, int height= 2000) {
        this->width = width;
        this->height = height;
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
        glDeleteRenderbuffers(1, &rbo);
    }

    ~Framebuffer() {
        if(rbo != 0 ) {
            glDeleteRenderbuffers(1, &rbo);
        }

        if(fbo != 0) {
            glDeleteFramebuffers(1, &fbo);
        }
    }
};
