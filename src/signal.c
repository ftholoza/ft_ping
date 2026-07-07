#include "ft_ping.h"

volatile sig_atomic_t g_running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    g_running = 0;
}

void init_signal(void)
{
    signal(SIGINT, handle_sigint);
}