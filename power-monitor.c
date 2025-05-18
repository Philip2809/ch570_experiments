#include "CH57x_common.h"

// Power monitor without NMI, same function as PowerMonitor but without NMI
void PowerMonitorNoNMI(FunctionalState s, VolM_LevelypeDef vl)
{
    uint8_t ctrl = R8_BAT_DET_CTRL;
    uint8_t cfg = R8_BAT_DET_CFG;

    if (s == DISABLE)
    {
        sys_safe_access_enable();
        R8_BAT_DET_CTRL = 0;
        sys_safe_access_disable();
    }
    else
    {
        cfg = vl & 0x03;
        ctrl = RB_BAT_MON_EN;
        sys_safe_access_enable();
        R8_BAT_DET_CTRL = ctrl;
        R8_BAT_DET_CFG = cfg;
        sys_safe_access_disable();
    }
}

typedef enum
{
    BL_BELOW_1V8 = 0,
    BL_1V8_2V0,  // 1
    BL_2V0_2V2,  // 2
    BL_2V2_2V4,  // 3
    BL_ABOVE_2V4 // 4
} BatteryVoltageLevel;

BatteryVoltageLevel check_voltage_level(void)
{
    // Check 2.4V
    PowerMonitorNoNMI(ENABLE, LPLevel_2V4);
    if (!(R8_BAT_STATUS & RB_BAT_STAT_LOW))
    {
        PowerMonitorNoNMI(DISABLE, 0);
        return BL_ABOVE_2V4; // Above 2.4V
    }

    // Check 2.2V
    PowerMonitorNoNMI(ENABLE, LPLevel_2V2);
    if (!(R8_BAT_STATUS & RB_BAT_STAT_LOW))
    {
        PowerMonitorNoNMI(DISABLE, 0);
        return BL_2V2_2V4; // Between 2.2V and 2.4V
    }

    // Check 2.0V
    PowerMonitorNoNMI(ENABLE, LPLevel_2V0);
    if (!(R8_BAT_STATUS & RB_BAT_STAT_LOW))
    {
        PowerMonitorNoNMI(DISABLE, 0);
        return BL_2V0_2V2; // Between 2.0V and 2.2V
    }

    // Check 1.8V
    PowerMonitorNoNMI(ENABLE, LPLevel_1V8);
    if (!(R8_BAT_STATUS & RB_BAT_STAT_LOW))
    {
        PowerMonitorNoNMI(DISABLE, 0);
        return BL_1V8_2V0; // Between 1.8V and 2.0V
    }

    PowerMonitorNoNMI(DISABLE, 0);
    return BL_BELOW_1V8; // Below 1.8V
}

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

    BatteryVoltageLevel vlevel = check_voltage_level();
    PRINT("Voltage level: %d\r\n", vlevel);

    while (1)
        ;
}