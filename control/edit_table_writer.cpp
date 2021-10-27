/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "edit_table_writer.h"

#include <fstream>

#include "log/logging.h"

#include "serializer.h"


edit_table_writer::edit_table_writer(const std::string& p_filename) :
    m_filename(p_filename)
{ }


bool edit_table_writer::write(const edit_table& p_table) const {
    LOG_INFO("Write edit table to file '" << m_filename << "'.")

    try {
        std::ofstream stream(m_filename);
        stream << serializer::get(p_table).dump();
        stream.close();
    }
    catch (std::exception& p_exception) {
        LOG_ERROR("Impossible edit table to write file '" << m_filename << "' (reason: '" << p_exception.what() << "').")
        return false;
    }

    return true;
}
