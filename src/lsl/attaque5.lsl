
string prenom = "";
string nom = "";

default
{
    state_entry() 
    {
        llListen( 0, "", llGetOwner(), "");
        llSensorRepeat ("", "", AGENT, 30.0, PI, 10.0);
  
        llSay( 0, "A tes ordres");
    }  
    
    listen( integer channel, string name, key id, string message ) 
    {
        llSay (0, "Message recu : <" + message + ">");
        // separate the input into blank-delmited tokens.
        list parsed = llParseString2List( message, [ " " ], [] );

        // get the first part--the "command".
        string command = llList2String( parsed, 0 );
        
        vector position = llGetPos();

        if( command == "objet-vavoir" )
        {
        prenom = llList2String (parsed, 1);
        nom = llList2String (parsed, 2);
        llSay (0, "Je vais voir " + prenom + " " + nom);
        }
    else if (command == "objet-arrete")
    {
        prenom = "";
        nom = "";
    }
}

    sensor (integer detected)
    {
    integer i;
    for (i=0; i<detected; i++)
    {
        llSay (0, "Je sens la presence de " + llDetectedName(i));
        if (llDetectedName(i) == prenom + " " + nom)
        {
        llSay (0, "Je vais le voir");
        llSetStatus (STATUS_PHYSICS, FALSE);
        llSetPos (llDetectedPos(i) + <0.0, 0.0, 2.0>);
        llSetStatus (STATUS_PHYSICS, TRUE);
        }
    }
    }
}
