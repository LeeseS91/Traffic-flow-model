################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SPG/SPG_extended.c \
../src/SPG/SPG_misc.c \
../src/SPG/SPG_polygon.c \
../src/SPG/SPG_primitives.c \
../src/SPG/SPG_rotation.c \
../src/SPG/SPG_surface.c 

OBJS += \
./src/SPG/SPG_extended.o \
./src/SPG/SPG_misc.o \
./src/SPG/SPG_polygon.o \
./src/SPG/SPG_primitives.o \
./src/SPG/SPG_rotation.o \
./src/SPG/SPG_surface.o 

C_DEPS += \
./src/SPG/SPG_extended.d \
./src/SPG/SPG_misc.d \
./src/SPG/SPG_polygon.d \
./src/SPG/SPG_primitives.d \
./src/SPG/SPG_rotation.d \
./src/SPG/SPG_surface.d 


# Each subdirectory must supply rules for building sources it contributes
src/SPG/%.o: ../src/SPG/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


