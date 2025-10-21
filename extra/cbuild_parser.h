#ifndef CBUILD_EXTRA_PARSER_H
#define CBUILD_EXTRA_PARSER_H

typedef uint8_t CB_Token;
enum {
  CB_Token_Symbol_Invalid = 0,
  CB_Token_Symbol_Eof,
  CB_Token_Symbol_Semicolon,
  CB_Token_Symbol_Comma,
  CB_Token_Symbol_Star,
  CB_Token_Symbol_Paren_Round_Open,
  CB_Token_Symbol_Paren_Round_Close,
  CB_Token_Symbol_Paren_Square_Open,
  CB_Token_Symbol_Paren_Square_Close,
  CB_Token_Symbol_Paren_Curly_Open,
  CB_Token_Symbol_Paren_Curly_Close,

  CB_Token_Typedef,
  CB_Token_Struct,
  CB_Token_Union,
  CB_Token_Enum,

  CB_Token_Type_Int,
  CB_Token_Type_Void,
  CB_Token_Type_Char,
  CB_Token_Type_Float,
  CB_Token_Type_Double,

  CB_Token_Modifier_Long,
  CB_Token_Modifier_Short,
  CB_Token_Modifier_Signed,
  CB_Token_Modifier_Unsigned,
  CB_Token_Modifier_Const,
  CB_Token_Modifier_Static,
  CB_Token_Modifier_Extern,
  CB_Token_Modifier_Restrict,
  CB_Token_Modifier_Volatile,

#if !OS_WINDOWS
  CB_Token_CompilerDirective_Attribute,
#endif
};

struct CB_Lexer {
  char *start;
  char *curr;
};

internal CB_Handle cb_cparser_preprocess(const char *file);
internal void cb_cparser_lexer_skip_whitespace(struct CB_Lexer *lexer);
internal CB_Token cb_cparser_lexer_token_get(struct CB_Lexer *lexer);

static void cb_cparser_parse(const char *file) {
  CB_Handle handle_file = cb_cparser_preprocess(file);
  const char *content = cb_handle_read(handle_file);
  cb_println(CB_LogLevel_Info, "%s preprocessed content [[\n%s]]", file, content);

  struct CB_Lexer lexer = {0};
  lexer.start = (char *)content;
  cb_cparser_lexer_token_get(&lexer);

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

internal void cb_cparser_lexer_skip_whitespace(struct CB_Lexer *lexer) {
  for (; *lexer->start == ' ' || *lexer->start == '\t' ||
         *lexer->start == '\r' || *lexer->start == '\n';
       ++lexer->start);
  lexer->curr = lexer->start;
}

internal CB_Token cb_cparser_lexer_token_get(struct CB_Lexer *lexer) {
  cb_cparser_lexer_skip_whitespace(lexer);
  // TODO(lb): big switch statement || lookup table?
}

#endif
