#pragma once

typedef enum {
    ID_DEBUG    = 0,
    ID_TITLE    = 1,
    ID_GAME     = 2,
    ID_SCORE    = 3,
    ID_CREDITS  = 4
} SCENE_ENUM;

typedef struct {
    void (* const init)(RenderContext *ctx);
    void (* const draw)(RenderContext *ctx);
    void (* const update)(RenderContext *ctx);
} SCENE;


// Definitions
#define TIMER_SECOND    50  // 50 because 50 fps (PAL), so 50 = 1 sec
#define ALIEN_AMOUNT    22  // The amount of aliens to "spawn"
#define ALIEN_MOVE      6   // The amount the aliens move
#define ALIEN_STEP      20  // How many horizontal steps before a vertical one

// Global variables
static SPRITE ALIEN_SPRITES[ALIEN_AMOUNT];
static SPRITE TITLE_SPRITE, PLAYER_SPRITE;
static SPRITE WALLFULL_SPRITE, WALLHIT_SPRITE;
static SPRITE WALLBRKN_SPRITE, WALLRUIN_SPRITE;

static int MOVE_COUNTER, MOVE_TIMER;
static int MOVE_X, MOVE_Y, FLIP;
static int MENU_CRS_Y, MENU_INDEX;

// Function declerations
void changeScene(RenderContext *ctx, SCENE_ENUM newScene);

/// SCENES
// Debug
static void debug_init(RenderContext *ctx)
{
    MOVE_COUNTER = ALIEN_STEP/2;
    MOVE_TIMER = MOVE_X = MOVE_Y = 0;
    FLIP = 1;

    int half = ALIEN_AMOUNT/2;
    int startX = 160-((half*18)/2);

    for (int i = 0; i < ALIEN_AMOUNT; i++)
    {
        int x = startX+18*(i >= half ? i-half : i);
        int y = 18+(i >= half ? 18 : 0);

        setupSprite(&ALIEN_SPRITES[i], x, y, 0);
    }
}
static void debug_draw(RenderContext *ctx)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: DEBUG");
        FntFlush(-1);
    #endif

    for (int i = 0; i < ALIEN_AMOUNT; i++)
    { sortSprite(0, ALIEN_SPRITES[i].x, ALIEN_SPRITES[i].y, ctx, &ALIEN_SPRITES[i]); }
}
static void debug_update(RenderContext *ctx)
{
    pollController(&con1);
    if (isPressed(con1, PAD_SELECT)) { changeScene(ctx, ID_TITLE); }


    MOVE_TIMER++;
    if (MOVE_TIMER % TIMER_SECOND == 0)
    {
        MOVE_COUNTER++;
        MOVE_X = ALIEN_MOVE * (FLIP);

        if (MOVE_COUNTER > ALIEN_STEP)
        {
            MOVE_COUNTER = 0;
            MOVE_Y = ALIEN_MOVE*2;
            FLIP = -FLIP;
        }

        for (int i = 0; i < ALIEN_AMOUNT; i++)
        {
            if (MOVE_Y == 0)
            { ALIEN_SPRITES[i].x += MOVE_X; }
            ALIEN_SPRITES[i].y += MOVE_Y;
        }

        MOVE_X = MOVE_Y = 0;
    }
}

// Title
static void title_init(RenderContext *ctx)
{
    MENU_CRS_Y = 108;
    MENU_INDEX = 0;

    MOVE_COUNTER = 10; FLIP = 1;
    MOVE_TIMER = MOVE_X = 0;

    setupSprite(&TITLE_SPRITE, 94, 16, 0);
    playXA2(THEME_XA, 0);
}
static void title_draw(RenderContext *ctx)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: TITLE\n");
        FntFlush(-1);
    #endif

    sortSprite2(0, ctx, &TITLE_SPRITE);

    // printFont(ctx, 160, 73, "=======================", font8x16, HCENTER_ALIGN | VCENTER_ALIGN);
    printFont(ctx, 160, 108, "Start", font8x16, HCENTER_ALIGN | VCENTER_ALIGN);
    printFont(ctx, 160, 128, "Scoreboard", font8x16, HCENTER_ALIGN | VCENTER_ALIGN);
    printFont(ctx, 160, 148, "Credits", font8x16, HCENTER_ALIGN | VCENTER_ALIGN);
    // printFont(ctx, 160, 183, "=======================", font8x16, HCENTER_ALIGN | VCENTER_ALIGN);

    printFont(ctx, 100, MENU_CRS_Y, "~2", font8x16, LEFT_ALIGN | VCENTER_ALIGN);
    printFont(ctx, 160, 238, "(C) 1995 HappySoft", font8x16, HCENTER_ALIGN | BOT_ALIGN);
}
static void title_update(RenderContext *ctx)
{
    pollController(&con1);
    
    if (isHeld(con1, PAD_START))
    { printFont(ctx, 160, 60, "radise", font8x16, HCENTER_ALIGN | VCENTER_ALIGN); }
    if (isPressed(con1, PAD_SELECT))
    { changeScene(ctx, ID_DEBUG); }

    if (isPressed(con1, PAD_DOWN))
    {
        MENU_CRS_Y += 20;
        MENU_INDEX++;

        if (MENU_CRS_Y > 148)
        { MENU_CRS_Y = 108; MENU_INDEX = 0; }
    }
    if (isPressed(con1, PAD_UP))
    {
        MENU_CRS_Y -= 20;
        MENU_INDEX--;

        if (MENU_CRS_Y < 108)
        { MENU_CRS_Y = 148; MENU_INDEX = 2; }
    }

    if (isPressed(con1, PAD_CROSS))
    { changeScene(ctx, MENU_INDEX+2); }

    MOVE_TIMER++;
    if (MOVE_TIMER % 4 == 0)
    {
        MOVE_COUNTER++;
        if (MOVE_COUNTER > 20)
        { FLIP = -FLIP; MOVE_COUNTER = 0; }

        TITLE_SPRITE.y += FLIP;
    }

    updateXA();
}

static void game_init(RenderContext *ctx)
{}
static void game_draw(RenderContext *ctx)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: GAME");
        FntFlush(-1);
    #endif
}
static void game_update(RenderContext *ctx)
{
    pollController(&con1);
    if (isPressed(con1, PAD_SELECT))
    { changeScene(ctx, ID_TITLE); }
}

static void score_init(RenderContext *ctx)
{}
static void score_draw(RenderContext *ctx)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: SCORE");
        FntFlush(-1);
    #endif
}
static void score_update(RenderContext *ctx)
{
    pollController(&con1);
    if (isPressed(con1, PAD_SELECT))
    { changeScene(ctx, ID_TITLE); }
}

static void credits_init(RenderContext *ctx)
{}
static void credits_draw(RenderContext *ctx)
{
    #ifdef DEBUG
        FntPrint(-1, "SCENE: CREDITS\n");
        FntFlush(-1);
    #endif
}
static void credits_update(RenderContext *ctx)
{
    pollController(&con1);
    if (isPressed(con1, PAD_SELECT))
    { changeScene(ctx, ID_TITLE); }
}


static SCENE sc_debug   = { debug_init, debug_draw, debug_update };
static SCENE sc_title   = { title_init, title_draw, title_update };
static SCENE sc_game    = { game_init, game_draw, game_update };
static SCENE sc_score   = { score_init, score_draw, score_update };
static SCENE sc_credits = { credits_init, credits_draw, credits_update };

static int CURR_SCENE = ID_TITLE;
static SCENE *SCENEs[] = {
    &sc_debug,
    &sc_title,
    &sc_game,
    &sc_score,
    &sc_credits
};

// Function constructions
void changeScene(RenderContext *ctx, SCENE_ENUM newScene)
{
    stopXA();

    CURR_SCENE = newScene;
    SCENEs[CURR_SCENE]->init(ctx);
}