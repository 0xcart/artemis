@rem Licensed under the terms of the MIT license reproduced below.
@rem
@rem Permission is hereby granted, free of charge, to any person obtaining a copy of
@rem this software and associated documentation files (the "Software"), to deal in
@rem the Software without restriction, including without limitation the rights to
@rem use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
@rem the Software, and to permit persons to whom the Software is furnished to do so,
@rem subject to the following conditions:
@rem
@rem The above copyright notice and this permission notice shall be included in all
@rem copies or substantial portions of the Software.
@rem
@rem THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
@rem IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
@rem FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
@rem COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
@rem IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
@rem CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

@rem Usage: build.bat [help|svl|clean|bootload]

@echo off
@setlocal enableextensions
@setlocal enabledelayedexpansion

@set BLD_THIS=%~nx0
@set BLD_BOARD=artemis_thing_plus
@set BLD_PART=APOLLO3
@set BLD_MCU=apollo3
@set BLD_CPU=cortex-m4
@set BLD_FPU=fpv4-sp-d16
@set BLD_FABI=hard
@set BLD_BINPATH=bin
@set BLD_SRCPATH=src
@set BLD_SDKPATH=AmbiqSuiteSDK
@set BLD_BOARDPATH=%BLD_SDKPATH%/boards_sfe/%BLD_BOARD%
@set BLD_TEMPLATEPATH=%BLD_SDKPATH%/boards_sfe/common/tools_sfe/templates
@set BLD_TOOLCHAIN=arm-none-eabi
@set BLD_CC=%BLD_TOOLCHAIN%-gcc
@set BLD_LD=%BLD_TOOLCHAIN%-ld
@set BLD_OC=%BLD_TOOLCHAIN%-objcopy
@set BLD_OD=%BLD_TOOLCHAIN%-objdump
@set BLD_AR=%BLD_TOOLCHAIN%-ar
@set BLD_OUTPUT=output
@set BLD_INCLUDE=-I%BLD_SRCPATH% -I%BLD_BOARDPATH%/bsp -I%BLD_SDKPATH%/utils -I%BLD_SDKPATH%/devices -I%BLD_SDKPATH%/mcu/%BLD_MCU% -I%BLD_SDKPATH%/CMSIS/AmbiqMicro/Include -I%BLD_SDKPATH%/CMSIS/ARM/Include
@set BLD_SOURCE=src/main.c %BLD_TEMPLATEPATH%/startup_gcc.c %BLD_SDKPATH%/devices/am_devices_led.c %BLD_SDKPATH%/utils/am_util_delay.c %BLD_SDKPATH%/utils/am_util_stdio.c
@set BLD_LIBRARY=%BLD_BOARDPATH%/bsp/gcc/bin/libam_bsp.a %BLD_SDKPATH%/mcu/%BLD_MCU%/hal/gcc/bin/libam_hal.a
@set BLD_DEFINE=-DPART_%BLD_PART% -DAM_PART_%BLD_PART% -DAM_CUSTOM_BDADDR -DAM_PACKAGE_BGA -DAM_DEBUG_PRINTF -DWSF_TRACE_ENABLED
@set BLD_CCFLAG=-mthumb -mcpu=%BLD_CPU% -mfpu=%BLD_FPU% -mfloat-abi=%BLD_FABI% -ffunction-sections -fdata-sections -std=c99 -Wall -g -O0 %BLD_DEFINE% %BLD_INCLUDE%
@set BLD_LDFLAG=-mthumb -mcpu=%BLD_CPU% -mfpu=%BLD_FPU% -mfloat-abi=%BLD_FABI% -nostartfiles -static -Wl,--gc-sections,--entry,Reset_Handler,-Map,%BLD_BINPATH%/%BLD_OUTPUT%_svl.map -Wl,--start-group -lm -lc -lgcc %BLD_LIBRARY% -Wl,--end-group

@rem Verify the build environment
%BLD_CC% --version >nul 2>&1
@if !ERRORLEVEL! neq 0 goto :BLD_CC_ERROR
@goto :BLD_ARGUMENTS

:BLD_CC_ERROR
@echo.
@echo  *** INVALID BUILD ENVIRONMENT ***
@echo.
@echo  Could not execute '%BLD_CC% --version'
@goto :BLD_END

:BLD_ARGUMENTS
@if [%1] equ [] goto :BLD_SVL
@if "%1" equ "help" goto :BLD_HELP
@if "%1" equ "svl" goto :BLD_SVL
@if "%1" equ "clean" goto :BLD_CLEAN
@if "%1" equ "bootload" goto :BLD_BOOTLOAD
@goto :BLD_INVALID

:BLD_INVALID
@echo.
@echo  *** INVALID ARGUMENT ***
@goto :BLD_HELP

:BLD_HELP
@echo.
@echo  Usage: %BLD_THIS% [help^|svl^|clean^|bootload]
@echo    help      print these help details
@echo    svl       create binary for use with SF SVL bootloader (default)
@echo    clean     delete all intermediate and binary files
@echo    bootload  load binary using the SF SVL bootloader
@goto :BLD_END

:BLD_SVL
@echo.
@echo  Making    '%BLD_BINPATH%' directory
@if not exist "%BLD_BINPATH%" mkdir "%BLD_BINPATH%"
@if !ERRORLEVEL! neq 0 goto :BLD_MKDIR_ERROR
@set BLD_OBJECT=
@for %%F in (%BLD_SOURCE%) do (
	set BLD_OBJECT=!BLD_OBJECT! %BLD_BINPATH%/%%~nF.o
	@echo  Compiling '%%F'
	%BLD_CC% -c %BLD_CCFLAG% %%F -o %BLD_BINPATH%/%%~nF.o
	@if !ERRORLEVEL! neq 0 goto :BLD_FAILURE
)
@echo  Linking   '%BLD_BINPATH%/%BLD_OUTPUT%_svl.axf' with script '%BLD_TEMPLATEPATH%/asb_svl_linker.ld'
%BLD_CC% -Wl,-T,%BLD_TEMPLATEPATH%/asb_svl_linker.ld -o %BLD_BINPATH%/%BLD_OUTPUT%_svl.axf %BLD_OBJECT% %BLD_LDFLAG%
@if !ERRORLEVEL! neq 0 goto :BLD_FAILURE
@echo  Creating  '%BLD_BINPATH%/%BLD_OUTPUT%_svl.lst'
%BLD_OD% -S %BLD_BINPATH%/%BLD_OUTPUT%_svl.axf > %BLD_BINPATH%/%BLD_OUTPUT%_svl.lst
@if !ERRORLEVEL! neq 0 goto :BLD_FAILURE
@echo  Creating  '%BLD_BINPATH%/%BLD_OUTPUT%_svl.bin'
%BLD_OC% -Obinary %BLD_BINPATH%/%BLD_OUTPUT%_svl.axf %BLD_BINPATH%/%BLD_OUTPUT%_svl.bin
@if !ERRORLEVEL! neq 0 goto :BLD_FAILURE
@goto :BLD_SUCCESS

:BLD_MKDIR_ERROR
@echo.
@echo  *** 'mkdir' FAILED ***
@echo.
@echo Could not create '%BLD_BINPATH%' directory
@goto :BLD_END

:BLD_FAILURE
@echo.
@echo  *** BUILD FAILED ***
@goto :BLD_END

:BLD_SUCCESS
@echo.
@echo  === BUILD SUCCESSFUL ===
@goto :BLD_END

:BLD_CLEAN
@echo.
@echo  Removing '%BLD_BINPATH%' directory
@if exist "%BLD_BINPATH%" rmdir /S /Q "%BLD_BINPATH%"
@if !ERRORLEVEL! neq 0 goto :BLD_RMDIR_ERROR
@goto :BLD_END

:BLD_RMDIR_ERROR
@echo.
@echo  *** 'rmdir' FAILED ***
@echo.
@echo Could not remove '%BLD_BINPATH%' directory
@goto :BLD_END

:BLD_BOOTLOAD
@goto :BLD_END

:BLD_END