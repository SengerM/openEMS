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

#include "processfields_td.h"
#include "Common/operator_base.h"
#include <iomanip>
#include <sstream>
#include <string>

ProcessFieldsTD::ProcessFieldsTD(Engine_Interface_Base* eng_if) : ProcessFields(eng_if)
{
	pad_length = 8;
}

ProcessFieldsTD::~ProcessFieldsTD()
{
}

int ProcessFieldsTD::Process()
{
	if (Enabled==false) return -1;
	if (filePattern.empty()) return -1;
	if (CheckTimestep()==false) return GetNextInterval();

	string filename;

	if (m_fileType==VTK_FILETYPE)
	{
		stringstream ss;
		ss << filePattern << std::setw( pad_length ) << std::setfill( '0' ) << m_Eng_Interface->GetNumberOfTimesteps() << ".vtk";
		filename = ss.str();
	}
	else
		filename = m_filename;

#ifdef OUTPUT_IN_DRAWINGUNITS
	double discLines_scaling = 1;
#else
	double discLines_scaling = Op->GetGridDelta();
#endif

	FDTD_FLOAT**** field = CalcField();

	if (m_fileType==VTK_FILETYPE)
	{
		ofstream file(filename.c_str());
		if (file.is_open()==false)
		{
			cerr << "ProcessFieldsTD::Process: can't open file '" << filename << "' for writing... abort! " << endl;
		};
		DumpVectorArray2VTK(file,GetFieldNameByType(m_DumpType),field,discLines,numLines,m_precision,string("Interpolation: ")+m_Eng_Interface->GetInterpolationTypeString(), m_Mesh_Type, discLines_scaling);
		file.close();
	}
	else if (m_fileType==HDF5_FILETYPE)
	{
		stringstream ss;
		ss << std::setw( pad_length ) << std::setfill( '0' ) << m_Eng_Interface->GetNumberOfTimesteps();
		DumpVectorArray2HDF5(filename.c_str(),string( ss.str() ),field,numLines,m_Eng_Interface->GetTime(m_dualTime));
	}
	else
		cerr << "ProcessFieldsTD::Process: unknown File-Type" << endl;

	Delete_N_3DArray(field,numLines);

	return GetNextInterval();
}
