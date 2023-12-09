#include "includes/audio.h"

// For uploading VAG data from the disc to the SPU
int uploadVAG(const void *data, int size)
{
    // Round the size up to the nearest multiple of 64, as SPU DMA transfers
	// are done in 64-byte blocks
    int _addr = next_sample_addr;
    int _size = (size + 63) & 0xffffffc0;

    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    SpuSetTransferStartAddr(_addr);

    SpuWrite((const uint32_t *) data, _size);
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

    next_sample_addr = _addr + _size;
    return _addr;
}

// Play a VAG file that's been uploaded to the SPU
void playVAG(VAG_FILE vag)
{
    int ch = next_channel;

    // Make sure the channel is stopped
    SpuSetKey(0, 1 << ch);

    // Set the channel's sample rate and start address. Note that the SPU
	// expects the sample rate to be in 4.12 fixed point format (with
	// 1.0 = 44100 Hz) and the address in 8-byte units; psxspu.h provides the
	// getSPUSampleRate() and getSPUAddr() macros to convert values to these
	// units
    SPU_CH_FREQ(ch) = getSPUSampleRate(vag.sample_rate);
    SPU_CH_ADDR(ch) = getSPUAddr(vag.address);

	// Set the channel's volume and ADSR parameters (0x80ff and 0x1fee are
	// dummy values that disable the ADSR envelope entirely)
    SPU_CH_VOL_L(ch) = 0x3fff;
    SPU_CH_VOL_R(ch) = 0x3fff;
    SPU_CH_ADSR1(ch) = 0x00ff;
    SPU_CH_ADSR2(ch) = 0x0000;

    // Start the channel
    SpuSetKey(1, 1 << ch);

    next_channel = (ch + 1) % 24;
}

// Callback for detecting end of channel (hooked by CdReadyCallback)
void xaCallback(CdlIntrResult event, uint8_t *payload)
{
    if (_CURR_XA->doesLoop)
    {
        if (event != CdlDataReady)
        { return; }

        CdGetSector(&sector, sizeof(sector) / 4);

        if (
            !(sector.xa_header[0].submode & XA_TYPE_AUDIO) &&
            !(sector.xa_header[1].submode & XA_TYPE_AUDIO)
        ) {
            CdControlF(CdlReadS, &_CURR_XA->file.pos);
            _CURR_XA->loops++;
        }
    }
}

int uploadXA(const char *filename, XA_FILE *xa, int doesLoop)
{
    CdlFILE file;
    CdlFILTER filter;

    if (CdSearchFile(&file, filename))
    {
        int sec, i;

        sec = CdPosToInt(&file.pos);
		printf("XA located at sector %d size %d.\n", sec, file.size);

        xa->file = file;
        EnterCriticalSection();
        CdReadyCallback(&xaCallback);
        ExitCriticalSection();

        i = CdlModeSpeed|CdlModeRT|CdlModeSF;
        CdControl(CdlSetmode, &i, 0);
        
        xa->filter.file = 1;
        xa->doesLoop = doesLoop;

        return 0;
    }

    printf("Unable to locate %s\n", filename);
    return -1;
}

void changeChannel(int channel)
{
    _XA_CHANNEL = channel;
    _CURR_XA->filter.chan = _XA_CHANNEL;
    CdControl(CdlSetfilter, &_CURR_XA->filter, 0);
    _CURR_XA->channel = _XA_CHANNEL;
}

void playXA(XA_FILE xa)
{
    _CURR_XA = &xa;
    _CURR_XA->filter.chan = _XA_CHANNEL;
    CdControl(CdlSetfilter, &_CURR_XA->filter, 0);
    CdControl(CdlReadS, &_CURR_XA->file.pos, 0);
    _CURR_XA->channel = _XA_CHANNEL;
}

void playXA2(XA_FILE xa, int channel)
{
    _XA_CHANNEL = channel;
    _CURR_XA = &xa;
    _CURR_XA->filter.chan = _XA_CHANNEL;
    CdControl(CdlSetfilter, &_CURR_XA->filter, 0);
    CdControl(CdlReadS, &_CURR_XA->file.pos, 0);
    _CURR_XA->channel = _XA_CHANNEL;
}

void stopXA()
{ CdControl(CdlPause, 0, 0); }

void updateXA()
{
    _XA_COUNTER++;
    if (_XA_COUNTER%60 == 59)
    { CdControl(CdlNop, 0, 0); }
}