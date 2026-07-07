#include "ft_ping.h"

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

void print_icmp_error(struct iphdr *ip, struct icmphdr *icmp)
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
}