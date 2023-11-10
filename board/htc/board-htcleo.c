/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2022, Ivaylo Ivanov <ivo.ivanov.ivanov1@gmail.com>
 */
#include <main.h>
#include "board-htcleo.h"

void board_init(void) {
    // Format (32bpp)
    writel(DMA_PACK_ALIGN_LSB|DMA_PACK_PATTERN_RGB|DMA_DITHER_EN|DMA_OUT_SEL_LCDC|DMA_IBUF_FORMAT_xRGB8888_OR_ARGB8888|DMA_DSTC0G_8BITS|DMA_DSTC1B_8BITS|DMA_DSTC2R_8BITS, MDP_DMA_P_CONFIG);
    // Stride
    writel(4 * 480, MDP_DMA_P_BUF_Y_STRIDE);
}
