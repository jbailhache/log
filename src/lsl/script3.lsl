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
        llSensorRepeat ("", "", AGENT, 20.0, PI, 1.0);
    }
    
    listen (integer channel, string name, key id, string message)
    {
        llSay (0, "Tu dis " + message + " ?");
    }

    sensor (integer detected)
    {
        integer i;
        string s;
        
        for (i=0; i<detected; i++)
        {
            
        llSay (0, "Je sens la presence de " + llDetectedName(i));
        }
    } 
}

