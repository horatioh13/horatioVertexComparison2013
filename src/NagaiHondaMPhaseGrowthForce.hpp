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

#ifndef NAGAIHONDAMPHASEGROWTHFORCE_HPP_
#define NAGAIHONDAMPHASEGROWTHFORCE_HPP_

#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

#include "NagaiHondaForce.hpp"
#include "VertexBasedCellPopulation.hpp"

#include <iostream>

/**
 * A force class for use in vertex-based simulations, based on a mechanical
 * model proposed by T. Nagai and H. Honda ("A dynamic cell model for the formation
 * of epithelial tissues", Philosophical Magazine Part B 81:699-719).
 *
 * Each of the model parameter member variables are rescaled such that mDampingConstantNormal
 * takes the default value 1, whereas Nagai and Honda (who denote the parameter by
 * nu) take the value 0.01.
 */
template<unsigned DIM>
class NagaiHondaMPhaseGrowthForce  : public NagaiHondaForce<DIM>
{
friend class TestForces;

private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<NagaiHondaForce<DIM> >(*this);
    }

public:

    /**
     * Constructor.
     */
    NagaiHondaMPhaseGrowthForce();

    /**
     * Destructor.
     */
    ~NagaiHondaMPhaseGrowthForce();

    /**
     * Overridden GetTargetAreaOfCell method which has cells growing over the M phase
     * duration only.
     *
     * Get the target area of a given cell. This grows linearly from
     * 0.5*A to A during the M phase of the cell cycle, then remains
     * at A for the rest of the cell cycle, where A denotes the
     * member variable mMatureCellTargetArea.
     *
     * @param pCell the cell
     * @return the cell's target area
     */
    double GetTargetAreaOfCell(const CellPtr pCell) const;

    /**
     * Overridden OutputForceParameters() method.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    void OutputForceParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(NagaiHondaMPhaseGrowthForce)

#endif /*NAGAIHONDAMPHASEGROWTHFORCE_HPP_*/