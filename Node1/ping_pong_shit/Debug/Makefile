################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Drivers \
../Credits


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Credits/credits.c \
../Drivers/button_driver.c \
../Drivers/can_driver.c \
../Drivers/joystick_driver.c \
../Drivers/mcp2515_driver.c \
../Drivers/oled_driver.c \
../Drivers/spi_driver.c \
../Drivers/SRAM_driver.c \
../Drivers/uart_driver.c \
../game.c \
../main.c \
../menu.c \
../test_code.c \
../timer.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Credits/credits.o \
Drivers/button_driver.o \
Drivers/can_driver.o \
Drivers/joystick_driver.o \
Drivers/mcp2515_driver.o \
Drivers/oled_driver.o \
Drivers/spi_driver.o \
Drivers/SRAM_driver.o \
Drivers/uart_driver.o \
game.o \
main.o \
menu.o \
test_code.o \
timer.o

OBJS_AS_ARGS +=  \
Credits/credits.o \
Drivers/button_driver.o \
Drivers/can_driver.o \
Drivers/joystick_driver.o \
Drivers/mcp2515_driver.o \
Drivers/oled_driver.o \
Drivers/spi_driver.o \
Drivers/SRAM_driver.o \
Drivers/uart_driver.o \
game.o \
main.o \
menu.o \
test_code.o \
timer.o

C_DEPS +=  \
Credits/credits.d \
Drivers/button_driver.d \
Drivers/can_driver.d \
Drivers/joystick_driver.d \
Drivers/mcp2515_driver.d \
Drivers/oled_driver.d \
Drivers/spi_driver.d \
Drivers/SRAM_driver.d \
Drivers/uart_driver.d \
game.d \
main.d \
menu.d \
test_code.d \
timer.d

C_DEPS_AS_ARGS +=  \
Credits/credits.d \
Drivers/button_driver.d \
Drivers/can_driver.d \
Drivers/joystick_driver.d \
Drivers/mcp2515_driver.d \
Drivers/oled_driver.d \
Drivers/spi_driver.d \
Drivers/SRAM_driver.d \
Drivers/uart_driver.d \
game.d \
main.d \
menu.d \
test_code.d \
timer.d

OUTPUT_FILE_PATH +=Ping_Pong_Shit.elf

OUTPUT_FILE_PATH_AS_ARGS +=Ping_Pong_Shit.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





























Credits/%.o: ../Credits/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=4915200 -DF_OSC=4915200  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega162 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Drivers/%.o: ../Drivers/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=4915200 -DF_OSC=4915200  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega162 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=4915200 -DF_OSC=4915200  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega162 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Ping_Pong_Shit.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega162  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Ping_Pong_Shit.elf" "Ping_Pong_Shit.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Ping_Pong_Shit.elf" "Ping_Pong_Shit.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "Ping_Pong_Shit.elf" > "Ping_Pong_Shit.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Ping_Pong_Shit.elf" "Ping_Pong_Shit.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "Ping_Pong_Shit.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Ping_Pong_Shit.elf" "Ping_Pong_Shit.a" "Ping_Pong_Shit.hex" "Ping_Pong_Shit.lss" "Ping_Pong_Shit.eep" "Ping_Pong_Shit.map" "Ping_Pong_Shit.srec" "Ping_Pong_Shit.usersignatures"
	