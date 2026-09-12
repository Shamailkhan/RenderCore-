#version 450 core

layout(lines) in;

layout(triangle_strip, max_vertices = 4) out;

in VS_OUT
{
    vec4 color;
    float thickness;
} gs_in[];

out GS_OUT
{
    vec4 color;
} gs_out;

uniform vec2 u_viewportSize;

void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;

    vec2 ndc0 = p0.xy / p0.w;
    vec2 ndc1 = p1.xy / p1.w;

    vec2 direction = ndc1 - ndc0;

    float len = length(direction);

    if (len < 0.000001)
        return;

    direction /= len;

    // Perpendicular direction
    vec2 normal = vec2(
        -direction.y,
         direction.x
    );

    // Average thickness of the two vertices
    float thickness =
        (gs_in[0].thickness +
         gs_in[1].thickness) * 0.5;

    // Convert pixels -> NDC
    vec2 pixelToNDC =
        vec2(
            2.0 / u_viewportSize.x,
            2.0 / u_viewportSize.y
        );

    // Half thickness
    vec2 offset =
        normal *
        thickness *
        0.5 *
        pixelToNDC;

    // --------------------------------------------------------
    // Endpoint 0
    // --------------------------------------------------------

    vec2 p0Plus = ndc0 + offset;
    vec2 p0Minus = ndc0 - offset;

    // --------------------------------------------------------
    // Endpoint 1
    // --------------------------------------------------------

    vec2 p1Plus = ndc1 + offset;
    vec2 p1Minus = ndc1 - offset;


    // --------------------------------------------------------
    // Emit quad
    // --------------------------------------------------------

    gs_out.color = gs_in[0].color;
    gl_Position = vec4(
        p0Plus * p0.w,
        p0.z,
        p0.w
    );
    EmitVertex();


    gs_out.color = gs_in[0].color;
    gl_Position = vec4(
        p0Minus * p0.w,
        p0.z,
        p0.w
    );
    EmitVertex();


    gs_out.color = gs_in[1].color;
    gl_Position = vec4(
        p1Plus * p1.w,
        p1.z,
        p1.w
    );
    EmitVertex();


    gs_out.color = gs_in[1].color;
    gl_Position = vec4(
        p1Minus * p1.w,
        p1.z,
        p1.w
    );
    EmitVertex();


    EndPrimitive();
}