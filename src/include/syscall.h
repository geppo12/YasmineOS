/*
	 Copyright (c) 2013 Giuseppe Monteleone, All rights reserved.
	 contact info@ing-monteleone.com

	 This file is part of 'Yasmin OS'

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
	 along with 'Yasmine OS'. If not, see <http://www.gnu.org/licenses/>.

	 SYSCALL EXCEPTION

	 As special exception, if your code use YasminOS as RTOS, you
	 may redistribute all object code that include this include file
	 and these system call under terms of your choice
*/

#ifndef SYSCALL_H_
#define SYSCALL_H_

// system call IDs
enum {
	DO_WAIT = 1,		// value 0 is reserved for start
	DO_SIGNAL,
	DO_RESCHEDULE,
	DO_SIGNAL_EVENT,
	DO_WAIT_EVENT,
	DO_RESUME_EVENT,	// called before wait exit
	DO_QUEUE_MUTEX,
	DO_UNQUEUE_MUTEX,
	DO_TRY_MUTEX
};

// SYSCALL are like function AAPCS use R0-R3 as scratch register so sw need to inform compiler

// System call macro without any parameter
#define SYS_CALL0(a)	do { asm volatile ("svc %0"::"I"(DO_##a): "r0","r1","r2","r3"); } while(0)

// System call macro with one parameter
#define SYS_CALL1(a,b)					\
	do { 								\
		register DWORD r = (DWORD)b;	\
		asm volatile (					\
			"mov r0,%[par1]	\n"			\
			"svc %[call]	\n"			\
			::[par1]"r"(r), [call]"I"(DO_##a) \
			: "r0","r1","r2","r3" 		\
		);								\
	} while(0)

// System call macro with two parameter
#define SYS_CALL2(a,p1,p2)				\
	do { 								\
		register DWORD r1 = (DWORD)p1;	\
		register DWORD r2 = (DWORD)p2; 	\
		asm volatile (					\
			"mov r0,%[par1]	\n"			\
			"mov r1,%[par2]	\n" 		\
			"svc %[call]	\n"			\
			::[par1]"r"(r1), [par2]"r"(r2), [call]"I"(DO_##a) \
			: "r0","r1","r2","r3" 		\
		);								\
	} while(0)

// efficent coding of SIGNAL system call
#define SIGNAL(task,signal)				\
	do { 								\
		asm volatile (					\
			"ldr r0,%[par1]	\n"			\
			"mov r1,%[par2]	\n" 		\
			"svc %[call]	\n"			\
			::[par1]"m"(task), [par2]"I"(signal), [call]"I"(DO_SIGNAL) \
			: "r0","r1","r2","r3" 		\
		);								\
	} while(0)

// alias for WAIT system call
#define WAIT()	SYS_CALL0(WAIT)


#endif /* SYSCALL_H_ */
