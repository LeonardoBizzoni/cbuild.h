#include "../../cbuild.h"

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  cb_fd file = cb_open("ls-curdir-log", CB_ACF_READ | CB_ACF_WRITE);

  cb_cmd cmd = {};
  cb_cmd_append(&cmd, "ls", "-lah", ".");
  cb_run(&cmd, .stdout = file);

  cb_cmd_append(&cmd, "cat");
  cb_run(&cmd, .stdin = file);

  cb_close(file);
}
