#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <string>
#include <glm/glm.hpp>

class Shader : protected QOpenGLFunctions_3_3_Core
{
public:
    Shader();
    ~Shader();

    bool compile(const std::string& vertexSource,
                 const std::string& geometrySource,
                 const std::string& fragmentSource);

    void bind() const;
    void release() const;

    void setUniform(const std::string& name, const glm::mat4& mat) const;
    void setUniform(const std::string& name, const glm::vec2& vec) const;
    void setUniform(const std::string& name, float value) const;

private:
    GLuint m_program;
    GLuint m_vertexShader;
    GLuint m_geometryShader;
    GLuint m_fragmentShader;

    GLuint compileShader(GLenum type, const std::string& source);
    bool linkProgram();
    GLint getUniformLocation(const std::string& name) const;
};