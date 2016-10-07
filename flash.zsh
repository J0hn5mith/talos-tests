JLinkGDBServer -device CC2538SF53 &
tmp=$!

echo "target remote localhost:2331
monitor interface jtag
monitor speed 5000
monitor endian little
monitor flash download = 1
monitor flash breakpoints = 1
monitor reset
load $1.elf
disconnect
quit" > .tmp.gdb
arm-none-eabi-gdb -x .tmp.gdb
rm -rf .tmp.gdb

kill $tmp
