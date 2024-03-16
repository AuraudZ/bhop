#pragma once
#include <Windows.h>
#include <cstddef>

enum CommandButtons : int
{
    IN_ATTACK = 1 << 0,
    IN_JUMP = 1 << 1,
    IN_DUCK = 1 << 2,
    in_forward = 1 << 3,
    in_back = 1 << 4,
    in_sliding = 1 << 10,
    in_left = 1 << 9,
    in_right = 1 << 10,
    in_use = 1 << 5,
    in_cancel = 1 << 12,
    in_sprint = 1 << 15,
    in_mele = 1 << 18,
    in_zoom = 1 << 16,
    in_switch = 1 << 19,
    in_grenade = 1 << 20,
    in_reload = 1 << 12,

};

enum FLAGS
{
    FL_ONGROUND = (1 << 0),
    FL_DUCKING = (1 << 1),
    FL_ATCONTROLS = (1 << 6),

};
class Vector3 {
    float x,y, z;
};

class c_usercmd
{
public:
    int command_number; //0
    int tick_count; //4
    int command_time;//8
    float viewangles[3]; //12
    BYTE gap18[4]; //18
    Vector3 localViewAngles; // 0x1C (0xC)
    Vector3 attackangles; // 0x28 
    Vector3 move;
    int	  buttons;//64
    BYTE   impulse; //68
    short weaponselect;
    int m_weaponSubType; // 0x47 (0x4)
    unsigned char byte4C; // 0x4C (0x1)
    unsigned char pad2[3]; // 0x4D (0x3)
    int m_randomSeed; // 0x50 (0x4)
    short mousedx;
    short mousedy;
    unsigned char pad3[90]; // 0x56 (0x5A)
    double m_msTime; // 0xB0 (0x8)
    unsigned char pad4[0x90]; // 0xB8 (0x90)
};