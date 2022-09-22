#ifndef VML_MKB2_MKB2_CAMERA_H
#define VML_MKB2_MKB2_CAMERA_H

#include "mkb2/gamecube.h"
#include "mkb2/common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Camera {
    struct Vec3f pos;
    struct Vec3f g_some_vec1;
    struct Vec3s rot;
    u8 g_some_mode; /* One byte representing camera mode (1 (dec) = SMB 1 style camera, 75 (dec) = SMB 2 style camera, and there's a lot more (everything from 1 to about 105 - a few are duplicates. Each value  corresponds to a different function in the camera function table */
    u8 field_0x1f;
    u8 field_0x20;
    u8 field_0x21;
    u8 field_0x22;
    u8 field_0x23;
    u16 g_some_bitflags;
    u8 field_0x26;
    u8 field_0x27;
    u32 field_0x28;
    u32 field_0x2c;
    u16 field_0x30;
    u16 field_0x32;
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
    u8 field_0x3a;
    u8 field_0x3b;
    u8 field_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 field_0x40;
    u8 field_0x41;
    u8 field_0x42;
    u8 field_0x43;
    u8 field_0x44;
    u8 field_0x45;
    u8 field_0x46;
    u8 field_0x47;
    u32 field_0x48;
    u32 field_0x4c;
    float field_0x50;
    float field_0x54;
    u16 g_some_counter1;
    u8 field_0x5a;
    u8 field_0x5b;
    u8 field_0x5c;
    u8 field_0x5d;
    u8 field_0x5e;
    u8 field_0x5f;
    u8 field_0x60;
    u8 field_0x61;
    u8 field_0x62;
    u8 field_0x63;
    u8 field_0x64;
    u8 field_0x65;
    u8 field_0x66;
    u8 field_0x67;
    u8 field_0x68;
    u8 field_0x69;
    u8 field_0x6a;
    u8 field_0x6b;
    u8 field_0x6c;
    u8 field_0x6d;
    u8 field_0x6e;
    u8 field_0x6f;
    u8 field_0x70;
    u8 field_0x71;
    u8 field_0x72;
    u8 field_0x73;
    u8 field_0x74;
    u8 field_0x75;
    u8 field_0x76;
    u8 field_0x77;
    u8 field_0x78;
    u8 field_0x79;
    u8 field_0x7a;
    u8 field_0x7b;
    u8 field_0x7c;
    u8 field_0x7d;
    u8 field_0x7e;
    u8 field_0x7f;
    u8 field_0x80;
    u8 field_0x81;
    u8 field_0x82;
    u8 field_0x83;
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
    u8 field_0x98;
    u8 field_0x99;
    u8 field_0x9a;
    u8 field_0x9b;
    u8 field_0x9c;
    u8 field_0x9d;
    u8 field_0x9e;
    u8 field_0x9f;
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
    Vec3f g_some_vec2;
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
    u8 field_0x104;
    u8 field_0x105;
    u8 field_0x106;
    u8 field_0x107;
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
    u8 field_0x136;
    u8 field_0x137;
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
    Mtx g_some_mtx1;
    Mtx g_some_mtx2;
    Mtx g_some_mtx3;
    Mtx g_some_mtx4;
    s32 field_0x20c;
    u8 field_0x210;
    u8 field_0x211;
    u8 field_0x212;
    u8 field_0x213;
    u8 field_0x214;
    u8 field_0x215;
    u8 field_0x216;
    u8 field_0x217;
    u8 field_0x218;
    u8 field_0x219;
    u8 field_0x21a;
    u8 field_0x21b;
    u8 field_0x21c;
    u8 field_0x21d;
    u8 field_0x21e;
    u8 field_0x21f;
    u8 field_0x220;
    u8 field_0x221;
    u8 field_0x222;
    u8 field_0x223;
    u8 field_0x224;
    u8 field_0x225;
    u8 field_0x226;
    u8 field_0x227;
    u8 field_0x228;
    u8 field_0x229;
    u8 field_0x22a;
    u8 field_0x22b;
    u8 field_0x22c;
    u8 field_0x22d;
    u8 field_0x22e;
    u8 field_0x22f;
    u8 field_0x230;
    u8 field_0x231;
    u8 field_0x232;
    u8 field_0x233;
    u8 field_0x234;
    u8 field_0x235;
    u8 field_0x236;
    u8 field_0x237;
    u8 field_0x238;
    u8 field_0x239;
    u8 field_0x23a;
    u8 field_0x23b;
    u8 field_0x23c;
    u8 field_0x23d;
    u8 field_0x23e;
    u8 field_0x23f;
    u8 field_0x240;
    u8 field_0x241;
    u8 field_0x242;
    u8 field_0x243;
    u8 field_0x244;
    u8 field_0x245;
    u8 field_0x246;
    u8 field_0x247;
    u8 field_0x248;
    u8 field_0x249;
    u8 field_0x24a;
    u8 field_0x24b;
    u8 field_0x24c;
    u8 field_0x24d;
    u8 field_0x24e;
    u8 field_0x24f;
    u8 field_0x250;
    u8 field_0x251;
    u8 field_0x252;
    u8 field_0x253;
    u8 field_0x254;
    u8 field_0x255;
    u8 field_0x256;
    u8 field_0x257;
    u8 field_0x258;
    u8 field_0x259;
    u8 field_0x25a;
    u8 field_0x25b;
    u8 field_0x25c;
    u8 field_0x25d;
    u8 field_0x25e;
    u8 field_0x25f;
    u8 field_0x260;
    u8 field_0x261;
    u8 field_0x262;
    u8 field_0x263;
    u8 field_0x264;
    u8 field_0x265;
    u8 field_0x266;
    u8 field_0x267;
    u8 field_0x268;
    u8 field_0x269;
    u8 field_0x26a;
    u8 field_0x26b;
    u8 field_0x26c;
    u8 field_0x26d;
    u8 field_0x26e;
    u8 field_0x26f;
    u8 field_0x270;
    u8 field_0x271;
    u8 field_0x272;
    u8 field_0x273;
    u8 field_0x274;
    u8 field_0x275;
    u8 field_0x276;
    u8 field_0x277;
    u8 field_0x278;
    u8 field_0x279;
    u8 field_0x27a;
    u8 field_0x27b;
    u8 field_0x27c;
    u8 field_0x27d;
    u8 field_0x27e;
    u8 field_0x27f;
    u8 field_0x280;
    u8 field_0x281;
    u8 field_0x282;
    u8 field_0x283;
    u8 field_0x284;
    u8 field_0x285;
    u8 field_0x286;
    u8 field_0x287;
    u8 field_0x288;
    u8 field_0x289;
    u8 field_0x28a;
    u8 field_0x28b;
} Camera;

extern Camera cameras[5];
extern Camera *g_current_camera;

#ifdef __cplusplus
}
#endif

#endif
