#ifndef VML_MKB2_MKB2_BALL_H
#define VML_MKB2_MKB2_BALL_H

#include "mkb2/gamecube.h"
#include "mkb2/common.h"
#include "mkb2/ape.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Ball {
    Status status; /* Actually called just "STAT" in the debug menu */
    u8 field_0x1;
    char monkey_count; /* Life counter - is actually signed? */
    u8 g_physics_state_flags; /* Something to do with physics state (Crashes the game if set wrong) (8 bit bitmask) */
    Vec3f pos;
    Vec3f prev_pos;
    Vec3f vel; /* Velocity/speed */
    short g_some_rot;
    short g_some_rot2;
    short g_some_rot3;
    char g_not_padding; /* Used to be called padding, but I saw it written to */
    u8 field_0x2f;
    float g_some_rot4;
    float g_some_rot5;
    float x_angle;
    float x_pos_copy;
    float g_some_rot6;
    float g_some_rot7;
    float y_angle;
    float y_pos_copy;
    float g_some_rot8;
    float g_some_rot9;
    float z_angle;
    float z_pos_copy;
    short g_some_rot10;
    short g_some_rot11;
    short g_some_rot12;
    short padding2;
    float ball_size;
    float acceleration;
    float restitution;
    float visual_scale;
    int banana_count;
    int score;
    int level_stopwatch;
    u8 field_0x84;
    u8 field_0x85;
    u8 field_0x86;
    u8 field_0x87;
    u8 field_0x88;
    u8 field_0x89;
    u8 field_0x8a;
    u8 field_0x8b;
    u8 field_0x8c;
    u8 field_0x8d;
    u8 field_0x8e;
    u8 field_0x8f;
    u8 field_0x90;
    u8 field_0x91;
    u8 field_0x92;
    u8 field_0x93;
    u8 field_0x94;
    u8 field_0x95;
    u8 field_0x96;
    u8 field_0x97;
    u32 g_some_bitfield;
    u32 g_some_flags; /* Some more flags related to ball state? The lowest-order bit may represent "is ball touching the ground" and I believe if affects the physics */
    u8 field_0xa0;
    u8 field_0xa1;
    u8 field_0xa2;
    u8 field_0xa3;
    u8 field_0xa4;
    u8 field_0xa5;
    u8 field_0xa6;
    u8 field_0xa7;
    u8 field_0xa8;
    u8 field_0xa9;
    u8 field_0xaa;
    u8 field_0xab;
    u8 field_0xac;
    u8 field_0xad;
    u8 field_0xae;
    u8 field_0xaf;
    u8 field_0xb0;
    u8 field_0xb1;
    u8 field_0xb2;
    u8 field_0xb3;
    u8 field_0xb4;
    u8 field_0xb5;
    u8 field_0xb6;
    u8 field_0xb7;
    u8 field_0xb8;
    u8 field_0xb9;
    u8 field_0xba;
    u8 field_0xbb;
    u8 field_0xbc;
    u8 field_0xbd;
    u8 field_0xbe;
    u8 field_0xbf;
    u8 field_0xc0;
    u8 field_0xc1;
    u8 field_0xc2;
    u8 field_0xc3;
    u8 field_0xc4;
    u8 field_0xc5;
    u8 field_0xc6;
    u8 field_0xc7;
    u8 field_0xc8;
    u8 field_0xc9;
    u8 field_0xca;
    u8 field_0xcb;
    u8 field_0xcc;
    u8 field_0xcd;
    u8 field_0xce;
    u8 field_0xcf;
    u8 field_0xd0;
    u8 field_0xd1;
    u8 field_0xd2;
    u8 field_0xd3;
    u8 field_0xd4;
    u8 field_0xd5;
    u8 field_0xd6;
    u8 field_0xd7;
    u8 field_0xd8;
    u8 field_0xd9;
    u8 field_0xda;
    u8 field_0xdb;
    u8 field_0xdc;
    u8 field_0xdd;
    u8 field_0xde;
    u8 field_0xdf;
    u8 field_0xe0;
    u8 field_0xe1;
    u8 field_0xe2;
    u8 field_0xe3;
    u8 field_0xe4;
    u8 field_0xe5;
    u8 field_0xe6;
    u8 field_0xe7;
    u8 field_0xe8;
    u8 field_0xe9;
    u8 field_0xea;
    u8 field_0xeb;
    u8 field_0xec;
    u8 field_0xed;
    u8 field_0xee;
    u8 field_0xef;
    u8 field_0xf0;
    u8 field_0xf1;
    u8 field_0xf2;
    u8 field_0xf3;
    u8 field_0xf4;
    u8 field_0xf5;
    u8 field_0xf6;
    u8 field_0xf7;
    u8 field_0xf8;
    u8 field_0xf9;
    u8 field_0xfa;
    u8 field_0xfb;
    u8 field_0xfc;
    u8 field_0xfd;
    u8 field_0xfe;
    u8 field_0xff;
    u8 field_0x100;
    u8 field_0x101;
    u8 field_0x102;
    u8 field_0x103;
    Ape *ape;
    u8 field_0x108;
    u8 field_0x109;
    u8 field_0x10a;
    u8 field_0x10b;
    u8 field_0x10c;
    u8 field_0x10d;
    u8 field_0x10e;
    u8 field_0x10f;
    u8 field_0x110;
    u8 field_0x111;
    u8 field_0x112;
    u8 field_0x113;
    u8 field_0x114;
    u8 field_0x115;
    u8 field_0x116;
    u8 field_0x117;
    u8 field_0x118;
    u8 field_0x119;
    u8 field_0x11a;
    u8 field_0x11b;
    u8 field_0x11c;
    u8 field_0x11d;
    u8 field_0x11e;
    u8 field_0x11f;
    u8 field_0x120;
    u8 field_0x121;
    u8 field_0x122;
    u8 field_0x123;
    u8 field_0x124;
    u8 field_0x125;
    u8 field_0x126;
    u8 field_0x127;
    u8 field_0x128;
    u8 field_0x129;
    u8 field_0x12a;
    u8 field_0x12b;
    u8 field_0x12c;
    u8 field_0x12d;
    u8 field_0x12e;
    u8 field_0x12f;
    u8 field_0x130;
    u8 field_0x131;
    u8 field_0x132;
    u8 field_0x133;
    u8 field_0x134;
    u8 field_0x135;
    s16 g_something_timer; /* Created by retype action */
    u8 field_0x138;
    u8 field_0x139;
    u8 field_0x13a;
    u8 field_0x13b;
    u8 field_0x13c;
    u8 field_0x13d;
    u8 field_0x13e;
    u8 field_0x13f;
    u8 field_0x140;
    u8 field_0x141;
    u8 field_0x142;
    u8 field_0x143;
    u8 field_0x144;
    u8 field_0x145;
    u8 field_0x146;
    u8 field_0x147;
    u8 field_0x148;
    u8 field_0x149;
    u8 field_0x14a;
    u8 field_0x14b;
    u8 field_0x14c;
    u8 field_0x14d;
    u8 field_0x14e;
    u8 field_0x14f;
    u8 field_0x150;
    u8 field_0x151;
    u8 field_0x152;
    u8 field_0x153;
    u8 field_0x154;
    u8 field_0x155;
    u8 field_0x156;
    u8 field_0x157;
    u8 field_0x158;
    u8 field_0x159;
    u8 field_0x15a;
    u8 field_0x15b;
    Vec3f g_some_pos1;
    u8 field_0x168;
    u8 field_0x169;
    u8 field_0x16a;
    u8 field_0x16b;
    u8 field_0x16c;
    u8 field_0x16d;
    u8 field_0x16e;
    u8 field_0x16f;
    u8 field_0x170;
    u8 field_0x171;
    u8 field_0x172;
    u8 field_0x173;
    u8 field_0x174;
    u8 field_0x175;
    u8 field_0x176;
    u8 field_0x177;
    u8 field_0x178;
    u8 field_0x179;
    u8 field_0x17a;
    u8 field_0x17b;
    u8 field_0x17c;
    u8 field_0x17d;
    u8 field_0x17e;
    u8 field_0x17f;
    u8 field_0x180;
    u8 field_0x181;
    u8 field_0x182;
    u8 field_0x183;
    u8 field_0x184;
    u8 field_0x185;
    u8 field_0x186;
    u8 field_0x187;
    u8 field_0x188;
    u8 field_0x189;
    u8 field_0x18a;
    u8 field_0x18b;
    u8 field_0x18c;
    u8 field_0x18d;
    u8 field_0x18e;
    u8 field_0x18f;
    u8 field_0x190;
    u8 field_0x191;
    u8 field_0x192;
    u8 field_0x193;
    u8 field_0x194;
    u8 field_0x195;
    u8 field_0x196;
    u8 field_0x197;
    u8 field_0x198;
    u8 field_0x199;
    u8 field_0x19a;
    u8 field_0x19b;
    u8 field_0x19c;
    u8 field_0x19d;
    u8 field_0x19e;
    u8 field_0x19f;
    u8 field_0x1a0;
    u8 field_0x1a1;
    u8 field_0x1a2;
    u8 field_0x1a3;
    u8 field_0x1a4;
    u8 field_0x1a5;
    u8 field_0x1a6;
    u8 field_0x1a7;
    u8 field_0x1a8;
    u8 field_0x1a9;
    u8 field_0x1aa;
    u8 field_0x1ab;
    u8 field_0x1ac;
    u8 field_0x1ad;
    u8 field_0x1ae;
    u8 field_0x1af;
} Ball;

extern Ball balls[8];
extern Ball *current_ball;

#ifdef __cplusplus
}
#endif

#endif

