/*////////////////////////////////////////////////////////////////////////////*/

#define ALLOCATE_RENDER_TYPE(name) malloc(sizeof(struct name##__Type))

typedef struct VertexAttrib
{
    nkBool     enabled;
    AttribType type;
    nkU32      components;
    nkU64      byte_offset;
}
VertexAttrib;

DEFINE_RENDER_TYPE(VertexBuffer)
{
    nkU64        byte_stride;
    GLuint       handle;
    VertexAttrib attribs[16];
};

DEFINE_RENDER_TYPE(RenderTarget)
{
    GLuint        handle;
    Texture       color_target;
    SamplerFilter filter;
    SamplerWrap   wrap;
};

DEFINE_RENDER_TYPE(Shader)
{
    GLuint program;
};

DEFINE_RENDER_TYPE(Texture)
{
    GLuint handle;
    nkVec2 size;
};

static GLuint g_vao;

static void renderer_init(void)
{
    // Load OpenGL functions in a native build.
    #if defined(BUILD_NATIVE)
    glewInit();
    #endif // BUILD_NATIVE

    printf("GPU Renderer   : %s\n", glGetString(GL_RENDERER));
    printf("GPU Vendor     : %s\n", glGetString(GL_VENDOR));
    printf("OpenGL Version : %s\n", glGetString(GL_VERSION));
    printf("GLSL Version   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // We need one Vertex Attribute Object in order to render with modern OpenGL.
    #if defined(BUILD_NATIVE)
    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);
    #endif // BUILD_NATIVE
}

static void renderer_quit(void)
{
    #if defined(BUILD_NATIVE)
    glDeleteVertexArrays(1, &g_vao);
    #endif // BUILD_NATIVE
}

static void set_viewport(nkF32 x, nkF32 y, nkF32 w, nkF32 h)
{
    GLint   vx = NK_CAST(GLint,   x);
    GLint   vy = NK_CAST(GLint,   y);
    GLsizei vw = NK_CAST(GLsizei, w);
    GLsizei vh = NK_CAST(GLsizei, h);

    glViewport(vx,vy,vw,vh);
}

static void enable_alpha_blend(void)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

static void disable_alpha_blend(void)
{
    glDisable(GL_BLEND);
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

static VertexBuffer vertex_buffer_create(void)
{
    VertexBuffer vbuf = ALLOCATE_RENDER_TYPE(VertexBuffer);
    if(!vbuf)
        fatal_error("Failed to allocate vertex buffer!");
    glGenBuffers(1, &vbuf->handle);
    memset(vbuf->attribs, 0, sizeof(vbuf->attribs));
    return vbuf;
}

static void vertex_buffer_destroy(VertexBuffer vbuf)
{
    if(!vbuf) return;
    glDeleteBuffers(1, &vbuf->handle);
    free(vbuf);
}

static void vertex_buffer_set_stride(VertexBuffer vbuf, nkU64 byte_stride)
{
    NK_ASSERT(vbuf);

    vbuf->byte_stride = byte_stride;
}

static void vertex_buffer_enable_attrib(VertexBuffer vbuf, nkU32 index, AttribType type, nkU32 comps, nkU64 byte_offset)
{
    NK_ASSERT(vbuf);
    NK_ASSERT(index < NK_ARRAY_SIZE(vbuf->attribs));

    vbuf->attribs[index].enabled = NK_TRUE;
    vbuf->attribs[index].type = type;
    vbuf->attribs[index].components = comps;
    vbuf->attribs[index].byte_offset = byte_offset;
}

static void vertex_buffer_disable_attrib(VertexBuffer vbuf, nkU32 index)
{
    NK_ASSERT(vbuf);
    NK_ASSERT(index < NK_ARRAY_SIZE(vbuf->attribs));

    vbuf->attribs[index].enabled = NK_FALSE;
}

static void vertex_buffer_update(VertexBuffer vbuf, void* data, nkU64 bytes, BufferType type)
{
    NK_ASSERT(vbuf);

    GLenum gl_type;
    switch(type)
    {
        case BufferType_Static: gl_type = GL_STATIC_DRAW; break;
        case BufferType_Dynamic: gl_type = GL_DYNAMIC_DRAW; break;
        case BufferType_Stream: gl_type = GL_STREAM_DRAW; break;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbuf->handle);
    glBufferData(GL_ARRAY_BUFFER, bytes, data, gl_type);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

static void vertex_buffer_draw(VertexBuffer vbuf, DrawMode draw_mode, nkU64 vert_count)
{
    NK_ASSERT(vbuf);

    if(!vert_count) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbuf->handle);

    // Map the primitive type to the appropriate GL enum.
    GLenum primitive;
    switch(draw_mode)
    {
        case DrawMode_Points: primitive = GL_POINTS; break;
        case DrawMode_LineStrip: primitive = GL_LINE_STRIP; break;
        case DrawMode_LineLoop: primitive = GL_LINE_LOOP; break;
        case DrawMode_Lines: primitive = GL_LINES; break;
        case DrawMode_TriangleStrip: primitive = GL_TRIANGLE_STRIP; break;
        case DrawMode_TriangleFan: primitive = GL_TRIANGLE_FAN; break;
        case DrawMode_Triangles: primitive = GL_TRIANGLES; break;
    }

    // Setup the attributes for the buffer.
    for(nkU64 i=0, n=NK_ARRAY_SIZE(vbuf->attribs); i<n; ++i)
    {
        VertexAttrib* attrib = &vbuf->attribs[i];
        if(attrib->enabled)
        {
            GLenum attrib_type;
            switch(attrib->type)
            {
                case AttribType_SignedByte: attrib_type = GL_BYTE; break;
                case AttribType_UnsignedByte: attrib_type = GL_UNSIGNED_BYTE; break;
                case AttribType_SignedInt: attrib_type = GL_INT; break;
                case AttribType_UnsignedInt: attrib_type = GL_UNSIGNED_INT; break;
                case AttribType_Float: attrib_type = GL_FLOAT; break;
            }

            glVertexAttribPointer(i, attrib->components, attrib_type, GL_FALSE,
                vbuf->byte_stride, NK_CAST(void*,attrib->byte_offset));
            glEnableVertexAttribArray(i);
        }
    }

    // Draw the buffer data using the provided primitive type.
    glDrawArrays(primitive, 0, NK_CAST(GLsizei,vert_count));

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

static RenderTarget render_target_create(nkS32 w, nkS32 h, SamplerFilter filter, SamplerWrap wrap)
{
    RenderTarget target = ALLOCATE_RENDER_TYPE(RenderTarget);
    if(!target)
        fatal_error("Failed to allocate render target!");
    target->filter = filter;
    target->wrap = wrap;
    render_target_resize(target, w, h);
    return target;
}

static void render_target_destroy(RenderTarget target)
{
    if(!target) return;
    glDeleteFramebuffers(1, &target->handle);
    texture_destroy(target->color_target);
    free(target);
}

static void render_target_resize(RenderTarget target, nkS32 w, nkS32 h)
{
    NK_ASSERT(target);

    if(w <= 0 || h <= 0) return;

    // Delete the old contents (if any).
    glDeleteFramebuffers(1, &target->handle);
    texture_destroy(target->color_target);

    glGenFramebuffers(1, &target->handle);
    glBindFramebuffer(GL_FRAMEBUFFER, target->handle);

    target->color_target = texture_create(w,h, 4, NULL, target->filter, target->wrap);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target->color_target->handle, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        fatal_error("Failed to complete framebuffer resize!");

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

static GLuint shader_compile(const nkChar* source, GLenum type)
{
    const nkChar* sources[2] = { NULL, source };

    if(type == GL_VERTEX_SHADER) sources[0] = "#version 330\n#define VERT_SHADER 1\n";
    if(type == GL_FRAGMENT_SHADER) sources[0] = "#version 330\n#define FRAG_SHADER 1\n";

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 2, sources, NULL);
    glCompileShader(shader);

    GLint success;
    glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        GLint info_log_length;
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        nkChar* info_log = malloc(info_log_length * sizeof(nkChar));
        if(info_log)
        {
            glGetProgramInfoLog(shader, info_log_length, NULL, info_log);
            printf("Failed to link shader:\n%s\n", info_log);
            free(info_log);
        }
        return GL_NONE;
    }

    return shader;
}

static Shader shader_create(void* data, nkU64 bytes)
{
    Shader shader = ALLOCATE_RENDER_TYPE(Shader);
    if(!shader)
        fatal_error("Failed to allocate shader!");

    GLuint vert = shader_compile(data, GL_VERTEX_SHADER);
    GLuint frag = shader_compile(data, GL_FRAGMENT_SHADER);

    shader->program = glCreateProgram();

    glAttachShader(shader->program, vert);
    glAttachShader(shader->program, frag);

    glLinkProgram(shader->program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint success;
    glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
    if(!success)
    {
        GLint info_log_length;
        glGetProgramiv(shader->program, GL_INFO_LOG_LENGTH, &info_log_length);
        nkChar* info_log = malloc(info_log_length * sizeof(nkChar));
        if(info_log)
        {
            glGetProgramInfoLog(shader->program, info_log_length, NULL, info_log);
            printf("Failed to link shader:\n%s\n", info_log);
            free(info_log);
        }
    }

    return shader;
}

static void shader_destroy(Shader shader)
{
    if(!shader) return;
    glDeleteProgram(shader->program);
    free(shader);
}

static void shader_bind(Shader shader)
{
    if(!shader) glUseProgram(GL_NONE);
    else glUseProgram(shader->program);
}

static void shader_set_bool(Shader shader, const nkChar* name, nkBool val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniform1i(location, NK_CAST(nkS32, val));
}

static void shader_set_int(Shader shader, const nkChar* name, nkS32 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniform1i(location, val);
}

static void shader_set_float(Shader shader, const nkChar* name, nkF32 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniform1f(location, val);
}

static void shader_set_vec2(Shader shader, const nkChar* name, nkVec2 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniform2fv(location, 1, val.raw);
}

static void shader_set_vec3(Shader shader, const nkChar* name, nkVec3 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniform3fv(location, 1, val.raw);
}

static void shader_set_vec4(Shader shader, const nkChar* name, nkVec4 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniform4fv(location, 1, val.raw);
}

static void shader_set_mat2(Shader shader, const nkChar* name, nkMat2 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniformMatrix2fv(location, 1, GL_FALSE, val.raw);
}

static void shader_set_mat3(Shader shader, const nkChar* name, nkMat3 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniformMatrix3fv(location, 1, GL_FALSE, val.raw);
}

static void shader_set_mat4(Shader shader, const nkChar* name, nkMat4 val)
{
    NK_ASSERT(shader);
    GLint location = glGetUniformLocation(shader->program, name);
    if(location == -1) printf("No shader uniform found: %s\n", name);
    glUniformMatrix4fv(location, 1, GL_FALSE, val.raw);
}

static GLenum sampler_filter_to_gl(SamplerFilter filter)
{
    switch(filter)
    {
        case SamplerFilter_Nearest: return GL_NEAREST; break;
        case SamplerFilter_Linear: return GL_LINEAR; break;
        default:
        {
            // Unsupported texture filter.
            NK_ASSERT(NK_FALSE);
        } break;
    }
    return GL_NONE;
}

static GLenum sampler_wrap_to_gl(SamplerWrap wrap)
{
    switch(wrap)
    {
        case SamplerWrap_Repeat: return GL_REPEAT; break;
        case SamplerWrap_Clamp: return GL_CLAMP_TO_EDGE; break;
        default:
        {
            // Unsupported texture wrap.
            NK_ASSERT(NK_FALSE);
        } break;
    }
    return GL_NONE;
}

static GLenum bpp_to_gl_format(nkS32 bpp)
{
    switch(bpp)
    {
        case(3): return GL_RGB; break;
        case(4): return GL_RGBA; break;
        default:
        {
            // Unsupported BPP that has no appropriate format.
            NK_ASSERT(NK_FALSE);
        } break;
    }
    return GL_NONE;
}

static Texture texture_create(nkS32 w, nkS32 h, nkS32 bpp, void* data, SamplerFilter filter, SamplerWrap wrap)
{
    Texture texture = ALLOCATE_RENDER_TYPE(Texture);
    if(!texture)
        fatal_error("Failed to allocate texture!");

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &texture->handle);
    glBindTexture(GL_TEXTURE_2D, texture->handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler_filter_to_gl(filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler_filter_to_gl(filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler_wrap_to_gl(wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler_wrap_to_gl(wrap));

    GLenum gl_format = bpp_to_gl_format(bpp);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, w,h, 0, gl_format, GL_UNSIGNED_BYTE, data);

    texture->size.x = NK_CAST(nkF32, w);
    texture->size.y = NK_CAST(nkF32, h);

    return texture;
}

static void texture_destroy(Texture texture)
{
    if(!texture) return;
    glDeleteTextures(1, &texture->handle);
    free(texture);
}

static nkVec2 texture_get_size(Texture texture)
{
    NK_ASSERT(texture);
    return texture->size;
}

static nkF32 texture_get_width (Texture texture)
{
    NK_ASSERT(texture);
    return texture->size.x;
}

static nkF32 texture_get_height(Texture texture)
{
    NK_ASSERT(texture);
    return texture->size.y;
}

/*////////////////////////////////////////////////////////////////////////////*/
