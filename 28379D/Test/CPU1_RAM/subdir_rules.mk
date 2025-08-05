################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1452482673: ../c2000.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/CCS_TI_Tools/ccs2020/ccs/utils/sysconfig_1.24.0/sysconfig_cli.bat" --script "D:/NUEDC/28379D/Test/c2000.syscfg" -o "syscfg" -s "D:/CCS_TI_Tools/C2000Ware_5_05_00_00/.metadata/sdk.json" -d "F2837xD" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-1452482673 ../c2000.syscfg
syscfg/board.h: build-1452482673
syscfg/board.cmd.genlibs: build-1452482673
syscfg/board.opt: build-1452482673
syscfg/board.json: build-1452482673
syscfg/pinmux.csv: build-1452482673
syscfg/c2000ware_libraries.cmd.genlibs: build-1452482673
syscfg/c2000ware_libraries.opt: build-1452482673
syscfg/c2000ware_libraries.c: build-1452482673
syscfg/c2000ware_libraries.h: build-1452482673
syscfg/clocktree.h: build-1452482673
syscfg: build-1452482673

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS_TI_Tools/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="D:/NUEDC/28379D/Test" --include_path="D:/NUEDC/28379D/Test/device" --include_path="D:/CCS_TI_Tools/C2000Ware_5_05_00_00/driverlib/f2837xd/driverlib/" --include_path="D:/CCS_TI_Tools/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="D:/NUEDC/28379D/Test/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS_TI_Tools/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="D:/NUEDC/28379D/Test" --include_path="D:/NUEDC/28379D/Test/device" --include_path="D:/CCS_TI_Tools/C2000Ware_5_05_00_00/driverlib/f2837xd/driverlib/" --include_path="D:/CCS_TI_Tools/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/NUEDC/28379D/Test/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


