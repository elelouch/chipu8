#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define PROGRAM_START 0x200
#define PROGRAM_END 0xe90
#define PROGRAM_SIZE PROGRAM_END - PROGRAM_START
#define MEMORY_SIZE 4096
#define STACK_SIZE 32
#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 32
#define KEYPAD_SIZE 16
#define REGISTERS_AMOUNT 16

struct chip8 {
	unsigned char memory[MEMORY_SIZE];
	unsigned char stack[STACK_SIZE];
	unsigned short sp;
	unsigned short pc;
	unsigned char grid[DISPLAY_WIDTH][DISPLAY_HEIGHT];
	unsigned char keypad[KEYPAD_SIZE];
	unsigned char regs[REGISTERS_AMOUNT];
};

void execute(struct chip8* c8, unsigned short instruction);
int initialize(struct chip8* c8, char* filepath);
unsigned short fetch_decode(struct chip8* c8);

int main(int argc, char *argv[])
{
	struct chip8 c8;
	char *program_path = NULL;
	unsigned short instruction;
	
	if (argc != 2) {
		return 4;
	}
	program_path = argv[1];
	initialize(&c8, program_path);

	while(1) {
		instruction = fetch_decode(&c8);
		if(!instruction) {
			break;
		}
        execute(&c8, instruction);
	}
	return 0;
}

int initialize(struct chip8* c8, char* filepath)
{
	unsigned char* mem = NULL;
	int fd = 0;
	unsigned int bread = 0;

	if (!c8 || !filepath) {
		exit(1);
	}

	memset(c8, '\0', sizeof(struct chip8));
	c8 -> pc = PROGRAM_START;

	// open rom
	fd = open(filepath, O_RDONLY);
	if (fd == -1) {
		exit(2);
	}
	mem = c8 -> memory;
	bread = read(fd, mem + PROGRAM_START, PROGRAM_SIZE);
	if(bread == -1) {
		exit(3);
	}
	close(fd);
	return 0;
}

unsigned short fetch_decode(struct chip8* c8)
{
	unsigned short* pc = 0;
	unsigned short instruction = 0;
	unsigned char* memory = NULL;
	unsigned char* two_bytes = NULL;

	if(!c8) return -1;
	pc = &c8 -> pc;
	memory = c8 -> memory;

	if(*pc == PROGRAM_END || memory[*pc] == '\0') {
		return 0;
	}

	two_bytes = (unsigned char*) &instruction;

	for(int i = 0; i < 2; i++) {
		two_bytes[i] = memory[*pc];
        (*pc)++;
	}
	return instruction;
}

void execute(struct chip8* c8, unsigned short instruction)
{
    char jump[] = "this is a jump";
    if (instruction ^ 0x6000) {
        write(STDOUT_FILENO, jump, sizeof(jump));
    }
}
