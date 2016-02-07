
#include "io430.h"
#include "intrinsics.h"
#include "time.h"

int temp[2],check=0;
double oldn=0,newn=0,haha=0,jieguo=0,temper=0,distamce=0;
unsigned long S=0;

int rising = 0;
unsigned int autOn = 0 ;

void delay(unsigned int t)
{
  unsigned int i,j;
  for(i=0 ; i<t ; i++)
    for(j=0 ; j<10 ; j++);
}

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
////Timer/////////////
  TA0CTL = TASSEL_1 + MC_1 + ID_2 + TACLR;
  TA0CCTL1 = OUTMOD_7;
    
////signal////
  P4DIR = 0xFF; //P4.0-3 for motor //4.4-4.7 for Keypad signal
  P4OUT = 0x00;
  
  P2DIR |= BIT5;
  
  P1DIR |= BIT2 ; //P1.2 PWM
  P1SEL |= BIT2;
  
    
  TA0CCR0 = 10; //CCR0 = 10 CCR1 = 5   15微秒 trig
  TA0CCR1 = 0 ;  //0 關PWM  5 開PWM
  
  TA1CCR0 = 1;  
  unsigned int b3 = 0 ,b4 = 0 ,b5 = 0 ;
  

////Project////
  __bis_SR_register(GIE);
      
while(1)
  {

////HC-SR04////  
///////開關 P W M//////
    /*
    TA0CCR1 = 5;
    delay(1000);
    newn = clock();
    oldn = newn ;
   
///////detect/////////
   int b1_1 = 0; 
   while(!b1_1){
        b1_1 = P1IN_bit.P1IN_1; // P1.1 收echo
        newn=clock();   //當下
        jieguo=(newn-oldn)/(double)(CLOCKS_PER_SEC); 
        P2OUT |= BIT5;
       
   }
   P2OUT &= ~BIT5; //D3 off
   TA0CCR1 = 0 ;  //PWM off
   temper=jieguo;
   distamce=temper/58;
    
      if( b1_1 == 1 ){ //收到
        
    //    rising = 1;
          //D3 on
        
        newn=clock();   //當下
        jieguo=(newn-oldn)/(double)(CLOCKS_PER_SEC); 
        P2OUT |= BIT5;
//        P1OUT &= ~BIT2 ;  //Source off
        
      }else {
        
        
        P2OUT &= ~BIT5; //D3 off
      }
     */   
///////////////////////////////////////// 
    if(autOn == 1){
      //自動停車
      P4OUT |= 0x06;                    //左
      delay(7000);
      P4OUT = 0x00;
      delay(7000);
      P4OUT |= 0x05;                        //後
      delay(7000);
      P4OUT = 0x00;
      delay(50000);
      autOn=0;
    }
    else{
      //遙控車
      
      //======first row #、0、*======
      P4OUT |= 0x10;
      b3 = P1IN_bit.P1IN_3;
      b4 = P1IN_bit.P1IN_4;
      b5 = P1IN_bit.P1IN_5;
      if(P4OUT_bit.P4OUT_4==0x01&&b3==0x01)
      {
          delay(100);
      }
      else if(P4OUT_bit.P4OUT_4==0x01&&b4==0x01)
        {
             delay(100);
        }
      else if(P4OUT_bit.P4OUT_4==0x01&&b5==0x01)
        {
             delay(100);
        }else P4OUT = 0x00; 
      //======second row 9、8、7======    
       P4OUT |= 0x20;
      b3 = P1IN_bit.P1IN_3;
      b4 = P1IN_bit.P1IN_4;
      b5 = P1IN_bit.P1IN_5;
      if(P4OUT_bit.P4OUT_5==0x01&&b3==0x01)
      {
                    
            delay(100);
      }
      else if(P4OUT_bit.P4OUT_5==0x01&&b4==0x01)
        {
          P4OUT |= 0x05;                        //後
          delay(100);
          while(1)
          {
             b4 = P1IN_bit.P1IN_4;
             if(b4 == 0) break;
          }
          P4OUT = 0x00;
   
        }
      else if(P4OUT_bit.P4OUT_5==0x01&&b5==0x01)
        {
            delay(100); 
        }else P4OUT = 0x00;
      //======third row 6、5、4======
       P4OUT |= 0x40;
      b3 = P1IN_bit.P1IN_3;
      b4 = P1IN_bit.P1IN_4;
      b5 = P1IN_bit.P1IN_5;
      if(P4OUT_bit.P4OUT_6==0x01&&b3==0x01)
      {
          P4OUT |= 0x09;                    //左
          delay(100);
      }
      else if(P4OUT_bit.P4OUT_6==0x01&&b4==0x01)
        {
            autOn = 1; //按5開始路邊停車
            delay(100); 
        }
      else if(P4OUT_bit.P4OUT_6==0x01&&b5==0x01)
        {
          P4OUT |= 0x06;                    //右
          delay(100);
        }else P4OUT = 0x00;
      //======fourth row 3、2、1======
       P4OUT |= 0x80;
      b3 = P1IN_bit.P1IN_3;
      b4 = P1IN_bit.P1IN_4;
      b5 = P1IN_bit.P1IN_5;
      if(P4OUT_bit.P4OUT_7==0x01&&b3==0x01)
      {
          delay(100);
      }
      else if(P4OUT_bit.P4OUT_7==0x01&&b4==0x01)
        {
          P4OUT |= 0x0A;                    //前
          delay(100);
           while(1)
          {
             b4 = P1IN_bit.P1IN_4;
             if(b4 == 0) break;
          }
          P4OUT = 0x00;
        }
      else if(P4OUT_bit.P4OUT_7==0x01&&b5==0x01)
        {
          delay(100);
        }else P4OUT = 0x00;
      
      delay(1000);
      }
     
  }//end of while
}

/*
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0A0_ISR(void)
{
  //C計時參考網址//////////////////
  //http://edisonx.pixnet.net/blog/post/52113788-%5Bc%5D-%E8%A8%88%E6%99%82%E5%99%A8%E6%95%B4%E7%90%86
  //////////////////////////////////
  
   //Source on
    P1OUT |= BIT1 ; 
    delay(100);
    P1OUT &= ~BIT1; //LED 時註解調
    
    newn=clock();  //當下
    oldn=newn;
    
   // TA0CCTL0 &= ~MC_1;
    
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TA1A0_ISR(void)
{
    switch(__even_in_range(TAIV, 10){
    case 0:
      break;
    case TAIV_CCIFG1:
      haha++;
      if(haha>2){
        TA0CCR1=0;
        haha = 0;
      }
      P1OUT^=BIT1;      
      //test
      
      int b1_1 =P1IN_bit.P1IN_1; // P1.1 收echo
      if( b1_1 == 1 ){ //收到
        
        rising = 1;
          //D3 on
        
        newn=clock();   //當下
        jieguo=(newn-oldn)/(double)(CLOCKS_PER_SEC); 
        P2OUT |= BIT5;
        P1OUT &= ~BIT2 ;  //Source off
        
      }else {
        P2OUT &= ~BIT5; //D3 off
      }    

        break;
    case TAIV_TAIFG:
        break;
    default:
        break;
  }
}

*/