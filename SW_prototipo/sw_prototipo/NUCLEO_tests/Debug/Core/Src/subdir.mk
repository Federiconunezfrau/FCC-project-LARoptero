################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/TTscheduler.cpp \
../Core/Src/TTsystem_init.cpp \
../Core/Src/TTsystem_mode_normal.cpp \
../Core/Src/TTtask_heartbeat.cpp \
../Core/Src/gpio_STM32.cpp \
../Core/Src/iStateMachine.cpp \
../Core/Src/icm42688.cpp \
../Core/Src/icm42688_utils.cpp \
../Core/Src/interface_TTtask.cpp \
../Core/Src/interface_observer.cpp \
../Core/Src/main.cpp \
../Core/Src/system.cpp 

C_SRCS += \
../Core/Src/gpio.c \
../Core/Src/spi.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

C_DEPS += \
./Core/Src/gpio.d \
./Core/Src/spi.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 

OBJS += \
./Core/Src/TTscheduler.o \
./Core/Src/TTsystem_init.o \
./Core/Src/TTsystem_mode_normal.o \
./Core/Src/TTtask_heartbeat.o \
./Core/Src/gpio.o \
./Core/Src/gpio_STM32.o \
./Core/Src/iStateMachine.o \
./Core/Src/icm42688.o \
./Core/Src/icm42688_utils.o \
./Core/Src/interface_TTtask.o \
./Core/Src/interface_observer.o \
./Core/Src/main.o \
./Core/Src/spi.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system.o \
./Core/Src/system_stm32f7xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

CPP_DEPS += \
./Core/Src/TTscheduler.d \
./Core/Src/TTsystem_init.d \
./Core/Src/TTsystem_mode_normal.d \
./Core/Src/TTtask_heartbeat.d \
./Core/Src/gpio_STM32.d \
./Core/Src/iStateMachine.d \
./Core/Src/icm42688.d \
./Core/Src/icm42688_utils.d \
./Core/Src/interface_TTtask.d \
./Core/Src/interface_observer.d \
./Core/Src/main.d \
./Core/Src/system.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/TTscheduler.cyclo ./Core/Src/TTscheduler.d ./Core/Src/TTscheduler.o ./Core/Src/TTscheduler.su ./Core/Src/TTsystem_init.cyclo ./Core/Src/TTsystem_init.d ./Core/Src/TTsystem_init.o ./Core/Src/TTsystem_init.su ./Core/Src/TTsystem_mode_normal.cyclo ./Core/Src/TTsystem_mode_normal.d ./Core/Src/TTsystem_mode_normal.o ./Core/Src/TTsystem_mode_normal.su ./Core/Src/TTtask_heartbeat.cyclo ./Core/Src/TTtask_heartbeat.d ./Core/Src/TTtask_heartbeat.o ./Core/Src/TTtask_heartbeat.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/gpio_STM32.cyclo ./Core/Src/gpio_STM32.d ./Core/Src/gpio_STM32.o ./Core/Src/gpio_STM32.su ./Core/Src/iStateMachine.cyclo ./Core/Src/iStateMachine.d ./Core/Src/iStateMachine.o ./Core/Src/iStateMachine.su ./Core/Src/icm42688.cyclo ./Core/Src/icm42688.d ./Core/Src/icm42688.o ./Core/Src/icm42688.su ./Core/Src/icm42688_utils.cyclo ./Core/Src/icm42688_utils.d ./Core/Src/icm42688_utils.o ./Core/Src/icm42688_utils.su ./Core/Src/interface_TTtask.cyclo ./Core/Src/interface_TTtask.d ./Core/Src/interface_TTtask.o ./Core/Src/interface_TTtask.su ./Core/Src/interface_observer.cyclo ./Core/Src/interface_observer.d ./Core/Src/interface_observer.o ./Core/Src/interface_observer.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system.cyclo ./Core/Src/system.d ./Core/Src/system.o ./Core/Src/system.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

