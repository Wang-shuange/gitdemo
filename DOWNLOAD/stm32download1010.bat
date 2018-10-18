cls
@echo off
STMFlashLoader.exe -c --pn 14 --br 256000 --db 8 --pr EVEN --sb 1  --to 2000   -i STM32_High-density_256K -e --all -d --a @08000000 --fn c:\APP.hex 


