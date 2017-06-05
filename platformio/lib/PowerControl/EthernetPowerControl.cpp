#include "EthernetPowerControl.h"

static void write_PHY (unsigned int PhyReg, unsigned short Value) {
   /* Write a data 'Value' to PHY register 'PhyReg'. */
   unsigned int tout;
   /* Hardware MII Management for LPC176x devices. */
   LPC_EMAC->MADR = DP83848C_DEF_ADR | PhyReg;
   LPC_EMAC->MWTD = Value;

   /* Wait utill operation completed */
   for (tout = 0; tout < MII_WR_TOUT; tout++) {
      if ((LPC_EMAC->MIND & MIND_BUSY) == 0) {
         break;
      }
   }
}

static unsigned short read_PHY (unsigned int PhyReg) {
   /* Read a PHY register 'PhyReg'. */
   unsigned int tout, val;

   LPC_EMAC->MADR = DP83848C_DEF_ADR | PhyReg;
   LPC_EMAC->MCMD = MCMD_READ;

   /* Wait until operation completed */
   for (tout = 0; tout < MII_RD_TOUT; tout++) {
      if ((LPC_EMAC->MIND & MIND_BUSY) == 0) {
         break;
      }
   }
   LPC_EMAC->MCMD = 0;
   val = LPC_EMAC->MRDD;
   
   return (val);
}

void EMAC_Init()
{
  unsigned int tout,regv;
  /* Power Up the EMAC controller. */
  Peripheral_PowerUp(LPC1768_PCONP_PCENET);
  
  LPC_PINCON->PINSEL2 = 0x50150105;
  LPC_PINCON->PINSEL3 &= ~0x0000000F;
  LPC_PINCON->PINSEL3 |= 0x00000005;

  /* Reset all EMAC internal modules. */
  LPC_EMAC->MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX |
             MAC1_SIM_RES | MAC1_SOFT_RES;
  LPC_EMAC->Command = CR_REG_RES | CR_TX_RES | CR_RX_RES;

  /* A short delay after reset. */
  for (tout = 100; tout; tout--);

  /* Initialize MAC control registers. */
  LPC_EMAC->MAC1 = MAC1_PASS_ALL;
  LPC_EMAC->MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
  LPC_EMAC->MAXF = ETH_MAX_FLEN;
  LPC_EMAC->CLRT = CLRT_DEF;
  LPC_EMAC->IPGR = IPGR_DEF;

  /* Enable Reduced MII interface. */
  LPC_EMAC->Command = CR_RMII | CR_PASS_RUNT_FRM;

  /* Reset Reduced MII Logic. */
  LPC_EMAC->SUPP = SUPP_RES_RMII;
  for (tout = 100; tout; tout--);
  LPC_EMAC->SUPP = 0;

  /* Put the DP83848C in reset mode */
  write_PHY (PHY_REG_BMCR, 0x8000);

  /* Wait for hardware reset to end. */
  for (tout = 0; tout < 0x100000; tout++) {
    regv = read_PHY (PHY_REG_BMCR);
    if (!(regv & 0x8000)) {
      /* Reset complete */
      break;
    }
  }
}


void PHY_PowerDown()
{
    if (!Peripheral_GetStatus(LPC1768_PCONP_PCENET))
        EMAC_Init(); //init EMAC if it is not already init'd
    
    unsigned int regv;
    regv = read_PHY(PHY_REG_BMCR);
    write_PHY(PHY_REG_BMCR, regv | (1 << PHY_REG_BMCR_POWERDOWN));
    regv = read_PHY(PHY_REG_BMCR);
    
    //shouldn't need the EMAC now.
    Peripheral_PowerDown(LPC1768_PCONP_PCENET);
    
    //and turn off the PHY OSC
     LPC_GPIO1->FIODIR |= 0x8000000;
     LPC_GPIO1->FIOCLR = 0x8000000;
}

void PHY_PowerUp()
{   
    if (!Peripheral_GetStatus(LPC1768_PCONP_PCENET))
        EMAC_Init(); //init EMAC if it is not already init'd
        
    LPC_GPIO1->FIODIR |= 0x8000000;
    LPC_GPIO1->FIOSET = 0x8000000;
    
    //wait for osc to be stable
    wait_ms(200);
    
    unsigned int regv;
    regv = read_PHY(PHY_REG_BMCR);
    write_PHY(PHY_REG_BMCR, regv & ~(1 << PHY_REG_BMCR_POWERDOWN));
    regv = read_PHY(PHY_REG_BMCR);
}

void PHY_EnergyDetect_Enable()
{
    if (!Peripheral_GetStatus(LPC1768_PCONP_PCENET))
        EMAC_Init(); //init EMAC if it is not already init'd
        
    unsigned int regv;
    regv = read_PHY(PHY_REG_EDCR);
    write_PHY(PHY_REG_BMCR, regv | (1 << PHY_REG_EDCR_ENABLE));
    regv = read_PHY(PHY_REG_EDCR);   
}

void PHY_EnergyDetect_Disable()
{
    if (!Peripheral_GetStatus(LPC1768_PCONP_PCENET))
        EMAC_Init(); //init EMAC if it is not already init'd
    unsigned int regv;
    regv = read_PHY(PHY_REG_EDCR);
    write_PHY(PHY_REG_BMCR, regv & ~(1 << PHY_REG_EDCR_ENABLE));
    regv = read_PHY(PHY_REG_EDCR);   
}