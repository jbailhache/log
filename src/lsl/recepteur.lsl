
integer listen_handle;
integer n;

default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");
        llListen( 0, "", llGetOwner(), "");
    }
    
    listen( integer channel, string name, key id, string message ) 
    {
        llOwnerSay ("canal " + (string)channel + " : " + name + " : " + message);
        // separate the input into blank-delmited tokens.
        list parsed = llParseString2List( message, [ " " ], [] );

        // get the first part--the "command".
        string command = llList2String( parsed, 0 );
        
        if (command == "!canal")
        {
            n = (integer) llList2String (parsed, 1);
            listen_handle = llListen(n, "", NULL_KEY, "");
 
        }
 
    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
}