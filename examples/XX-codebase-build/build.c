#include "../../cbuild.h"

#define Codebase_Path "-I./src/base"
#define Codebase_Module_Gui "/DOS_GUI=1"
#define Codebase_Module_Sound "/DOS_SOUND=1"

#define Infile  "src/main.c"
#define Outfile "main"

#if OS_WINDOWS
#  define SystemSharedLibs "/link", "Ws2_32.lib"
#  define GenericFlags "/Zc:preprocessor", "/RTC1", "/GA", "/Gw", \
                       "/permissive", "/fastfail", "/sdl"
#  define ReleaseFlags "/O2", "/Ot", "/Ob3", "/GL", "/Qpar", \
                       "/Qspectre-load", "/Qspectre-load-cf"
#  define DebugFlags "/Od", "/Zi", "/fsanitize=address", \
                     "/DDEBUG=1", "/DENABLE_ASSERT=1"
#  define CppFlags "/TP", "/std:c++latest"
#  define CAnnoyingWarnings "/wd4477", "/wd4996"
#  define CppAnnoyingWarnings
#  define Output "/Fe" Outfile ".exe"
#else
#  define SystemSharedLibs "-lpthread", "-lm"
#  define GenericFlags "-pedantic", "-Wall", "-Werror", "-Wextra",         \
                       "-Wconversion", "-Wdouble-promotion", "-Wshadow",   \
                       "-Wundef", "-Wcast-qual", "-Wmissing-declarations", \
                       "-Wredundant-decls"
#  define ReleaseFlags "-O3", "-s", "-march=native", "-flto", \
                       "-D_FORTIFY_SOURCE=2", "-Wno-unused-variable"
#  define DebugFlags "-O0", "-g3", "-ggdb", "-DENABLE_ASSERT=1",       \
                     "-DDEBUG=1", "-fsanitize=address,undefined,leak", \
                     "-fsanitize-trap", "-fstack-protector-strong"
#  define CppFlags "-std=c++23", "-fno-exceptions"
#  define CAnnoyingWarnings "-Wno-unused-function",                   \
                            "-Wno-initializer-overrides",             \
                            "-Wno-c23-extensions",                    \
                            "-Wno-gnu-zero-variadic-macro-arguments", \
                            "-Wno-sign-conversion",                   \
                            "-Wno-unused-parameter"
#  define CppAnnoyingWarnings CAnnoyingWarnings            \
                              "-Wno-gnu-anonymous-struct", \
                              "-Wno-gnu-anonymous-struct", \
                              "-Wno-nested-anon-types"
#  define Output "-o", Outfile

#  define OPENGL "-lGL", "-lGLU", "-DUSING_OPENGL=1"
#  if OS_LINUX
#    define X11 "-DLNX_X11=1", "-lX11", "-lXext"
#    define Wayland "-DLNX_WAYLAND=1", "-lxkbcommon"
#  elif OS_BSD
#    define X11 "-DBSD_X11=1", "-lX11", "-lXext"
#    define Wayland "-DBSD_WAYLAND=1", "-lxkbcommon"
#  elif OS_MAC
#    error I dont have a mac
#  endif
#endif

int main(int argc, char **argv) {
  cb_rebuild_self(argc, argv);

  bool dbg   = true;
  bool cpp   = false;
  bool clang = true;
  bool gui   = false;
  bool sound = false;

  for (int32_t i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "release")) {
      dbg = false;
    } else if (!strcmp(argv[i], "cpp")) {
      cpp = true;
    } else if (!strcmp(argv[i], "gcc")) {
      clang = false;
    } else if (!strcmp(argv[i], "gui")) {
      gui = true;
    } else if (!strcmp(argv[i], "sound")) {
      sound = true;
    }
  }

  cb_cmd cmd = {};
  cb_cmd_append(&cmd, "git", "submodule", "update", "--recursive");
  cb_proc_handle codebase_updater = cb_run(&cmd, .async = true);

#if OS_WINDOWS
  cb_cmd_push(&cmd, "cl.exe");
#else
  if (clang) {
    cb_cmd_push(&cmd, "clang");
  } else {
    cb_cmd_push(&cmd, "gcc");
  }
#endif

  cb_cmd_append(&cmd, Output);
  cb_cmd_append(&cmd, Infile, Codebase_Path);
  cb_cmd_append(&cmd, GenericFlags, SystemSharedLibs);

  if (cpp) {
    cb_cmd_append(&cmd, CppFlags, CppAnnoyingWarnings);
  } else {
    cb_cmd_append(&cmd, CAnnoyingWarnings);
  }

  if (dbg) {
    cb_cmd_append(&cmd, DebugFlags);
  } else {
    cb_cmd_append(&cmd, ReleaseFlags);
  }

  if (gui) {
    cb_cmd_append(&cmd, Codebase_Module_Gui);
    cb_cmd_append(&cmd, OPENGL);
#if OS_LINUX || OS_BSD
    cb_cmd_append(&cmd, !strcmp(cb_getenv("XDG_SESSION_TYPE"), "x11")
                        ? X11 : Wayland);
#endif
  }

  if (sound) {
    cb_cmd_append(&cmd, Codebase_Module_Sound);
  }

  cb_proc_wait(codebase_updater);
  cb_run(&cmd);
}
