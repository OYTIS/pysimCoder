/*
COPYRIGHT (C) 2016  Roberto Bucher (roberto.bucher@supsi.ch)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#include <pyblock.h>

#define TIMEOUT -1

#include <canopen.h>
#include <stdlib.h>
#include <unistd.h>

static uint8_t pos_mode[8]    = {0x2f, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00};
static uint8_t write_value[8] = {0x23, 0x7a, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t write_init[8]  = {0x2b, 0x40, 0x60, 0x00, 0x0f, 0x00, 0x00, 0x00};
static uint8_t write_exec[8]  = {0x2b, 0x40, 0x60, 0x00, 0x3f, 0x00, 0x00, 0x00};

static void init(python_block *block)
{
   if(canOpenTH(block->str)) exit(1);

  sendMsg(0x600+block->intPar[0],pos_mode,8); 
  usleep(50000);
}

static void inout(python_block *block)
{
  int *U_can;

  uint8_t DATA[8];
  double *u = block->u[0];
  unsigned short *index;
  
  U_can = (int *) &write_value[4];
  *U_can = (int) (u[0]);

  sendMsg(0x600+block->intPar[0],write_value,8);
  sendMsg(0x600+block->intPar[0],write_init,8);
  sendMsg(0x600+block->intPar[0],write_exec,8);
}

static void end(python_block *block)
{
  canClose();
}

void FH_3XXX_X(int Flag, python_block *block)
{
  if (Flag==CG_OUT){          /* set output */
    inout(block);
  }
  else if (Flag==CG_END){     /* termination */ 
    end(block);
  }
  else if (Flag ==CG_INIT){    /* initialisation */
    init(block);
  }
}


