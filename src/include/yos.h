/*
	 Copyright (c) 2013 Giuseppe Monteleone, All rights reserved.
	 contact info@ing-monteleone.com

	 This file is part of 'YasminOS'

	 GNU Lesser General Public License Usage
	 this file may be used under the terms of the GNU Lesser
	 General Public License version 2.1 as published by the Free Software
	 Foundation and appearing in the file LICENSE.LGPL included in the
	 packaging of this file.  Please review the following information to
	 ensure the GNU Lesser General Public License version 2.1 requirements
	 will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

	 THIS SOFTWARE IS PROVIDED BY GIUSEPPE MONTELEONE ``AS IS'' AND ANY
	 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
	 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GIUSEPPE MONTELEONE BE
	 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
	 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
	 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	 You should have received a copy of the GNU General Public License
	 along with 'YasminOS'. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef YOS_H_
#define YOS_H_

#include <types.h>
#include <cortex_m0.h>
#include <syscall.h>

// this define all memory for a task (context+stack)
#define TASK_SIZE	256

typedef void (*YOS_Routine_t)(void);

// exit irq macro. Set pending bit if return goes back in thread mode
#define EXIT_IRQ()									\
	do {											\
		register DWORD reg;							\
		asm volatile ("mov %0,lr":"=r"(reg));		\
		if ((reg & 4) != 0)							\
			CTX_SCB->ICSR |= CTX_SCBICSR_PendSVSet; \
	} while(0)

typedef struct YOS_Task_s YOS_Task_t;

struct YOS_Task_s {
	YOS_Task_t *tNext;	// must be first member of structure
	DWORD	tPsp:31;
	DWORD	tWaiting:1;
};

void YOS_InitOs(void *taskMemory, void *taskTopMemory);
void YOS_Start(void);
YOS_Task_t *YOS_AddTask(YOS_Routine_t,int);

#endif /* YOS_H_ */
