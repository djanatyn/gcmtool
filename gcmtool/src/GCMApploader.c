/*
 *  GCMApploader.c
 *  gcmtool
 *
 *  Created by spike on Mon May 24 2004.
 *  Copyright (c) 2004 Sadistech. All rights reserved.
 *
 */

#include "GCMApploader.h"
#include "GCMutils.h"
#include <stdlib.h>

GCMApploaderStruct *GCMRawApploaderToStruct(char *rawApploader) {
	/*
	**  experimental function for creating a struct out of the apploader
	*/
	
	GCMApploaderStruct *a = (GCMApploaderStruct*)malloc(sizeof(GCMApploaderStruct));
	
	memcpy(a->date, rawApploader, GCM_APPLOADER_DATE_LENGTH);
	rawApploader += GCM_APPLOADER_DATE_LENGTH;
	
	a->entrypoint = ntohl(*((u32*)rawApploader)++);
	a->size = ntohl(*((u32*)rawApploader)++);
	a->unknown = ntohl(*((u32*)rawApploader)++); // some unknown value...
	
	rawApploader += 4; //skip some more padding... ?
	
	a->code = (char*)malloc(a->size);
	memcpy(a->code, rawApploader, (a->size));
	
	return a;
}

void GCMApploaderStructToRaw(GCMApploaderStruct *a, char *buf) {
	/*
	**  copies *a into *buf as raw data
	**  suitable for writing to a GCM or apploader.bin
	**
	**  the allocated size of *buf should be (a->size + GCM_APPLOADER_CODE_OFFSET)
	*/
	
	if (!a) return;
	
	char *start = buf;
	
	memcpy(buf, a->date, GCM_APPLOADER_DATE_LENGTH);
	buf += GCM_APPLOADER_DATE_LENGTH;
	
	u32 *entrypoint = (u32*)malloc(sizeof(u32));
	*entrypoint = htonl(a->entrypoint);
	memcpy(buf, entrypoint, GCM_APPLOADER_ENTRYPOINT_LENGTH);
	free(entrypoint);
	buf += GCM_APPLOADER_ENTRYPOINT_LENGTH;
	
	u32 *size = (u32*)malloc(sizeof(u32));
	*size = htonl(a->size);
	memcpy(buf, size, GCM_APPLOADER_SIZE_LENGTH);
	free(size);
	buf += GCM_APPLOADER_SIZE_LENGTH;
	
	u32 *unknown = (u32*)malloc(sizeof(u32));
	*unknown = htonl(a->unknown);
	memcpy(buf, unknown, GCM_APPLOADER_UNKNOWN_LENGTH);
	free(unknown);
	buf += GCM_APPLOADER_UNKNOWN_LENGTH;
	
	buf += 4; // skip some padding ?
	
	memcpy(buf, a->code, (a->size));
	
	buf = start;
}

void GCMFreeApploaderStruct(GCMApploaderStruct *a) {
	/*
	**  convenience method for freeing an ApploaderStruct
	*/

	if (!a) return;
	
	if (a->date != NULL)
		free(a->date);
	if (a->code != NULL)
		free(a->code);
		
	free(a);
}