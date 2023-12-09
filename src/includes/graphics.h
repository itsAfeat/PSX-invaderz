#pragma once

typedef struct {
    u_short tpage;
    u_short clut;
    u_char  u,v;
    u_int   x,y;
    u_char  w,h;
    int     semiTrans;
} SPRITE;

static char *TIMs[] = {""};
static SPRITE **SPRITEs;

// Display functions
void flipBuffers(RenderContext *ctx);
void flipBuffers2(RenderContext *ctx1, RenderContext *ctx2);

// Setup / loading functions
void getSprite(TIM_IMAGE *tim, SPRITE *sprite);
void setupSprite(SPRITE *sprite, int x, int y, int semiTrans);
void loadTexture(u_int *tim, TIM_IMAGE *tparam);

// Sorting (drawing) functions
void sortSprite(int fixed, int x, int y, RenderContext *ctx, SPRITE *sprite);
void sortSprite2(int fixed, RenderContext *ctx, SPRITE *sprite);