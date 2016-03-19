integer num = 3;
float dist = 21.0;
string texture;
integer random;
list TextureList = [                         // your UUID texture list
     "33d2a02e-f5a4-a5fa-4b6e-93acbdae770b", // ? img
     "4f0a8f2d-0c1c-1156-4f5e-36e9aa05a7d7"  // + img
     ];

startgoo()
{
    random = llFloor(llFrand((float)llGetListLength(TextureList)));
    texture = llList2String(TextureList, random);
    llParticleSystem([
      PSYS_PART_FLAGS , 0
    | PSYS_PART_BOUNCE_MASK
    | PSYS_PART_FOLLOW_VELOCITY_MASK
    | PSYS_PART_EMISSIVE_MASK,
      PSYS_SRC_PATTERN,          PSYS_SRC_PATTERN_EXPLODE,
      PSYS_SRC_TEXTURE,          texture,
      PSYS_SRC_MAX_AGE,          60.0,
      PSYS_PART_MAX_AGE,         10.0,
      PSYS_SRC_BURST_RATE,       30.0, //opt.
      PSYS_SRC_BURST_PART_COUNT, num,
      PSYS_SRC_BURST_RADIUS,     dist,
      PSYS_SRC_BURST_SPEED_MIN,  3.0,
      PSYS_SRC_BURST_SPEED_MAX,  5.0,
      PSYS_SRC_ACCEL,            <0.0,0.0,-9.0>,
      PSYS_PART_START_SCALE,     <3.0,3.0,3.0>,
      PSYS_SRC_OMEGA,            <0.0,0.0,0.0>
      ]);
    llSleep(0.3);
    llParticleSystem([]);
}

default
{
    on_rez(integer param)
    {
     llResetScript();
    }

    touch_start(integer total_number)
    {
     if (llDetectedKey(0) == llGetOwner()) startgoo();
    }
}

