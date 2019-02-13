

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>          
#include <list>
#include <vector>
#include <thread>
#include <ctime>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <iostream>       // std::cout
#include <thread>         // std::thread

#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#define ucout std::wcout
#define SOCKADDR_IN struct sockaddr_in
#define SOCKET int
#define WORD  unsigned short 
#define IsMulticastAddress(__MCastIP)		(((__MCastIP & 0xFF) >= 0xE0) && ((__MCastIP & 0xFF) <= 0xEF))
#define closesocket(a) (close(a))
#define WSAGetLastError() (errno)
#define PACKET_SIZE 188
#define MAX_PACKET 7
#define BYTE  unsigned char
#define LPBYTE unsigned char*
typedef  unsigned long ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))


class UDPMulticastReceiver
{
public:
	UDPMulticastReceiver(void)
    {
	m_sock = 0;
	ErrorCode = 0;
    }
	~UDPMulticastReceiver(void)
    {
	Unload();
    }
	int Load(const char* ip_address, WORD upd_port, int RecvBufferSize,const char* ip_address_out_bound = NULL)
    {
        ErrorCode = 0;

        int cr;
        


        m_sock = socket( AF_INET, SOCK_DGRAM, 0);
        if(m_sock ==  0)
        {
            ErrorCode = WSAGetLastError();
            return 0;
        }
        /* bind the socket to the internet address */
        memset(&m_addrRecv, 0, sizeof(struct sockaddr_in));


        m_addrRecv.sin_family = AF_INET;
        m_addrRecv.sin_port = htons(upd_port);
        char reuse = 1;
        cr = setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR,(char *)&reuse, sizeof(reuse));

        if (bind(m_sock, (struct sockaddr *)&m_addrRecv, sizeof(m_addrRecv)) != 0) 
        {
            ErrorCode = WSAGetLastError();
            closesocket(m_sock);
            return 0;
        }
        struct in_addr addr;
        if((ip_address_out_bound)&&(strlen(ip_address_out_bound)>0))
        {
            if (inet_pton(AF_INET, ip_address_out_bound, &addr) == 1)
            {
                //			addr.s_addr = inet_addr(ip_address_out_bound);
                cr = setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr));
            }
        }
        /*
        * initialisation de la structure imr
        */

        //m_address = inet_addr(ip_address);
        if (inet_pton(AF_INET, ip_address, &m_address) == 1)
        {
            if (IsMulticastAddress(m_address))
            {
                struct ip_mreq m_imr;
                m_imr.imr_multiaddr.s_addr = m_address;
                if ((ip_address_out_bound) && (strlen(ip_address_out_bound) > 0))
                {
                    if (inet_pton(AF_INET, ip_address_out_bound, &m_imr.imr_interface) != 1)
                    {
                        ErrorCode = WSAGetLastError();
                        closesocket(m_sock);
                        return 0;
                    }
                    //m_imr.imr_interface.s_addr = inet_addr(ip_address_out_bound);
                }
                else
                    m_imr.imr_interface.s_addr = htonl(INADDR_ANY);

                if ((cr = setsockopt(m_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&m_imr, sizeof(m_imr))) == -1)
                {
                    ErrorCode = WSAGetLastError();
                    closesocket(m_sock);
                    return 0;
                }
            }
        }

        if((cr = setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *)&RecvBufferSize, sizeof(RecvBufferSize)))<0)
        {
            ErrorCode = WSAGetLastError();
            closesocket(m_sock);
            return 0;
        }

        return 1;
    }    
	int Unload(void)
    {
        int cr;
        if (m_sock != 0)
        {

            if (IsMulticastAddress(m_address))
            {
                cr = setsockopt(m_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&m_imr, sizeof(m_imr));
            }
            closesocket(m_sock);
            m_sock = 0;
        }
        return 1;
    }
	int Recv(char *p,int* lplen)
    {
        unsigned int l = sizeof(m_addrRecv);
        int status = recvfrom(m_sock, p, *lplen, 0,(struct sockaddr*)&m_addrRecv, &l);
        if (status < 0) 
        {
            ErrorCode = WSAGetLastError();
            return 0;
        }
        *lplen = status;
        return 1;
    }
	int GetLastError()
    {
        return ErrorCode;
    }
private:
	SOCKET m_sock;
	SOCKADDR_IN  m_addrRecv;
	int ErrorCode;
	unsigned long m_address;
	struct ip_mreq m_imr;

};

bool ParseCommandLine(int argc, char* argv[],
	std::string& LocalPrometheusPort,
	std::string& LocalTSAddress,
    WORD& LocalTSPort,
	bool& verbose
)
{
    bool result = false;
	verbose = false;
	LocalPrometheusPort = "8080";
    LocalTSAddress = "127.0.0.1";
    LocalTSPort = 1234;
	try
	{
		while (--argc != 0)
		{

			std::string option = *++argv;
			if (option[0] == '-')
			{
				if (option == "--prometheusport")
				{
					if (--argc != 0)
						LocalPrometheusPort = *++argv;
				}
				else if (option == "--tsport")
				{
					if (--argc != 0)
                    {
						std::string s = *++argv;
                        size_t pos = s.find(':');
                        if((pos>0) && (pos < (s.length()-1)))
                        {
                            LocalTSAddress = s.substr(0,pos);
                            LocalTSPort = (WORD) atoi(s.substr(pos+1,s.length()-1).c_str());
                        
                        }
                    }
				}
				else if (option == "--verbose")
				{
					verbose = true;
				}
				else
					return result;
			}
		}
		if ((LocalPrometheusPort.length() > 0) &&
			(LocalTSAddress.length() > 0) )
		{
				result = true;
		}
	}
	catch (...)
	{
		result = false;
	}
	return result;
}


void TimeThread(prometheus::Counter& counter)
{

  for (;;) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // increment the counter by one (second)
    counter.Increment();
  }
}

void TSThread(prometheus::Gauge& gauge, std::string LocalTSAddress, WORD LocalTSPort, bool verbose )
{
    int buffersize = PACKET_SIZE*70;
	char buffer[70*PACKET_SIZE];
    if(verbose)
    std::cout << "Prepare TS Stream reception: " << LocalTSAddress <<":" << LocalTSPort <<  std::endl;

    UDPMulticastReceiver Receiver;
    if(Receiver.Load(LocalTSAddress.c_str(),LocalTSPort,buffersize,NULL))
    {

        if(verbose)
            std::cout << "TS Stream reception ready: " << LocalTSAddress <<":" << LocalTSPort <<  std::endl;
        int len; 
        unsigned long GlobalCounter=0;
        bool bStop = false;
        len = sizeof(buffer);
        while( (Receiver.Recv(buffer,&len)) && (bStop == false))
        {
            if(len != 0)
            {
                BYTE* packet = (LPBYTE)buffer;
                int l   = len;
                 uint64_t PCR = 0;
                 WORD PID = 0;

                while((packet != NULL) && (l >= PACKET_SIZE))
                {
                    if((packet[0]==0x47)&& ((packet[1]&0x80)==0))
                    {
                        if( ((packet[3]&0x20)!=0) && (packet[4]!=0))
                        {
                            if(packet[5]&0x10)
                            {
                            WORD PID= MAKEWORD(packet[2], packet[1]&0x1F);
                            uint64_t t = (((packet[6] << 24) | (packet[7] << 16) | (packet[8] << 8) | (packet[9])) & 0xFFFFFFFF);
                            t = t << 1;

                            if((packet[10] & 0x80)== 0x80)
                                PCR = (t) | 0x000000001;
                            else
                                PCR = (t) & 0xFFFFFFFFE;

                            double pcr = (double) PCR/90000;
                            gauge.Set(pcr);
                            if(verbose)
                                std::cout << "PCR received value " << PCR << " - " << pcr << " s " << std::endl;
                            }
                        }
                    }
                    l -= PACKET_SIZE;
                    packet += PACKET_SIZE;
                }

                GlobalCounter += len;
            }
            len = sizeof(buffer);            
        }
    }


}

int main(int argc, char* argv[]) {
    using namespace prometheus;
    std::string	LocalPrometheusPort = "8080";
    std::string  LocalTSAddress = "127.0.0.1";
    WORD LocalTSPort = 1234;
	bool verbose = false;


  	bool result = ParseCommandLine(argc, argv, LocalPrometheusPort,LocalTSAddress,LocalTSPort, verbose);
	if (result == false)
	{
		std::cout << "PCR Gauge Custom Prometheus Client : syntax error" << std::endl;
		std::cout << "Syntax:" << std::endl;
		std::cout << "PCRGauge --prometheusport \"<local TCP port used to receive Prometheus request>\" " << std::endl;
		std::cout << "         --tsport \"<local UDP port used to receive the TS stream>\" " << std::endl;
		std::cout << "        [--verbose] " << std::endl;
		return 0;
	}

    if(verbose)
        std::cout << "Prepare Prometheus Server on port: " << LocalPrometheusPort << std::endl;
    // create an http server running on port 8080
    Exposer exposer{LocalPrometheusPort.c_str(), "/metrics", 1};
    if(verbose)
        std::cout << "Prometheus Server ready on port: " << LocalPrometheusPort << std::endl;

    // create a metrics registry with component=main labels applied to all its
    // metrics
    auto registry = std::make_shared<Registry>();

    // add a new counter family to the registry (families combine values with the
    // same name, but distinct label dimensions)
    auto& counter_family = BuildCounter()
                                .Name("time_running_seconds_total")
                                .Help("How many seconds is this server running?")
                                .Labels({{"label", "value"}})
                                .Register(*registry);

    // add a counter to the metric family
    prometheus::Counter& time_counter = counter_family.Add(
        {{"time_counter", "value"}});

    auto& gauge_family =  BuildGauge()
                                .Name("ts_timestamps")
                                .Help("Display TS timestamps")
                                .Labels({{"label", "value"}})
                                .Register(*registry);
    
    prometheus::Gauge& pcr_counter = gauge_family.Add(
        {{"pcr_counter", "value"}});

    // ask the exposer to scrape the registry on incoming scrapes
    exposer.RegisterCollectable(registry);

    // Create Threads for TS Timestamps
    std::thread thts   (TSThread, std::ref(pcr_counter),LocalTSAddress, LocalTSPort, verbose);

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // increment the counter by one (second)
        time_counter.Increment();
    }
  return 0;
}