#ifndef ELF_H
#define ELF_H
#include <mod.h>
#include <block.h>
#define MAX_SYMS 512
extern void * kernel_start;

typedef struct { 
	const char *name; 
	intptr_t ptr; 
	int flag;
} kernel_symbol_t;


typedef struct __attribute__((packed))
{
	uint8_t  id[16];
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint32_t entry;
	uint32_t phoff;
	uint32_t shoff;
	uint32_t flags;
	uint16_t size;
	uint16_t phsize;
	uint16_t phnum;
	uint16_t shsize;
	uint16_t shnum;
	uint16_t strndx;
	char *shbuf;
	unsigned strtab_addr, symtab_addr, strtabsz, syment_len;
} elf_header_t;

typedef struct __attribute__((packed))
{
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t address;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t alignment;
	uint32_t sect_size;
} elf32_section_header_t;

typedef struct __attribute__((packed))
{
	int   offset;
	uint32_t  info;
} elf32_reloc_entry_t;

typedef struct __attribute__((packed)) {
	uint32_t r_offset;
	uint32_t r_info;
} elf32_rel_t;

typedef struct __attribute__((packed))
{
	uint32_t name;
	uint32_t address;
	uint32_t size;
	uint8_t  info;
	uint8_t  other;
	uint16_t shndx;
} elf32_symtab_entry_t;

typedef struct __attribute__((packed))
{
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_addr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} elf32_program_header_t;

static inline int is_valid_elf32(char *buf, short type)
{
	elf_header_t * eh;
	eh = (elf_header_t*)buf;
	if(memcmp(eh->id + 1, (uint8_t*)"ELF", 3)
		|| eh->machine != 0x03
		|| eh->type != type)
		return 0;
	return 1;
}

#define PH_LOAD    1
#define PH_DYNAMIC 2
#define PH_INTERP  3


#define EDT_NEEDED   1
#define EDT_PLTRELSZ 2
#define EDT_STRTAB   5
#define EDT_SYMTAB   6

#define EDT_STRSZ   10
#define EDT_SYMENT  11

#define EDT_REL     17
#define EDT_RELSZ   18
#define EDT_RELENT  19
#define EDT_PLTREL  20

#define EDT_TEXTREL 22
#define EDT_JMPREL  23

#define SHN_COMMON 0xFF02
#define SHN_ABS    0xFF01


#define ELF_R_NONE     0
#define ELF_R_32       1
#define ELF_R_PC32     2
#define ELF_R_GOT32    3
#define ELF_R_PLT32    4
#define ELF_R_COPY     5
#define ELF_R_GLOBDAT  6
#define ELF_R_JMPSLOT  7
#define ELF_R_RELATIVE 8
#define ELF_R_GOTOFF   9
#define ELF_R_GOTPC   10

#define ELF_R_SYM(i)  ((i)>>8)
#define ELF_R_TYPE(i) ((unsigned char)(i))
#define ELF32_ST_TYPE(i) ((i)&0xf)
typedef struct {
	uint16_t d_tag;
	union {
		uint32_t d_val;
		uint32_t d_ptr;
	} d_un;
} elf32_dyn_t;

typedef struct
{
  elf32_symtab_entry_t *symtab;
  uint32_t      symtabsz;
  const char   *strtab;
  uint32_t      strtabsz;
  unsigned lookable;
} elf32_t;

static inline int read_data(int fp, char *buf, unsigned off, int length)
{
	return do_sys_read_flags(get_file_pointer((task_t *)current_task, fp), off, buf, length);
}

#define GET_RELOC_SYM(i)  ((i)>>8)
#define GET_RELOC_TYPE(i) ((unsigned char)(i))

#define GET_SYMTAB_BIND(i)   ((i)>>4)
#define GET_SYMTAB_TYPE(i)   ((i)&0xf)

#define SHN_UNDEF   0
void _add_kernel_sym_user(const intptr_t func, const char * funcstr);
void _add_kernel_symbol(const intptr_t func, const char * funcstr);
intptr_t find_kernel_function_user(char * unres);
intptr_t find_kernel_function(char * unres);
void init_kernel_symbols(void);
int parse_elf_module(module_t *mod, uint8_t * buf, char *name, int);
const char *elf_lookup_symbol (uint32_t addr, elf32_t *elf);
elf32_t parse_kernel_elf(struct multiboot *mb, elf32_t *);
extern elf32_t kernel_elf;
const char *elf_lookup_symbol (uint32_t addr, elf32_t *elf);
char *get_symbol_string(uint8_t *buf, uint32_t index);
void _add_kernel_symbol(const intptr_t func, const char * funcstr);
intptr_t find_kernel_function(char * unres);
int remove_kernel_symbol(char * unres);
elf32_symtab_entry_t * fill_symbol_struct(uint8_t * buf, uint32_t symbol);
intptr_t get_section_offset(uint8_t * buf, uint32_t info);
int process_elf(char *mem, int fp, unsigned *start, unsigned *end);
unsigned long long get_epoch_time();
void remove_dfs_node(char *name);
int kernel_cache_sync_slow(int all);
int ttyx_ioctl(int min, int cmd, int arg);
int sys_setsid();
int proc_set_callback(int major, int( *callback)(char rw, struct inode *inode, 
	int m, char *buf));
int proc_get_major();
void delay_sleep(int t);
#endif
