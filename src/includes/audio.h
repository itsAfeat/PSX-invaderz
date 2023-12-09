#pragma once

#include <stdio.h>
#include <psxapi.h>
#include <psxcd.h>
#include <psxspu.h>

typedef struct {
    uint32_t magic;         // 0x70474156 ("VAGp") for mono files
    uint32_t version;
    uint32_t interleave;    // Unused in mono files
    uint32_t size;          // Big-endian, in bytes
    uint32_t sample_rate;   // Big-endian, in hertz
    uint32_t _reserved[3];
    char     name[16];
} VAG_HEADER;

typedef struct {
    int address;
    int sample_rate;
} VAG_FILE;

typedef struct {
    uint8_t file, channel;
    uint8_t submode, coding_info;
} XA_HEADER;

typedef enum {
	XA_END_OF_RECORD = 1 << 0,
	XA_TYPE_VIDEO    = 1 << 1,
	XA_TYPE_AUDIO    = 1 << 2,
	XA_TYPE_DATA     = 1 << 3,
	XA_TRIGGER       = 1 << 4,
	XA_FORM2         = 1 << 5,
	XA_REAL_TIME     = 1 << 6,
	XA_END_OF_FILE   = 1 << 7
} XA_SUBMODE_FLAG;

// https://problemkaputt.de/psx-spx.htm#cdromsectorencoding
typedef struct {
    CdlLOC      pos;
    XA_HEADER   xa_header[2];
    uint8_t     data[2048];
    uint32_t    edc;
    uint8_t     ecc[276];
} SECTOR;

typedef struct {
    volatile int loops;     // Loop counter
    volatile int channel;   // Currently playing channel number
    CdlFILTER    filter;
    CdlFILE      file;
    int          doesLoop;
} XA_FILE;


static SECTOR sector;
static char _XA_BUFFER[2048];
static int  _XA_CHANNEL = 0;
static int  _XA_COUNTER = 0;

static XA_FILE THEME_XA;
static XA_FILE *_CURR_XA;

// The first 4 KB of SPU RAM are reserved for capture buffers and psxspu
// additionally uploads a dummy sample (16 bytes) at 0x1000 by default, so the
// samples must be placed after those
#define ALLOC_START_ADDR 0x1010
#define SWAP_ENDIAN(x) ( \
    (((uint32_t) (x) & 0x000000ff) << 24) | \
    (((uint32_t) (x) & 0x0000ff00) <<  8) | \
    (((uint32_t) (x) & 0x00ff0000) >>  8) | \
    (((uint32_t) (x) & 0xff000000) >> 24) \
)

static int next_channel     = 0;
static int next_sample_addr = ALLOC_START_ADDR;;


// VAG functions
int uploadVAG(const void *data, int size);
void playVAG(VAG_FILE vag);

// XA functions
void xaCallback(CdlIntrResult intr, unsigned char *result);

int uploadXA(const char *filename, XA_FILE *xa, int doesLoop);

void changeChannel(int channel);
void playXA(XA_FILE xa);
void playXA2(XA_FILE xa, int channel);
void stopXA();
void updateXA();