################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SDL/SDL_win32_main.c 

OBJ_SRCS += \
../src/SDL/SDL_win32_main.obj 

OBJS += \
./src/SDL/SDL_win32_main.o 

C_DEPS += \
./src/SDL/SDL_win32_main.d 


# Each subdirectory must supply rules for building sources it contributes
src/SDL/%.o: ../src/SDL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


