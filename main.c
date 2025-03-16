#include "common.h"
#include "disassembler.h"

size_t load_rom(char *rom_path, uint8_t **dest_ptr)
{
    FILE *file;
    size_t file_size;

    printf("Loading ROM at path: %s\n", rom_path);

    file = fopen(rom_path, "rb");

    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_SUCCESS);
    }

    printf("ROM opened successfully.\n");

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);

    printf("ROM size: %lu bytes.\n", file_size);

    rewind(file);
    *dest_ptr = malloc(file_size+1);

    if (*dest_ptr == NULL)
    {
        perror("Failed to allocate space for ROM");
        exit(EXIT_SUCCESS);
    }
    
    fread(*dest_ptr, 1, file_size, file);
    fclose(file);

    return file_size;
}

int main(int argc, char *argv[])
{
    printf("Hello!\n");
    usleep(500000);

    if (argc < 2)
    {
        printf("Next time, input a file path. Goodbye!\n");
        sleep(1);
        return EXIT_SUCCESS;
    }

    char *rom_path = argv[1];
    uint8_t *rom_ptr = NULL;

    size_t rom_size = load_rom(rom_path, &rom_ptr);

    printf("ROM loaded to memory.\n");
    sleep(1);

    disassemble(rom_ptr, rom_size);

    free(rom_ptr);

    return EXIT_SUCCESS;
}
