#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

#define BUFFER_SIZE 65536

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];

    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    printf("[+] Raw socket created. Listening...\n");

    while (1) {
        int data_size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                 &saddr, &saddr_len);
        if (data_size < 0) {
            perror("Recvfrom error");
            break;
        }

        struct ethhdr *eth = (struct ethhdr *)buffer;

        if (ntohs(eth->h_proto) == ETH_P_IP) {
            struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));

            struct sockaddr_in src, dst;
            memset(&src, 0, sizeof(src));
            memset(&dst, 0, sizeof(dst));

            src.sin_addr.s_addr = ip->saddr;
            dst.sin_addr.s_addr = ip->daddr;

            printf("\n[IP PACKET]\n");
            printf(" From: %s\n", inet_ntoa(src.sin_addr));
            printf(" To  : %s\n", inet_ntoa(dst.sin_addr));
            printf(" Protocol: %d\n", ip->protocol);

            if (ip->protocol == IPPROTO_TCP) {
                struct tcphdr *tcp =
                    (struct tcphdr *)(buffer + sizeof(struct ethhdr) + ip->ihl * 4);

                printf("  TCP Src Port: %u\n", ntohs(tcp->source));
                printf("  TCP Dst Port: %u\n", ntohs(tcp->dest));
            }

            else if (ip->protocol == IPPROTO_UDP) {
                struct udphdr *udp =
                    (struct udphdr *)(buffer + sizeof(struct ethhdr) + ip->ihl * 4);

                printf("  UDP Src Port: %u\n", ntohs(udp->source));
                printf("  UDP Dst Port: %u\n", ntohs(udp->dest));
            }

            else if (ip->protocol == IPPROTO_ICMP) {
                struct icmphdr *icmp =
                    (struct icmphdr *)(buffer + sizeof(struct ethhdr) + ip->ihl * 4);

                printf("  ICMP Type: %d Code: %d\n",
                       icmp->type, icmp->code);
            }
        }
    }

    close(sockfd);
    return 0;
}
