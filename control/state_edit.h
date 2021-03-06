/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <map>
#include <set>

#include "context.h"
#include "event.h"
#include "index_info.h"


class state_edit {
private:
    enum class column_element {
        number = 0,
        name,
        address,
        type,
        multiplier,
        value,
        last_element
    };

    using column_position_map = std::map<column_element, std::size_t>;

    using column_name_map = std::map<column_element, std::string>;

    using column_names = std::vector<std::string>;

    using optional_columns = std::set<column_element>;

private:
    static constexpr char INVALID_VALUE[] = "########";

    static const column_name_map COLUMN_NAME_MAP;

    static const optional_columns OPTIONAL_COLUMNS;

public:
    event operator()(context& p_context) const;

private:
    static void show_table(context& p_context);

    static optional_columns get_table_optional_columns(const context& p_context);

    static column_position_map get_column_position_map(const context& p_context);

    static column_names get_column_names(const column_position_map& p_position);

    static std::string get_column_name(const column_element p_element);

    static index_info::user_instruction get_index_user_instruction(const context& p_context);

    static std::string get_single_value_from_stdin();

    static event ask_next_action(context& p_context);

    static void handle_set_event(context& p_context);

    static void handle_remove_event(context& p_context);

    static void handle_save_event(const context& p_context);

    static void handle_load_event(context& p_context);

    static void handle_revert_event(context& p_context);

    static void handle_rename_event(context& p_context);

    static void handle_set_multiplier(context& p_context);

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_edit& p_state);
};