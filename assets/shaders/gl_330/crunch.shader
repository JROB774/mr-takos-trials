uniform sampler2D u_texture;

const float k_resolution = 10.0;
const float k_saturation = 1.5;

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
    vec4 frag = texture(u_texture, v_texcoord);

    vec3 fract_frag = frag.rgb - fract(frag.rgb * k_resolution) / k_resolution;
    float luma = dot(fract_frag, vec3(0.3, 0.59, 0.11));
    vec3 chroma = (fract_frag - luma) * k_saturation;

    o_fragcolor = vec4(luma + chroma, 1.0);
}

#endif /* FRAG_SHADER ////////////////////////////////////////////////////////*/
