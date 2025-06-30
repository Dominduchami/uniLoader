ENTRY(_start);
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
TARGET(binary)
INPUT(blob/Image)
INPUT(blob/dtb)
INPUT(blob/ramdisk)
SECTIONS
{
        __start_address = (0x80008000 > 0x1) ? 0x80008000 : 0x80008000;
        . = __start_address;
        .boot : {
                arch/arm/start.o
        }
        .dtb ALIGN(0x1000) : {
                dtb = .;
                blob/dtb
        }
        .kernel ALIGN(0x1000) : {
                kernel = .;
                blob/Image
        }
       .ramdisk ALIGN(0x1000) : {
  ramdisk = .;
  blob/ramdisk
 }
 kernel_size = SIZEOF(.kernel);
 ramdisk_size = SIZEOF(.ramdisk);
}
