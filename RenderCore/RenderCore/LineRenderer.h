#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct LineVertex
{
    glm::vec3 position;
    glm::vec4 color;
    float thickness;
};

class LineRenderer
{
public:
    LineRenderer();
    ~LineRenderer();

    void Init();
    void Render();

    // Dynamic lines
    void AddLine(
        glm::vec3 pos,
        glm::vec3 pos1,
        glm::vec4 color,
        float thickness);

    void AddLineStrip(
        const std::vector<glm::vec3>& vertices,
        glm::vec4 color,
        float thickness);

    // Static lines
    void AddStaticLine(
        glm::vec3 pos,
        glm::vec3 pos1,
        glm::vec4 color,
        float thickness);

    void AddStaticLineStrip(
        const std::vector<glm::vec3>& vertices,
        glm::vec4 color,
        float thickness);

    // Clear
    void ClearDynamicLines();
    void ClearStaticLines();

    // Debug/statistics
    size_t GetDynamicLineCount() const
    {
        return m_lines.size() / 2;
    }

    size_t GetStaticLineCount() const
    {
        return m_staticLines.size() / 2;
    }

    bool HasStaticLinesDirty() const
    {
        return m_staticLinesDirty;
    }

private:

    // ============================================================
    // DYNAMIC LINES
    // ============================================================

    GLuint m_dynamicVAO = 0;
    GLuint m_dynamicVBO = 0;

    std::vector<LineVertex> m_lines;


    // ============================================================
    // STATIC LINES
    // ============================================================

    GLuint m_staticVAO = 0;
    GLuint m_staticVBO = 0;

    std::vector<LineVertex> m_staticLines;

    bool m_staticLinesDirty = false;
    size_t m_staticLinesCount = 0;


    // ============================================================
    // HELPERS
    // ============================================================

    void UploadStaticLines();

    void SetupVertexAttributes(
        GLuint vao,
        GLuint vbo);
};