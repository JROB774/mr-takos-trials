/*////////////////////////////////////////////////////////////////////////////*/

#define MAX_POSTPROCESS_EFFECTS 32

typedef struct PostProcess
{
    Shader       effects[MAX_POSTPROCESS_EFFECTS];
    nkS32        effect_count;
    RenderTarget target;
    VertexBuffer buffer;
}
PostProcess;

static PostProcess g_postprocess;

static void postprocess_init(void)
{
    g_postprocess.target = render_target_create(SCREEN_WIDTH,SCREEN_HEIGHT, SamplerFilter_Nearest, SamplerWrap_Clamp);
    g_postprocess.buffer = vertex_buffer_create();

    nkVec4 vertices[4];

    vertices[0] = (nkVec4){ -1.0f, 1.0f, 0.0f,1.0f };
    vertices[1] = (nkVec4){ -1.0f,-1.0f, 0.0f,0.0f };
    vertices[2] = (nkVec4){  1.0f, 1.0f, 1.0f,1.0f };
    vertices[3] = (nkVec4){  1.0f,-1.0f, 1.0f,0.0f };

    vertex_buffer_set_stride   (g_postprocess.buffer, sizeof(nkF32)*4);
    vertex_buffer_enable_attrib(g_postprocess.buffer, 0, AttribType_Float, 4, 0);
    vertex_buffer_update       (g_postprocess.buffer, vertices, sizeof(vertices), BufferType_Static);
}

static void postprocess_quit(void)
{
    vertex_buffer_destroy(g_postprocess.buffer);
    render_target_destroy(g_postprocess.target);
}

static void postprocess_push_effect(Shader effect)
{
    NK_ASSERT(g_postprocess.effect_count < MAX_POSTPROCESS_EFFECTS);
    g_postprocess.effects[g_postprocess.effect_count++] = effect;
}

static void postprocess_pop_effect(void)
{
    if(g_postprocess.effect_count == 0) return;
    g_postprocess.effect_count--;
}

static void postprocess_clear_effects(void)
{
    g_postprocess.effect_count = 0;
}

static RenderTarget postprocess_get_result(void)
{
    if(g_postprocess.effect_count == 0)
    {
        return get_screen();
    }

    // Ping-pong back-and-forth between buffers to render effects.
    nkBool result_in_src = NK_FALSE;

    RenderTarget src = get_screen();
    RenderTarget dst = g_postprocess.target;

    for(nkS32 i=0; i<g_postprocess.effect_count; ++i)
    {
        set_viewport(0.0f,0.0f,SCREEN_WIDTH,SCREEN_HEIGHT);

        render_target_bind(dst);

        texture_bind(src->color_target, 0);
        shader_bind(g_postprocess.effects[i]);

        vertex_buffer_draw(g_postprocess.buffer, DrawMode_TriangleStrip, 4);

        result_in_src = !result_in_src;

        NK_SWAP(RenderTarget, src, dst);
    }

    return ((result_in_src) ? src : dst);
}

/*////////////////////////////////////////////////////////////////////////////*/
