#
# arch/moxie/boot/Makefile
#

targets := linux.bin linux.bin.gz

OBJCOPYFLAGS_linux.bin  := -O binary

$(obj)/linux.bin: vmlinux FORCE
	[ -n $(CONFIG_INITRAMFS_SOURCE) ] && [ ! -e $(CONFIG_INITRAMFS_SOURCE) ] && \
	touch $(CONFIG_INITRAMFS_SOURCE) || echo "No CPIO image"
	$(call if_changed,objcopy)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

$(obj)/linux.bin.gz: $(obj)/linux.bin FORCE
	$(call if_changed,gzip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

clean-kernel += linux.bin linux.bin.gz

