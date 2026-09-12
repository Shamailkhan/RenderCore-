#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <QString>

// ============================================================
// SHADER MANAGER
// ============================================================
//
// Minimal shader wrapper matching the `extern class ShaderManager*
// g_shaderManager;` declaration used by LineRenderer.cpp. Loads the
// vertex/geometry/fragment shaders (embedded as Qt resources, see
// shaders/shaders.qrc) and exposes Bind()/SetUniform* helpers.
//
class ShaderManager
{
public:
    ShaderManager() = default;
    ~ShaderManager();

    // Loads and links the line-drawing shader program from
    // :/shaders/LineRenderer.{vert,geom,frag}. Returns false and
    // logs to stderr (via qWarning) on failure.
    bool LoadLineShader();

    void Bind();
    void Unbind();

    void SetUniform(const char* name, const glm::mat4& value);
    void SetUniform(const char* name, const glm::vec2& value);

    GLuint GetProgram() const { return m_program; }

private:
    static QString ReadResourceText(const QString& resourcePath);

    GLuint CompileShader(GLenum stage, const QString& source, const char* debugName);

    GLuint m_program = 0;
};
