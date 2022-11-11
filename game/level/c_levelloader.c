#include "c_levelloader.h"

#include "fixed_level_info.h"
#include "hardcoded_levels.h"

#include <assert.h>

const Material materials[3] = {ART_GRASS, ART_WATER, ART_ROAD};

void levelloader_load_binary_level(Level *level, unsigned int level_nr, const char *filename) {
    FILE *level_file = fopen(filename, "rb");

    if (level_file == NULL)
    {
        fatal("Error: could not open %s", filename);
    }

    level->source_file_name = calloc(strlen(filename) + 1, sizeof(char));
    strcpy(level->source_file_name, filename);

    //Start form 8th byte
    fseek(level_file, 8, SEEK_SET);

    uint16_t width;
    uint16_t height;
    fread(&width, 2, 1, level_file);
    fread(&height, 2, 1, level_file);
    level->width = ntohs(width);
    level->height = ntohs(height);

    uint8_t titel_length;
    fread(&titel_length, 1, 1, level_file);

    level->name = calloc(titel_length + 1, sizeof(char));
    fread(level->name, sizeof(char), titel_length, level_file);
    level->name[titel_length] = '\0';

    level->points = calloc(level->width, sizeof(LevelPoint*));

    //Sets a buffer and a buffer position to read bytes 2 bits at a time.
    uint8_t buffer;
    uint8_t temp_buffer;
    int buffer_position = 6;
    int prev_GM;
    int prev_GH;
    fread(&buffer, 1, 1, level_file);

    for (int x = 0; x < level->width; x++)
    {
        level->points[x] = calloc(level->height, sizeof(LevelPoint));
        for (int y = 0; y < level->height; y++)
        {  
            temp_buffer = buffer >>buffer_position;
            int GM = temp_buffer & 0b11;
            buffer_position -= 2;
            if (buffer_position < 0)
            {
                fread(&buffer, 1, 1, level_file);
                buffer_position = 6;
            }

            if (GM == 3)
            {
                level->points[x][y].ground_material = materials[prev_GM];
                level->points[x][y].ground_height = (prev_GH + 1);
            }
            else
            {
                temp_buffer = buffer >>buffer_position;
                int GH = temp_buffer & 0b11;
                buffer_position -= 2;
                if (buffer_position < 0)
                {
                    fread(&buffer, 1, 1, level_file);
                    buffer_position = 6;
                }
                level->points[x][y].ground_material = materials[GM];
                level->points[x][y].ground_height = (GH + 1);
                prev_GM = GM;
                prev_GH = GH;
            }
        }
    }

    uint16_t item_count;
    fread(&item_count, 2, 1, level_file);
    level->item_count = item_count;
    level->items = calloc(item_count, sizeof(LevelItem));

    for (size_t i = 0; i < level->item_count; i++)
    {
        uint16_t x_coord;
        fread(&x_coord, 2, 1, level_file);
        level->items[i].x = x_coord;

        uint16_t y_coord;
        fread(&y_coord, 2, 1, level_file);
        level->items[i].y = y_coord;
        
        uint8_t LIT_VAR;
        fread(&LIT_VAR, 1, 1, level_file);
        uint8_t LIT = LIT_VAR >>5;
        LIT = LIT & 0b111;
        uint8_t VAR = LIT_VAR & 0b11111;

        int color;
        int direction;
        int art_variant;
        int count;

        switch (LIT)
        {
        case 0:
            level->items[i].type = CAR;
            color = VAR & 0b111;
            direction = (VAR >>3) & 0b11;
            level->items[i].color = color;
            level->items[i].dir = direction;

            break;
        case 1:
            level->items[i].type = EXIT;
            direction = (VAR >>3) & 0b11;
            level->items[i].dir = direction;

            break;
        case 2:
            level->items[i].type = FILTER;
            count = (VAR >>2) & 0b111;
            level->items[i].count = count;

            break;
        case 3:
            level->items[i].type = CRATE;
            color = (VAR >>2) & 0b111;
            level->items[i].color = color;

            break;
        case 4:
            level->items[i].type = DECORATION;
            art_variant = (VAR >>3) & 0b11;
            level->items[i].art_variant = art_variant;

            break;
        case 5:
            level->items[i].type = BUILDING;
            direction = VAR & 0b11;
            count = (VAR >>2) & 0b111;
            level->items[i].dir = direction;
            level->items[i].count = count;

            break;
        case 6:
            level->items[i].type = MARKER;
            color = (VAR >>2) & 0b111;
            level->items[i].color = color;
            
            break;
        default:
            fatal("Error: level item does not exists ");
            break;
        }
        printf("{%d, %d, %d, %d, %d, %d, %d}\n", level->items[i].x, level->items[i].y, level->items[i].type,
         level->items[i].dir, level->items[i].color, level->items[i].art_variant, level->items[i].count);
    }
    fclose(level_file);
}

void levelloader_load_fixed_level(Level *level, unsigned int level_nr) {
    assert(level != NULL);
    assert(level_nr >= 0);
    //printf("\n%x\n",level_nr);
    assert(level_nr <TOTAL_LEVEL_COUNT);
    
    //TODO: uncomment the next line
    levelloader_load_binary_level(level, level_nr, level_binary_file[level_nr]);
    
    //TODO: comment the next line
    //load_hardcoded_level(level, level_nr);
}

void levelloader_load_demo_level(Level *level) {
    assert(level != NULL);
    //printf("loading demo");
    //TODO: uncomment the next line
    levelloader_load_binary_level(level, DEMO_LEVEL_NR, level_binary_file[DEMO_LEVEL_NR]);

    //TODO: comment the next line
    //load_hardcoded_level(level, DEMO_LEVEL_NR);
}
