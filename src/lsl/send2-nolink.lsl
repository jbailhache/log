default
{
    state_entry()
    {
        llSay(0, "Hello, Avatar!");
        llSay (1234, "Test message");
    }

    touch_start(integer total_number)
    {
        llSay(0, "Touched.");
    }
}