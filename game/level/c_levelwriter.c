#include "c_levelwriter.h"


#include <byteswap.h>

void levelwriter_write_binary_level(Level *level, const char *filename, bool use_same) {
    FILE *level_file = fopen(filename, "wb");

    if (level_file == NULL)
    {
        fatal("Error: could not open %s", filename);
    }

    uint16_t version = __bswap_16(2021);
    char header[6] = {'S', 'P', 'G', 'b', 'i', 'n'};

    fwrite(header, sizeof(char), 6, level_file);
    fwrite(&version, sizeof(uint16_t), 1, level_file);

    uint16_t width = __bswap_16(level->width);
    uint16_t height = __bswap_16(level->height);
    fwrite(&width, 2, 1, level_file);
    fwrite(&height, 2, 1, level_file);

    uint8_t name_length = strlen(level->name);
    fwrite(&name_length, 1, 1, level_file);
    fwrite(level->name, sizeof(char), name_length, level_file);

    //Tile storing: using a buffer byte that gets filled per 2 bits and stored once filled.
    uint8_t buffer = 0;
    uint8_t new_tile;
    int buffer_position = 0;
    Material prev_GM;
    int prev_GH; 

    for (int x = 0; x < level->width; x++)
    {
        for (int y = 0; y < level->height; y++)
        {
            if (buffer_position == 8)
            {
                fwrite(&buffer, 1, 1, level_file);
                buffer = 0;
                buffer_position = 0;
            }
            
           
            if (level->points[x][y].ground_material == prev_GM && level->points[x][y].ground_height == prev_GH && use_same)
            {
                new_tile = 3;
                buffer = (buffer <<2) | new_tile;
                buffer_position += 2;
                if (buffer_position == 8)
                {
                    fwrite(&buffer, 1, 1, level_file);
                    buffer = 0;
                    buffer_position = 0;
                }
            }
            
            else
            {
                switch (level->points[x][y].ground_material)
                {
                case ART_GRASS:
                    new_tile = 0;
                    buffer = (buffer <<2) | new_tile;
                    buffer_position += 2;
                    break;
                case ART_WATER:
                    new_tile = 1;
                    buffer = (buffer <<2) | new_tile;
                    buffer_position += 2;
                    break;
                case ART_ROAD:
                    new_tile = 2;
                    buffer = (buffer <<2) | new_tile;
                    buffer_position += 2;
                    break;
                default:
                    fatal("Error: ground material not supported\n");
                    break;
                }
                if (buffer_position == 8)
                {
                    fwrite(&buffer, 1, 1, level_file);
                    buffer = 0;
                    buffer_position = 0;
                }
                buffer = (buffer <<2) | (level->points[x][y].ground_height - 1);
                buffer_position += 2;
                prev_GM = level->points[x][y].ground_material;
                prev_GH = level->points[x][y].ground_height;
            }
        }
    }

    //Filling up buffer with padding if it still has empty spaces.
    if(buffer_position != 0)
    {
        buffer = buffer <<(8-buffer_position) | 0;
        fwrite(&buffer, 1, 1, level_file);
    }

    uint16_t item_count = __bswap_16(level->item_count);
    fwrite(&item_count, 2, 1, level_file);

    for (int i = 0; i < level->item_count; i++)
    {
        LevelItem current_item = level->items[i];

        uint16_t x_coord = __bswap_16(current_item.x);
        uint16_t y_coord = __bswap_16(current_item.y);
        fwrite(&x_coord, 2, 1, level_file);
        fwrite(&y_coord, 2, 1, level_file);

        uint8_t LIT_VAR = current_item.type;
        
        switch (current_item.type)
        {
        case CAR:
            LIT_VAR = (LIT_VAR <<2) | current_item.dir;
            LIT_VAR = (LIT_VAR <<3) | current_item.color;
            break;
        case EXIT:
            LIT_VAR = (LIT_VAR <<2) | current_item.dir;
            LIT_VAR = LIT_VAR <<3;
            break;
        case FILTER:
            LIT_VAR = (LIT_VAR <<3) | current_item.count;
            LIT_VAR = LIT_VAR <<2;
            break;
        case CRATE:
            LIT_VAR = (LIT_VAR <<3) | current_item.color;
            LIT_VAR = LIT_VAR <<2;
            break;
        case DECORATION:
            LIT_VAR = (LIT_VAR <<2) | current_item.art_variant;
            LIT_VAR = LIT_VAR <<3;
            break;
        case BUILDING:
            LIT_VAR = (LIT_VAR <<3) | current_item.count;
            LIT_VAR = (LIT_VAR <<2) | current_item.dir;
            break;
        case MARKER:
            LIT_VAR = (LIT_VAR <<3) | current_item.color;
            LIT_VAR = LIT_VAR <<2;
            break;
        default:
            fatal("Error: level item not supported\n");
            break;
        }
        fwrite(&LIT_VAR, 1, 1, level_file);
    }
    fclose(level_file);
}
