// Date: 2023-04-06 
// Author: https://github.com/7taras/
// Memory Organization of W5500 chip.

#pragma once

// 3 Register and Memory Organization 
// W5500 has one Common Register Block, eight Socket Register Blocks, and TX/RX Buffer Blocks allocated to each Socket. 
// Each block is selected by the BSB[4:0](Block Select Bit) of SPI Frame. 
// Figure 20 shows the selected block by the BSB[4:0] and the available offset address range of Socket TX/RX Buffer Blocks. 
// Each Socket’s TX Buffer Block exists in one 16KB TX memory physically and is initially allocated with 2KB. 
// Also, Each Socket’s RX Buffer Block exists in one 16KB RX Memory physically and is initially allocated with 2KB. 
// Regardless of the allocated size of each Socket TX/RX Buffer, it can be accessible within the 16 bits offset address range (From 0x0000 to 0xFFFF). 
// Refer to ‘Chapter 3.3’ for more information about 16KB TX/RX Memory organization and access method. 
// 
// W5500 имеет:
// - один Common Register Block.
// - восемь Socket Register Blocks - каждый 2kByte (0x0000-0xFFFF), итого 16kB.
// - восемь TX/RX Buffer Blocks к каждому Socket - каждый буфер 2kByte (0x0000-0xFFFF), итого 16kB + 16kB.
// Независимо от выделенного размера каждого буфера Socket TX/RX, он может быть доступен в пределах 16-битного диапазона адресов смещения (от 0x0000 до 0xFFFF).

// 3.1 Common Register Block 
// Common Register Block configures the general information of W5500 such as IP and MAC address. 
// This block can be selected by the BSB[4:0] value of SPI Frame. Table 3 defines the offset address of registers in this block. 
// Refer to ‘Chapter 4.1’ for more details about each register.
//
// 3.2 Socket Register Block 
// W5500 supports 8 Sockets for communication channel. Each Socket is controlled by Socket n Register Block(when 0≤n≤7). 
// The n value of Socket n Register can be selected by BSB[4:0] of SPI Frame. Table 4 defines the 16bits Offset Address of registers in Socket n Register Block. 
// Refer to ‘Chapter 4.2’ for more details about each register.
//
// 3.3 Memory 
// W5500 has one 16KB TX memory for Socket n TX Buffer Blocks and one 16KB RX memory for Socket n RX buffer Blocks. 
// 16KB TX memory is initially allocated in 2KB size for each Socket TX Buffer Block (2KB X 8 = 16KB). 
// The initial allocated 2KB size of Socket n TX Buffer can be re-allocated by using ‘Socket n TX Buffer Size Register (Sn_TXBUF_SIZE)’. 
// 0 to Socket 7. Its physical memory address is automatically determined in 16KB TX memory. 
// Therefore, the total sum of Sn_TXBUF_SIZE should be not exceed 16 in case of error in data transmission. 
// The 16KB RX memory allocation method is the same as the 16KB TX memory allocation method. 
// 16KB RX memory is initially allocated into 2KB size for each Socket RX Buffer Block (2KB X 8 = 16KB). 
// The initial allocated 2KB size of Socket n RX Buffer can be re-allocated by using ‘Socket n RX Buffer Size Register (Sn_RXBUF_SIZE)’. 
// When all Sn_RXBUF_SIZE registers have been configured, the Socket RX Buffer is allocated 
// with the configured size in 16KB RX Memory and is assigned sequentially from Socket 0 to Socket 7. 
// The physical memory address of the Socket RX Buffer is automatically determined in 16KB RX memory. 
// Therefore, the total sum of Sn_RXBUF_SIZE should not exceed 16, data reception error will occur if exceeded. 
// For 16KB TX/RX memory allocation, refer to Sn_TXBUF_SIZE & Sn_RXBUF_SIZE in ‘Chapter 4.2’.
// 
// The Socket n TX Buffer Block allocated in 16KB TX memory is buffer for saving data to be transmitted by host. 
// The 16bits Offset Address of Socket n TX Buffer Block has 64KB address space ranged from 0x0000 to 0xFFFF, 
// and it is configured with reference to ‘Socket n TX Write Pointer Register (Sn_TX_WR)’ & ‘Socket n TX Read Pointer Register(Sn_RX_RD)’. 
// However, the 16bits Offset Address automatically converts into the physical address to be accessible in 16KB TX memory such as Figure 20. 
// Refer to ‘Chapter 4.2’ for Sn_TX_WR & Sn_TX_RD.
// 
// The Socket n RX Buffer Block allocated in 16KB RX memory is buffer for saving the received data through the Ethernet. 
// The 16bits Offset Address of Socket n RX Buffer Block has 64KB address space ranged from 0x0000 to 0xFFFF, and it is configured with 
// reference to ‘Socket n RX RD Pointer Register (Sn_RX_RD)’ & ‘Socket n RX Write Pointer Register (Sn_RX_WR)’. 
// However, the 16bits Offset Address automatically converts into the physical address to be accessible in 16KB RX memory such as Figure 20.
// Refer to ‘Chapter 4.2’ for Sn_RX_RD & Sn_RX_WR.

// ------------------------------------------------------------------------
// Common Registers
// Register Notation : [Read/Write/ReadClearWrite1] [Address] [Reset value]
// ------------------------------------------------------------------------

#define W5500_MR 0x0000
#define W5500_RST 7
#define W5500_WOL 5
#define W5500_PB 4
#define W5500_PPPoE 3
#define W5500_FARP 1
// MR (Mode Register) [R/W] [0x0000] [0x00]
// MR is used for S/W reset, ping block mode and PPPoE mode
// 7 RST	If this bit is ‘1’, All internal registers will be initialized. 
//		It will be automatically cleared as ‘0’ after S/W reset. 
// 6 Reserved
// 5 WOL	Wake on LAN			0 : Disable WOL mode	1 : Enable WOL mode
// 4 PB		Ping Block Mode 	0 : Disable Ping block	1 : Enable Ping block 
//		If the bit is ‘1’, it blocks the response to a ping request.
// 3 PPPoE 	PPoE Mode 	0 : Disable PPPoE mode 1 : Enable PPPoE mode 
//		If you use ADSL, this bit should be ‘1’.
// 2 Reserved
// 1 FARP	Force ARP 0 : Disable Force ARP mode 1 : Enable Force ARP mode 
// In Force ARP mode, It forces on sending ARP Request whenever data is sent.
// 0 Reserved

#define W5500_GAR 0x0001
// GAR (Gateway IP Address Register) [R/W] [0x0001 – 0x0004] [0x00] 
// GAR configures the default gateway address.
// Example - In case of “192.168.0.1”
// 0x0001 		0x0002 		0x0003 		0x0004 
// 192 (0xC0) 	168 (0xA8) 	0 (0x00) 	1 (0x01)

#define W5500_SUBR 0x0005
// SUBR (Subnet Mask Register) [R/W] [0x0005 – 0x0008] [0x00] 
// SUBR configures the subnet mask address. 
// Example - In case of “255.255.255.0” 
// 0x0005		0x0006		0x0007		0x0008 
// 255 (0xFF)	255 (0xFF)	255 (0xFF)	0 (0x00) 

#define W5500_SHAR 0x0009
// SHAR (Source Hardware Address Register) [R/W] [0x0009 – 0x000E] [0x00] 
// SHAR configures the source hardware address. 
// Example - In case of “00.08.DC.01.02.03” 
// 0x0009	0x000A	0x000B	0x000C	0x000D	0x000E 
// 0x00		0x08	0xDC	0x01	0x02	0x03 

#define W5500_SIPR 0x000F
// SIPR (Source IP Address Register) [R/W] [0x000F – 0x0012] [0x00] 
// SIPR configures the source IP address. 
// Example - In case of “192.168.0.2” 
// 0x000F		0x0010		0x0011		0x0012 
// 192 (0xC0)	168 (0xA8)	0 (0x00) 	2 (0x02) 

#define W5500_INTLEVEL 0x0013
// INTLEVEL (Interrupt Low Level Timer Register) [R/W] [0x0013 – 0x0014] [0x0000]
// INTLEVEL configures the Interrupt Assert Wait Time (IAWT). 
// When the next interrupt occurs, Interrupt PIN (INTn ) will assert to low after INTLEVEL time. 

#define W5500_IR 0x0015
#define W5500_CONFLICT 7
#define W5500_UNREACH 6
#define W5500_IR_PPPoE 5
#define W5500_MP 4
// IR (Interrupt Register) [R/W] [0x0015] [0x00] 
// IR indicates the interrupt status. 
// Each bit of IR can be cleared when the host writes ‘1’ value to each bit. 
// If IR is not equal to ‘0x00’, INTn PIN is asserted low until it is ‘0x00’. 
// 7 CONFLICT IP Conflict. 
// 	Bit is set as ‘1’ when own source IP address is same with the sender IP address in the received ARP request. 
// 6 UNREACH 	Destination unreachable. 
// 	When receiving the ICMP (Destination port unreachable) packet, this bit is set as ‘1’. 
// 	When this bit is ‘1’, Destination Information such as IP address and Port number may be checked with the corresponding UIPR & UPORTR.
// 5 PPPoE	PPPoE Connection Close. When PPPoE is disconnected during PPPoE mode, this bit is set. 
// 4 MP		Magic Packet. When WOL mode is enabled and receives the magic packet over UDP, this bit is set. 
// 3-0 Reserved

#define W5500_IMR 0x0016
#define W5500_IM_IR7 7
#define W5500_IM_IR6 6
#define W5500_IM_IR5 5
#define W5500_IM_IR4 4
// IMR (Interrupt Mask Register) [R/W][0x0016][0x00] 
// IMR is used to mask interrupts. 
// Each bit of IMR corresponds to each bit of IR. 
// When a bit of IMR is ‘1’ and the corresponding bit of IR is ‘1’, an interrupt will be issued. 
// In other words, if a bit of IMR is ‘0’, an interrupt will not be issued even if the corresponding bit of IR is ‘1’. 
// 7 IM_IR7 IP Conflict Interrupt Mask		0: Disable; 1: Enable
// 6 Destination unreachable Interrupt Mask 	0: Disable; 1: Enable
// 5 PPPoE Close Interrupt Mask			0: Disable; 1: Enable
// 4 Magic Packet Interrupt Mask 		0: Disable; 1: Enable
// 3-0 Reserved

#define W5500_SIR 0x0017
#define W5500_S7_INT 7
#define W5500_S6_INT 6
#define W5500_S5_INT 5
#define W5500_S4_INT 4
#define W5500_S3_INT 3
#define W5500_S2_INT 2
#define W5500_S1_INT 1
#define W5500_S0_INT 0
// SIR (Socket Interrupt Register) [R/W] [0x0017] [0x00] 
// SIR indicates the interrupt status of Socket. 
// Each bit of SIR be still ‘1’ until Sn_IR is cleared by the host. 
// If Sn_IR is not equal to ‘0x00’, the n-th bit of SIR is ‘1’ and INTn PIN is asserted until SIR is ‘0x00’.
// When the interrupt of Socket n occurs, the n-th bit of SIR becomes ‘1’.

#define W5500_SIMR 0x0018
#define W5500_S7_IMR 7
#define W5500_S6_IMR 6
#define W5500_S5_IMR 5
#define W5500_S4_IMR 4
#define W5500_S3_IMR 3
#define W5500_S2_IMR 2
#define W5500_S1_IMR 1
#define W5500_S0_IMR 0
// SIMR (Socket Interrupt Mask Register) [R/W] [0x0018] [0x00] 
// Each bit of SIMR corresponds to each bit of SIR.
// When a bit of SIMR is ‘1’ and the corresponding bit of SIR is ‘1’, Interrupt will be issued. 
// In other words, if a bit of SIMR is ‘0’, an interrupt will be not issued even if the corresponding bit of SIR is ‘1’.
// 7-0 Sn_IMR Socket n(Sn_INT) Interrupt Mask 0: Disable Socket n Interrupt 1: Enable Socket n Interrupt

#define W5500_RTR 0x0019
// RTR (Retry Time-value Register) [R/W] [0x0019 – 0x001A] [0x07D0] 
// RTR configures the retransmission timeout period. 
// The unit of timeout period is 100us and the default of RTR is ‘0x07D0’ or ‘2000’. 
// And so the default timeout period is 200ms(100us X 2000). 
// During the time configured by RTR, W5500 waits for the peer response to the packet 
// that is transmitted by Sn_CR(CONNECT, DISCON, CLOSE, SEND, SEND_MAC, SEND_KEEP command). 
// If the peer does not respond within the RTR time, W5500 retransmits the packet or issues timeout. 
// Example - When timeout-period is set as 400ms, RTR = (400ms / 1ms) X 10 = 4000(0x0FA0) 
// 0x0019	0x001A 
// 0x0F		0xA0 

#define W5500_RCR 0x001B
// RCR (Retry Count Register) [R/W] [0x001B] [0x08] 
// RCR configures the number of time of retransmission. 
// When retransmission occurs as many as ‘RCR+1’, Timeout interrupt is issued (Sn_IR[TIMEOUT] = ‘1’). 
// Example - RCR = 0x0007 
// 0x001B 
// 0x07 

#define W5500_PTIMER 0x001C
// PTIMER (PPP Link Control Protocol Request Timer Register) [R/W] [0x001C] [0x0028] 
// PTIMER configures the time for sending LCP echo request. The unit of time is 25ms. 
// Example - in case that PTIMER is 200, 200 * 25(ms) = 5000(ms) = 5 seconds 

#define W5500_PMAGIC 0x001D
// PMAGIC (PPP Link Control Protocol Magic number Register) [R/W] [0x001D] [0x00] 
// PMAGIC configures the 4bytes magic number to be used in LCP echo request. 
// Example - PMAGIC = 0x01 
// 0x001D 
// 0x01 
// LCP Magic number = 0x01010101 

#define W5500_PHAR 0x001E
// PHAR (Destination Hardware Address Register in PPPoE mode) [R/W] [0x001E-0x0023] [0x0000] 
// PHAR should be written to the PPPoE server hardware address acquired in PPPoE connection process 
// Example - In case that destination hardware address is 00:08:DC:12:34:56 
// 0x001E	0x001F	0x0020	0x0021	0x0022	0x0023 
// 0x00		0x08	0xDC	0x12	0x34	0x56 

#define W5500_PSID 0x0024
// PSID (Session ID Register in PPPoE mode) [R/W] [0x0024-0x0025] [0x0000] 
// PSID should be written to the PPPoE sever session ID acquired in PPPoE connection process. 
// Example - In case that Session ID is 0x1234 
// 0x0024		0x0025 
// 18 (0x12)	52(0x34) 

#define W5500_PMRU 0x0026
// PMRU (Maximum Receive Unit in PPPoE mode) [R/W] [0x0026-0x0027] [0xFFFF] 
// PMRU configures the maximum receive unit of PPPoE. 
// Example - in case that maximum receive unit in PPPoE is 0x1234 
// 0x0026		0x0027 
// 18 (0x12)	52 (0x34) 

#define W5500_UIPR 0x0028
#define W5500_UPORTR 0x002C
// UIPR (Unreachable IP Address Register) [R] [0x0028-0x002B] [0x00000000] 
// UPORTR (Unreachable Port Register) [R] [0x002C-0x002D] [0x0000] 
// W5500 receives an ICMP packet(Destination port unreachable) when data is sent to a port number 
// which socket is not open and UNREACH bit of IR becomes ‘1’ 
// and UIPR & UPORTR indicates the destination IP address & port number respectively. 
// Example - In case of “192.168.0.11” 
// 0x0028		0x0029		0x002A		0x002B 
// 192 (0xC0)	168 (0xA8)	0 (0x00)	11 (0x0E) 
// Example - In case of “0x1234” 
// 0x002C		0x002D 
// 18 (0x12)	52(0x34) 

#define W5500_PHYCFGR 0x002E
#define W5500_PHYCFGR_RST 7
#define W5500_OPMD 6
#define W5500_OPMDC2 5
#define W5500_OPMDC1 4
#define W5500_OPMDC0 3
#define W5500_DPX 2
#define W5500_SPD 1
#define W5500_LNK 0
// PHYCFGR (W5500 PHY Configuration Register) [R/W] [0x002E] [0b10111XXX] 
// PHYCFGR configures PHY operation mode and resets PHY. 
// In addition, PHYCFGR indicates the status of PHY such as duplex, Speed, Link. 
// 7 RST Reset [R/W] When this bit is ‘0’, internal PHY is reset. After PHY reset, it should be set as ‘1’.
// 6 OPMD Configure PHY Operation Mode 1: Configure with OPMDC[2:0] in PHYCFGR 0: Configure with the H/W PINs(PMODE[2:0]) 
// 5-3 OPMDC Operation Mode Configuration Bit[R/W] These bits select the operation mode of PHY such as following table.
// 	5 4 3 Description 
// 	0 0 0 10BT Half-duplex, Auto-negotiation disabled 
// 	0 0 1 10BT Full-duplex, Auto-negotiation disabled 
// 	0 1 0 100BT Half-duplex, Auto-negotiation disabled 
// 	0 1 1 100BT Full-duplex, Auto-negotiation disabled 
// 	1 0 0 100BT Half-duplex, Auto-negotiation enabled 
// 	1 0 1 Not used 
// 	1 1 0 Power Down mode 
// 	1 1 1 All capable, Auto-negotiation enabled 
// 2 DPX Duplex Status [Read Only] 1: Full duplex 0: Half duplex 
// 1 SPD Speed Status [Read Only] 1: 100Mpbs based 0: 10Mpbs based 
// 0 LNK Link Status [Read Only] 1: Link up 0: Link down 

#define W5500_VERSIONR 0x0039
// VERSIONR (W5500 Chip Version Register) [R] [0x0039] [0x04] 
// VERSIONR always indicates the W5500 version as 0x04.

// ------------------------------------------------------------------------
// Socket Registers 
// Register Notation : [Read/Write/ReadClearWrite1] [Address] [Reset value]
// ------------------------------------------------------------------------

#define W5500_Sn_MR 0x0000
// Sn_MR (Socket n Mode Register) [R/W] [0x0000] [0x00] 
// Sn_MR configures the option or protocol type of Socket n. 

#define W5500_Sn_CR 0x0001
// Sn_CR (Socket n Command Register) [R/W] [0x0001] [0x00] 
// This is used to set the command for Socket n such as OPEN, CLOSE, CONNECT, LISTEN, SEND, and RECEIVE. 
// After W5500 accepts the command, the Sn_CR register is automatically cleared to 0x00. 
// Even though Sn_CR is cleared to 0x00, the command is still being processed. 
// To check whether the command is completed or not, please check the Sn_IR or Sn_SR. 

#define W5500_Sn_IR 0x0002
// Sn_IR (Socket n Interrupt Register) [RCW1] [0x0002] [0x00] 
// Sn_IR indicates the status of Socket Interrupt such as establishment, termination, receiving data, timeout). 
// When an interrupt occurs and the corresponding bit of Sn_IMR is ‘1’, the corresponding bit of Sn_IR becomes ‘1’. 
// In order to clear the Sn_IR bit, the host should write the bit to ‘1’.

#define W5500_Sn_SR 0x0003
// Sn_SR (Socket n Status Register) [R] [0x0003] [0x00]  
// Sn_SR indicates the status of Socket n. 
// The status of Socket n is changed by Sn_CR or some special control packet as SYN, FIN packet in TCP. 

#define W5500_Sn_PORT 0x0004
// Sn_PORT (Socket n Source Port Register) [R/W] [0x0004-0x0005] [0x0000] 
// Sn_PORT configures the source port number of Socket n. 
// It is valid when Socket n is used in TCP/UDP mode. It should be set before OPEN command is ordered. 
// Example - In case of Socket 0 Port = 5000(0x1388), configure as below, 
// 0x0004	0x0005 
// 0x13		0x88 

#define W5500_Sn_DHAR 0x0006
// Sn_DHAR (Socket n Destination Hardware Address Register) [R/W] [0x0006-0x000B] [0xFFFFFFFFFFFF] 
// Sn_DHAR configures the destination hardware address of Socket n when using SEND_MAC command in UDP mode 
// or it indicates that it is acquired in ARP-process by CONNECT/SEND command. 
// Example - In case of Socket 0 Destination Hardware address = 08.DC.00.01.02.10, configure as below. 
// 0x0006	0x0007	0x0008	0x0009	0x000A	0x000B 
// 0x08		0xDC	x00	0x01	0x02	0x0A 

#define W5500_Sn_DIPR 0x000C
// Sn_DIPR (Socket n Destination IP Address Register) [R/W] [0x000C-0x000F] [0x00000000] 
// Sn_DIPR configures or indicates the destination IP address of Socket n. 
// It is valid when Socket n is used in TCP/UDP mode. 
// In TCP client mode, it configures an IP address of ‘TCP server’ before CONNECT command. 
// In TCP server mode, it indicates an IP address of ‘TCP client’ after successfully establishing connection. 
// In UDP mode, it configures an IP address of peer to be received the UDP packet by SEND or SEND_MAC command. 
// Example - In case of Socket 0 Destination IP address = 192.168.0.11, configure as below. 
// 0x000C 		0x000D		0x000E		0x000F 
// 192 (0xC0)	168 (0xA8)	0 (0x00)	11 (0x0B) 

#define W5500_Sn_DPORT 0x0010
// Sn_DPORT (Socket n Destination Port Register) [R/W] [0x0010-0x0011] [0x00] 
// Sn_DPORT configures or indicates the destination port number of Socket n. 
// It is valid when Socket n is used in TCP/UDP mode. 
// In TCP client mode, it configures the listen port number of ‘TCP server’ before CONNECT command. 
// In TCP server mode, it indicates the port number of ‘TCP client’ after successfully establishing connection. 
// In UDP mode, it configures the port number of peer to be transmitted the UDP packet by SEND/SEND_MAC command. 
// Example - In case of Socket 0 Destination Port = 5000(0x1388), configure as below, 
// 0x0010 	0x0011 
// 0x13		0x88 

#define W5500_Sn_MSSR 0x0012
// Sn_MSSR (Socket n Maximum Segment Size Register) [R/W] [0x0012-0x0013] [0x0000] 
// This register is used for MSS (Maximum Segment Size) of TCP, 
// and the register displays MSS set by the other party when TCP is activated in Passive Mode. 
// Example - In case of Socket 0 MSS = 1460 (0x05B4), configure as below, 
// 0x0012	0x0013 
// 0x05 	0xB4 

#define W5500_Sn_TOS 0x0015
// Sn_TOS (Socket n IP Type of Service Register) [R/W] [0x0015] [0x00] 
// Sn_TOS configures the TOS(Type Of Service field in IP Header) of Socket n. It is set before OPEN command.

#define W5500_Sn_TTL 0x0016
// Sn_TTL (Socket n TTL Register) [R/W] [0x0016] [0x80] 
// Sn_TTL configures the TTL(Time To Live field in IP header) of Socket n. It is set before OPEN command.

#define W5500_Sn_RXBUF_SIZE 0x001E
// Sn_RXBUF_SIZE (Socket n RX Buffer Size Register) [R/W] [0x001E] [0x02] 
// Sn_RXBUF_SIZE configures the RX buffer block size of Socket n. 
// Socket n RX Buffer Block size can be configured with 1,2,4,8, and 16 Kbytes. 
// If a different size is configured, the data cannot be normally received from a peer. 
// Although Socket n RX Buffer Block size is initially configured to 2Kbytes, user can re-configure its size using Sn_RXBUF_SIZE. 
// The total sum of Sn_RXBUF_SIZE cannot be exceed 16Kbytes. 
// When exceeded, the data reception error is occurred. 
// When all Sn_RXBUF_SIZE have been configured, Socket n RX Buffer is allocated with 
// the configured size in 16KB RX Memory and is assigned sequentially from Socket 0 to Socket 7. 
// Socket n RX Buffer Block can be accessible with the 16bits Offset Address ranged from 
// 0x0000 to 0xFFFF regardless of the configured size. (Refer to Sn_RX_RD & Sn_RX_WR). 
// Value (dec) 	0 	1	2	4	8	16 
// Buffer size	0KB	1KB	2KB	4KB	8KB	16KB 
// Example - Socket 0 RX Buffer Size = 8KB 
// 0x001E 
// 0x08 

#define W5500_Sn_TXBUF_SIZE 0x001F
// Sn_TXBUF_SIZE (Socket n TX Buffer Size Register) [R/W] [0x001F] [0x02] 
// Sn_TXBUF_SIZE configures the TX buffer block size of Socket n. 
// Socket n TX Buffer Block size can be configured with 1,2,4,8, and 16 Kbytes. 
// If a different size is configured, the data can’t be normally transmitted to a peer. 
// Although Socket n TX Buffer Block size is initially configured to 2Kbytes, 
// user can be re-configure its size using Sn_TXBUF_SIZE. 
// The total sum of Sn_TXBUF_SIZE cannot be exceed 16Kbytes. 
// When exceeded, the data transmission error is occurred. 
// When all Sn_TXBUF_SIZE have been configured, Socket n TX Buffer is allocated 
// with the configured size in 16KB TX Memory and is assigned sequentially from Socket 0 to Socket 7. 
// Socket n TX Buffer Block can be accessible with 16bits Offset Address ranged 
// from 0x0000 to 0xFFFF regardless of the configured size. (Refer to Sn_TX_WR & Sn_TX_RD). 
// Value (dec) 	0 	1	2	4	8	16 
// Buffer size	0KB	1KB	2KB	4KB	8KB	16KB 
// Example - Socket 0 TX Buffer Size = 4KB 
// 0x001F
// 0x04

#define W5500_Sn_TX_FSR 0x0020
// Sn_TX_FSR (Socket n TX Free Size Register) [R] [0x0020-0x0021] [0x0800] 
// Sn_TX_FSR indicates the free size of Socket n TX Buffer Block. 
// It is initialized to the configured size by Sn_TXBUF_SIZE. Data bigger than Sn_TX_FSR should not be saved 
// in the Socket n TX Buffer because the bigger data overwrites the previous saved data not yet sent. 
// Therefore, check before saving the data to the Socket n TX Buffer, and 
// if data is equal or smaller than its checked size, transmit the data with 
// SEND/SEND_MAC command after saving the data in Socket n TX buffer. 
// But, if data is bigger than its checked size, transmit the data after dividing into the checked size 
// and saving in the Socket n TX buffer. 
// If Sn_MR(P[3:0]) is not TCP mode(‘0001’), it is automatically calculated as the 
// difference between ‘Socket n TX Write Pointer (Sn_TX_WR)’ and ‘Socket n TX Read Pointer (Sn_TX_RD)’. 
// If Sn_MR(P[3:0]) is TCP mode(‘0001’), it is automatically calculated as the 
// difference between Sn_TX_WR and the internal ACK pointer which indicates the point 
// of data is received already by the connected peer. 
// Example - In case of 2048(0x0800) in S0_TX_FSR, 
// 0x0020	0x0021 
// 0x08		0x00 
// Note) Because this register for representing the size information is 16 bits, it is impossible to read all bytes at the same time. 
// Before 16 bit-read operation is not completed, the value may be changed. 
// Therefore, it is recommended that you read all 16-bits twice or more until getting the same value.

#define W5500_Sn_TX_RD 0x0022
// Sn_TX_RD (Socket n TX Read Pointer Register) [R] [0x0022-0x0023] [0x0000] 
// Sn_TX_RD is initialized by OPEN command. 
// However, if Sn_MR(P[3:0]) is TCP mode(‘0001’), it is re-initialized while connecting with TCP. 
// After its initialization, it is auto-increased by SEND command. 
// SEND command transmits the saved data from the current Sn_TX_RD to the Sn_TX_WR in the Socket n TX Buffer. 
// After transmitting the saved data, the SEND command increases the Sn_TX_RD as same as the Sn_TX_WR. 
// If its increment value exceeds the maximum value 0xFFFF, (greater than 0x10000 and the carry bit occurs), 
// then the carry bit is ignored and will automatically update with the lower 16bits value. 

#define W5500_Sn_TX_WR 0x0024
// Sn_TX_WR (Socket n TX Write Pointer Register) [R/W] [0x0024-0x0025] [0x0000] 
// Sn_TX_WR is initialized by OPEN command. 
// However, if Sn_MR(P[3:0]) is TCP mode(‘0001’), it is re-initialized while connecting with TCP. 
// It should be read or to be updated like as follows. 
// 1. Read the starting address for saving the transmitting data. 
// 2. Save the transmitting data from the starting address of Socket n TX buffer.
// 3. After saving the transmitting data, update Sn_TX_WR to the increased value as many as transmitting data size. 
// If the increment value exceeds the maximum value 0xFFFF(greater than 0x10000 and the carry bit occurs), 
// then the carry bit is ignored and will automatically update with the lower 16bits value. 
// 4. Transmit the saved data in Socket n TX Buffer by using SEND/SEND command 

#define W5500_Sn_RX_RSR 0x0026
// Sn_RX_RSR (Socket n Received Size Register) [R] [0x0026-0x0027] [0x0000] 
// Sn_RX_RSR indicates the data size received and saved in Socket n RX Buffer. 
// Sn_RX_RSR does not exceed the Sn_RXBUF_SIZE and is calculated as the difference between 
// ‘Socket n RX Write Pointer (Sn_RX_WR)’ and ‘Socket n RX Read Pointer (Sn_RX_RD)’. 
// Example - In case of 2048(0x0800) in S0_RX_RSR, 
// 0x0026	0x0027 
// 0x08		0x00 
// Note) Because this register for representing the size information is 16 bits, 
// it is impossible to read all bytes at the same time. 
// Before 16 bit-read operation is not completed, the value may be changed. 
// Therefore, it is recommended that you read all 16-bits twice or more until getting the same value.

#define W5500_Sn_RX_RD 0x0028
// Sn_RX_RD (Socket n RX Read Data Pointer Register) [R/W] [0x0028-0x0029] [0x0000] 
// Sn_RX_RD is initialized by OPEN command. Make sure to be read or updated as follows. 
// 1. Read the starting save address of the received data 
// 2. Read data from the starting address of Socket n RX Buffer. 
// 3. After reading the received data, Update Sn_RX_RD to the increased value as many as the reading size. 
// If the increment value exceeds the maximum value 0xFFFF, that is, is greater than 0x10000 and the carry bit occurs, 
// update with the lower 16bits value ignored the carry bit. 
// 4. Order RECV command is for notifying the updated Sn_RX_RD to W5500. 
// Example - In case of 2048(0x0800) in S0_RX_RD, 
// 0x0028	0x0029 
// 0x08		0x00

#define W5500_Sn_RX_WR 0x002A
// Sn_RX_WR (Socket n RX Write Pointer Register) [R] [0x002A-0x002B] [0x0000] 
// Sn_RX_WR is initialized by OPEN command and it is auto-increased by the data reception. 
// If the increased value exceeds the maximum value 0xFFFF, (greater than 0x10000 and the carry bit occurs), 
// then the carry bit is ignored and will automatically update with the lower 16bits value. 
// Example - In case of 2048(0x0800) in S0_RX_WR, 
// 0x002A 	0x002B 
// 0x08		0x00 

#define W5500_Sn_IMR 0x002C
// Sn_IMR (Socket n Interrupt Mask Register) [R/W] [0x002C] [0xFF] 
// Sn_IMR masks the interrupt of Socket n. 
// Each bit corresponds to each bit of Sn_IR. 
// When a Socket n Interrupt is occurred and the corresponding bit of Sn_IMR is ‘1’, the corresponding bit of Sn_IR becomes ‘1’. 
// When both the corresponding bit of Sn_IMR and Sn_IR are ‘1’ and the n-th bit of SIR is ‘1’, Host is interrupted by asserted INTn PIN to low. 
// 7-5 Reserved
// 4 SENDOK Sn_IR(SENDOK) Interrupt Mask 
// 3 TIMEOUT Sn_IR(TIMEOUT) Interrupt Mask 
// 2 RECV Sn_IR(RECV) Interrupt Mask 
// 1 DISCON Sn_IR(DISCON) Interrupt Mask 
// 0 CON Sn_IR(CON) Interrupt Mask 

#define W5500_Sn_FRAG 0x002D
// Sn_FRAG (Socket n Fragment Register) [R/W] [0x002D-0x002E] [0x4000] 
// Sn_FRAG configures the FRAG(Fragment field in IP header).
// Example - Sn_FRAG0 = 0x0000 (Don’t Fragment) 
// 0x002D 	0x002E 
// 0x00 	0x00 

#define W5500_Sn_KPALVTR 0x002F
// Sn_KPALVTR (Socket n Keep Alive Time Register) [R/W] [0x002F] [0x00] 
// Sn_KPALVTR configures the transmitting timer of ‘KEEP ALIVE(KA)’ packet of SOCKETn. 
// It is valid only in TCP mode, and ignored in other modes. The time unit is 5s. 
// KA packet is transmittable after Sn_SR is changed to SOCK_ESTABLISHED and after 
// the data is transmitted or received to/from a peer at least once. 
// In case of 'Sn_KPALVTR > 0', W5500 automatically transmits KA packet after time-period for checking the TCP connection (Auto-keepalive-process). 
// In case of 'Sn_KPALVTR = 0', Auto-keep-alive-process will not operate, and KA packet can be transmitted by SEND_KEEP command by the host (Manual-keep-alive-process). 
// Manual-keep-alive-process is ignored in case of 'Sn_KPALVTR > 0'. 
// Example - Sn_KPALVTR = 10 (Keep Alive packet will be transmitted every 50 seconds.) 
// 0x002F 
// 0x0A 
