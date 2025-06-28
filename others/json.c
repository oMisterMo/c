#include <stdio.h>
#include <stdlib.h>
// #include "cJSON.h"
#include <cjson/cJSON.h>
#include "raylib.h"

char* LoadJSON(const char *filename) {
    // Load json file into memory
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File opening failed");
        // return EXIT_FAILURE;    // error this function returns char*
    }

    printf("FILE OPENED...\n");
    printf("\n");
    
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    printf("\t...Load data in ram\n");
    printf("\t...length (bytes): %ld\n", length);
    printf("\n");
    char *data = malloc(length + 1);
    fread(data, 1, length, fp);
    data[length] = '\0';
    fclose(fp);
    printf("FILE CLOSED...\n");
    printf("\n");

    return data;
}

void FIRST_ATTEMPT(const cJSON *json) {
    // const cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
    // printf("Name: %s\n", name->valuestring);
    const cJSON *frames = cJSON_GetObjectItemCaseSensitive(json, "frames");
    const cJSON *meta = cJSON_GetObjectItemCaseSensitive(json, "meta");

    char *app = meta->child->valuestring;
    char *version = meta->child->next->valuestring;
    char *image = meta->child->next->next->valuestring;
    char *format = meta->child->next->next->next->valuestring;
    cJSON *size = meta->child->next->next->next->next;
    int w = size->child->valueint;
    int h = size->child->next->valueint;
    int scale = size->next->valueint;

    // When printing, you must deallocated data
    char *frames_print = cJSON_Print(frames);
    char *meta_print = cJSON_Print(meta);

    printf("==========================\n");
    printf("JSON\n");
    printf("==========================\n");
    printf("app: %s\n", app);
    printf("version: %s\n", version);
    printf("image: %s\n", image);
    printf("format: %s\n", format);
    printf("w,h: %d, %d\n", w, h);
    printf("scale: %d\n", scale);
    printf("\n");
    printf("%b\n", cJSON_IsObject(meta));
    printf("%s\n", meta_print);
    // printf("%s\n", cJSON_PrintUnformatted(meta));
    // printf("%s\n", cJSON_Print(frames));

    free(meta_print);
    free(frames_print);
}

Rectangle* SECOND_ATTEMPT(const cJSON *json) {
    const cJSON *frame = NULL;
    const cJSON *frames = NULL;
    const cJSON *meta = NULL;


    frames = cJSON_GetObjectItemCaseSensitive(json, "frames");
    meta = cJSON_GetObjectItemCaseSensitive(json, "meta");

    // Create an array holding all of our Rectangles
    int len = cJSON_GetArraySize(frames);
    int i = 0;
    Rectangle *sprites = malloc(sizeof(Rectangle) * len);

    printf("=== Load Rectangles ===\n");
    printf("len: %d\n", len);
    printf("\n");

    cJSON_ArrayForEach(frame, frames) {
        cJSON *filename = cJSON_GetObjectItemCaseSensitive(frame, "filename");
        cJSON *srcRect = filename->next;

        cJSON *x = cJSON_GetObjectItemCaseSensitive(srcRect, "x");
        cJSON *y = cJSON_GetObjectItemCaseSensitive(srcRect, "y");
        cJSON *w = cJSON_GetObjectItemCaseSensitive(srcRect, "w");
        cJSON *h = cJSON_GetObjectItemCaseSensitive(srcRect, "h");

        sprites[i] = (Rectangle) { x->valueint,y->valueint,w->valueint,h->valueint };

        // printf("filename: %s\n", filename->valuestring);
        // printf("%d %d %d %d\n", x->valueint, y->valueint, w->valueint, h->valueint);
        // printf("-------------------\n");
        ++i;
    }


    return sprites;
}

// cc json.c -lcjson && ./a.out
int main(void) {

    // Load json file into memory
    FILE *fp = fopen("../graphics/resources/sprites/poke.json", "rb");
    if (!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;    // error this function returns char*
    }

    printf("FILE OPENED...\n");
    printf("\n");
    
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    printf("\t...Load data in RAM\n");
    printf("\t...length (bytes): %ld\n", length);
    printf("\n");
    char *data = malloc(length + 1);
    fread(data, 1, length, fp);
    data[length] = '\0';
    fclose(fp);
    printf("FILE CLOSED...\n");
    printf("\n");
    
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        printf("Error before: %s\n", cJSON_GetErrorPtr());
        return 1;
    }

    // Parse the data
    // FIRST_ATTEMPT(json);
    Rectangle *sprites = SECOND_ATTEMPT(json);

    for (int i = 0; i < 35; ++i) {
        Rectangle rec = sprites[i];
        printf("%d,%d,%d,%d\n", (int) rec.x, (int) rec.y, (int) rec.width, (int) rec.height);
    }
    


    cJSON_Delete(json);
    free(data);
    free(sprites);



    return 0;
}
