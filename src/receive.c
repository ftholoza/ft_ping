#include "ft_ping.h"

int receive_ping(t_ping *ping)
{
    char                buffer[RECV_SIZE];
    struct sockaddr_in  recv_addr;
    socklen_t           recv_len;
    ssize_t             received;
    struct iphdr        *ip;
    struct icmphdr      *icmp;
    int                 ip_header_len;


    while (1)
    {
        recv_len = sizeof(recv_addr);
        
        received = recvfrom(ping->sockfd, buffer, sizeof(buffer), 0,
                (struct sockaddr *)&recv_addr, &recv_len);
        
        //stocking time of the receive
        gettimeofday(&ping->end, NULL);
        
        if (received < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return 1;
            perror("recvfrom");
            return -1;
        }
    
        ip = (struct iphdr *)buffer;
        //ip->ihl number of 4octets/32bits word in the header 
        ip_header_len = ip->ihl * 4;
    
        //check if the received packet is not broken 
        if (received < ip_header_len + (ssize_t)sizeof(struct icmphdr))
            return 1;
    
        /*buffer =
            | header ip | header icmp | data |
        */
    
    
        //check if its a echo reply or print the type of packet with verbose if its enabled
        icmp = (struct icmphdr *)(buffer + ip_header_len);
    
        if (icmp->type == ICMP_ECHO)
        {
            continue;
        }
    
        if (icmp->type != ICMP_ECHOREPLY)
        {
            ping->reply.bytes = received - ip_header_len;
            print_icmp_error(ping, ip, icmp);
            return 1;
        }
        
    
        //if its not an echoreply of our ping continue
        if (icmp->un.echo.id != getpid())
            return 1;
    
        ping->reply.bytes = received - ip_header_len;
        //ttl to check how many routers we traveled
        ping->reply.ttl = ip->ttl;
        ping->reply.sequence = icmp->un.echo.sequence;
    
        snprintf(ping->reply.ip,
            sizeof(ping->reply.ip),
            "%s",
            inet_ntoa(recv_addr.sin_addr));
        
        return 0;
    }
}