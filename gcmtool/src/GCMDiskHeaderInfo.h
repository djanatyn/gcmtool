/*
 *  GCMDiskHeaderInfo.h
 *  gcmtool
 *
 *  Created by spike on Mon May 24 2004.
 *  Copyright (c) 2004 Sadistech. All rights reserved.
 *
 */

/*
**  Utility functions for dealing with bi2.bin from a gamecube dvd image (.gcm or .iso)
*/

#ifndef _GCM_DISK_HEADER_INFO_H_
#define _GCM_DISK_HEADER_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef struct gcm_disk_header_info_struct {
	u32		debugMonitorSize;
	u32		simulatedMemorySize;
	u32		argumentOffset;
	u32		debugFlag;
	u32		trackLocation;
	u32		trackSize;
	u32		countryCode;
	u32		unknown1;
	u32		unknown2;
}GCMDiskHeaderInfoStruct;

//functions and stuff

void GCMDiskHeaderInfoStructToRaw(GCMDiskHeaderInfoStruct *d, char *buf);
GCMDiskHeaderInfoStruct *GCMRawDiskHeaderInfoToStruct(char *rawInfo);

//free up memory
void GCMFreeDiskHeaderInfoStruct(GCMDiskHeaderInfoStruct *dhi);

#ifdef __cplusplus
};
#endif

#endif /* _GCM_DISK_HEADER_INFO_H_ */
