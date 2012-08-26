/*
 * Enums.h
 *
 *  Created on: Aug 24, 2012
 *      Author: mauron
 */

#ifndef ENUMS_H_
#define ENUMS_H_

// enumeration for the state of the benchmark
enum
{
	Running,
	Stopped
}MandelbrotState;

enum
{
	true,
	false
}FirstPass;

enum
{
	DSP,
	CPU
}Platform;

enum
{
	True,
	False
}DSPCanRun;

enum
{
	yes,
	no
}CreateLogFile;

enum
{
	zero,
	nonZero
}ElementValue;


#endif /* ENUMS_H_ */
