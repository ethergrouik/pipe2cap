#define APPNAME "Pipe2Cap"
#define VERSION "0.1.0"
#define DEVNAME "Mr_Ethergrouik"
#define RELEASEDATE "20160222"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <windows.h>
#include "pcap.h"
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_MTU     65535

void print_packet( unsigned char * pkt_data, int iLength);
void iface_listing(void);
int pcap_send( pcap_t *fp, unsigned char *pkt, int iLength );
int read_stream (int fd, void *d, int iLength );
int show_status( int status, int iFramesCpt );
void usage( void );

typedef struct stProject{

    pcap_t *pcap_handle_stdin;

    int iIfaceId;
    char *pIfaceName;
    pcap_t *pcap_handle_adapter;

    int iPcapLoopTimer;

}stProject;

stProject *pstProject;

