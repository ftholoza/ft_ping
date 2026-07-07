#include "ft_ping.h"

int send_ping(t_ping *ping)
{
    ssize_t sent;

    //stocking time of the send
    gettimeofday(&ping->start, NULL);

    sent = sendto(ping->sockfd,
            ping->packet,
            PACKET_SIZE,
            0,
            (struct sockaddr *)ping->addr,
            sizeof(struct sockaddr_in));

    if (sent < 0)
    {
        perror("sendto");
        return -1;
    }

    return 0;
}