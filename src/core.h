#pragma once

#define OT_LENGTH 16
#define BUFFER_LENGTH 8192

typedef struct {
    DISPENV disp;
    DRAWENV draw;

    uint32_t ot[OT_LENGTH];
    uint8_t  buffer[BUFFER_LENGTH];
} RenderBuffer;

typedef struct {
    RenderBuffer buffers[2];
    uint8_t      *next_packet;
    int          active_buffer;
} RenderContext;


void setupContext(RenderContext *ctx, int w, int h, int r, int g, int b)
{
    SetDefDispEnv(&(ctx->buffers[0].disp), 0, 0, w, h);
    SetDefDispEnv(&(ctx->buffers[1].disp), 0, h, w, h);

    ctx->buffers[0].disp.screen.y = ctx->buffers[1].disp.screen.y = 24;
    SetVideoMode(MODE_PAL);

    SetDefDrawEnv(&(ctx->buffers[0].draw), 0, 0, w, h);
    SetDefDrawEnv(&(ctx->buffers[1].draw), 0, h, w, h);

    setRGB0(&(ctx->buffers[0].draw), r, g, b);
    setRGB0(&(ctx->buffers[1].draw), r, g, b);

    ctx->buffers[0].draw.isbg = 1;
    ctx->buffers[1].draw.isbg = 1;

    ctx->active_buffer = 0;
    ctx->next_packet= ctx->buffers[0].buffer;
    ClearOTagR(ctx->buffers[0].ot, OT_LENGTH);
}

void init(RenderContext* ctx, int w, int h, int r, int g, int b)
{
    ResetGraph(0);
    CdInit();

    setupContext(ctx, w, h, r, g, b);

    SetDispMask(1);
    FntLoad(960, 0);
    FntOpen(0, 8, w, h, 0, 100);
}

char* loadFile(const char* filename)
{
    CdlFILE filePos;
    int numsecs;
    char* buff;

    if (CdSearchFile(&filePos, (char*) filename) == NULL)
    { printf("%s could not be found!\n", filename); }
    else
    {
        numsecs = (filePos.size+2047)/2048;
        buff = (char*)malloc(2048*numsecs);

        CdControl(CdlSetloc, (u_char*)&filePos.pos, 0);
        CdRead(numsecs, (u_long*)buff, CdlModeSpeed);
        CdReadSync(0, 0);
    }

    return buff;
}
