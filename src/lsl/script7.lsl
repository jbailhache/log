// This is a script designed to orbit its owner.
vector startPos;
vector curPos;

vector offset;             // offset from Agent
integer iteration;
float rotationRate;       // degrees of rotation per iteration
float sensorInterval;     // seconds between sensor scan.

default
{
    state_entry()
    {
        llOwnerSay( "Hello, Avatar! Touch to start orbiting." );
        llSetStatus( 1, FALSE );  // turn Physics off.
        offset = < 2, 2, 1 >;
        iteration = 0;
        rotationRate = .05;
        sensorInterval = 10;
    
        startPos = llGetPos();
        curPos = startPos;
        
        llSleep( .1 );

        key id = llGetOwner();
        llSensorRepeat( "", id, AGENT, 96, PI, sensorInterval );
    }

    sensor(integer total_number)
    {
        
        iteration++;

        if( iteration > 300 )
        {
           llResetScript();
        }

        integer i;
        
        for (i=0; i<total_number; i++)
        {
            llSay (0, "Je sens la presence de " + llDetectedName(i));
        }

        
        if( llDetectedOwner( 0 ) != llGetOwner() )  
        {        // the detected Agent is my owner.
           vector position = llDetectedPos(0);  // find Owner position.

           // calculate next object position relative both to the Owner's 
           // position and the current time interval counter.  That is, 
           // use the iteration counter to define a rotation, multiply 
           // the rotation by the constant offset to get a rotated offset 
           // vector, and add that rotated offset to the current position
           // to defne the new position. 
          
           float degreeRotation = llRound( rotationRate * iteration ) % 360;
           rotation Rotation = 
                llEuler2Rot( < 0, 0, degreeRotation * DEG_TO_RAD > );
           vector rotatedOffset = offset * Rotation;
           position = position + <1.0, 0.0, 2.0>;
           
           
           
           
          
           // change the location of the object and save the current (rotated) 
           // offset for use during the next iteration.
           llSetPos( position ); 
           llSetStatus (STATUS_PHYSICS, TRUE);
           llApplyImpulse (<0.0, 0.0, -1.0>, FALSE);
           offset = rotatedOffset; 
        }  
    }
}
