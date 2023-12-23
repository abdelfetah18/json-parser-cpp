#pragma once

#include "JSONError.h"

template <typename T>
class JSONErrorOr {
   private:
    T m_value;
    JSONError* m_error;
    bool m_is_error = false;

   public:
    JSONErrorOr(T value) : m_value(value), m_is_error(false) {}

    JSONErrorOr(JSONError* error) : m_error(error), m_is_error(true) {}

    bool isError() { return m_is_error; }

    T value() { return m_value; }
    JSONError* error() { return m_error; }

    std::string errorMessage() { return m_error->message(); }
};