
string prenom = "";
string nom = "";

default
{
    state_entry() 
    {
        llListen( 0, "", llGetOwner(), "");
	  llSensorRepeat ("", "", AGENT, 20.0, PI, 10.0);
  
        llSay( 0, "A tes ordres");
    }  
    
    listen( integer channel, string name, key id, string message ) 
    {
        // separate the input into blank-delmited tokens.
        list parsed = llParseString2List( message, [ " " ], [] );

        // get the first part--the "command".
        string command = llList2String( parsed, 0 );
        
        vector position = llGetPos();

        if( command == "objet-vavoir" )
        {
	    prenom = llList2String (parsed, 1);
	    nom = llList2String (parsed, 2);
        }
	else if (command == "objet-arrete")
	{
	    prenom = "";
	    nom = "";
        }
    }

    sensor (integer detected)
    {
	for (i=0; i<detected; i++)
	{
	    if (llDetectedName(i) == prenom + " " + nom)
	    {
		llSetStatus (STATUS_PHYSICS, TRUE);
		llSetPos (llDetectedPos(i) + <0.0, 0.0, 2.0>);
	    }
	}
    }
}
