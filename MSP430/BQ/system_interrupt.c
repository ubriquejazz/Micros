/**
  ******************************************************************************
  * @file    i2c.c
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file I2C driver
  ******************************************************************************
  */

#include "i2c.h"

extern uint8_t RX_Buffer [MAX_BUFFER_SIZE];

/* ReceiveBuffer: Buffer used to receive data in the ISR
 * RXByteCtr: Number of bytes left to receive
 * ReceiveIndex: The index of the next byte to be received in ReceiveBuffer
 * TransmitBuffer: Buffer used to transmit data in the ISR
 * TXByteCtr: Number of bytes left to transfer
 * TransmitIndex: The index of the next byte to be transmitted in TransmitBuffer
 * */
uint8_t ReceiveBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t RXByteCtr = 0;
uint8_t ReceiveIndex = 0;
uint8_t TransmitBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t TXByteCtr = 0;
uint8_t TransmitIndex = 0;

/* Used to track the state of the software state machine*/
I2C_Mode MasterMode = IDLE_MODE;

/* The Register Address/Command to use*/
uint8_t TransmitRegAddr = 0;

/********************************/
/* I2C Write and Read Functions */
/********************************/

I2C_Mode I2C_Master_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    /* Initialize state machine */
    MasterMode = TX_REG_ADDRESS_MODE;
    TransmitRegAddr = reg_addr;

    //Copy register data to TransmitBuffer
    CopyArray(reg_data, TransmitBuffer, count);

    TXByteCtr = count;
    RXByteCtr = 0;
    ReceiveIndex = 0;
    TransmitIndex = 0;

    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB0IE |= UCTXIE;                        // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);              // Enter LPM0 w/ interrupts
    return MasterMode;
}

// NOTE: the received data is available in ReceiveBuffer
I2C_Mode I2C_Master_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count)
{
    /* Initialize state machine */
    MasterMode = TX_REG_ADDRESS_MODE;
    TransmitRegAddr = reg_addr;
    RXByteCtr = count;
    TXByteCtr = 0;
    ReceiveIndex = 0;
    TransmitIndex = 0;

    /* Initialize slave address and interrupts */
    UCB0I2CSA = dev_addr;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB0IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB0IE |= UCTXIE;                        // Enable TX interrupt

    UCB0CTLW0 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(LPM0_bits + GIE);              // Enter LPM0 w/ interrupts

    return MasterMode;
}

unsigned char CRC8(unsigned char *ptr, unsigned char len)
{
    unsigned char i;
    unsigned char crc=0;
    while(len--!=0)
    {
        for(i=0x80; i!=0; i/=2)
        {
            if((crc & 0x80) != 0)
            {
                crc *= 2;
                crc ^= 0x107;
            }
            else
                crc *= 2;

            if((*ptr & i)!=0)
                crc ^= 0x107;
        }
        ptr++;
    }
    return(crc);
}

void I2C_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    #if CRC_Mode
    {
        uint8_t crc_count = 0;
        crc_count = count * 2;
        uint8_t crc1stByteBuffer [3] = {0x10, reg_addr, reg_data[0]};
        unsigned int j;
        unsigned int i;
        uint8_t temp_crc_buffer [3];
        
        TX_Buffer[0] = reg_data[0];
        TX_Buffer[1] = CRC8(crc1stByteBuffer,3);

        j = 2;
        for(i=1; i<count; i++)
        {
            TX_Buffer[j] = reg_data[i];
            j = j + 1;
            temp_crc_buffer[0] = reg_data[i];
            TX_Buffer[j] = CRC8(temp_crc_buffer,1);
            j = j + 1;
        }
        I2C_Master_WriteReg(dev_addr, reg_addr, TX_Buffer, crc_count);
    }
    #else
        I2C_Master_WriteReg(dev_addr, reg_addr, reg_data, count);
    #endif
}

// NOTE: the received data is available in RX_Buffer
int I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count)
{
    unsigned int RX_CRC_Fail = 0;  // reset to 0. If in CRC Mode and CRC fails, this will be incremented.

    #if CRC_Mode
    {
        uint8_t crc_count = 0;
        crc_count = count * 2;
        unsigned int j;
        unsigned int i;
        unsigned char CRC = 0;
        uint8_t temp_crc_buffer [3];
        
        I2C_Master_ReadReg(dev_addr, reg_addr, crc_count);
        uint8_t crc1stByteBuffer [4] = {0x10, reg_addr, 0x11, ReceiveBuffer[0]};
        CRC = CRC8(crc1stByteBuffer,4);
        if (CRC != ReceiveBuffer[1])
            RX_CRC_Fail += 1;
        
        RX_Buffer[0] = ReceiveBuffer[0];
        
        j = 2; 
        for (i=1; i<count; i++)
        {
            RX_Buffer[i] = ReceiveBuffer[j];
            temp_crc_buffer[0] = ReceiveBuffer[j];
            j = j + 1;
            CRC = CRC8(temp_crc_buffer,1);
            if (CRC != ReceiveBuffer[j])
                RX_CRC_Fail += 1;
            j = j + 1;
        }
        //CopyArray(ReceiveBuffer, RX_Buffer, crc_count);
    }
    #else
        I2C_Master_ReadReg(dev_addr, reg_addr, count);
        CopyArray(ReceiveBuffer, RX_Buffer, count);
    #endif
    if (RX_CRC_Fail > 0)
        return -1;
    else
        return 0;
}

//******************************************************************************
// I2C Interrupt ***************************************************************
//******************************************************************************
// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
    P1IFG &= ~BIT4;                         // Clear P1.4 IFG
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  //Must read from UCB0RXBUF
  uint8_t rx_val = 0;
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts
    case USCI_I2C_UCALIFG:   break;         // Vector 2: ALIFG
        __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
        break;
    case USCI_I2C_UCNACKIFG:                // Vector 4: NACKIFG
        __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
        break;
    case USCI_I2C_UCSTTIFG:  break;         // Vector 6: STTIFG
    case USCI_I2C_UCSTPIFG:  break;         // Vector 8: STPIFG
    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    
    case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
        
      rx_val = UCB0RXBUF;
      if (RXByteCtr) {
        ReceiveBuffer[ReceiveIndex++] = rx_val;
        RXByteCtr--;
      }
      if (RXByteCtr == 1) {
        UCB0CTLW0 |= UCTXSTP;
      }
      else if (RXByteCtr == 0) {
        UCB0IE &= ~UCRXIE;
        MasterMode = IDLE_MODE;
        __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
      }
      break;
        
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
        
      switch (MasterMode)
      {
        case TX_REG_ADDRESS_MODE:
            UCB0TXBUF = TransmitRegAddr;
            if (RXByteCtr)
                MasterMode = SWITCH_TO_RX_MODE;   // Need to start receiving now
            else
                MasterMode = TX_DATA_MODE;        // Continue to transmision with the data in Transmit Buffer
            break;

        case SWITCH_TO_RX_MODE:
            UCB0IE |= UCRXIE;              // Enable RX interrupt
            UCB0IE &= ~UCTXIE;             // Disable TX interrupt
            UCB0CTLW0 &= ~UCTR;            // Switch to receiver
            MasterMode = RX_DATA_MODE;     // State state is to receive data
            UCB0CTLW0 |= UCTXSTT;          // Send repeated start
            if (RXByteCtr == 1) {
                //Must send stop since this is the N-1 byte
                while((UCB0CTLW0 & UCTXSTT));
                UCB0CTLW0 |= UCTXSTP;      // Send stop condition
            }
            break;

        case TX_DATA_MODE:
            if (TXByteCtr) {
                UCB0TXBUF = TransmitBuffer[TransmitIndex++];
                TXByteCtr--;
            }
            else {
                //Done with transmission
                UCB0CTLW0 |= UCTXSTP;     // Send stop condition
                MasterMode = IDLE_MODE;
                UCB0IE &= ~UCTXIE;                  // disable TX interrupt
                __bic_SR_register_on_exit(CPUOFF);  // Exit LPM0
            }
            break;

        default:
            __no_operation();
            break;
        } 
        break;  // En Vector 24

    default: 
        break;
  }
}

