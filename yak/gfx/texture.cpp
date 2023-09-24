#include "texture.h"

#include "stb_image.h"

Texture::Texture(string file_path, GLint format) {
    path = file_path;
    load(file_path, format);
}

void Texture::load(string file_path, GLint format) {
    s32 w, h;

    stbi_set_flip_vertically_on_load(true);
    s32 read_mode = format == GL_RGBA ? STBI_rgb_alpha : STBI_rgb;
    u8 *image = stbi_load(file_path.data(), &w, &h, 0, read_mode);

    if (!image) {
        log_fatal("Failed to load texture '%s'", file_path.c_str());
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind(s32 slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}
