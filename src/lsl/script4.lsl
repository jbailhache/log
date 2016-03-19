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
        llSensorRepeat ("", "", AGENT, 100.0, PI, 10.0);
	llSetStatus (STATUS_PHYSICS, TRUE);
	llApplyImpulse (<0,0,1>, FALSE);
    }
    
    listen (integer channel, string name, key id, string message)
    {
        llSay (0, "Tu dis " + message + " ?");
    }

    sensor (integer detected)
    {
        integer i;
	integer j;
        string s;
        
        for (i=0; i<detected; i++)
        {
            llSay (0, "Je sens la presence de " + llDetectedName(i));
        }
    } 
}

