#ifndef SOCKUTILS_H
#define SOCKUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <net/ethernet.h> 
#include <netinet/ip_icmp.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <netinet/ip.h>
#include <netinet/igmp.h>
#include <netinet/icmp6.h>

#include <pcap.h>

/********************* Usefull macros *********************/

#define FORCE_INLINE __attribute__((always_inline)) inline

#define ___my_swab16(x) \
    ((u_int16_t)( \
    (((u_int16_t)(x) & (u_int16_t)0x00ffU) << 8) | \
    (((u_int16_t)(x) & (u_int16_t)0xff00U) >> 8) ))

#define ___my_swab32(x) \
    ((u_int32_t)( \
    (((u_int32_t)(x) & (u_int32_t)0x000000ffUL) << 24) | \
    (((u_int32_t)(x) & (u_int32_t)0x0000ff00UL) <<  8) | \
    (((u_int32_t)(x) & (u_int32_t)0x00ff0000UL) >>  8) | \
    (((u_int32_t)(x) & (u_int32_t)0xff000000UL) >> 24) ))


#define ETH_P_ALL   0x0003


#define BUFF_SIZE 65536
#define ETH2_HEADER_LEN 14
#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define ITF_MAX_NBR 0x10

#define PCAP_FILTER_SIZE  64
#define RECORD_FILENAME_SIZE  32

#define PCAP_NETMASK_UNKNOWN 0xffffffff


/********************* OSI Layer 2 protocols structs *********************/

// IEEE 1905 1a Header

typedef struct ieee_1905_header {

    uint8_t msg_version;
    uint8_t reserved;
    uint16_t msg_type;
    uint16_t msg_id;
    uint8_t frag_id;
    uint8_t last_frag;

} __attribute__((packed)) ieee_1905_header;


// LLTD Header 

typedef struct lltd_header {

    uint8_t version;
    uint8_t service_type;
    uint8_t reserved;
    uint8_t function;
    unsigned char real_dest[MAC_LENGTH];
    unsigned char real_src[MAC_LENGTH];

} __attribute__((packed)) lltd_header;


// HOMEPLUG AV (POWERLINE) Header

typedef struct homeplug_av_header {

    uint8_t protocol;
    uint16_t type;
    uint8_t frag;

} __attribute__((packed)) homeplug_av_header;


// HOMEPLUG Standard Header

typedef struct homeplug_header {

    uint8_t ctrl_field;
    uint8_t mac_entry;
    uint8_t entry_length;
    unsigned char spe_vendor[3];

} __attribute__((packed)) homeplug_header;


// PN-DCP Header

typedef struct profinet_dcp_header {

    uint16_t frame_id;
    uint8_t serv_id;
    uint8_t serv_type;
    uint32_t xid;
    uint16_t resp_delay;
    uint16_t data_len;
    uint8_t option;
    uint8_t sub_option;
    uint16_t block_len; 

} __attribute__((packed)) profinet_dcp_header;


/********************* OSI Layer 3 protocols structs *********************/

// IPv6 Header 

typedef struct ipv6_header {

    unsigned int
        version : 4,
        traffic_class : 8,
        flow_label : 20;

    uint16_t length;
    uint8_t  next_header;
    uint8_t  hop_limit;

    struct in6_addr src;
    struct in6_addr dst;

} __attribute__((packed)) ipv6_header;


// ICMPv6 Header

typedef struct icmp6_header{

    uint8_t type;
    uint8_t code;
    uint16_t cksum;

    // for id and seqno fields 
    uint32_t data;

} __attribute__((packed)) icmp6_header;


// ARP Header

typedef struct arp_header {

    uint16_t hardware_type;
    uint16_t protocol_type;
    unsigned char hardware_len;
    unsigned char protocol_len;
    uint16_t opcode;
    unsigned char sender_mac[MAC_LENGTH];
    unsigned char sender_ip[IPV4_LENGTH];
    unsigned char target_mac[MAC_LENGTH];
    unsigned char target_ip[IPV4_LENGTH];

} __attribute__((packed)) arp_header;


// ICMPv4 Header

typedef struct icmp_header {

    u_int8_t type;
    u_int8_t code;
    u_int16_t checksum;
    
    union{
        struct echo
        {
            u_int16_t id;
            u_int16_t sequence;

        } echo;

        u_int32_t gateway;
        
        struct frag
        {
            u_int16_t __unused;
            u_int16_t mtu;
        } frag;
    } un;

} __attribute__((packed)) icmp_header;


/********************* OSI Layer 7 protocols structs *********************/

// DNS Header 

typedef struct dns_header {

    unsigned short id;
 
    unsigned char rd :1;
    unsigned char tc :1;
    unsigned char aa :1;
    unsigned char opcode :4;
    unsigned char qr :1;
 
    unsigned char rcode :4;
    unsigned char cd :1;
    unsigned char ad :1;
    unsigned char z :1;
    unsigned char ra :1;
 
    unsigned short q_count;
    unsigned short ans_count;
    unsigned short auth_count;
    unsigned short add_count;

} __attribute__((packed)) dns_header;


/************************************* Functions declarations *************************************/

int setup_promiscuous_mode(int, int);
void print_itf_list();

void print_ethernet_header(unsigned char*, int);
void process_ip_packet(unsigned char* , int);
void process_frame(unsigned char* , int);
void print_arp_header(unsigned char*);
void print_profinet_dcp_header(unsigned char*);
void print_ip_header(unsigned char* , int);
void print_ip6_header(unsigned char*, int);
void print_tcp_packet(unsigned char * , int );
void print_udp_packet(unsigned char * , int );
void print_icmp_packet(unsigned char* , int );
void print_igmp_header(unsigned char*, int);

void print_homeplug_av_header(unsigned char*);
void print_homeplug_header(unsigned char*);
void print_ieee_1905_header(unsigned char*, int);

void print_lltd_header(unsigned char*);

void print_dns_packet(unsigned char*, int);

void print_icmpv6_packet(unsigned char*, int, int);

void print_data(unsigned char* , int);
void print_hex_ascii_line(const u_char*, int, int);
uint16_t in_cksum(uint16_t *addr, int len);

void print_current_time();

#endif
