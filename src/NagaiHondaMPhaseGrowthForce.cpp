/*

Copyright (c) 2005-2013, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "NagaiHondaMPhaseGrowthForce.hpp"

template<unsigned DIM>
NagaiHondaMPhaseGrowthForce<DIM>::NagaiHondaMPhaseGrowthForce()
   : NagaiHondaForce<DIM>()
{
}

template<unsigned DIM>
NagaiHondaMPhaseGrowthForce<DIM>::~NagaiHondaMPhaseGrowthForce()
{
}


template<unsigned DIM>
double NagaiHondaMPhaseGrowthForce<DIM>::GetTargetAreaOfCell(const CellPtr pCell) const
{
    // Get target area A of a healthy cell in S, G2 or M phase
    double cell_target_area = this->GetMatureCellTargetArea();

    double cell_age = pCell->GetAge();
    double m_duration = pCell->GetCellCycleModel()->GetMDuration();

    if (pCell->HasCellProperty<ApoptoticCellProperty>())
    {
        // Age of cell when apoptosis begins
        if (pCell->GetStartOfApoptosisTime() - pCell->GetBirthTime() < m_duration)
        {
            cell_target_area *= 0.5*(1 + (pCell->GetStartOfApoptosisTime() - pCell->GetBirthTime())/m_duration);
        }

        // The target area of an apoptotic cell decreases linearly to zero (and past it negative)
        cell_target_area = cell_target_area - 0.5*cell_target_area/(pCell->GetApoptosisTime())*(SimulationTime::Instance()->GetTime()-pCell->GetStartOfApoptosisTime());

        // Don't allow a negative target area
        if (cell_target_area < 0)
        {
            cell_target_area = 0;
        }
    }
    else
    {
        // The target area of a proliferating cell increases linearly from A/2 to A over the course of the M phase
        if (cell_age < m_duration)
        {
            cell_target_area *= 0.5*(1 + cell_age/m_duration);
        }
    }

    return cell_target_area;
}

template<unsigned DIM>
void NagaiHondaMPhaseGrowthForce<DIM>::OutputForceParameters(out_stream& rParamsFile)
{
    // Call method on direct parent class
    NagaiHondaForce<DIM>::OutputForceParameters(rParamsFile);
}


/////////////////////////////////////////////////////////////////////////////
// Explicit instantiation
/////////////////////////////////////////////////////////////////////////////

template class NagaiHondaMPhaseGrowthForce<1>;
template class NagaiHondaMPhaseGrowthForce<2>;
template class NagaiHondaMPhaseGrowthForce<3>;


// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(NagaiHondaMPhaseGrowthForce)
