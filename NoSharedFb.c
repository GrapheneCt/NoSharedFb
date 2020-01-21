#include <taihen.h>
#include <vitasdk.h>
#include <psp2/kernel/modulemgr.h> 

void _start() __attribute__((weak, alias("module_start")));
uint32_t text_addr, text_size, data_addr, data_size;

int module_start(SceSize argc, const void *args) {

	tai_module_info_t info;
	info.size = sizeof(info);
	int ret = taiGetModuleInfo("SceShell", &info);
	if (ret < 0) {
		return SCE_KERNEL_START_FAILED;
	}

	// Modified from TheOfficialFloW/Adrenaline
	SceKernelModuleInfo mod_info;
	mod_info.size = sizeof(SceKernelModuleInfo);
	ret = sceKernelGetModuleInfo(info.modid, &mod_info);
	if (ret < 0) {
		return SCE_KERNEL_START_FAILED;
	}
	text_addr = (uint32_t)mod_info.segments[0].vaddr;
	text_size = (uint32_t)mod_info.segments[0].memsz;
	data_addr = (uint32_t)mod_info.segments[1].vaddr;
	data_size = (uint32_t)mod_info.segments[1].memsz;

	uint32_t offset;

	switch (info.module_nid) {
	case 0x0552F692: // retail 3.60 SceShell
		offset = 0x27DC0F;
		break;

	case 0x5549BF1F: // retail 3.65 SceShell
	case 0x34B4D82E: // retail 3.67 SceShell
	case 0x12DAC0F3: // retail 3.68 SceShell
		offset = 0x27DFD7;
		break;

	case 0x587F9CED: // PTEL 3.65 SceShell
		offset = 0x27637B;
		break;

	case 0xE6A02F2B: // PDEL 3.65 SceShell
		offset = 0x271B21;
		break;

	default:
		return SCE_KERNEL_START_FAILED;
	}

	//BNE -> BEQ
	taiInjectData(info.modid, 0x00, offset, "\xD0", 0x01);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	return SCE_KERNEL_STOP_SUCCESS;
}
