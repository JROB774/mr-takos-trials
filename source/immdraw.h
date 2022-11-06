#ifndef IMMDRAW_H__ /*////////////////////////////////////////////////////////*/
#define IMMDRAW_H__

typedef struct ImmVertex
{
    nkVec2 pos;
    nkVec2 tex;
    nkVec4 col;
}
ImmVertex;

typedef struct ImmRect
{
    nkF32 x,y,w,h;
}
ImmRect;

static void imm_init(void);
static void imm_quit(void);

static Texture imm_load_texture_from_file(const nkChar* file_name, SamplerFilter filter, SamplerWrap wrap);
static Shader  imm_load_shader_from_file (const nkChar* file_name);

static nkMat4 imm_get_projection(void);
static nkMat4 imm_get_view      (void);
static nkMat4 imm_get_model     (void);

static void imm_set_projection(nkMat4 projection);
static void imm_set_view      (nkMat4 view);
static void imm_set_model     (nkMat4 model);

static nkVec4 imm_get_texture_color(void);

static void imm_set_texture_color(nkVec4 color);

static void imm_begin (DrawMode draw_mode, Texture tex);
static void imm_end   (void);
static void imm_vertex(ImmVertex v);

static void imm_point         (nkF32  x, nkF32  y,                     nkVec4 color);
static void imm_line          (nkF32 x1, nkF32 y1, nkF32 x2, nkF32 y2, nkVec4 color);
static void imm_rect_outline  (nkF32  x, nkF32  y, nkF32  w, nkF32  h, nkVec4 color);
static void imm_rect_filled   (nkF32  x, nkF32  y, nkF32  w, nkF32  h, nkVec4 color);
static void imm_circle_outline(nkF32  x, nkF32  y, nkF32  r, nkS32  n, nkVec4 color);
static void imm_circle_filled (nkF32  x, nkF32  y, nkF32  r, nkS32  n, nkVec4 color);

static void imm_texture   (Texture tex, nkF32 x, nkF32 y,                                                  ImmRect* clip);
static void imm_texture_ex(Texture tex, nkF32 x, nkF32 y, nkF32 sx, nkF32 sy, nkF32 angle, nkVec2* anchor, ImmRect* clip);

static void imm_begin_texture_batch(Texture tex);
static void imm_end_texture_batch  (void);
static void imm_texture_batched    (nkF32 x, nkF32 y,                                                  ImmRect* clip);
static void imm_texture_batched_ex (nkF32 x, nkF32 y, nkF32 sx, nkF32 sy, nkF32 angle, nkVec2* anchor, ImmRect* clip);

#endif /* IMMDRAW_H__ ////////////////////////////////////////////////////////*/
