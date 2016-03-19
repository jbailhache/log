default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");
        state test;
    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
}

state test
{
    state_entry()
    {
        llSay (0, "Test");
}
}

