#include <iostream>
#include <pcap.h>
#include <netinet/ether.h> // for struct ether_header
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <cstring>
void
packet_pcap_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {

    // Check for VLAN tag
    //bool is_vlan = ntohs(eth_header->ether_type) == ETHERTYPE_VLAN;

    struct ether_header eth_header;
    memcpy(&eth_header, packet, sizeof(struct ether_header));

    // Check if EtherType is IPv4
    if (ntohs(eth_header.ether_type) == ETHERTYPE_IP) {
        // Extract IPv4 header
        struct iphdr ip_header;
        memcpy(&ip_header, packet + sizeof(struct ether_header), sizeof(struct iphdr));

        // Extract source and destination IP addresses
        struct in_addr src_ip, dst_ip;
        src_ip.s_addr = ip_header.saddr;
        dst_ip.s_addr = ip_header.daddr;

        // Print MAC addresses and IP addresses
        printf("Source MAC: %s\n", ether_ntoa((struct ether_addr *)&eth_header.ether_shost));
        printf("Destination MAC: %s\n", ether_ntoa((struct ether_addr *)&eth_header.ether_dhost));
        printf("Source IP: %s\n", inet_ntoa(src_ip));
        printf("Destination IP: %s\n", inet_ntoa(dst_ip));
    } else {
        printf("Not an IPv4 packet.\n");
    }
}

int main(int argc, char *argv[]) {
    // Open the pcap file for reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline(argv[1], nullptr);
    if (pcap == nullptr) {
        std::cout << "Error opening pcap file:" << std::endl;
        std::cout << errbuf << std::endl;
        return 1;
    }

    // Loop through each packet in the pcap file and call the packet_handler function
    if (pcap_loop(pcap, -1, packet_pcap_handler, nullptr) != 0) {
        std::cerr << "Error reading packets from pcap file" << std::endl;
        pcap_close(pcap);
        return 1;
    }

    // Close the pcap file
    pcap_close(pcap);

    return 0;
}
