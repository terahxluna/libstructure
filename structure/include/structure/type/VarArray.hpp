/*
 * Copyright (c) 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Intel Corporation nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "structure/structure_export.h"

#include "structure/type/Block.hpp"
#include "structure/detail/disjunction.hpp"

#include <utility>

namespace structure
{
/** Represents a Variable-Length Array (aka VLA) */
class STRUCTURE_EXPORT VarArray : public Block
{
public:
    /** Constructs a Variable-Length Array of any kind of field.
     *
     * @param[in] itemType The structure that is repeated in the array.
     * @param[in] args Attributes of the VarArray
     */
    template <class ItemType, typename... Args>
    VarArray(const std::string &name, ItemType &&itemType, Args &&... args)
        : Block(name, std::forward<ItemType>(itemType), std::forward<Args>(args)...)
    {
        static_assert(is_structure<ItemType>::value,
                      "The ItemType in a VarArray must be a Structure");
        static_assert(not disjunction<is_structure<Args>...>::value,
                      "The arguments after ItemType must not be Structures");
    }

    std::string getTypeName() const override;

private:
    std::unique_ptr<StructureValue> doBuild(ValueImporter &importer,
                                            const std::string &path) const override;
};
} // namespace structure
