/*
**	GCMBnrToolMain.c
**	Written by spike <spike@sadistech.com>
**
**	Comments to be added soon enough... blah. ;)
*/

//autoheader stuff
#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#define VERSION ""
#endif 

#include <stdio.h>
#include "GCMBnr.h"

//commandline params...
#define ARG_HELP					"-?"
#define ARG_HELP_SYN				"--help"
#define ARG_HELP_OPT				""
#define ARG_HELP_HELP				"Displays this help."

#define ARG_SET_ICON				"-i"
#define ARG_SET_ICON_SYN			"--icon"
#define ARG_SET_ICON_OPT			"[ " OPT_FORMAT_RAW " | " OPT_FORMAT_PPM " ] <pathname>"
#define ARG_SET_ICON_HELP			"Grabs the icon from <pathname> and injects it into the bnr. " OPT_FORMAT_RAW " is the default"

#define ARG_GET_ICON				"-gi"
#define ARG_GET_ICON_SYN			"--get-icon"
#define ARG_GET_ICON_OPT			"[ " OPT_FORMAT_RAW " | " OPT_FORMAT_PPM " ] <pathname>"
#define ARG_GET_ICON_HELP			"Extracts the icon from the bnr in the desired format. " OPT_FORMAT_RAW " is the default"

#define ARG_SET_NAME				"-n"
#define ARG_SET_NAME_SYN			"--name"
#define ARG_SET_NAME_OPT			"<name>"
#define ARG_SET_NAME_HELP			"Sets the name field to <name>"

#define ARG_SET_DEVELOPER			"-d"
#define ARG_SET_DEVELOPER_SYN		"--developer"
#define ARG_SET_DEVELOPER_OPT		"<developer>"
#define ARG_SET_DEVELOPER_HELP		"Sets the developer field to <developer>"

#define ARG_SET_FULL_NAME			"-fn"
#define ARG_SET_FULL_NAME_SYN		"--full-name"
#define ARG_SET_FULL_NAME_OPT		"<full_name>"
#define ARG_SET_FULL_NAME_HELP		"Sets the full name field to <full_name>"

#define ARG_SET_FULL_DEVELOPER		"-fd"
#define ARG_SET_FULL_DEVELOPER_SYN  "--full-deveoper"
#define ARG_SET_FULL_DEVELOPER_OPT  "<full_developer>"
#define ARG_SET_FULL_DEVELOPER_HELP "Sets the full developer field to <full_developer>"

#define ARG_SET_DESCRIPTION			"-d"
#define ARG_SET_DESCRIPTION_SYN		"--description"
#define ARG_SET_DESCRIPTION_OPT		"<description>"
#define ARG_SET_DESCRIPTION_HELP	"Sets the description field to <full_description>"

//additional options...
#define OPT_FORMAT_RAW				"-raw"
#define OPT_FORMAT_PPM				"-ppm"

//macros... although they may be simple...
//these are for getting help and synonyms and stuff
#define ARG_SYN(ARG)		ARG ## _SYN
#define PRINT_HELP(ARG)		printf("\t" ARG "%s" ARG ## _SYN " " ARG ## _OPT "\n\t\t" ARG ## _HELP "\n\n", strcmp("", ARG ## _SYN) == 0 ? "" : ", ");

// these are for the argument parsing engine...
#define GET_NEXT_ARG		*(++argv)
#define SKIP_NARG(n)		*(argv += n)	
#define CHECK_ARG(ARG)		strcmp(ARG, currentArg) == 0 || strcmp(ARG ## _SYN, currentArg) == 0
#define PEEK_ARG			*(argv + 1)
#define PEEK_NARG(n)		*(argv + n)


void printUsage();
void printExtendedUsage();

void openBnr();
void closeBnr();

FILE *bnrFile;
char *filename;

int main(int argc, char **argv) {

	char *newName = NULL;
	char *newDeveloper = NULL;
	char *newFullName = NULL;
	char *newFullDeveloper = NULL;
	char *newDescription = NULL;

	char *currentArg = NULL;
	do {
		currentArg = GET_NEXT_ARG;
		if (!currentArg) {
			//eek, there's no arg! must be an argument error... print usage...
			
			printUsage();
			exit(1);
		} else if (CHECK_ARG(ARG_HELP)) {
			//they want the extended usage, so print it and bail...
			
			printExtendedUsage();
			exit(1);
		} else if (CHECK_ARG(ARG_SET_NAME)) {
			//they want to set the name...
			if (PEEK_ARG) {
				//if there is a next argument...
				newName = GET_NEXT_ARG;
			}
		} else if (CHECK_ARG(ARG_SET_DEVELOPER)) {
			//they want to set the developer...
			if (PEEK_ARG) {
				//if there's a next argument
				newDeveloper = GET_NEXT_ARG;
			}
		} else if (CHECK_ARG(ARG_SET_FULL_NAME)) {
			//the want to set the full name
			if (PEEK_ARG) {
				//if there's a next argument
				newFullName = GET_NEXT_ARG;
			}
		} else if (CHECK_ARG(ARG_SET_FULL_DEVELOPER)) {
			//they want to set the full developer
			if (PEEK_ARG) {
				//if there's a next argument
				newFullDeveloper = GET_NEXT_ARG;
			}
		} else if (CHECK_ARG(ARG_SET_DESCRIPTION)) {
			//they want to set the description...
			if (PEEK_ARG) {
				//if there's a next argument
				newDescription = GET_NEXT_ARG;
			}
		} else {
			//if the argument doesn't fit anything else... it must be the filename.
			// set the filename and stop looping... start processing!
			filename = currentArg;
			
			break;
		}
	}while (*argv);
	
	openBnr();

	//read the file into a buffer...
	fseek(bnrFile, 0, SEEK_END);
	unsigned long len = ftell(bnrFile);
	rewind(bnrFile);

	char *data = (char*)malloc(len);
	
	if (fread(data, 1, len, bnrFile) != len) {
		printf("Reading from file... (%s)\n", filename);
		exit(1);
	}

	GCMBnrStruct *b = GCMRawBnrToStruct(data);

	//display bnr...
	printf("Version:	   \t%c\n", b->version);
	printf("Name:          \t%s\n", b->name);
	printf("Developer:     \t%s\n", b->developer);
	printf("Full Name:     \t%s\n", b->fullName);
	printf("Full Developer:\t%s\n", b->fullDeveloper);
	printf("Description:   \t%s\n", b->description);

	int fileChanged = 0;

	if (newName != NULL) {
		//let's set the name...
		bzero(b->name, GCM_BNR_GAME_NAME_LENGTH);
		strcpy(b->name, newName);
		fileChanged = 1;
	}
	
	if (newDeveloper != NULL) {
		//let's set the developer...
		bzero(b->developer, GCM_BNR_DEVELOPER_LENGTH);
		strcpy(b->developer, newDeveloper);
		fileChanged = 1;
	}
	
	if (newFullName != NULL) {
		bzero(b->fullName, GCM_BNR_FULL_TITLE_LENGTH);
		strcpy(b->fullName, newFullName);
		fileChanged = 1;
	}
	
	if (newFullDeveloper != NULL) {
		bzero(b->fullDeveloper, GCM_BNR_FULL_DEVELOPER_LENGTH);
		strcpy(b->fullDeveloper, newFullDeveloper);
		fileChanged = 1;
	}
	
	if (newDescription != NULL) {
		bzero(b->description, GCM_BNR_DESCRIPTION_LENGTH);
		strcpy(b->description, newDescription);
		fileChanged = 1;
	}
	
	if (fileChanged) {
		rewind(bnrFile);
		char *buf = (char*)malloc(GCM_BNR_LENGTH_V1);
		GCMBnrStructToRaw(b, buf);
		if (fwrite(buf, 1, GCM_BNR_LENGTH_V1, bnrFile) != GCM_BNR_LENGTH_V1) {
			printf("error writing to bnr! (%s)\n", filename);
			exit(1);
		}
	}
	
	closeBnr();
/*
	FILE *ofile = NULL;

	char outfilename[255];
	strcpy(outfilename, filename);
	strcat(outfilename, ".ppm");

	if (!(ofile = fopen(outfilename, "w"))) {
		printf("ERROR!\n");
		exit(1);
	}

	len = GCM_BNR_GRAPHIC_WIDTH * GCM_BNR_GRAPHIC_HEIGHT * 3 + 256;
	char *pic = (char*)malloc(len);
	
	GCMBnrGetImagePPM(b, pic);

	if (fwrite(pic, 1, len, ofile) != len) {
		printf("woops!\n");
		exit(1);
	}

	fclose(ofile);*/
}

void openBnr() {
	if (!(bnrFile = fopen(filename, "r+"))) {
		printf("Error opening %s\n", filename);
		exit(1);
	}
}

void closeBnr() {
	fclose(bnrFile);
}

void printUsage() {
	printf("gcmbnrtool %s- Utility for working with .bnr files from GameCube DVD Images (GCMs)\n", VERSION);
	printf("http://gcmtool.sourceforge.net\n\n");
	printf("Usage:\t");
	printf("gcmbnrtool <bnr_file>\n\n");
	printf("Use -? to view extended usage.\n\n");
}

void printExtendedUsage() {
	printUsage();
	
	PRINT_HELP(ARG_HELP);
}