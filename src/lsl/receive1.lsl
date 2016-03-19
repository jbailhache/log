default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");

    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
    
    link_message (integer sender,integer num, string str, key id)
    {
        llOwnerSay ("link_message: Message recu : <" + str + ">");
    } 
 }