################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CalculateMandelbrot.c \
../Callbacks_lib.c \
../Mandelbrot_lib.c \
../UIBuilder_lib.c \
../main.c 

OBJS += \
./CalculateMandelbrot.o \
./Callbacks_lib.o \
./Mandelbrot_lib.o \
./UIBuilder_lib.o \
./main.o 

C_DEPS += \
./CalculateMandelbrot.d \
./Callbacks_lib.d \
./Mandelbrot_lib.d \
./UIBuilder_lib.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc `pkg-config --cflags --libs gtk+-2.0`  -pthread -I"/acct/s1/mauron/workspace/dasc" -I"/usr/include/gtk-2.0" -I"/acct/s1/mauron/workspace/dasc/includes" -I"/usr/lib/x86_64-linux-gnu/gtk-2.0/include" -I"/usr/include/atk-1.0" -I"/usr/include/cairo" -I"/usr/include/gdk-pixbuf-2.0" -I"/usr/include/pango-1.0" -I"/usr/include/gio-unix-2.0/" -I"/usr/include/glib-2.0" -I"/usr/lib/x86_64-linux-gnu/glib-2.0/include" -I"/usr/include/pixman-1" -I"/usr/include/freetype2" -I"/usr/include/libpng12" -O0 -g3 -Wall -c -fmessage-length=0 -fopenmp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


