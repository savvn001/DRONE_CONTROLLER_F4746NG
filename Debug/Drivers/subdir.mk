################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MY_NRF24.c \
../Drivers/UI.c \
../Drivers/dwt_delay.c 

OBJS += \
./Drivers/MY_NRF24.o \
./Drivers/UI.o \
./Drivers/dwt_delay.o 

C_DEPS += \
./Drivers/MY_NRF24.d \
./Drivers/UI.d \
./Drivers/dwt_delay.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MY_NRF24.o: ../Drivers/MY_NRF24.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/MY_NRF24.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/UI.o: ../Drivers/UI.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/UI.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/dwt_delay.o: ../Drivers/dwt_delay.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I/Users/nick_savva/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FatFs -I../Inc -I../Drivers/CMSIS/Include -I"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/GUI" -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/dwt_delay.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

