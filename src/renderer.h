#ifndef _RENDERER_H

namespace entity_type
{
    enum Enum
    {
        Topic,
        Arrow
    };
};

struct shader_program
{
    int program_id;
    int vertex_shader_id;
    int fragment_shader_id;
};

struct entity
{
    entity_type::Enum   type;
    GLuint              vao_id;
    int                 vertex_count;   // vertext buffer
    int                 index_count;    // index buffer
    vec3                position;
    unsigned int        flags;
};

namespace graphics
{
    int create_vao();
    void destroy_vao(int vao);
    void draw_rect(vec2 pos, vec2 size);
}

#ifdef _RENDERER_IMPLEMENTATION_H
namespace graphics
{
    void draw_rect(vec2 pos, vec2 size)
    {
        glBegin(GL_QUADS);
            float x = pos.x/g_state.window_width - 0.5f;
            float y = pos.y/g_state.window_height - 0.5f;
            float w = size.w/g_state.window_width;
            float h = size.h/g_state.window_height;
            printf("%f %f %f %f %d %d \n", x, y, w, h, g_state.window_width, g_state.window_height);
            glVertex2f(x, y);
            glVertex2f(x+w, y);
            glVertex2f(x+w, y+h);
            glVertex2f(x, y+h);
        glEnd();
    }
}
#endif //_RENDERER_IMPLEMENTATION_H

#endif //_RENDERER_H
