#pragma once

#include <string.h>
#include <stdio.h>
#include "core.h"

typedef struct {
    u_short tpage;
    u_short clut;
    u_char  w,h;    // The width and height of the characters
    u_int   mIndex; // The max index for the UV calculations
} FONT_SPRITE;

typedef enum {
    LEFT_ALIGN      = 0b0000,
    RIGHT_ALIGN     = 0b0001,
    HCENTER_ALIGN   = 0b0010,
    TOP_ALIGN       = 0b0000,
    BOT_ALIGN       = 0b0100,
    VCENTER_ALIGN   = 0b1000
} FONT_ALIGN;

static FONT_SPRITE font8x16;

void getFont(TIM_IMAGE* tim, FONT_SPRITE *fontSprt, int w, int h, int mIndex);
void printFont(RenderContext *ctx, int x, int y, const char *string, FONT_SPRITE fontSprt, FONT_ALIGN align);