#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Librerias/ADC/ADC.c Librerias/EEPROM/EEPROM.c Librerias/INT/INT.c Librerias/MAX7219/Max7219.c Librerias/PCINT/PCINT.c Librerias/TIMER0/TIMER0.c Librerias/TIMER1/TIMER1.c Librerias/USART/USARTAtmega328P.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Librerias/ADC/ADC.o ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o ${OBJECTDIR}/Librerias/INT/INT.o ${OBJECTDIR}/Librerias/MAX7219/Max7219.o ${OBJECTDIR}/Librerias/PCINT/PCINT.o ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Librerias/ADC/ADC.o.d ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o.d ${OBJECTDIR}/Librerias/INT/INT.o.d ${OBJECTDIR}/Librerias/MAX7219/Max7219.o.d ${OBJECTDIR}/Librerias/PCINT/PCINT.o.d ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o.d ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o.d ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Librerias/ADC/ADC.o ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o ${OBJECTDIR}/Librerias/INT/INT.o ${OBJECTDIR}/Librerias/MAX7219/Max7219.o ${OBJECTDIR}/Librerias/PCINT/PCINT.o ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=Librerias/ADC/ADC.c Librerias/EEPROM/EEPROM.c Librerias/INT/INT.c Librerias/MAX7219/Max7219.c Librerias/PCINT/PCINT.c Librerias/TIMER0/TIMER0.c Librerias/TIMER1/TIMER1.c Librerias/USART/USARTAtmega328P.c main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATmega328P
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Librerias/ADC/ADC.o: Librerias/ADC/ADC.c  .generated_files/flags/default/dc3d254f828ac4a54ae89cfd7a40c9463ea29155 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/ADC" 
	@${RM} ${OBJECTDIR}/Librerias/ADC/ADC.o.d 
	@${RM} ${OBJECTDIR}/Librerias/ADC/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/ADC/ADC.o.d" -MT "${OBJECTDIR}/Librerias/ADC/ADC.o.d" -MT ${OBJECTDIR}/Librerias/ADC/ADC.o -o ${OBJECTDIR}/Librerias/ADC/ADC.o Librerias/ADC/ADC.c 
	
${OBJECTDIR}/Librerias/EEPROM/EEPROM.o: Librerias/EEPROM/EEPROM.c  .generated_files/flags/default/c0682e9bbaf646bbee8aeefad022da1d9b32dfc2 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/EEPROM" 
	@${RM} ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/EEPROM/EEPROM.o.d" -MT "${OBJECTDIR}/Librerias/EEPROM/EEPROM.o.d" -MT ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o -o ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o Librerias/EEPROM/EEPROM.c 
	
${OBJECTDIR}/Librerias/INT/INT.o: Librerias/INT/INT.c  .generated_files/flags/default/8b608e69cda59691098112087c11ecd074fb915 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/INT" 
	@${RM} ${OBJECTDIR}/Librerias/INT/INT.o.d 
	@${RM} ${OBJECTDIR}/Librerias/INT/INT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/INT/INT.o.d" -MT "${OBJECTDIR}/Librerias/INT/INT.o.d" -MT ${OBJECTDIR}/Librerias/INT/INT.o -o ${OBJECTDIR}/Librerias/INT/INT.o Librerias/INT/INT.c 
	
${OBJECTDIR}/Librerias/MAX7219/Max7219.o: Librerias/MAX7219/Max7219.c  .generated_files/flags/default/6070e2017acb20ff2039c537cf1951f11495d815 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/MAX7219" 
	@${RM} ${OBJECTDIR}/Librerias/MAX7219/Max7219.o.d 
	@${RM} ${OBJECTDIR}/Librerias/MAX7219/Max7219.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/MAX7219/Max7219.o.d" -MT "${OBJECTDIR}/Librerias/MAX7219/Max7219.o.d" -MT ${OBJECTDIR}/Librerias/MAX7219/Max7219.o -o ${OBJECTDIR}/Librerias/MAX7219/Max7219.o Librerias/MAX7219/Max7219.c 
	
${OBJECTDIR}/Librerias/PCINT/PCINT.o: Librerias/PCINT/PCINT.c  .generated_files/flags/default/952dbd9cdad0299f52adf3d006f69099515a6181 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/PCINT" 
	@${RM} ${OBJECTDIR}/Librerias/PCINT/PCINT.o.d 
	@${RM} ${OBJECTDIR}/Librerias/PCINT/PCINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/PCINT/PCINT.o.d" -MT "${OBJECTDIR}/Librerias/PCINT/PCINT.o.d" -MT ${OBJECTDIR}/Librerias/PCINT/PCINT.o -o ${OBJECTDIR}/Librerias/PCINT/PCINT.o Librerias/PCINT/PCINT.c 
	
${OBJECTDIR}/Librerias/TIMER0/TIMER0.o: Librerias/TIMER0/TIMER0.c  .generated_files/flags/default/a1a02a07e8fb363e2f9490065ee6ad708d05ed43 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/TIMER0" 
	@${RM} ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o.d 
	@${RM} ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/TIMER0/TIMER0.o.d" -MT "${OBJECTDIR}/Librerias/TIMER0/TIMER0.o.d" -MT ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o -o ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o Librerias/TIMER0/TIMER0.c 
	
${OBJECTDIR}/Librerias/TIMER1/TIMER1.o: Librerias/TIMER1/TIMER1.c  .generated_files/flags/default/1cada28a8d4ffb1959b4344ea6d8490b76601216 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/TIMER1" 
	@${RM} ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o.d 
	@${RM} ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/TIMER1/TIMER1.o.d" -MT "${OBJECTDIR}/Librerias/TIMER1/TIMER1.o.d" -MT ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o -o ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o Librerias/TIMER1/TIMER1.c 
	
${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o: Librerias/USART/USARTAtmega328P.c  .generated_files/flags/default/3532701e641c86c2500429d4ec819d0de50ef197 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/USART" 
	@${RM} ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o.d 
	@${RM} ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o.d" -MT "${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o.d" -MT ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o -o ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o Librerias/USART/USARTAtmega328P.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/c2cb458cb653ce5542bf3627e778d70e6a9bc37e .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
else
${OBJECTDIR}/Librerias/ADC/ADC.o: Librerias/ADC/ADC.c  .generated_files/flags/default/a4b2fd06ffb6def0089f60f731127606f50cf090 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/ADC" 
	@${RM} ${OBJECTDIR}/Librerias/ADC/ADC.o.d 
	@${RM} ${OBJECTDIR}/Librerias/ADC/ADC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/ADC/ADC.o.d" -MT "${OBJECTDIR}/Librerias/ADC/ADC.o.d" -MT ${OBJECTDIR}/Librerias/ADC/ADC.o -o ${OBJECTDIR}/Librerias/ADC/ADC.o Librerias/ADC/ADC.c 
	
${OBJECTDIR}/Librerias/EEPROM/EEPROM.o: Librerias/EEPROM/EEPROM.c  .generated_files/flags/default/5be94eeb3815a4c63b1fd457ad35cd5c5eb6b6f4 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/EEPROM" 
	@${RM} ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/EEPROM/EEPROM.o.d" -MT "${OBJECTDIR}/Librerias/EEPROM/EEPROM.o.d" -MT ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o -o ${OBJECTDIR}/Librerias/EEPROM/EEPROM.o Librerias/EEPROM/EEPROM.c 
	
${OBJECTDIR}/Librerias/INT/INT.o: Librerias/INT/INT.c  .generated_files/flags/default/fdbed1faddad856fd3b9e984ca18ca1cd3c7d750 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/INT" 
	@${RM} ${OBJECTDIR}/Librerias/INT/INT.o.d 
	@${RM} ${OBJECTDIR}/Librerias/INT/INT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/INT/INT.o.d" -MT "${OBJECTDIR}/Librerias/INT/INT.o.d" -MT ${OBJECTDIR}/Librerias/INT/INT.o -o ${OBJECTDIR}/Librerias/INT/INT.o Librerias/INT/INT.c 
	
${OBJECTDIR}/Librerias/MAX7219/Max7219.o: Librerias/MAX7219/Max7219.c  .generated_files/flags/default/e88bb201032b72be161ba6677feaa08145feb99e .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/MAX7219" 
	@${RM} ${OBJECTDIR}/Librerias/MAX7219/Max7219.o.d 
	@${RM} ${OBJECTDIR}/Librerias/MAX7219/Max7219.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/MAX7219/Max7219.o.d" -MT "${OBJECTDIR}/Librerias/MAX7219/Max7219.o.d" -MT ${OBJECTDIR}/Librerias/MAX7219/Max7219.o -o ${OBJECTDIR}/Librerias/MAX7219/Max7219.o Librerias/MAX7219/Max7219.c 
	
${OBJECTDIR}/Librerias/PCINT/PCINT.o: Librerias/PCINT/PCINT.c  .generated_files/flags/default/4c9a0d949235aa8a9621d17f5a1f13ccbe281a5a .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/PCINT" 
	@${RM} ${OBJECTDIR}/Librerias/PCINT/PCINT.o.d 
	@${RM} ${OBJECTDIR}/Librerias/PCINT/PCINT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/PCINT/PCINT.o.d" -MT "${OBJECTDIR}/Librerias/PCINT/PCINT.o.d" -MT ${OBJECTDIR}/Librerias/PCINT/PCINT.o -o ${OBJECTDIR}/Librerias/PCINT/PCINT.o Librerias/PCINT/PCINT.c 
	
${OBJECTDIR}/Librerias/TIMER0/TIMER0.o: Librerias/TIMER0/TIMER0.c  .generated_files/flags/default/45928d669c970be0a1084122cf7664922571aee7 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/TIMER0" 
	@${RM} ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o.d 
	@${RM} ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/TIMER0/TIMER0.o.d" -MT "${OBJECTDIR}/Librerias/TIMER0/TIMER0.o.d" -MT ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o -o ${OBJECTDIR}/Librerias/TIMER0/TIMER0.o Librerias/TIMER0/TIMER0.c 
	
${OBJECTDIR}/Librerias/TIMER1/TIMER1.o: Librerias/TIMER1/TIMER1.c  .generated_files/flags/default/bc46e8af5e424123934792932b20346330b9efb0 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/TIMER1" 
	@${RM} ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o.d 
	@${RM} ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/TIMER1/TIMER1.o.d" -MT "${OBJECTDIR}/Librerias/TIMER1/TIMER1.o.d" -MT ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o -o ${OBJECTDIR}/Librerias/TIMER1/TIMER1.o Librerias/TIMER1/TIMER1.c 
	
${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o: Librerias/USART/USARTAtmega328P.c  .generated_files/flags/default/e9ce404e4db12c2894c43b9a18183962fb08286b .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}/Librerias/USART" 
	@${RM} ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o.d 
	@${RM} ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o.d" -MT "${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o.d" -MT ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o -o ${OBJECTDIR}/Librerias/USART/USARTAtmega328P.o Librerias/USART/USARTAtmega328P.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/619c3a5edf0ef208d989c7c61de7171e35341760 .generated_files/flags/default/2f9e58fd8ce1a2611b0e0fd33168db129b9285f3
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -funsigned-char -funsigned-bitfields -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/Conversor.X.${IMAGE_TYPE}.hex"
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
