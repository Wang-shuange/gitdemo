cls
@echo off
STMFlashLoader.exe -c --pn 46 --br 115200 --db 8 --pr EVEN --sb 1 --ec OFF  --to 10000 --co ON  -Dtr --Hi -Rts --Lo  -i STM32_High-density_256K -e --all -d --a @08000000 --fn c:\APP.hex  -Dtr --Lo -Rts --Hi --Lo -o --get --fn c:\option.txt >info.xls 
PAUSE 