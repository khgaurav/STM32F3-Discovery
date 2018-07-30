################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Components/l3gd20/l3gd20.c 

OBJS += \
./Utilities/Components/l3gd20/l3gd20.o 

C_DEPS += \
./Utilities/Components/l3gd20/l3gd20.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Components/l3gd20/%.o: ../Utilities/Components/l3gd20/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F3 -DSTM32F30 -DSTM32F303VCTx -DSTM32F3DISCOVERY -DDEBUG -DSTM32F303xC -DUSE_HAL_DRIVER -I"/home/gauravkh/git/STM32F3-Discovery/PWM/HAL_Driver/Inc/Legacy" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/STM32F3-Discovery" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/Components/lsm303dlhc" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/Components/Common" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/Utilities/Components/l3gd20" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/inc" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/CMSIS/device" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/CMSIS/core" -I"/home/gauravkh/git/STM32F3-Discovery/PWM/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


