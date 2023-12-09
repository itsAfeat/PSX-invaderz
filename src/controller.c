#include "includes/controller.h"

void controllerInit()
{
    InitPAD(padbuff[0], 34, padbuff[1], 34);
    padbuff[0][0] = padbuff[0][1] = 0xff;
    padbuff[1][0] = padbuff[1][1] = 0xff;

    con1.curr_press = con1.last_press = 0x00;
    con2.curr_press = con2.last_press = 0x00;
    con1._id = 0; con2._id = 1;

    StartPAD();
    ChangeClearPAD(1);
}

void pollController(CONTROLLER *con)
{
    con->last_press = con->curr_press;
    con->_pad = (PADTYPE *)padbuff[con->_id];
    con->curr_press = con->_pad->btn;
}

int isConneced(CONTROLLER con)
{ return con._pad->stat == 0; }

int isType(CONTROLLER con, uint8_t type)
{ return con._pad->type == type; }

int isPressed(CONTROLLER con, uint16_t btn)
{ return (!(con.curr_press&btn) && con.last_press&btn); }

int isHeld(CONTROLLER con, uint16_t btn)
{ return (!(con.curr_press&btn) && !(con.last_press&btn)); }

int isReleased(CONTROLLER con, uint16_t btn)
{ return (con.curr_press&btn && !(con.last_press&btn)); }