[
  (keyword_computed)
  (keyword_reference)
  (keyword_end)
  (keyword_async)
  (keyword_all)
  (keyword_batch)
  (keyword_expired)
  (keyword_purge)
  (keyword_revoke)
  (keyword_revoked)
  (keyword_sequence)
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

(scripting_function
  (keyword_function) @keyword.function)

(scripting_function
  (keyword_async) @keyword.coroutine)

(scripting_function
  (js_function_body) @embedded)

(access_statement
  [
    (keyword_grant)
    (keyword_show)
    (keyword_revoke)
    (keyword_purge)
  ] @keyword)

(define_sequence_statement
  (keyword_sequence) @keyword)

(variable_name) @variable.parameter
