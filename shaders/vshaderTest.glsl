// hello world vertex shader
#version 400 compatibility

in vec4 vertex_position;

void main()
{
    gl_Position = vec4(vertex_position.xy, 0.0, 1.0);
}
