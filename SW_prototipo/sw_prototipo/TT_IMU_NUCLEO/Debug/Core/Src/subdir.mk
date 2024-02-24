################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/CNI.cpp \
../Core/Src/gpio_STM32.cpp \
../Core/Src/icm42688.cpp \
../Core/Src/icm42688_utils.cpp \
../Core/Src/main.cpp \
../Core/Src/modeFailSilent.cpp \
../Core/Src/modeNormal.cpp \
../Core/Src/myUtils.cpp \
../Core/Src/system.cpp \
../Core/Src/taskCNIsendData.cpp \
../Core/Src/taskHeartbeat.cpp \
../Core/Src/taskIMUgetData.cpp \
../Core/Src/taskWatchdog.cpp \
../Core/Src/timeTriggeredScheduler.cpp \
../Core/Src/timeTriggeredTask.cpp 

C_SRCS += \
../Core/Src/can.c \
../Core/Src/gpio.c \
../Core/Src/iwdg.c \
../Core/Src/spi.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

C_DEPS += \
./Core/Src/can.d \
./Core/Src/gpio.d \
./Core/Src/iwdg.d \
./Core/Src/spi.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 

OBJS += \
./Core/Src/CNI.o \
./Core/Src/can.o \
./Core/Src/gpio.o \
./Core/Src/gpio_STM32.o \
./Core/Src/icm42688.o \
./Core/Src/icm42688_utils.o \
./Core/Src/iwdg.o \
./Core/Src/main.o \
./Core/Src/modeFailSilent.o \
./Core/Src/modeNormal.o \
./Core/Src/myUtils.o \
./Core/Src/spi.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system.o \
./Core/Src/system_stm32f7xx.o \
./Core/Src/taskCNIsendData.o \
./Core/Src/taskHeartbeat.o \
./Core/Src/taskIMUgetData.o \
./Core/Src/taskWatchdog.o \
./Core/Src/tim.o \
./Core/Src/timeTriggeredScheduler.o \
./Core/Src/timeTriggeredTask.o \
./Core/Src/usart.o 

CPP_DEPS += \
./Core/Src/CNI.d \
./Core/Src/gpio_STM32.d \
./Core/Src/icm42688.d \
./Core/Src/icm42688_utils.d \
./Core/Src/main.d \
./Core/Src/modeFailSilent.d \
./Core/Src/modeNormal.d \
./Core/Src/myUtils.d \
./Core/Src/system.d \
./Core/Src/taskCNIsendData.d \
./Core/Src/taskHeartbeat.d \
./Core/Src/taskIMUgetData.d \
./Core/Src/taskWatchdog.d \
./Core/Src/timeTriggeredScheduler.d \
./Core/Src/timeTriggeredTask.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/CNI.cyclo ./Core/Src/CNI.d ./Core/Src/CNI.o ./Core/Src/CNI.su ./Core/Src/can.cyclo ./Core/Src/can.d ./Core/Src/can.o ./Core/Src/can.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/gpio_STM32.cyclo ./Core/Src/gpio_STM32.d ./Core/Src/gpio_STM32.o ./Core/Src/gpio_STM32.su ./Core/Src/icm42688.cyclo ./Core/Src/icm42688.d ./Core/Src/icm42688.o ./Core/Src/icm42688.su ./Core/Src/icm42688_utils.cyclo ./Core/Src/icm42688_utils.d ./Core/Src/icm42688_utils.o ./Core/Src/icm42688_utils.su ./Core/Src/iwdg.cyclo ./Core/Src/iwdg.d ./Core/Src/iwdg.o ./Core/Src/iwdg.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/modeFailSilent.cyclo ./Core/Src/modeFailSilent.d ./Core/Src/modeFailSilent.o ./Core/Src/modeFailSilent.su ./Core/Src/modeNormal.cyclo ./Core/Src/modeNormal.d ./Core/Src/modeNormal.o ./Core/Src/modeNormal.su ./Core/Src/myUtils.cyclo ./Core/Src/myUtils.d ./Core/Src/myUtils.o ./Core/Src/myUtils.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system.cyclo ./Core/Src/system.d ./Core/Src/system.o ./Core/Src/system.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su ./Core/Src/taskCNIsendData.cyclo ./Core/Src/taskCNIsendData.d ./Core/Src/taskCNIsendData.o ./Core/Src/taskCNIsendData.su ./Core/Src/taskHeartbeat.cyclo ./Core/Src/taskHeartbeat.d ./Core/Src/taskHeartbeat.o ./Core/Src/taskHeartbeat.su ./Core/Src/taskIMUgetData.cyclo ./Core/Src/taskIMUgetData.d ./Core/Src/taskIMUgetData.o ./Core/Src/taskIMUgetData.su ./Core/Src/taskWatchdog.cyclo ./Core/Src/taskWatchdog.d ./Core/Src/taskWatchdog.o ./Core/Src/taskWatchdog.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/timeTriggeredScheduler.cyclo ./Core/Src/timeTriggeredScheduler.d ./Core/Src/timeTriggeredScheduler.o ./Core/Src/timeTriggeredScheduler.su ./Core/Src/timeTriggeredTask.cyclo ./Core/Src/timeTriggeredTask.d ./Core/Src/timeTriggeredTask.o ./Core/Src/timeTriggeredTask.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

