#include "CH57x_common.h"

__HIGH_CODE
int main()
{
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_60MHz);
    GPIOA_SetBits(bTXD_0);
    GPIOA_ModeCfg(bRXD_0, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(bTXD_0, GPIO_ModeOut_PP_5mA);
    UART_DefInit();

    PRINT("Hello from CH570!\r\n");

    while (1);
}