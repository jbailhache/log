
string mon_nom;
string dest;
string prenom = "";
string nom = "";
integer mode=0;
float hauteur=10.0;

init() 
    {
        mon_nom = llGetObjectName();
        llListen( 0, "", NULL_KEY, "");
        llSensorRepeat ("", "", AGENT, 30.0, PI, 10.0);  
        llOwnerSay( "A tes ordres");
    }  
    
default
{
    
    state_entry ()
    {
        init();
    }

    on_rez (integer x)
    {
        init();
    }
    
    listen( integer channel, string name, key id, string message ) 
    {
        llOwnerSay (name + " : " + message);
        if (id == llGetOwner())
        {
        // llSay (0, "Message recu : <" + message + ">");
        // separate the input into blank-delmited tokens.
        list parsed = llParseString2List( message, [ " " ], [] );

        string dest = llList2String (parsed, 0);
        llOwnerSay ("dest=<"+dest+"> mon_nom=<"+mon_nom+">");
        if (dest == mon_nom)
        {
         llOwnerSay ("Oui maitre");

         // get the first part--the "command".
         string command = llList2String( parsed, 1 );
         
         vector position = llGetPos();

         if( command == "suis" )
         {
          prenom = llList2String (parsed, 2);
          nom = llList2String (parsed, 3);
          llOwnerSay ("Je suis " + prenom + " " + nom);
         }
         else if (command == "arrete")
         {
          prenom = "";
          nom = "";
         }
         else if (command == "flotte")
          mode=0;
         else if (command == "tombe")
          mode=1;
         else if (command == "bouscule")
          mode=2;
         else if (command == "hauteur")
          hauteur = (float) llList2String(parsed,2);
        }
       }
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