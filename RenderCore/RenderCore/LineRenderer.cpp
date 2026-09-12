#include "LineRenderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>

#include "ShaderManager.h"
#include "Camera.h"

// External references
extern ShaderManager* g_shaderManager;
extern Camera* g_camera;


// ============================================================
// CONSTRUCTOR
// ============================================================

LineRenderer::LineRenderer()
    : m_dynamicVAO(0)
    , m_dynamicVBO(0)
    , m_staticVAO(0)
    , m_staticVBO(0)
    , m_staticLinesDirty(false)
    , m_staticLinesCount(0)
{
    // Reserve CPU-side memory.
    //
    // This is NOT a ring buffer.
    // It is simply vector capacity to reduce reallocations.

    m_lines.reserve(50000);
    m_staticLines.reserve(100000);
}


// ============================================================
// DESTRUCTOR
// ============================================================

LineRenderer::~LineRenderer()
{
    if (m_dynamicVBO != 0)
        glDeleteBuffers(1, &m_dynamicVBO);

    if (m_dynamicVAO != 0)
        glDeleteVertexArrays(1, &m_dynamicVAO);


    if (m_staticVBO != 0)
        glDeleteBuffers(1, &m_staticVBO);

    if (m_staticVAO != 0)
        glDeleteVertexArrays(1, &m_staticVAO);
}


// ============================================================
// SETUP VERTEX ATTRIBUTES
// ============================================================

void LineRenderer::SetupVertexAttributes(
    GLuint vao,
    GLuint vbo)
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    // --------------------------------------------------------
    // Position
    // --------------------------------------------------------

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LineVertex),
        (void*)offsetof(LineVertex, position)
    );


    // --------------------------------------------------------
    // Color
    // --------------------------------------------------------

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LineVertex),
        (void*)offsetof(LineVertex, color)
    );


    // --------------------------------------------------------
    // Thickness
    // --------------------------------------------------------

    glEnableVertexAttribArray(2);

    glVertexAttribPointer(
        2,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(LineVertex),
        (void*)offsetof(LineVertex, thickness)
    );


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


// ============================================================
// INITIALIZATION
// ============================================================

void LineRenderer::Init()
{
    // ========================================================
    // DYNAMIC BUFFER
    // ========================================================

    glGenVertexArrays(1, &m_dynamicVAO);

    glGenBuffers(1, &m_dynamicVBO);


    glBindVertexArray(m_dynamicVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_dynamicVBO);

    // Start with no data.
    //
    // The buffer will be resized automatically when dynamic
    // lines are uploaded.

    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_DYNAMIC_DRAW
    );

    SetupVertexAttributes(
        m_dynamicVAO,
        m_dynamicVBO
    );


    // ========================================================
    // STATIC BUFFER
    // ========================================================

    glGenVertexArrays(1, &m_staticVAO);

    glGenBuffers(1, &m_staticVBO);


    glBindVertexArray(m_staticVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_staticVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        0,
        nullptr,
        GL_STATIC_DRAW
    );

    SetupVertexAttributes(
        m_staticVAO,
        m_staticVBO
    );


    // ========================================================
    // CLEANUP
    // ========================================================

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


// ============================================================
// ADD DYNAMIC LINE
// ============================================================

void LineRenderer::AddLine(
    glm::vec3 pos,
    glm::vec3 pos1,
    glm::vec4 color,
    float thickness)
{
    m_lines.push_back({
        pos,
        color,
        thickness
    });

    m_lines.push_back({
        pos1,
        color,
        thickness
    });
}


// ============================================================
// ADD DYNAMIC LINE STRIP
// ============================================================

void LineRenderer::AddLineStrip(
    const std::vector<glm::vec3>& vertices,
    glm::vec4 color,
    float thickness)
{
    if (vertices.size() < 2)
        return;

    for (size_t i = 0; i + 1 < vertices.size(); ++i)
    {
        AddLine(
            vertices[i],
            vertices[i + 1],
            color,
            thickness
        );
    }
}


// ============================================================
// ADD STATIC LINE
// ============================================================

void LineRenderer::AddStaticLine(
    glm::vec3 pos,
    glm::vec3 pos1,
    glm::vec4 color,
    float thickness)
{
    m_staticLines.push_back({
        pos,
        color,
        thickness
    });

    m_staticLines.push_back({
        pos1,
        color,
        thickness
    });

    m_staticLinesDirty = true;
}


// ============================================================
// ADD STATIC LINE STRIP
// ============================================================

void LineRenderer::AddStaticLineStrip(
    const std::vector<glm::vec3>& vertices,
    glm::vec4 color,
    float thickness)
{
    if (vertices.size() < 2)
        return;

    for (size_t i = 0; i + 1 < vertices.size(); ++i)
    {
        AddStaticLine(
            vertices[i],
            vertices[i + 1],
            color,
            thickness
        );
    }
}


// ============================================================
// UPLOAD STATIC LINES
// ============================================================

void LineRenderer::UploadStaticLines()
{
    if (!m_staticLinesDirty)
        return;


    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_staticVBO
    );


    // ========================================================
    // Empty static data
    // ========================================================

    if (m_staticLines.empty())
    {
        glBufferData(
            GL_ARRAY_BUFFER,
            0,
            nullptr,
            GL_STATIC_DRAW
        );

        m_staticLinesCount = 0;
        m_staticLinesDirty = false;

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return;
    }


    // ========================================================
    // Upload complete static buffer
    // ========================================================

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            m_staticLines.size() * sizeof(LineVertex)
        ),
        m_staticLines.data(),
        GL_STATIC_DRAW
    );


    m_staticLinesCount = m_staticLines.size();

    m_staticLinesDirty = false;


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


// ============================================================
// CLEAR DYNAMIC LINES
// ============================================================

void LineRenderer::ClearDynamicLines()
{
    m_lines.clear();
}


// ============================================================
// CLEAR STATIC LINES
// ============================================================

void LineRenderer::ClearStaticLines()
{
    m_staticLines.clear();

    m_staticLinesDirty = true;

    m_staticLinesCount = 0;
}


// ============================================================
// RENDER
// ============================================================

void LineRenderer::Render()
{
    // ========================================================
    // Upload static data only when necessary
    // ========================================================

    UploadStaticLines();


    if (m_lines.empty() && m_staticLinesCount == 0)
        return;


    // ========================================================
    // OPENGL STATE
    // ========================================================

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );


    // ========================================================
    // SHADER
    // ========================================================

    g_shaderManager->Bind();

    const glm::mat4 mvp =
        g_camera->GetProjectionMatrix() *
        g_camera->GetViewMatrix();

    g_shaderManager->SetUniform("u_mvp", mvp);
    g_shaderManager->SetUniform("u_viewportSize", g_camera->GetViewportSize());


    // ========================================================
    // DYNAMIC LINES
    // ========================================================

    if (!m_lines.empty())
    {
        glBindVertexArray(m_dynamicVAO);

        glBindBuffer(
            GL_ARRAY_BUFFER,
            m_dynamicVBO
        );


        // ----------------------------------------------------
        // Upload current frame's dynamic data
        // ----------------------------------------------------

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                m_lines.size() * sizeof(LineVertex)
            ),
            m_lines.data(),
            GL_DYNAMIC_DRAW
        );


        // ----------------------------------------------------
        // Geometry shader receives GL_LINES
        // ----------------------------------------------------

        glDrawArrays(
            GL_LINES,
            0,
            static_cast<GLsizei>(
                m_lines.size()
            )
        );
    }


    // ========================================================
    // STATIC LINES
    // ========================================================

    if (m_staticLinesCount > 0)
    {
        glBindVertexArray(m_staticVAO);

        glDrawArrays(
            GL_LINES,
            0,
            static_cast<GLsizei>(
                m_staticLinesCount
            )
        );
    }


    // ========================================================
    // CLEANUP
    // ========================================================

    glBindVertexArray(0);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    g_shaderManager->Unbind();


    // ========================================================
    // Dynamic data only exists for this frame
    // ========================================================

    ClearDynamicLines();
}