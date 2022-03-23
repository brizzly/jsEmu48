/*
 *     /
 *    /__  ___  ___  ____
 *   /  / /  / /__/ / / / /  /
 *  /  / /__/ /__  /   / /__/
 *      /
 *     /    version 0.9.0
 *
 * Copyright 2002 Daniel Nilsson
 *
 * This file is part of hpemu.
 *
 * Hpemu is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Hpemu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hpemu; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "bus.h"
#include "rom.h"


#if defined(_WIN32)
  #include <windows.h> // memset()
  #include <shlwapi.h> // PathRemoveFileSpecA
#endif

#if defined(__APPLE__)
  #import <CoreFoundation/CoreFoundation.h>
  #include <string.h> // memset()
  #include <ctype.h>	// isalpha
  #include <unistd.h> // getcwd
#endif


#ifdef __linux__
  #include <libgen.h>         // dirname
  #include <unistd.h>         // readlink
  #include <linux/limits.h>   // PATH_MAX
#endif

static char	WorkingPath[512];


static void getExePath()
{
	char	programPath[1024];
	char	temp[1024];
	memset(programPath,0,sizeof(programPath));
	memset(temp,0,sizeof(temp));
	
#if defined(__APPLE__)
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)programPath, PATH_MAX)) // Error: expected unqualified-id before 'if'
	{
		// error!
	}
	CFRelease(resourcesURL); // error: expected constructor, destructor or type conversion before '(' token
	
	chdir(programPath); // error: expected constructor, destructor or type conversion before '(' token
	//std::cout << "Current Path: " << path << std::endl; // error: expected constructor, destructor or type conversion before '<<' token
#endif
	
#if defined(_WIN32)
	GetModuleFileNameA(NULL,temp,sizeof(temp));
	PathRemoveFileSpecA(temp);
	
	//recopy path and duplicate "\"
	int j=0;
	for(int i =0;i<(int)(strlen(temp)+1);i++)
	{
		programPath[j]=temp[i];
		if (temp[i]=='\\')
			programPath[j]='/'; // '\\';
		j++;
	}
#endif

#ifdef __linux__
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    const char* path;
    if (count != -1) {
      path = dirname(result);
      strcpy(programPath, path);
    }
#endif

    memset(WorkingPath, 0, sizeof(WorkingPath));
    strcpy(WorkingPath, programPath);

    printf("exec path = %s\n", WorkingPath);
}


static int file_size(char *name)
{
	memset(WorkingPath, 0, sizeof(WorkingPath));
	getExePath();
	
	FILE *f;
	char fullpath[1024];
	sprintf(fullpath, "%s/%s", WorkingPath, name);
	printf("%s\n", fullpath);
	f = fopen(fullpath, "r");
	if(!f) {
		return 0;
	}
	fseek(f, 0, SEEK_END); // seek to end of file
	int size = (int)ftell(f); // get current file pointer
	fseek(f, 0, SEEK_SET); // seek back to beginning of file
	// proceed with allocating memory and reading the file
	fclose(f);
	return size;
}

void rom_init(void)
{
	printf("rom_init..\n");
    int size;
    char *name = "hpemu.rom";
    byte *buf, *ptr1, *ptr2;
    //PACKFILE *f;
	FILE *f;

    size = file_size(name);
    if (!size) {
		printf("rom_init failed\n");
		exit(0x10);
    }
    if (size != 256*1024 && size != 512*1024 && size != 1024*1024) {
		printf("rom_init failed2..\n");
		exit (0x11);
    }
    buf = malloc(size);
    if (!buf) {
		printf("rom_init failed3..\n");
		exit(0x12);
    }
	
	char fullpath[1024];
	sprintf(fullpath, "%s/%s", WorkingPath, name);

    //f = pack_fopen(name, "r");
	f = fopen(fullpath, "r");
    if (!f) {
		printf("rom_init failed4..\n");
		exit(0x13);
    }
	int r = (int)fread(buf, sizeof(char), size, f);
    if (r != size) { // pack_fread
		printf("rom_init failed5..\n");
		exit(0x14);
    }
   	//pack_fclose(f);
	fclose(f);
    if (buf[0] & 0xF0 || buf[1] & 0xF0) {
	if (size == 1024*1024) {
		printf("rom_init failed6..\n");
	    exit(0x15);
	}
	buf = realloc(buf, size*2);
	if (!buf) {
		printf("rom_init failed7..\n");
	    exit(0x16);
	}
	ptr1 = buf+size-1;
	size *= 2;
	ptr2 = buf+size-1;
	do {
	    *(ptr2--) = (*ptr1 >> 4) & 0x0F;
	    *(ptr2--) = *(ptr1--) & 0x0F;
	} while (ptr1 != ptr2);
    }
    bus_info.rom_data = buf;
    bus_info.rom_mask = size - 1;
	printf("rom_init succeed!\n");
}

void rom_exit(void)
{
    free(bus_info.rom_data);
    bus_info.rom_data = NULL;
    bus_info.rom_mask = 0x00000;
}
