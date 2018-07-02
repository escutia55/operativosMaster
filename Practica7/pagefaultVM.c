#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mmu.h"

#define NUMPROCS 4
#define PAGES 6
#define PAGESIZE 4096
#define PHISICALMEMORY 48*1024 
#define TOTFRAMES PHISICALMEMORY/PAGESIZE
#define RESIDENTSETSIZE PHISICALMEMORY/(PAGESIZE*NUMPROCS)

extern char *base;
extern int framesbegin;
extern int idproc;
extern int systemframetablesize;
extern int ptlr;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE *ptbr;


int getfreeframe();
int searchvirtualframe();
int getfifo();
int get_OldPage();
int getfreeframeVM();

int pagefault(char *vaddress)
{
    int i;
    int frame,vframe; 
    long pag_a_expulsar;
    int fd;
    char buffer[PAGESIZE];
    int pag_del_proceso;
    long OldPage;
    int vmframe;

    // A partir de la dirección que provocó el fallo, calculamos la página
    pag_del_proceso=(long) vaddress>>12;


    // Si la página del proceso está en un marco virtual del disco
    vmframe = getfreeframeVM();
    if(-1 != vmframe)
    {
		// Lee el marco virtual al buffer
        readblock(buffer, vmframe);
        // Libera el frame virtual
        systemframetable[vmframe].assigned=1;

    }


    // Cuenta los marcos asignados al proceso
    i=countframesassigned();

    // Si ya ocupó todos sus marcos, expulsa una página
    if(i>=RESIDENTSETSIZE)
    {
		// Buscar una página a expulsar
        OldPage = get_OldPage();
		
		// Poner el bit de presente en 0 en la tabla de páginas
        (ptbr+OldPage)->presente = 0;

        // Si la página ya fue modificada, grábala en disco
        if((ptbr+OldPage)->modificado == 1)
        {
			// Escribe el frame de la página en el archivo de respaldo y pon en 0 el bit de modificado
           saveframe((ptbr+OldPage)->framenumber);
            (ptbr+OldPage)->modificado = 0;                                
        }
		
        // Busca un frame virtual en memoria secundaria
		// Si no hay frames virtuales en memoria secundaria regresa error
		{
            return(-1);
        }
        // Copia el frame a memoria secundaria, actualiza la tabla de páginas y libera el marco de la memoria principal
    }

    // Busca un marco físico libre en el sistema
	// Si no hay marcos físicos libres en el sistema regresa error
    {
        return(-1); // Regresar indicando error de memoria insuficiente
    }

    // Si la página estaba en memoria secundaria
    {
        // Cópialo al frame libre encontrado en memoria principal y transfiérelo a la memoria física
    }
   
	// Poner el bit de presente en 1 en la tabla de páginas y el frame 


    return(1); // Regresar todo bien
}


int get_OldPage(){
  unsigned long tarrivedOld = ptbr->tarrived;
  int oldPage = 0;

    for (int i = 0; i < PAGES; i++) { 
      if(((ptbr+i)->tarrived  < tarrivedOld) && ((ptbr+i)->presente == 1) && ((ptbr+i)->tarrived != 0)){ 
        tarrivedOld = (ptbr+i)->tarrived; //pagina mas vieja que este presente
        oldPage = i;
      }
      else
        tarrivedOld = ptbr->tarrived;
    }

  return oldPage; 
}

int getfreeframeVM()
{
    int i;
    // Busca un marco libre en el sistema
    for(i=framesbegin +12;i<systemframetablesize*2+framesbegin;i++)
        if(!systemframetable[i].assigned)
        {
            systemframetable[i].assigned=1;
            break;
        }
    if(i<systemframetablesize+framesbegin)
        systemframetable[i].assigned=1;
    else
        i=-1;
    return(i);
}


