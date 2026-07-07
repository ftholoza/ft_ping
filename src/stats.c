#include "ft_ping.h"
#include <math.h>
#include <stdio.h>

void update_stats(t_ping *ping, double rtt)
{
    ping->received++;
    ping->total_rtt += rtt;
    ping->total_rtt_sq += rtt * rtt;

    if (ping->received == 1 || rtt < ping->min_rtt)
        ping->min_rtt = rtt;

    if (ping->received == 1 || rtt > ping->max_rtt)
        ping->max_rtt = rtt;
}

void print_stats(t_ping *ping)
{
    double loss;
    double avg;
    double variance;
    double stddev;

    if (ping->transmitted == 0)
        loss = 0.0;
    else
        loss = ((ping->transmitted - ping->received) * 100.0)
            / ping->transmitted;

    printf("\n--- %s ping statistics ---\n", ping->target);
    printf("%d packets transmitted, %d received, %.0f%% packet loss\n",
           ping->transmitted,
           ping->received,
           loss);

    if (ping->received > 0)
    {
        avg = ping->total_rtt / ping->received;
        variance = (ping->total_rtt_sq / ping->received) - (avg * avg);

        /* Protection contre les erreurs d'arrondi */
        if (variance < 0)
            variance = 0;

        stddev = sqrt(variance);

        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
               ping->min_rtt,
               avg,
               ping->max_rtt,
               stddev);
    }
}