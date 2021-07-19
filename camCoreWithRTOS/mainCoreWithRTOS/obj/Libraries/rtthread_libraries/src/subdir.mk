################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/rtthread_libraries/src/clock.c \
../Libraries/rtthread_libraries/src/components.c \
../Libraries/rtthread_libraries/src/cpu.c \
../Libraries/rtthread_libraries/src/idle.c \
../Libraries/rtthread_libraries/src/ipc.c \
../Libraries/rtthread_libraries/src/irq.c \
../Libraries/rtthread_libraries/src/kservice.c \
../Libraries/rtthread_libraries/src/mem.c \
../Libraries/rtthread_libraries/src/memheap.c \
../Libraries/rtthread_libraries/src/mempool.c \
../Libraries/rtthread_libraries/src/object.c \
../Libraries/rtthread_libraries/src/scheduler.c \
../Libraries/rtthread_libraries/src/slab.c \
../Libraries/rtthread_libraries/src/thread.c \
../Libraries/rtthread_libraries/src/timer.c 

OBJS += \
./Libraries/rtthread_libraries/src/clock.o \
./Libraries/rtthread_libraries/src/components.o \
./Libraries/rtthread_libraries/src/cpu.o \
./Libraries/rtthread_libraries/src/idle.o \
./Libraries/rtthread_libraries/src/ipc.o \
./Libraries/rtthread_libraries/src/irq.o \
./Libraries/rtthread_libraries/src/kservice.o \
./Libraries/rtthread_libraries/src/mem.o \
./Libraries/rtthread_libraries/src/memheap.o \
./Libraries/rtthread_libraries/src/mempool.o \
./Libraries/rtthread_libraries/src/object.o \
./Libraries/rtthread_libraries/src/scheduler.o \
./Libraries/rtthread_libraries/src/slab.o \
./Libraries/rtthread_libraries/src/thread.o \
./Libraries/rtthread_libraries/src/timer.o 

C_DEPS += \
./Libraries/rtthread_libraries/src/clock.d \
./Libraries/rtthread_libraries/src/components.d \
./Libraries/rtthread_libraries/src/cpu.d \
./Libraries/rtthread_libraries/src/idle.d \
./Libraries/rtthread_libraries/src/ipc.d \
./Libraries/rtthread_libraries/src/irq.d \
./Libraries/rtthread_libraries/src/kservice.d \
./Libraries/rtthread_libraries/src/mem.d \
./Libraries/rtthread_libraries/src/memheap.d \
./Libraries/rtthread_libraries/src/mempool.d \
./Libraries/rtthread_libraries/src/object.d \
./Libraries/rtthread_libraries/src/scheduler.d \
./Libraries/rtthread_libraries/src/slab.d \
./Libraries/rtthread_libraries/src/thread.d \
./Libraries/rtthread_libraries/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/rtthread_libraries/src/%.o: ../Libraries/rtthread_libraries/src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"C:\Users\29275\Desktop\mainCoreWithRTOS\Libraries\rtthread_libraries\bsp" -I"C:\Users\29275\Desktop\mainCoreWithRTOS\Libraries\rtthread_libraries\components\finsh" -I"C:\Users\29275\Desktop\mainCoreWithRTOS\Libraries\rtthread_libraries\include" -I"C:\Users\29275\Desktop\mainCoreWithRTOS\Libraries\rtthread_libraries\include\libc" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

