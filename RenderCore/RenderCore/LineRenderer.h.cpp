#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>   // or QOpenGLFunctions

struct LineVertex
{
    glm::vec3 position;
    glm::vec4 color;
    float thickness;
};

class Shader;

class LineRenderer
{
public:
    LineRenderer();
    ~LineRenderer();

    void Init();
    void Render(const glm::mat4& mvp, const glm::vec2& viewportSize, Shader* shader);

    // Dynamic lines (cleared every frame)
    void AddLine(glm::vec3 pos, glm::vec3 pos1, glm::vec4 color, float thickness);
    void AddLineStrip(const std::vector<glm::vec3>& vertices, glm::vec4 color, float thickness);
    void ClearDynamicLines();

    // Static lines (persistent)
    void AddStaticLine(glm::vec3 pos, glm::vec3 pos1, glm::vec4 color, float thickness);
    void AddStaticLineStrip(const std::vector<glm::vec3>& vertices, glm::vec4 color, float thickness);
    void ClearStaticLines();

    size_t GetDynamicLineCount() const { return m_lines.size() / 2; }
    size_t GetStaticLineCount() const { return m_staticLines.size() / 2; }
    bool HasStaticLinesDirty() const { return m_staticLinesDirty; }

private:
    void UploadStaticLines();
    void SetupVertexAttributes(GLuint vao, GLuint vbo);

    GLuint m_dynamicVAO, m_dynamicVBO;
    std::vector<LineVertex> m_lines;

    GLuint m_staticVAO, m_staticVBO;
    std::vector<LineVertex> m_staticLines;
    bool m_staticLinesDirty = false;
    size_t m_staticLinesCount = 0;
};