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
/*
class InputStream {
public:
	InputStream()
    {
	address = std::string("");
	port = 0;
    prefix = std::string("");
    }    
	std::string address;
	WORD port;
	std::string prefix;
};


class InputStream {
public:
    InputStream(
        	std::string _address,
            WORD _port,
            std::string _prefix,
            prometheus::Gauge&  _pcr_timestamp,
            prometheus::Gauge&  _pcr_timestamp_acquisition_time,
            prometheus::Gauge&  _opcr_timestamp,
            prometheus::Gauge&  _opcr_timestamp_acquisition_time,
            prometheus::Gauge&  _dts_timestamp,
            prometheus::Gauge&  _dts_timestamp_acquisition_time,
            prometheus::Gauge&  _pts_timestamp,
            prometheus::Gauge&  _pts_timestamp_acquisition_time,
            prometheus::Gauge&  _t12_timestamp,
            prometheus::Gauge&  _t12_timestamp_acquisition_time)
    {
	address = _address;
	port = _port;
    prefix = _prefix;

    pcr_timestamp.Set(_pcr_timestamp.Value());
    pcr_timestamp_acquisition_time.Set(_pcr_timestamp_acquisition_time.Value());
    opcr_timestamp.Set(_opcr_timestamp.Value());
    opcr_timestamp_acquisition_time.Set( _opcr_timestamp_acquisition_time.Value());
    dts_timestamp.Set(_dts_timestamp.Value());
    dts_timestamp_acquisition_time.Set(_dts_timestamp_acquisition_time.Value());
    pts_timestamp.Set(_pts_timestamp.Value());
    pts_timestamp_acquisition_time.Set(_pts_timestamp_acquisition_time.Value());
    t12_timestamp.Set(_t12_timestamp.Value());
    t12_timestamp_acquisition_time.Set(_t12_timestamp_acquisition_time.Value());  

    }
    InputStream()
    {
	address = std::string("");
	port = 0;
	prefix = std::string("");
    pcr_timestamp.Set(GetGauge().Value());
    pcr_timestamp_acquisition_time.Set(GetGauge().Value());
    opcr_timestamp.Set(GetGauge().Value());
    opcr_timestamp_acquisition_time.Set(GetGauge().Value());
    dts_timestamp.Set(GetGauge().Value());
    dts_timestamp_acquisition_time.Set(GetGauge().Value());
    pts_timestamp.Set(GetGauge().Value());
    pts_timestamp_acquisition_time.Set(GetGauge().Value());
    t12_timestamp.Set(GetGauge().Value());
    t12_timestamp_acquisition_time.Set(GetGauge().Value());    
    }
    InputStream(const InputStream& i)
    {
	address = i.address;
	port = i.port;
	prefix = i.prefix;
    pcr_timestamp.Set(i.pcr_timestamp.Value());
    pcr_timestamp_acquisition_time.Set(i.pcr_timestamp_acquisition_time.Value());
    opcr_timestamp.Set(i.opcr_timestamp.Value());
    opcr_timestamp_acquisition_time.Set(i.opcr_timestamp_acquisition_time.Value());
    dts_timestamp.Set(i.dts_timestamp.Value());
    dts_timestamp_acquisition_time.Set(i.dts_timestamp_acquisition_time.Value());
    pts_timestamp.Set(i.pts_timestamp.Value());
    pts_timestamp_acquisition_time.Set(i.pts_timestamp_acquisition_time.Value());
    t12_timestamp.Set(i.t12_timestamp.Value());
    t12_timestamp_acquisition_time.Set(i.t12_timestamp_acquisition_time.Value());    
    }
	std::string address;
	WORD port;
	std::string prefix;
    prometheus::Gauge&  pcr_timestamp = GetGauge();
    prometheus::Gauge&  pcr_timestamp_acquisition_time = GetGauge();
    prometheus::Gauge&  opcr_timestamp = GetGauge();
    prometheus::Gauge&  opcr_timestamp_acquisition_time = GetGauge();
    prometheus::Gauge&  dts_timestamp  = GetGauge();
    prometheus::Gauge&  dts_timestamp_acquisition_time = GetGauge();
    prometheus::Gauge&  pts_timestamp = GetGauge();
    prometheus::Gauge&  pts_timestamp_acquisition_time = GetGauge();
    prometheus::Gauge&  t12_timestamp = GetGauge();
    prometheus::Gauge&  t12_timestamp_acquisition_time = GetGauge();    


    prometheus::Gauge _gauge;

    prometheus::Gauge& GetGauge()
    {
        return _gauge;
    }
};
*/
class InputStream {
public:
    InputStream(
        	std::string _address,
            WORD _port,
            std::string _prefix,
            std::thread *p)
    {
	address = _address;
	port = _port;
    prefix = _prefix;
    pThread = p;   
    }
    InputStream()
    {
	address = std::string("");
	port = 0;
	prefix = std::string("");   
    pThread = nullptr;   
    }
    InputStream(const InputStream& i)
    {
	address = i.address;
	port = i.port;
	prefix = i.prefix; 
    pThread = nullptr;   
    }
	std::string address;
	WORD port;
	std::string prefix;
    std::thread *pThread;
};

class UDPReceiver
{
public:
	UDPReceiver(void)
    {
	m_sock = 0;
	ErrorCode = 0;
    }
	~UDPReceiver(void)
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
                cr = setsockopt(m_sock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr));
            }
        }
        /*
        * imr structure initialization
        */

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
std::vector<std::string> split(const std::string& s, char seperator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}
bool ParseCommandLine(int argc, char* argv[],
	std::string& LocalPrometheusPort,
    std::vector<InputStream>& Array,
	bool& verbose
)
{
    bool result = false;
	verbose = false;
    const char* endPointPrefix = "EndPoint_";
	LocalPrometheusPort = "8080";
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
                        std::vector<std::string> arrayEndPoint =  split(*++argv,',');
                        if(arrayEndPoint.size()>0)
                        {
                            int index = 0;
                            for(std::string  s : arrayEndPoint)
                            {
                                std::vector<std::string> endPoint =  split(s,':');
                                if(endPoint.size()==2)
                                {
                                    InputStream *pis = new InputStream();
                                    if(pis!=nullptr)
                                    {
                                        pis->address = endPoint[0];
                                        pis->prefix = endPointPrefix + std::to_string(index) ;
                                        pis->port =  (WORD) atoi(endPoint[1].c_str()); 
                                        Array.push_back(*pis);
                                    }
                                }
                                else if(endPoint.size()==3)
                                {
                                    InputStream *pis = new InputStream();
                                    if(pis!=nullptr)
                                    {
                                        pis->address = endPoint[1];
                                        pis->prefix = endPoint[0] ;
                                        pis->port =  (WORD) atoi(endPoint[2].c_str());
                                        Array.push_back(*pis);
                                    }
                                }
                                else
                                {
                                    return result;
                                }
                                index++;
                            }
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
		if (Array.size() > 0)
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
std::string nowstring(long t)
{
  long localt =  t/1000000000;
  std::time_t now= std::time(&localt);
  std::tm* now_tm= std::gmtime(&now);
  char buf[42];
  std::strftime(buf, 42, "%F %X", now_tm);
  return buf;
}
long now() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}
int GetPCRInAdaptationField(LPBYTE Packet, uint64_t* lpPCR)
{
	if((Packet[0]==0x47)&& ((Packet[1]&0x80)==0))
	{
		if( ((Packet[3]&0x20)!=0) && (Packet[4]!=0))
		 {
			 if(Packet[5]&0x10)
			 {
				uint64_t pcr_base = (((Packet[6] << 24) | (Packet[7] << 16) | (Packet[8] << 8) | (Packet[9])) & 0xFFFFFFFF);
				pcr_base = pcr_base << 1;

				if((Packet[10] & 0x80)== 0x80)
					pcr_base = (pcr_base) | 0x000000001;
				else
					pcr_base = (pcr_base) & 0xFFFFFFFFE;

				uint64_t pcr_ext =  ( ((Packet[10] & 0x01) == 0x01)? 0x100 : 0x00);
				pcr_ext = pcr_ext | Packet[11];

				*lpPCR = pcr_base*300 + pcr_ext;
				return 1;
			 }
		 }
	}
	return 0;
}
int GetOPCRInAdaptationField(LPBYTE Packet, uint64_t* lpOPCR)
{
	if((Packet[0]==0x47)&& ((Packet[1]&0x80)==0))
	{
		if( ((Packet[3]&0x20)!=0) && (Packet[4]!=0))
		 {
			 if(Packet[5]&0x08)
			 {
				int Offset = 2;
				if(Packet[5]&0x10)
					Offset += 6;

				uint64_t t = (((Packet[4+Offset] << 24) | (Packet[5+Offset] << 16) | (Packet[6+Offset] << 8) | (Packet[7+Offset])) & 0xFFFFFFFF);
				t = t << 1;

				*lpOPCR = (t) | ((Packet[8+Offset] & 0x80) >> 7);
				return 1;
			 }
		 }
	}
	return 0;

}
int GetDTSInAdaptationField(LPBYTE Packet, uint64_t* lpDTS)
{
	if((Packet[0]==0x47)&& ((Packet[1]&0x80)==0))
	{
		if( ((Packet[3]&0x20)!=0) && (Packet[4]!=0))
		 {
			 if(Packet[5]&0x01)
			 {
				 int Offset = 2;
				 if(Packet[5]&0x10)
					 Offset += 6;
				 if(Packet[5]&0x08)
					 Offset += 6;
				 if(Packet[5]&0x04)
					 Offset += 1;
				 if(Packet[5]&0x02)
					 Offset += 2;
				 if(Packet[5]&0x01)
				 {
					 Offset += 2;
					 if(Packet[4 + Offset - 1] & 0x80)
						 Offset += 2;
					 if(Packet[4 + Offset - 1] & 0x40)
						 Offset += 3;
					 if(Packet[4 + Offset - 1] & 0x20)
					 {
						 uint64_t t = Packet[4 + Offset]& 0x0E;
						 t = (t << 29)&     0x00000001C0000000;
						 t |=        ((Packet[4 + Offset + 1]& 0xFF) << 22)& 0x000000003FC00000;
						 t |=        ((Packet[4 + Offset + 2]& 0xFE) << 14)& 0x00000000003F8000;
						 t |=        ((Packet[4 + Offset + 3]& 0xFF) << 7) & 0x0000000000007F80;
						 t |=        ((Packet[4 + Offset + 4]& 0xFE) >> 1) & 0x000000000000007F;
						*lpDTS = t;
						 return 1;
					 }
				 }

			 }
		 }
	}
	return 0;
}
int GetDTSInPES(LPBYTE Packet, uint64_t* lpDTS)
{
	int Offset = 4;
	if((Packet[0]==0x47)&& ((Packet[1]&0x80)==0))
	{
		if((Packet[3]&0x20)!=0)
			Offset += (Packet[4]+1);
		if((Packet[3]&0x10)!=0)
		{
			if( (Packet[Offset]==0x00) &&
				(Packet[Offset+1]==0x00) &&
				(Packet[Offset+2]==0x01))
			{
				if( (Packet[Offset+3]!=0xBC) && //program stream map
					(Packet[Offset+3]!=0xBE) && // padding stream
					(Packet[Offset+3]!=0xBF) && // private stream
					(Packet[Offset+3]!=0xF0) && // ECM
					(Packet[Offset+3]!=0xF1) && // EMM
					(Packet[Offset+3]!=0xFF) && // program stream directroy
					(Packet[Offset+3]!=0xF2) && // DSMCC stream
					(Packet[Offset+3]!=0xF8))   // Type E stream
				{
					if((Packet[Offset+7]& 0xC0) == 0xC0)
					{
						 uint64_t t = Packet[14 + Offset]& 0x0E;
						 t = (t << 29)&     0x00000001C0000000;
						 t |=        ((Packet[14 + Offset + 1]& 0xFF) << 22)& 0x000000003FC00000;
						 t |=        ((Packet[14 + Offset + 2]& 0xFE) << 14)& 0x00000000003F8000;
						 t |=        ((Packet[14 + Offset + 3]& 0xFF) << 7) & 0x0000000000007F80;
						 t |=        ((Packet[14 + Offset + 4]& 0xFE) >> 1) & 0x000000000000007F;
						*lpDTS = t;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
int GetPTSInPES(LPBYTE Packet, uint64_t* lpPTS)
{
	int Offset = 4;
	if((Packet[0]==0x47)&& ((Packet[1]&0x80)==0))
	{
		if((Packet[3]&0x20)!=0)
			Offset += (Packet[4]+1);
		if((Packet[3]&0x10)!=0)
		{
			if( (Packet[Offset]==0x00)&&
				(Packet[Offset+1]==0x00)&&
				(Packet[Offset+2]==0x01))
			{
				if( (Packet[Offset+3]!=0xBC) && //program stream map
					(Packet[Offset+3]!=0xBE) && // padding stream
					(Packet[Offset+3]!=0xBF) && // private stream
					(Packet[Offset+3]!=0xF0) && // ECM
					(Packet[Offset+3]!=0xF1) && // EMM
					(Packet[Offset+3]!=0xFF) && // program stream directroy
					(Packet[Offset+3]!=0xF2) && // DSMCC stream
					(Packet[Offset+3]!=0xF8))   // Type E stream
				{
					if((Packet[Offset+7]& 0x80) == 0x80)
					{
						 uint64_t t = (Packet[9 + Offset]& 0x0E);
						 t = (t << 29)&     0x00000001C0000000;
						 t |=        ((Packet[9 + Offset + 1]& 0xFF) << 22)& 0x000000003FC00000;
						 t |=        ((Packet[9 + Offset + 2]& 0xFE) << 14)& 0x00000000003F8000;
						 t |=        ((Packet[9 + Offset + 3]& 0xFF) << 7) & 0x0000000000007F80;
						 t |=        ((Packet[9 + Offset + 4]& 0xFE) >> 1) & 0x000000000000007F;
						*lpPTS = t;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
int GetT12(LPBYTE Packet, uint64_t* lpPTS)
{
    // To be complated
    *lpPTS = 0;
    return 0;
}
void TSThread(prometheus::Gauge& pcr_timestamp,
              prometheus::Gauge& pcr_timestamp_acquisition_time,
              prometheus::Gauge& opcr_timestamp,
              prometheus::Gauge& opcr_timestamp_acquisition_time, 
              prometheus::Gauge& dts_timestamp,
              prometheus::Gauge& dts_timestamp_acquisition_time, 
              prometheus::Gauge& pts_timestamp,
              prometheus::Gauge& pts_timestamp_acquisition_time, 
              prometheus::Gauge& t12_timestamp,
              prometheus::Gauge& t12_timestamp_acquisition_time, 
              std::string LocalTSAddress, 
              WORD LocalTSPort, 
              bool verbose )
{
    int buffersize = PACKET_SIZE*70;
	char buffer[70*PACKET_SIZE];
    if(verbose)
    std::cout << "Prepare TS Stream reception: " << LocalTSAddress <<":" << LocalTSPort <<  std::endl;

    UDPReceiver Receiver;
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
                int length   = len;
                 uint64_t PCR = 0;
                 uint64_t l;
                 WORD PID = 0;

                while((packet != NULL) && (length >= PACKET_SIZE))
                {
                    if((packet[0]==0x47)&& ((packet[1]&0x80)==0))
                    {
                        if(GetPCRInAdaptationField((LPBYTE)packet,&l))
                        {
                            double timestamp = (double) l/27000000;
                            pcr_timestamp.Set(timestamp);
                            pcr_timestamp_acquisition_time.Set(now());
                            if(verbose)
                                std::cout << "PCR received value " << l << " - " << timestamp << " s " << std::endl;
                            

                        }
						if(GetOPCRInAdaptationField((LPBYTE)packet,&l))
                        {
                            double timestamp = (double) l/27000000;
                            opcr_timestamp.Set(timestamp);
                            opcr_timestamp_acquisition_time.Set(now());
                            if(verbose)
                                std::cout << "OPCR received value " << l << " - " << timestamp << " s " << std::endl;

                        }
						 if(GetDTSInAdaptationField((LPBYTE)packet,&l))
                        {
                            double timestamp =  (double)l/90000;
                            dts_timestamp.Set(timestamp);
                            dts_timestamp_acquisition_time.Set(now());
                            if(verbose)
                                std::cout << "DTS received value " << l << " - " << timestamp << " s " << std::endl;

                        }
						 if(GetDTSInPES((LPBYTE)packet,&l))
                        {
                            double timestamp = (double) l/90000;
                            dts_timestamp.Set(timestamp);
                            dts_timestamp_acquisition_time.Set(now());
                            if(verbose)
                                std::cout << "DTS received value " << l << " - " << timestamp << " s " << std::endl;

                        }
						 if(GetPTSInPES((LPBYTE)packet,&l))
                        {
                            double timestamp = (double) l/90000;
                            pts_timestamp.Set(timestamp);
                            pts_timestamp_acquisition_time.Set(now());
                            if(verbose)
                                std::cout << "PTS received value " << l << " - " << timestamp << " s " << std::endl;
                        }
                        if(GetT12((LPBYTE)packet,&l))
                        {
                            double timestamp = (double) l;
                            t12_timestamp.Set(timestamp);
                            t12_timestamp_acquisition_time.Set(now());
                            if(verbose)
                                std::cout << "T-12 received value " << l << " - " << timestamp << " s " << std::endl;
                        }
                    }
                    length -= PACKET_SIZE;
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
	bool verbose = false;
    std::vector<InputStream> InputStreamArray;
    
    if(verbose)
    {
        long n = now();
        std::string ns = nowstring(n);
        std::cout << "Start Time tick (nano seconds): " << n << " Start Time String : " <<ns << std::endl;
    }
  	bool result = ParseCommandLine(argc, argv, LocalPrometheusPort,InputStreamArray, verbose);
	if (result == false)
	{
		std::cout << "PCR Gauge Custom Prometheus Client : syntax error" << std::endl;
		std::cout << "Syntax:" << std::endl;
		std::cout << "PCRGauge --prometheusport \"<local TCP port used to receive Prometheus request>\" " << std::endl;
		std::cout << "         --tsport \"<local IP address and UDP port used to receive the TS stream>\" " << std::endl;
		std::cout << "        [--verbose] " << std::endl;
		std::cout << "For instance: " << std::endl;
		std::cout << "    PCRGauge --prometheusport 8008 --tsport input_stream_0:239.0.0.1:1234,127.0.0.1:5678,output_stream_0:192.168.1.5:8888" << std::endl;
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


    for(InputStream is : InputStreamArray)
    {
        if(verbose)
            std::cout << "Preparing timestamps for : " << is.address << ":" << is.port << " with prefix: " << is.prefix << std::endl;
        auto& pcr_timestamp_family =  BuildGauge()
                                    .Name(is.prefix + "_pcr_timestamp")
                                    .Help("Display PCR timestamp for "+ is.prefix )
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge&  pcr_timestamp = pcr_timestamp_family.Add(
            {{is.prefix + "_pcr_timestamp", "value"}});


        auto& pcr_timestamp_acquisition_time_family =  BuildGauge()
                                    .Name(is.prefix + "_pcr_acquisition_time")
                                    .Help("Display PCR Acquisition Time for " + is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& pcr_timestamp_acquisition_time = pcr_timestamp_acquisition_time_family.Add(
            {{is.prefix + "_pcr_timestamp_acquisition_time", "value"}});

        auto& opcr_timestamp_family =  BuildGauge()
                                    .Name(is.prefix + "_opcr_timestamp")
                                    .Help("Display OPCR timestamp for " + is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& opcr_timestamp = opcr_timestamp_family.Add(
            {{is.prefix + "_opcr_timestamp", "value"}});

        auto& opcr_timestamp_acquisition_time_family =  BuildGauge()
                                    .Name(is.prefix + "_opcr_acquisition_time")
                                    .Help("Display OPCR Acquisition Time for " + is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& opcr_timestamp_acquisition_time = opcr_timestamp_acquisition_time_family.Add(
            {{is.prefix + "_opcr_timestamp_acquisition_time", "value"}});

        auto& dts_timestamp_family =  BuildGauge()
                                    .Name(is.prefix + "_dts_timestamp")
                                    .Help("Display DTS timestamp for "+ is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& dts_timestamp = dts_timestamp_family.Add(
            {{is.prefix + "_dts_timestamp", "value"}});

        auto& dts_timestamp_acquisition_time_family =  BuildGauge()
                                    .Name(is.prefix + "_dts_acquisition_time")
                                    .Help("Display DTS Acquisition Time for "+ is.prefix )
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& dts_timestamp_acquisition_time = dts_timestamp_acquisition_time_family.Add(
            {{is.prefix + "_dts_timestamp_acquisition_time", "value"}});

        auto& pts_timestamp_family =  BuildGauge()
                                    .Name(is.prefix + "_pts_timestamp")
                                    .Help("Display PTS timestamp for "+ is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& pts_timestamp = pts_timestamp_family.Add(
            {{is.prefix + "_pts_timestamp", "value"}});

        auto& pts_timestamp_acquisition_time_family =  BuildGauge()
                                    .Name(is.prefix + "_pts_acquisition_time")
                                    .Help("Display PTS Acquisition Time for "+ is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);        
        prometheus::Gauge& pts_timestamp_acquisition_time = pts_timestamp_acquisition_time_family.Add(
            {{is.prefix + "_pts_timestamp_acquisition_time", "value"}});


        auto& t12_timestamp_family =  BuildGauge()
                                    .Name(is.prefix + "_t12_timestamp")
                                    .Help("Display T-12 timestamp for " + is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& t12_timestamp = t12_timestamp_family.Add(
            {{is.prefix + "_t12_timestamp", "value"}});


        auto& t12_timestamp_acquisition_time_family =  BuildGauge()
                                    .Name(is.prefix + "_t12_acquisition_time")
                                    .Help("Display T-12 Acquisition Time for "+ is.prefix)
                                    .Labels({{"label", "value"}})
                                    .Register(*registry);
        prometheus::Gauge& t12_timestamp_acquisition_time = t12_timestamp_acquisition_time_family.Add(
            {{is.prefix + "_t12_timestamp_acquisition_time", "value"}});
        // ask the exposer to scrape the registry on incoming scrapes
        exposer.RegisterCollectable(registry);
        if(verbose)
            std::cout << "Preparation of timestamps done for : " << is.address << ":" << is.port << " with prefix: " << is.prefix << std::endl;


        is.pThread = new  std::thread (TSThread, std::ref(pcr_timestamp), 
                                    std::ref(pcr_timestamp_acquisition_time),
                                    std::ref(opcr_timestamp), 
                                    std::ref(opcr_timestamp_acquisition_time),
                                    std::ref(dts_timestamp), 
                                    std::ref(dts_timestamp_acquisition_time),
                                    std::ref(pts_timestamp), 
                                    std::ref(pts_timestamp_acquisition_time),
                                    std::ref(t12_timestamp), 
                                    std::ref(t12_timestamp_acquisition_time),
                                    is.address, is.port, verbose);
    }

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // increment the counter by one (second)
        time_counter.Increment();
    }
  return 0;
}

