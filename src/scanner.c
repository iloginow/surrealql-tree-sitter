#include "tree_sitter/parser.h"
#include <stdbool.h>
#include <stdint.h>

enum TokenType {
    JS_FUNCTION_BODY,
};

void *tree_sitter_surrealql_external_scanner_create() { return NULL; }
void tree_sitter_surrealql_external_scanner_destroy(void *payload) { (void)payload; }
void tree_sitter_surrealql_external_scanner_reset(void *payload) { (void)payload; }
unsigned tree_sitter_surrealql_external_scanner_serialize(void *payload, char *buffer) {
    (void)payload; (void)buffer;
    return 0;
}
void tree_sitter_surrealql_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
    (void)payload; (void)buffer; (void)length;
}

static void advance(TSLexer *lexer) {
    lexer->advance(lexer, false);
}

static void skip(TSLexer *lexer) {
    lexer->advance(lexer, true);
}

// Scan a JS single-line string delimited by `quote`. Handles \-escapes.
static void scan_js_string(TSLexer *lexer, int32_t quote) {
    while (lexer->lookahead != 0 && lexer->lookahead != '\n') {
        if (lexer->lookahead == '\\') {
            advance(lexer);
            if (lexer->lookahead != 0) advance(lexer);
        } else if (lexer->lookahead == quote) {
            advance(lexer);
            return;
        } else {
            advance(lexer);
        }
    }
}

// Scan a JS template literal. Handles \-escapes and nested ${...} correctly.
static void scan_template_literal(TSLexer *lexer) {
    while (lexer->lookahead != 0) {
        if (lexer->lookahead == '\\') {
            advance(lexer);
            if (lexer->lookahead != 0) advance(lexer);
        } else if (lexer->lookahead == '`') {
            advance(lexer);
            return;
        } else if (lexer->lookahead == '$') {
            advance(lexer);
            if (lexer->lookahead == '{') {
                // Nested expression inside template literal — track brace depth
                advance(lexer);
                int depth = 1;
                while (lexer->lookahead != 0 && depth > 0) {
                    int32_t c = lexer->lookahead;
                    advance(lexer);
                    if (c == '{') {
                        depth++;
                    } else if (c == '}') {
                        depth--;
                    } else if (c == '\'' || c == '"') {
                        scan_js_string(lexer, c);
                    } else if (c == '`') {
                        scan_template_literal(lexer);
                    }
                }
            }
        } else {
            advance(lexer);
        }
    }
}

bool tree_sitter_surrealql_external_scanner_scan(
    void *payload,
    TSLexer *lexer,
    const bool *valid_symbols
) {
    (void)payload;

    if (!valid_symbols[JS_FUNCTION_BODY]) return false;

    // Skip leading whitespace
    while (lexer->lookahead == ' ' || lexer->lookahead == '\t' ||
           lexer->lookahead == '\n' || lexer->lookahead == '\r') {
        skip(lexer);
    }

    if (lexer->lookahead != '{') return false;
    advance(lexer); // consume opening '{'

    int depth = 1;

    while (depth > 0 && lexer->lookahead != 0) {
        int32_t c = lexer->lookahead;
        advance(lexer);

        if (c == '{') {
            depth++;
        } else if (c == '}') {
            depth--;
        } else if (c == '\'') {
            scan_js_string(lexer, '\'');
        } else if (c == '"') {
            scan_js_string(lexer, '"');
        } else if (c == '`') {
            scan_template_literal(lexer);
        } else if (c == '/') {
            if (lexer->lookahead == '/') {
                // Single-line comment — consume to end of line
                advance(lexer);
                while (lexer->lookahead != 0 && lexer->lookahead != '\n') {
                    advance(lexer);
                }
            } else if (lexer->lookahead == '*') {
                // Block comment — consume until */
                advance(lexer); // consume '*'
                while (lexer->lookahead != 0) {
                    if (lexer->lookahead == '*') {
                        advance(lexer);
                        if (lexer->lookahead == '/') {
                            advance(lexer);
                            break;
                        }
                    } else {
                        advance(lexer);
                    }
                }
            }
            // Division operator — already advanced past '/', nothing else to do
        }
    }

    lexer->result_symbol = JS_FUNCTION_BODY;
    return true;
}
