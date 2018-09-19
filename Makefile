TARGET = hello64
BOOTLOADER = bootloader.raw
KERNEL = kernel.raw
BOOTLOADER_DIR = src/bootloader
KERNEL_DIR = src/kernel

all: $(TARGET)

$(TARGET): kernel bootloader
	cat $(BOOTLOADER_DIR)/$(BOOTLOADER) $(KERNEL_DIR)/$(KERNEL) > $@

kernel:
	@$(MAKE) -C $(KERNEL_DIR)

bootloader:
	@$(MAKE) -C $(BOOTLOADER_DIR)

.PHONY: clean

clean:
	@$(MAKE) -C $(BOOTLOADER_DIR) clean
	@$(MAKE) -C $(KERNEL_DIR) clean
	$(RM) $(TARGET) instr
