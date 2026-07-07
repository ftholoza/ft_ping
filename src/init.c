#include "ft_ping.h"

static int setup_timeout(t_ping *ping)
{
    struct timeval timeout;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;


    //SO_RCVTIMEO set timout
    if (setsockopt(ping->sockfd, SOL_SOCKET, SO_RCVTIMEO,
            &timeout, sizeof(timeout)) < 0)
    {
        perror("setsockopt");
        return 1;
    }
    return 0;
}

static int parse_args(t_ping *ping, int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "-?") == 0)
    {
        print_usage();
        return 1;
    }

    if (argc == 2)
    {
        snprintf(ping->target, sizeof(ping->target), "%s", argv[1]);
        return 0;
    }

    if (argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        ping->verbose = 1;
        snprintf(ping->target, sizeof(ping->target), "%s", argv[2]);
        return 0;
    }

    fprintf(stderr, "Usage: %s [-v] destination\n", argv[0]);
    return 1;
}

int init_ping(t_ping *ping, int argc, char **argv)
{
    memset(ping, 0, sizeof(*ping));

    if (parse_args(ping, argc, argv))
        return 1;

    init_signal();

    ping->sequence = 0;
    //ping sequence start at zero on the netutils-20

    if (resolve_host(ping->target, &ping->res, ping->ip_str) < 0)
        return 1;

    ping->addr = (struct sockaddr_in *)ping->res->ai_addr;

    //set socket for rawsocket
    ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (ping->sockfd < 0)
    {
        perror("socket");
        freeaddrinfo(ping->res);
        return 1;
    }

    //ttl test
    int ttl = 3;

    if (setsockopt(ping->sockfd, IPPROTO_IP, IP_TTL,
            &ttl, sizeof(ttl)) < 0)
    {
        perror("setsockopt TTL");
        close(ping->sockfd);
        freeaddrinfo(ping->res);
        return 1;
    }

    //set socket timout 
    if (setup_timeout(ping))
    {
        close(ping->sockfd);
        freeaddrinfo(ping->res);
        return 1;
    }

    printf("PING %s (%s): %d data bytes\n",
        ping->target,
        ping->ip_str,
        PACKET_SIZE - (int)sizeof(struct icmphdr));

    return 0;
}

void destroy_ping(t_ping *ping)
{
    if (ping->sockfd > 0)
        close(ping->sockfd);
    if (ping->res)
        freeaddrinfo(ping->res);
}