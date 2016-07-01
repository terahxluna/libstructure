/*
 * Copyright (c) 2016, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "value/GenericFieldValue.hpp"
#include "Visitor.hpp"

#include <initializer_list>
#include <string>
#include <iostream>
#include <type_traits>
#include <stdexcept>

namespace structure
{
/** An instantiated Field
 *
 * @tparam FieldType The type of the field with which this value is instantiated.
 */
template <typename FieldType>
class FieldValue : public GenericFieldValue
{
    // This prevents hard-to-understand compilation errors.
    static_assert(std::is_base_of<GenericField, FieldType>::value,
                  "The FieldType of a FieldValue must be a GenericField.");

public:
    /** Constructs a value from a Field and typed value */
    FieldValue(FieldType field, typename FieldType::Storage value)
        : mStructure(field), mValue(value)
    {
        if (not field.isAllowed(value)) {
            throw std::runtime_error("Illegal value");
        }
    }

    /** Constructs a value from a Field and string */
    FieldValue(FieldType field, const std::string &value)
        : FieldValue(field, field.fromString(value))
    {
    }

    std::string getValue() const override { return std::to_string(mValue); }
    void setValue(std::string value) { mValue = mStructure.fromString(value); }

    typename FieldType::Storage getTypedValue() const { return mValue; }
    void setTypedValue(typename FieldType::Storage value) { mValue = value; }

    /** @returns the Field type corresponding to this value
     *
     * Same as getStructure() but with a stronger type.
     */
    const FieldType &getField() const { return mStructure; }
    const Structure &getStructure() const override { return mStructure; }

    void accept(ValueVisitor &visitor) const override { visitor.visit(*this); }
    void accept(StorageVisitor &visitor) const override { visitor.visitStorage(mValue); }

private:
    const FieldType mStructure;
    typename FieldType::Storage mValue;
};
}
