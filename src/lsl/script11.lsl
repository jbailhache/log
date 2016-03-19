default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");
        state test;
    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
}

state test
{
    state_entry()
    {
        llSay (0, "Test");
        llListen (0, "", llGetOwner(), "");
        llSensorRepeat ("", "", AGENT, 20.0, PI, 10.0);
        llSetStatus (STATUS_PHYSICS, TRUE);
    
    }
    
    listen (integer channel, string name, key id, string message)
    {
        llSay (0, "Tu dis " + message + " ?");
    }

    sensor (integer detected)
    {
        integer i;
        llApplyImpulse (<0,0,5>, FALSE);
        string noms;
        noms="";
        for (i=0; i<detected; i++)
        {
            // llSay (0, "Je sens la presence de " + llDetectedName(i));
            noms=noms+llDetectedName(i)+"; ";
            if (llDetectedOwner(i) == llGetOwner())
            {
                llSetStatus (STATUS_PHYSICS, FALSE);
                llSetPos (llDetectedPos(i) + <0.0, 0.0, 3.0>);
                llSetStatus (STATUS_PHYSICS, TRUE);
            }
        }
        // llSay (0, "Salut "  + llDetectedName(0));
        llSay(0,noms);
        // llSetStatus (STATUS_PHYSICS, FALSE);
        // llSetPos (llDetectedPos(1)+<0.0, 1.0, 3.0>);
        // llSetStatus (STATUS_PHYSICS, TRUE);
    } 
}