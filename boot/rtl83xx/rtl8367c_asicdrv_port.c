/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 70627 $
 * $Date: 2016-08-09 16:49:57 +0800 (週二, 09 八月 2016) $
 *
 * Purpose : RTL8367C switch high-level API for RTL8367C
 * Feature : Port security related functions
 *
 */

#include <rtl8367c_asicdrv_port.h>

#include <string.h>

/* Function Name:
 *      rtl8367c_setAsicPortUnknownDaBehavior
 * Description:
 *      Set UNDA behavior
 * Input:
 *      port        - port ID
 *      behavior 	- 0: flooding to unknwon DA portmask; 1: drop; 2:trap; 3: flooding
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_NOT_ALLOWED  - Invalid behavior
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortUnknownDaBehavior(rtk_uint32 port, rtk_uint32 behavior)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(behavior >= L2_UNDA_BEHAVE_END)
		return RT_ERR_NOT_ALLOWED;

	if(port < 8)
		return rtl8367c_setAsicRegBits(RTL8367C_REG_UNKNOWN_UNICAST_DA_PORT_BEHAVE, RTL8367C_Port0_ACTION_MASK << (port * 2), behavior);
	else
		return rtl8367c_setAsicRegBits(RTL8367C_REG_UNKNOWN_UNICAST_DA_PORT_BEHAVE_EXT, RTL8367C_PORT8_ACTION_MASK << ((port-8) * 2), behavior);
}
/* Function Name:
 *      rtl8367c_getAsicPortUnknownDaBehavior
 * Description:
 *      Get UNDA behavior
 * Input:
 *      port        - port ID
 * Output:
 *      pBehavior 	- 0: flooding to unknwon DA portmask; 1: drop; 2:trap; 3: flooding
 * Return:
 *      RT_ERR_OK 	- Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortUnknownDaBehavior(rtk_uint32 port, rtk_uint32 *pBehavior)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

	if(port < 8)
		return rtl8367c_getAsicRegBits(RTL8367C_REG_UNKNOWN_UNICAST_DA_PORT_BEHAVE, RTL8367C_Port0_ACTION_MASK << (port * 2), pBehavior);
	else
		return rtl8367c_getAsicRegBits(RTL8367C_REG_UNKNOWN_UNICAST_DA_PORT_BEHAVE_EXT, RTL8367C_PORT8_ACTION_MASK << ((port-8) * 2), pBehavior);
}
/* Function Name:
 *      rtl8367c_setAsicPortUnknownSaBehavior
 * Description:
 *      Set UNSA behavior
 * Input:
 *      behavior 	- 0: flooding; 1: drop; 2:trap
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_NOT_ALLOWED  - Invalid behavior
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortUnknownSaBehavior(rtk_uint32 behavior)
{
    if(behavior >= L2_BEHAVE_SA_END)
		return RT_ERR_NOT_ALLOWED;

	return rtl8367c_setAsicRegBits(RTL8367C_PORT_SECURIT_CTRL_REG, RTL8367C_UNKNOWN_SA_BEHAVE_MASK, behavior);
}
/* Function Name:
 *      rtl8367c_getAsicPortUnknownSaBehavior
 * Description:
 *      Get UNSA behavior
 * Input:
 *      pBehavior 	- 0: flooding; 1: drop; 2:trap
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 	- Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortUnknownSaBehavior(rtk_uint32 *pBehavior)
{
	return rtl8367c_getAsicRegBits(RTL8367C_PORT_SECURIT_CTRL_REG, RTL8367C_UNKNOWN_SA_BEHAVE_MASK, pBehavior);
}
/* Function Name:
 *      rtl8367c_setAsicPortUnmatchedSaBehavior
 * Description:
 *      Set Unmatched SA behavior
 * Input:
 *      behavior 	- 0: flooding; 1: drop; 2:trap
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_NOT_ALLOWED  - Invalid behavior
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortUnmatchedSaBehavior(rtk_uint32 behavior)
{
    if(behavior >= L2_BEHAVE_SA_END)
		return RT_ERR_NOT_ALLOWED;

	return rtl8367c_setAsicRegBits(RTL8367C_PORT_SECURIT_CTRL_REG, RTL8367C_UNMATCHED_SA_BEHAVE_MASK, behavior);
}
/* Function Name:
 *      rtl8367c_getAsicPortUnmatchedSaBehavior
 * Description:
 *      Get Unmatched SA behavior
 * Input:
 *      pBehavior 	- 0: flooding; 1: drop; 2:trap
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 	- Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortUnmatchedSaBehavior(rtk_uint32 *pBehavior)
{
	return rtl8367c_getAsicRegBits(RTL8367C_PORT_SECURIT_CTRL_REG, RTL8367C_UNMATCHED_SA_BEHAVE_MASK, pBehavior);
}

/* Function Name:
 *      rtl8367c_setAsicPortUnmatchedSaMoving
 * Description:
 *      Set Unmatched SA moving state
 * Input:
 *      port        - Port ID
 *      enabled  	- 0: can't move to new port; 1: can move to new port
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_PORT_ID      - Error Port ID
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortUnmatchedSaMoving(rtk_uint32 port, rtk_uint32 enabled)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(RTL8367C_REG_L2_SA_MOVING_FORBID, port, (enabled == 1) ? 0 : 1);
}

/* Function Name:
 *      rtl8367c_getAsicPortUnmatchedSaMoving
 * Description:
 *      Get Unmatched SA moving state
 * Input:
 *      port        - Port ID
 * Output:
 *      pEnabled  	- 0: can't move to new port; 1: can move to new port
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_PORT_ID      - Error Port ID
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortUnmatchedSaMoving(rtk_uint32 port, rtk_uint32 *pEnabled)
{
    rtk_uint32 data;
    ret_t retVal;

    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_L2_SA_MOVING_FORBID, port, &data)) != RT_ERR_OK)
        return retVal;

    *pEnabled = (data == 1) ? 0 : 1;
    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicPortUnknownDaFloodingPortmask
 * Description:
 *      Set UNDA flooding portmask
 * Input:
 *      portmask 	- portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_PORT_MASK 	- Invalid portmask
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortUnknownDaFloodingPortmask(rtk_uint32 portmask)
{
	if(portmask > RTL8367C_PORTMASK)
		return RT_ERR_PORT_MASK;

	return rtl8367c_setAsicReg(RTL8367C_UNUCAST_FLOADING_PMSK_REG, portmask);
}
/* Function Name:
 *      rtl8367c_getAsicPortUnknownDaFloodingPortmask
 * Description:
 *      Get UNDA flooding portmask
 * Input:
 *      pPortmask 	- portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 	- Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortUnknownDaFloodingPortmask(rtk_uint32 *pPortmask)
{
	return rtl8367c_getAsicReg(RTL8367C_UNUCAST_FLOADING_PMSK_REG, pPortmask);
}
/* Function Name:
 *      rtl8367c_setAsicPortUnknownMulticastFloodingPortmask
 * Description:
 *      Set UNMC flooding portmask
 * Input:
 *      portmask 	- portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_PORT_MASK 	- Invalid portmask
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortUnknownMulticastFloodingPortmask(rtk_uint32 portmask)
{
	if(portmask > RTL8367C_PORTMASK)
		return RT_ERR_PORT_MASK;

	return rtl8367c_setAsicReg(RTL8367C_UNMCAST_FLOADING_PMSK_REG, portmask);
}
/* Function Name:
 *      rtl8367c_getAsicPortUnknownMulticastFloodingPortmask
 * Description:
 *      Get UNMC flooding portmask
 * Input:
 *      pPortmask 	- portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 	- Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortUnknownMulticastFloodingPortmask(rtk_uint32 *pPortmask)
{
	return rtl8367c_getAsicReg(RTL8367C_UNMCAST_FLOADING_PMSK_REG, pPortmask);
}
/* Function Name:
 *      rtl8367c_setAsicPortBcastFloodingPortmask
 * Description:
 *      Set Bcast flooding portmask
 * Input:
 *      portmask 	- portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_PORT_MASK 	- Invalid portmask
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortBcastFloodingPortmask(rtk_uint32 portmask)
{
	if(portmask > RTL8367C_PORTMASK)
		return RT_ERR_PORT_MASK;

	return rtl8367c_setAsicReg(RTL8367C_BCAST_FLOADING_PMSK_REG, portmask);
}
/* Function Name:
 *      rtl8367c_getAsicPortBcastFloodingPortmask
 * Description:
 *      Get Bcast flooding portmask
 * Input:
 *      pPortmask 	- portmask(0~0xFF)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 	- Success
 *      RT_ERR_SMI  - SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortBcastFloodingPortmask(rtk_uint32 *pPortmask)
{
	return rtl8367c_getAsicReg(RTL8367C_BCAST_FLOADING_PMSK_REG, pPortmask);
}
/* Function Name:
 *      rtl8367c_setAsicPortBlockSpa
 * Description:
 *      Set disabling blocking frame if source port and destination port are the same
 * Input:
 *      port 	- Physical port number (0~7)
 *      permit 	- 0: block; 1: permit
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortBlockSpa(rtk_uint32 port, rtk_uint32 permit)
{
	if(port >= RTL8367C_PORTNO)
		return RT_ERR_PORT_ID;

	return rtl8367c_setAsicRegBit(RTL8367C_SOURCE_PORT_BLOCK_REG, port, permit);
}
/* Function Name:
 *      rtl8367c_getAsicPortBlockSpa
 * Description:
 *      Get disabling blocking frame if source port and destination port are the same
 * Input:
 *      port 	- Physical port number (0~7)
 *      pPermit 	- 0: block; 1: permit
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortBlockSpa(rtk_uint32 port, rtk_uint32* pPermit)
{
	if(port >= RTL8367C_PORTNO)
		return RT_ERR_PORT_ID;

	return rtl8367c_getAsicRegBit(RTL8367C_SOURCE_PORT_BLOCK_REG, port, pPermit);
}
/* Function Name:
 *      rtl8367c_setAsicPortDos
 * Description:
 *      Set DOS function
 * Input:
 *      type 	- DOS type
 *      drop 	- 0: permit; 1: drop
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_OUT_OF_RANGE - Invalid payload index
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortDos(rtk_uint32 type, rtk_uint32 drop)
{
	if(type >= DOS_END)
		return RT_ERR_OUT_OF_RANGE;

	return rtl8367c_setAsicRegBit(RTL8367C_REG_DOS_CFG, RTL8367C_DROP_DAEQSA_OFFSET + type, drop);
}
/* Function Name:
 *      rtl8367c_getAsicPortDos
 * Description:
 *      Get DOS function
 * Input:
 *      type 	- DOS type
 *      pDrop 	- 0: permit; 1: drop
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_OUT_OF_RANGE - Invalid payload index
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortDos(rtk_uint32 type, rtk_uint32* pDrop)
{
	if(type >= DOS_END)
		return RT_ERR_OUT_OF_RANGE;

	return rtl8367c_getAsicRegBit(RTL8367C_REG_DOS_CFG, RTL8367C_DROP_DAEQSA_OFFSET + type,pDrop);
}
/* Function Name:
 *      rtl8367c_setAsicPortForceLink
 * Description:
 *      Set port force linking configuration
 * Input:
 *      port 		- Physical port number (0~7)
 *      pPortAbility - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortForceLink(rtk_uint32 port, rtl8367c_port_ability_t *pPortAbility)
{
    rtk_uint32 regData = 0;

    /* Invalid input parameter */
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    regData |= pPortAbility->forcemode << 12;
    regData |= pPortAbility->mstfault << 9;
    regData |= pPortAbility->mstmode << 8;
    regData |= pPortAbility->nway << 7;
    regData |= pPortAbility->txpause << 6;
    regData |= pPortAbility->rxpause << 5;
    regData |= pPortAbility->link << 4;
    regData |= pPortAbility->duplex << 2;
    regData |= pPortAbility->speed;

    return rtl8367c_setAsicReg(RTL8367C_REG_MAC0_FORCE_SELECT+port, regData);
}
/* Function Name:
 *      rtl8367c_getAsicPortForceLink
 * Description:
 *      Get port force linking configuration
 * Input:
 *      port 		- Physical port number (0~7)
 *      pPortAbility - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortForceLink(rtk_uint32 port, rtl8367c_port_ability_t *pPortAbility)
{
    ret_t retVal;
    rtk_uint32 regData;

    /* Invalid input parameter */
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    retVal = rtl8367c_getAsicReg(RTL8367C_REG_MAC0_FORCE_SELECT + port, &regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    pPortAbility->forcemode = (regData >> 12) & 0x0001;
    pPortAbility->mstfault  = (regData >> 9) & 0x0001;
    pPortAbility->mstmode   = (regData >> 8) & 0x0001;
    pPortAbility->nway      = (regData >> 7) & 0x0001;
    pPortAbility->txpause   = (regData >> 6) & 0x0001;
    pPortAbility->rxpause   = (regData >> 5) & 0x0001;
    pPortAbility->link      = (regData >> 4) & 0x0001;
    pPortAbility->duplex    = (regData >> 2) & 0x0001;
    pPortAbility->speed     = regData & 0x0003;

    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_getAsicPortStatus
 * Description:
 *      Get port link status
 * Input:
 *      port 		- Physical port number (0~7)
 *      pPortAbility - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortStatus(rtk_uint32 port, rtl8367c_port_status_t *pPortStatus)
{
    ret_t retVal;
    rtk_uint32 regData;

    /* Invalid input parameter */
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    retVal = rtl8367c_getAsicReg(RTL8367C_REG_PORT0_STATUS+port,&regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    pPortStatus->lpi1000  = (regData >> 11) & 0x0001;
    pPortStatus->lpi100   = (regData >> 10) & 0x0001;
    pPortStatus->mstfault = (regData >> 9) & 0x0001;
    pPortStatus->mstmode  = (regData >> 8) & 0x0001;
    pPortStatus->nway     = (regData >> 7) & 0x0001;
    pPortStatus->txpause  = (regData >> 6) & 0x0001;
    pPortStatus->rxpause  = (regData >> 5) & 0x0001;
    pPortStatus->link     = (regData >> 4) & 0x0001;
    pPortStatus->duplex   = (regData >> 2) & 0x0001;
    pPortStatus->speed    = regData  & 0x0003;

    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_setAsicPortForceLinkExt
 * Description:
 *      Set external interface force linking configuration
 * Input:
 *      id 			- external interface id (0~2)
 *      portAbility - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortForceLinkExt(rtk_uint32 id, rtl8367c_port_ability_t *pPortAbility)
{
    rtk_uint32 retVal, regValue, regValue2, type;
    rtk_uint32 reg_data = 0;

    /* Invalid input parameter */
    if(id >= RTL8367C_EXTNO)
        return RT_ERR_OUT_OF_RANGE;

    reg_data |= pPortAbility->forcemode << 12;
    reg_data |= pPortAbility->mstfault << 9;
    reg_data |= pPortAbility->mstmode << 8;
    reg_data |= pPortAbility->nway << 7;
    reg_data |= pPortAbility->txpause << 6;
    reg_data |= pPortAbility->rxpause << 5;
    reg_data |= pPortAbility->link << 4;
    reg_data |= pPortAbility->duplex << 2;
    reg_data |= pPortAbility->speed;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if (1 == type)
    {
        if(1 == id)
        {
            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;
        }

    	if(0 == id || 1 == id)
    	    return rtl8367c_setAsicReg(RTL8367C_REG_DIGITAL_INTERFACE0_FORCE + id, reg_data);
    	else
    	    return rtl8367c_setAsicReg(RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, reg_data);
    }
    else if (2 == type)
    {
        if (1 == id)
        {
             if((retVal = rtl8367c_setAsicRegBit(0x1311, 2, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(0x1311, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 4, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 6, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                return retVal;

            if (pPortAbility->link == 1)
            {
                if((retVal = rtl8367c_setAsicRegBit(0x1311, 4, 0)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(0x1311, 4, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if((retVal = rtl8367c_setAsicRegBits(0x1311, 0x3, 2)) != RT_ERR_OK)
                    return retVal;
            }


            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;
        }
        else if (2 == id)
        {
            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 2, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(0x13c4, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 4, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 6, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                return retVal;

            if (pPortAbility->link == 1)
            {
                if((retVal = rtl8367c_setAsicRegBit(0x13c4, 4, 0)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(0x13c4, 4, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if((retVal = rtl8367c_setAsicRegBits(0x13c4, 0x3, 2)) != RT_ERR_OK)
                    return retVal;
            }

            if((retVal = rtl8367c_setAsicRegBit(0x1dc1, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(0x1dc1, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1dc1, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1dc1, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1dc1, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;
        }

    }
    else if(3 == type)
    {
        if(1 == id)
        {
            /*for 1000x/100fx/1000x_100fx, param has to bet set to serdes registers*/
            if((retVal = rtl8367c_getAsicReg(0x1d41, &regValue)) != RT_ERR_OK)
                return retVal;
            if((regValue & 0xa0) == 0xa0)
            {
                if((retVal = rtl8367c_getAsicRegBits(0x1d95, 0x1f00, &regValue2)) != RT_ERR_OK)
                    return retVal;

                if(regValue2 == 0x4)
                {
                    /*1000X*/
                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
                    
                    if((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0x9000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFDFFF;
                    reg_data |= 0x40;
                    if(pPortAbility->forcemode)
                        reg_data &= 0xffffefff;
                    else
                        reg_data |= 0x1000;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data &= (~0x80);
                    
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &= (~0x100);
                    
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                        return retVal;

                }
                else if(regValue2 == 0x5)
                {
                    /*100FX*/
                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
                    
                    if((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0xB000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFFFBF;
                    reg_data |= 0x2000;
                    reg_data &= 0xffffefff;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data &= (~0x80);
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &= (~0x100);
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                        return retVal;

                }
                else if(regValue2 == 0x7)
                {
                    /*100FX*/
                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
                    
                    if((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0x9000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFDFFF;
                    reg_data |= 0x40;
                    if(pPortAbility->forcemode)
                        reg_data &= 0xffffefff;
                    else
                        reg_data |= 0x1000;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data &= (~0x80);
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &=(~0x100);
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0xB000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFFFBF;
                    reg_data |= 0x2000;
                    reg_data &= 0xffffefff;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data &= 0xffffff7f;
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &= 0xfffffeff;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                        return retVal;

                }

            }
            
        
            if((retVal = rtl8367c_setAsicRegBit(0x1311, 2, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(0x1311, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 4, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 6, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x1311, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                return retVal;

            if (pPortAbility->link == 1)
            {
                if((retVal = rtl8367c_setAsicRegBit(0x1311, 4, 0)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(0x1311, 4, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if((retVal = rtl8367c_setAsicRegBits(0x1311, 0x3, 2)) != RT_ERR_OK)
                    return retVal;
            }

        }
        else if (2 == id)
        {
            /*for 1000x/100fx/1000x_100fx, param has to bet set to serdes registers*/
            if((retVal = rtl8367c_getAsicReg(0x1d95, &regValue)) != RT_ERR_OK)
                return retVal;
            if((regValue & 0x3) == 0x3)
            {
                if((retVal = rtl8367c_getAsicRegBits(0x1d95, 0x1f00, &regValue2)) != RT_ERR_OK)
                    return retVal;

                if(regValue2 == 0x4)
                {
                    /*1000X*/
                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
                    
                    if((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0x9000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFDFFF;
                    reg_data |= 0x40;
                    if(pPortAbility->forcemode)
                        reg_data &= 0xffffefff;
                    else
                        reg_data |= 0x1000;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data &= 0xffffff7f;
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &= 0xfffffeff;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                        return retVal;

                }
                else if(regValue2 == 0x5)
                {
                    /*100FX*/
                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
                    
                    if((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0xB000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFFFBF;
                    reg_data |= 0x2000;
                    reg_data &= 0xffffefff;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data &= 0xffffff7f;
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &= 0xfffffeff;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                        return retVal;

                }
                else if(regValue2 == 0x7)
                {
                    /*100FX*/
                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                        return retVal;
                    
                    if((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 4*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0x9000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 1,  bit13 set to 0, bit12 nway_en*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFDFFF;
                    reg_data |= 0x40;
                    if(pPortAbility->forcemode)
                        reg_data &= 0xffffefff;
                    else
                        reg_data |= 0x1000;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data &= 0xffffff7f;
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &= 0xfffffeff;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 0  bit 12  set 1,  bit15~13 = 5*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFF0FFF;
                    reg_data |= 0xB000;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 0 2  bit 6  set 0,  bit13 set to 1, bit12 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFFFBF;
                    reg_data |= 0x2000;
                    reg_data &= 0xffffefff;
                       
                    if((retVal = rtl8367c_setAsicSdsReg(0,0,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                    /* 0 4 2  bit 8  rx pause,  bit7 tx pause*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 2, &reg_data)) != RT_ERR_OK)
                        return retVal;
                    if (pPortAbility->txpause)
                        reg_data |= 0x80;
                    else 
                        reg_data  &= 0xffffff7f;
                    if (pPortAbility->rxpause)
                        reg_data |= 0x100;
                    else 
                        reg_data &= 0xfffffeff;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,2, reg_data)) != RT_ERR_OK)
                        return retVal;

                     /* 0 4 0  bit 12  set 0*/
                    if((retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &reg_data)) != RT_ERR_OK)
                        return retVal;   
                    reg_data &= 0xFFFFEFFF;
                    if((retVal = rtl8367c_setAsicSdsReg(0,4,0, reg_data)) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                        return retVal;

                }

            }
        
            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 2, pPortAbility->duplex)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBits(0x13c4, 0x3, pPortAbility->speed)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 4, pPortAbility->link)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 6, pPortAbility->txpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 5, pPortAbility->rxpause)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicRegBit(0x13c4, 12, pPortAbility->forcemode)) != RT_ERR_OK)
                return retVal;

            if (pPortAbility->link == 1)
            {
                if((retVal = rtl8367c_setAsicRegBit(0x13c4, 4, 0)) != RT_ERR_OK)
                    return retVal;

                if((retVal = rtl8367c_setAsicRegBit(0x13c4, 4, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if((retVal = rtl8367c_setAsicRegBits(0x13c4, 0x3, 2)) != RT_ERR_OK)
                    return retVal;
            }
        }

        if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, pPortAbility->duplex)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, pPortAbility->speed)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, pPortAbility->link)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, pPortAbility->txpause)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, pPortAbility->rxpause)) != RT_ERR_OK)
            return retVal;

    }

    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_getAsicPortForceLinkExt
 * Description:
 *      Get external interface force linking configuration
 * Input:
 *      id 			- external interface id (0~1)
 *      pPortAbility - port ability configuration
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortForceLinkExt(rtk_uint32 id, rtl8367c_port_ability_t *pPortAbility)
{
    rtk_uint32  reg_data, regValue, type;
    rtk_uint32  sgmiiSel;
    rtk_uint32  hsgmiiSel;
    ret_t       retVal;

    /* Invalid input parameter */
    if(id >= RTL8367C_EXTNO)
        return RT_ERR_OUT_OF_RANGE;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if (1 == type)
    {
        if(1 == id)
        {
            if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, &sgmiiSel)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, &hsgmiiSel)) != RT_ERR_OK)
                return retVal;

            if( (sgmiiSel == 1) || (hsgmiiSel == 1) )
            {
                memset(pPortAbility, 0x00, sizeof(rtl8367c_port_ability_t));
                pPortAbility->forcemode = 1;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->duplex = reg_data;

                if((retVal = rtl8367c_getAsicRegBits(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->speed = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->link = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->txpause = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->rxpause = reg_data;

                return RT_ERR_OK;
            }
        }

    	if(0 == id || 1 == id)
        	retVal = rtl8367c_getAsicReg(RTL8367C_REG_DIGITAL_INTERFACE0_FORCE+id, &reg_data);
    	else
    	    retVal = rtl8367c_getAsicReg(RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, &reg_data);

        if(retVal != RT_ERR_OK)
            return retVal;

        pPortAbility->forcemode = (reg_data >> 12) & 0x0001;
        pPortAbility->mstfault  = (reg_data >> 9) & 0x0001;
        pPortAbility->mstmode   = (reg_data >> 8) & 0x0001;
        pPortAbility->nway      = (reg_data >> 7) & 0x0001;
        pPortAbility->txpause   = (reg_data >> 6) & 0x0001;
        pPortAbility->rxpause   = (reg_data >> 5) & 0x0001;
        pPortAbility->link      = (reg_data >> 4) & 0x0001;
        pPortAbility->duplex    = (reg_data >> 2) & 0x0001;
        pPortAbility->speed     = reg_data & 0x0003;
    }
    else if (2 == type)
    {
        if (id == 1)
        {
            if ((retVal = rtl8367c_getAsicReg(0x1311, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
        else if (2 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(0x13c4, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
    }
    else if (3 == type)
    {
        if (id == 1)
        {
            if ((retVal = rtl8367c_getAsicReg(0x1311, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
        else if (2 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(0x13c4, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
    }
    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_setAsicPortExtMode
 * Description:
 *      Set external interface mode configuration
 * Input:
 *      id 		- external interface id (0~2)
 *      mode 	- external interface mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortExtMode(rtk_uint32 id, rtk_uint32 mode)
{
    ret_t   retVal;
    rtk_uint32 i, regValue, type, option;
    rtk_uint32 redData[][2] =   { {0x04D7, 0x0480}, {0xF994, 0x0481}, {0x21A2, 0x0482}, {0x6960, 0x0483}, {0x9728, 0x0484}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x83F2, 0x002E} };
    rtk_uint32 redDataSB[][2] = { {0x04D7, 0x0480}, {0xF994, 0x0481}, {0x31A2, 0x0482}, {0x6960, 0x0483}, {0x9728, 0x0484}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x83F2, 0x002E} };
    rtk_uint32 redData1[][2] =  { {0x82F1, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    rtk_uint32 redData5[][2] =  { {0x82F1, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    rtk_uint32 redData6[][2] =  { {0x82F1, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    rtk_uint32 redData8[][2] =  { {0x82F1, 0x0500}, {0xF995, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    rtk_uint32 redData9[][2] =  { {0x82F1, 0x0500}, {0xF995, 0x0501}, {0x31A2, 0x0502}, {0x796C, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };
    rtk_uint32 redDataHB[][2] = { {0x82F0, 0x0500}, {0xF195, 0x0501}, {0x31A2, 0x0502}, {0x7960, 0x0503}, {0x9728, 0x0504}, {0x9D85, 0x0423}, {0xD810, 0x0424}, {0x0F80, 0x0001}, {0x83F2, 0x002E} };

    if(id >= RTL8367C_EXTNO)
        return RT_ERR_OUT_OF_RANGE;

    if(mode >= EXT_END)
        return RT_ERR_OUT_OF_RANGE;


    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }


    if (1==type)
    {
        if( (mode == EXT_SGMII) || (mode == EXT_HSGMII) )
        {
            if(id != 1)
                return RT_ERR_PORT_ID;

            if((retVal = rtl8367c_setAsicReg(0x13C0, 0x0249)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicReg(0x13C1, &option)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(0x13C0, 0x0000)) != RT_ERR_OK)
                return retVal;
        }

        if(mode == EXT_GMII)
        {
            if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_EXT0_RGMXF, RTL8367C_EXT0_RGTX_INV_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_EXT1_RGMXF, RTL8367C_EXT1_RGTX_INV_OFFSET, 1)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_EXT_TXC_DLY, RTL8367C_EXT1_GMII_TX_DELAY_MASK, 5)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_EXT_TXC_DLY, RTL8367C_EXT0_GMII_TX_DELAY_MASK, 6)) != RT_ERR_OK)
                return retVal;
        }

        /* Serdes reset */
        if( (mode == EXT_TMII_MAC) || (mode == EXT_TMII_PHY) )
        {
            if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_BYPASS_LINE_RATE, id, 1)) != RT_ERR_OK)
                return retVal;
        }
        else
        {
            if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_BYPASS_LINE_RATE, id, 0)) != RT_ERR_OK)
                return retVal;
        }

        if(mode == EXT_SGMII)
        {
            if(option == 0)
            {
                for(i = 0; i <= 7; i++)
                {
                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redData[i][0])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redData[i][1])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;
                }
            }
            else
            {
                for(i = 0; i <= 7; i++)
                {
                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redDataSB[i][0])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redDataSB[i][1])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;
                }
            }
        }

        if(mode == EXT_HSGMII)
        {
            if(option == 0)
            {
                if( (retVal = rtl8367c_setAsicReg(0x13c2, 0x0249)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicReg(0x1301, &regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(0x13c2, 0x0000)) != RT_ERR_OK)
                    return retVal;

                if ( ((regValue & 0x00F0) >> 4) == 0x0001)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redData1[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redData1[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0005)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redData5[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redData5[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0006)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redData6[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redData6[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0008)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redData8[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redData8[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
                else if ( ((regValue & 0x00F0) >> 4) == 0x0009)
                {
                    for(i = 0; i <= 8; i++)
                    {
                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redData9[i][0])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redData9[i][1])) != RT_ERR_OK)
                            return retVal;

                        if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                            return retVal;
                    }
                }
            }
            else
            {
                for(i = 0; i <= 8; i++)
                {
                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, redDataHB[i][0])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, redDataHB[i][1])) != RT_ERR_OK)
                        return retVal;

                    if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;
                }
            }
        }

        /* Only one ext port should care SGMII setting */
        if(id == 1)
        {

            if(mode == EXT_SGMII)
            {
                if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 1)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode == EXT_HSGMII)
            {
                if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != RT_ERR_OK)
                    return retVal;
            }
        }

        if(0 == id || 1 == id)
        {
            if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT, RTL8367C_SELECT_GMII_0_MASK << (id * RTL8367C_SELECT_GMII_1_OFFSET), mode)) != RT_ERR_OK)
                return retVal;
        }
        else
        {
            if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1, RTL8367C_SELECT_GMII_2_MASK, mode)) != RT_ERR_OK)
                return retVal;
        }

        /* Serdes not reset */
        if( (mode == EXT_SGMII) || (mode == EXT_HSGMII) )
        {
            if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x7106)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, 0x0003)) != RT_ERR_OK)
                return retVal;

            if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                return retVal;
        }
    }
    else if (2 == type)
    {
        /* Serdes reset */
        if( (mode == EXT_TMII_MAC) || (mode == EXT_TMII_PHY) )
        {
            if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_BYPASS_LINE_RATE, id+2, 1)) != RT_ERR_OK)
                return retVal;
        }
        else
        {
            if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_BYPASS_LINE_RATE, id+2, 0)) != RT_ERR_OK)
                return retVal;
        }

        /*set MAC mode*/
        if (id == 1)
        {
            if(mode == EXT_HSGMII)
                return RT_ERR_PORT_ID;

            if (mode == EXT_SGMII)
            {
                /*cfg port8 with MII mac mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;

                /*enable port8 SGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 14, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X || mode == EXT_100FX || mode == EXT_1000X_100FX)
            {
                /*cfg port8 with MII mac mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable port8 SGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 14, 0)) != RT_ERR_OK)
                    return retVal;

                /*set fiber link up*/
                if((retVal = rtl8367c_setAsicRegBit(0x6210, 11, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                /*cfg port8 with MII mac mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, mode)) != RT_ERR_OK)
                    return retVal;

                /*disable port8 SGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 14, 0)) != RT_ERR_OK)
                    return retVal;
            }
            /*disable SDS 1*/
            if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 0x1f)) != RT_ERR_OK)
                return retVal;
        }
        else if(id == 2)
        {
            if (mode == EXT_HSGMII)
            {
                if ((retVal = rtl8367c_setAsicReg(0x130, 7)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(0x39f, 7)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(0x3fa, 7)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                if ((retVal = rtl8367c_setAsicReg(0x130, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(0x39f, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicReg(0x3fa, 4)) != RT_ERR_OK)
                    return retVal;

            }


            if (mode == EXT_SGMII)
            {
                /*cfg port9 with MII mac mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;

                /*enable port9 SGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 6, 1)) != RT_ERR_OK)
                    return retVal;

                /*disable port9 HSGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 7, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_HSGMII)
            {
                /*cfg port9 with MII mac mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable port9 SGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 6, 0)) != RT_ERR_OK)
                    return retVal;

                /*enable port9 HSGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 7, 1)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X || mode == EXT_100FX || mode == EXT_1000X_100FX)
            {
                /*cfg port9 with MII mac mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable port9 SGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 6, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable port9 HSGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 7, 0)) != RT_ERR_OK)
                    return retVal;

                /*set fiber link up*/
                if((retVal = rtl8367c_setAsicRegBit(0x6200, 11, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else
            {
                /*cfg port9 with MII mac mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, mode)) != RT_ERR_OK)
                    return retVal;

                /*disable port9 SGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 6, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable port9 HSGMII*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d92, 7, 0)) != RT_ERR_OK)
                    return retVal;
            }
            /*disable SDS 0*/
            if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 0x1f)) != RT_ERR_OK)
                return retVal;
        }

        /*SET TO RGMII MODE*/
        if (mode == EXT_RGMII)
        {
            /*disable paral led pad*/
            if ((retVal = rtl8367c_setAsicReg(RTL8367C_REG_PARA_LED_IO_EN3, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicReg(RTL8367C_REG_PARA_LED_IO_EN1, 0)) != RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_setAsicReg(RTL8367C_REG_PARA_LED_IO_EN2, 0)) != RT_ERR_OK)
                return retVal;

            /*set MAC8 mode*/
            if (id == 1)
            {
                /*1: RGMII1 bias work at 3.3V, 0: RGMII1 bias work at 2.5V*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1303, 9, 1)) != RT_ERR_OK)
                    return retVal;

                /*drving 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1303, 6, 1)) != RT_ERR_OK)
                    return retVal;

                /*drving 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1303, 4, 1)) != RT_ERR_OK)
                    return retVal;

                /*show rate = 0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1303, 1, 0)) != RT_ERR_OK)
                    return retVal;

                /*EXT1 RGMII TXC delay 2ns*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1307, 3, 1)) != RT_ERR_OK)
                    return retVal;

                /*cfg_Ext1_rgtxc_dly = 0*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13f9, 0x38, 0)) != RT_ERR_OK)
                    return retVal;

                /*RXDLY = 0*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1307, 0x7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_rg1_dn = 4*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1304, 0x7000, 4)) != RT_ERR_OK)
                    return retVal;

                /*cfg_rg1_dp = 4*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13f9, 0x700, 4)) != RT_ERR_OK)
                    return retVal;
            }
            else if (id == 2)
            {
                /*1: RGMII1 bias work at 3.3V, 0: RGMII1 bias work at 2.5V*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1303, 10, 1)) != RT_ERR_OK)
                    return retVal;

                /*drving 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x13e2, 2, 1)) != RT_ERR_OK)
                    return retVal;

                /*drving 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x13e2, 1, 1)) != RT_ERR_OK)
                    return retVal;

                /*show rate = 0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x13e2, 0, 0)) != RT_ERR_OK)
                    return retVal;

                /*EXT1 RGMII TXC delay 2ns*/
                if ((retVal = rtl8367c_setAsicRegBit(0x13c5, 3, 1)) != RT_ERR_OK)
                    return retVal;

                /*cfg_Ext1_rgtxc_dly = 0*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13f9, 0x1c0, 0)) != RT_ERR_OK)
                    return retVal;

                /*RXDLY = 0*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c5, 0x7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_rg1_dn = 4*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13e2, 0x1c0, 4)) != RT_ERR_OK)
                    return retVal;

                /*cfg_rg1_dp = 4*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13e2, 0x38, 4)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else if (mode == EXT_SGMII)
        {
            if (id == 1)
            {
                /*sds 1     reg 1    page 0x21     write value  0xec91*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0xec91)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5) | 1)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C1)) != RT_ERR_OK)
                    return retVal;

                /*sds 1     reg 5    page 0x24     write value  0x5825*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x5825)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x24<<5) | 5)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 2)) != RT_ERR_OK)
                    return retVal;

                /*?????????????????*/

            }
            else if (id == 2)
            {
                /*sds 0     reg 0    page 0x28     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x28<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 0    page 0x24     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x24<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 5    page 0x21     write value  0x8dc3*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x8dc3)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5) | 5)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 2)) != RT_ERR_OK)
                    return retVal;

                /*?????????????????*/
            }
        }
        else if (mode == EXT_HSGMII)
        {
            if (id == 2)
            {
                /*sds 0     reg 0    page 0x28     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x28<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 0    page 0x24     write value  0x942c*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x942c)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x24<<5) | 0)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;

                /*sds 0     reg 5    page 0x21     write value  0x8dc3*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x8dc3)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5) | 5)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                    return retVal;


                /* optimizing HISGMII performance while RGMII used & */
                /*sds 0     reg 9     page 0x21     write value 0x3931*/
                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_DATA, 0x3931)) != RT_ERR_OK)
                        return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_ADR, (0x21<<5)|9) ) != RT_ERR_OK)
                        return retVal;

                if( (retVal = rtl8367c_setAsicReg(RTL8367C_REG_SDS_INDACS_CMD, 0x00C0)) != RT_ERR_OK)
                        return retVal;


                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 0x12)) != RT_ERR_OK)
                    return retVal;

                /*?????????????????*/
            }
        }
        else if (mode == EXT_1000X)
        {
            if (id == 1)
            {

                if( (retVal = rtl8367c_setAsicSdsReg(1, 1, 0x21, 0xec91)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 5, 0x24, 0x5825)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 4)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds1 1000M*/
                if( (retVal = rtl8367c_getAsicSdsReg(1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0x9000;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(1, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFdFFF;
                regValue |= 0x40;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;


                if( (retVal = rtl8367c_getAsicSdsReg(1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 4)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x6000, 0)) != RT_ERR_OK)
                    return retVal;

            }
            else if (id == 2)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 0x28, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 0x24, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 5, 0x21, 0x8dc3)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 4)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds0 1000M*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 0x1f)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0x9000;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFDFFF;
                regValue |= 0x40;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 4)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0xe0, 0)) != RT_ERR_OK)
                    return retVal;

            }
        }
        else if (mode == EXT_100FX)
        {
            if (id == 1)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(1, 1, 0x21, 0xec91)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 5, 0x24, 0x5825)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 5)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds1 100M*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_getAsicSdsReg(1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0xb000;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(1, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFFFBF;
                regValue |= 0x2000;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;
#if 0
                if( (retVal = rtl8367c_setAsicReg(0x6214, 0x1a0)) != RT_ERR_OK)
                    return retVal;
#endif
                if( (retVal = rtl8367c_getAsicSdsReg(1, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 5)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x6000, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if (id == 2)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 0x28, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 0x24, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 5, 0x21, 0x8dc3)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 5)) != RT_ERR_OK)
                    return retVal;

                /*patch speed change sds0 100M*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 0x1f)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFF0FFF;
                regValue |= 0xb000;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(0, 0, 2, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFFFBF;
                regValue |= 0x2000;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 2, regValue)) != RT_ERR_OK)
                    return retVal;

                if( (retVal = rtl8367c_getAsicSdsReg(0, 4, 0, &regValue)) != RT_ERR_OK)
                    return retVal;
                regValue &= 0xFFFFEFFF;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 4, 0, regValue)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 5)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0xe0, 0)) != RT_ERR_OK)
                    return retVal;
            }
        }
        else if (mode == EXT_1000X_100FX)
        {
            if (id == 1)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(1, 1, 0x21, 0xec91)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 5, 0x24, 0x5825)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 13, 0, 0x4616)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(1, 1, 0, 0xf20)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f00, 7)) != RT_ERR_OK)
                    return retVal;
            }
            else if (id == 2)
            {
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 0x28, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 0, 0x24, 0x942c)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 5, 0x21, 0x8dc3)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 13, 0, 0x4616)) != RT_ERR_OK)
                    return retVal;
                if( (retVal = rtl8367c_setAsicSdsReg(0, 1, 0, 0xf20)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d92, 0x1f, 7)) != RT_ERR_OK)
                    return retVal;
            }
        }

    }
    else if (3 == type)
    {
        /*set for mac 6*/
        if (1 == id)
        {
            if (mode == EXT_SGMII)
            {
                /*disable mac6 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;
                /*bit5: cfg_mac6_fib=0 & bit7: cfg_mac6_fib2=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_fib=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 1, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_sel_sgmii= 0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 0, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_hsgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_sgmii= 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 2, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_HSGMII)
            {
                /*disable mac6 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;
                /*bit5: cfg_mac6_fib=0 & bit7: cfg_mac6_fib2=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_fib=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 1, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_sel_sgmii= 0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 0, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_hsgmii=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 11, 1)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_sgmii= 0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 6, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 2, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode == EXT_1000X)
            {
                /*disable mac6 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_sds_mode_sel_new=1 & cfg_mac7_fib=0 & cfg_mac7_sel_sgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_force_autodet=1 &  cfg_dis_det=1*/
                if ((retVal = rtl8367c_setAsicReg(0x13eb, 0x15bb)) != RT_ERR_OK)
                    return retVal;
                /*cfg_sds_frc_mode=1 &  cfg_sds_mode=3b'100 (force sds FIB1000 mode) 4*/
                if ((retVal = rtl8367c_setAsicReg(0x13e7, 0xc)) != RT_ERR_OK)
                    return retVal;
                
                
                /*bit5: cfg_mac6_fib=1 & bit7: cfg_mac6_fib2=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 1)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_hsgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_sgmii= 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode == EXT_100FX)
            {
                /*disable mac6 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_sds_mode_sel_new=0 & cfg_mac7_fib=0 & cfg_mac7_sel_sgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_force_autodet=1 &  cfg_dis_det=1*/
                if ((retVal = rtl8367c_setAsicReg(0x13eb, 0x15bb)) != RT_ERR_OK)
                    return retVal;
                /*cfg_sds_frc_mode=1 &  cfg_sds_mode=3b'101 (force sds FIB100 mode)*/
                if ((retVal = rtl8367c_setAsicReg(0x13e7, 0xc)) != RT_ERR_OK)
                    return retVal;
                
                
                /*bit5: cfg_mac6_fib=1 & bit7: cfg_mac6_fib2=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 1)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_hsgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_sgmii= 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode == EXT_1000X_100FX)
            {
                /*disable mac6 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_sds_mode_sel_new=0 & cfg_mac7_fib=0 & cfg_mac7_sel_sgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_force_autodet=1 &  cfg_dis_det=1*/
                if ((retVal = rtl8367c_setAsicReg(0x13eb, 0x15bb)) != RT_ERR_OK)
                    return retVal;
                /*cfg_sds_frc_mode=1 &  cfg_sds_mode=3b'101 (force sds FIB100 mode)*/
                if ((retVal = rtl8367c_setAsicReg(0x13e7, 0xc)) != RT_ERR_OK)
                    return retVal;
                
                
                /*bit5: cfg_mac6_fib=1 & bit7: cfg_mac6_fib2=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 1)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 1)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_hsgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_sgmii= 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 6, 1)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                    return retVal;
            }
            else if(mode < EXT_SGMII)
            {
                /*disable mac6 sel SerDes mode*/
                if ((retVal = rtl8367c_setAsicReg(0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;

                /*bit5: cfg_mac6_fib=0 & bit7: cfg_mac6_fib2=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                if (mode < EXT_GMII)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, mode)) != RT_ERR_OK)
                        return retVal;
                }
                else if(mode == EXT_RMII_MAC)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 6)) != RT_ERR_OK)
                        return retVal;
                }
                else if(mode == EXT_RMII_PHY)
                {
                    if ((retVal = rtl8367c_setAsicRegBits(0x1305, 0xf0, 7)) != RT_ERR_OK)
                        return retVal;
                }
            }          
            
        }
        else if (2 == id)
        {
            if (mode == EXT_SGMII)
            {
                /*disable mac7 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;
                /*bit5: cfg_mac6_fib=0 & bit7: cfg_mac6_fib2=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_fib=0*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_sds_mode_sel_new=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x1F (reset mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_hsgmii=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 11, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac6_sel_sgmii= 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d11, 6, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_sel_sgmii= 1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 0, 1)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x2 (SGMII mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x2)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 2, 0)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X)
            {
                /*disable mac7 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable mac6 sel serdes*/
                if ((retVal = rtl8367c_setAsicReg(0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;

                /*cfg_sds_mode_sel_new=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x1F (reset mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                
                /*bit5: cfg_mac6_fib=0 & bit7: cfg_mac6_fib2=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_sel_sgmii= 1 & cfg_mac7_fib=1*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 3)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x4 (FIB1000 mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x4)) != RT_ERR_OK)
                    return retVal;
                
            }
            else if (mode == EXT_100FX)
            {
                /*disable mac7 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable mac6 sel serdes*/
                if ((retVal = rtl8367c_setAsicReg(0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;

                /*cfg_sds_mode_sel_new=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x1F (reset mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                
                /*bit5: cfg_mac6_fib=0 & bit7: cfg_mac6_fib2=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_sel_sgmii= 1 & cfg_mac7_fib=1*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 3)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x4 (FIB1000 mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x5)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode == EXT_1000X_100FX)
            {
                /*disable mac7 xMII mode*/
                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, 0)) != RT_ERR_OK)
                    return retVal;

                /*disable mac6 sel serdes*/
                if ((retVal = rtl8367c_setAsicReg(0x1d11, 0x1500)) != RT_ERR_OK)
                    return retVal;

                /*cfg_sds_mode_sel_new=1*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d95, 13, 1)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x1F (reset mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x1f)) != RT_ERR_OK)
                    return retVal;
                
                /*bit5: cfg_mac6_fib=0 & bit7: cfg_mac6_fib2=0*/
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 5, 0)) != RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicRegBit(0x1d41, 7, 0)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_sel_sgmii= 1 & cfg_mac7_fib=1*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 3)) != RT_ERR_OK)
                    return retVal;

                /*new_cfg_sds_mode=0x4 (FIB1000 mode)*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 0x1f00, 0x7)) != RT_ERR_OK)
                    return retVal;
            }
            else if (mode < EXT_SGMII)
            {
                /*disable mac7 sel SerDes mode*/
                if ((retVal = rtl8367c_setAsicReg(0x1d95, 0x1f00)) != RT_ERR_OK)
                    return retVal;

                /*cfg_mac7_sel_sgmii= 0 & cfg_mac7_fib=0*/
                if ((retVal = rtl8367c_setAsicRegBits(0x1d95, 3, 0)) != RT_ERR_OK)
                    return retVal;

                if ((retVal = rtl8367c_setAsicRegBits(0x13c3, 0xf, mode)) != RT_ERR_OK)
                    return retVal;

            }
            
        }

    }
    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_getAsicPortExtMode
 * Description:
 *      Get external interface mode configuration
 * Input:
 *      id 		- external interface id (0~1)
 *      pMode 	- external interface mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 *      RT_ERR_OUT_OF_RANGE - input parameter out of range
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortExtMode(rtk_uint32 id, rtk_uint32 *pMode)
{
    ret_t   retVal;
    rtk_uint32 regData, regValue, type;

    if(id >= RTL8367C_EXTNO)
        return RT_ERR_OUT_OF_RANGE;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }


    if (1 == type)
    {

        if (1 == id)
        {
            if( (retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, &regData)) != RT_ERR_OK)
                return retVal;

            if(1 == regData)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            if( (retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, &regData)) != RT_ERR_OK)
                return retVal;

            if(1 == regData)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
        }

    	if(0 == id || 1 == id)
      	    return rtl8367c_getAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT, RTL8367C_SELECT_GMII_0_MASK << (id * RTL8367C_SELECT_GMII_1_OFFSET), pMode);
    	else
           return rtl8367c_getAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1, RTL8367C_SELECT_GMII_2_MASK, pMode);

    }
    else if (2 == type)
    {
        if (1 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(0x1d92, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData & 0x4000)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            else if (((regData >> 8) & 0x1f) == 4)
            {
                *pMode = EXT_1000X;
                return RT_ERR_OK;
            }
            else if (((regData >> 8) & 0x1f) == 5)
            {
                *pMode = EXT_100FX;
                return RT_ERR_OK;
            }
            else if (((regData >> 8) & 0x1f) == 7)
            {
                *pMode = EXT_1000X_100FX;
                return RT_ERR_OK;
            }

            return rtl8367c_getAsicRegBits(0x1305, 0xf0, pMode);
        }
        else if (2 == id)
        {
#if 0
            if ((retVal = rtl8367c_getAsicRegBit(0x1d92, 6, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData == 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            if ((retVal = rtl8367c_getAsicRegBit(0x1d92, 7, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData == 1)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
#endif
            if ((retVal = rtl8367c_getAsicReg(0x1d92, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData & 0x40)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else if (regData & 0x80)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 4)
            {
                *pMode = EXT_1000X;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 5)
            {
                *pMode = EXT_100FX;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 7)
            {
                *pMode = EXT_1000X_100FX;
                return RT_ERR_OK;
            }

            return rtl8367c_getAsicRegBits(0x1305, 0xf, pMode);
        }
    }
    else if(3 == type)
    {
        if (1 == id)
        {

            if ((retVal = rtl8367c_getAsicReg(0x1d95, &regData))!=RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_getAsicReg(0x1d41, &regValue))!=RT_ERR_OK)
                return retVal;

            if((regValue & 0xa0)  == 0xa0 )
            {
                regData = regData >> 8;
                if((regData & 0x1f) == 4)
                {
                    *pMode = EXT_1000X;
                     return RT_ERR_OK;
                }
                else if((regData & 0x1f) == 5)
                {
                    *pMode = EXT_100FX;
                     return RT_ERR_OK;
                }
                else if((regData & 0x1f) == 7)
                {
                    *pMode = EXT_1000X_100FX;
                     return RT_ERR_OK;
                }

            }


            if ((retVal = rtl8367c_getAsicReg(0x1d11, &regData))!=RT_ERR_OK)
                return retVal;

            if((regData >> 6) & 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else if((regData >> 11) & 1)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
            else
            {
                if ((retVal = rtl8367c_getAsicRegBits(0x1305, 0xf0, &regData))!=RT_ERR_OK)
                    return retVal;

                if(regData < 6)
                    *pMode = regData;
                else if(regData == 6)
                    *pMode = EXT_RMII_MAC;
                else if(regData == 7)
                    *pMode = EXT_RMII_PHY;

                return RT_ERR_OK;
            }
        }
        else if (2 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(0x1d95, &regData))!=RT_ERR_OK)
                return retVal;

            if(((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x4))
            {
                *pMode = EXT_1000X;
                    return RT_ERR_OK;
            }
            else if (((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x5))
            {
                *pMode = EXT_100FX;
                    return RT_ERR_OK;
            }
            else if (((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x7))
            {
                *pMode = EXT_1000X_100FX;
                    return RT_ERR_OK;
            }
            else if(regData & 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else
            {
                if ((retVal = rtl8367c_getAsicRegBits(0x13c3, 0xf, &regData))!=RT_ERR_OK)
                    return retVal;

                *pMode = regData;

                return RT_ERR_OK;
            }
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8370_setAsicPortEnableAll
 * Description:
 *      Set ALL ports enable.
 * Input:
 *      enable - enable all ports.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortEnableAll(rtk_uint32 enable)
{
    if(enable >= 2)
        return RT_ERR_INPUT;

    return rtl8367c_setAsicRegBit(RTL8367C_REG_PHY_AD, RTL8367C_PDNPHY_OFFSET, !enable);
}

/* Function Name:
 *      rtl8367c_getAsicPortEnableAll
 * Description:
 *      Set ALL ports enable.
 * Input:
 *      enable - enable all ports.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 			- Success
 *      RT_ERR_SMI  		- SMI access error
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortEnableAll(rtk_uint32 *pEnable)
{
    ret_t retVal;
    rtk_uint32 regData;

    retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_PHY_AD, RTL8367C_PDNPHY_OFFSET, &regData);
    if(retVal !=  RT_ERR_OK)
        return retVal;

    if (regData==0)
        *pEnable = 1;
    else
        *pEnable = 0;

    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_setAsicPortSmallIpg
 * Description:
 *      Set small ipg egress mode
 * Input:
 *      port 	- Physical port number (0~7)
 *      enable 	- 0: normal, 1: small
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortSmallIpg(rtk_uint32 port, rtk_uint32 enable)
{
	if(port >= RTL8367C_PORTNO)
		return RT_ERR_PORT_ID;

	return rtl8367c_setAsicRegBit(RTL8367C_PORT_SMALL_IPG_REG(port), RTL8367C_PORT0_MISC_CFG_SMALL_TAG_IPG_OFFSET, enable);
}

/* Function Name:
 *      rtl8367c_getAsicPortSmallIpg
 * Description:
 *      Get small ipg egress mode
 * Input:
 *      port 	- Physical port number (0~7)
 *      pEnable 	- 0: normal, 1: small
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortSmallIpg(rtk_uint32 port, rtk_uint32* pEnable)
{
	if(port >= RTL8367C_PORTNO)
		return RT_ERR_PORT_ID;

	return rtl8367c_getAsicRegBit(RTL8367C_PORT_SMALL_IPG_REG(port), RTL8367C_PORT0_MISC_CFG_SMALL_TAG_IPG_OFFSET, pEnable);
}

/* Function Name:
 *      rtl8367c_setAsicPortLoopback
 * Description:
 *      Set MAC loopback
 * Input:
 *      port 	- Physical port number (0~7)
 *      enable 	- 0: Disable, 1: enable
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortLoopback(rtk_uint32 port, rtk_uint32 enable)
{
    if(port >= RTL8367C_PORTNO)
		return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(RTL8367C_PORT_MISC_CFG_REG(port), RTL8367C_PORT0_MISC_CFG_MAC_LOOPBACK_OFFSET, enable);
}

/* Function Name:
 *      rtl8367c_getAsicPortLoopback
 * Description:
 *      Set MAC loopback
 * Input:
 *      port 	- Physical port number (0~7)
 * Output:
 *      pEnable - 0: Disable, 1: enable
 * Return:
 *      RT_ERR_OK 		- Success
 *      RT_ERR_SMI  	- SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortLoopback(rtk_uint32 port, rtk_uint32 *pEnable)
{
    if(port >= RTL8367C_PORTNO)
		return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBit(RTL8367C_PORT_MISC_CFG_REG(port), RTL8367C_PORT0_MISC_CFG_MAC_LOOPBACK_OFFSET, pEnable);
}

/* Function Name:
 *      rtl8367c_setAsicPortRTCTEnable
 * Description:
 *      Set RTCT Enable echo response mode
 * Input:
 *      portmask 	- Port mask of RTCT enabled (0-4)
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK 		    - Success
 *      RT_ERR_SMI  	    - SMI access error
 *      RT_ERR_PORT_MASK    - Invalid port mask
 * Note:
 *      RTCT test takes 4.8 seconds at most.
 */
ret_t rtl8367c_setAsicPortRTCTEnable(rtk_uint32 portmask)
{
    ret_t       retVal;
    rtk_uint32  regData;
    rtk_uint32  port;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regData)) != RT_ERR_OK)
        return retVal;

    if( (regData == 0x0276) || (regData == 0x0597) )
        return RT_ERR_CHIP_NOT_SUPPORTED;

    for(port = 0; port <= 10 ; port++)
    {
        if(portmask & (0x0001 << port))
        {
             if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa422, &regData)) != RT_ERR_OK)
                 return retVal;

             regData &= 0x7FFF;
             if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa422, regData)) != RT_ERR_OK)
                 return retVal;

             regData |= 0x00F2;/*RTCT set to  echo response mode*/
             if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa422, regData)) != RT_ERR_OK)
                 return retVal;

             regData |= 0x0001;
             if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa422, regData)) != RT_ERR_OK)
                 return retVal;
        }
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setAsicPortRTCTDisable
 * Description:
 *      Set RTCT Disable
 * Input:
 *      portmask 	- Port mask of RTCT enabled (0-4)
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK 		    - Success
 *      RT_ERR_SMI  	    - SMI access error
 *      RT_ERR_PORT_MASK    - Invalid port mask
 * Note:
 *      RTCT test takes 4.8 seconds at most.
 */
ret_t rtl8367c_setAsicPortRTCTDisable(rtk_uint32 portmask)
{
    ret_t       retVal;
    rtk_uint32  regData;
    rtk_uint32  port;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regData)) != RT_ERR_OK)
        return retVal;

    if( (regData == 0x0276) || (regData == 0x0597) )
        return RT_ERR_CHIP_NOT_SUPPORTED;

    for(port = 0; port <= 10 ; port++)
    {
        if(portmask & (0x0001 << port))
        {
             if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa422, &regData)) != RT_ERR_OK)
                 return retVal;

             regData &= 0x7FFF;
             if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa422, regData)) != RT_ERR_OK)
                 return retVal;

             regData |= 0x00F0;
             if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa422, regData)) != RT_ERR_OK)
                 return retVal;

             regData &= ~0x0001;
             if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa422, regData)) != RT_ERR_OK)
                 return retVal;
        }
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      rtl8367c_getAsicPortRTCTResult
 * Description:
 *      Get RTCT result
 * Input:
 *      port 	- Port ID of RTCT result
 * Output:
 *      pResult - The result of port ID
 * Return:
 *      RT_ERR_OK 		            - Success
 *      RT_ERR_SMI  	            - SMI access error
 *      RT_ERR_PORT_MASK            - Invalid port mask
 *      RT_ERR_PHY_RTCT_NOT_FINISH  - RTCT test doesn't finish.
 * Note:
 *      RTCT test takes 4.8 seconds at most.
 *      If this API returns RT_ERR_PHY_RTCT_NOT_FINISH,
 *      users should wait a whole then read it again.
 */
ret_t rtl8367c_getAsicPortRTCTResult(rtk_uint32 port, rtl8367c_port_rtct_result_t *pResult)
{
    ret_t       retVal;
    rtk_uint32  regData, finish = 1;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regData)) != RT_ERR_OK)
        return retVal;

    if( (regData == 0x6367) )
    {
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa422, &regData)) != RT_ERR_OK)
            return retVal;

        if((regData & 0x8000) == 0x8000)
        {
            /* Channel A */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802a)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelAOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelAShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelAMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelALinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel B */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802e)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelBOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelBShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelBMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelBLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel C */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8032)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelCOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelCShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelCMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelCLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel D */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8036)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelDOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelDShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelDMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelDLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel A Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802c)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelALen = (regData / 2);

            /* Channel B Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8030)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelBLen = (regData / 2);

            /* Channel C Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8034)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelCLen = (regData / 2);

            /* Channel D Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8038)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelDLen = (regData / 2);
        }
        else
            finish = 0;
    }
    else if(regData == 0x6368)
    {
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa422, &regData)) != RT_ERR_OK)
            return retVal;

        if((regData & 0x8000) == 0x8000)
        {
            /* Channel A */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802b)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelAOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelAShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelAMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelALinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel B */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802f)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelBOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelBShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelBMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelBLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel C */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8033)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelCOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelCShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelCMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelCLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel D */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8037)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelDOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelDShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelDMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelDLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel A Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802d)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelALen = (regData / 2);

            /* Channel B Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8031)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelBLen = (regData / 2);

            /* Channel C Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8035)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelCLen = (regData / 2);

            /* Channel D Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8039)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelDLen = (regData / 2);
        }
    }
    else if((regData == 0x6511) || (regData == 0x0801))
    {
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa422, &regData)) != RT_ERR_OK)
            return retVal;

        if((regData & 0x8000) == 0x8000)
        {
            /* Channel A */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802a)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelAOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelAShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelAMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelALinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel B */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802e)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelBOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelBShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelBMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelBLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel C */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8032)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelCOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelCShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelCMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelCLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel D */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8036)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelDOpen       = (regData == 0x0048) ? 1 : 0;
            pResult->channelDShort      = (regData == 0x0050) ? 1 : 0;
            pResult->channelDMismatch   = ((regData == 0x0042) || (regData == 0x0044)) ? 1 : 0;
            pResult->channelDLinedriver = (regData == 0x0041) ? 1 : 0;

            /* Channel A Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x802c)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelALen = (regData / 2);

            /* Channel B Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8030)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelBLen = (regData / 2);

            /* Channel C Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8034)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelCLen = (regData / 2);

            /* Channel D Length */
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xa436, 0x8038)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xa438, &regData)) != RT_ERR_OK)
                return retVal;

            pResult->channelDLen = (regData / 2);
        }
    }
    else
        return RT_ERR_CHIP_NOT_SUPPORTED;

    if(finish == 0)
        return RT_ERR_PHY_RTCT_NOT_FINISH;
    else
        return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_sdsReset
 * Description:
 *      Reset Serdes
 * Input:
 *      id 	- EXT ID
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK 		            - Success
 *      RT_ERR_SMI  	            - SMI access error
 * Note:
 *      None.
 */
ret_t rtl8367c_sdsReset(rtk_uint32 id)
{
    rtk_uint32 retVal, regValue, state, i, option;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            option = 0;
            break;
        case 0x0652:
        case 0x6368:
            option = 1;
            break;
        case 0x0801:
        case 0x6511:
            option = 2;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if(option == 0)
    {
        if (1 == id)
        {
            if((retVal = rtl8367c_setAsicReg(0x6601, 0x0000)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6602, 0x1401)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6600, 0x00C0)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6601, 0x0000)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6602, 0x1403)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6600, 0x00C0)) != RT_ERR_OK)
                return retVal;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(option == 1)
    {
        if (1 == id)
        {
            if((retVal = rtl8367c_getAsicReg(0x1311, &state)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(0x1311, 0x66)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(0x1311, 0x1066)) != RT_ERR_OK)
                return retVal;

            while(1)
            {
                if((retVal = rtl8367c_getAsicReg(0x1d9d, &regValue)) != RT_ERR_OK)
                    return retVal;
                if((regValue >> 8) & 1)
                    break;
            }

            for (i=0; i<0xffff; i++);

            if((retVal = rtl8367c_setAsicReg(0x133d, 0x2)) != RT_ERR_OK)
                return retVal;

            for (i=0; i<0xffff; i++);

            if((retVal = rtl8367c_setAsicReg(0x6601, 0x0)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6602, 0x1401)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6600, 0xc1)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6601, 0x0)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6602, 0x1403)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6600, 0xc1)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(0x133d, 0x0)) != RT_ERR_OK)
                return retVal;

            for (i=0; i<0xffff; i++);

            if((retVal = rtl8367c_setAsicReg(0x1311, state)) != RT_ERR_OK)
                return retVal;


        }
        else if (2== id)
        {
            if((retVal = rtl8367c_getAsicReg(0x13c4, &state)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(0x13c4, 0x66)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(0x13c4, 0x1066)) != RT_ERR_OK)
                return retVal;

            while(1)
            {
                if((retVal = rtl8367c_getAsicReg(0x1d9d, &regValue)) != RT_ERR_OK)
                    return retVal;
                if((regValue >> 9) & 1)
                    break;
            }

            for (i=0; i<0xffff; i++);

            if((retVal = rtl8367c_setAsicReg(0x133d, 0x2)) != RT_ERR_OK)
                return retVal;

            for (i=0; i<0xffff; i++);

            if((retVal = rtl8367c_setAsicReg(0x6601, 0x0)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6602, 0x1401)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6600, 0xc0)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6601, 0x0)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6602, 0x1403)) != RT_ERR_OK)
                return retVal;
            if((retVal = rtl8367c_setAsicReg(0x6600, 0xc0)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_setAsicReg(0x133d, 0x0)) != RT_ERR_OK)
                return retVal;

            for (i=0; i<0xffff; i++);

            if((retVal = rtl8367c_setAsicReg(0x13c4, state)) != RT_ERR_OK)
                return retVal;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(option == 2)
    {
        if((retVal = rtl8367c_setAsicRegBit(0x1322, 3, 1)) != RT_ERR_OK)
                return retVal;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getSdsLinkStatus
 * Description:
 *      Get SGMII status
 * Input:
 *      id 	- EXT ID
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK 		            - Success
 *      RT_ERR_SMI  	            - SMI access error
 * Note:
 *      None.
 */
ret_t rtl8367c_getSdsLinkStatus(rtk_uint32 ext_id, rtk_uint32 *pSignalDetect, rtk_uint32 *pSync, rtk_uint32 *pLink)
{
    rtk_uint32 retVal, regValue, type;


    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 0;
            break;
        case 0x0652:
        case 0x6368:
            type = 1;
            break;
        case 0x0801:
        case 0x6511:
            type = 2;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if(type == 0)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x003D))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0080))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
            *pSync = (regValue & 0x0001) ? 1 : 0;
            *pLink = (regValue & 0x0010) ? 1 : 0;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 1)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x003D))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0081))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
            *pSync = (regValue & 0x0001) ? 1 : 0;
            *pLink = (regValue & 0x0010) ? 1 : 0;
        }
        else if (2 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x003D))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0080))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
            *pSync = (regValue & 0x0001) ? 1 : 0;
            *pLink = (regValue & 0x0010) ? 1 : 0;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 2)
    {
        if((retVal = rtl8367c_getAsicSdsReg(0, 30, 1, &regValue)) != RT_ERR_OK)
            return retVal;
        if((retVal = rtl8367c_getAsicSdsReg(0, 30, 1, &regValue)) != RT_ERR_OK)
            return retVal;

        *pSignalDetect = (regValue & 0x0100) ? 1 : 0;
        *pSync = (regValue & 0x0001) ? 1 : 0;
        *pLink = (regValue & 0x0010) ? 1 : 0;

    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_setSgmiiNway
 * Description:
 *      Set SGMII Nway
 * Input:
 *      ext_id 	    - EXT ID
 *      state       - SGMII Nway state
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK 		            - Success
 *      RT_ERR_SMI  	            - SMI access error
 * Note:
 *      None.
 */
ret_t rtl8367c_setSgmiiNway(rtk_uint32 ext_id, rtk_uint32 state)
{
    rtk_uint32 retVal, regValue, type;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 0;
            break;
        case 0x0652:
        case 0x6368:
            type = 1;
            break;
        case 0x0801:
        case 0x6511:
            type = 2;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if(type == 0)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0080))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                   return retVal;

            if(state)
                  regValue |= 0x0200;
            else
                  regValue &= ~0x0200;

            regValue |= 0x0100;

            if ((retVal = rtl8367c_setAsicReg(0x6602, regValue))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x00C1))!=RT_ERR_OK)
                   return retVal;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 1)
    {
        if (1 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0081))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                   return retVal;

            if(state)
                  regValue |= 0x0200;
            else
                  regValue &= ~0x0200;

            regValue |= 0x0100;

            if ((retVal = rtl8367c_setAsicReg(0x6602, regValue))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                   return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x00C1))!=RT_ERR_OK)
                   return retVal;
        }
        else if (2 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0080))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            if(state)
                regValue |= 0x0200;
            else
                regValue &= ~0x0200;

            regValue |= 0x0100;

            if ((retVal = rtl8367c_setAsicReg(0x6602, regValue))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x00C0))!=RT_ERR_OK)
                return retVal;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 2)
    {
        if ((retVal = rtl8367c_getAsicSdsReg(0, 2, 0, &regValue))!=RT_ERR_OK)
            return retVal;

        if(state & 1)
            regValue &= ~0x100;
        else
            regValue |= 0x100;

        if ((retVal = rtl8367c_setAsicSdsReg(0, 2, 0, regValue))!=RT_ERR_OK)
            return retVal;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      rtl8367c_getSgmiiNway
 * Description:
 *      Get SGMII Nway
 * Input:
 *      ext_id 	    - EXT ID
 *      state       - SGMII Nway state
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK 		            - Success
 *      RT_ERR_SMI  	            - SMI access error
 * Note:
 *      None.
 */
ret_t rtl8367c_getSgmiiNway(rtk_uint32 ext_id, rtk_uint32 *pState)
{
    rtk_uint32 retVal, regValue, type;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 0;
            break;
        case 0x0652:
        case 0x6368:
            type = 1;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if(type == 0)
    {
        if (1 == ext_id)
        {
                if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0080))!=RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                    return retVal;

                if(regValue & 0x0200)
                    *pState = 1;
                else
                    *pState = 0;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 1)
    {
        if (1 == ext_id)
        {
                if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0081))!=RT_ERR_OK)
                    return retVal;
                if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                    return retVal;

                if(regValue & 0x0200)
                    *pState = 1;
                else
                    *pState = 0;
        }
        else if (2 == ext_id)
        {
            if ((retVal = rtl8367c_setAsicReg(0x6601, 0x0002))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_setAsicReg(0x6600, 0x0080))!=RT_ERR_OK)
                return retVal;
            if ((retVal = rtl8367c_getAsicReg(0x6602, &regValue))!=RT_ERR_OK)
                return retVal;

            if(regValue & 0x0200)
                *pState = 1;
            else
                *pState = 0;
        }
        else
            return RT_ERR_PORT_ID;
    }
    else if(type == 2)
    {
        if ((retVal = rtl8367c_getAsicSdsReg(0, 2, 0, &regValue))!=RT_ERR_OK)
            return retVal;

        if(regValue & 0x100)
            *pState = 0;
        else
            *pState = 1;
    }

    return RT_ERR_OK;
}
