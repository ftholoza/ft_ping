#include "ft_ping.h"

int main(int argc, char **argv)
{
    t_ping ping;

    if (init_ping(&ping, argc, argv))
        return (1);

    ping_loop(&ping);

    destroy_ping(&ping);

    return (0);
}


