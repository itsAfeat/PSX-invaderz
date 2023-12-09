#include "includes/core.h"
#include "includes/graphics.h"
#include "includes/font.h"
#include "includes/controller.h"
#include "includes/audio.h"
#include "includes/game.h"


void loadSprite(const char *filename, SPRITE *sprt)
{
    TIM_IMAGE _tim;
    u_int *_buffer = (u_int*) loadFile(filename);
    loadTexture(_buffer, &_tim);
    getSprite(&_tim, sprt);
    free(_buffer);
}

void loadFont(const char *filename, FONT_SPRITE *sprt, int w, int h, int mIndex)
{
    TIM_IMAGE _tim;
    u_int *_buffer = (u_int*) loadFile(filename);
    loadTexture(_buffer, &_tim);
    getFont(&_tim, sprt, w, h, mIndex);
    free(_buffer);
}

void printLoadingFont(RenderContext *ctx, FONT_SPRITE fontSprt)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: LOADING\n");
        FntFlush(-1);
    #endif

    RenderBuffer *disp_buffer = &(ctx->buffers[ctx->active_buffer ^ 1]);
    ClearOTagR(disp_buffer->ot, OT_LENGTH);
    printFont(ctx, 160, 128, "loading...", fontSprt, HCENTER_ALIGN | VCENTER_ALIGN);
    flipBuffers(ctx);
}

void loadStuff(RenderContext *ctx)
{
    // This is shit... This whole method is shit

    loadFont("\\FONT8X16.TIM;1", &font8x16, 8, 16, 28);
    printLoadingFont(ctx, font8x16);

    loadSprite("\\TITLE.TIM;1", &TITLE_SPRITE);
    printLoadingFont(ctx, font8x16);

    loadSprite("\\PLAYER.TIM;1", &PLAYER_SPRITE);
    printLoadingFont(ctx, font8x16);

    loadSprite("\\WALLFULL.TIM;1", &WALLFULL_SPRITE);
    printLoadingFont(ctx, font8x16);

    loadSprite("\\WALLHIT.TIM;1", &WALLHIT_SPRITE);
    printLoadingFont(ctx, font8x16);

    loadSprite("\\WALLBRKN.TIM;1", &WALLBRKN_SPRITE);
    printLoadingFont(ctx, font8x16);

    loadSprite("\\WALLRUIN.TIM;1", &WALLRUIN_SPRITE);
    printLoadingFont(ctx, font8x16);


    TIM_IMAGE _tim;
    u_int *_buffer = (u_int*)loadFile("\\ALIEN1.TIM;1");
    loadTexture(_buffer, &_tim);
    for (int i = 0; i < ALIEN_AMOUNT; i++)
    {
        printLoadingFont(ctx, font8x16);

        SPRITE ALIEN;
        getSprite(&_tim, &ALIEN);
        ALIEN_SPRITES[i] = ALIEN;
    }
    free(_buffer);

    uploadXA("\\THEME.XA;1", &THEME_XA, 1);
}


int main()
{
    init();
    controllerInit();

    RenderContext ctx;
    setupContext(&ctx, 320, 256, 0, 0, rgbToCVector(0, 0, 0));
    loadStuff(&ctx);

    // changeChannel(0);
    SCENEs[CURR_SCENE]->init(&ctx);
    for (;;)
    {
        SCENEs[CURR_SCENE]->update(&ctx);
        SCENEs[CURR_SCENE]->draw(&ctx);

        flipBuffers(&ctx);
    }

    return 0;
}