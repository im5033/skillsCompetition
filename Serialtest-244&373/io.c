

// ���u����(�@21��IO) 
// +----------+----------+
// | ATMEGA8  | USB      |
// +----------+----------+
// | PD2(�T�w)| D+       |
// | PD4(�T�w)| D-       |
// +----------+----------+
// | ATMEGA8  | AD       |
// +----------+----------+
// | PB0(�T�w)| D0       |
// | PB1(�T�w)| D1       |
// | PB2(�T�w)| D2       |
// | PB3(�T�w)| D3       |
// | PB4(�T�w)| D4       |
// | PB5(�T�w)| D5       |
// | PD6(�T�w)| D6       |
// | PD7(�T�w)| D7       |
// +----------+----------+
// | ATMEGA8  | �i�ϥ�   |
// +----------+----------+
// | PC0      |          |
// | PC1      |          |
// | PC2      |          |
// | PC3      |          |
// | PC4      |          |
// | PC5      |          |
// | PC6(rst) |          |
// | PD0      |          |
// | PD1      |          |
// | PD3      |          |
// | PD5      |          |
// +----------+----------+

/* =======  ���}���w�q  =============  */
#define PORTH PORTD                                     //AD D7-D6 ,PD7-PD6 ,���D���|���
#define PORTL PORTB                                     //AD D5-D0 ,PB5-PB0 ,���D���|���

//void Init_IO(void);

//void Init_IO(void)  /* IO �]�w */  
//{
	                  // �]�wIO���}�\��- = 0 is ��J ; = 1 is ��X

//  	DDRB = 0x3f;    //    0      0      1      1      1      1      1      1				       
//  	DDRC = 0x2F;    //    0      0      1      0      1      1      1      1    
//  	DDRD = 0xeA;    //    1      1      1      0      0      0      0      0 
                   
                    //  �]�w��X���}��X
                    //  PORTB7 PORTB6 PORTB5 PORTB4 PORTB3 PORTB2 PORTB1 PORTB0
//  	PORTB = 0x0;    //    0      0      0      0      0      0      0      0
                    //    -    PORTC6 PORTC5 PORTC4 PORTC3 PORTC2 PORTC1 PORTC0							 
//    PORTC = 0x0;    //    0      0      0      0      0      0      0      0
  	                //  PORTD7 PORTD6 PORTD5 PORTD4 PORTD3 PORTD2 PORTD1 PORTD0
//  	PORTD = 0x0;    //    0      0      0      0      0      0      0      0  

   
//} 
