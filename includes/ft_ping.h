#ifndef FT_PING_H
# define FT_PING_H
# define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/time.h>
#include <errno.h>

#define PACKET_SIZE 64
#define RECV_SIZE 1024

#define PACKET_SIZE 64
#define RECV_SIZE 1024


extern volatile sig_atomic_t g_running;

typedef struct s_reply
{
    char ip[INET_ADDRSTRLEN];
    int bytes;
    int ttl;
    int sequence;
} t_reply;

typedef struct s_ping
{
    int verbose;

    int sockfd;
    struct addrinfo *res;
    struct sockaddr_in *addr;

    char target[256];
    char ip_str[INET_ADDRSTRLEN];
    char packet[PACKET_SIZE];

    int transmitted;
    int received;
    
    double min_rtt;
    double max_rtt;
    double total_rtt;
    double total_rtt_sq;


    int sequence;
    struct timeval start;
    struct timeval end;
    t_reply reply;
} t_ping;

void handle_sigint(int sig);
void init_signal(void);

void update_stats(t_ping *ping, double rtt);
void print_stats(t_ping *ping);

int     init_ping(t_ping *ping, int argc, char **argv);
void    destroy_ping(t_ping *ping);

uint16_t    checksum(void *data, size_t len);
void        build_packet(t_ping *ping);

int receive_ping(t_ping *ping);

int resolve_host(char *target, struct addrinfo **res, char *ip_str);

int send_ping(t_ping *ping);

int ping_loop(t_ping *ping);

void print_usage(void);

void print_icmp_error(t_ping *ping, struct iphdr *ip, struct icmphdr *icmp);
void print_ip_data(struct icmphdr *icmp);

#endif