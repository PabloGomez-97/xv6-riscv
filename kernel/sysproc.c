#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
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
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
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


uint64
sys_getppid(void){
  struct proc *p=myproc();
  return p->parent ? p->parent->pid : 0; //Devuelve el PID del padre o 0 si no tiene padre
}

uint64
sys_getancestor(void) {
  int n;

  argint(0, &n);

  struct proc *p = myproc();
  for (int i = 0; i < n; i++) {
    if (p->parent == 0)

    p = p->parent;
  }
  return p->pid;
}

extern int set_priority(int pid, int priority);
extern int set_boost(int pid, int boost);

// Declaramos las funciones de mprotect y munprotect
int mprotect(pagetable_t pagetable, void *addr, int len);
int munprotect(pagetable_t pagetable, void *addr, int len);

uint64 sys_mprotect(void) {
    uint64 addr;
    int len;

    // Llama a argaddr y argint sin verificar un valor de retorno.
    //  se asume que si algo sale mal con estos, habrá un efecto visible en la ejecución por como esta seteado xv6.
    argaddr(0, (uint64*)&addr);
    argint(1, &len);

    // Validación manual: si addr o len no son válidos, retorna -1.
    if (addr == 0 || len <= 0)
        return -1;

    return mprotect(myproc()->pagetable, (void *)addr, len);
}

uint64 sys_munprotect(void) {
    uint64 addr;
    int len;

    argaddr(0, (uint64*)&addr);
    argint(1, &len);

    if (addr == 0 || len <= 0)
        return -1;

    return munprotect(myproc()->pagetable, (void *)addr, len);
}

uint64
sys_chmod(void) {
    char path[MAXPATH];
    int mode;

    // Usa un búfer de tamaño fijo en lugar de un puntero doble
    if (argstr(0, path, MAXPATH) < 0 || argint(1, &mode) < 0) {
        return -1; // Error al obtener argumentos
    }

    struct inode *ip = namei(path); // Encuentra el inodo del archivo
    if (!ip) return -1;

    begin_op();
    ilock(ip);

    if (ip->perm == 5) { // Si el archivo es inmutable
        iunlockput(ip);
        end_op();
        return -1; // No se puede cambiar permisos de un archivo inmutable
    }

    ip->perm = mode; // Cambia los permisos
    iupdate(ip);
    iunlockput(ip);
    end_op();

    return 0; // Operación exitosa
}