/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
*
* Abstract: Switch core vlan table access driver source code.
*
* ---------------------------------------------------------------
*/

#include <strings.h>
#include <linux/config.h>
#include <rtl_types.h>
#include <rtl_errno.h>
#include <rtl8196x/asicregs.h>
#include <rtl8196x/swCore.h>
#include <rtl8196x/vlanTable.h>
#include <rtl8196x/swTable.h>
#include <asm/mipsregs.h>

extern void tableAccessForeword(uint32, uint32, void *);
extern int32 swTable_addEntry(uint32 tableType, uint32 eidx, void *entryContent_P);
extern int32 swTable_modifyEntry(uint32 tableType, uint32 eidx, void *entryContent_P);
extern int32 swTable_forceAddEntry(uint32 tableType, uint32 eidx, void *entryContent_P);
extern int32 swTable_readEntry(uint32 tableType, uint32 eidx, void *entryContent_P);

#define swab16(x) \
           ((x & (uint16)0x00ffU) << 8) | \
            ((x & (uint16)0xff00U) >> 8)

#define swab32(x) \
            ((x & (uint32)0x000000ffUL) << 24) | \
             ((x & (uint32)0x0000ff00UL) <<  8) | \
             ((x & (uint32)0x00ff0000UL) >>  8) | \
             ((x & (uint32)0xff000000UL) >> 24)

#ifdef _LITTLE_ENDIAN
#define htonl(x) swab32(x)
#define htons(x) swab16(x)
#else
#define htonl(x) (x)
#define htons(x) (x)
#endif

int32 swCore_netifCreate(uint32 idx, rtl_netif_param_t * param)
{
    netif_table_t    entryContent;
    
    ASSERT_CSP(param);
    
    swTable_readEntry(TYPE_NETINTERFACE_TABLE, idx, &entryContent);

    if ( entryContent.valid )
    {
       return EEXIST;
    }

    bzero( (void *) &entryContent, sizeof(entryContent) );
    entryContent.valid = param->valid;
    entryContent.vid = param->vid;

#if defined(CONFIG_RTL8197F)

//    entryContent.mac47_19 = (((param->gMac.mac47_32) << 13) | ((param->gMac.mac31_16) >> 3)) & 0x1FFFFFFF;
//    entryContent.mac18_0 = (((param->gMac.mac31_16) << 16) | (param->gMac.mac15_0)) & 0x7FFFF;
	entryContent.mac47_19 = (param->gMac.octet[0]<<21) | (param->gMac.octet[1]<<13) | (param->gMac.octet[2]<<5) |
					 (param->gMac.octet[3]>>3);
	entryContent.mac18_0 = (param->gMac.octet[3]<<16) | (param->gMac.octet[4]<<8) | (param->gMac.octet[5]);

	entryContent.inACLStartH = (param->inAclStart >>1)&0x7f;
	entryContent.inACLStartL = param->inAclStart&0x1;
	entryContent.inACLEnd    = param->inAclEnd;
	entryContent.outACLStart = param->outAclStart;
	entryContent.outACLEnd   = param->outAclEnd;

	entryContent.enHWRouteV6 = param->enableRouteV6;

	entryContent.enHWRoute   = param->enableRoute;

	switch(param->macAddrNumber) {
		case 0:
		case 1:
		    entryContent.macMaskL = 1;
		    entryContent.macMaskH = 3;
		break;
		case 2:
		    entryContent.macMaskL = 0;
		    entryContent.macMaskH = 3;
		break;
		case 4:
		    entryContent.macMaskL = 0;
		    entryContent.macMaskH = 2;
		break;
		case 8:
		    entryContent.macMaskL = 0;
		    entryContent.macMaskH = 0;
			break;
		default:
		    return FAILED;//Not permitted macNumber value
	}
	entryContent.mtu   = param->mtu;
	entryContent.mtuV6 = param->mtuV6;

#elif defined(CONFIG_RTL8198C)
    entryContent.mac47_19 = ((param->gMac.mac47_32 << 13) | (param->gMac.mac31_16 >> 3)) & 0x1FFFFFFF;
    entryContent.mac18_0 = ((param->gMac.mac31_16 << 16) | param->gMac.mac15_0) & 0x7FFFF;

	entryContent.inACLStartH = (param->inAclStart >>1)&0x7f;
	entryContent.inACLStartL = param->inAclStart&0x1;
	entryContent.inACLEnd    = param->inAclEnd;
	entryContent.outACLStart = param->outAclStart;
	entryContent.outACLEnd   = param->outAclEnd;
	entryContent.enHWRouteV6 = 0;
	entryContent.enHWRoute   = param->enableRoute;
	switch(param->macAddrNumber) {
		case 0:
		case 1:
		    entryContent.macMaskL = 1;
		    entryContent.macMaskH = 3;
		break;
		case 2:
		    entryContent.macMaskL = 0;
		    entryContent.macMaskH = 3;
		break;
		case 4:
		    entryContent.macMaskL = 0;
		    entryContent.macMaskH = 2;
		break;
		case 8:
		    entryContent.macMaskL = 0;
		    entryContent.macMaskH = 0;
			break;
		default:
		    return EEXIST;//Not permitted macNumber value
	}

	entryContent.mtu   = param->mtu;
	entryContent.mtuv6 = param->mtu;
#else
    entryContent.mac47_19 = ((param->gMac.mac47_32 << 13) | (param->gMac.mac31_16 >> 3)) & 0x1FFFFFFF;
    entryContent.mac18_0 = ((param->gMac.mac31_16 << 16) | param->gMac.mac15_0) & 0x7FFFF;

    entryContent.inACLStartH = (param->inAclStart >> 2) & 0x1f;
    entryContent.inACLStartL = param->inAclStart & 0x3;
    entryContent.inACLEnd = param->inAclEnd;
    entryContent.outACLStart = param->outAclStart;
    entryContent.outACLEnd = param->outAclEnd;
    entryContent.enHWRoute = param->enableRoute;

    entryContent.macMask = 8 - (param->macAddrNumber & 0x7);

    entryContent.mtuH = param->mtu >> 3;
    entryContent.mtuL = param->mtu & 0x7;
#endif

    /* Write into hardware */
    if ( swTable_addEntry(TYPE_NETINTERFACE_TABLE, idx, &entryContent) == 0 )
        return 0;
    else
        /* There might be something wrong */
        ASSERT_CSP( 0 );
}

int32 vlanTable_create(uint32 vid, rtl_vlan_param_t * param)
{
    vlan_table_t    entryContent;
    
    ASSERT_CSP(param);
    
    swTable_readEntry(TYPE_VLAN_TABLE, vid, &entryContent);
    
    bzero( (void *) &entryContent, sizeof(entryContent) );
    entryContent.memberPort = param->memberPort & ALL_PORT_MASK;
    entryContent.egressUntag = param->egressUntag;
    entryContent.fid = param->fid;

#if defined(CONFIG_RTL8196D) || defined(CONFIG_RTL8196E)||defined(CONFIG_RTL8881A)
    entryContent.vid = vid;
#endif

    /* Write into hardware */
    if ( swTable_addEntry(TYPE_VLAN_TABLE, vid, &entryContent) == 0 )
        return 0;
    else
        /* There might be something wrong */
        ASSERT_CSP( 0 );
}

