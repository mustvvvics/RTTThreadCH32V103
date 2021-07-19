################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/communicationCam.c \
../CODE/displayCam.c \
../CODE/esp8266Cam.c \
../CODE/laneAnalyze.c \
../CODE/laneCommon.c \
../CODE/timerPitCam.c \
../CODE/valueCam.c 

OBJS += \
./CODE/communicationCam.o \
./CODE/displayCam.o \
./CODE/esp8266Cam.o \
./CODE/laneAnalyze.o \
./CODE/laneCommon.o \
./CODE/timerPitCam.o \
./CODE/valueCam.o 

C_DEPS += \
./CODE/communicationCam.d \
./CODE/displayCam.d \
./CODE/esp8266Cam.d \
./CODE/laneAnalyze.d \
./CODE/laneCommon.d \
./CODE/timerPitCam.d \
./CODE/valueCam.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.o: ../CODE/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\include" -I"F:\SCHOOL\文件\智能车\全向组\RTTThreadCH32V103\camCoreWithRTOS\camCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

