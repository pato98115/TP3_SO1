cmd_/home/jerea00/TP3_SO1/TP3_SO1/desencriptador/desencriptador.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds  --build-id  -o /home/jerea00/TP3_SO1/TP3_SO1/desencriptador/desencriptador.ko /home/jerea00/TP3_SO1/TP3_SO1/desencriptador/desencriptador.o /home/jerea00/TP3_SO1/TP3_SO1/desencriptador/desencriptador.mod.o ;  true