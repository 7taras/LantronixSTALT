// Date: 2023-04-10
// Author: https://github.com/7taras/
// SPI Operation of W5500 chip.

// 2.2 SPI Frame 
// W5500 SPI Frame consists of 16bits Offset Address in Address Phase, 8bits Control Phase and N bytes Data Phase as shown in Figure 7. 
// The 8bits Control Phase is reconfigured with Block Select bits (BSB[4:0]), Read/Write Access Mode bit (RWB) and SPI Operation Mode (OM[1:0]). 
// Block Select bits select the block to which the Offset Address belongs. 
// W5500 supports Sequential Data Read/Write. 
// It processes the data from the base (the Offset Address which is set for 2/4/N byte Sequential data processing) 
// and the next data by increasing the Offset Address (auto increment addressing) by 1. 

// 2.2.1 Address Phase 
// This Address Phase specifies the 16 bits Offset Address for the W5500 Registers and TX/RX Buffer Blocks. 
// The 16-bit Offset Address value is transferred from MSB to LSB sequentially. 
// The SPI frame with 2/4/N byte data phase supports the Sequential Data Read/Write in which Offset address automatically increases by 1 every 1 byte data. 

// 2.2.2 Control Phase 
// The Control Phase specifies the Block to which the Offset Address (set by Address Phase) belongs, the Read/Write Access Mode and the SPI Operation Mode. 
// Данная часть пакета описывает к какому блоку памяти обращение, тип обращения - чтение или запись, и выбор режима SPI.
// 7-3 BSB	Block Select Bits 			00000: Selects Common Register
//										00001: Selects Socket 0 Register
//										00010: Selects Socket 0 TX Buffer 
//										00011: Selects Socket 0 КX Buffer 
//										00100: Reserved
//										00101: Selects Socket 1 Register
//										00110: Selects Socket 1 TX Buffer 
//										00111: Selects Socket 1 КX Buffer
// 2 RWB	Read/Write Access Mode Bit	0: Read; 1: Write
// 1-0 OM	SPI Operation Mode Bits 	00: Variable Data Length Mode, N-Bytes Data Phase (1 ≤ N)
//										01: Fixed Data Length Mode , 1 Byte Data Length (N = 1)
//										10: Fixed Data Length Mode , 2 Byte Data Length (N = 1)
//										11: Fixed Data Length Mode , 4 Byte Data Length (N = 1)
// SPI Operation Mode supports two modes, the Variable Length and the Fixed Length Data Mode. 
// 	- Variable Length Data Mode (VDM) : Data Length is controlled by SCSn. 
//		External Host makes SCSn Signal Assert (High-to-Low) the start of the SPI Frame Address Phase to W5500. 
//	- Fixed Length Data Mode (FDM) : In FDM, the Data Length is set by OM[1:0], these are not ‘00’ value. 
//		So, the SCSn signal should be Low state, and has one Length type (among 1 Bytes, 2 Bytes, 4 Bytes) according to the OM[1:0] value.
// Поддерживается 2 режима: данные произвольной длины и фиксированной (1, 2 ил 4 байта).
// Для режима произвольной длины необходимо удержание линии SCSn в "0" во время передачи данных.

// 2.2.3 Data Phase 
// With the Control Phase set by the SPI Operation Mode Bits OM[1:0], the Data Phase is set by two types of length, 
// one type is the N-Bytes length (VDM mode) and the other type is 1/2/4 Bytes (FDM mode). 
// At this time, 1 byte data is transferred through MOSI or MISO signal from MSB to LSB sequentially.

// 5.5.1 Reset Timing 
// для сброса тянется к земле на время мин 500 мс, целевое - 1 мс;
// через 1 мс внутренний сигнал PLOCK  (PLL Lock) будет установлен в HIGH;
// PLL_CLK - тактирование чипа;

// 5.5.4 SPI Timing
// FSCK, SCK Clock Frequency, max 80 MHz, целевая - 30 МГц
// Время на обработку сигнала SCSn до передачи, Tcss = 5 ns min;
// Время на обработку сигнала SCSn после передачи, Tcsh = 5 ns min;
// Время удержания сигнала SCSn в "1" по окончанию передачи, Tcs = 30 ns min;
