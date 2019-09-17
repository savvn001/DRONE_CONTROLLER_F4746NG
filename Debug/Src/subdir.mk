################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f7xx.c 

OBJS += \
./Src/main.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f7xx.o 

C_DEPS += \
./Src/main.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f7xx_hal_msp.o: ../Src/stm32f7xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f7xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f7xx_it.o: ../Src/stm32f7xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f7xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f7xx.o: ../Src/system_stm32f7xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f7xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

