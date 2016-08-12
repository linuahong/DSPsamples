################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"D:/CCS5.5.0/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6740 -g --define=omapl138 --include_path="D:/CCS5.5.0/ccsv5/tools/compiler/c6000_7.4.4/include" --include_path="../include" --include_path="../include/hw" --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


