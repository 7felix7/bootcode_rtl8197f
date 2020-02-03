/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 8252 $
 * $Date: 2010-01-29 14:04:02 +0800 (?Ÿæ?äº? 29 ä¸€??2010) $
 *
 * Purpose : RTK switch high-level API for RTL8367/RTL8367B
 * Feature : Here is a list of all functions and variables in this module.
 * API source : Realtek_Unmanaged_Switch_API_V1.3.9_20160804
 *
 */
#include <linux/config.h>
#include <string.h>
#include <rtk_api.h>
#include <rtk_types.h>
#include <rtk_error.h>

#include <rtl8367c_asicdrv_port.h>
#include <rtl8367c_asicdrv_phy.h>
#include <rtl8367c_asicdrv_cputag.h>
#include <rtl8367c_asicdrv_rma.h>

//all necssary method but meme size big

#include <rtk_switch.h>
#include <led.h>
#include <port.h>
#include <cpu.h>
#include <rtl8367c_asicdrv.h>
#include <rtl8367c_asicdrv_led.h>
#include <rtl8367c_asicdrv_misc.h>
#include <rtl8367c_asicdrv_green.h>
#include <rtl8367c_asicdrv_lut.h>
#include <rtl8367c_asicdrv_rma.h>
#include <rtl8367c_asicdrv_mirror.h>
#include <rtl8367c_asicdrv_portIsolation.h>
#include <rtl8367c_asicdrv_meter.h>
#include <rtl8367c_asicdrv_scheduling.h>
#include <rtl8367c_asicdrv_inbwctrl.h>

#include <rtk_lan_wan_isolation.h>

//#include <asm/rtl8198.h>

#define REG1301_8367R_VB				0x1010
#define REG1301_8367RB					0x1000

#define DELAY_800MS_FOR_CHIP_STATABLE() {  }

rtk_uint16      (*init_para)[2];
rtk_uint16      init_size;
extern unsigned int smi_write(unsigned int mAddrs, unsigned int rData);
#define EEE_OCP_PHY_ADDR    (0xA5D0)

#ifdef CONFIG_SW_8366SC
#define EXT_PORT_FOR_HOST EXT_PORT1
#else //8367RB
#define EXT_PORT_FOR_HOST EXT_PORT0
#endif	

//NEW API do not have these API
ret_t rtl8367b_setAsicMaxLengthInRx(rtk_uint32 maxLength)
{	
	return rtl8367c_setAsicRegBits(RTL8367C_REG_MAX_LENGTH_LIMINT_IPG, RTL8367C_MAX_LENTH_CTRL_MASK, maxLength);
}

rtk_api_ret_t rtk_switch_maxPktLen_set(rtk_switch_maxPktLen_t len)
{
    rtk_api_ret_t retVal;

    if (len>=MAXPKTLEN_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367b_setAsicMaxLengthInRx(len)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}



#if 0 //NEW API

rtk_api_ret_t rtk_switch_probe(switch_chip_t *pSwitchChip)
{
#if defined(FORCE_PROBE_RTL8367C)

    *pSwitchChip = CHIP_RTL8367C;
    halCtrl = &rtl8367c_hal_Ctrl;

#elif defined(FORCE_PROBE_RTL8370B)

    *pSwitchChip = CHIP_RTL8370B;
    halCtrl = &rtl8370b_hal_Ctrl;

#else
    rtk_uint32 retVal;
    rtk_uint32 data;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &data)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (data)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            *pSwitchChip = CHIP_RTL8367C;
            halCtrl = &rtl8367c_hal_Ctrl;
            break;
        case 0x0652:
        case 0x6368:
            *pSwitchChip = CHIP_RTL8370B;
            halCtrl = &rtl8370b_hal_Ctrl;
            break;
        case 0x0801:
            *pSwitchChip = CHIP_RTL8364B;
            halCtrl = &rtl8364b_hal_Ctrl;
            break;
        default:
            return RT_ERR_FAILED;
    }
#endif

    return RT_ERR_OK;
}

#if defined(FORCE_PROBE_RTL8367C)
static rtk_switch_halCtrl_t *halCtrl = &rtl8367c_hal_Ctrl;
#elif defined(FORCE_PROBE_RTL8370B)
static rtk_switch_halCtrl_t *halCtrl = &rtl8370b_hal_Ctrl;
#else
static rtk_switch_halCtrl_t *halCtrl = NULL;
#endif


rtk_api_ret_t rtk_switch_initialState_set(init_state_t state)
{
    if(state >= INIT_STATE_END)
        return RT_ERR_FAILED;

    init_state = state;
    return RT_ERR_OK;
}

rtk_api_ret_t rtk_switch_init(void)
{
    rtk_uint32  retVal;
    rtl8367c_rma_t rmaCfg;
    switch_chip_t   switchChip;

    /* probe switch */
    if((retVal = rtk_switch_probe(&switchChip)) != RT_ERR_OK)
        return retVal;

    /* Set initial state */

    if((retVal = rtk_switch_initialState_set(INIT_COMPLETED)) != RT_ERR_OK)
        return retVal;

    /* Initial */
    switch(switchChip)
    {
        case CHIP_RTL8367C:
            if((retVal = _rtk_switch_init_8367c()) != RT_ERR_OK)
                return retVal;
            break;
        case CHIP_RTL8370B:
            if((retVal = _rtk_switch_init_8370b()) != RT_ERR_OK)
                return retVal;
            break;
        case CHIP_RTL8364B:
            if((retVal = _rtk_switch_init_8364b()) != RT_ERR_OK)
                return retVal;
            break;
        default:
            return RT_ERR_CHIP_NOT_FOUND;
    }

    /* Set Old max packet length to 16K */
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_MAX_LENGTH_LIMINT_IPG, RTL8367C_MAX_LENTH_CTRL_MASK, 3)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_MAX_LEN_RX_TX, RTL8367C_MAX_LEN_RX_TX_MASK, 3)) != RT_ERR_OK)
        return retVal;

    /* ACL Mode */
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_ACL_ACCESS_MODE, RTL8367C_ACL_ACCESS_MODE_MASK, 1)) != RT_ERR_OK)
        return retVal;

    /* Max rate */
    if((retVal = rtk_rate_igrBandwidthCtrlRate_set(halCtrl->hsg_logical_port, RTL8367C_QOS_RATE_INPUT_MAX_HSG, DISABLED, ENABLED)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtk_rate_egrBandwidthCtrlRate_set(halCtrl->hsg_logical_port, RTL8367C_QOS_RATE_INPUT_MAX_HSG, ENABLED)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x03fa, 0x0007)) != RT_ERR_OK)
        return retVal;

    /* Change unknown DA to per port setting */
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_PORT_SECURIT_CTRL_REG, RTL8367C_UNKNOWN_UNICAST_DA_BEHAVE_MASK, 3)) != RT_ERR_OK)
        return retVal;

    /* LUT lookup OP = 1 */
    if ((retVal = rtl8367c_setAsicLutIpLookupMethod(1))!=RT_ERR_OK)
        return retVal;

    /* Set RMA */
    rmaCfg.portiso_leaky = 0;
    rmaCfg.vlan_leaky = 0;
    rmaCfg.keep_format = 0;
    rmaCfg.trap_priority = 0;
    rmaCfg.discard_storm_filter = 0;
    rmaCfg.operation = 0;
    if ((retVal = rtl8367c_setAsicRma(2, &rmaCfg))!=RT_ERR_OK)
        return retVal;

    /* Enable TX Mirror isolation leaky */
    if ((retVal = rtl8367c_setAsicPortMirrorIsolationTxLeaky(ENABLED)) != RT_ERR_OK)
        return retVal;

    /* INT EN */
    if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_IO_MISC_FUNC, RTL8367C_INT_EN_OFFSET, 1)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

#endif //END NEW API

#if defined(CONFIG_SW_83XX)&&defined(CONFIG_LAN_WAN_ISOLATION)
#include <rtl8367c_asicdrv_portIsolation.h>
#include <rtl8196x/asicregs.h>
#include <asm/delay.h>
#if 0 
/* Function Name:
 *      rtl8367b_setAsicPortIsolationPermittedPortmask
 * Description:
 *      Set permitted port isolation portmask
 * Input:
 *      port 			- Physical port number (0~7)
 *      permitPortmask 	- port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_PORT_ID  	- Invalid port number
 *      RT_ERR_PORT_MASK  	- Invalid portmask
 * Note:
 *      None
 */

ret_t rtl8367b_setAsicPortIsolationPermittedPortmask(rtk_uint32 port, rtk_uint32 permitPortmask)
{
    if(port >= RTL8367B_PORTNO)
        return RT_ERR_PORT_ID;

    if( permitPortmask > RTL8367B_PORTMASK)
        return RT_ERR_PORT_MASK;
    
    return rtl8367c_setAsicReg(RTL8367B_PORT_ISOLATION_PORT_MASK_REG(port), permitPortmask);
}
/* Function Name:
 *      rtl8367b_getAsicPortIsolationPermittedPortmask
 * Description:
 *      Get permitted port isolation portmask
 * Input:
 *      port 				- Physical port number (0~7)
 *      pPermitPortmask 	- port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_PORT_ID  	- Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367b_getAsicPortIsolationPermittedPortmask(rtk_uint32 port, rtk_uint32 *pPermitPortmask)
{
    if(port >= RTL8367B_PORTNO)
        return RT_ERR_PORT_ID;
    
    return rtl8367c_getAsicReg(RTL8367B_PORT_ISOLATION_PORT_MASK_REG(port), pPermitPortmask);
}

/* Function Name:
 *      rtk_port_isolation_set
 * Description:
 *      Set permitted port isolation portmask
 * Input:
 *      port        - port id.
 *      portmask    - Permit port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number.
 *      RT_ERR_PORT_MASK    - Invalid portmask.
 * Note:
 *      This API set the port mask that a port can trasmit packet to of each port
 *      A port can only transmit packet to ports included in permitted portmask
 */
rtk_api_ret_t rtk_port_isolation_set(rtk_port_t port, rtk_portmask_t portmask)
{
    rtk_api_ret_t retVal;

    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;

    if ( portmask.bits[0] > RTK_MAX_PORT_MASK)
        return RT_ERR_PORT_MASK;

    if ((retVal = rtl8367c_setAsicPortIsolationPermittedPortmask(port, portmask.bits[0])) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      rtk_port_isolation_get
 * Description:
 *      Get permitted port isolation portmask
 * Input:
 *      port - Port id.
 * Output:
 *      pPortmask - Permit port mask
 * Return:
 *      RT_ERR_OK           - OK
 *      RT_ERR_FAILED       - Failed
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API get the port mask that a port can trasmit packet to of each port
 *      A port can only transmit packet to ports included in permitted portmask
 */
rtk_api_ret_t rtk_port_isolation_get(rtk_port_t port, rtk_portmask_t *pPortmask)
{
    rtk_api_ret_t retVal;

    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID;

    if ((retVal = rtl8367c_getAsicPortIsolationPermittedPortmask(port, &pPortmask->bits[0])) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}
#endif
#endif

int RTL83XX_init(void)
{
    rtk_portmask_t portmask;
    unsigned int ret, regData;
 
    /* Set external interface 0 to RGMII with Force mode, 1000M, Full-duple, enable TX&RX pause*/
    rtk_port_mac_ability_t mac_cfg;
    rtk_mode_ext_t mode ;
 
    /* Initial Chip */
    ret = rtk_switch_init();

    /* Enable LED Group 0&1 from P0 to P4 */
    portmask.bits[0]=0x1F;
    rtk_led_enable_set(LED_GROUP_0, &portmask);
    rtk_led_enable_set(LED_GROUP_1, &portmask);
 
    mode = MODE_EXT_RGMII ;
    mac_cfg.forcemode = MAC_FORCE;
    mac_cfg.speed = SPD_1000M;
    mac_cfg.duplex = FULL_DUPLEX;
    mac_cfg.link = PORT_LINKUP;
    mac_cfg.nway = DISABLED;
    mac_cfg.txpause = ENABLED;
    mac_cfg.rxpause = ENABLED;
    rtk_port_macForceLinkExt_set(EXT_PORT_FOR_HOST,mode,&mac_cfg);
 
    /* Set RGMII Interface 0 TX delay to 2ns and RX to step 4 */
    // set the tx/rx delay in 8197D site
    //rtk_port_rgmiiDelayExt_set(EXT_PORT_1, 1, 4);
    rtk_port_rgmiiDelayExt_set(EXT_PORT_FOR_HOST, 0, 2);
 
    /* set port 5 as CPU port */
    rtk_cpu_enable_set(ENABLE);
    rtk_cpu_tagPort_set(EXT_PORT_FOR_HOST, CPU_INSERT_TO_NONE);

    rtl8367c_setAsicReg(RTL8367C_REG_UNDA_FLOODING_PMSK, 0xff);
    rtl8367c_setAsicReg(RTL8367C_REG_UNMCAST_FLOADING_PMSK, 0xff);
    rtl8367c_setAsicReg(RTL8367C_REG_BCAST_FLOADING_PMSK, 0xff);

#if defined(CONFIG_SW_83XX)&&defined(CONFIG_LAN_WAN_ISOLATION)
    {
	    rtk_portmask_t pm;

	    pm.bits[0] = (0x3001f & ~(1<<4));
	    rtk_port_isolation_set(0, &pm);
        rtk_port_isolation_set(1, &pm);
	    rtk_port_isolation_set(2, &pm);
	    rtk_port_isolation_set(3, &pm);
	    pm.bits[0] = 0x30010;
	    rtk_port_isolation_set(4, &pm);
    }

#endif

	rtk_switch_maxPktLen_set(MAXPKTLEN_16000B);
	rtl8367c_setAsicReg(0x1B03, 0x0936);

    return ret; 
}
