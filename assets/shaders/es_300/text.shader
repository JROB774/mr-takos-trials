precision mediump float;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform bool u_usetex;

uniform sampler2D u_texture;

#ifdef VERT_SHADER /*/////////////////////////////////////////////////////////*/

in vec2 i_pos;
in vec2 i_tex;
in vec4 i_col;

out vec2 v_texcoord;
out vec4 v_color;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(i_pos,0,1);
    v_texcoord = i_tex;
    v_color = i_col;
}

#endif /* VERT_SHADER ////////////////////////////////////////////////////////*/

#ifdef FRAG_SHADER /*/////////////////////////////////////////////////////////*/

in vec2 v_texcoord;
in vec4 v_color;

out vec4 o_fragcolor;

void main()
{
    o_fragcolor = v_color;
    if(u_usetex)
    {
        o_fragcolor.a = texture(u_texture, v_texcoord).r;
    }
}

#endif /* FRAG_SHADER ////////////////////////////////////////////////////////*/
