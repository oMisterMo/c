#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "cJSON.h"
#include <cjson/cJSON.h>
#include "raylib.h"

typedef struct Pokemon {
    char *filename;     // 8 bytes
    Rectangle srcRect;  // 16 bytes
} Pokemon;

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

Pokemon* SECOND_ATTEMPT(const cJSON *json) {
    const cJSON *frame = NULL;
    const cJSON *frames = NULL;
    const cJSON *meta = NULL;


    frames = cJSON_GetObjectItemCaseSensitive(json, "frames");
    meta = cJSON_GetObjectItemCaseSensitive(json, "meta");

    // Create an array holding all of our Rectangles
    int len = cJSON_GetArraySize(frames);
    int i = 0;
    Pokemon *sprites = malloc(sizeof(Pokemon) * len);

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

        // 1
        // memcpy(sprites->filename, filename, sizeof(filename));

        // 2
        // sprites[i].filename = malloc(strlen(filename->valuestring));
        // strcpy(sprites[i].filename, filename->valuestring);

        //3 
        sprites[i].filename = filename->valuestring;
        sprites[i].srcRect = (Rectangle) { x->valueint,y->valueint,w->valueint,h->valueint };

        // printf("filename: %s\n", filename->valuestring);
        // printf("%d %d %d %d\n", x->valueint, y->valueint, w->valueint, h->valueint);
        // printf("-------------------\n");
        ++i;
    }


    return sprites;
}

Rectangle FindPokemon(Pokemon *sprites, int size, char *name) {
    for (int i = 0; i < size; ++i) {
        Pokemon poke = sprites[i];
        printf("does %s == %s\n", name, poke.filename);
        if (strcmp(name, poke.filename) == 0) return poke.srcRect;
    }

    printf("\n");
    printf("Not Found :(\n");
    return (Rectangle) {0, 0, 0, 0};
}

// cc json.c -lcjson && ./a.out
int main(void) {

    // Load json file into memory, store in *data
    FILE *fp = fopen("../graphics/resources/sprites/poke.json", "rb");
    if (!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;
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





    
    // Parse the data
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        printf("Error before: %s\n", cJSON_GetErrorPtr());
        return 1;
    }

    // FIRST_ATTEMPT(json);
    Pokemon *sprites = SECOND_ATTEMPT(json);

    for (int i = 0; i <= 391; ++i) {
        char *name = sprites[i].filename;
        Rectangle rec = sprites[i].srcRect;
        // printf("%s\n", name);
        // printf("%s: %d,%d,%d,%d\n", name, (int) rec.x, (int) rec.y, (int) rec.width, (int) rec.height);
    }


    // Now sort the data? Or dont. Just get the srcRect through the filename

    Rectangle bounds = FindPokemon(sprites, 391, "9");
    if (bounds.x == 0 && bounds.y == 0 && bounds.width == 0 && bounds.height == 0) {
        printf("\n\nNO.\n\n");
    } else {
        printf("\n");
        printf("=================\n");
        printf("%d,%d,%d,%d\n", (int) bounds.x, (int) bounds.y, (int) bounds.width, (int) bounds.height);
        printf("=================\n");
    }

    


    // data = memory allocated for json file
    free(data);

    // json = cJSON object, the data post parse
    cJSON_Delete(json);

    // sprite = the pokemon collection I created
    free(sprites);



    return 0;
}
