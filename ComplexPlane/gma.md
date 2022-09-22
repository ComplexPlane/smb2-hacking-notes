# GMA Texture Flags

0: UNK_BIT0
1: UNK_BIT1
2: REPEAT_U
3: MIRROR_U
4: REPEAT_V
5: MIRROR_V
6: UNK_BIT6
7: UNK_BIT7

8: ENABLE_MIPMAPS
9: UNK_BIT9
10: UNK_BIT10
11: G_NEAR
12: G_HEIGHTMAP_BLEND_GRAYSCALE
13: G_ALPHA_MASK
14: UNK_BIT14
15: UNK_BIT15

16: UNK_BIT16
17: ENABLE_UV_SCROLL
18: UNK_BIT18
... rest are unk

# Rendering

Nopping these three functions in the DOL (part of avdisp likely) seems to prevent all 3D stuff from rendering, besides effects
0x80069ae8
0x80069c00
0x80069d14