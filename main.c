
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

#include "fonctionMoteur.h"
#include <stdlib.h>

#define CANTX_PIN   19
#define CANRX_PIN   18
#define button 14
#define button2 15

int flag=1;
int k_flag;

interrupt void xint1_isr(void);
interrupt void xint2_isr(void);

__interrupt void canaISR(void);
void scia_echoback_init(void);
void scia_fifo_init(void);
void scia_xmit(int a);
void scia_msg(char *msg);

void ConfigureADC(void);
void SetupADCSoftware(void);

//
// Globals
//
Uint16 AdcaResult0;
Uint16 AdcaResult1;
Uint16 AdcbResult0;

int k_adc;
int k_adc1;
int k_adc2;
int k_adc3;

composanteMoteur M1,M2,M12;
Uint16 LoopCount;
Uint16 index;
Uint16 check;
int32 vitess;
int NumCommand;
int TypeCommand;
int32 Valeur;

int k;
int number;
long string_to_number;

void main(void) {

    //Uint16 ReceivedChar;
    char ReceivedChar;
    char *msg;
    char *mybuffer;

    InitSysCtrl();
    InitGpio();
//-----------------CAN--------------------
    GPIO_SetupPinMux(CANTX_PIN,GPIO_MUX_CPU1,3);
    GPIO_SetupPinOptions(CANTX_PIN,GPIO_OUTPUT,GPIO_PUSHPULL);

    GPIO_SetupPinMux(CANRX_PIN,GPIO_MUX_CPU1,3);
    GPIO_SetupPinOptions(CANRX_PIN,GPIO_INPUT,GPIO_ASYNC);

//---------------LED----------------
    GPIO_SetupPinMux(12,GPIO_MUX_CPU1,0);
    GPIO_SetupPinOptions(12,GPIO_OUTPUT,GPIO_ASYNC);

    GPIO_SetupPinMux(13,GPIO_MUX_CPU1,0);
    GPIO_SetupPinOptions(13,GPIO_OUTPUT,GPIO_ASYNC);


//--------USB Cable: SCI-A----------
    GPIO_SetupPinMux(84, GPIO_MUX_CPU1, 5); //28
    GPIO_SetupPinOptions(84, GPIO_INPUT, GPIO_PUSHPULL); //28
    GPIO_SetupPinMux(85, GPIO_MUX_CPU1, 5); //29
    GPIO_SetupPinOptions(85, GPIO_OUTPUT, GPIO_ASYNC); //29

// ------- button ------
    GPIO_SetupPinMux(button,GPIO_MUX_CPU1,0);
    GPIO_SetupPinOptions(button,GPIO_INPUT,GPIO_ASYNC);

    GPIO_SetupPinMux(button2,GPIO_MUX_CPU1,0);
    GPIO_SetupPinOptions(button2,GPIO_INPUT,GPIO_ASYNC);

    CANInit(CANA_BASE);
    CANClkSourceSelect(CANA_BASE,0);
    CANBitRateSet(CANA_BASE,200000000,1000000); // il faut que la fréquence du Micro soit 2* plus grande que celle configuré
    CANIntEnable(CANA_BASE,  CAN_INT_ERROR );

// -------------- PICK ------------------------
    GPIO_SetupPinMux(66,GPIO_MUX_CPU1,0);
    GPIO_SetupPinOptions(66,GPIO_OUTPUT,GPIO_ASYNC);

        DINT;
        InitPieCtrl();

        IER=0x0000;
        IFR=0x0000;

        InitPieVectTable();
        scia_fifo_init();       // Initialize the SCI-A FIFO
        scia_echoback_init();   // Initialize SCI-A for echoback

        EALLOW;
        PieVectTable.CANA0_INT=canaISR;

        PieVectTable.XINT1_INT = xint1_isr; // xint1
        PieVectTable.XINT2_INT = xint2_isr; // xint 2

        EDIS;

        PieCtrlRegs.PIEIER9.bit.INTx5=1;
        PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
        PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Gropu 1 INT4
        PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Gropu 1 INT5


        IER|=M_INT9;
        IER |= M_INT1;

        GPIO_SetupXINT1Gpio(14);
        GPIO_SetupXINT2Gpio(15);

        XintRegs.XINT1CR.bit.POLARITY = 0;          // 0 is Falling edge interrupt
        XintRegs.XINT2CR.bit.POLARITY = 0;          // Falling edge interrupt

        XintRegs.XINT1CR.bit.ENABLE = 1;            // Enable XINT1
        XintRegs.XINT2CR.bit.ENABLE = 1;            // Enable XINT2



        EINT;

        CANGlobalIntEnable(CANA_BASE, CAN_GLB_INT_CANINT0);

        ERTM;
        CANEnable(CANA_BASE);
    /***************************************************************************/
        initMsgRetour();

        ConfigurationMoteur(IDMoteur1,&M1,2);

        ConfigurationMoteur(IDMoteur2,&M2,3);
        ConfigurationMoteur(IDMoteur1_Moteur2,&M12,4);

      //  ConfigurationMoteur(IDMoteur1,&M1,2);
        SAP(&M1,MaximumPositionSpeed,1000);
        SAP(&M1,MicroStepsResolution, 5);
        SAP(&M1,MaxAcceleration, 500);

        SAP(&M2,MaximumPositionSpeed,1000);
        SAP(&M2,MicroStepsResolution, 2);
        SAP(&M2,MaxAcceleration, 500);

        DELAY_US(500);

        GPIO_WritePin(12,1);
        GPIO_WritePin(13,1);
        GPIO_WritePin(14,1);
        GPIO_WritePin(15,1);

        DELAY_US(10);


        msg = "\r\n\n\nHello World!\0";
        scia_msg(msg);

        DELAY_US(500);


        // test CAN communication
           int i = VerificationCommande(1);
           if (i==1)
             {
              GPIO_WritePin(12,0);
             }
             else
              GPIO_WritePin(12,1);
           //
           //Configure the ADCs and power them up
           //
           ConfigureADC();
           //
           //Setup the ADCs for software conversions
           //
           SetupADCSoftware();

           GPIO_WritePin(66,0);

           k_adc = 1;
           k_adc1 = 1;
           k_adc2 = 1;
           k_adc3 = 1;

           LoopCount = 0;

    for(;;){

        k_flag = flag;

        while (flag == 1)
        {
        while(SciaRegs.SCIFFRX.bit.RXFFST == 0) {

        AdcaRegs.ADCSOCFRC1.all = 0x0003; //SOC0 and SOC1
        AdcbRegs.ADCSOCFRC1.all = 0x0003; //SOC0 and SOC1

      //  GPIO_WritePin(69,0);

        //
        //wait for ADCA to complete, then acknowledge flag
        //
        while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

        while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0);
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
        DELAY_US(50);
        // sensor above 4000
        // sensor left 3200
        //
        //store results
        //
        AdcaResult0 = AdcaResultRegs.ADCRESULT0;
        AdcaResult1 = AdcaResultRegs.ADCRESULT1;
        AdcbResult0 = AdcbResultRegs.ADCRESULT1;


        if ((AdcbResult0 > 2200) && (k_adc2 == 1)) {
            MST(&M12); //2480
            k_adc2 = 0;
        }

        if ((AdcaResult1 > 2900) && (k_adc1 == 1)) {
            MST(&M12); //3200
            k_adc1 = 0;
        }

        if ((AdcaResult0 > 2950) && (k_adc == 1)) {
            MST(&M12); // 2950
            k_adc=0;
        }
        if ((AdcaResult0 < 650) && (k_adc3 == 1)) // 2950
                  {
            MST(&M12); // 2950
            k_adc3=0;
                  }


        } // wait for empty state

            //
            // Get character
            //
            ReceivedChar = SciaRegs.SCIRXBUF.all;
            if (ReceivedChar == 'S') check=1;
            else if(ReceivedChar == 'R') check=2;
            else if(ReceivedChar == 'T') check =3;
            else if(ReceivedChar == 'P') check =4;
            else if(ReceivedChar == 'Q') check =5;
            else if(ReceivedChar == 'H') check =6;
            else if(ReceivedChar == 'L') check =7;

            if(check==4) { GPIO_WritePin(66,1); }
            if(check==5) { GPIO_WritePin(66,0); }

            if (check == 6)
            {
                char Ky_tu=ReceivedChar;
                if(Ky_tu!='H'&& Ky_tu!='!')
                {
                    mybuffer[index]=Ky_tu;
                    index++;
                }
                if (Ky_tu=='!')
                   {
                                  /* int i=0;
                                   check=0;
                                   msg = "You sent: \0";
                                   scia_msg(msg);
                                   scia_msg(mybuffer);
                                   scia_msg("\r\n");
                                   for (i=0; i<index;i++){mybuffer[i]=NULL;}
                                   index=0;*/
                    int i=0;
                    check=0;
                                  // msg = "You sent: \0";
                                   //scib_msg(msg);

                    string_to_number = atoll(mybuffer);
                    NumCommand = string_to_number/10000000;
                    TypeCommand = (string_to_number - NumCommand*10000000)/10000;
                    Valeur = (string_to_number - NumCommand*10000000)-TypeCommand*10000;
                    scia_msg(mybuffer);
                    scia_msg("\0");

                    CANRTdata(&M1,NumCommand,TypeCommand,0,Valeur);

                    for (i=0; i<index;i++){mybuffer[i]=NULL;}
                    k=index;
                    index=0;

                    }
            }


            if (check==1)
            {
                char Ky_tu=ReceivedChar;
                if(Ky_tu!='S'&& Ky_tu!='!')
                {
                    mybuffer[index]=Ky_tu;
                    index++;
                }
                if (Ky_tu=='!')
                   {
                                  /* int i=0;
                                   check=0;
                                   msg = "You sent: \0";
                                   scia_msg(msg);
                                   scia_msg(mybuffer);
                                   scia_msg("\r\n");
                                   for (i=0; i<index;i++){mybuffer[i]=NULL;}
                                   index=0;*/
                    int i=0;
                    check=0;
                                  // msg = "You sent: \0";
                                   //scib_msg(msg);

                    string_to_number = atoll(mybuffer);
                    number = string_to_number/10000;
                    vitess = string_to_number - number*10000;
                    scia_msg(mybuffer);
                    scia_msg("\0");

                    CANRTdata(&M1,number,0,0,vitess);

                    for (i=0; i<index;i++){mybuffer[i]=NULL;}
                    k=index;
                    index=0;

                    }
            }
            if (check==2)
                        {
                            char Ky_tu=ReceivedChar;
                            if(Ky_tu!='R'&& Ky_tu!='!')
                            {
                                mybuffer[index]=Ky_tu;
                                index++;
                            }
                            if (Ky_tu=='!')
                               {
                                              /* int i=0;
                                               check=0;
                                               msg = "You sent: \0";
                                               scia_msg(msg);
                                               scia_msg(mybuffer);
                                               scia_msg("\r\n");
                                               for (i=0; i<index;i++){mybuffer[i]=NULL;}
                                               index=0;*/
                                int i=0;
                                check=0;
                                              // msg = "You sent: \0";
                                               //scib_msg(msg);

                                string_to_number = atoll(mybuffer);
                                number = string_to_number/10000;
                                vitess = string_to_number - number*10000;
                                scia_msg(mybuffer);
                                scia_msg("\0");

                               // TQL(&M2,number,vitess);
                                CANRTdata(&M2,number,0,0,vitess);
                                for (i=0; i<index;i++){mybuffer[i]=NULL;}
                                k=index;
                                index=0;

                                }
        }
            if (check==3)
                       {
                           char Ky_tu=ReceivedChar;
                           if(Ky_tu!='T'&& Ky_tu!='!')
                           {
                               mybuffer[index]=Ky_tu;
                               index++;
                           }
                           if (Ky_tu=='!')
                              {
                                             /* int i=0;
                                              check=0;
                                              msg = "You sent: \0";
                                              scia_msg(msg);
                                              scia_msg(mybuffer);
                                              scia_msg("\r\n");
                                              for (i=0; i<index;i++){mybuffer[i]=NULL;}
                                              index=0;*/
                               int i=0;
                               check=0;
                                             // msg = "You sent: \0";
                                              //scib_msg(msg);

                               string_to_number = atoll(mybuffer);
                               number = string_to_number/10000;
                               vitess = string_to_number - number*10000;
                               scia_msg(mybuffer);
                               scia_msg("\0");

                               CANRTdata(&M12,number,0,0,vitess);

                               for (i=0; i<index;i++){mybuffer[i]=NULL;}
                               k=index;
                               index=0;

                               }
                       }

            if(AdcbResult0 < 1700) k_adc2 = 1;
            if(AdcaResult1 < 2400) k_adc1 = 1;
            if(AdcaResult0 < 2500) k_adc = 1;
            if(AdcaResult0 > 1500)  k_adc3 = 1;
        } // while




    }//for

}//void main


__interrupt void canaISR(void)
{

    CANGlobalIntClear(CANA_BASE, CAN_GLB_INT_CANINT0);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

}


//
//  scia_echoback_init - Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F,
//                       default, 1 STOP bit, no parity
//
void scia_echoback_init()
{
    //
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    //

    SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
                                    // No parity,8 char bits,
                                    // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;  // enable TX, RX, internal SCICLK,
                                    // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.
    //
    //01 44
    SciaRegs.SCIHBAUD.all = 0x0000;
    SciaRegs.SCILBAUD.all = 0x00D8;

    SciaRegs.SCICTL1.all = 0x0023;  // Relinquish SCI from Reset
}


//
// scia_xmit - Transmit a character from the SCI
//
void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    SciaRegs.SCITXBUF.all =a;
}


void scia_msg(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}


//
// scia_fifo_init - Initialize the SCI FIFO
//
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all = 0xE040;
    SciaRegs.SCIFFRX.all = 0x2044;
    SciaRegs.SCIFFCT.all = 0x0;
}


//
// ConfigureADC - Write ADC configurations and power up the ADC for both
//                ADC A and ADC B
//
void ConfigureADC(void)
{
    EALLOW;

    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcbRegs.ADCCTL2.bit.PRESCALE = 5; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    //
    //Set pulse positions to late
    //
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //
    //power up the ADCs
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //
    //delay for 1ms to allow ADC time to power up
    //
    DELAY_US(1000);

    EDIS;
}


//
// SetupADCSoftware - Setup ADC channels and acquisition window
//
void SetupADCSoftware(void)
{
    Uint16 acqps;

    //
    //determine minimum acquisition window (in SYSCLKS) based on resolution
    //
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }

    //
    //Select the channels to convert and end of conversion flag
    //ADCA
    //
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is acqps +
                                           //1 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;  //SOC1 will convert pin A1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps +
                                           //1 SYSCLK cycles
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1; //end of SOC1 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

   // EDIS;

    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin B0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is acqps +
                                           //1 SYSCLK cycles
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;  //SOC1 will convert pin B1
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps +
                                           //1 SYSCLK cycles
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 1; //end of SOC1 will set INT1 flag
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    EDIS;



}

//
// End of file
//

interrupt void xint1_isr(void)
   {
       while(GPIO_ReadPin(14)==0){}

        flag = 0;

       GPIO_WritePin(13,0);

       // Acknowledge this interrupt to get more from group 1
       //
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
       //TempX1Count = Xint1Count;
        DELAY_US(10);
   }

interrupt void xint2_isr(void)
       {
           while(GPIO_ReadPin(15)==0){}

           flag = 1;

           GPIO_WritePin(13,1);

           // Acknowledge this interrupt to get more from group 1
           //
            PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
           //TempX1Count = Xint1Count;
            DELAY_US(10);
       }
