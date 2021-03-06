#ifndef TSS_H
#define TSS_H
typedef struct tss_entry_struct
{
	u32int prev_tss;
	u32int esp0;       // The stack pointer to load when we change to kernel mode.
	u32int ss0;        // The stack segment to load when we change to kernel mode.
	u32int esp1;       // Unused...
	u32int ss1;
	u32int esp2;
	u32int ss2;
	u32int cr3;
	u32int eip;
	u32int eflags;
	u32int eax;
	u32int ecx;
	u32int edx;
	u32int ebx;
	u32int esp;
	u32int ebp;
	u32int esi;
	u32int edi;
	u32int es;         // The value to load into ES when we change to kernel mode.
	u32int cs;         // The value to load into CS when we change to kernel mode.
	u32int ss;         // The value to load into SS when we change to kernel mode.
	u32int ds;         // The value to load into DS when we change to kernel mode.
	u32int fs;         // The value to load into FS when we change to kernel mode.
	u32int gs;         // The value to load into GS when we change to kernel mode.
	u32int ldt;        // Unused...
	u16int trap;
	u16int iomap_base;
} __attribute__((packed)) tss_entry_t;

extern void tss_flush();

#endif
