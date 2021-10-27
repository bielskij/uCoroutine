/*
 * internal.h
 *
 *  Created on: 23.10.2021
 *      Author: Jaroslaw Bielski (bielski.j@gmail.com)
 */

#ifndef UCOROUTINE_INTERNAL_H_
#define UCOROUTINE_INTERNAL_H_

#define UCOROUTINE_PC_INIT 0

struct _uCoroutine {
	uCoroutineFunc func;
	void          *funcData;
	uint16_t pc;
};

#endif /* UCOROUTINE_INTERNAL_H_ */
