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


// #include "ft_ping.h"


// uint16_t checksum(void *data, size_t len)
// {
//     uint32_t sum = 0;
//     uint16_t *ptr = data;

//     while (len > 1)
//     {
//         sum += *ptr++;
//         len -= 2;
//     }
//     if (len == 1)
//         sum += *(uint8_t *)ptr;

//     while (sum >> 16)
//         sum = (sum & 0xffff) + (sum >> 16);

//     return (uint16_t)(~sum);
// }

// void build_packet(t_ping *ping)
// {
//     struct icmphdr *icmp;

//     memset(ping->packet, 0, PACKET_SIZE);

//     icmp = (struct icmphdr *)ping->packet;
//     icmp->type = ICMP_ECHO;
//     icmp->code = 0;
//     icmp->un.echo.id = getpid();
//     icmp->un.echo.sequence = ping->sequence;
//     icmp->checksum = 0;

//     memset(ping->packet + sizeof(struct icmphdr), 'A',
//            PACKET_SIZE - sizeof(struct icmphdr));

//     icmp->checksum = checksum(ping->packet, PACKET_SIZE);
// }

// int send_ping(t_ping *ping)
// {
//     ssize_t sent;

//     gettimeofday(&ping->start, NULL);

//     sent = sendto(ping->sockfd, ping->packet, PACKET_SIZE, 0,
//                   (struct sockaddr *)ping->addr,
//                   sizeof(struct sockaddr_in));

//     if (sent < 0)
//     {
//         perror("sendto");
//         return -1;
//     }
//     return 0;
// }

// int receive_ping(t_ping *ping)
// {
//     char buffer[RECV_SIZE];
//     struct sockaddr_in recv_addr;
//     socklen_t recv_len = sizeof(recv_addr);
//     ssize_t received;
//     struct iphdr *ip;
//     struct icmphdr *icmp;
//     int ip_header_len;

//     received = recvfrom(ping->sockfd, buffer, sizeof(buffer), 0,
//                         (struct sockaddr *)&recv_addr, &recv_len);

//     gettimeofday(&ping->end, NULL);

//     if (received < 0)
//     {
//         if (errno == EAGAIN || errno == EWOULDBLOCK)
//             return 1; // timeout, pas une erreur fatale
        
//         perror("recvfrom");
//         return -1;
//     }

//     ip = (struct iphdr *)buffer;
//     ip_header_len = ip->ihl * 4;
//     icmp = (struct icmphdr *)(buffer + ip_header_len);

//     if (icmp->type != ICMP_ECHOREPLY ||
//         icmp->un.echo.id != getpid())
//         return 1;

//     ping->reply.bytes = received - ip_header_len;
//     ping->reply.ttl = ip->ttl;
//     ping->reply.sequence = icmp->un.echo.sequence;
//     strcpy(ping->reply.ip, inet_ntoa(recv_addr.sin_addr));

//     return 0;
// }

// int resolve_host(char *target, struct addrinfo **res, char *ip_str)
// {
//     struct addrinfo hints;
//     struct sockaddr_in *addr;
//     int ret;

//     memset(&hints, 0, sizeof(hints));
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_RAW;
//     hints.ai_protocol = IPPROTO_ICMP;

//     ret = getaddrinfo(target, NULL, &hints, res);
//     if (ret != 0)
//     {
//         fprintf(stderr, "%s: unknown host\n", target);
//         return -1;
//     }

//     addr = (struct sockaddr_in *)(*res)->ai_addr;
//     inet_ntop(AF_INET, &addr->sin_addr, ip_str, INET_ADDRSTRLEN);

//     return 0;
// }

// int main(int argc, char **argv)
// {
//     t_ping ping;

//     memset(&ping, 0, sizeof(ping));

//     if (argc != 2)
//     {
//         fprintf(stderr, "Usage: %s destination\n", argv[0]);
//         return 1;
//     }

//     init_signal();

//     snprintf(ping.target, sizeof(ping.target), "%s", argv[1]);
//     ping.sequence = 0;

//     if (resolve_host(ping.target, &ping.res, ping.ip_str) < 0)
//         return 1;

//     ping.addr = (struct sockaddr_in *)ping.res->ai_addr;

//     ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
//     if (ping.sockfd < 0)
//     {
//         perror("socket");
//         freeaddrinfo(ping.res);
//         return 1;
//     }

//     struct timeval timeout;

//     timeout.tv_sec = 1;
//     timeout.tv_usec = 0;

//     if (setsockopt(ping.sockfd, SOL_SOCKET, SO_RCVTIMEO,
//                    &timeout, sizeof(timeout)) < 0)
//     {
//         perror("setsockopt");
//         close(ping.sockfd);
//         freeaddrinfo(ping.res);
//         return 1;
//     }

//     printf("PING %s (%s): %d data bytes\n",
//            ping.target,
//            ping.ip_str,
//            PACKET_SIZE - (int)sizeof(struct icmphdr));

//     while (g_running)
//     {
//         int ret;

//         build_packet(&ping);

//         if (send_ping(&ping) < 0)
//             break;

//         ping.transmitted++;

//         ret = receive_ping(&ping);

//         if (ret == 0)
//         {
//             double rtt;

//             rtt = (ping.end.tv_sec - ping.start.tv_sec) * 1000.0
//                 + (ping.end.tv_usec - ping.start.tv_usec) / 1000.0;

//             update_stats(&ping, rtt);

//             printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
//                    ping.reply.bytes,
//                    ping.reply.ip,
//                    ping.reply.sequence,
//                    ping.reply.ttl,
//                    rtt);
//         }
//         else if (ret < 0)
//             break;

//         ping.sequence++;
//         sleep(1);
//     }

//     print_stats(&ping);

//     close(ping.sockfd);
//     freeaddrinfo(ping.res);
//     return 0;
// }