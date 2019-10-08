################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/BoardSupport.c" \
"../Sources/DCMOTORSandENCODERS.c" \
"../Sources/ISR.c" \
"../Sources/Input_Capture.c" \
"../Sources/ServoMotors.c" \
"../Sources/UART.c" \
"../Sources/arm_cm0.c" \
"../Sources/colorSensor.c" \
"../Sources/main.c" \
"../Sources/mcg.c" \
"../Sources/sa_mtb.c" \
"../Sources/ultraSonic.c" \

C_SRCS += \
../Sources/BoardSupport.c \
../Sources/DCMOTORSandENCODERS.c \
../Sources/ISR.c \
../Sources/Input_Capture.c \
../Sources/ServoMotors.c \
../Sources/UART.c \
../Sources/arm_cm0.c \
../Sources/colorSensor.c \
../Sources/main.c \
../Sources/mcg.c \
../Sources/sa_mtb.c \
../Sources/ultraSonic.c \

OBJS += \
./Sources/BoardSupport.o \
./Sources/DCMOTORSandENCODERS.o \
./Sources/ISR.o \
./Sources/Input_Capture.o \
./Sources/ServoMotors.o \
./Sources/UART.o \
./Sources/arm_cm0.o \
./Sources/colorSensor.o \
./Sources/main.o \
./Sources/mcg.o \
./Sources/sa_mtb.o \
./Sources/ultraSonic.o \

C_DEPS += \
./Sources/BoardSupport.d \
./Sources/DCMOTORSandENCODERS.d \
./Sources/ISR.d \
./Sources/Input_Capture.d \
./Sources/ServoMotors.d \
./Sources/UART.d \
./Sources/arm_cm0.d \
./Sources/colorSensor.d \
./Sources/main.d \
./Sources/mcg.d \
./Sources/sa_mtb.d \
./Sources/ultraSonic.d \

OBJS_QUOTED += \
"./Sources/BoardSupport.o" \
"./Sources/DCMOTORSandENCODERS.o" \
"./Sources/ISR.o" \
"./Sources/Input_Capture.o" \
"./Sources/ServoMotors.o" \
"./Sources/UART.o" \
"./Sources/arm_cm0.o" \
"./Sources/colorSensor.o" \
"./Sources/main.o" \
"./Sources/mcg.o" \
"./Sources/sa_mtb.o" \
"./Sources/ultraSonic.o" \

C_DEPS_QUOTED += \
"./Sources/BoardSupport.d" \
"./Sources/DCMOTORSandENCODERS.d" \
"./Sources/ISR.d" \
"./Sources/Input_Capture.d" \
"./Sources/ServoMotors.d" \
"./Sources/UART.d" \
"./Sources/arm_cm0.d" \
"./Sources/colorSensor.d" \
"./Sources/main.d" \
"./Sources/mcg.d" \
"./Sources/sa_mtb.d" \
"./Sources/ultraSonic.d" \

OBJS_OS_FORMAT += \
./Sources/BoardSupport.o \
./Sources/DCMOTORSandENCODERS.o \
./Sources/ISR.o \
./Sources/Input_Capture.o \
./Sources/ServoMotors.o \
./Sources/UART.o \
./Sources/arm_cm0.o \
./Sources/colorSensor.o \
./Sources/main.o \
./Sources/mcg.o \
./Sources/sa_mtb.o \
./Sources/ultraSonic.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/BoardSupport.o: ../Sources/BoardSupport.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/BoardSupport.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/BoardSupport.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DCMOTORSandENCODERS.o: ../Sources/DCMOTORSandENCODERS.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/DCMOTORSandENCODERS.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/DCMOTORSandENCODERS.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/ISR.o: ../Sources/ISR.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ISR.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ISR.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Input_Capture.o: ../Sources/Input_Capture.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Input_Capture.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Input_Capture.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/ServoMotors.o: ../Sources/ServoMotors.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ServoMotors.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ServoMotors.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/UART.o: ../Sources/UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/UART.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/UART.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/arm_cm0.o: ../Sources/arm_cm0.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/arm_cm0.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/arm_cm0.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/colorSensor.o: ../Sources/colorSensor.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/colorSensor.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/colorSensor.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/mcg.o: ../Sources/mcg.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/mcg.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/mcg.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/ultraSonic.o: ../Sources/ultraSonic.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ultraSonic.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ultraSonic.o"
	@echo 'Finished building: $<'
	@echo ' '


