default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");
        llSetText("look at me blue", <0.0, 0.0, 1.0>, 1.0);
        llOwnerSay("OK");
        llSetColor(<0.0, 1.0, 0.0>, ALL_SIDES); // darken
        
        llSetAlpha(0.7, ALL_SIDES); // make translucent

        llSetStatus(STATUS_PHYSICS, FALSE); llSleep(0.1);
        llSetPos(llGetPos() + <0.0, 0.0, 2.1>); // teleport up the Z axis
        llSetPos(llGetPos() + <0.0, 0.0, -2.1>); // teleport back down the Z axis
        llSetLocalRot(llRotBetween(<1, 0, 0>, llGetSunDirection())); // turn the East face to the Sun
        llSetLocalRot(llEuler2Rot(ZERO_VECTOR)); // turn the East face to the East
        llSetStatus(STATUS_PHYSICS, TRUE); llSleep(0.1);
        llSetBuoyancy(0.9); // bounce well, without floating
        llApplyImpulse(<0.0, 0.0, 1.0>, TRUE); // advance along the Z axis
        llApplyRotationalImpulse(<0.0, 0.0, 3.0>, TRUE); // yaw about the Z axis
        llSetStatus(STATUS_PHYSICS, FALSE); llSetStatus(STATUS_PHYSICS, TRUE); // zero rot inertia

         llOwnerSay( (string) llGetAgentSize(llGetLinkKey(llGetNumberOfPrims())) ); // often not ZERO_VECTOR while avatar sits
        llOwnerSay( (string) llKey2Name(llGetLinkKey(llGetNumberOfPrims())) ); // often the name of the sitting avatar
        llOwnerSay(llList2CSV( [ZERO_VECTOR, FALSE, TRUE, STATUS_PHYSICS, PI] )); // some named code 
llDialog(llGetOwner(), "A clarifying demo?", ["No", "Yes"], 7); // chat some Q & A
        llDialog(llGetOwner(), "Choose an arc:", ["PI_BY_TWO", "PI", "TWO_PI"], 7); // chat some Q & A

        
    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
}