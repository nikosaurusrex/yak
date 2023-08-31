#include "framebuffer.h"

static void attach_color_texture(GLuint id, GLint type, s32 width, s32 height, s32 index) {
    GLint internal;
    if (type == GL_RGBA) {
        internal = GL_RGBA8;
    } else if (type == GL_RED_INTEGER) {
        internal = GL_R32I;
    } else {
        log_error("Specified invalid color attachment type for framebuffer");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, type, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
}

static void attach_depth_texture(GLuint id, GLint type, s32 width, s32 height) {
    glTexStorage2D(GL_TEXTURE_2D, 1, type, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, id, 0);
}

Framebuffer::Framebuffer(s32 width, s32 height, array<GLint> attachments)
    : width(width), height(height) {
    
    for (GLint attachment : attachments) {
        if (attachment == GL_DEPTH24_STENCIL8) {
            depth_attachment_type = attachment;
        } else {
            color_attachments_types.push_back(attachment);
        }
    }

    reload();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &id);
    glDeleteTextures(color_attachments.size(), color_attachments.data());
    glDeleteTextures(1, &depth_attachment);
}

void Framebuffer::reload() {
    if (id) {
        glDeleteFramebuffers(1, &id);
        glDeleteTextures(color_attachments.size(), color_attachments.data());
        glDeleteTextures(1, &depth_attachment);

        color_attachments.clear();
        depth_attachment = 0;
    }

    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    if (!color_attachments_types.empty()) {
        color_attachments.resize(color_attachments_types.size());
        glGenTextures(color_attachments.size(), color_attachments.data());

        for (s32 i = 0; i < color_attachments.size(); ++i) {
            glBindTexture(GL_TEXTURE_2D, color_attachments[i]);
            attach_color_texture(color_attachments[i], color_attachments_types[i], width, height, i);
        }
    }

    if (depth_attachment_type) {
        glGenTextures(1, &depth_attachment);
        glBindTexture(GL_TEXTURE_2D, depth_attachment);

        attach_depth_texture(depth_attachment, depth_attachment_type, width, height);
    }

    if (color_attachments.size() > 1) {
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(color_attachments.size(), buffers);
    } else if (color_attachments.empty()) {
        glDrawBuffer(GL_NONE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glViewport(0, 0, width, height);
}

void Framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(s32 width, s32 height) {
    this->width = width;
    this->height = height;

    reload();
}

GLuint Framebuffer::get(u32 index) {
    return color_attachments[index];
}

void Framebuffer::clear(u32 index, s32 value) {
    // glClearTexImage(color_attachments[index], 0, color_attachments_types[index], GL_INT, &value);
}

