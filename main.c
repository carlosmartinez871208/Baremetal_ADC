/*
** main.c file made by Carlos Martinez
*/
#define PERIPHERAL_BASE (0x40000000ul)

#define AHB1_PERIPHERAL_OFFSET (0x00020000ul)
#define AHB1_PERIPHERAL_BASE (PERIPHERAL_BASE + AHB1_PERIPHERAL_OFFSET)

#define GPIOA_OFFSET (0x0000ul)
#define GPIOA_BASE (AHB1_PERIPHERAL_BASE + GPIOA_OFFSET)

#define RCC_OFFSET (0x3800ul)
#define RCC_BASE (AHB1_PERIPHERAL_BASE + RCC_OFFSET)

#define RCC_AHB1ENR_OFFSET (0x30ul)
#define RCC_AHB1ENR (*(volatile unsigned int*)(RCC_BASE + RCC_AHB1ENR_OFFSET))

#define GPIOA_ENABLE  (0x1u)
#define GPIOA_DISABLE (0x0u)

#define GPIOAEN (GPIOA_ENABLE << 0) /* Or (GPIOA_DISABLE << 0) */

#define GPIOA_MODER_OFFSET (0x0ul)
#define GPIOA_MODER (*(volatile unsigned int*)(GPIOA_BASE + GPIOA_MODER_OFFSET))

#define GPIOx_MODER_INPUT (0x0u)
#define GPIOx_MODER_OUTPUT (0x1u)
#define GPIOx_MODER_ALTERNATE (0x2u)
#define GPIOx_MODER_ANALOG (0x3)

/*
** (1u<<10)    // Set bit 10 to 1.
** &=~(1u<<11) // Set bit 11 to 0.
*/

#define MODER5 (GPIOx_MODER_OUTPUT << 10) /* Could be any of the other three options */

#define GPIOA_ODR_OFFSET (0x14ul)
#define GPIOA_ODR (*(volatile unsigned int*)(GPIOA_BASE + GPIOA_ODR_OFFSET))

#define ODR5 (1U << 5)
#define PA5 (ODR5)
#define LED_PIN (PA5)

/* 
** ADC configuration 
** In this case PA1 (channel1 - ADC1_1) will configured.
*/
#define APB2_PERIPHERAL_OFFSET (0x10000ul)
#define APB2_PERIPHERAL_BASE (PERIPHERAL_BASE + APB2_PERIPHERAL_OFFSET)

#define ADC1_OFFSET (0x2000ul)
#define ADC1_BASE (APB2_PERIPHERAL_BASE + ADC1_OFFSET)

#define RCC_APB2ENR_OFFSET (0x44ul)
#define RCC_APB2ENR (*(volatile unsigned int*)(RCC_BASE + RCC_APB2ENR_OFFSET))

/*
** ADC_SQR registers are used to check the sequence of every channel.
** i.e. If we want channel 6 as first in the sequence the SQ1 in ADC_SQR3 shall be filled with 6.
*/
#define ADC1_SQR1_OFFSET (0x2Cul)
#define ADC1_SQR1 (*(volatile unsigned int*)(ADC1_BASE + ADC1_SQR1_OFFSET))

#define ADC1_SQR3_OFFSET (0x34ul)
#define ADC1_SQR3 (*(volatile unsigned int*)(ADC1_BASE + ADC1_SQR3_OFFSET))

#define ADC1_CR2_OFFSET (0x08ul)
#define ADC1_CR2 (*(volatile unsigned int*)(ADC1_BASE + ADC1_CR2_OFFSET))

#define ADC1_SR_OFFSET (0x0ul)
#define ADC1_SR (*(volatile unsigned int*)(ADC1_BASE + ADC1_SR_OFFSET))

#define ADC1_DR_OFFSET (0x4Cul)
#define ADC1_DR (*(volatile unsigned int*)(ADC1_BASE + ADC1_DR_OFFSET))

#define RCC_APB2ENR_ADC1EN (1ul << 8)

#define MODER1 (3ul<<2)
#define ADC1_SQR1_L (0x0ul) /* Only one channel is in sequence */
#define ADC1_SQR3_SQ1 (0x1ul) /* Channel 1 as first in the sequence. */
#define ADC1_CR2_ADON (0x1ul) /* Enable ADC */
#define ADC1_CR2_SWSTART (1ul << 30) /* start convertion of regular channels */
#define ADC1_SR_EOC (1ul << 1)
#define ADC1_DR_DATA_MASK (0xFFFul)

const unsigned int delay = 1000000u;

unsigned int speed;

int main (void)
{
    int adc_value = 0ul;
    /* Enable clock access to GPIOA */
    RCC_AHB1ENR |= GPIOAEN;
    /* Configure ADC GPIO pin as Analog mode.*/
    GPIOA_MODER |= MODER1;
    /* Enable clock access to ADC1 */
    RCC_APB2ENR |= RCC_APB2ENR_ADC1EN;
    /* Conversion sequence start */
    ADC1_SQR3 = ADC1_SQR3_SQ1;
    /* Convertion sequence length */
    ADC1_SQR1 = ADC1_SQR1_L;
    /* Enable ADC module */
    ADC1_CR2 |= ADC1_CR2_ADON;
    /* Set PA5 as output*/
    GPIOA_MODER |= MODER5;
    while(1u)
    {
        /*** ADC read ***/
        /* Start ADC conversion */
        ADC1_CR2 |= ADC1_CR2_SWSTART;
        /* Wait for convertion to be completed. */
        while (ADC1_SR_EOC!=(ADC1_SR & ADC1_SR_EOC));
        /* Read converted result. */
        adc_value = (ADC1_DR & ADC1_DR_DATA_MASK);
        if(adc_value < 100u)
        {
            /* Set PA5 low */
            GPIOA_ODR &= ~LED_PIN;
        }
        else
        {
            /* Set PA5 high */
            GPIOA_ODR |= LED_PIN;
        }
    }
}