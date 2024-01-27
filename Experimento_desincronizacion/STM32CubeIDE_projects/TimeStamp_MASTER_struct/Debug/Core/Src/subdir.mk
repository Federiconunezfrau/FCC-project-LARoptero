################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/CANmaster.cpp \
../Core/Src/CANmsg.cpp \
../Core/Src/PCIF.cpp \
../Core/Src/PCIFmsg.cpp \
../Core/Src/gpio_STM32.cpp \
../Core/Src/main.cpp \
../Core/Src/stateMachine.cpp \
../Core/Src/timeStampService.cpp 

C_SRCS += \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c 

C_DEPS += \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d 

OBJS += \
./Core/Src/CANmaster.o \
./Core/Src/CANmsg.o \
./Core/Src/PCIF.o \
./Core/Src/PCIFmsg.o \
./Core/Src/gpio_STM32.o \
./Core/Src/main.o \
./Core/Src/stateMachine.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o \
./Core/Src/timeStampService.o 

CPP_DEPS += \
./Core/Src/CANmaster.d \
./Core/Src/CANmsg.d \
./Core/Src/PCIF.d \
./Core/Src/PCIFmsg.d \
./Core/Src/gpio_STM32.d \
./Core/Src/main.d \
./Core/Src/stateMachine.d \
./Core/Src/timeStampService.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/CANmaster.cyclo ./Core/Src/CANmaster.d ./Core/Src/CANmaster.o ./Core/Src/CANmaster.su ./Core/Src/CANmsg.cyclo ./Core/Src/CANmsg.d ./Core/Src/CANmsg.o ./Core/Src/CANmsg.su ./Core/Src/PCIF.cyclo ./Core/Src/PCIF.d ./Core/Src/PCIF.o ./Core/Src/PCIF.su ./Core/Src/PCIFmsg.cyclo ./Core/Src/PCIFmsg.d ./Core/Src/PCIFmsg.o ./Core/Src/PCIFmsg.su ./Core/Src/gpio_STM32.cyclo ./Core/Src/gpio_STM32.d ./Core/Src/gpio_STM32.o ./Core/Src/gpio_STM32.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stateMachine.cyclo ./Core/Src/stateMachine.d ./Core/Src/stateMachine.o ./Core/Src/stateMachine.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su ./Core/Src/timeStampService.cyclo ./Core/Src/timeStampService.d ./Core/Src/timeStampService.o ./Core/Src/timeStampService.su

.PHONY: clean-Core-2f-Src

