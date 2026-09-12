#include "ShaderManager.h"

#include <glm/gtc/type_ptr.hpp>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <vector>


// ============================================================
// DESTRUCTOR
// ============================================================

ShaderManager::~ShaderManager()
{
    if (m_program != 0)
        glDeleteProgram(m_program);
}


// ============================================================
// READ RESOURCE TEXT
// ============================================================

QString ShaderManager::ReadResourceText(const QString& resourcePath)
{
    QFile file(resourcePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "ShaderManager: failed to open" << resourcePath;
        return QString();
    }

    QTextStream stream(&file);
    return stream.readAll();
}


// ============================================================
// COMPILE SHADER
// ============================================================

GLuint ShaderManager::CompileShader(
    GLenum stage,
    const QString& source,
    const char* debugName)
{
    const QByteArray sourceBytes = source.toUtf8();
    const char* sourcePtr = sourceBytes.constData();

    const GLuint shader = glCreateShader(stage);
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> log(static_cast<size_t>(logLength) + 1, '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());

        qWarning() << "ShaderManager: failed to compile" << debugName
                   << "-" << log.data();

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}


// ============================================================
// LOAD LINE SHADER
// ============================================================

bool ShaderManager::LoadLineShader()
{
    const QString vertSrc = ReadResourceText(":/shaders/LineRenderer.vert");
    const QString geomSrc = ReadResourceText(":/shaders/LineRenderer.geom");
    const QString fragSrc = ReadResourceText(":/shaders/LineRenderer.frag");

    if (vertSrc.isEmpty() || geomSrc.isEmpty() || fragSrc.isEmpty())
        return false;

    const GLuint vert = CompileShader(GL_VERTEX_SHADER,   vertSrc, "LineRenderer.vert");
    const GLuint geom = CompileShader(GL_GEOMETRY_SHADER, geomSrc, "LineRenderer.geom");
    const GLuint frag = CompileShader(GL_FRAGMENT_SHADER, fragSrc, "LineRenderer.frag");

    if (vert == 0 || geom == 0 || frag == 0)
    {
        if (vert != 0) glDeleteShader(vert);
        if (geom != 0) glDeleteShader(geom);
        if (frag != 0) glDeleteShader(frag);
        return false;
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vert);
    glAttachShader(m_program, geom);
    glAttachShader(m_program, frag);
    glLinkProgram(m_program);

    GLint linked = GL_FALSE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);

    // Shaders are no longer needed once linked into the program.
    glDeleteShader(vert);
    glDeleteShader(geom);
    glDeleteShader(frag);

    if (linked == GL_FALSE)
    {
        GLint logLength = 0;
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> log(static_cast<size_t>(logLength) + 1, '\0');
        glGetProgramInfoLog(m_program, logLength, nullptr, log.data());

        qWarning() << "ShaderManager: failed to link program -" << log.data();

        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }

    return true;
}


// ============================================================
// BIND / UNBIND
// ============================================================

void ShaderManager::Bind()
{
    glUseProgram(m_program);
}

void ShaderManager::Unbind()
{
    glUseProgram(0);
}


// ============================================================
// UNIFORMS
// ============================================================

void ShaderManager::SetUniform(const char* name, const glm::mat4& value)
{
    const GLint location = glGetUniformLocation(m_program, name);
    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderManager::SetUniform(const char* name, const glm::vec2& value)
{
    const GLint location = glGetUniformLocation(m_program, name);
    if (location != -1)
        glUniform2fv(location, 1, glm::value_ptr(value));
}
