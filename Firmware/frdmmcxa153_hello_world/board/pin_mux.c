/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v16.0
processor: MCXA153
package_id: MCXA153VLH
mcu_data: ksdk2_0
processor_version: 16.3.0
external_user_signals: {}
pin_labels:
- {pin_num: '1', pin_signal: P1_7/WUU0_IN9/TRIG_OUT2/LPUART2_CTS_B/CT_INP7/ADC0_A23, label: RED_LED, identifier: RED_LED}
- {pin_num: '62', pin_signal: P1_4/WUU0_IN8/FREQME_CLK_IN0/LPSPI0_PCS3/LPUART2_RXD/CT1_MAT2/ADC0_A20/CMP0_IN2, label: GREEN_LED, identifier: GREEN_LED}
- {pin_num: '64', pin_signal: P1_6/TRIG_IN2/LPSPI0_PCS1/LPUART2_RTS_B/CT_INP6/ADC0_A22, label: YELLOW_LED, identifier: YELLOW_LED}
- {pin_num: '63', pin_signal: P1_5/FREQME_CLK_IN1/LPSPI0_PCS2/LPUART2_TXD/CT1_MAT3/ADC0_A21/CMP1_IN2, label: IN_4, identifier: IN_4}
- {pin_num: '18', pin_signal: P2_4/CT_INP14/CT1_MAT0, label: IN_3, identifier: IN_3}
- {pin_num: '19', pin_signal: P2_5/CT_INP15/CT1_MAT1, label: IN_2, identifier: IN_2}
- {pin_num: '38', pin_signal: P3_12/LPUART2_RTS_B/CT1_MAT2/PWM0_X0, label: IN_1, identifier: IN_1}
- {pin_num: '16', pin_signal: P2_2/TRIG_IN6/LPUART0_RTS_B/LPUART2_TXD/CT_INP12/CT2_MAT2/ADC0_A4/CMP0_IN0, label: ALARM}
- {pin_num: '37', pin_signal: P3_13/LPUART2_CTS_B/CT1_MAT3/PWM0_X1, label: BUZZER, identifier: BUZZER}
- {pin_num: '17', pin_signal: P2_3/WUU0_IN19/TRIG_IN7/LPUART0_CTS_B/LPUART2_RXD/CT_INP13/CT2_MAT3/ADC0_A2/CMP1_IN0, label: BUZZER, identifier: BUZZER}
- {pin_num: '35', pin_signal: P3_15/LPUART2_TXD/CT_INP7, label: HCSR04_ECHO, identifier: HCSR04_ECHO}
- {pin_num: '36', pin_signal: P3_14/WUU0_IN25/LPUART2_RXD/CT_INP6/PWM0_X2, label: HCSR04_TRIG, identifier: HCSR04_TRIG}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33_core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '51', peripheral: LPUART0, signal: RX, pin_signal: P0_2/TDO/SWO/LPUART0_RXD/LPSPI0_SCK/CT0_MAT0/UTICK_CAP0/I3C0_PUR, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: up, pull_enable: enable, input_buffer: enable, invert_input: normal}
  - {pin_num: '52', peripheral: LPUART0, signal: TX, pin_signal: P0_3/TDI/LPUART0_TXD/LPSPI0_SDO/CT0_MAT1/UTICK_CAP1/CMP0_OUT/CMP1_IN1, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: up, pull_enable: enable, input_buffer: enable, invert_input: normal}
  - {pin_num: '1', peripheral: GPIO1, signal: 'GPIO, 7', pin_signal: P1_7/WUU0_IN9/TRIG_OUT2/LPUART2_CTS_B/CT_INP7/ADC0_A23, direction: OUTPUT, gpio_init_state: 'false'}
  - {pin_num: '1', peripheral: WUU0, signal: 'P, 9', pin_signal: P1_7/WUU0_IN9/TRIG_OUT2/LPUART2_CTS_B/CT_INP7/ADC0_A23, identifier: ''}
  - {pin_num: '62', peripheral: GPIO1, signal: 'GPIO, 4', pin_signal: P1_4/WUU0_IN8/FREQME_CLK_IN0/LPSPI0_PCS3/LPUART2_RXD/CT1_MAT2/ADC0_A20/CMP0_IN2, direction: OUTPUT}
  - {pin_num: '62', peripheral: WUU0, signal: 'P, 8', pin_signal: P1_4/WUU0_IN8/FREQME_CLK_IN0/LPSPI0_PCS3/LPUART2_RXD/CT1_MAT2/ADC0_A20/CMP0_IN2, identifier: ''}
  - {pin_num: '64', peripheral: GPIO1, signal: 'GPIO, 6', pin_signal: P1_6/TRIG_IN2/LPSPI0_PCS1/LPUART2_RTS_B/CT_INP6/ADC0_A22, direction: OUTPUT}
  - {pin_num: '63', peripheral: GPIO1, signal: 'GPIO, 5', pin_signal: P1_5/FREQME_CLK_IN1/LPSPI0_PCS2/LPUART2_TXD/CT1_MAT3/ADC0_A21/CMP1_IN2, direction: OUTPUT}
  - {pin_num: '18', peripheral: GPIO2, signal: 'GPIO, 4', pin_signal: P2_4/CT_INP14/CT1_MAT0, direction: OUTPUT}
  - {pin_num: '19', peripheral: GPIO2, signal: 'GPIO, 5', pin_signal: P2_5/CT_INP15/CT1_MAT1, direction: OUTPUT}
  - {pin_num: '38', peripheral: GPIO3, signal: 'GPIO, 12', pin_signal: P3_12/LPUART2_RTS_B/CT1_MAT2/PWM0_X0, direction: OUTPUT}
  - {pin_num: '37', peripheral: CTIMER1, signal: 'MATCH, 3', pin_signal: P3_13/LPUART2_CTS_B/CT1_MAT3/PWM0_X1}
  - {pin_num: '35', peripheral: GPIO3, signal: 'GPIO, 15', pin_signal: P3_15/LPUART2_TXD/CT_INP7, direction: INPUT, gpio_per_interrupt: kGPIO_InterruptEitherEdge}
  - {pin_num: '36', peripheral: GPIO3, signal: 'GPIO, 14', pin_signal: P3_14/WUU0_IN25/LPUART2_RXD/CT_INP6/PWM0_X2, direction: OUTPUT}
  - {pin_num: '36', peripheral: WUU0, signal: 'P, 25', pin_signal: P3_14/WUU0_IN25/LPUART2_RXD/CT_INP6/PWM0_X2, identifier: ''}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Write to GPIO1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateGPIO1);
    /* Write to GPIO2: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateGPIO2);
    /* Write to GPIO3: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateGPIO3);
    /* Write to PORT0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT0);
    /* Write to PORT1: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT1);
    /* Write to PORT2: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT2);
    /* Write to PORT3: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GatePORT3);
    /* GPIO1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    /* GPIO2 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO2_RST_SHIFT_RSTn);
    /* GPIO3 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    /* LPUART0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kLPUART0_RST_SHIFT_RSTn);
    /* PORT0 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT0_RST_SHIFT_RSTn);
    /* PORT1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT1_RST_SHIFT_RSTn);
    /* PORT2 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT2_RST_SHIFT_RSTn);
    /* PORT3 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kPORT3_RST_SHIFT_RSTn);
    /* CTIMER1 peripheral is released from reset */
    RESET_ReleasePeripheralReset(kCTIMER1_RST_SHIFT_RSTn);

    gpio_pin_config_t GREEN_LED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO1_4 (pin 62)  */
    GPIO_PinInit(BOARD_INITPINS_GREEN_LED_GPIO, BOARD_INITPINS_GREEN_LED_PIN, &GREEN_LED_config);

    gpio_pin_config_t IN_4_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO1_5 (pin 63)  */
    GPIO_PinInit(BOARD_INITPINS_IN_4_GPIO, BOARD_INITPINS_IN_4_PIN, &IN_4_config);

    gpio_pin_config_t YELLOW_LED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO1_6 (pin 64)  */
    GPIO_PinInit(BOARD_INITPINS_YELLOW_LED_GPIO, BOARD_INITPINS_YELLOW_LED_PIN, &YELLOW_LED_config);

    gpio_pin_config_t RED_LED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO1_7 (pin 1)  */
    GPIO_PinInit(BOARD_INITPINS_RED_LED_GPIO, BOARD_INITPINS_RED_LED_PIN, &RED_LED_config);

    gpio_pin_config_t IN_3_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO2_4 (pin 18)  */
    GPIO_PinInit(BOARD_INITPINS_IN_3_GPIO, BOARD_INITPINS_IN_3_PIN, &IN_3_config);

    gpio_pin_config_t IN_2_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO2_5 (pin 19)  */
    GPIO_PinInit(BOARD_INITPINS_IN_2_GPIO, BOARD_INITPINS_IN_2_PIN, &IN_2_config);

    gpio_pin_config_t IN_1_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO3_12 (pin 38)  */
    GPIO_PinInit(BOARD_INITPINS_IN_1_GPIO, BOARD_INITPINS_IN_1_PIN, &IN_1_config);

    gpio_pin_config_t HCSR04_TRIG_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO3_14 (pin 36)  */
    GPIO_PinInit(BOARD_INITPINS_HCSR04_TRIG_GPIO, BOARD_INITPINS_HCSR04_TRIG_PIN, &HCSR04_TRIG_config);

    gpio_pin_config_t HCSR04_ECHO_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PIO3_15 (pin 35)  */
    GPIO_PinInit(BOARD_INITPINS_HCSR04_ECHO_GPIO, BOARD_INITPINS_HCSR04_ECHO_PIN, &HCSR04_ECHO_config);

    /* Interrupt configuration on GPIO3_15 (pin 35): Interrupt on either edge */
    GPIO_SetPinInterruptConfig(BOARD_INITPINS_HCSR04_ECHO_GPIO, BOARD_INITPINS_HCSR04_ECHO_PIN, kGPIO_InterruptEitherEdge);

    const port_pin_config_t port0_2_pin51_config = {/* Internal pull-up resistor is enabled */
                                                    .pullSelect = kPORT_PullUp,
                                                    /* Low internal pull resistor value is selected. */
                                                    .pullValueSelect = kPORT_LowPullResistor,
                                                    /* Fast slew rate is configured */
                                                    .slewRate = kPORT_FastSlewRate,
                                                    /* Passive input filter is disabled */
                                                    .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                    /* Open drain output is disabled */
                                                    .openDrainEnable = kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    .driveStrength = kPORT_LowDriveStrength,
                                                    /* Normal drive strength is configured */
                                                    .driveStrength1 = kPORT_NormalDriveStrength,
                                                    /* Pin is configured as LPUART0_RXD */
                                                    .mux = kPORT_MuxAlt2,
                                                    /* Digital input enabled */
                                                    .inputBuffer = kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    .invertInput = kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    .lockRegister = kPORT_UnlockRegister};
    /* PORT0_2 (pin 51) is configured as LPUART0_RXD */
    PORT_SetPinConfig(PORT0, 2U, &port0_2_pin51_config);

    const port_pin_config_t port0_3_pin52_config = {/* Internal pull-up resistor is enabled */
                                                    .pullSelect = kPORT_PullUp,
                                                    /* Low internal pull resistor value is selected. */
                                                    .pullValueSelect = kPORT_LowPullResistor,
                                                    /* Fast slew rate is configured */
                                                    .slewRate = kPORT_FastSlewRate,
                                                    /* Passive input filter is disabled */
                                                    .passiveFilterEnable = kPORT_PassiveFilterDisable,
                                                    /* Open drain output is disabled */
                                                    .openDrainEnable = kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    .driveStrength = kPORT_LowDriveStrength,
                                                    /* Normal drive strength is configured */
                                                    .driveStrength1 = kPORT_NormalDriveStrength,
                                                    /* Pin is configured as LPUART0_TXD */
                                                    .mux = kPORT_MuxAlt2,
                                                    /* Digital input enabled */
                                                    .inputBuffer = kPORT_InputBufferEnable,
                                                    /* Digital input is not inverted */
                                                    .invertInput = kPORT_InputNormal,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    .lockRegister = kPORT_UnlockRegister};
    /* PORT0_3 (pin 52) is configured as LPUART0_TXD */
    PORT_SetPinConfig(PORT0, 3U, &port0_3_pin52_config);

    /* PORT1_4 (pin 62) is configured as P1_4, WUU0_IN8 */
    PORT_SetPinMux(BOARD_INITPINS_GREEN_LED_PORT, BOARD_INITPINS_GREEN_LED_PIN, kPORT_MuxAlt0);

    PORT1->PCR[4] = ((PORT1->PCR[4] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT1_5 (pin 63) is configured as P1_5 */
    PORT_SetPinMux(BOARD_INITPINS_IN_4_PORT, BOARD_INITPINS_IN_4_PIN, kPORT_MuxAlt0);

    PORT1->PCR[5] = ((PORT1->PCR[5] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT1_6 (pin 64) is configured as P1_6 */
    PORT_SetPinMux(BOARD_INITPINS_YELLOW_LED_PORT, BOARD_INITPINS_YELLOW_LED_PIN, kPORT_MuxAlt0);

    PORT1->PCR[6] = ((PORT1->PCR[6] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT1_7 (pin 1) is configured as P1_7, WUU0_IN9 */
    PORT_SetPinMux(BOARD_INITPINS_RED_LED_PORT, BOARD_INITPINS_RED_LED_PIN, kPORT_MuxAlt0);

    PORT1->PCR[7] = ((PORT1->PCR[7] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_IBE_MASK)))

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    PORT2->PCR[4] = ((PORT2->PCR[4] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_MUX_MASK | PORT_PCR_IBE_MASK)))

                     /* Pin Multiplex Control: PORT2_4 (pin 18) is configured as P2_4. */
                     | PORT_PCR_MUX(PORT2_PCR4_MUX_mux00)

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    PORT2->PCR[5] = ((PORT2->PCR[5] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_MUX_MASK | PORT_PCR_IBE_MASK)))

                     /* Pin Multiplex Control: PORT2_5 (pin 19) is configured as P2_5. */
                     | PORT_PCR_MUX(PORT2_PCR5_MUX_mux00)

                     /* Input Buffer Enable: Enables. */
                     | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT3_12 (pin 38) is configured as P3_12 */
    PORT_SetPinMux(BOARD_INITPINS_IN_1_PORT, BOARD_INITPINS_IN_1_PIN, kPORT_MuxAlt0);

    PORT3->PCR[12] = ((PORT3->PCR[12] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT3_13 (pin 37) is configured as CT1_MAT3 */
    PORT_SetPinMux(BOARD_INITPINS_BUZZER_PORT, BOARD_INITPINS_BUZZER_PIN, kPORT_MuxAlt4);

    PORT3->PCR[13] = ((PORT3->PCR[13] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT3_14 (pin 36) is configured as P3_14, WUU0_IN25 */
    PORT_SetPinMux(BOARD_INITPINS_HCSR04_TRIG_PORT, BOARD_INITPINS_HCSR04_TRIG_PIN, kPORT_MuxAlt0);

    PORT3->PCR[14] = ((PORT3->PCR[14] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));

    /* PORT3_15 (pin 35) is configured as P3_15 */
    PORT_SetPinMux(BOARD_INITPINS_HCSR04_ECHO_PORT, BOARD_INITPINS_HCSR04_ECHO_PIN, kPORT_MuxAlt0);

    PORT3->PCR[15] = ((PORT3->PCR[15] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_IBE_MASK)))

                      /* Input Buffer Enable: Enables. */
                      | PORT_PCR_IBE(PCR_IBE_ibe1));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
