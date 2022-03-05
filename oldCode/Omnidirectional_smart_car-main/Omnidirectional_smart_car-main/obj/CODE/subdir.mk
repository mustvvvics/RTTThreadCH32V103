################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CODE/fliter.c \
../CODE/function.c \
../CODE/key.c \
../CODE/motor.c \
../CODE/pid.c \
../CODE/uart_com.c \
../CODE/value.c 

OBJS += \
./CODE/fliter.o \
./CODE/function.o \
./CODE/key.o \
./CODE/motor.o \
./CODE/pid.o \
./CODE/uart_com.o \
./CODE/value.o 

C_DEPS += \
./CODE/fliter.d \
./CODE/function.d \
./CODE/key.d \
./CODE/motor.d \
./CODE/pid.d \
./CODE/uart_com.d \
./CODE/value.d 


# Each subdirectory must supply rules for building sources it contributes
CODE/%.o: ../CODE/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

