################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/board/board.c \
../Libraries/board/clock_config.c 

OBJS += \
./Libraries/board/board.o \
./Libraries/board/clock_config.o 

C_DEPS += \
./Libraries/board/board.d \
./Libraries/board/clock_config.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/board/%.o: ../Libraries/board/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"E:\onedrive\OneDrive - stu.cqupt.edu.cn\Desktop\rt\mainCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"E:\onedrive\OneDrive - stu.cqupt.edu.cn\Desktop\rt\mainCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"E:\onedrive\OneDrive - stu.cqupt.edu.cn\Desktop\rt\mainCoreWithRTOS\Libraries\rtthread_libraries\include" -I"E:\onedrive\OneDrive - stu.cqupt.edu.cn\Desktop\rt\mainCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

