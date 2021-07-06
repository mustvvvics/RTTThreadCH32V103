################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/rtthread_libraries/libcpu/cpuport.c 

S_UPPER_SRCS += \
../Libraries/rtthread_libraries/libcpu/context_gcc.S \
../Libraries/rtthread_libraries/libcpu/interrupt_gcc.S 

OBJS += \
./Libraries/rtthread_libraries/libcpu/context_gcc.o \
./Libraries/rtthread_libraries/libcpu/cpuport.o \
./Libraries/rtthread_libraries/libcpu/interrupt_gcc.o 

S_UPPER_DEPS += \
./Libraries/rtthread_libraries/libcpu/context_gcc.d \
./Libraries/rtthread_libraries/libcpu/interrupt_gcc.d 

C_DEPS += \
./Libraries/rtthread_libraries/libcpu/cpuport.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/rtthread_libraries/libcpu/%.o: ../Libraries/rtthread_libraries/libcpu/%.S
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"../Libraries/wch_libraries/Startup" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\libcpu" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
Libraries/rtthread_libraries/libcpu/%.o: ../Libraries/rtthread_libraries/libcpu/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\include" -I"F:\SCHOOL\文件\智能车\全向组\WchWorkSPace\mainCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

