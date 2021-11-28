/****************************************************************************
 * apps/boot/mcuboot/mcuboot_confirm_main.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <endian.h>

#include <errno.h>
#include <stdio.h>

#include <bootutil/bootutil_public.h>

/****************************************************************************
 * Private Functions
 ****************************************************************************/
#define BOOT_HEADER_MAGIC_V1 0x96f3b83d
#define BOOT_HEADER_SIZE_V1 32

struct mcuboot_img_sem_ver {
	uint8_t major;
	uint8_t minor;
	uint16_t revision;
	uint32_t build_num;
};

struct mcuboot_img_header_v1 {
	/** The size of the image, in bytes. */
	uint32_t image_size;
	/** The image version. */
	struct mcuboot_img_sem_ver sem_ver;
};

struct mcuboot_img_header {
	/**
	 * The version of MCUboot the header is built for.
	 *
	 * The value 1 corresponds to MCUboot versions 1.x.y.
	 */
	uint32_t mcuboot_version;
	/**
	 * The header information. It is only valid to access fields
	 * in the union member corresponding to the mcuboot_version
	 * field above.
	 */
	union {
		/** Header information for MCUboot version 1. */
		struct mcuboot_img_header_v1 v1;
	} h;
};

struct mcuboot_v1_raw_header {
	uint32_t header_magic;
	uint32_t image_load_address;
	uint16_t header_size;
	uint16_t pad;
	uint32_t image_size;
	uint32_t image_flags;
	struct {
		uint8_t major;
		uint8_t minor;
		uint16_t revision;
		uint32_t build_num;
	} version;
	uint32_t pad2;
} __packed;

static int boot_read_v1_header(uint8_t area_id,
			       struct mcuboot_v1_raw_header *v1_raw)
{
  const struct flash_area *fa;
  int rc;

  rc = flash_area_open(area_id, &fa);
  if (rc) {
    return rc;
  }

  /*
   * Read and sanity-check the raw header.
   */
  rc = flash_area_read(fa, 0, v1_raw, sizeof(*v1_raw));
  flash_area_close(fa);
  if (rc) {
    return rc;
  }

  v1_raw->header_magic = le32toh(v1_raw->header_magic);
  v1_raw->image_load_address = le32toh(v1_raw->image_load_address);
  v1_raw->header_size = le16toh(v1_raw->header_size);
  v1_raw->image_size = le32toh(v1_raw->image_size);
  v1_raw->image_flags = le32toh(v1_raw->image_flags);
  v1_raw->version.revision = le16toh(v1_raw->version.revision);
  v1_raw->version.build_num = le32toh(v1_raw->version.build_num);

  /*
   * Sanity checks.
   *
   * Larger values in header_size than BOOT_HEADER_SIZE_V1 are
   * possible, e.g. if Zephyr was linked with
   * CONFIG_ROM_START_OFFSET > BOOT_HEADER_SIZE_V1.
   */
  if ((v1_raw->header_magic != BOOT_HEADER_MAGIC_V1)
  ||  (v1_raw->header_size   < BOOT_HEADER_SIZE_V1)) {
    return -EIO;
  }

  return 0;
}

/****************************************************************************
 * mcuboot_confirm_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  int rc;
  struct mcuboot_v1_raw_header v1_raw;
  size_t v1_min_size = (sizeof(uint32_t) + sizeof(struct mcuboot_img_header_v1));

  rc = boot_read_v1_header(0, &v1_raw);
  if (rc) {
    printf("Only header version 1 can be read. Abort!\n");
    return rc;
  }

  printf("Image version %d.%d.%d.%ld\n", v1_raw.version.major,
                                         v1_raw.version.minor,
                                         v1_raw.version.revision,
                                         v1_raw.version.build_num);

  return 0;
}
