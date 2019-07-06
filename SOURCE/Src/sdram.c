/*
 * sdram.c - SDRAM code for T-962 reflow controller
 *
 * 
 */


#include <stdio.h>
#include <stdint.h> /* For uint32_t definition */
#include "___GLOBALS__.h"
#include "common_defs.h"
#include "stm32h7xx_hal.h"
#include "init_io.h"
#include "sdram.h"

 
 







/* local function declarations */
static void SDRAM_Initialization_sequence(uint32_t RefreshCount);






/* FMC initialization function */
void Initialize_SDRAM(void)
{
    SDRAM_HandleTypeDef* pHsdram = &g_PeriphCtrl.hsdram;
    FMC_SDRAM_TimingTypeDef SdramTiming;

    /** Perform the SDRAM2 memory initialization sequence
    */
    pHsdram->Instance = FMC_SDRAM_DEVICE;
    /* pHsdram->Init */
    pHsdram->Init.SDBank                = FMC_SDRAM_BANK2;
    pHsdram->Init.ColumnBitsNumber      = FMC_SDRAM_COLUMN_BITS_NUM_10;
    pHsdram->Init.RowBitsNumber         = FMC_SDRAM_ROW_BITS_NUM_13;
    pHsdram->Init.MemoryDataWidth       = FMC_SDRAM_MEM_BUS_WIDTH_16;
    pHsdram->Init.InternalBankNumber    = FMC_SDRAM_INTERN_BANKS_NUM_4;
    pHsdram->Init.CASLatency            = FMC_SDRAM_CAS_LATENCY_3;
    pHsdram->Init.WriteProtection       = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    pHsdram->Init.SDClockPeriod         = FMC_SDRAM_CLOCK_PERIOD_2;
    pHsdram->Init.ReadBurst             = FMC_SDRAM_RBURST_ENABLE;
    pHsdram->Init.ReadPipeDelay         = FMC_SDRAM_RPIPE_DELAY_0;
    /* Timing configuration for 100Mhz as SDRAM clock frequency (System clock is up to 200Mhz) */
    SdramTiming.LoadToActiveDelay    = 2;
    SdramTiming.ExitSelfRefreshDelay = 7;
    SdramTiming.SelfRefreshTime      = 4;
    SdramTiming.RowCycleDelay        = 7;
    SdramTiming.WriteRecoveryTime    = 2;
    SdramTiming.RPDelay              = 2;
    SdramTiming.RCDDelay             = 2;

    if (HAL_SDRAM_Init(pHsdram, &SdramTiming) != HAL_OK)
    {
        Error_Handler();
    }
    else
    {
        /* SDRAM initialization sequence */
        SDRAM_Initialization_sequence(REFRESH_COUNT);
    }
    /* return */
    return;
}
/**/
/***********************************************/



/**
  * @brief  Programs the SDRAM device.
  * @param  RefreshCount: SDRAM refresh counter value
  * @retval None
  */
static void SDRAM_Initialization_sequence(uint32_t RefreshCount)
{
    SDRAM_HandleTypeDef* pHsdram = &g_PeriphCtrl.hsdram;
    FMC_SDRAM_CommandTypeDef Command = {0};
    __IO uint32_t tmpmrd = 0;

    /* Step 1: Configure a clock configuration enable command */
    Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(pHsdram, &Command, SDRAM_TIMEOUT);

    /* Step 2: Insert 100 us minimum delay */
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    HAL_Delay(1);

    /* Step 3: Configure a PALL (precharge all) command */
    Command.CommandMode            = FMC_SDRAM_CMD_PALL;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(pHsdram, &Command, SDRAM_TIMEOUT);

    /* Step 4: Configure an Auto Refresh command */
    Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber      = 8;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(pHsdram, &Command, SDRAM_TIMEOUT);

    /* Step 5: Program the external memory mode register */
    tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |\
                       SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |\
                       SDRAM_MODEREG_CAS_LATENCY_3           |\
                       SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
                       SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand(pHsdram, &Command, SDRAM_TIMEOUT);

    /* Step 6: Set the refresh rate counter */
    /* Set the device refresh rate */
    HAL_SDRAM_ProgramRefreshRate(pHsdram, RefreshCount);
    
    /* return */
    return;
}
/**/
/**********************************************/