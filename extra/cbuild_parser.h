#ifndef CBUILD_EXTRA_PARSER_H
#define CBUILD_EXTRA_PARSER_H

#define CB_TOKEN_LIST(X)                  \
  X(CB_Token_Symbol_Invalid)              \
  X(CB_Token_Symbol_Eof)                  \
  X(CB_Token_Symbol_Semicolon)            \
  X(CB_Token_Symbol_Comma)                \
  X(CB_Token_Symbol_Star)                 \
  X(CB_Token_Symbol_Paren_Round_Open)     \
  X(CB_Token_Symbol_Paren_Round_Close)    \
  X(CB_Token_Symbol_Paren_Square_Open)    \
  X(CB_Token_Symbol_Paren_Square_Close)   \
  X(CB_Token_Symbol_Paren_Curly_Open)     \
  X(CB_Token_Symbol_Paren_Curly_Close)    \
                                          \
  X(CB_Token_Sizeof)                      \
  X(CB_Token_Typedef)                     \
  X(CB_Token_Struct)                      \
  X(CB_Token_Union)                       \
  X(CB_Token_Enum)                        \
                                          \
  X(CB_Token_ControlFlow_Break)           \
  X(CB_Token_ControlFlow_Continue)        \
  X(CB_Token_ControlFlow_Return)          \
  X(CB_Token_ControlFlow_Goto)            \
  X(CB_Token_ControlFlow_If)              \
  X(CB_Token_ControlFlow_Else)            \
  X(CB_Token_ControlFlow_Switch)          \
  X(CB_Token_ControlFlow_Case)            \
  X(CB_Token_ControlFlow_Default)         \
  X(CB_Token_ControlFlow_Do)              \
  X(CB_Token_ControlFlow_While)           \
  X(CB_Token_ControlFlow_For)             \
                                          \
  X(CB_Token_Type_Int)                    \
  X(CB_Token_Type_Auto)                   \
  X(CB_Token_Type_Void)                   \
  X(CB_Token_Type_Char)                   \
  X(CB_Token_Type_Float)                  \
  X(CB_Token_Type_Double)                 \
                                          \
  X(CB_Token_Literal_Integer)             \
  X(CB_Token_Literal_Real)                \
  X(CB_Token_Literal_String)              \
  X(CB_Token_Literal_Identifier)          \
                                          \
  X(CB_Token_Modifier_Long)               \
  X(CB_Token_Modifier_Short)              \
  X(CB_Token_Modifier_Signed)             \
  X(CB_Token_Modifier_Unsigned)           \
  X(CB_Token_Modifier_Const)              \
  X(CB_Token_Modifier_Static)             \
  X(CB_Token_Modifier_Extern)             \
  X(CB_Token_Modifier_Restrict)           \
  X(CB_Token_Modifier_Volatile)           \
  X(CB_Token_CompilerDirective_Attribute)

#define X(Type) Type,
typedef uint8_t CB_Token;
enum {
  CB_TOKEN_LIST(X)
};
#undef X

static void cb_cparser_parse(const char *file);

internal CB_Handle cb_cparser_preprocess(const char *file);
internal CB_Token cb_cparser_lexer_token_matches(const char *rest_of_token,
                                                 int32_t length_of_token,
                                                 int32_t lexer_head_skip,
                                                 CB_Token type_if_match);
internal CB_Token cb_clexer_get_token(void);
internal CB_Token cb_clexer_get_token_keyword(void);
internal const char* cb_clexer_token_string(CB_Token token);


internal struct {
  const char *head;
  const char *offset;
} cb_clexer = {0};

static void cb_cparser_parse(const char *file) {
  CB_Handle handle_file = cb_cparser_preprocess(file);
  const char *content = cb_handle_read(handle_file);
  cb_println(CB_LogLevel_Info, "%s preprocessed content [[\n%s]]", file, content);

  cb_clexer.head = content;
  CB_Token token = cb_clexer_get_token();
  const char *token_str = cb_clexer_token_string(token);
  printf("Token read is: %s\n", token_str);
  cb_clexer.head += (size_t)cb_clexer.offset;

  cb_handle_close(handle_file);
}


internal CB_Handle cb_cparser_preprocess(const char *file) {
  CB_Handle preprocessed_file = cb_handle_open_tmp();
  CB_Cmd proc_preprocessor = {0};
#if OS_WINDOWS
#else
  cb_cmd_append(&proc_preprocessor, "gcc", "-E", "-P", (char *)file);
  if (cb_cmd_run(&proc_preprocessor, .stdout = preprocessed_file).status_code) {
    cb_cmd_append(&proc_preprocessor, "clang", "-E", "-P", (char *)file);
    cb_assert(!cb_cmd_run(&proc_preprocessor, .stdout = preprocessed_file).status_code
              && "Install a compiler idk what to tell you.");
  }
#endif
  return preprocessed_file;
}

internal inline bool cb_clexer_numeric(char ch) { return ch >= '0' && ch <= '9'; }
internal inline bool cb_clexer_alphabetic(char ch) { return (ch >= 'a' && ch <= 'z') ||
                                                            (ch >= 'A' && ch <= 'Z') ||
                                                            (ch == '_'); }
internal inline bool cb_clexer_alphanumeric(char ch) { return cb_clexer_alphabetic(ch) ||
                                                              cb_clexer_numeric(ch); }

internal CB_Token cb_clexer_get_token(void) {
  for (; *cb_clexer.head == ' '  || *cb_clexer.head == '\t' ||
         *cb_clexer.head == '\r' || *cb_clexer.head == '\n';
       ++cb_clexer.head);
  cb_clexer.offset = 0;

  if (cb_clexer_alphabetic(*cb_clexer.head)) { return cb_clexer_get_token_keyword(); }

  return CB_Token_Symbol_Eof;
}

internal CB_Token cb_clexer_get_token_keyword(void) {
  for (;
       cb_clexer_alphanumeric(*(cb_clexer.head + (size_t)cb_clexer.offset));
       cb_clexer.offset += 1);

  switch (*cb_clexer.head) {
  case 'a': {
    return cb_cparser_lexer_token_matches("uto", 3, 1, CB_Token_Type_Auto);
  } break;
  case 'b': {
    return cb_cparser_lexer_token_matches("reak", 4, 1, CB_Token_ControlFlow_Break);
  } break;
  case 'c': {
    switch (*(cb_clexer.head + 1)) {
    case 'a': {
      return cb_cparser_lexer_token_matches("se", 2, 2, CB_Token_ControlFlow_Case);
    } break;
    case 'h': {
      return cb_cparser_lexer_token_matches("ar", 2, 2, CB_Token_Type_Char);
    } break;
    case 'o': {
      if (*(cb_clexer.head + 2) != 'n') { return CB_Token_Literal_Identifier; }
      switch (*(cb_clexer.head + 3)) {
      case 's': {
        return cb_cparser_lexer_token_matches("t", 1, 4, CB_Token_Modifier_Const);
      } break;
      case 't': {
        return cb_cparser_lexer_token_matches("inue", 4, 4, CB_Token_ControlFlow_Continue);
      } break;
      }
    } break;
    }
  } break;
  case 'd': {
    switch (*(cb_clexer.head + 1)) {
    case 'o': {
      if (cb_cparser_lexer_token_matches("", 0, 2, CB_Token_ControlFlow_Do) == CB_Token_Literal_Identifier) {
        return cb_cparser_lexer_token_matches("uble", 4, 2, CB_Token_Type_Double);
      }
    } break;
    case 'e': {
      return cb_cparser_lexer_token_matches("fault", 5, 2, CB_Token_ControlFlow_Default);
    } break;
    }
  } break;
  case 'e': {
    switch (*(cb_clexer.head + 1)) {
    case 'l': {
      return cb_cparser_lexer_token_matches("se", 2, 2, CB_Token_ControlFlow_Else);
    } break;
    case 'n': {
      return cb_cparser_lexer_token_matches("um", 2, 2, CB_Token_Enum);
    } break;
    case 'x': {
      return cb_cparser_lexer_token_matches("tern", 4, 2, CB_Token_Modifier_Extern);
    } break;
    }
  } break;
  case 'f': {
    switch (*(cb_clexer.head + 1)) {
    case 'l': {
      return cb_cparser_lexer_token_matches("oat", 3, 2, CB_Token_Type_Float);
    } break;
    case 'o': {
      return cb_cparser_lexer_token_matches("r", 1, 2, CB_Token_ControlFlow_For);
    } break;
    }
  } break;
  case 'g': {
    return cb_cparser_lexer_token_matches("oto", 3, 1, CB_Token_ControlFlow_Goto);
  } break;
  case 'i': {
    switch (*(cb_clexer.head + 1)) {
    case 'f': {
      return cb_cparser_lexer_token_matches("", 0, 2, CB_Token_ControlFlow_If);
    } break;
    case 'n': {
      return cb_cparser_lexer_token_matches("t", 1, 2, CB_Token_Type_Int);
    } break;
    }
  } break;
  case 'l': {
    return cb_cparser_lexer_token_matches("ong", 3, 1, CB_Token_Modifier_Long);
  } break;
  case 'r': {
    if (*(cb_clexer.head + 1) != 'e') { return CB_Token_Literal_Identifier; }
    switch (*(cb_clexer.head + 2)) {
    case 'g': {
      return cb_cparser_lexer_token_matches("ister", 5, 3, CB_Token_ControlFlow_If);
    } break;
    case 't': {
      return cb_cparser_lexer_token_matches("urn", 3, 3, CB_Token_ControlFlow_Return);
    } break;
    }
  } break;
  case 's': {
    switch (*(cb_clexer.head + 1)) {
    case 'h': {
      return cb_cparser_lexer_token_matches("ort", 3, 2, CB_Token_Modifier_Short);
    } break;
    case 'i': {
      switch (*(cb_clexer.head + 2)) {
      case 'g': {
        return cb_cparser_lexer_token_matches("ned", 3, 3, CB_Token_Modifier_Signed);
      } break;
      case 'z': {
        return cb_cparser_lexer_token_matches("eof", 3, 3, CB_Token_Sizeof);
      } break;
      }
    } break;
    case 't': {
      switch (*(cb_clexer.head + 2)) {
      case 'a': {
        return cb_cparser_lexer_token_matches("tic", 3, 3, CB_Token_Modifier_Static);
      } break;
      case 'r': {
        return cb_cparser_lexer_token_matches("uct", 3, 3, CB_Token_Struct);
      } break;
      }
    } break;
    case 'w': {
      return cb_cparser_lexer_token_matches("itch", 4, 2, CB_Token_ControlFlow_Switch);
    } break;
    }
  } break;
  case 't': {
    return cb_cparser_lexer_token_matches("ypedef", 6, 1, CB_Token_Typedef);
  } break;
  case 'u': {
    if (*(cb_clexer.head + 1) != 'n') { return CB_Token_Literal_Identifier; }
    switch (*(cb_clexer.head + 2)) {
    case 'i': {
      return cb_cparser_lexer_token_matches("on", 2, 3, CB_Token_Union);
    } break;
    case 's': {
      return cb_cparser_lexer_token_matches("igned", 5, 3, CB_Token_Modifier_Unsigned);
    } break;
    }
  } break;
  case 'v': {
    if (*(cb_clexer.head + 1) != 'o') { return CB_Token_Literal_Identifier; }
    switch (*(cb_clexer.head + 2)) {
    case 'i': {
      return cb_cparser_lexer_token_matches("d", 1, 3, CB_Token_Type_Void);
    } break;
    case 'l': {
      return cb_cparser_lexer_token_matches("atile", 5, 3, CB_Token_Modifier_Volatile);
    } break;
    }
  } break;
  case 'w': {
    return cb_cparser_lexer_token_matches("hile", 4, 1, CB_Token_ControlFlow_While);
  } break;
  }
  return CB_Token_Literal_Identifier;
}

internal CB_Token cb_cparser_lexer_token_matches(const char *rest_of_token,
                                                 int32_t length_of_token,
                                                 int32_t lexer_head_skip,
                                                 CB_Token type_if_match) {
  if (length_of_token != (size_t)cb_clexer.offset - lexer_head_skip) { return CB_Token_Literal_Identifier; }
  for (int32_t i = 0; i < length_of_token; ++i) {
    if ((cb_clexer.head + lexer_head_skip)[i] != rest_of_token[i]) { return CB_Token_Literal_Identifier; }
  }
  return type_if_match;
}

internal const char* cb_clexer_token_string(CB_Token token) {
#define X(Token) case Token: return #Token;
  switch (token) {
    CB_TOKEN_LIST(X)
  }
#undef X
}

#endif
