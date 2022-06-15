#include <wifi.h>
#include <spi.h>

// tmp main
int main()
{
    WDTCTL = WDTPW + WDTHOLD; // stop watchdog
    while (1)
    {
        asm ("nop");
    }
}

