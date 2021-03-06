#include <arch/i386/idt.h>

#include <string.h>

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

void init_idt(){
	idt_ptr.limit=sizeof(idt_entry_t)*256-1;
	idt_ptr.base=(uint32_t)&idt_entries;
	memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
					//10001110
	idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8e);

	idt_flush((uint32_t)&idt_ptr);
}

void idt_set_gate(uint8_t index, uint32_t base, uint16_t selector,
			uint8_t flags){
	idt_entries[index].base_low	=base&0xffff;
	idt_entries[index].base_high	=(base>>16)&0xffff;

	idt_entries[index].selector	=selector;

	idt_entries[index].flags	=flags;

	idt_entries[index].zero		=0;
}

void set_idt_reg(idt_entry_t *addr, uint16_t limit){

	idt_ptr_t idt_reg;
	idt_reg.limit=limit*8;
	idt_reg.base=(unsigned long) addr;
	__asm__ __volatile__("lidt %0": :"g" (idt_reg));
}
