#ifndef RENDER_H__ /*/////////////////////////////////////////////////////////*/
#define RENDER_H__

#define DECLARE_RENDER_TYPE(name) struct name##__Type; typedef struct name##__Type* name;
#define DEFINE_RENDER_TYPE(name) struct name##__Type

DECLARE_RENDER_TYPE(VertexBuffer);
DECLARE_RENDER_TYPE(RenderTarget);
DECLARE_RENDER_TYPE(Shader);
DECLARE_RENDER_TYPE(Texture);

NK_ENUM(DrawMode, nkS32)
{
    DrawMode_Points,
    DrawMode_LineStrip,
    DrawMode_LineLoop,
    DrawMode_Lines,
    DrawMode_TriangleStrip,
    DrawMode_TriangleFan,
    DrawMode_Triangles
};

NK_ENUM(AttribType, nkS32)
{
    AttribType_SignedByte,
    AttribType_UnsignedByte,
    AttribType_SignedInt,
    AttribType_UnsignedInt,
    AttribType_Float
};

NK_ENUM(BufferType, nkS32)
{
    BufferType_Static,
    BufferType_Dynamic,
    BufferType_Stream
};

NK_ENUM(SamplerFilter, nkS32)
{
    SamplerFilter_Nearest,
    SamplerFilter_Linear
};

NK_ENUM(SamplerWrap, nkS32)
{
    SamplerWrap_Repeat,
    SamplerWrap_Clamp
};

static void init_renderer(void);
static void quit_renderer(void);

static void clear_screen_v(nkVec4 color);
static void clear_screen_f(nkF32 r, nkF32 g, nkF32 b, nkF32 a);

static void vertex_buffer_create        (VertexBuffer* vbuf);
static void vertex_buffer_destroy       (VertexBuffer* vbuf);
static void vertex_buffer_set_stride    (VertexBuffer* vbuf, nkU64 byte_stride);
static void vertex_buffer_enable_attrib (VertexBuffer* vbuf, nkU32 index, AttribType type, nkU32 comps, nkU64 byte_offset);
static void vertex_buffer_disable_attrib(VertexBuffer* vbuf, nkU32 index);
static void vertex_buffer_update        (VertexBuffer* vbuf, void* data, nkU64 bytes, BufferType type);
static void vertex_buffer_draw          (VertexBuffer* vbuf, DrawMode draw_mode, nkU64 vert_count);

static void render_target_create (RenderTarget* target, nkS32 w, nkS32 h, SamplerFilter filter, SamplerWrap wrap);
static void render_target_destroy(RenderTarget* target);
static void render_target_resize (RenderTarget* target, nkS32 w, nkS32 h);

static void shader_create   (Shader* shader, void* data, nkU64 bytes);
static void shader_destroy  (Shader* shader);
static void shader_bind     (Shader* shader);
static void shader_set_bool (Shader* shader, const nkChar* name, nkBool val);
static void shader_set_int  (Shader* shader, const nkChar* name, nkS32  val);
static void shader_set_float(Shader* shader, const nkChar* name, nkF32  val);
static void shader_set_vec2 (Shader* shader, const nkChar* name, nkVec2 val);
static void shader_set_vec3 (Shader* shader, const nkChar* name, nkVec3 val);
static void shader_set_vec4 (Shader* shader, const nkChar* name, nkVec4 val);
static void shader_set_mat2 (Shader* shader, const nkChar* name, nkMat2 val);
static void shader_set_mat3 (Shader* shader, const nkChar* name, nkMat3 val);
static void shader_set_mat4 (Shader* shader, const nkChar* name, nkMat4 val);

static void   texture_create    (Texture* texture, nkS32 w, nkS32 h, nkS32 bpp, void* data, SamplerFilter filter, SamplerWrap wrap);
static void   texture_destroy   (Texture* texture);
static nkVec2 texture_get_size  (Texture* texture);
static nkF32  texture_get_width (Texture* texture);
static nkF32  texture_get_height(Texture* texture);

#endif /* RENDER_H__ /////////////////////////////////////////////////////////*/
