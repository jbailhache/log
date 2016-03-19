
replic(integer x)
{
    llOwnerSay ("x=" + (string)x + " param=" + (string)llGetStartParameter());
    if(0)llRezObject ("cellule1",
     llGetPos()+<llFrand(0.1),llFrand(0.1),llFrand(0.1)>,
     <0.0, 0.0, 0.0>,
     llEuler2Rot(<0,0,0>),
     llGetStartParameter()-1);
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
    

    touch_start(integer total_number)
    {
        llOwnerSay("Touched.");
        llRezObject("cellule1",
     llGetPos()+<llFrand(1),llFrand(1),llFrand(1)>,
     <0.0, 0.0, 0.0>,
     llEuler2Rot(<0,0,0>),
     llGetStartParameter()-1);   
    }
}
