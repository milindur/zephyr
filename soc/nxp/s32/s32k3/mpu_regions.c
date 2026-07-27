/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/devicetree.h>
#include <zephyr/linker/devicetree_regions.h>
#include <zephyr/arch/arm/mpu/arm_mpu_mem_cfg.h>

#if !defined(CONFIG_XIP)
extern char _rom_attr[];
#endif

#define REGION_PERIPHERAL_BASE_ADDRESS 0x40000000
#define REGION_PERIPHERAL_SIZE         REGION_512M
#define REGION_PPB_BASE_ADDRESS        0xE0000000
#define REGION_PPB_SIZE                REGION_1M

static struct arm_mpu_region mpu_regions[] = {

	/* ERR011573: use first region to prevent speculative access in entire memory space */
	MPU_REGION_ENTRY("UNMAPPED", 0, {REGION_4G | MPU_RASR_XN_Msk | P_NA_U_NA_Msk}),

	/* Keep before CODE region so it can be overlapped by SRAM CODE in non-XIP systems */
	MPU_REGION_ENTRY("SRAM", DT_CHOSEN_SRAM_ADDR, REGION_RAM_ATTR(REGION_SRAM_SIZE)),

#ifdef CONFIG_XIP
#if CONFIG_FLASH_SIZE > (4 * 1024)
	/*
	 * Code flash larger than 4 MiB (S32K358 and friends: 8 MiB at
	 * 0x0040_0000) rounds up to an 8 MiB PMSAv7 region, but the S32K3
	 * code flash base is only 4 MiB aligned, which PMSAv7 does not
	 * allow. Cover the flash with two 4 MiB regions instead.
	 */
	MPU_REGION_ENTRY("FLASH_0", CONFIG_FLASH_BASE_ADDRESS, REGION_FLASH_ATTR(REGION_4M)),
	MPU_REGION_ENTRY("FLASH_1", CONFIG_FLASH_BASE_ADDRESS + 0x400000,
			 REGION_FLASH_ATTR(REGION_4M)),
#else
	MPU_REGION_ENTRY("FLASH", CONFIG_FLASH_BASE_ADDRESS, REGION_FLASH_ATTR(REGION_FLASH_SIZE)),
#endif
#else
	/* Run from SRAM */
	MPU_REGION_ENTRY("CODE", DT_CHOSEN_SRAM_ADDR, {(uint32_t)_rom_attr}),
#endif

	MPU_REGION_ENTRY("PERIPHERALS", REGION_PERIPHERAL_BASE_ADDRESS,
			 REGION_IO_ATTR(REGION_PERIPHERAL_SIZE)),

	MPU_REGION_ENTRY("PPB", REGION_PPB_BASE_ADDRESS, REGION_PPB_ATTR(REGION_PPB_SIZE)),
};

const struct arm_mpu_config mpu_config = {
	.num_regions = ARRAY_SIZE(mpu_regions),
	.mpu_regions = mpu_regions,
};
