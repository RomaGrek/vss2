#include "hal.h"

int i = 0;
int start_value = 500;
int arrPinD[] = {
        GPIO_PIN_3,
        GPIO_PIN_4,
        GPIO_PIN_5,
        GPIO_PIN_6,
        GPIO_PIN_8,
        GPIO_PIN_9,
        GPIO_PIN_11,
        GPIO_PIN_12
};

void checkSpeed() {
    GPIO_PinState sw1 = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
    GPIO_PinState sw2 = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8);
    GPIO_PinState sw3 = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10);
    GPIO_PinState sw4 = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12);
    int s = (1 * sw4) + (2 * sw3) + (2 * 2 * sw2) + (2 * 2 * 2 * sw1);
    WRITE_REG(TIM7_ARR, start_value + (s * 50));
//    /* 0x0 */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_RESET &&
//    sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x1  */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_RESET &&
//        sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value + (1 * 50));
//    }
//    /* 0x2  */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_RESET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value + (2 * 50));
//    }
//    /* 0x3  */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_RESET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value + 150);
//    }
//    /* 0x4  */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value + 200);
//    }
//    /* 0x5  */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value + 250);
//    }
//    /* 0x6  */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x7  */
//    if (sw1 == GPIO_PIN_RESET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x8  */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_RESET &&
//        sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x9  */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_RESET &&
//        sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x10 */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_RESET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x11 */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_RESET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x12 */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x13 */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_RESET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x14 */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_RESET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
//    /* 0x15 */
//    if (sw1 == GPIO_PIN_SET && sw2 == GPIO_PIN_SET &&
//        sw3 == GPIO_PIN_SET && sw4 == GPIO_PIN_SET) {
//        WRITE_REG(TIM7_ARR, start_value);
//    }
}

/* функция обработчик прерывания (горение) */
void TIM6_IRQ_Handler() {
    HAL_GPIO_WritePin(GPIOD, arrPinD[i], GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, arrPinD[7 - i], GPIO_PIN_SET);
    if (i != 7) {
        HAL_GPIO_WritePin(GPIOD, arrPinD[i + 1], GPIO_PIN_SET);
    }
}

/* функция обработчик прерывания (горение) */
void TIM7_IRQ_Handler() {
    HAL_GPIO_WritePin(GPIOD, arrPinD[i], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, arrPinD[7 - i], GPIO_PIN_RESET);
    if (i != 7) {
        HAL_GPIO_WritePin(GPIOD, arrPinD[i + 1], GPIO_PIN_RESET);
    }
    checkSpeed();
    i++;
    if (i == 8) {
        i = 0;
    }
}

int umain() {

    /* региструрем обработчики прерываний */
    registerTIM6_IRQHandler(TIM6_IRQ_Handler);
    registerTIM7_IRQHandler(TIM7_IRQ_Handler);

    /* даем разрешение на прерывания */
    __enable_irq();

    /* настройка таймеров на работу */

    WRITE_REG(TIM6_ARR, 50);
    WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM6_PSC, 0);

    WRITE_REG(TIM7_ARR, 500);
    WRITE_REG(TIM7_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM7_PSC, 0);

//    while (1) {
        WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
        WRITE_REG(TIM7_CR1, TIM_CR1_CEN);
//    }


    return 0;
}
