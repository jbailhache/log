integer n;
string texte;
integer i;

    init()
    {
        llOwnerSay ("En service");
        llListen( 0, "", llGetOwner(), "");
    }

default
{

    state_entry()
    {
         init();
    }

    on_rez (integer x)
    {
        init();
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
            // texte = llList2String (parsed,2);
        texte = "";
        for (i=2; i<10; i++)
         texte = texte + llList2String (parsed, i) + " ";
        llOwnerSay ("Envoi canal " + (string)n + " : " + texte);
            llRegionSay (n, texte);             
        }
 
    }
    
}