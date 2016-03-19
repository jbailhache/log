integer listen_handle;

default
{
    state_entry() 
    {
        llSay(0, "Hello, Avatar!");
        listen_handle = llListen(1234, "", NULL_KEY, "");

    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
    
    listen( integer channel, string name, key id, string message )    
    {
        llOwnerSay ("listen: Message recu : <" + message + ">");
    } 
 }