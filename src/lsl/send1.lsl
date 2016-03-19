integer listen_handle;

default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");
        llOwnerSay ("Hello, my owner!");
        listen_handle = llListen(0, "", llGetOwner(), "");

    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
    
    listen( integer channel, string name, key id, string message ) 
    {
        llOwnerSay ("listen : message recu : <" + message + ">"); 
        llSay (0, "listen : Message recu : <" + message + ">");  
        llMessageLinked (LINK_ALL_OTHERS, 0, message, id);    
    }

}