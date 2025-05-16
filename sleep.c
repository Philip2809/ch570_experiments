#define HAL_SLEEP TRUE

#include "HAL.h"
#include "SLEEP.h"
#include "RTC.h"

/*********************************************************************
 * Global variable from RTC.c
 */
extern volatile uint32_t RTCTigFlag;
extern uint32_t lsiFrq;

/*********************************************************************
 * @fn      US_TO_TICK
 *
 * @brief   Convert microseconds to RTC tick using LSI freq
 */
#define US_TO_TICK(us) ((uint32_t)((us) / (1000000.0f / lsiFrq)))

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program
 *
 * @return  none
 */
__HIGH_CODE
int main()
{
    // System Clock
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_60MHz);

    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_Floating);

    UART_DefInit();

    // --- RTC Init ---
    HAL_TimeInit();

    // --- Set RTC trigger for 5 seconds in the future ---
    uint32_t delay_seconds = 5;
    uint32_t ticks = US_TO_TICK(delay_seconds * 1000000);
    RTC_SetTignTime(R32_RTC_CNT_LSI + ticks);

    // --- Enable RTC Wakeup ---
    PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_RTC_WAKE, Fsys_Delay_4096);

    // --- Enter low power sleep ---
    mDelaymS(100);
    LowPower_Sleep(RB_PWR_EXTEND);

    while (1)
        ;
}
