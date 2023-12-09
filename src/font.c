#include "includes/font.h"

void getFont(TIM_IMAGE* tim, FONT_SPRITE *fontSprt, int w, int h, int mIndex)
{
    fontSprt->tpage = getTPage(tim->mode&0x3, 0, tim->prect->x, tim->prect->y);
    if (tim->mode&0x8) { fontSprt->clut = getClut(tim->crect->x, tim->crect->y); }

    fontSprt->w = w;
    fontSprt->h = h;
    fontSprt->mIndex = mIndex;
}

void printFont(RenderContext *ctx, int x, int y, const char *string, FONT_SPRITE fontSprt, FONT_ALIGN align)
{
    if ((align&RIGHT_ALIGN) != 0)
    { x -= (getStrWidth(string)*fontSprt.w); }
    if ((align&BOT_ALIGN) != 0)
    { y -= (getStrHeight(string)*fontSprt.h); }

    if ((align&HCENTER_ALIGN) != 0)
    { x -= (getStrWidth(string)*fontSprt.w)/2; }
    if ((align&VCENTER_ALIGN) != 0)
    { y -= (getStrHeight(string)*fontSprt.h)/2; }

    int start_x = x;
    int customChar = 0;

    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '\n')
        { x = start_x; y += fontSprt.h; continue; }
        if (string[i] == '~' && !customChar)
        { customChar = 1; continue; }

        int u_offset, v_offset;
        int index = ((int)string[i])-32;

        if (index != 0)
        {
            if (customChar && string[i] != '~')
            { index = ((int)'~'-32)+(string[i]-'0'); customChar = 0; }

            int u = index-(((int)index/fontSprt.mIndex)*fontSprt.mIndex);
            u_offset = u == 0 ? fontSprt.mIndex : u;
            v_offset = (int)index/fontSprt.mIndex;

            if (index%fontSprt.mIndex == 0) { v_offset--; }
            u_offset--;
        }
        else
        { x += fontSprt.w; continue; }

        SPRT* sprt;
        DR_TPAGE* tpage;

        sprt = (SPRT*)ctx->next_packet;
        setSprt(sprt);
        setXY0(sprt, x, y);
        setWH(sprt, fontSprt.w, fontSprt.h);
        setUV0(sprt, fontSprt.w*u_offset, fontSprt.h*v_offset);
        setShadeTex(sprt, 1);
        sprt->clut = fontSprt.clut;

        addPrim(ctx->buffers[ctx->active_buffer].ot, sprt);
        ctx->next_packet += sizeof(SPRT);

        tpage = (DR_TPAGE*)ctx->next_packet;
        setDrawTPage(tpage, 0, 1, fontSprt.tpage);
        addPrim(ctx->buffers[ctx->active_buffer].ot, tpage);
        ctx->next_packet += sizeof(DR_TPAGE);

        x += fontSprt.w;
    }
}