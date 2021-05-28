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
    /* Переводи двоичное значение с переключателей дополнительного модуля в десятичную СИ что бы понять на сколько умножать T */
    int s = (1 * sw4) + (2 * sw3) + (2 * 2 * sw2) + (2 * 2 * 2 * sw1);

    WRITE_REG(TIM7_ARR, start_value + (s * 50));
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

        WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
        WRITE_REG(TIM7_CR1, TIM_CR1_CEN);


    return 0;
}
