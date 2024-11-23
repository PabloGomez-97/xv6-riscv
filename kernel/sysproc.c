#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_getppid(void)
{
  return myproc()->parent->pid;
}

uint64
sys_getancestor(void)
{
  int n;
  struct proc *p = myproc();

  int argResult;
  argint(0, &n);
  argResult = n;
  if (argResult < 0) // Obtener el parámetro n
    return -1;

  for (int i = 0; i < n; i++)
  {
    if (p->parent == 0) // No hay más ancestros
      return -1;
    p = p->parent; // Avanza al siguiente ancestro
  }

  return p->pid;
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

static int
validate_addr_len(void *addr, int len, struct proc *p)
{
  uint64 va = (uint64)addr;

  // Validar que la dirección esté alineada a página
  if (va % PGSIZE != 0)
    return -1;

  // Validar que la longitud sea positiva
  if (len <= 0)
    return -1;

  // Verificar que el rango de memoria pertenece al proceso
  uint64 end_va = va + (len * PGSIZE);

  if (va >= MAXVA || end_va > MAXVA || end_va < va)
    return -1;

  // Verificar que las páginas están mapeadas
  for (uint64 a = va; a < end_va; a += PGSIZE)
  {
    pte_t *pte = walk(p->pagetable, a, 0);
    if (pte == 0 || (*pte & PTE_V) == 0)
      return -1;
  }

  return 0;
}

uint64
sys_mprotect(void)
{
  uint64 addr; // Cambiado de void* a uint64
  int len;

  if (argaddr(0, &addr) < 0 || argint(1, &len) < 0) // Usando uint64 en lugar de void*
    return -1;

  // Validar dirección y longitud
  if (validate_addr_len((void *)addr, len, myproc()) < 0) // Casting addr a void*
    return -1;

  // Recorrer las páginas y modificar los permisos
  uint64 a = addr;
  for (int i = 0; i < len; i++)
  {
    pte_t *pte = walk(myproc()->pagetable, a + i * PGSIZE, 0);
    if (pte == 0 || (*pte & PTE_V) == 0)
      return -1;
    *pte &= ~PTE_W; // Desactivar bit de escritura
  }

  // Flush TLB para que los cambios surtan efecto
  sfence_vma();
  return 0;
}

uint64
sys_munprotect(void)
{
  uint64 addr; // Cambiado de void* a uint64
  int len;

  if (argaddr(0, &addr) < 0 || argint(1, &len) < 0) // Usando uint64 en lugar de void*
    return -1;

  // Validar dirección y longitud
  if (validate_addr_len((void *)addr, len, myproc()) < 0) // Casting addr a void*
    return -1;

  // Recorrer las páginas y modificar los permisos
  uint64 a = addr;
  for (int i = 0; i < len; i++)
  {
    pte_t *pte = walk(myproc()->pagetable, a + i * PGSIZE, 0);
    if (pte == 0 || (*pte & PTE_V) == 0)
      return -1;
    *pte |= PTE_W; // Activar bit de escritura
  }

  // Flush TLB para que los cambios se guarden
  sfence_vma();
  return 0;
}