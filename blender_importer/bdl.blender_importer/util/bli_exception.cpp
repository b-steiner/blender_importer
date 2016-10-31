#include <bdl.blender_importer/blender_importer.hpp>
#include "bli_exception.hpp"

using namespace bdl::blender_importer;

bli_exception::bli_exception(const std::string& message) : m_message(message)
{ }