
string prenom = "";
string nom = "";
integer mode=0;
float hauteur=10.0;

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

        if( command == "!suis" )
        {
        prenom = llList2String (parsed, 1);
        nom = llList2String (parsed, 2);
        llSay (0, "Je suis " + prenom + " " + nom);
        }
    else if (command == "!arrete")
    {
        prenom = "";
        nom = "";
    }
    else if (command == "!flotte")
     mode=0;
    else if (command == "!tombe")
     mode=1;
    else if (command == "!bouscule")
     mode=2;
      else if (command == "!hauteur")
      hauteur = (float) llList2String(parsed,1);
      
}

    sensor (integer detected)
    {
    integer i;
    for (i=0; i<detected; i++)
    {
        // llSay (0, "Je sens la presence de " + llDetectedName(i));
        if (llDetectedName(i) == prenom + " " + nom)
        {
        // llSay (0, "Je vais le voir");
        llSetStatus (STATUS_PHYSICS, FALSE);
        if (mode==2)
        llSetPos (llDetectedPos(i));
      else
        llSetPos (llDetectedPos(i) + <0.0, 0.0, hauteur>);
        if (mode)
         llSetStatus (STATUS_PHYSICS, TRUE);
        }
    }
    }
}