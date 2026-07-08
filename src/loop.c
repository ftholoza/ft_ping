#include "ft_ping.h"

static double compute_rtt(struct timeval start, struct timeval end)
{
    return ((end.tv_sec - start.tv_sec) * 1000.0
        + (end.tv_usec - start.tv_usec) / 1000.0);
}

static void print_reply(t_ping *ping, double rtt)
{
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms",
        ping->reply.bytes,
        ping->reply.ip,
        ping->reply.sequence,
        ping->reply.ttl,
        rtt);

    printf("\n");
}

int ping_loop(t_ping *ping)
{
    int     ret;
    double  rtt;

    while (g_running)
    {
        build_packet(ping);

        if (send_ping(ping) < 0)
            return (-1);

        ping->transmitted++;

        ret = receive_ping(ping);
        if (ret < 0)
            return (-1);

        if (ret == 0)
        {
            //store the round trip time
            rtt = compute_rtt(ping->start, ping->end);
            update_stats(ping, rtt);
            print_reply(ping, rtt);
        }

        ping->sequence++;
        sleep(1);
    }

    print_stats(ping);
    return (0);
}