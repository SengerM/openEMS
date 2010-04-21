/*
*	Copyright (C) 2010 Thorsten Liebig (Thorsten.Liebig@gmx.de)
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>
#include "operator.h"

class Engine
{
public:
	static Engine* New(const Operator* op);
	virtual ~Engine();

	virtual void Init();
	virtual void Reset();

	//!Iterate a number of timesteps
	virtual bool IterateTS(unsigned int iterTS);

	virtual unsigned int GetNumberOfTimesteps() {return numTS;};

	virtual FDTD_FLOAT**** GetVoltages() {return volt;};
	virtual FDTD_FLOAT**** GetCurrents() {return curr;};

protected:
	Engine(const Operator* op);
	const Operator* Op;

	virtual void UpdateVoltages();
	virtual void ApplyVoltageExcite();
	virtual void UpdateCurrents();
	virtual void ApplyCurrentExcite();

	unsigned int numLines[3];

	FDTD_FLOAT**** volt;
	FDTD_FLOAT**** curr;
	unsigned int numTS;

	ofstream file_et1;
};

#endif // ENGINE_H
