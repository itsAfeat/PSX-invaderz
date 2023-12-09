#pragma once

#include <sys/types.h>
#include <stdlib.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <psxspu.h>
#include <psxcd.h>
#include <stdio.h>

#define OT_LENGTH       16
#define BUFFER_LENGTH   8192
#define DEBUG


typedef struct
{
    DISPENV disp_env;
    DRAWENV draw_env;

    uint32_t ot[OT_LENGTH];
    uint8_t  buffer[BUFFER_LENGTH];
} RenderBuffer;


typedef struct
{
    RenderBuffer buffers[2];
    uint8_t      *next_packet;
    int          active_buffer;
} RenderContext;


// Initialization / setup functions
void init();
void setupContext(RenderContext *ctx, int w, int h, int x, int y, CVECTOR rgb);

// CD functions
char *loadFile(const char *filename);

// General purpose functions
int clamp(int i, int min, int max);
int isBetween(int n, int min, int max);
CVECTOR rgbToCVector(int r, int g, int b);
int getStrWidth(const char* string);
int getStrHeight(const char* string);