#ifndef CBUILD_EXTRA_CODE_GENERATION_H
#define CBUILD_EXTRA_CODE_GENERATION_H

typedef struct {
  char **values;
  size_t count;
  size_t capacity;

  size_t total_length;
  struct {
    int32_t has_args;
    int32_t spacing;
  } signature;
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

  gen->signature.has_args = 0;
  gen->signature.spacing = 0;
  gen->total_length = 0;
}

static void cb_gen_push(CB_Generator *gen, char *string) {
  cb_dyn_push(gen, string);
  gen->total_length += strlen(string);
}

static void cb_gen_sig_begin(CB_Generator *gen, char *signature_no_args) {
  cb_gen_push(gen, cb_format("%s(", signature_no_args));
  gen->signature.spacing = strlen(signature_no_args) + 1;
}

static void cb_gen_sig_arg(CB_Generator *gen, char *argument) {
  if (gen->signature.has_args > 0 && gen->signature.has_args % 4 == 0) {
    cb_gen_push(gen, ",\n");
    cb_gen_push(gen, cb_format("%*s", gen->signature.spacing, ""));
  } else if (gen->signature.has_args) {
    cb_gen_push(gen, ", ");
  }
  gen->signature.has_args += 1;
  cb_gen_push(gen, argument);
}

static void cb_gen_sig_end(CB_Generator *gen, bool implementation) {
  if (!gen->signature.has_args) {
    cb_gen_push(gen, "void");
  }
  cb_gen_push(gen, ")");
  if (!implementation) {
    cb_gen_push(gen, ";\n");
  }
  gen->signature.has_args = 0;
  gen->signature.spacing = 0;
}

#endif
