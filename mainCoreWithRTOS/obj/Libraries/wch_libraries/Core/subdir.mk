################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/wch_libraries/Core/core_riscv.c 

OBJS += \
./Libraries/wch_libraries/Core/core_riscv.o 

C_DEPS += \
./Libraries/wch_libraries/Core/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/wch_libraries/Core/%.o: ../Libraries/wch_libraries/Core/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\include" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

