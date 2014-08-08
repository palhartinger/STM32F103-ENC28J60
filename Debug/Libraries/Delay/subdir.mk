################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/Delay/Delay.c 

OBJS += \
./Libraries/Delay/Delay.o 

C_DEPS += \
./Libraries/Delay/Delay.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Delay/%.o: ../Libraries/Delay/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/home/thanhtruong/workspace/STM32F103_ENC28J60/Libraries/Delay" -I"/home/thanhtruong/workspace/STM32F103_ENC28J60/Libraries/Ethernet/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


