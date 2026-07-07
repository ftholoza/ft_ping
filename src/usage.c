#include "ft_ping.h"

#include "ft_ping.h"

void print_usage(void)
{
    printf("Usage: ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
    printf(" Options controlling ICMP request types:\n");
    printf("      --echo                 send ICMP_ECHO packets (default)\n\n");
    printf(" Options valid for all request types:\n\n");
    printf("  -v, --verbose              verbose output\n\n");
    printf(" Options valid for --echo requests:\n\n");
    printf("  -?, --help                 give this help list\n");
    printf("      --usage                give a short usage message\n");
    printf("  -V, --version              print program version\n\n");
    printf("Mandatory or optional arguments to long options are also mandatory or optional\n");
    printf("for any corresponding short options.\n\n");
    printf("Options marked with (root only) are available only to superuser.\n\n");
    printf("Report bugs to <bug-inetutils@gnu.org>.\n");
}