/* mbed PowerControl Library
  * Copyright (c) 2010 Michael Wei
  */ 
  
#ifndef MBED_POWERCONTROL_H 
#define MBED_POWERCONTROL_H 

//shouldn't have to include, but fixes weird problems with defines
#include "TARGET_LPC1768/LPC17xx.h"

//System Control Register
// bit 0: Reserved
// bit 1: Sleep on Exit
#define LPC1768_SCR_SLEEPONEXIT     0x2
// bit 2: Deep Sleep
#define LPC1768_SCR_SLEEPDEEP       0x4
// bit 3: Resereved
// bit 4: Send on Pending
#define LPC1768_SCR_SEVONPEND        0x10
// bit 5-31: Reserved

//Power Control Register
// bit 0: Power mode control bit 0 (power-down mode)
#define LPC1768_PCON_PM0            0x1
// bit 1: Power mode control bit 1 (deep power-down mode)
#define LPC1768_PCON_PM1            0x2
// bit 2: Brown-out reduced power mode
#define LPC1768_PCON_BODRPM         0x4
// bit 3: Brown-out global disable
#define LPC1768_PCON_BOGD           0x8
// bit 4: Brown-out reset disable
#define LPC1768_PCON_BORD           0x10
// bit 5-7 : Reserved
// bit 8: Sleep Mode Entry Flag
#define LPC1768_PCON_SMFLAG         0x100
// bit 9: Deep Sleep Entry Flag
#define LPC1768_PCON_DSFLAG         0x200
// bit 10: Power Down Entry Flag
#define LPC1768_PCON_PDFLAG         0x400
// bit 11: Deep Power Down Entry Flag
#define LPC1768_PCON_DPDFLAG        0x800
// bit 12-31: Reserved

//"Sleep Mode" (WFI).
inline void Sleep(void)
{
    __WFI();
}

//"Deep Sleep" Mode
inline void DeepSleep(void)
{
   SCB->SCR |= LPC1768_SCR_SLEEPDEEP;
   __WFI();
}

//"Power-Down" Mode
inline void PowerDown(void)
{
   SCB->SCR |= LPC1768_SCR_SLEEPDEEP;
   LPC_SC->PCON &= ~LPC1768_PCON_PM1;
   LPC_SC->PCON |= LPC1768_PCON_PM0;
   __WFI();
   //reset back to normal
  LPC_SC->PCON &= ~(LPC1768_PCON_PM1 | LPC1768_PCON_PM0);
}

//"Deep Power-Down" Mode
inline void DeepPowerDown(void)
{
   SCB->SCR |= LPC1768_SCR_SLEEPDEEP;
   LPC_SC->PCON |= LPC1768_PCON_PM1 | LPC1768_PCON_PM0;
   __WFI();
   //reset back to normal
  LPC_SC->PCON &= ~(LPC1768_PCON_PM1 | LPC1768_PCON_PM0);
}

//shut down BOD during power-down/deep sleep
inline void BrownOut_ReducedPowerMode_Enable(void)
{
    LPC_SC->PCON |= LPC1768_PCON_BODRPM;
}

//turn on BOD during power-down/deep sleep
inline void BrownOut_ReducedPowerMode_Disable(void)
{
    LPC_SC->PCON &= ~LPC1768_PCON_BODRPM;
}

//turn off brown out circutry
inline void BrownOut_Global_Disable(void)
{
    LPC_SC->PCON |= LPC1768_PCON_BOGD;
}

//turn on brown out circutry
inline void BrownOut_Global_Enable(void)
{
    LPC_SC->PCON &= !LPC1768_PCON_BOGD;
}

//turn off brown out reset circutry
inline void BrownOut_Reset_Disable(void)
{
    LPC_SC->PCON |= LPC1768_PCON_BORD;
}

//turn on brown outreset  circutry
inline void BrownOut_Reset_Enable(void)
{
    LPC_SC->PCON &= ~LPC1768_PCON_BORD;
}
//Peripheral Control Register
// bit 0: Reserved
// bit 1: PCTIM0: Timer/Counter 0 power/clock enable
#define LPC1768_PCONP_PCTIM0        0x2
// bit 2: PCTIM1: Timer/Counter 1 power/clock enable
#define LPC1768_PCONP_PCTIM1        0x4
// bit 3: PCUART0: UART 0 power/clock enable
#define LPC1768_PCONP_PCUART0       0x8
// bit 4: PCUART1: UART 1 power/clock enable
#define LPC1768_PCONP_PCUART1       0x10
// bit 5: Reserved
// bit 6: PCPWM1: PWM 1 power/clock enable
#define LPC1768_PCONP_PCPWM1        0x40
// bit 7: PCI2C0: I2C interface 0 power/clock enable
#define LPC1768_PCONP_PCI2C0        0x80
// bit 8: PCSPI: SPI interface power/clock enable
#define LPC1768_PCONP_PCSPI         0x100
// bit 9: PCRTC: RTC power/clock enable
#define LPC1768_PCONP_PCRTC         0x200
// bit 10: PCSSP1: SSP interface 1 power/clock enable
#define LPC1768_PCONP_PCSSP1        0x400
// bit 11: Reserved
// bit 12: PCADC: A/D converter power/clock enable
#define LPC1768_PCONP_PCADC         0x1000
// bit 13: PCCAN1: CAN controller 1 power/clock enable
#define LPC1768_PCONP_PCCAN1        0x2000
// bit 14: PCCAN2: CAN controller 2 power/clock enable
#define LPC1768_PCONP_PCCAN2        0x4000
// bit 15: PCGPIO: GPIOs power/clock enable
#define LPC1768_PCONP_PCGPIO        0x8000
// bit 16: PCRIT: Repetitive interrupt timer power/clock enable
#define LPC1768_PCONP_PCRIT         0x10000
// bit 17: PCMCPWM: Motor control PWM power/clock enable
#define LPC1768_PCONP_PCMCPWM       0x20000
// bit 18: PCQEI: Quadrature encoder interface power/clock enable
#define LPC1768_PCONP_PCQEI         0x40000
// bit 19: PCI2C1: I2C interface 1 power/clock enable
#define LPC1768_PCONP_PCI2C1        0x80000
// bit 20: Reserved
// bit 21: PCSSP0: SSP interface 0 power/clock enable
#define LPC1768_PCONP_PCSSP0        0x200000
// bit 22: PCTIM2: Timer 2 power/clock enable
#define LPC1768_PCONP_PCTIM2        0x400000
// bit 23: PCTIM3: Timer 3 power/clock enable
#define LPC1768_PCONP_PCQTIM3       0x800000
// bit 24: PCUART2: UART 2 power/clock enable
#define LPC1768_PCONP_PCUART2       0x1000000
// bit 25: PCUART3: UART 3 power/clock enable
#define LPC1768_PCONP_PCUART3       0x2000000
// bit 26: PCI2C2: I2C interface 2 power/clock enable
#define LPC1768_PCONP_PCI2C2        0x4000000
// bit 27: PCI2S: I2S interface power/clock enable
#define LPC1768_PCONP_PCI2S         0x8000000
// bit 28: Reserved
// bit 29: PCGPDMA: GP DMA function power/clock enable
#define LPC1768_PCONP_PCGPDMA       0x20000000
// bit 30: PCENET: Ethernet block power/clock enable
#define LPC1768_PCONP_PCENET        0x40000000
// bit 31: PCUSB: USB interface power/clock enable
#define LPC1768_PCONP_PCUSB         0x80000000

//Powers Up specified Peripheral(s)
inline unsigned int Peripheral_PowerUp(unsigned int bitMask)
{
    return LPC_SC->PCONP |= bitMask;
}   

//Powers Down specified Peripheral(s) 
inline unsigned int Peripheral_PowerDown(unsigned int bitMask)
{
    return LPC_SC->PCONP &= ~bitMask;
}

//returns if the peripheral is on or off
inline bool Peripheral_GetStatus(unsigned int peripheral)
{
    return (LPC_SC->PCONP & peripheral) ? true : false;
}

#endif