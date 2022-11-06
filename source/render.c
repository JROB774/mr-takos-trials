/*////////////////////////////////////////////////////////////////////////////*/

static void init_renderer(void)
{
    // @Incomplete: ...
}

static void quit_renderer(void)
{
    // @Incomplete: ...
}

static void clear_screen_v(nkVec4 color)
{
    glClearColor(color.r,color.g,color.b,color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void clear_screen_f(nkF32 r, nkF32 g, nkF32 b, nkF32 a)
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void vertex_buffer_create(VertexBuffer* vbuf)
{
    // @Incomplete: ...
}

static void vertex_buffer_destroy(VertexBuffer* vbuf)
{
    // @Incomplete: ...
}
static void vertex_buffer_set_stride(VertexBuffer* vbuf, nkU64 byte_stride)
{
    // @Incomplete: ...
}

static void vertex_buffer_enable_attrib(VertexBuffer* vbuf, nkU32 index, AttribType type, nkU32 comps, nkU64 byte_offset)
{
    // @Incomplete: ...
}

static void vertex_buffer_disable_attrib(VertexBuffer* vbuf, nkU32 index)
{
    // @Incomplete: ...
}

static void vertex_buffer_update(VertexBuffer* vbuf, void* data, nkU64 bytes, BufferType type)
{
    // @Incomplete: ...
}

static void vertex_buffer_draw(VertexBuffer* vbuf, DrawMode draw_mode, nkU64 vert_count)
{
    // @Incomplete: ...
}

static void render_target_create(RenderTarget* target, nkS32 w, nkS32 h, SamplerFilter filter, SamplerWrap wrap)
{
    // @Incomplete: ...
}

static void render_target_destroy(RenderTarget* target)
{
    // @Incomplete: ...
}

static void render_target_resize(RenderTarget* target, nkS32 w, nkS32 h)
{
    // @Incomplete: ...
}

static void shader_create(Shader* shader, void* data, nkU64 bytes)
{
    // @Incomplete: ...
}

static void shader_destroy(Shader* shader)
{
    // @Incomplete: ...
}

static void shader_bind(Shader* shader)
{
    // @Incomplete: ...
}

static void shader_set_bool(Shader* shader, const nkChar* name, nkBool val)
{
    // @Incomplete: ...
}

static void shader_set_int(Shader* shader, const nkChar* name, nkS32 val)
{
    // @Incomplete: ...
}

static void shader_set_float(Shader* shader, const nkChar* name, nkF32 val)
{
    // @Incomplete: ...
}

static void shader_set_vec2(Shader* shader, const nkChar* name, nkVec2 val)
{
    // @Incomplete: ...
}

static void shader_set_vec3(Shader* shader, const nkChar* name, nkVec3 val)
{
    // @Incomplete: ...
}

static void shader_set_vec4(Shader* shader, const nkChar* name, nkVec4 val)
{
    // @Incomplete: ...
}

static void shader_set_mat2(Shader* shader, const nkChar* name, nkMat2 val)
{
    // @Incomplete: ...
}

static void shader_set_mat3(Shader* shader, const nkChar* name, nkMat3 val)
{
    // @Incomplete: ...
}

static void shader_set_mat4(Shader* shader, const nkChar* name, nkMat4 val)
{
    // @Incomplete: ...
}

static void texture_create(Texture* texture, nkS32 w, nkS32 h, nkS32 bpp, void* data, SamplerFilter filter, SamplerWrap wrap)
{
    // @Incomplete: ...
}

static void texture_destroy(Texture* texture)
{
    // @Incomplete: ...
}

static nkVec2 texture_get_size(Texture* texture)
{
    // @Incomplete: ...
    return (nkVec2){ 0.0f, 0.0f };
}

static nkF32 texture_get_width (Texture* texture)
{
    // @Incomplete: ...
    return 0.0f;
}

static nkF32 texture_get_height(Texture* texture)
{
    // @Incomplete: ...
    return 0.0f;
}

/*////////////////////////////////////////////////////////////////////////////*/
