################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/seekfree_peripheral/SEEKFREE_18TFT.c \
../Libraries/seekfree_peripheral/SEEKFREE_ABSOLUTE_ENCODER.c \
../Libraries/seekfree_peripheral/SEEKFREE_FONT.c \
../Libraries/seekfree_peripheral/SEEKFREE_ICM20602.c \
../Libraries/seekfree_peripheral/SEEKFREE_IIC.c \
../Libraries/seekfree_peripheral/SEEKFREE_IPS114_SPI.c \
../Libraries/seekfree_peripheral/SEEKFREE_MPU6050.c \
../Libraries/seekfree_peripheral/SEEKFREE_MT9V03X.c \
../Libraries/seekfree_peripheral/SEEKFREE_OLED.c \
../Libraries/seekfree_peripheral/SEEKFREE_PRINTF.c \
../Libraries/seekfree_peripheral/SEEKFREE_TSL1401.c \
../Libraries/seekfree_peripheral/SEEKFREE_UART_7725.c \
../Libraries/seekfree_peripheral/SEEKFREE_VIRSCO.c \
../Libraries/seekfree_peripheral/SEEKFREE_WIRELESS.c 

OBJS += \
./Libraries/seekfree_peripheral/SEEKFREE_18TFT.o \
./Libraries/seekfree_peripheral/SEEKFREE_ABSOLUTE_ENCODER.o \
./Libraries/seekfree_peripheral/SEEKFREE_FONT.o \
./Libraries/seekfree_peripheral/SEEKFREE_ICM20602.o \
./Libraries/seekfree_peripheral/SEEKFREE_IIC.o \
./Libraries/seekfree_peripheral/SEEKFREE_IPS114_SPI.o \
./Libraries/seekfree_peripheral/SEEKFREE_MPU6050.o \
./Libraries/seekfree_peripheral/SEEKFREE_MT9V03X.o \
./Libraries/seekfree_peripheral/SEEKFREE_OLED.o \
./Libraries/seekfree_peripheral/SEEKFREE_PRINTF.o \
./Libraries/seekfree_peripheral/SEEKFREE_TSL1401.o \
./Libraries/seekfree_peripheral/SEEKFREE_UART_7725.o \
./Libraries/seekfree_peripheral/SEEKFREE_VIRSCO.o \
./Libraries/seekfree_peripheral/SEEKFREE_WIRELESS.o 

C_DEPS += \
./Libraries/seekfree_peripheral/SEEKFREE_18TFT.d \
./Libraries/seekfree_peripheral/SEEKFREE_ABSOLUTE_ENCODER.d \
./Libraries/seekfree_peripheral/SEEKFREE_FONT.d \
./Libraries/seekfree_peripheral/SEEKFREE_ICM20602.d \
./Libraries/seekfree_peripheral/SEEKFREE_IIC.d \
./Libraries/seekfree_peripheral/SEEKFREE_IPS114_SPI.d \
./Libraries/seekfree_peripheral/SEEKFREE_MPU6050.d \
./Libraries/seekfree_peripheral/SEEKFREE_MT9V03X.d \
./Libraries/seekfree_peripheral/SEEKFREE_OLED.d \
./Libraries/seekfree_peripheral/SEEKFREE_PRINTF.d \
./Libraries/seekfree_peripheral/SEEKFREE_TSL1401.d \
./Libraries/seekfree_peripheral/SEEKFREE_UART_7725.d \
./Libraries/seekfree_peripheral/SEEKFREE_VIRSCO.d \
./Libraries/seekfree_peripheral/SEEKFREE_WIRELESS.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/seekfree_peripheral/%.o: ../Libraries/seekfree_peripheral/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"../Libraries/wch_libraries/Core" -I"../Libraries/wch_libraries/Peripheral" -I"../Libraries/wch_libraries/Startup" -I"../Libraries/seekfree_libraries" -I"../Libraries/seekfree_peripheral" -I"../Libraries/board" -I"../CODE" -I"../USER" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

