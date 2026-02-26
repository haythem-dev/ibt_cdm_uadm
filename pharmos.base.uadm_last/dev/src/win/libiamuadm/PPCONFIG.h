#pragma once
#pragma once
#ifndef __C_PPCONFIG_H____
#define __C_PPCONFIG_H____

typedef struct PPCONFIG
{
	char MAIN_HOST[20 +1];
	char BACKUP_HOST[20 +1];
	char MAIN_DB[20 +1];
	char BACKUP_DB[20 +1];

} PPCONFIG;

#endif // !__C_PPCONFIG_H____



