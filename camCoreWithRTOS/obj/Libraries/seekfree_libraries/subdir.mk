################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/seekfree_libraries/common.c \
../Libraries/seekfree_libraries/misc.c \
../Libraries/seekfree_libraries/zf_adc.c \
../Libraries/seekfree_libraries/zf_camera.c \
../Libraries/seekfree_libraries/zf_flash.c \
../Libraries/seekfree_libraries/zf_gpio.c \
../Libraries/seekfree_libraries/zf_nvic.c \
../Libraries/seekfree_libraries/zf_pit.c \
../Libraries/seekfree_libraries/zf_pwm.c \
../Libraries/seekfree_libraries/zf_qtimer.c \
../Libraries/seekfree_libraries/zf_spi.c \
../Libraries/seekfree_libraries/zf_systick.c \
../Libraries/seekfree_libraries/zf_uart.c 

OBJS += \
./Libraries/seekfree_libraries/common.o \
./Libraries/seekfree_libraries/misc.o \
./Libraries/seekfree_libraries/zf_adc.o \
./Libraries/seekfree_libraries/zf_camera.o \
./Libraries/seekfree_libraries/zf_flash.o \
./Libraries/seekfree_libraries/zf_gpio.o \
./Libraries/seekfree_libraries/zf_nvic.o \
./Libraries/seekfree_libraries/zf_pit.o \
./Libraries/seekfree_libraries/zf_pwm.o \
./Libraries/seekfree_libraries/zf_qtimer.o \
./Libraries/seekfree_libraries/zf_spi.o \
./Libraries/seekfree_libraries/zf_systick.o \
./Libraries/seekfree_libraries/zf_uart.o 

C_DEPS += \
./Libraries/seekfree_libraries/common.d \
./Libraries/seekfree_libraries/misc.d \
./Libraries/seekfree_libraries/zf_adc.d \
./Libraries/seekfree_libraries/zf_camera.d \
./Libraries/seekfree_libraries/zf_flash.d \
./Libraries/seekfree_libraries/zf_gpio.d \
./Libraries/seekfree_libraries/zf_nvic.d \
./Libraries/seekfree_libraries/zf_pit.d \
./Libraries/seekfree_libraries/zf_pwm.d \
./Libraries/seekfree_libraries/zf_qtimer.d \
./Libraries/seekfree_libraries/zf_spi.d \
./Libraries/seekfree_libraries/zf_systick.d \
./Libraries/seekfree_libraries/zf_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/seekfree_libraries/%.o: ../Libraries/seekfree_libraries/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"C:\Users\29275\Desktop\WchWorkSPace\camCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"C:\Users\29275\Desktop\WchWorkSPace\camCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"C:\Users\29275\Desktop\WchWorkSPace\camCoreWithRTOS\Libraries\rtthread_libraries\include" -I"C:\Users\29275\Desktop\WchWorkSPace\camCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

