
integer ng = 4;
float d=0.3;
integer i;

replic (integer x)
{
    if (x < ng)
    {
           for (i=0; i<ng-x; i++) 
           {
            llSleep(1);
            llRezObject ("cellule",
                llGetPos()+<-d,-d,-d>+<llFrand(2*d),llFrand(2*d),llFrand(2*d)>,
                <0.0, 0.0, 0.0>,
                llEuler2Rot(<0,0,0>),
                llGetStartParameter()+1);
            }
    }
}

default
{
    on_rez (integer x)
    {
        if (x > 0)
            replic(x);
    }

    touch_start (integer x)
    {
        replic (0);
    }    
    
    object_rez(key id)
    {
        llGiveInventory(id,"cellule");
    }   
    
}
