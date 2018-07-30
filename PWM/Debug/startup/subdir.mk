################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f303xc.s 

OBJS += \
./startup/startup_stm32f303xc.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/gauravkh/git/STM32F3-Discovery/PWM/HAL_Driver/Inc/Legacy" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/STM32F3-Discovery" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/Components/lsm303dlhc" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/Components/Common" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/Components/l3gd20" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/inc" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/CMSIS/device" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/CMSIS/core" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/HAL_Driver/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


