#pragma once

#include <psxpad.h>
#include <psxapi.h>
#include <sys/types.h>

static u_char padbuff[2][34];

typedef struct {
    PADTYPE *_pad;
    int _id;
    int curr_press;
    int last_press;
} CONTROLLER;

static CONTROLLER con1, con2;

// Basic setup and polling functions
void controllerInit();
void pollController(CONTROLLER *con);

// All the is- functions (checking presses, connections etc)
int isConneced(CONTROLLER con);
int isType(CONTROLLER con, uint8_t type);
int isPressed(CONTROLLER con, uint16_t btn);
int isHeld(CONTROLLER con, uint16_t btn);
int isReleased(CONTROLLER con, uint16_t btn);