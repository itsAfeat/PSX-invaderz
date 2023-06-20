#define DEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxapi.h>
#include <psxpad.h>
#include <psxcd.h>

#include "core.h"
#include "graphics.h"
#include "font.h"
#include "controller.h"
#include "audio.h"
#include "game.h"

#define SCREEN_W 320
#define SCREEN_H 256

void loadStuff()
{

}

int main()
{
    RenderContext ctx;
    init(&ctx, SCREEN_W, SCREEN_H, 0, 120, 50);
    loadStuff();

    SPRITE font;
    SPRITE ayylmao1;

    TIM_IMAGE _tim;
    u_int* _buffer = (u_int*)loadFile("\\FONT.TIM;1");
    loadTexture(_buffer, &_tim);
    getSprite(&_tim, &font);
    free(_buffer);

    _buffer = (u_int*)loadFile("\\AYYLMAO1.TIM;1");
    loadTexture(_buffer, &_tim);
    getSprite(&_tim, &ayylmao1);
    free(_buffer);

    for(;;)
    {
        FntPrint(-1, "GOOFY AH");
        FntFlush(-1);
        
        sortSprite(&ctx, 0, 64, 64, &font);
        sortSprite(&ctx, 0, 128, 32, &ayylmao1);
        display(&ctx);
    }

    return 0;
}
