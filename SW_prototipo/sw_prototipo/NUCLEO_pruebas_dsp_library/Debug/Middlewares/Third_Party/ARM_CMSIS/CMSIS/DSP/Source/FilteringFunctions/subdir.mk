################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.c \
../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctionsF16.c 

C_DEPS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.d \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctionsF16.d 

OBJS += \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.o \
./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctionsF16.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/%.o Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/%.su Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/%.cyclo: ../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/%.c Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core_A/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/PrivateInclude/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-FilteringFunctions

clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-FilteringFunctions:
	-$(RM) ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.cyclo ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.su ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctionsF16.cyclo ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctionsF16.d ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctionsF16.o ./Middlewares/Third_Party/ARM_CMSIS/CMSIS/DSP/Source/FilteringFunctions/FilteringFunctionsF16.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-ARM_CMSIS-2f-CMSIS-2f-DSP-2f-Source-2f-FilteringFunctions

