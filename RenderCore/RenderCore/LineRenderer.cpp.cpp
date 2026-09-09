#include "LineRenderer.h"
#include "Shader.h"
#include <GL/glew.h>

LineRenderer::LineRenderer()
    : m_dynamicVAO(0), m_dynamicVBO(0), m_staticVAO(0), m_staticVBO(0)
{
    m_lines.reserve(50000);
    m_staticLines.reserve(100000);
}

LineRenderer::~LineRenderer()
{
    if (m_dynamicVBO) glDeleteBuffers(1, &m_dynamicVBO);
    if (m_dynamicVAO) glDeleteVertexArrays(1, &m_dynamicVAO);
    if (m_staticVBO) glDeleteBuffers(1, &m_staticVBO);
    if (m_staticVAO) glDeleteVertexArrays(1, &m_staticVAO);
}

void LineRenderer::SetupVertexAttributes(GLuint vao, GLuint vbo)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)offsetof(LineVertex, thickness));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LineRenderer::Init()
{
    // Dynamic
    glGenVertexArrays(1, &m_dynamicVAO);
    glGenBuffers(1, &m_dynamicVBO);
    glBindVertexArray(m_dynamicVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_dynamicVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    SetupVertexAttributes(m_dynamicVAO, m_dynamicVBO);

    // Static
    glGenVertexArrays(1, &m_staticVAO);
    glGenBuffers(1, &m_staticVBO);
    glBindVertexArray(m_staticVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_staticVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    SetupVertexAttributes(m_staticVAO, m_staticVBO);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LineRenderer::AddLine(glm::vec3 pos, glm::vec3 pos1, glm::vec4 color, float thickness)
{
    m_lines.push_back({pos, color, thickness});
    m_lines.push_back({pos1, color, thickness});
}

void LineRenderer::AddLineStrip(const std::vector<glm::vec3>& vertices, glm::vec4 color, float thickness)
{
    if (vertices.size() < 2) return;
    for (size_t i = 0; i + 1 < vertices.size(); ++i)
        AddLine(vertices[i], vertices[i+1], color, thickness);
}

void LineRenderer::AddStaticLine(glm::vec3 pos, glm::vec3 pos1, glm::vec4 color, float thickness)
{
    m_staticLines.push_back({pos, color, thickness});
    m_staticLines.push_back({pos1, color, thickness});
    m_staticLinesDirty = true;
}

void LineRenderer::AddStaticLineStrip(const std::vector<glm::vec3>& vertices, glm::vec4 color, float thickness)
{
    if (vertices.size() < 2) return;
    for (size_t i = 0; i + 1 < vertices.size(); ++i)
        AddStaticLine(vertices[i], vertices[i+1], color, thickness);
}

void LineRenderer::ClearDynamicLines()
{
    m_lines.clear();
}

void LineRenderer::ClearStaticLines()
{
    m_staticLines.clear();
    m_staticLinesDirty = true;
    m_staticLinesCount = 0;
}

void LineRenderer::UploadStaticLines()
{
    if (!m_staticLinesDirty) return;

    glBindBuffer(GL_ARRAY_BUFFER, m_staticVBO);
    if (m_staticLines.empty()) {
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
        m_staticLinesCount = 0;
    } else {
        glBufferData(GL_ARRAY_BUFFER, m_staticLines.size() * sizeof(LineVertex),
                     m_staticLines.data(), GL_STATIC_DRAW);
        m_staticLinesCount = m_staticLines.size();
    }
    m_staticLinesDirty = false;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void LineRenderer::Render(const glm::mat4& mvp, const glm::vec2& viewportSize, Shader* shader)
{
    UploadStaticLines();

    if (m_lines.empty() && m_staticLinesCount == 0)
        return;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->bind();
    shader->setUniform("u_mvp", mvp);
    shader->setUniform("u_viewportSize", viewportSize);

    // Dynamic
    if (!m_lines.empty()) {
        glBindVertexArray(m_dynamicVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_dynamicVBO);
        glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(LineVertex),
                     m_lines.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, (GLsizei)m_lines.size());
    }

    // Static
    if (m_staticLinesCount > 0) {
        glBindVertexArray(m_staticVAO);
        glDrawArrays(GL_LINES, 0, (GLsizei)m_staticLinesCount);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    shader->release();

    ClearDynamicLines(); // dynamic lines are per-frame only
}