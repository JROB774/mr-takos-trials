uniform sampler2D u_texture;

const vec2 k_intensity = vec2(0.0025,0.0025);

#ifdef VERT_SHADER /*/////////////////////////////////////////////////////////*/

layout (location = 0) in vec4 i_xyuv;

out vec2 v_texcoord;

void main()
{
    gl_Position = vec4(i_xyuv.xy,0,1);
    v_texcoord = i_xyuv.zw;
}

#endif /* VERT_SHADER ////////////////////////////////////////////////////////*/

#ifdef FRAG_SHADER /*/////////////////////////////////////////////////////////*/

in vec2 v_texcoord;

out vec4 o_fragcolor;

void main()
{
    o_fragcolor.r = texture(u_texture, v_texcoord - vec2( k_intensity.x,0.0)).r;
    o_fragcolor.g = texture(u_texture, v_texcoord - vec2(0.0,-k_intensity.y)).g;
    o_fragcolor.b = texture(u_texture, v_texcoord - vec2(-k_intensity.x,0.0)).b;
    o_fragcolor.a = 1.0;
}

#endif /* FRAG_SHADER ////////////////////////////////////////////////////////*/
