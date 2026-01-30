nasm -f bin boot.asm -o boot.bin
g++ -m32 -c kernel.cpp -o kernel.o -ffreestanding -O2 -fno-exceptions -fno-rtti
ld -m i386pe -o kernel.tmp -Ttext 0x1000 kernel.o
objcopy -O binary kernel.tmp kernel.bin

cat boot.bin kernel.bin > AlinD_OS.img
truncate -s 1440k AlinD_OS.img

echo "AlinD_OS.img basariyla olusturuldu!"