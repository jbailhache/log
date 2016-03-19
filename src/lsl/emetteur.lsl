integer n;
string texte;

default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");
        llListen( 0, "", llGetOwner(), "");
    }

    listen( integer channel, string name, key id, string message ) 
    {
        // llOwnerSay ("canal " + (string)channel + " : " + name + " : " + message);
        // separate the input into blank-delmited tokens.
        list parsed = llParseString2List( message, [ " " ], [] );

        // get the first part--the "command".
        string command = llList2String( parsed, 0 );
        
        if (command == "!envoi")
        {
            n = (integer) llList2String (parsed, 1);
            texte = llList2String (parsed,2);
            llSay (n, texte);             
        }
 
    }
    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
}