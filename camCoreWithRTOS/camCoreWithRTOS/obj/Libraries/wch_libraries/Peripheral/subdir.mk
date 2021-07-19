################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/wch_libraries/Peripheral/ch32v10x_adc.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_bkp.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_crc.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_dbgmcu.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_dma.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_exti.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_flash.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_gpio.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_i2c.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_iwdg.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_misc.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_pwr.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_rcc.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_rtc.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_spi.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_tim.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_usart.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_usb.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_usb_host.c \
../Libraries/wch_libraries/Peripheral/ch32v10x_wwdg.c 

OBJS += \
./Libraries/wch_libraries/Peripheral/ch32v10x_adc.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_bkp.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_crc.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_dbgmcu.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_dma.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_exti.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_flash.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_gpio.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_i2c.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_iwdg.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_misc.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_pwr.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_rcc.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_rtc.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_spi.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_tim.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_usart.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_usb.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_usb_host.o \
./Libraries/wch_libraries/Peripheral/ch32v10x_wwdg.o 

C_DEPS += \
./Libraries/wch_libraries/Peripheral/ch32v10x_adc.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_bkp.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_crc.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_dbgmcu.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_dma.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_exti.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_flash.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_gpio.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_i2c.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_iwdg.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_misc.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_pwr.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_rcc.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_rtc.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_spi.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_tim.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_usart.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_usb.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_usb_host.d \
./Libraries/wch_libraries/Peripheral/ch32v10x_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/wch_libraries/Peripheral/%.o: ../Libraries/wch_libraries/Peripheral/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\include" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

