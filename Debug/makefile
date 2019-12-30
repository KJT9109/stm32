################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Middlewares/Third_Party/LwIP/src/netif/ppp/subdir.mk
-include Middlewares/Third_Party/LwIP/src/netif/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/ipv6/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/ipv4/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/subdir.mk
-include Middlewares/Third_Party/LwIP/src/apps/mqtt/subdir.mk
-include Middlewares/Third_Party/LwIP/src/api/subdir.mk
-include LWIP/Target/subdir.mk
-include LWIP/App/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Src/subdir.mk
-include Arducam/subdir.mk
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
STM32F4_5MP_OV5642_TCP.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32F4_5MP_OV5642_TCP.list \


# All Target
all: STM32F4_5MP_OV5642_TCP.elf secondary-outputs

# Tool invocations
STM32F4_5MP_OV5642_TCP.elf: $(OBJS) $(USER_OBJS) C:\Users\rlawl\Workplaces\STM32F4_5MP_OV5642_TCP\STM32F407VETX_FLASH.ld
	arm-none-eabi-gcc -o "STM32F4_5MP_OV5642_TCP.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\rlawl\Workplaces\STM32F4_5MP_OV5642_TCP\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32F4_5MP_OV5642_TCP.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32F4_5MP_OV5642_TCP.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32F4_5MP_OV5642_TCP.list"
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
