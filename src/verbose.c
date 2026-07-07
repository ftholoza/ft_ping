#include "ft_ping.h"

#include "ft_ping.h"

static void print_ip_header(struct iphdr *ip)
{
    struct in_addr src;
    struct in_addr dst;
    unsigned short frag_off;

    src.s_addr = ip->saddr;
    dst.s_addr = ip->daddr;
    frag_off = ntohs(ip->frag_off);

    printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
    printf(" %1x  %1x  %02x", ip->version, ip->ihl, ip->tos);
    printf(" %04x %04x",
        ntohs(ip->tot_len),
        ntohs(ip->id));
    printf("   %1x %04x",
        (frag_off & 0xe000) >> 13,
        frag_off & 0x1fff);
    printf("  %02x  %02x %04x",
        ip->ttl,
        ip->protocol,
        ntohs(ip->check));
    printf(" %s ", inet_ntoa(src));
    printf(" %s ", inet_ntoa(dst));
    printf("\n");
}

static void print_ip_hdr_dump(struct iphdr *ip)
{
    size_t          i;
    unsigned char   *bytes;

    bytes = (unsigned char *)ip;

    printf("IP Hdr Dump:\n ");
    i = 0;
    while (i < sizeof(struct iphdr))
    {
        printf("%02x", bytes[i]);
        if (i % 2)
            printf(" ");
        i++;
    }
    printf("\n");
}

void print_ip_data(struct icmphdr *icmp)
{
    struct iphdr    *ip;
    unsigned char   *data;
    int             hlen;
    int             size;
    int             type;
    int             code;
    unsigned int    id;
    unsigned int    seq;

    ip = (struct iphdr *)((unsigned char *)icmp + sizeof(struct icmphdr));

    print_ip_hdr_dump(ip);
    print_ip_header(ip);

    hlen = ip->ihl * 4;
    data = (unsigned char *)ip + hlen;
    size = ntohs(ip->tot_len) - hlen;

    if (ip->protocol == IPPROTO_ICMP)
    {
        type = data[0];
        code = data[1];

        printf("ICMP: type %u, code %u, size %u",
            type,
            code,
            size);

        if (type == ICMP_ECHOREPLY || type == ICMP_ECHO)
        {
            id = data[4] * 256 + data[5];
            seq = data[6] * 256 + data[7];

            printf(", id 0x%04x, seq 0x%04x", id, seq);
        }
        printf("\n");
    }
}

static void print_dest_unreach(struct icmphdr *icmp)
{
    if (icmp->code == ICMP_NET_UNREACH)
        printf("Destination Net Unreachable\n");
    else if (icmp->code == ICMP_HOST_UNREACH)
        printf("Destination Host Unreachable\n");
    else if (icmp->code == ICMP_PROT_UNREACH)
        printf("Destination Protocol Unreachable\n");
    else if (icmp->code == ICMP_PORT_UNREACH)
        printf("Destination Port Unreachable\n");
    else if (icmp->code == ICMP_FRAG_NEEDED)
        printf("Frag needed and DF set\n");
    else if (icmp->code == ICMP_SR_FAILED)
        printf("Source Route Failed\n");
    else
        printf("Destination Unreachable, Bad Code: %d\n", icmp->code);
}

static void print_time_exceeded(struct icmphdr *icmp)
{
    if (icmp->code == ICMP_EXC_TTL)
        printf("Time to live exceeded\n");
    else if (icmp->code == ICMP_EXC_FRAGTIME)
        printf("Frag reassembly time exceeded\n");
    else
        printf("Time exceeded, Bad Code: %d\n", icmp->code);
}

static void print_redirect(struct icmphdr *icmp)
{
    if (icmp->code == ICMP_REDIR_NET)
        printf("Redirect Network\n");
    else if (icmp->code == ICMP_REDIR_HOST)
        printf("Redirect Host\n");
    else if (icmp->code == ICMP_REDIR_NETTOS)
        printf("Redirect Type of Service and Network\n");
    else if (icmp->code == ICMP_REDIR_HOSTTOS)
        printf("Redirect Type of Service and Host\n");
    else
        printf("Redirect, Bad Code: %d\n", icmp->code);
}

void print_icmp_error(t_ping *ping, struct iphdr *ip, struct icmphdr *icmp)
{
    struct in_addr addr;

    addr.s_addr = ip->saddr;

    printf("From %s: ", inet_ntoa(addr));

    if (icmp->type == ICMP_DEST_UNREACH)
        print_dest_unreach(icmp);
    else if (icmp->type == ICMP_TIME_EXCEEDED)
        print_time_exceeded(icmp);
    else if (icmp->type == ICMP_REDIRECT)
        print_redirect(icmp);
    else if (icmp->type == ICMP_PARAMETERPROB)
        printf("Parameter problem\n");
    else
        printf("ICMP type=%d code=%d\n", icmp->type, icmp->code);
    if (ping->verbose)
        print_ip_data(icmp);
}