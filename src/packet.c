#include "ft_ping.h"

uint16_t checksum(void *data, size_t len)
{
    uint32_t sum;
    uint16_t *ptr;

    sum = 0;
    ptr = data;

    //adding data 16bits by 16bits in the sum

    while (len > 1)
    {
        sum += *ptr++;
        len -= 2;
    }

    //adding last bits if len is odd

    if (len == 1)
        sum += *(uint8_t *)ptr;

    //folding the sum on itself if its bigger than 16bits
    
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    //inversing bits its part of the protocol
    return ((uint16_t)(~sum));
}

void build_packet(t_ping *ping)
{
    struct icmphdr *icmp;

    memset(ping->packet, 0, PACKET_SIZE);

    icmp = (struct icmphdr *)ping->packet;

    //ICMP_ECHO we want a ping request, the receiver will send a echoreply to this
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->un.echo.id = getpid();
    icmp->un.echo.sequence = ping->sequence;

    //fill the data of the packet 
    memset(ping->packet + sizeof(struct icmphdr),
           'A',
           PACKET_SIZE - sizeof(struct icmphdr));

    icmp->checksum = checksum(ping->packet, PACKET_SIZE);
}