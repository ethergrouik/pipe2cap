#define APPNAME "Pipe2Cap"
#define VERSION "0.2.0"
#define DEVNAME "Mr_Ethergrouik"
#define RELEASEDATE "20160222"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "pcap.h"
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef __WIN32__
    #include <io.h>
    #include <windows.h>
#else
    #include <string.h>
#endif // __WIN32__

#define MAX_MTU     65535

#ifdef __WIN32__

int read_stream (int fd, void *d, int iLength );

#endif // __WIN32__

void print_packet( unsigned char * pkt_data, int iLength);
void iface_listing(void);
int pcap_send( pcap_t *fp, unsigned char *pkt, int iLength );
int show_status( int status, int iFramesCpt );
void usage( void );

typedef struct stProject{

    int iIfaceId;
    char *pIfaceName;
    pcap_t *pcap_handle_adapter;
    int iPcapLoopTimer;

}stProject;

stProject *pstProject;

