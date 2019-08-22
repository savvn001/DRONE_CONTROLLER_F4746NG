################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include GUI\ files/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Drivers/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
DRONE_CONTROLLER_F4746NG.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
DRONE_CONTROLLER_F4746NG.list \


# All Target
all: DRONE_CONTROLLER_F4746NG.elf secondary-outputs

# Tool invocations
DRONE_CONTROLLER_F4746NG.elf: $(OBJS) $(USER_OBJS) /Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/STM32F746NGHX_FLASH.ld
	arm-none-eabi-gcc -o "DRONE_CONTROLLER_F4746NG.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"/Users/nick_savva/STM32CubeIDE/workspace_1.0.1/DRONE_CONTROLLER_F4746NG/STM32F746NGHX_FLASH.ld" --specs=nosys.specs -Wl,-Map="DRONE_CONTROLLER_F4746NG.map" -Wl,--gc-sections -static -u _scanf_float -u _printf_float --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

DRONE_CONTROLLER_F4746NG.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "DRONE_CONTROLLER_F4746NG.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
