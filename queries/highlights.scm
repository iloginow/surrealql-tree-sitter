[
  (keyword_computed)
  (keyword_reference)
  (keyword_end)
] @keyword

[
  (keyword_none)
] @constant.builtin

(reference_on_delete_clause
  (keyword_on) @constant.builtin
  (keyword_delete) @constant.builtin)

(reference_on_delete_clause
  [
    (keyword_ignore)
    (keyword_unset)
    (keyword_cascade)
    (keyword_reject)
  ] @keyword)

(reference_on_delete_clause
  (keyword_then) @keyword.control)

(assert_clause
  (keyword_assert) @keyword)

(function_call
  (builtin_function_name) @function.builtin)

(function_call
  (custom_function_name) @function)

(function_call
  (function_name) @function)

(variable_name) @variable.parameter
