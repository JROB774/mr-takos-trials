/*////////////////////////////////////////////////////////////////////////////*/

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

/*////////////////////////////////////////////////////////////////////////////*/
