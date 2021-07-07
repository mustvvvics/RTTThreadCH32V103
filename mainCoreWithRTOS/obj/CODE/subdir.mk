################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/button.c \
../CODE/buzzer.c \
../CODE/communication.c \
../CODE/display.c \
../CODE/encoder.c \
../CODE/esp8266.c \
../CODE/motor.c \
../CODE/pid.c \
../CODE/timer_pit.c \
../CODE/value.c 

OBJS += \
./CODE/button.o \
./CODE/buzzer.o \
./CODE/communication.o \
./CODE/display.o \
./CODE/encoder.o \
./CODE/esp8266.o \
./CODE/motor.o \
./CODE/pid.o \
./CODE/timer_pit.o \
./CODE/value.o 

C_DEPS += \
./CODE/button.d \
./CODE/buzzer.d \
./CODE/communication.d \
./CODE/display.d \
./CODE/encoder.d \
./CODE/esp8266.d \
./CODE/motor.d \
./CODE/pid.d \
./CODE/timer_pit.d \
./CODE/value.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.o: ../CODE/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\mainCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\mainCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\mainCoreWithRTOS\Libraries\rtthread_libraries\include" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\mainCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

