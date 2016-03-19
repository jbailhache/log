
default
{
    state_entry()
    {
        llOwnerSay("Hello, Avatar!");
    }

    touch_start(integer total_number)
    {
        llOwnerSay("Touched.");
       
       
        llParticleSystem( 
          [PSYS_PART_FLAGS,       PSYS_PART_WIND_MASK ,
           PSYS_SRC_PATTERN,      
           PSYS_SRC_PATTERN_EXPLODE,
           //PSYS_SRC_PATTERN_DROP | PSYS_SRC_PATTERN_ANGLE_CONE, 
           PSYS_PART_START_COLOR, <0.5, 0.6, 0.7>,
           PSYS_SRC_ANGLE_BEGIN, 0.5,
           PSYS_SRC_ANGLE_END, 1.0,
           PSYS_SRC_ACCEL, <0.0, 0.0, -4.0>,
           PSYS_PART_START_SCALE, <0.05, 0.5, 0.0>,
           PSYS_SRC_BURST_RADIUS, 0.3,
           PSYS_SRC_BURST_RATE, 0.01,
           PSYS_SRC_BURST_PART_COUNT, 3,
           PSYS_PART_START_ALPHA, 0.5
          ] );
         
        llSleep(10);
        llParticleSystem([]);

    }
}
