#include "includes/core.h"

void init()
{
    ResetGraph(0);
    CdInit();
    SpuInit();

    #ifdef DEBUG
        FntLoad(960, 0);
        FntOpen(0, 8, 320, 256, 0, 100);
    #endif // DEBUG
}

void setupContext(RenderContext *ctx, int w, int h, int x, int y, CVECTOR rgb)
{
    // Place the framebuffers vertically in VRAM
    SetDefDrawEnv(&(ctx->buffers[0].draw_env), x, y, w, h);
    SetDefDispEnv(&(ctx->buffers[0].disp_env), x, y, w, h);
    SetDefDrawEnv(&(ctx->buffers[1].draw_env), x, h, w, h);
    SetDefDispEnv(&(ctx->buffers[1].disp_env), x, h, w, h);

    // Screen offset to center the picture vertically
    ctx->buffers[0].disp_env.screen.y = 24;
    ctx->buffers[1].disp_env.screen.y = 24;

    // Force PAL video standard
    SetVideoMode(MODE_PAL);

    // Set the default background color and enable "auto clearing"
    setRGB0(&(ctx->buffers[0].draw_env), rgb.r, rgb.g, rgb.b);
    setRGB0(&(ctx->buffers[1].draw_env), rgb.r, rgb.g, rgb.b);
    ctx->buffers[0].draw_env.isbg = 1;
    ctx->buffers[1].draw_env.isbg = 1;

    // Initialize the first buffer and clear it... in reverse
    // for some apparent reason, that as of writing this comment
    // is unbeknownst to me. Fuck the playstation
    ctx->active_buffer = 0;
    ctx->next_packet   = ctx->buffers[0].buffer;
    ClearOTagR(ctx->buffers[0].ot, OT_LENGTH);

    // Turn on the video output
    SetDispMask(1);
}

char *loadFile(const char *filename)
{
    CdlFILE filePos;
    int numsecs;
    char* buffer;

    if (CdSearchFile(&filePos, (char *) filename) == NULL)
    {
        #ifdef DEBUG
            printf("%s not found\n", filename);
        #endif // DEBUG
    }
    else
    {
        #ifdef DEBUG
            printf("%s was found\n", filename);
        #endif // DEBUG

        numsecs = (filePos.size+2047)/2048;
        buffer  = (char *)malloc(2048*numsecs);

        CdControl(CdlSetloc, (u_char *)&filePos.pos, 0);
        CdRead(numsecs, (u_long *)buffer, CdlModeSpeed);
        CdReadSync(0, 0);
    }

    return buffer;
}

int clamp(int i, int min, int max)
{
    const int t = i < min ? min : i;
    return t > max ? max : t;
}

int isBetween(int n, int min, int max)
{ return (n >= min && n <= max); }

CVECTOR rgbToCVector(int r, int g, int b)
{
    CVECTOR cv;
    cv.r = r;
    cv.g = g;
    cv.b = b;

    return cv;
}

int getStrWidth(const char* string)
{
    int cw = 0; int gw = 0; int i = 0;
    char c = string[0];

    while (c != '\0')
    {
        c = string[i];

        if (c == '\n' || c == '\0')
        {
            if (cw > gw) { gw = cw; }
            cw = 0;
        }
        else { cw++; }
        i++;
    }

    return gw;
}

int getStrHeight(const char* string)
{
    int h = 1; int i = 0;
    char c = string[0];

    while (c != '\0')
    {
        if (c == '\n') { h++; }
        i++;

        c = string[i];
    }

    return h;
}