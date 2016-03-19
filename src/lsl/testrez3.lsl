
integer ng = 3;

replic(integer x)
{
    llOwnerSay ("x=" + (string)x + " param=" + (string)llGetStartParameter()); 
    llSleep(1);
    if (x < ng)   
    llRezObject ("cellule",
     llGetPos()+<llFrand(0.3),llFrand(0.3),llFrand(0.3)>,
     <0.0, 0.0, 0.0>,
     llEuler2Rot(<0,0,0>),
     llGetStartParameter()+1);
}

default
{
    state_entry()
    {
        llOwnerSay("Hello, Avatar!");
        replic(0);
    }
    
    on_rez (integer x)
    {
        replic(x);
    }
    
    object_rez(key id)
    {
        llGiveInventory(id,"cellule");
    }

    
}
