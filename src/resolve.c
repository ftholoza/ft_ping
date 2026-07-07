#include "ft_ping.h"

int resolve_host(char *target, struct addrinfo **res, char *ip_str)
{
    struct addrinfo     hints;
    struct sockaddr_in  *addr;
    int                 ret;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;   //option to get only IPv4 destinations
    hints.ai_socktype = SOCK_RAW;  //ping uses raw sockets 
    hints.ai_protocol = IPPROTO_ICMP; //we use ICMP protocol

    //get a struct with every informations i need to use sendto()

    ret = getaddrinfo(target, NULL, &hints, res);
    if (ret != 0)
    {
        fprintf(stderr, "%s: unknown host\n", target);
        return -1;
    }

    addr = (struct sockaddr_in *)(*res)->ai_addr;

    //inet network to presentation
    
    if (!inet_ntop(AF_INET, &addr->sin_addr, ip_str, INET_ADDRSTRLEN))
    {
        perror("inet_ntop");
        freeaddrinfo(*res);
        *res = NULL;
        return -1;
    }

    return 0;
}