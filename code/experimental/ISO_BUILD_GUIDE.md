# AlinD OS - ISO Derleme Rehberi (VirtualBox İçin)

Bu rehber, oluşturduğumuz gerçek işletim sistemi çekirdeğini bir `.iso` dosyasına dönüştürmenizi sağlar.

## Gerekli Araçlar (MSYS2 Terminalinde)

Şu araçların kurulu olması gerekir:
```bash
pacman -S nasm xorriso grub mingw-w64-x86_64-gcc
```

## Derleme Adımları

Aşağıdaki komutları sırayla MSYS2 terminalinde çalıştırın:

1. **Assembly Dosyasını Derle:**
   ```bash
   nasm -f elf32 boot.asm -o boot.o
   ```

2. **C++ Kernel'ı Derle:**
   ```bash
   g++ -m32 -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
   ```

3. **Dosyaları Linkle (Bağla):**
   ```bash
   ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o
   ```

4. **ISO Yapısını Oluştur:**
   ```bash
   mkdir -p isodir/boot/grub
   cp kernel.bin isodir/boot/
   cp grub.cfg isodir/boot/grub/
   grub-mkrescue -o AlinD_OS.iso isodir
   ```

## VirtualBox'ta Çalıştırma

1. VirtualBox'ı açın ve "Yeni" butonuna tıklayın.
2. Tür olarak "Other" ve Sürüm olarak "Other/Unknown (32-bit)" seçin.
3. ISO dosyası olarak oluşturduğumuz `AlinD_OS.iso` dosyasını takın.
4. Başlat'a basın!

---
© 2026 AlinD OS Team
