################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./F2837xS_Adc.obj" \
"./F2837xS_CodeStartBranch.obj" \
"./F2837xS_CpuTimers.obj" \
"./F2837xS_DBGIER.obj" \
"./F2837xS_DefaultISR.obj" \
"./F2837xS_GlobalVariableDefs.obj" \
"./F2837xS_Gpio.obj" \
"./F2837xS_PieCtrl.obj" \
"./F2837xS_PieVect.obj" \
"./F2837xS_SysCtrl.obj" \
"./F2837xS_struct.obj" \
"./F2837xS_usDelay.obj" \
"./can.obj" \
"./fonctionMoteur.obj" \
"./interrupt.obj" \
"./main.obj" \
$(GEN_CMDS__FLAG) \
-l"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/lib/rts2800_fpu32.lib" \
-l"C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_common/cmd/2837xS_Generic_RAM_lnk.cmd" \
-l"C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_headers/cmd/F2837xS_Headers_nonBIOS.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
test_f28377s.out \

EXE_OUTPUTS__QUOTED += \
"test_f28377s.out" \

BIN_OUTPUTS += \
test_f28377s.hex \

BIN_OUTPUTS__QUOTED += \
"test_f28377s.hex" \


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "test_f28377s.out" secondary-outputs

# Tool invocations
test_f28377s.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"test_f28377s.map" --warn_sections -i"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/lib" -i"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" -i"C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_common/cmd" -i"C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_headers/cmd" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="test_f28377s_linkInfo.xml" --entry_point=code_start --rom_model -o "test_f28377s.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

test_f28377s.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/hex2000" --boot --sci8 --ascii -o "test_f28377s.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "F2837xS_Adc.obj" "F2837xS_CodeStartBranch.obj" "F2837xS_CpuTimers.obj" "F2837xS_DBGIER.obj" "F2837xS_DefaultISR.obj" "F2837xS_GlobalVariableDefs.obj" "F2837xS_Gpio.obj" "F2837xS_PieCtrl.obj" "F2837xS_PieVect.obj" "F2837xS_SysCtrl.obj" "F2837xS_struct.obj" "F2837xS_usDelay.obj" "can.obj" "fonctionMoteur.obj" "interrupt.obj" "main.obj" 
	-$(RM) "F2837xS_Adc.d" "F2837xS_CpuTimers.d" "F2837xS_DefaultISR.d" "F2837xS_GlobalVariableDefs.d" "F2837xS_Gpio.d" "F2837xS_PieCtrl.d" "F2837xS_PieVect.d" "F2837xS_SysCtrl.d" "F2837xS_struct.d" "can.d" "fonctionMoteur.d" "interrupt.d" "main.d" 
	-$(RM) "F2837xS_CodeStartBranch.d" "F2837xS_DBGIER.d" "F2837xS_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

