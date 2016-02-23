#include "main.h"

void usage( void ){

    printf( "\nUsage: 'data_stream' | app network_adapter_id\n" );
    printf( "=======\n");
    printf( "Forward remote sniffed frames to a local network adapter using pcap.\n\n");
    printf("How to:\n");
    printf("=======\n");
    printf("- Add a MS loopback adapter to your Windows box ( let's image that its id will be '1' );\n");
    printf("- Disable all network properties of this new adapter;\n");
    printf("- Install winpcap again in order to add this new adapter to the pcap interfaces list;\n");
    printf("- Connect to your linux remote box like this:\n\n");
    printf("plink.exe -ssh -pw password login@192.168.1.1 \"tcpdump -n -s 0 -i eth0 -w - not port ssh\" | c:\\users\\user\\Downloads\\pipe2cap.exe 1\n\n");
    printf("=> it will forward frames captured by tcpdump on 192.168.1.1 (except ssh) to your local MS loopback adapter identified by its id ( 1 ).\n\n");
    printf("- Start your favorites tools and listen to your MS loopback adapter.\n\n");
    printf("Enjoy!!!\n\n");
    iface_listing();
}

int show_status( int status, int iFramesCpt ){

    int newStatus = 0;
    newStatus = status;

    switch( newStatus ){
        case 0:
            printf( "\r-" );
            break;
        case 1:
            printf( "\r/" );
            break;
        case 2:
            printf( "\r|" );
            break;
        case 3:
            printf( "\r\\" );
            break;
        }
    printf( " : %d", iFramesCpt );
    newStatus++;
    if ( newStatus == 4 ){
        newStatus = 0;
    }
    return newStatus;
}

int read_stream (int fd, void *d, int iLength ){
    int iCpt = 0;
    int result;
    //printf( "R: %d\n", iLength );
    for ( iCpt = 0; iCpt < iLength; ){
        result = read ( fd,(( char* ) d ) + iCpt,( iLength - iCpt ) );
        if ( result == -1 ){
            return -1;
        }
        iCpt += result;
    }
    return iLength;
}

void print_packet( unsigned char * pkt_data, int iLength){

    unsigned char *ptr = NULL;
    int i=0;
    ptr = pkt_data;
    i = iLength;
    do{
        printf( "%s%02X",( i == iLength ) ? " " : ":", *ptr++ );
    }while( --i > 0 );
    printf( "\n" );

}

void iface_listing( void ){

	pcap_if_t *alldevs;
	pcap_if_t *d;

	char errbuf[ PCAP_ERRBUF_SIZE+1 ];

	int iCpt = 0;

	printf( "Available interfaces:\n" );
    printf( "=====================\n" );
	if( pcap_findalldevs( &alldevs, errbuf ) == -1 ){
		printf( "No network interface found!" );
		return;
	}

	// Scan the list printing every entry
	for( d = alldevs; d; d = d->next ){
        if ( d->name != NULL ){
            printf( " %d : %s\t(%s)\n", iCpt, ( const char* )d->name, ( const char* )d->description );
        }
        iCpt++;
	}
	printf( "\n\n" );

	// Free the device list
	pcap_freealldevs(alldevs);
}

int pcap_send( pcap_t *fp, unsigned char *pkt, int iLength ){

    if ( iLength > MAX_MTU ){
        return 1;
    }
    if ( pcap_sendpacket( fp, pkt, iLength ) != 0 ){
        return 1;
    }
    return 0;

}

int main( int argc, char *argv[] ){

	pcap_if_t *alldevs;
	pcap_if_t *d;
    int iCpt = 0;
    char errbuf[ PCAP_ERRBUF_SIZE ];
    unsigned char buf[ MAX_MTU ];
    int status = 0;
    int iFramesCpt = 0;

    struct pcap_file_header stFileHeader;

    system("cls");
    printf( "\n%s %s ( %s )\n", APPNAME, VERSION, RELEASEDATE );

    for ( iCpt = 0 ; iCpt < ( strlen( APPNAME ) + strlen( VERSION ) + strlen( RELEASEDATE ) + 6 ) ; iCpt++ ){
        printf("=");
    }
    printf("\n");
    printf( "%s ( ethergrouik@gmail.com )\n\n", DEVNAME );

    assert ( sizeof ( stFileHeader ) == 24 );
    assert ( sizeof ( unsigned int ) == 4 );
    assert ( sizeof ( unsigned short ) == 2 );

    errbuf[ 0 ]='\0';
    memset( &errbuf, 0, PCAP_ERRBUF_SIZE );

    if ( NULL == ( pstProject = ( stProject* )malloc( sizeof( stProject ) ) ) )
        exit( EXIT_FAILURE );

    pstProject->pIfaceName = NULL;
    pstProject->iPcapLoopTimer = 10;

    if ( argc == 1 ){
        usage();
        exit( EXIT_SUCCESS );
    }

    if ( ( pstProject->iIfaceId = atoi(argv[ 1 ] ) ) < 0 ){
        usage();
        exit( EXIT_FAILURE );
    }

	if( pcap_findalldevs( &alldevs, errbuf ) == -1 ){
		printf( "No network interface found!" );
		exit( EXIT_FAILURE );
	}

    iCpt=0;
    for( d = alldevs; d; d = d->next ){
        if ( pstProject->iIfaceId == iCpt ){
            printf( "Iface2: %s (%s)\n\n", d->name, d->description );
            pstProject->pIfaceName = strdup( d->name );
        }
        iCpt++;
    }

    errbuf[ 0 ] = '\0';
    memset( &errbuf, 0, PCAP_ERRBUF_SIZE );

    if( NULL== ( pstProject->pcap_handle_adapter = pcap_open_live( pstProject->pIfaceName,BUFSIZ, 1, pstProject->iPcapLoopTimer, errbuf ) ) ){
        printf( "Error: cannot pcap_open_live %s!\n", pstProject->pIfaceName );
        exit( EXIT_FAILURE );
    }

    if ( setmode( fileno( stdin ), O_BINARY ) == -1 ){
        printf( "Error: cannot setmode stdin to O_BINARY\n" );
        exit( EXIT_FAILURE );
    }

    // first we need to skip 24 bytes of file header
    read_stream( STDIN_FILENO, &stFileHeader, 24 );

    while ( 1 ){

        struct pcap_pkthdr stPktHeader;

        buf[ 0 ]='\0';
        memset( &buf, 0, PCAP_ERRBUF_SIZE );

        read_stream( STDIN_FILENO, &stPktHeader, sizeof ( stPktHeader ) );

        if ( stPktHeader.caplen > 0){

            //printf("%d // %d\n", stPktHeader.caplen, stPktHeader.len );
            if ( stPktHeader.caplen <= MAX_MTU ){

                iFramesCpt++;
                status = show_status( status, iFramesCpt );
                if ( iFramesCpt == 0xffffffff - 5 ){
                    iFramesCpt = 0;
                    printf( "+%d\n", 0xffffffff-5 );
                }

                read_stream( STDIN_FILENO, buf, stPktHeader.caplen );
                //print_packet( buf, stPktHeader.caplen );
                pcap_send( pstProject->pcap_handle_adapter, buf, stPktHeader.len );
            }
            else{
                printf( "W: MTU=%d\n", stPktHeader.caplen );
            }
        }
        else{
            printf( "W: caplen < 0\n" );
        }
    }
    printf( "The End!!!\n" );

    return 0;
}

