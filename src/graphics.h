#pragma once

typedef struct {
    u_short tpage;
    u_short clut;
    u_char  u,v;
    u_char  w,h;
    u_int   x,y;
    int     semiTrans;
} SPRITE;


void display(RenderContext* ctx)
{
    DrawSync(0);
    VSync(0);

    RenderBuffer* draw_buffer = &(ctx->buffers[ctx->active_buffer]);
    RenderBuffer* disp_buffer = &(ctx->buffers[ctx->active_buffer^1]);

    PutDispEnv(&(disp_buffer->disp));
    DrawOTagEnv(&(draw_buffer->ot[OT_LENGTH-1]), &(draw_buffer->draw));

    ctx->active_buffer ^= 1;
    ctx->next_packet    = disp_buffer->buffer;
    ClearOTagR(disp_buffer->ot, OT_LENGTH);
}

void getSprite(TIM_IMAGE* tim, SPRITE* sprite)
{
    sprite->tpage = getTPage(tim->mode&0x3, 0, tim->prect->x, tim->prect->y);
    if (tim->mode&0x8) { sprite->clut = getClut(tim->crect->x, tim->crect->y); }

    sprite->w = tim->prect->w<<(2-tim->mode&0x3);
    sprite->h = tim->prect->w;

    sprite->u = (tim->prect->x&0x3f)<<(2-tim->mode&0x3);
    sprite->v = tim->prect->y&0xff;
}

void setupSprite(SPRITE* sprite, int x, int y, int semiTrans)
{
    sprite->x = x;
    sprite->y = y;
    sprite->semiTrans = semiTrans;
}

void loadTexture(u_int* tim, TIM_IMAGE* tparam)
{
    GetTimInfo((uint32_t*) tim, tparam);
    LoadImage(tparam->prect, (u_long*)tparam->paddr);
    DrawSync(0);

    if (tparam->mode&0x8)
    {
        LoadImage(tparam->crect, (u_long*)tparam->caddr);
        DrawSync(0);
    }
}

void* newPrimitive(RenderContext* ctx, size_t size)
{
    RenderBuffer* buffer = &(ctx->buffers[ctx->active_buffer]);
    uint8_t*      prim   = ctx->next_packet;

    addPrim(buffer->ot, prim);
    ctx->next_packet += size;

    assert(ctx->next_packet <= &(buffer->buffer[BUFFER_LENGTH]));
    return (void*) prim;
}

void sortSprite(RenderContext* ctx, int fixed, int x, int y, SPRITE* sprite)
{
    SPRT* sprt;
    DR_TPAGE* tpage;

    sprt = (SPRT*) newPrimitive(ctx, sizeof(SPRT));
    setSprt(sprt);

    if (fixed)  { setXY0(sprt, x>>12, y>>12); }
    else        { setXY0(sprt, x, y); }
    setWH(sprt, sprite->w, sprite->h);
    setUV0(sprt, sprite->u, sprite->v);
    setShadeTex(sprt, 1);
    setSemiTrans(sprt, sprite->semiTrans);
    sprt->clut = sprite->clut;

    tpage = (DR_TPAGE*) newPrimitive(ctx, sizeof(DR_TPAGE));
    setDrawTPage(tpage, 0, 1, sprite->tpage);
}
