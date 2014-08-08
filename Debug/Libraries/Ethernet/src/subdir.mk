################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/Ethernet/src/EtherShield.c \
../Libraries/Ethernet/src/dhcp.c \
../Libraries/Ethernet/src/dnslkup.c \
../Libraries/Ethernet/src/enc28j60.c \
../Libraries/Ethernet/src/ip_arp_udp_tcp.c \
../Libraries/Ethernet/src/websrv_help_functions.c 

OBJS += \
./Libraries/Ethernet/src/EtherShield.o \
./Libraries/Ethernet/src/dhcp.o \
./Libraries/Ethernet/src/dnslkup.o \
./Libraries/Ethernet/src/enc28j60.o \
./Libraries/Ethernet/src/ip_arp_udp_tcp.o \
./Libraries/Ethernet/src/websrv_help_functions.o 

C_DEPS += \
./Libraries/Ethernet/src/EtherShield.d \
./Libraries/Ethernet/src/dhcp.d \
./Libraries/Ethernet/src/dnslkup.d \
./Libraries/Ethernet/src/enc28j60.d \
./Libraries/Ethernet/src/ip_arp_udp_tcp.d \
./Libraries/Ethernet/src/websrv_help_functions.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Ethernet/src/%.o: ../Libraries/Ethernet/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_ITM -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I"/home/thanhtruong/workspace/STM32F103_ENC28J60/Libraries/Delay" -I"/home/thanhtruong/workspace/STM32F103_ENC28J60/Libraries/Ethernet/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


