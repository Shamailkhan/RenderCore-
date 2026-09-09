#include "Shader.h"
#include <QDebug>

Shader::Shader() : m_program(0), m_vertexShader(0), m_geometryShader(0), m_fragmentShader(0)
{
    initializeOpenGLFunctions();
}

Shader::~Shader()
{
    if (m_program) glDeleteProgram(m_program);
    if (m_vertexShader) glDeleteShader(m_vertexShader);
    if (m_geometryShader) glDeleteShader(m_geometryShader);
    if (m_fragmentShader) glDeleteShader(m_fragmentShader);
}

GLuint Shader::compileShader(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        qDebug() << "Shader compilation error:" << infoLog;
        return 0;
    }
    return shader;
}

bool Shader::compile(const std::string& vertexSource,
                     const std::string& geometrySource,
                     const std::string& fragmentSource)
{
    m_vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (!m_vertexShader) return false;

    m_geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySource);
    if (!m_geometryShader) return false;

    m_fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!m_fragmentShader) return false;

    m_program = glCreateProgram();
    glAttachShader(m_program, m_vertexShader);
    glAttachShader(m_program, m_geometryShader);
    glAttachShader(m_program, m_fragmentShader);
    return linkProgram();
}

bool Shader::linkProgram()
{
    glLinkProgram(m_program);
    GLint success;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
        qDebug() << "Shader linking error:" << infoLog;
        return false;
    }
    return true;
}

void Shader::bind() const
{
    glUseProgram(m_program);
}

void Shader::release() const
{
    glUseProgram(0);
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_program, name.c_str());
}

void Shader::setUniform(const std::string& name, const glm::mat4& mat) const
{
    GLint loc = getUniformLocation(name);
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(const std::string& name, const glm::vec2& vec) const
{
    GLint loc = getUniformLocation(name);
    if (loc != -1)
        glUniform2fv(loc, 1, glm::value_ptr(vec));
}

void Shader::setUniform(const std::string& name, float value) const
{
    GLint loc = getUniformLocation(name);
    if (loc != -1)
        glUniform1f(loc, value);
}