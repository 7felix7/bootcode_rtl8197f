#ifndef __MS_DRAM_PARA_COS_H
#define __MS_DRAM_PARA_COS_H

#ifdef DDR_CUSTOM_PHY
const struct dram_phy_ip_info phy_calibrating_custom={
.mck_default = 5,
.mdqs_default = -7,
.dqs_en_default = 0x0000000e,
.rx_fifo_default = 0x0000000a,
.odt_default = 0x000007fe,
.dqs_driving = 0x000000ff, //default value
.dck_driving = 0x000000ff, //default value
.cmd_driving = 0x00000000, //default value
.ba_addr_driving = 0x00000000, //default value
.dq_driving = 0x0000000f, //default value
.rx_delay_tap_0 = 0x0f0f0f0f,
.rx_delay_tap_1 = 0x0f0f0f0f,
.rx_delay_tap_2 = 0x0f0f0f0f,
.rx_delay_tap_3 = 0x0f0f0f0f,
.rx_delay_tap_n_0 = 0x0f0f0f0f,
.rx_delay_tap_n_1 = 0x0f0f0f0f,
.rx_delay_tap_n_2 = 0x0f0f0f0f,
.rx_delay_tap_n_3 = 0x0f0f0f0f,
.pre_shift_0 = 0xaaaaaaaa,
.pre_shift_1 = 0xaaaaaaaa,
.post_shift_0 = 0xaaaaaaaa,
.post_shift_1 = 0xaaaaaaaa,
.tx_delay_tap_0 = 0x33333333,
.tx_delay_tap_1 = 0x33333333,
.dqs_dm_delay_tap = 0x3434,
};
#endif

#endif //_MS_DRAM_PARA_H
 
