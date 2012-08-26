################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MandelbrotBenchmark

dependents:
	-cd /acct/s1/mauron/workspace/dasc/Debug && $(MAKE) all

# Tool invocations
MandelbrotBenchmark: $(OBJS) $(USER_OBJS) /acct/s1/mauron/workspace/dasc/Debug/libdasc.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -fopenmp -L"/acct/s1/mauron/workspace/dasc/Debug" -fopenmp -o "MandelbrotBenchmark" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) MandelbrotBenchmark
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/acct/s1/mauron/workspace/dasc/Debug/libdasc.so:

-include ../makefile.targets
