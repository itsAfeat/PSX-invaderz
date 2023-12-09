#include "includes/core.h"
#include "includes/graphics.h"

#include <assert.h>

void flipBuffers(RenderContext *ctx)
{
    // Wait for the GPU to finish drawing, and for vblank
    // to prevent screen tearing
    DrawSync(0);
    VSync(0);

    RenderBuffer *draw_buffer = &(ctx->buffers[ctx->active_buffer]);
    RenderBuffer *disp_buffer = &(ctx->buffers[ctx->active_buffer ^ 1]);

	// Display the framebuffer the GPU has just finished drawing and start
	// rendering the display list that was filled up in the main loop.
    PutDispEnv(&(disp_buffer->disp_env));
    DrawOTagEnv(&(draw_buffer->ot[OT_LENGTH - 1]), &(draw_buffer->draw_env));

	// Switch over to the next buffer, clear it and reset the packet allocation
	// pointer.
    ctx->active_buffer ^= 1;
    ctx->next_packet    = disp_buffer->buffer;
    ClearOTagR(disp_buffer->ot, OT_LENGTH);
}

void flipBuffers2(RenderContext *ctx1, RenderContext *ctx2)
{
    // Wait for the GPU to finish drawing, and for vblank
    // to prevent screen tearing
    DrawSync(0);
    VSync(0);

    RenderBuffer *draw_buffer1 = &(ctx1->buffers[ctx1->active_buffer]);
    RenderBuffer *disp_buffer1 = &(ctx1->buffers[ctx1->active_buffer ^ 1]);
    RenderBuffer *draw_buffer2 = &(ctx2->buffers[ctx2->active_buffer]);
    RenderBuffer *disp_buffer2 = &(ctx2->buffers[ctx2->active_buffer ^ 1]);

	// Display the framebuffer the GPU has just finished drawing and start
	// rendering the display list that was filled up in the main loop.
    PutDispEnv(&(disp_buffer1->disp_env));
    DrawOTagEnv(&(draw_buffer1->ot[OT_LENGTH - 1]), &(draw_buffer1->draw_env));
    PutDispEnv(&(disp_buffer2->disp_env));
    DrawOTagEnv(&(draw_buffer2->ot[OT_LENGTH - 1]), &(draw_buffer2->draw_env));

	// Switch over to the next buffer, clear it and reset the packet allocation
	// pointer.
    ctx1->active_buffer ^= 1;
    ctx1->next_packet    = disp_buffer1->buffer;
    ClearOTagR(disp_buffer1->ot, OT_LENGTH);
    
    ctx2->active_buffer ^= 1;
    ctx2->next_packet    = disp_buffer2->buffer;
    ClearOTagR(disp_buffer2->ot, OT_LENGTH);
}

void getSprite(TIM_IMAGE *tim, SPRITE *sprite)
{
    sprite->tpage = getTPage(tim->mode&0x3, 0, tim->prect->x, tim->prect->y);
    if (tim->mode&0x8) { sprite->clut = getClut(tim->crect->x, tim->crect->y); }

    sprite->w = tim->prect->w<<(2-tim->mode&0x3);
    sprite->h = tim->prect->h;

    sprite->u = (tim->prect->x&0x3f)<<(2-tim->mode&0x3);
    sprite->v = tim->prect->y&0xff;

    sprite->u = (tim->prect->x&0x3f)<<(2-tim->mode&0x3);
    sprite->v = tim->prect->y&0xff;
}

void setupSprite(SPRITE *sprite, int x, int y, int semiTrans)
{
    sprite->x = x;
    sprite->y = y;
    sprite->semiTrans = semiTrans;
}

void loadTexture(u_int *tim, TIM_IMAGE *tparam)
{
    GetTimInfo((uint32_t *) tim, tparam);
    LoadImage(tparam->prect, tparam->paddr);
    DrawSync(0);

    if (tparam->mode&0x8)
    {
        LoadImage(tparam->crect, tparam->caddr);
        DrawSync(0);
    }
}

void sortSprite(int fixed, int x, int y, RenderContext *ctx, SPRITE *sprite)
{
    SPRT *sprt;
    DR_TPAGE *tpage;

    sprt = (SPRT*)ctx->next_packet;
    setSprt(sprt);

    if (fixed)  { setXY0(sprt, x>>12, y>>12); }
    else        { setXY0(sprt, x, y); }
    setWH(sprt, sprite->w, sprite->h);
    setUV0(sprt, sprite->u, sprite->v);
    setShadeTex(sprt, 1);
    setSemiTrans(sprt, sprite->semiTrans);
    sprt->clut = sprite->clut;

    addPrim(ctx->buffers[ctx->active_buffer].ot, sprt);
    
    ctx->next_packet += sizeof(SPRT);
	assert(ctx->next_packet <= &(ctx->buffers[ctx->active_buffer].buffer[BUFFER_LENGTH]));

    tpage = (DR_TPAGE *)ctx->next_packet;
    setDrawTPage(tpage, 0, 1, sprite->tpage);
    addPrim(ctx->buffers[ctx->active_buffer].ot, tpage);
    
    ctx->next_packet += sizeof(DR_TPAGE);
	assert(ctx->next_packet <= &(ctx->buffers[ctx->active_buffer].buffer[BUFFER_LENGTH]));
}

void sortSprite2(int fixed, RenderContext *ctx, SPRITE *sprite)
{ sortSprite(fixed, sprite->x, sprite->y, ctx, sprite); }