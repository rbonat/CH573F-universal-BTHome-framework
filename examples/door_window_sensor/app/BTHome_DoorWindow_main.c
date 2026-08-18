#include "CONFIG.h"
#include "HAL.h"
#include "gattprofile.h"
#include "BTHome_DoorWindow.h"

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x04, 0x02, 0x04};
#endif

__attribute__((section(".highcode")))
__attribute__((noinline))
void Main_Circulation(void)
{
    while(1)
    {
        BTHome_DoorWindow_Poll();
        TMOS_SystemProcess();
        BTHome_DoorWindow_SleepIfIdle();
    }
}

int main(void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All & ~SENSOR_PIN, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif
    CH57X_BLEInit();
    HAL_Init();
    HAL_TimeInit();
    GAPRole_PeripheralInit();
    BTHome_DoorWindow_Init();
    Main_Circulation();
}
