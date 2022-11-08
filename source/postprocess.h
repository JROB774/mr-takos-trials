#ifndef POSTPROCESS_H__ /*////////////////////////////////////////////////////*/
#define POSTPROCESS_H__

static void         postprocess_init         (void);
static void         postprocess_quit         (void);
static void         postprocess_push_effect  (Shader effect);
static void         postprocess_pop_effect   (void);
static void         postprocess_clear_effects(void);
static void         postprocess_execute      (void);
static RenderTarget postprocess_get_result   (void);

#endif /* POSTPROCESS_H__ ////////////////////////////////////////////////////*/
