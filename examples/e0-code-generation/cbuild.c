#include "../../cbuild.h"
#include "../../extra/cbuild_codegen.h"
#include "../../extra/cbuild_linagen.h"

int32_t main(int32_t argc, char **argv) {
  cb_rebuild_self(argc, argv);

  if (!cb_file_exists("linear_algebra.h")) {
    CB_Generator vec = {0};
    cb_gen_push(&vec, "#ifndef CB_LINEAR_ALGEBRA_H\n"
                      "#define CB_LINEAR_ALGEBRA_H\n\n");

    cb_gen_push(&vec, "#define linagen_fn static inline\n\n");
    cb_linagen_typedef_vecn_unnamed(&vec, "float", 2);
    cb_linagen_typedef_vecn(&vec, "float", 3, "x", "y", "z");
    cb_linagen_typedef_vecn(&vec, "float", 5, "x", "y", "z", 0);
    cb_linagen_typedef_matnn(&vec, "float", 3);

    cb_linagen_defun_vecn_add(&vec, "float", 2, false);
    cb_linagen_defun_vecn_sub(&vec, "float", 2, false);
    cb_linagen_defun_vecn_hadamard_prod(&vec, "float", 2, false);
    cb_linagen_defun_vecn_hadamard_div(&vec, "float", 2, false);
    cb_linagen_defun_vecn_scale(&vec, "float", 2, false);
    cb_linagen_defun_vecn_equal(&vec, "float", 2, false);
    cb_linagen_defun_vecn_near(&vec, "float", 2, false);
    cb_linagen_defun_vecn_lerp(&vec, "float", 2, false);
    cb_linagen_defun_vecn_normalize(&vec, "float", 2, false);
    cb_linagen_defun_vecn_dot(&vec, "float", 2, false);
    cb_linagen_defun_vecn_magnitude(&vec, "float", 2, false);
    cb_linagen_defun_vecn_magnitude64(&vec, "float", 2, false);
    cb_linagen_defun_vecn_distance(&vec, "float", 2, false);
    cb_linagen_defun_vecn_distance2(&vec, "float", 2, false);
    cb_gen_push(&vec, "\n");

    cb_linagen_defun_vecn_add(&vec, "float", 3, false);
    cb_linagen_defun_vecn_sub(&vec, "float", 3, false);
    cb_linagen_defun_vecn_hadamard_prod(&vec, "float", 3, false);
    cb_linagen_defun_vecn_hadamard_div(&vec, "float", 3, false);
    cb_linagen_defun_vecn_scale(&vec, "float", 3, false);
    cb_linagen_defun_vecn_equal(&vec, "float", 3, false);
    cb_linagen_defun_vecn_near(&vec, "float", 3, false);
    cb_linagen_defun_vecn_lerp(&vec, "float", 3, false);
    cb_linagen_defun_vecn_normalize(&vec, "float", 3, false);
    cb_linagen_defun_vec3_cross(&vec, "float", false);
    cb_linagen_defun_vecn_dot(&vec, "float", 3, false);
    cb_linagen_defun_vecn_mulm(&vec, "float", 3, false);
    cb_linagen_defun_vecn_magnitude(&vec, "float", 3, false);
    cb_linagen_defun_vecn_magnitude64(&vec, "float", 3, false);
    cb_linagen_defun_vecn_distance(&vec, "float", 3, false);
    cb_linagen_defun_vecn_distance2(&vec, "float", 3, false);
    cb_gen_push(&vec, "\n");

    cb_linagen_defun_vecn_add(&vec, "float", 5, false);
    cb_linagen_defun_vecn_sub(&vec, "float", 5, false);
    cb_linagen_defun_vecn_hadamard_prod(&vec, "float", 5, false);
    cb_linagen_defun_vecn_hadamard_div(&vec, "float", 5, false);
    cb_linagen_defun_vecn_scale(&vec, "float", 5, false);
    cb_linagen_defun_vecn_equal(&vec, "float", 5, false);
    cb_linagen_defun_vecn_near(&vec, "float", 5, false);
    cb_linagen_defun_vecn_lerp(&vec, "float", 5, false);
    cb_linagen_defun_vecn_normalize(&vec, "float", 5, false);
    cb_linagen_defun_vecn_dot(&vec, "float", 5, false);
    cb_linagen_defun_vecn_magnitude(&vec, "float", 5, false);
    cb_linagen_defun_vecn_magnitude64(&vec, "float", 5, false);
    cb_linagen_defun_vecn_distance(&vec, "float", 5, false);
    cb_linagen_defun_vecn_distance2(&vec, "float", 5, false);
    cb_gen_push(&vec, "\n");

    cb_linagen_defun_matnn_add(&vec, "float", 3, false);
    cb_linagen_defun_matnn_sub(&vec, "float", 3, false);
    cb_linagen_defun_matnn_hadamard_prod(&vec, "float", 3, false);
    cb_linagen_defun_matnn_hadamard_div(&vec, "float", 3, false);
    cb_linagen_defun_matnn_scale(&vec, "float", 3, false);
    cb_linagen_defun_matnn_dot(&vec, "float", 3, false);
    cb_linagen_defun_matnn_mulv(&vec, "float", 3, false);
    cb_linagen_defun_matnn_identity(&vec, "float", 3, false);
    cb_gen_write(&vec, "linear_algebra.h", false);

    cb_gen_push(&vec, "\n#ifndef CB_LINEAR_ALGEBRA_NO_IMPL\n");
    cb_linagen_defun_vecn_add(&vec, "float", 2, true);
    cb_linagen_defun_vecn_sub(&vec, "float", 2, true);
    cb_linagen_defun_vecn_hadamard_prod(&vec, "float", 2, true);
    cb_linagen_defun_vecn_hadamard_div(&vec, "float", 2, true);
    cb_linagen_defun_vecn_scale(&vec, "float", 2, true);
    cb_linagen_defun_vecn_equal(&vec, "float", 2, true);
    cb_linagen_defun_vecn_near(&vec, "float", 2, true);
    cb_linagen_defun_vecn_lerp(&vec, "float", 2, true);
    cb_linagen_defun_vecn_normalize(&vec, "float", 2, true);
    cb_linagen_defun_vecn_dot(&vec, "float", 2, true);
    cb_linagen_defun_vecn_magnitude(&vec, "float", 2, true);
    cb_linagen_defun_vecn_magnitude64(&vec, "float", 2, true);
    cb_linagen_defun_vecn_distance(&vec, "float", 2, true);
    cb_linagen_defun_vecn_distance2(&vec, "float", 2, true);
    cb_gen_push(&vec, "\n");

    cb_linagen_defun_vecn_add(&vec, "float", 3, true);
    cb_linagen_defun_vecn_sub(&vec, "float", 3, true);
    cb_linagen_defun_vecn_hadamard_prod(&vec, "float", 3, true);
    cb_linagen_defun_vecn_hadamard_div(&vec, "float", 3, true);
    cb_linagen_defun_vecn_scale(&vec, "float", 3, true);
    cb_linagen_defun_vecn_equal(&vec, "float", 3, true);
    cb_linagen_defun_vecn_near(&vec, "float", 3, true);
    cb_linagen_defun_vecn_lerp(&vec, "float", 3, true);
    cb_linagen_defun_vecn_normalize(&vec, "float", 3, true);
    cb_linagen_defun_vec3_cross(&vec, "float", true);
    cb_linagen_defun_vecn_dot(&vec, "float", 3, true);
    cb_linagen_defun_vecn_mulm(&vec, "float", 3, true);
    cb_linagen_defun_vecn_magnitude(&vec, "float", 3, true);
    cb_linagen_defun_vecn_magnitude64(&vec, "float", 3, true);
    cb_linagen_defun_vecn_distance(&vec, "float", 3, true);
    cb_linagen_defun_vecn_distance2(&vec, "float", 3, true);
    cb_gen_push(&vec, "\n");

    cb_linagen_defun_vecn_add(&vec, "float", 5, true);
    cb_linagen_defun_vecn_sub(&vec, "float", 5, true);
    cb_linagen_defun_vecn_hadamard_prod(&vec, "float", 5, true);
    cb_linagen_defun_vecn_hadamard_div(&vec, "float", 5, true);
    cb_linagen_defun_vecn_scale(&vec, "float", 5, true);
    cb_linagen_defun_vecn_equal(&vec, "float", 5, true);
    cb_linagen_defun_vecn_near(&vec, "float", 5, true);
    cb_linagen_defun_vecn_lerp(&vec, "float", 5, true);
    cb_linagen_defun_vecn_normalize(&vec, "float", 5, true);
    cb_linagen_defun_vecn_dot(&vec, "float", 5, true);
    cb_linagen_defun_vecn_magnitude(&vec, "float", 5, true);
    cb_linagen_defun_vecn_magnitude64(&vec, "float", 5, true);
    cb_linagen_defun_vecn_distance(&vec, "float", 5, true);
    cb_linagen_defun_vecn_distance2(&vec, "float", 5, true);
    cb_gen_push(&vec, "\n");

    cb_linagen_defun_matnn_add(&vec, "float", 3, true);
    cb_linagen_defun_matnn_sub(&vec, "float", 3, true);
    cb_linagen_defun_matnn_hadamard_prod(&vec, "float", 3, true);
    cb_linagen_defun_matnn_hadamard_div(&vec, "float", 3, true);
    cb_linagen_defun_matnn_scale(&vec, "float", 3, true);
    cb_linagen_defun_matnn_dot(&vec, "float", 3, true);
    cb_linagen_defun_matnn_mulv(&vec, "float", 3, true);
    cb_linagen_defun_matnn_identity(&vec, "float", 3, true);
    cb_gen_push(&vec, "#endif // CB_LINEAR_ALGEBRA_NO_IMPL\n");
    cb_gen_push(&vec, "#endif // CB_LINEAR_ALGEBRA_H\n");
    cb_gen_write(&vec, "linear_algebra.h", true);
  }

  CB_Cmd cmd = {0};
#if OS_WINDOWS
  cb_cmd_append(&cmd, "cl", "/Fe:main", "main.c");
#else
  cb_cmd_append(&cmd, "gcc", "-o", "main", "main.c", "-lm");
#endif
  return cb_cmd_run(&cmd).status_code;
}
