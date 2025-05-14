/*
	edited by: Osama Atta
	author: Matthew Boeding
	version: v3.0
  Adapted from: Subharthi Banerjee, Ph.D.

	README

/// The sole reason to provide this code is to make your TFTLCD (ILI9341)
/// up and running

/// Note: Most of the code is in one place. This is not ideal and may be changed
// in the future

/// Use C or inline assembly program as you please.

/// ** the code uses P0 for 8-bit interface
/// ** IOM --> P3^4
/// ** CD --> P3^5
////  I recommend leaving these definitions for UART implementation later.
////
/// ** RD --> P3^7
/// ** WR --> P3^6

/// Refer to the header file to change decoding addresses for your specific design.

/// Please do not post any of the code from this course to GITHUB.

*/


/*//  *************** IMPORTANT *********************/

/* It may need redfinition of pins like*/
/*#include <8051.h> */


#include "ecen4330lcdh.h"
#include "font.h"

/* keypad configuration*/
uint8_t keypad[4][4] =	{{'D','E','0','F'},
				{'C','9','8','7'},
				{'B','6','5','4'},
				{'A','3','2','1'} };
uint8_t colloc, rowloc;
/* store it in a variable the lcd address*/
__xdata uint8_t* lcd_address = (uint8_t __xdata*) __LCD_ADDRESS__;
__xdata uint8_t* seg7_address = (uint8_t __xdata*) __SEG_7_ADDRESS__;

unsigned int END = __END_RAM__; //65534


volatile unsigned char received_byte=0;
volatile unsigned char recieved_flag = 0;

 #define write8inline(d) {			\
 	IOM = 1;							\
	*lcd_address = d; 						\
	IOM = 0;							\
}

#define write8 write8inline
/* data write*/
#define write8DataInline(d) {	\
	CD = 1; 					\
	write8(d);					\
}
/* command or register write*/
#define write8RegInline(d) {	\
	CD = 0; 					\
	write8(d);					\
}

/* inline definitions*/
#define write8Reg write8RegInline
#define write8Data write8DataInline

uint16_t cursor_x, cursor_y;  /*// cursor_y and cursor_x globals*/
uint8_t textsize, rotation; /*// textsize and rotation*/
uint16_t
    textcolor,      /*///< 16-bit background color for print()*/
    textbgcolor;    /*///< 16-bit text color for print()*/
uint16_t
    _width,         /*///< Display width as modified by current rotation*/
    _height;        /*///< Display height as modified by current rotation*/

void ISR_receive() __interrupt (4) {
	if (RI == 1){
		//received_byte = SBUF;
		received_byte = SBUF - 0x40;
		RI = 0;
		recieved_flag= 1;

	}
}

void iowrite8(uint8_t __xdata* map_address, uint8_t d) {
	IOM = 1;
	*map_address = d;
	*map_address = d;
	*map_address = d;
	*map_address = d;
	IOM = 0;
}

void delay (int16_t d) /*// x 1ms*/
{
	int i,j;
	for (i=0;i<d;i++) /*// this is For(); loop delay used to define delay value in Embedded C*/
	{
	for (j=0;j<1000;j++);
	}
}

void writeRegister8(uint8_t a, uint8_t d) {
	/*IOM = 0;*/
	CD = __CMD__;
	write8(a);
	CD = __DATA__;
	write8(d);
	/*IOM = 1;*/
}

void writeRegister16(uint16_t a, uint16_t d){
	uint8_t hi, lo;
 	hi = (a) >> 8;
 	lo = (a);
	/*/IOM = 0;
   //	CD = 0;*/
 	write8Reg(hi);
 	write8Reg(lo);
  	hi = (d) >> 8;
  	lo = (d);
  	CD = 1 ;
  	write8Data(hi);
  	write8Data(lo);
/*/	IOM =1;*/
}

void UART_Init(){
    SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
    TMOD = 0x20;  // Timer1 in Mode2. in 8 bit auto reload
    TH1 =  0xFB;  // Load timer value for 9600 baudrate
    TR1 = 1;      // Turn ON the timer for Baud rate generation
    ES  = 1;      // Enable Serial Interrupt
    EA  = 1;      // Enable Global Interrupt bit
}


void UART_transmit(unsigned char byte){
    SBUF = byte;
    while(TI == 1);
    TI = 0;
}	


void setCursor(uint16_t x, uint16_t y){
	cursor_x = x;
	cursor_y = y;
}
/* set text color*/
void setTextColor(uint16_t x, uint16_t y){
	textcolor =  x;
	textbgcolor = y;
}

/* set text size*/
void setTextSize(uint8_t s){
	if (s > 8) return;
	textsize = (s>0) ? s : 1 ;
}

void setRotation(uint8_t flag){
	switch(flag) {
		case 0:
			flag = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
			_width = TFTWIDTH;
			_height = TFTHEIGHT;
			break;
		case 1:
			flag = (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
			_width = TFTHEIGHT;
			_height = TFTWIDTH;
			break;
		case 2:
			flag = (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
			_width = TFTWIDTH;
			_height = TFTHEIGHT;
			break;
	  case 3:
			flag = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
			_width = TFTHEIGHT;
			_height = TFTWIDTH;
			break;
		default:
			flag = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
			_width = TFTWIDTH;
			_height = TFTHEIGHT;
			break;
	}
	writeRegister8(ILI9341_MEMCONTROL, flag);
}

/* set address definition*/
void setAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	/*IOM = 0;*/
	write8Reg(0x2A);
	write8Data(x1 >> 8);
	write8Data(x1);
	write8Data(x2 >> 8);
	write8Data(x2);

	write8Reg(0x2B);
	write8Data(y1 >> 8);
	write8Data(y1);
	write8Data(y2 >> 8);
	write8Data(y2);
	/*write8Reg(0x2C);*/
    /*/IOM =1;*/


}

void TFT_LCD_INIT(void){
	/*char ID[5];*/
	/*//int id;*/
	_width = TFTWIDTH;
	_height = TFTHEIGHT;

	/*all low*/
	IOM = 1;
	/*RDN = 1;*/
	CD = 1;

	write8Reg(0x00);
	write8Data(0x00);write8Data(0x00);write8Data(0x00);
	/*IOM = 1;*/
	delay(200);

	/*IOM = 0;*/

	writeRegister8(ILI9341_SOFTRESET, 0);
    delay(50);
    writeRegister8(ILI9341_DISPLAYOFF, 0);
    delay(10);






    writeRegister8(ILI9341_POWERCONTROL1, 0x23);
    writeRegister8(ILI9341_POWERCONTROL2, 0x11);
    write8Reg(ILI9341_VCOMCONTROL1);
		write8Data(0x3d);
		write8Data(0x30);
    writeRegister8(ILI9341_VCOMCONTROL2, 0xaa);
    writeRegister8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    write8Reg(ILI9341_PIXELFORMAT);
	write8Data(0x55);write8Data(0x00);
    writeRegister16(ILI9341_FRAMECONTROL, 0x001B);

    writeRegister8(ILI9341_ENTRYMODE, 0x07);
    /* writeRegister32(ILI9341_DISPLAYFUNC, 0x0A822700);*/



    writeRegister8(ILI9341_SLEEPOUT, 0);
    delay(150);
    writeRegister8(ILI9341_DISPLAYON, 0);
    delay(500);
		setAddress(0,0,_width-1,_height-1);
     /* ************* Start Initial Sequence ILI9341 controller **********/

    IOM = 0;
}
void drawPixel(uint16_t x3,uint16_t y3,uint16_t color1)
{

	/* not using to speed up*/
	/*/if ((x3 < 0) ||(x3 >= TFTWIDTH) || (y3 < 0) || (y3 >= TFTHEIGHT))
	//{
	//	return;
	//}*/
	setAddress(x3,y3,x3+1,y3+1);

	/*/IOM = 0;*/

    CD=0; write8(0x2C);

	CD = 1;
	write8(color1>>8);write8(color1);
	/*IOM = 1;*/
}

/*draw a circle with this function*/
// void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
// 	int f = 1 - r;
//     int ddF_x = 1;
//     int ddF_y = -2 * r;
//     int x = 0;
//     int y = r;
//     IOM = 1;
//     drawPixel(x0  , y0+r, color);
//     drawPixel(x0  , y0-r, color);
//     drawPixel(x0+r, y0  , color);
//     drawPixel(x0-r, y0  , color);
//     while (x<y) {
//         if (f >= 0) {
//             y--;
//             ddF_y += 2;
//             f += ddF_y;
//         }
//         x++;
//         ddF_x += 2;
//         f += ddF_x;
//         drawPixel(x0 + x, y0 + y, color);
//         drawPixel(x0 - x, y0 + y, color);
//         drawPixel(x0 + x, y0 - y, color);
//         drawPixel(x0 - x, y0 - y, color);
//         drawPixel(x0 + y, y0 + x, color);
//         drawPixel(x0 - y, y0 + x, color);
//         drawPixel(x0 + y, y0 - x, color);
//         drawPixel(x0 - y, y0 - x, color);
//     }
//     IOM = 0;
// }

// void testCircles(uint8_t radius, uint16_t color) {
//   int  x, y, r2 = radius * 2, w = _width  + radius, h = _height + radius;
//   for(x=0; x<w; x+=r2) {
//     for(y=0; y<h; y+=r2) {
//       drawCircle(x, y, radius, color);
//     }
//   }
// }

void fillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t color){
	if ((x >= TFTWIDTH) || (y >= TFTHEIGHT))
	{
		return;
	}

	if ((x+w-1) >= TFTWIDTH)
	{
		w = TFTWIDTH-x;
	}

	if ((y+h-1) >= TFTHEIGHT)
	{
		h = TFTHEIGHT-y;
	}

	setAddress(x, y, x+w-1, y+h-1);
    /*IOM = 0;*/


	write8Reg(0x2C);
	/*IOM = 1; IOM = 0;*/
	CD = 1;
	for(y=h; y>0; y--)
	{
		for(x=w; x>0; x--)
		{

			write8(color>>8); write8(color);

		}
	}
	/*IOM = 1;*/
}

void fillScreen(uint16_t Color){
	/*uint8_t VH,VL;*/
	long len = (long)TFTWIDTH * (long)TFTHEIGHT;

	 int blocks;

   uint8_t  i, hi = Color >> 8,
              lo = Color;

    blocks = (uint16_t)(len / 64); /* 64 pixels/block*/
	setAddress(0,0,TFTWIDTH-1,TFTHEIGHT-1);

	/*IOM = 0;*/


	write8Reg(0x2C);
	/*IOM = 1; IOM = 0;*/
		CD = 1;
		write8(hi); write8(lo);

		len--;
		while(blocks--) {
      i = 16; /* 64 pixels/block / 4 pixels/pass*/
      do {

				write8(hi); write8(lo);write8(hi); write8(lo);
				write8(hi); write8(lo);write8(hi); write8(lo);
      } while(--i);
    }
    for(i = (char)len & 63; i--; ) {

      write8(hi); write8(lo);

    }

	/*IOM = 1;*/

}

void drawChar(int16_t x, int16_t y, uint8_t c,uint16_t color, uint16_t bg, uint8_t size){
	uint8_t i = 0;
	uint8_t j = 0;
	if ((x >=TFTWIDTH) || /* Clip right*/
	    (y >=TFTHEIGHT)           || /* Clip bottom*/
	    ((x + 6 * size - 1) < 0) || /* Clip left*/
	    ((y + 8 * size - 1) < 0))   /* Clip top*/
	{
		return;
	}

	for (i=0; i<6; i++ )
	{
		uint8_t line;

		if (i == 5)
		{
			line = 0x0;
		}
		else
		{
			line = pgm_read_byte(font+(c*5)+i);
		}

		for (j = 0; j<8; j++)
		{
			if (line & 0x1)
			{
				if (size == 1) /* default size*/
				{
					drawPixel(x+i, y+j, color);
				}
				else {  /* big size*/
					fillRect(x+(i*size), y+(j*size), size, size, color);
				}
			} else if (bg != color)
			{
				if (size == 1) /*default size*/
				{
					drawPixel(x+i, y+j, bg);
				}
				else
				{  /* big size*/
					fillRect(x+i*size, y+j*size, size, size, bg);
				}
			}

			line >>= 1;
		}
	}

}

void write(uint8_t c)/*write a character at setted coordinates after setting location and colour*/
{
	if (c == '\n')
	{
		cursor_y += textsize*8;
		cursor_x  = 0;
	}
	else if (c == '\r')
	{
		/* skip em*/
	}
	else
	{
		drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
		cursor_x += textsize*6;
	}
}

void LCD_string_write(int8_t *str)
{
	int16_t i;
	for(i=0;str[i]!=0;i++)	/* Send each char of string till the NULL */
	{
		write(str[i]);	/* Call transmit data function */
	}
}

// test RAM function
/*
void testRAM(unsigned char d){
	unsigned int i;
	__xdata unsigned char* ram_address;
	for (i = __START_RAM__; i<__END_RAM__; i++) {
		IOM = 0;
		ram_address = (unsigned char __xdata*)(i);
		*ram_address = d;
		IOM = 1;
	}
}
*/

void freeType() {
	uint8_t count = 0;
	uint8_t d;
	while(1){


		if (count == 8) {
			d = '\n';
			count = 0;
			write(d);
		}
		else{
			d = keyDetect();
			write(d);
		}

		count++;
	}
}

uint8_t keyDetect(){
	__KEYPAD_PORT__=0xF0;			/*set port direction as input-output*/
	do
	{
		__KEYPAD_PORT__ = 0xF0;
		colloc = __KEYPAD_PORT__;
		colloc&= 0xF0;	/* mask port for column read only */
	}while(colloc != 0xF0);		/* read status of column */

	do
	{
		do
		{
			if (recieved_flag == 1){
				recieved_flag = 0;
				return received_byte; // this will return value from ESP when keydetect is being called
			}

			delay(20);	/* 20ms key debounce time */
			colloc = (__KEYPAD_PORT__ & 0xF0);	/* read status of column */
		}while(colloc == 0xF0);	/* check for any key press */

		delay(1);
		colloc = (__KEYPAD_PORT__ & 0xF0);
	}while(colloc == 0xF0);
	while(1)
	{
		/* now check for rows */
		__KEYPAD_PORT__= 0xFE;											/* check for pressed key in 1st row */
		colloc = (__KEYPAD_PORT__ & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 0;
			break;
		}

		__KEYPAD_PORT__ = 0xFD;									/* check for pressed key in 2nd row */
		colloc = (__KEYPAD_PORT__ & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 1;
			break;
		}

	__KEYPAD_PORT__ = 0xFB;			/* check for pressed key in 3rd row */
	colloc = (__KEYPAD_PORT__ & 0xF0);
	if(colloc != 0xF0)
	{
		rowloc = 2;
		break;
	}

	__KEYPAD_PORT__ = 0xF7;			/* check for pressed key in 4th row */
	colloc = (__KEYPAD_PORT__ & 0xF0);
	if(colloc != 0xF0)
	{
		rowloc = 3;
		break;
	}
}

	if(colloc == 0xE0)
	{
		return(keypad[rowloc][0]);
	}
	else if(colloc == 0xD0)
	{
		return(keypad[rowloc][1]);
	}
	else if(colloc == 0xB0)
	{
		return(keypad[rowloc][2]);
	}
	else
	{
		return(keypad[rowloc][3]);
	}
}

// uint16_t reverse(uint8_t d) {
// 	uint16_t rev = 0;
// 	uint16_t val = 0;
// 	while(d >= 1){
// 		val = d%10;
// 		d = d/10;
// 		rev = rev * 10 + val;
// 	}
// 	return rev;
// }

// void asciiToDec(uint8_t d) {
// 	uint8_t val;
// 	uint16_t id;
// 	id = reverse(d);
// 	while (id >= 1){
// 		val = id % 10;
// 		id = id/10;
// 		write(val + '0');
// 	}
// 	write('\n');
// }

// void asciiToHex(uint8_t d) {
// 	uint8_t val;
// 	uint8_t store[2];
// 	uint8_t i =0;
// 	store[0] = 0;
// 	store[1] = 0;
// 	while (d >= 1){
// 		val = d % 16;
// 		d = d/16;
// 		if (val <= 9) {
// 			store[i] = val + '0';
// 		}
// 		else {
// 			store[i] = (val%10) + 'A';
// 		}
// 		i++;
// 	}
// 	write(store[1]);
// 	write(store[0]);
// 	/*write('\n');*/
// }

void writeSomeLines(){
	setRotation(1);
  fillScreen(BLACK);

	setTextSize(5);
	setTextColor(CYAN, BLACK);
	LCD_string_write("Welcome\n");
	setTextSize(2);
	LCD_string_write("Osama Atta\n");
	LCD_string_write("ECEN-4330\n"); //welcome screen
	delay(200);
}
/*================================================================================================================================== */
unsigned char ioread8(unsigned int __xdata* map_address) {
	unsigned char d = 0;
	IOM = 1; 
	delay(50);
	d = *map_address;
	d = *map_address;
	d = *map_address; //three times for possible delay in IOM
	IOM = 0;
	return d;
}
/*================================================================================================================================== */
void LCD_CLEAR() {
	fillScreen(BLACK);
	setCursor(0,0); 
}
/*================================================================================================================================== */
unsigned int charToDecimal(char dec){
	if (dec == '0'){
		return 0;
	}
	else if (dec == '1'){
		return 1;
	}
	else if (dec == '2'){
		return 2;
	}
	else if (dec == '3'){
		return 3;
	}
	else if (dec == '4'){
		return 4;
	}
	else if (dec == '5'){
		return 5;
	}
	else if (dec == '6'){
		return 6;
	}
	else if (dec == '7'){
		return 7;
	}
	else if (dec == '8'){
		return 8;
	}
	else if (dec == '9'){
		return 9;
	}
	else if (dec == 'A'){
		return 10;
	}
	else if (dec == 'B'){
		return 11;
	}
	else if (dec == 'C'){
		return 12;
	}
	else if (dec == 'D'){
		return 13;
	}
	else if (dec == 'E'){
		return 14;
	}
	else{
		return 15;
	}
}
/*================================================================================================================================== */
unsigned char decToChar(unsigned int in) {
	if (in == 0){
		return '0';
	}
	else if (in == 1){
		return '1';
	}
	else if (in == 2){
		return '2';
	}
	else if (in == 3){
		return '3';
	}
	else if (in == 4){
		return '4';
	}
	else if (in == 5){
		return '5';
	}
	else if (in == 6){
		return '6';
	}
	else if (in == 7){
		return '7';
	}
	else if (in == 8){
		return '8';
	}
	else if (in == 9){
		return '9';
	}
	else if (in == 10){
		return 'A';
	}
	else if (in == 11){
		return 'B';
	}
	else if (in == 12){
		return 'C';
	}
	else if (in == 13){
		return 'D';
	}
	else if (in == 14){
		return 'E';
	}
	else if(in == 15){
		return 'F';
	}
    else{
        return 1000;
    }
}
/*================================================================================================================================== */
unsigned int userByte() { //check difference between xdata and internal in execution
	char num0, num1;
	
	num0 = keyDetect(); //take in value
	write(num0); //print the value entered
	num1 = keyDetect();
	write(num1);
	LCD_string_write("\n");
	
	return ((charToDecimal(num0)*16) + (charToDecimal(num1)*1)); //hex
}
/*================================================================================================================================== */
unsigned int userWord() { //check difference between xdata and internal in execution
	 char value0, value1, value2, value3;
	 
	value0 = keyDetect(); //take in value
	write(value0); //print value entered
	value1 = keyDetect();
	write(value1);
	value2 = keyDetect();
	write(value2);
	value3 = keyDetect();
	write(value3);
	LCD_string_write("\n");
	
	return (charToDecimal(value0)*4096) + (charToDecimal(value1)*256) + (charToDecimal(value2)*16) + (charToDecimal(value3)*1); //hex
}
/*================================================================================================================================== */
void decimalToHexPrint(unsigned int decimalValue, char length){ //no array decimal to hex print function

    unsigned int divValue0, divValue1, divValue2, divValue3;

    unsigned int temp = decimalValue;

    divValue3 = temp / 4096 ;
    temp = temp % 4096;

    divValue2 = temp / 256 ;
    temp = temp % 256;

    divValue1 = temp / 16;
    temp = temp % 16 ;

    divValue0 = temp / 1 ;
    temp = temp % 1 ;

    if(length == 4){ //word
        write(decToChar(divValue3));
		write(decToChar(divValue2));
		write(decToChar(divValue1));
		write(decToChar(divValue0));
		write('h');	
    }
    else if(length == 2){ //byte
        write(decToChar(divValue1));
		write(decToChar(divValue0));
        write('h');	
    }

}
/*================================================================================================================================== */
long getBlockDataType() {
	__xdata unsigned char takenIn;
	
	do {
		LCD_CLEAR();
		__code unsigned char* ask = " Data Size?\n  1-Byte\n  2-Word\n  3-Double Word\n";
		LCD_string_write(ask);

		takenIn = keyDetect();
		write(takenIn); //print input
		delay(100);
	} while (takenIn != '1' && takenIn != '2' && takenIn != '3');	//stay in the loop until a valid selection is made

	return takenIn;
}
/*================================================================================================================================== */
long getBlockNumber(){
    __xdata unsigned int input;
    do{
        LCD_CLEAR();
        LCD_string_write(" Input block number\n");
        input = userWord();
    }while(input <= 0); //greater than zero error checking

    LCD_string_write("input value:\n");
    decimalToHexPrint(input,4); //print the value that was input to validate
    delay(200);
    return input;
}
/*================================================================================================================================== */
long getAddress(){// created for simplicity

    __xdata unsigned int input;

    input = userWord();//simply use userWord function but also print input.

    LCD_string_write("Value:\n");
    decimalToHexPrint(input,4);
    delay(200);

    return input;
}
/*================================================================================================================================== */
long getData(){
	__xdata unsigned int input;


    input = userByte();//simply use userByte function but also print input.

    LCD_string_write("Value:\n");
    decimalToHexPrint(input,2);
    delay(200);

    return input;
}
/*================================================================================================================================== */
void executeDump(volatile __xdata unsigned int address, volatile __xdata unsigned int size, volatile __xdata unsigned char type, volatile __xdata unsigned char printpg){

	volatile __xdata unsigned int *addressPointer;
	volatile __xdata unsigned int counter = 0;
	volatile __xdata unsigned int pageNumber = 1;
	volatile __xdata unsigned int pageCounter = 0;
	volatile __xdata unsigned int ramValue;
	volatile __xdata char pageSelect;	
	__code  unsigned char* pageText = "<--1  E-Exit  0-->\n"; //paging interface
	volatile __xdata unsigned int pageSelected;
	volatile __xdata unsigned int executeComplete;
	volatile __xdata unsigned int loop;
	volatile __xdata unsigned int typecount;
	
	do{ //loop for total pages
		pageSelected = 0;
		executeComplete = 0;
		pageCounter = 0;
		typecount = 0;
		
		LCD_CLEAR();
		LCD_string_write("Dump Page:");
		decimalToHexPrint(pageNumber, 4); //page number
		
		if(type == 1){
			LCD_string_write("\nDumping ");
			decimalToHexPrint(size, 4);
			LCD_string_write(" Bytes\n"); //how much stuff to be printed
		}
		else if (type == 2){
			LCD_string_write("\nDumping ");
			decimalToHexPrint(size, 4);
			LCD_string_write(" Words\n");
		}
		else if (type == 4){
			LCD_string_write("\n");
			decimalToHexPrint(size, 4);
			LCD_string_write(" Double Words\n");
		}
		
		while((pageCounter < printpg) && (counter < size)) { //loop for values in a page
			
			if(address + type >  END){
				break; //avoid overflow
			}


			decimalToHexPrint(address, 4);
			LCD_string_write(":");
			
			for(loop = 0; loop < type; loop++) //loop for data per line
			{
				addressPointer = (unsigned int __xdata*) address; 
				ramValue = *addressPointer;

				decimalToHexPrint(ramValue, 2);
				pageCounter+=1;
				
				if(address ==  END){
				break; //avoid overflow
				}
				else{
					address +=1;
				}
			}
			LCD_string_write("\n");
			delay(100);

			counter +=1;
			typecount += 1;
			if(address ==  END || counter == size ){
				
				if (address == END)
				{
					addressPointer = (unsigned int __xdata*) (0xFFFF); //for printing 0xFFFF
					ramValue = *addressPointer;
					decimalToHexPrint((unsigned int) addressPointer, 4);
					LCD_string_write(":");
					decimalToHexPrint(ramValue, 2);
					LCD_string_write("\n");

				}
				
				break;
			}
		}

		LCD_string_write(pageText); //print paging interface
	
		do {

			pageSelect = keyDetect();
			
			switch (pageSelect) {
				case '0':  //going forward
				if(counter < size && address + type <=  END){
					
					pageNumber += 1;
					pageSelected = 1;
				}
				break;
				case '1'://going back
				if(pageNumber > 1){ 
					
					address = address - (printpg + pageCounter);
					counter = counter - (10 + typecount); 
					pageNumber -= 1;
					pageSelected = 1;
				}
				break;
				case 'E': //exit
					delay(100);
					pageSelected = 1;
					executeComplete = 1;
					break; 
				
			}
		} while (pageSelected == 0);
	}while (executeComplete == 0);


}
/*================================================================================================================================== */
void setupDump(){
	iowrite8(seg7_address,0x7A);	//value is D
	delay(200);

	LCD_CLEAR();

    LCD_string_write(" DUMP");
    delay(100);

    __xdata unsigned char blockDataType = charToDecimal(getBlockDataType()); //will have decimal
    __xdata unsigned int blockNumber = getBlockNumber();
	LCD_CLEAR();
	LCD_string_write(" Input starting address\n");
    __xdata unsigned int startingAddress = getAddress();

     if(blockDataType == 1){ //simply for select the amount of values per page
		executeDump(startingAddress, blockNumber, 1, 10);
	 }
	 else if (blockDataType == 2){
	 	executeDump(startingAddress, blockNumber, 2, 20);
	 }
	 else  if (blockDataType == 3){
	 	executeDump(startingAddress, blockNumber, 4, 40);
	 }

}
/*================================================================================================================================== */
void moveExecute(volatile __xdata unsigned int address, volatile __xdata unsigned int size, volatile __xdata unsigned char type, volatile __xdata unsigned int destination){
	LCD_CLEAR();
	LCD_string_write(" Please wait...\n");

	
	volatile __xdata unsigned int *currentAddress = 0;
	volatile __xdata unsigned char ramValue = 0;


	switch (type){ //calculate proper size based on datatype
		case 2:
			size = size * 2;
		break;
		case 3:
			size = size * 4;
		break;
	}
	
	while(address <=  END && destination <=  END && size != 0) //loop for size
	{
		
		//IOM = 0;
		currentAddress = (unsigned int __xdata*) (address); //Points to source address
		ramValue = *currentAddress;  //Reading the RAM address
		
		currentAddress = (unsigned int __xdata*) (destination); //Points to destination address
		*currentAddress = ramValue;	 //Writing the RAM Address

		delay(100);
		
		address += 1;
		destination += 1;
		size -= 1;
	
	}

	LCD_CLEAR();
	LCD_string_write("Move Complete");
	delay(200);
	
}
/*================================================================================================================================== */
void setupMove(){

	iowrite8(seg7_address,0x3E);	//value is B
	delay(200);
	
	LCD_CLEAR();

    LCD_string_write(" MOVE");
    delay(100);
	LCD_CLEAR();

	volatile __xdata unsigned char blockDataType = charToDecimal(getBlockDataType()); //will have decimal value
    volatile __xdata unsigned int blockNumber = getBlockNumber();

	LCD_CLEAR();
	LCD_string_write(" Input starting address\n");
    volatile __xdata unsigned int startingAddress = getAddress(); //source address
	LCD_CLEAR();

	LCD_string_write(" Input destiation address\n");
	volatile __xdata unsigned int destinationAddress = getAddress(); //destination address
	LCD_CLEAR();

	moveExecute(startingAddress, blockNumber, blockDataType, destinationAddress);

}
/*================================================================================================================================== */
void testRAM(uint8_t d){
	 __xdata unsigned int i;
	 __xdata unsigned int weNeedToFindThis;
	 __xdata unsigned int *ram_address;

	iowrite8(seg7_address,0xEE);	//value is A
	delay(100);

	for (i = __START_RAM__; i < __END_RAM__; i++) {
		ram_address = (unsigned int __xdata*)(i);
		*ram_address = d;
		weNeedToFindThis = *ram_address;
		if (weNeedToFindThis != d ){
			LCD_CLEAR();
			LCD_string_write("Test Failed\n");
			LCD_string_write("Stage one. Address:\n");
            decimalToHexPrint(i,4);
			LCD_string_write("value recieved:\n");
			decimalToHexPrint(weNeedToFindThis,2);
			delay(200);
			return;
		}

	}

	d = ~(d); //complement

	for (i = __START_RAM__; i < __END_RAM__; i++) {
		ram_address = (unsigned int __xdata*)(i);
		*ram_address = d;

		weNeedToFindThis = *ram_address;

		if (weNeedToFindThis != d ){
			LCD_CLEAR();
			LCD_string_write("Test Failed\n");
			LCD_string_write("Stage Two. Address:\n");
            decimalToHexPrint(i,4);
			LCD_string_write("value recieved:\n");
			decimalToHexPrint(weNeedToFindThis,2);
			delay(200);
			return;
		}


	}
	

	LCD_CLEAR();
	LCD_string_write("Test Passed");
	delay(200);
	return;
}
/*================================================================================================================================== */
void executeCount(__xdata unsigned int address, __xdata unsigned int size, __xdata unsigned char type, __xdata unsigned char value){

	volatile __xdata unsigned int *currentAddress = 0;
	volatile __xdata unsigned char ramValue;
	volatile __xdata unsigned char search = 0;
	volatile __xdata unsigned int pageSelected;
	volatile __xdata unsigned int executeComplete;
	volatile __xdata unsigned int count = 0;
	volatile __xdata unsigned char dir = 1;
	volatile __xdata char ctrl;
	volatile __xdata unsigned int sizeCount = size;
	volatile __xdata unsigned int addressHold = address;

	switch (type){
		case 2:
			size = size * 2;
		break;
		case 3:
			size = size * 4;
		break;
	}


	LCD_string_write(" Searching for:");
	decimalToHexPrint(value, 2);

	delay(200);

	do{

		currentAddress = (unsigned int __xdata*) addressHold; //Points to current address
		ramValue = *currentAddress;	 //Reading the RAM address

		if(ramValue == value){
			count += 1;
		}

		addressHold += 1;
		sizeCount -= 1;

	}while((addressHold <=  END) && (sizeCount > 0));


	do{
		pageSelected = 0;
		executeComplete = 0;
		do{


			currentAddress = (unsigned int __xdata*) address; //Points to current address
			ramValue = *currentAddress;	 //Reading the RAM address


			if(ramValue == value){
				LCD_CLEAR();
				LCD_string_write("Searching ");
				decimalToHexPrint(ramValue, 2);
				write('h');
				
				LCD_string_write("\nValue Found at \nAddress: ");
				decimalToHexPrint(address, 4);

				delay(100);

				
				
				LCD_string_write("\nTotal Found: ");
				decimalToHexPrint(count, 4);

				delay(100);
				
				break;
			}

			if(dir == 1){
				address += 1;
				size -= 1;
			}
			else if (dir == 0){
				address -= 1;
				size += 1;
			}
					
		}while((address <=  END) && (size > 0) );
			
		if(count == 0){
			LCD_string_write("\nValue NOT Found!\n");
			break;
		}
		
		LCD_string_write("\n<--1  E-Exit  0-->\n");
		
		do {
			ctrl = keyDetect();
			
			switch (ctrl) {
				case '0':  //NExt Page
				if(size > 0 && address <  END){
					LCD_string_write("0 - Next Page\n");
					delay(50);
					
					dir = 1;		//Direction is onwards;
					address += 1;		//Next address
					size -= 1;		//Update the Size counter
					pageSelected = 1;
				}
				break;
				case '1':	//Previous Page
				if(count > 1){
					if(size == 0 || address ==  END){
						count += 1;
					}
					LCD_string_write("1 - Previous Page");
					delay(50);

					dir = 0;		//Direction is backwards
					address -= 1;		//Previous address
					size += 1;		//Update the Size Counter
					pageSelected = 1;
				}
				break;
				case 'E':
				pageSelected = 1;
				executeComplete = 1;
				break;
				//default:
			}
			
		} while (pageSelected == 0);
	}while (executeComplete == 0);
	

	LCD_string_write("\nCount Complete");
	
	delay(150);
}
/*================================================================================================================================== */
void setupCount(){
	iowrite8(seg7_address,0x9C);	//value is C
	delay(200);
	
	LCD_CLEAR();

    LCD_string_write(" Count");
    delay(100);
	LCD_CLEAR();

	__xdata unsigned char blockDataType = charToDecimal(getBlockDataType()); //will have decimal value
    __xdata unsigned int blockNumber = getBlockNumber();

	LCD_CLEAR();
	LCD_string_write(" Input starting address\n");
    __xdata unsigned int startingAddress = getAddress(); //starting address
	LCD_CLEAR();

	LCD_string_write(" Input value to count\n");
	__xdata unsigned int lookupValue = getData();// value to count
	LCD_CLEAR();

	executeCount(startingAddress, blockNumber, blockDataType, lookupValue);
}
/*================================================================================================================================== */
void executeFind(volatile __xdata unsigned int address, volatile __xdata unsigned int size, volatile __xdata unsigned char type, volatile __xdata unsigned char value){
	
	volatile __xdata unsigned int *currentAddress = 0;
	volatile __xdata unsigned char ramValue = 0;
	volatile __xdata unsigned char search = 0;
	volatile __xdata unsigned int pageSelected;
	volatile __xdata unsigned int executeComplete;
	volatile __xdata unsigned int count = 0;
	volatile __xdata unsigned char dir = 1;		//1 =Forward, 0 = backwards
	volatile __xdata char ctrl;


	switch (type){
		case 2:
			size = size * 2;
		break;
		case 3:
			size = size * 4;
		break;
	}


	LCD_string_write(" Searching for:");
	decimalToHexPrint(value, 2);

	delay(100);


	do{
		pageSelected = 0;
		executeComplete = 0;
		do{

			currentAddress = (unsigned int __xdata*) address; //Points to current address
			ramValue = *currentAddress;	 //Reading the RAM address
			ramValue = *currentAddress;

			if(ramValue == value){
				LCD_CLEAR();
				LCD_string_write("Searching ");
				decimalToHexPrint(ramValue, 2);
				write('h');
				
				LCD_string_write("\nValue Found at \nAddress: ");
				decimalToHexPrint(address, 4);

				delay(50);


				
				if(dir == 1){
					count += 1;
				}
				else if (dir == 0){
					count -= 1;
				}
				
				

				
				break; 
			}
			if(dir == 1){
				address += 1;
				size -= 1;
			}
			else if (dir == 0){
				address -= 1;
				size += 1;
			}
					
		}while((address <=  END) && (size > 0) );
			
		if(count == 0){
			LCD_string_write("\nValue NOT Found!\n");
			break;
		}
		
		LCD_string_write("\n<--1  E-Exit  0-->\n");
		
		do {
			ctrl = keyDetect();
			
			switch (ctrl) {
				case '0':  //Next Page
				if(size > 0 && address <  END){
					LCD_string_write("0 - Next Page\n");
					delay(50);
					
					dir = 1;		//Direction is onwards;
					address += 1;		//Next address
					size -= 1;		//Update the Size counter
					pageSelected = 1;
				}
				break;
				case '1':	//Previous Page
				if(count > 1){
					if(size == 0 || address ==  END){
						count += 1;
					}
					LCD_string_write("1 - Previous Page");
					delay(50);

					dir = 0;		//going backwards
					address -= 1;		//Previous address
					size += 1;		//Update the Size Counter
					pageSelected = 1;
				}
				break;
				case 'E':
				pageSelected = 1;
				executeComplete = 1;
				break;
			}
			
		} while (pageSelected == 0);
	}while (executeComplete == 0);
	

	LCD_string_write("\nCount Complete");
	
	delay(150);
}
/*================================================================================================================================== */
void setupFind(){

	iowrite8(seg7_address,0x8E);	//value is F
	delay(200);
	
	LCD_CLEAR();

    LCD_string_write(" FIND");
    delay(100);
	LCD_CLEAR();

	__xdata unsigned char blockDataType = charToDecimal(getBlockDataType()); //will have decimal value
    __xdata unsigned int blockNumber = getBlockNumber();

	LCD_CLEAR();
	LCD_string_write(" Input starting address\n");
    __xdata unsigned int startingAddress = getAddress(); //get starting address
	LCD_CLEAR();

	LCD_string_write(" Input value to count\n");
	__xdata unsigned int lookupValue = getData(); //get value to look for
	LCD_CLEAR();

	executeFind(startingAddress, blockNumber, blockDataType, lookupValue);
}
/*================================================================================================================================== */
void executeEdit(__xdata unsigned int address){

	volatile __xdata unsigned int *currentAddress = 0;
	volatile __xdata unsigned char ramValue = 0;
 	volatile __xdata char ctrl;
	volatile __xdata unsigned int pageSelected;
	volatile __xdata unsigned int executeComplete;

	do{
		pageSelected = 0;
		executeComplete = 0;
		
		LCD_CLEAR();
		
		LCD_string_write("\nAddress: ");
		decimalToHexPrint(address, 4); //print address we are on
		
		LCD_string_write("\nCurrent Value: ");
		currentAddress = (unsigned int __xdata*) address; //Points to current address
		ramValue = *currentAddress;                    //Reading the RAM address
		decimalToHexPrint(ramValue, 2);

		LCD_string_write("\nEnter New Value\n");
		ramValue = userByte();
		*currentAddress = ramValue;		//Editing the RAM Address
		
		LCD_string_write("\nAddress: ");
		decimalToHexPrint(address, 4);
		
		LCD_string_write("\nNew Value: ");
		currentAddress = (unsigned int __xdata*) address; //Points to current address
		ramValue = *currentAddress;                    //Reading the RAM address
		decimalToHexPrint(ramValue, 2);

	

		LCD_string_write("\n1-Exit  0-->\n");
		do {

			ctrl = keyDetect();
			
			switch (ctrl) {
				case '0':  //Next Page
				if(address < END){   //avoid Overflow
					address += 1;		//next Address
					LCD_string_write("0 - Continue");
					delay(50);
					pageSelected = 1;
					executeComplete = 0;
				}
				break;
				case '1': //exit
				LCD_string_write("1 - Exiting");
				delay(50);
				LCD_string_write("\nEdit Complete");
				delay(100);

				pageSelected = 1;
				executeComplete = 1;
				break;

			}
			
		} while (pageSelected == 0);
		
	}while(executeComplete == 0);
}
/*================================================================================================================================== */
void setupEdit(){
	
	iowrite8(seg7_address,0x9E);	//value is E
	delay(200);
	
	LCD_CLEAR();
	LCD_string_write("EDIT");
    delay(100);
	LCD_CLEAR();


	LCD_CLEAR();
	LCD_string_write("Input address\n");
    __xdata unsigned int address = getAddress(); //get address to start editing at
	LCD_CLEAR();

	executeEdit(address);
}
/*================================================================================================================================== */
void showMenu() { //menu text
	__code unsigned char* menu = " A-Ram Check\n B-Move\n C-Count\n D-Dump\n E-Edit\n F-Find\n 1-Temp & Phot\n 2-Free Type";
	LCD_string_write(menu);
}
/*================================================================================================================================== */
void mainMenu() {
	
	LCD_CLEAR();
	
	while(1) {
		LCD_CLEAR();
		showMenu();
		__xdata unsigned char menuSelection = keyDetect();
		LCD_string_write("\n");
		delay(50);
		write(menuSelection);
		delay(100);
		
		//Find what the key press was
		switch (menuSelection) {
			case 'A':
				LCD_CLEAR();
				__xdata uint8_t valueToTestRAM; 

				LCD_string_write(" Enter Byte:\n");
				valueToTestRAM = userByte();

				LCD_CLEAR();
				LCD_string_write(" Please Wait...");
				testRAM(valueToTestRAM);

				break;

			case 'B' :
				setupMove();
				break;
			
			case 'C' :
				setupCount();
				break;

			case 'D' :
				setupDump();
				break;

			case 'E':
				setupEdit();
				break;
			
			case 'F':
				setupFind();
				break;

			case '1' :
				LCD_CLEAR();
				__xdata unsigned char valueFromPhot = ioread8(__PHOT_ADDRESS__);
				valueFromPhot = ioread8(__PHOT_ADDRESS__);
				LCD_string_write(" LIGHT VALUE:\n");
				decimalToHexPrint(valueFromPhot,2);

				__xdata unsigned char valueFromTemp = ioread8((uint8_t __xdata*)(__TEMP_ADDRESS__));
				valueFromTemp = ioread8((uint8_t __xdata*)(__TEMP_ADDRESS__));
				LCD_string_write("\n\n TEMP VALUE:\n");
				decimalToHexPrint(valueFromTemp,2);
				delay(300);
				break;

			case '2':
			 LCD_string_write(" Free Type: \n");
				while(1) {	
					freeType();
				} 
				break;

			default:
				LCD_CLEAR();
				LCD_string_write(" No choice made");
				delay(100);
		}	
	}
}
/*================================================================================================================================== */
void main(void) {
	CD = 0;
	IOM = 0;
	


	IOM = 0;
	CD = 1;

	TFT_LCD_INIT();
	
	writeSomeLines();

	setRotation(1);
	LCD_CLEAR();
	fillScreen(BLACK);
	setTextColor(GREEN, BLACK);
	setCursor(0,0);

	UART_Init();

  	mainMenu();
}
