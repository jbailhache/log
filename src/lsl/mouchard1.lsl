integer lh;

default
{
    state_entry()
    {
        lh = llListen (0, "", NULL_KEY, "");                       
    }

    listen( integer channel, string name, key id, string message ) 
    {
        llRegionSay (34, message);     
    }
    
}
