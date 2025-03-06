# Baremetal ADC

## What is an ADC?
ADC means Analog to Digital Converter.

ADC translates analog signals to digital numbers.

### ADC Resolution

The samllest change that it can be detected by the ADC.

e.g. Taking Vref as 5V.

| n-bit  | Number of steps | Step size             |
|--------|-----------------|-----------------------|
| 8 bit  | 256             | 5V / 256   = 19.56mV  |
| 10 bit | 1,024           | 5V / 1024  =  4.88mV  |
| 12 bit | 4,096           | 5V / 4096  =  1.2mV   |
| 16 bit | 65,536          | 5V / 65536 =  0.076mV |

## ADC independent modes.

### Single- channel, single conversion mode.

ADC performs a single conversion of a single channel **x** and stops after conversion is complete.

Use case: measurement of voltage level to determine if a system should be started or not.

### Multichannel (scan), single conversion mode.

Used to converte multiple channels successively.

Up to 16 different channels with different sampling times can be converted on the STM32f401re.

Use case: multiple measurement of voltage level to determine if a system should be started or not.

### Single-channel continuous conversion mode.

Used to convert a single channel continously.

Works in the background without intervention form the CPU.

Use case: measurement of room temperature continous to adjust air conditioner.

### Multichannel continuous conversion mode.

Used to convert a multiple channels continously.

Up to 16 different channels with different sampling times can be converted on the STM32f401re.

Use case: measurement of multiple accelerometers to adjust joints of a robotic arm.

### Injected continuous conversion mode.

Intended for use when conversion is triggered by an external event or by software.

The injected group has priority over the regular channel group.

Interrupts the convertion of the current channel in the regular channel group.

Use case: for syncronizing the convertion of channels to an event.