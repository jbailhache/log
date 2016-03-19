
string prenom = "";
string nom = "";
float imp = 100.0;

default 
{
 state_entry ()
 {
  llListen( 0, "", llGetOwner(), "");
  llSensorRepeat ("", "", AGENT, 100.0, PI, 10.0);
  llOwnerSay("A tes ordres");
 }
 listen( integer channel, string name, key id, string message ) 
 {
  list parsed = llParseString2List( message, [ " " ], [] );
  string dest = llList2String (parsed, 0);
  if (dest == llGetObjectName())
  {
   llOwnerSay ("Oui maitre");
   string command = llList2String( parsed, 1);
   if (command == "ejecte")
   {
    prenom = llList2String (parsed, 2);
    nom = llList2String (parsed, 3);
    imp = (float) llList2String (parsed, 4);
    llOwnerSay ("J'ejecte " + prenom + " " + nom);
   }
  }
 }
 sensor (integer n)
 {
  integer i;
  for (i=0; i<n; i++)
  {
    if (llDetectedName(i) == prenom + " " + nom)
    {
     llPushObject (llDetectedKey(i), <0, 0, imp>, ZERO_VECTOR, FALSE);
    }
  }
 }
}
