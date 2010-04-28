/*-
 * Copyright (c) 2003-2009 RMI Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of RMI Corporation, nor the names of its contributors,
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * RMI_BSD */
/* DO NOT EDIT THIS FILE
 * This file has been autogenerated by ./gen_struct_offsets
 */
#ifndef _SHARED_STRUCTS_OFFSETS_H
#define _SHARED_STRUCTS_OFFSETS_H

/* struct boot1_info offsets */
#define boot1_info_boot_level_off 0
#define boot1_info_io_base_off 8
#define boot1_info_output_device_off 16
#define boot1_info_uart_print_off 24
#define boot1_info_led_output_off 32
#define boot1_info_init_off 40
#define boot1_info_exit_off 48
#define boot1_info_warm_reset_off 56
#define boot1_info_wakeup_off 64
#define boot1_info_cpu_online_map_off 72
#define boot1_info_master_reentry_sp_off 80
#define boot1_info_master_reentry_gp_off 88
#define boot1_info_master_reentry_fn_off 96
#define boot1_info_slave_reentry_fn_off 104
#define boot1_info_magic_dword_off 112
#define boot1_info_uart_putchar_off 120
#define boot1_info_size_off 128
#define boot1_info_uart_getchar_off 136
#define boot1_info_nmi_handler_off 144
#define boot1_info_psb_version_off 152
#define boot1_info_mac_addr_off 160
#define boot1_info_cpu_frequency_off 168
#define boot1_info_board_version_off 176
#define boot1_info_malloc_off 184
#define boot1_info_free_off 192
#define boot1_info_alloc_pbuf_off 200
#define boot1_info_free_pbuf_off 208
#define boot1_info_psb_os_cpu_map_off 216
#define boot1_info_userapp_cpu_map_off 224
#define boot1_info_wakeup_os_off 232
#define boot1_info_psb_mem_map_off 240

/* struct boot1_info size */
#define boot1_info_size 248

/* boot1_info version */
#define boot1_info_version 1


#endif
