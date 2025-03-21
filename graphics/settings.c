/**
 *
 * Details here of WaveFormat
 *
 * monitor 0 | 1 | 2 ...
 * fullscreen true | false
 *
 *
 */
#if !defined(SETTING_)
#define SETTING_



#include <stdio.h>
#include <string.h>
#include "raylib.h"

char *filename = "settings";

void SaveCurrentSettings() {
    int monitor = GetCurrentMonitor();
    bool fullscreen = IsWindowFullscreen();

    // char *text = TextFormat("monitor %d\nfullscreen %d\n", monitor, fullscreen);
    char buf[256];
    snprintf(buf, sizeof(buf), "monitor=%d\nfullscreen=%d\n", monitor, fullscreen);
    SaveFileText(filename, buf);
}

void LoadCurrentSettings() {
    char *file = LoadFileText(filename);
    // char *data = LoadFileData(filename, sizeof(*file));
    printf("\nFile:\n\n");
    printf("%s\n", file);

    printf("---\n");
    char *ptr = strtok(file, "\n");
    int i = 0;
    while (ptr != NULL) {
        // Loop through tokens
        printf("%d: %s\n", ++i, ptr);
        ptr = strtok(NULL, "\n");
    }

    UnloadFileText(file);
}

int main(void) {

    if (!IsFileNameValid(filename)) {
        return -1;
    }
    // 
    if (FileExists(filename)) {
        // Load exisiting file...
        printf("Loading settings...\n");
        LoadCurrentSettings();
        
    } else {
        printf("File '%s' does not exist.\n", filename);
        SaveCurrentSettings();
    }
    
    return 0;
}



#endif // SETTING_
