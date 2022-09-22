First ball struct starts at:
805BC9A0

Byte at 0x3:
Set to 3: insta-goal
Set to 5: ball takes off like post-goal, but camera follows

# Yoshimaster

//Structs
typedef struct {
    u8 unk0;                //00
    u8 unk1;                //01
    u8 lives;                //02
    u8 ballSubmode;            //03
    Vec position;            //04
    Vec prevPosition;        //10
    Vec velocity;            //1C
    u16 rotation[3];        //28
    Mtx transform;            //30
    u16 angularVelocity[3];    //60 TODO: verify
    float unk68;            //68
    float gravity;            //6C
    float restitution;        //70
    float scale;            //74
    int bananas;            //78
    int score;                //7C
    int unkTimer;            //80
    float unk84;            //84
    float unk88;            //88
    float unk8C;            //8C
    u16 unk90;                //90
    u16 unk92;                //92
    int unk94;                //94
} ball_t;
typedef struct {
    
    ball_t balls[4];        //40

} ball_info_t;
The ballSubmode determines which one of the ball subs gets run (it's an index into a function table)

Also sizeof(ball_t)==0x1A4 in SMB1
