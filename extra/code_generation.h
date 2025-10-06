#ifndef CBUILD_EXTRA_CODE_GENERATION_H
#define CBUILD_EXTRA_CODE_GENERATION_H

typedef struct {
  char **values;
  size_t count;
  size_t capacity;
  size_t total_length;

  bool has_args;
} CB_Generator;

// ======================================================================
// Implementations
static void cb_gen_write(CB_Generator *gen, char *filepath, bool append_mode) {
  CB_Handle file = cb_handle_open(filepath, append_mode
                                            ? CB_AccessFlag_Append
                                            : CB_AccessFlag_Write);
  char *full_string = malloc(gen->total_length + 1);
  memset(full_string, 0, gen->total_length + 1);
  for (size_t i = 0; i < gen->count; ++i) {
    strcat(full_string, gen->values[i]);
  }
  cb_handle_write(file, full_string);
  free(full_string);
  cb_handle_close(file);
  cb_dyn_free(gen);
  gen->has_args = false;
}

static void cb_gen_push(CB_Generator *gen, char *string) {
  cb_dyn_push(gen, string);
  gen->total_length += strlen(string);
}

static void cb_gen_sig_begin(CB_Generator *gen, char *signature_no_args) {
  cb_gen_push(gen, cb_format("%s(", signature_no_args));
}

static void cb_gen_sig_arg(CB_Generator *gen, char *argument) {
  if (gen->has_args) {
    cb_gen_push(gen, ", ");
  }
  gen->has_args = true;
  cb_gen_push(gen, argument);
}

static void cb_gen_sig_end(CB_Generator *gen, bool implementation) {
  if (!gen->has_args) {
    cb_gen_push(gen, "void");
  }
  cb_gen_push(gen, ")");
  if (!implementation) {
    cb_gen_push(gen, ";\n");
  }
}

#endif
