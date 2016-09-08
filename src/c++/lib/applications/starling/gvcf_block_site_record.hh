// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Strelka - Small Variant Caller
// Copyright (c) 2009-2016 Illumina, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//

///
/// \author Chris Saunders
///

#pragma once


#include "gvcf_locus_info.hh"
#include "gvcf_options.hh"
#include "blt_util/stream_stat.hh"


/// manages compressed site record blocks output in the gVCF
///
struct gvcf_block_site_record : public GermlineSiteLocusInfo
{
    typedef GermlineSiteLocusInfo base_t;

    explicit
    gvcf_block_site_record(
        const gvcf_options& opt)
        : base_t(1),
          frac_tol(static_cast<double>(opt.block_percent_tol)/100.)
        , abs_tol(opt.block_abs_tol)
    {
        reset();
    }

    void
    reset()
    {
        base_t::clear();
        count=0;
        block_gqx.reset();
        block_dpu.reset();
        block_dpf.reset();
        pos=-1;
        isBlockGqxDefined = _isNonRef=false;
    }

    /// determine if the given site could be joined to this block:
    bool
    testCanSiteJoinSampleBlock(
        const GermlineDiploidSiteLocusInfo& locus,
        const unsigned sampleIndex) const;

    /// determine if the given site could be joined to this block:
    bool
    testCanSiteJoinSampleBlock(
        const GermlineContinuousSiteLocusInfo& locus,
        const unsigned samleIndex) const;

    /// add site to the current block
    void
    joinSiteToSampleBlock(
        const GermlineSiteLocusInfo& locus,
        const unsigned sampleIndex);

    bool is_nonref(const unsigned /*sampleIndex*/) const override
    {
        return _isNonRef;
    }

private:

    /// reduce diploid/continuous site logical duplication by putting common tests here
    ///
    /// \return false if block cannot be shared, true otherwise (not enough by itself to conclude sharable)
    bool
    testCanSiteJoinSampleBlockShared(
        const GermlineSiteLocusInfo& locus,
        const unsigned sampleIndex) const;

    void
    setNonRef(const bool isNonRef)
    {
        _isNonRef = isNonRef;
    }

public:
    const double frac_tol;
    const int abs_tol;
    int count;
    stream_stat block_gqx;
    stream_stat block_dpu;
    stream_stat block_dpf;

    bool isBlockGqxDefined;
    //stream_stat _blockMQ;

private:
    bool _isNonRef;
};
