#ifndef CBUILD_H
#define CBUILD_H

#if defined(__GNUC__) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
#  define COMPILER_GCC 1
#elif defined(__clang__) && !defined(_MSC_VER)
#  define COMPILER_CLANG 1
#elif defined(_MSC_VER)
#  define COMPILER_CL 1
#else
#  error Unsupported compiler
#endif

#if defined(__gnu_linux__)
#  define OS_LINUX 1
#elif defined(__unix__)
#  define OS_BSD 1
#elif defined(_WIN32)
#  define OS_WINDOWS 1
#elif defined(__APPLE__)
#  define OS_MAC 1
#else
#  error Unknown operating system
#endif

#if defined(__cplusplus)
#  define CPP 1
#else
#  define CPP 0
#endif

#if !defined(COMPILER_GCC)
#  define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
#  define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_CL)
#  define COMPILER_CL 0
#endif

#if !defined(OS_LINUX)
#  define OS_LINUX 0
#endif
#if !defined(OS_BSD)
#  define OS_BSD 0
#endif
#if !defined(OS_MAC)
#  define OS_MAC 0
#endif
#if !defined(OS_WINDOWS)
#  define OS_WINDOWS 0
#endif
#if !defined(OS_NONE)
#  define OS_NONE 0
#endif

#if !CPP
#  if __STDC_VERSION__ >= 199901L
#    include <stdbool.h>
#  else
typedef enum {false, true} bool;
#  endif
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#if OS_WINDOWS
#  include <windows.h>
   typedef HANDLE cb_proc_handle;
#else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <sys/wait.h>
#  define CB_PROC_HANDLE_INVALID -1
   typedef pid_t cb_proc_handle;
#endif

#ifndef _assert_break
#  if OS_WINDOWS
#    define _assert_break() __debugbreak()
#  else
#    define _assert_break() __builtin_trap()
#  endif
#endif
#define Assert(COND) do { if (!(COND)) { _assert_break(); } } while (0)
#define StackPush(Head, Nodeptr)                                               \
  LLPushFrontCustom((Head), (Head), (Nodeptr), next)
#define StackPop(Head) (Head ? (Head = Head->next) : 0)
#define LLPushFrontCustom(Head, Last, Nodeptr, Next)                           \
  (!(Head) ? (Head) = (Nodeptr), (Last) = (Nodeptr)                            \
           : ((Nodeptr)->Next = (Head), (Head) = (Nodeptr)))
#define QueuePush(Head, Last, Nodeptr)                                         \
  LLPushBackCustom((Head), (Last), (Nodeptr), next)
#define QueuePop(Head) (Head ? (Head = Head->next) : 0)
#define LLPushBackCustom(Head, Last, Nodeptr, Next)                            \
  (!(Head) ? (Head) = (Last) = (Nodeptr)                                       \
           : ((Last) ? ((Last)->Next = (Nodeptr), (Last) = (Nodeptr))          \
                     : ((Head)->Next = (Last) = (Nodeptr))))

struct cb_path_list {
  char **values;
  size_t count;
  size_t capacity;
};
typedef struct cb_path_list cb_cmd;

struct cb_run_args {
  bool async;
  bool reset;
};

typedef struct {
  cb_proc_handle *values;
  size_t count;
  size_t capacity;
} cb_procs;

#ifndef CB_DYN_DEFAULT_CAPACITY
#  define CB_DYN_DEFAULT_CAPACITY 8
#endif

#if OS_WINDOWS
#  define CB_CMD_REBUILD_SELF(Exe_name, Builder_src) "cl.exe", "/Fe:", (Exe_name), (Builder_src)
#else
#  define CB_CMD_REBUILD_SELF(Exe_name, Builder_src) "cc", "-o", (Exe_name), (Builder_src)
#endif

#define cb_dyn_reserve(Dynarr, HowMany) cb_dyn_reserve_custom((Dynarr), (HowMany), values, count, capacity)
#define cb_dyn_free(Dynarr) cb_dyn_free_custom((Dynarr), values)
#define cb_dyn_push(Dynarr, Node) cb_dyn_push_custom((Dynarr), (Node), values, count, capacity)
#define cb_dyn_append(Dynarr, Array, Size) cb_dyn_append_custom((Dynarr), (Array), (Size), values, count, capacity)
#define cb_cmd_push(Dynarr, Value) cb_dyn_push(Dynarr, Value)
#define cb_cmd_append_dyn(Dynarr, Values, Count) cb_dyn_append((Dynarr), (Values), (Count));
#define cb_cmd_append(Dynarr, ...) cb_dyn_append((Dynarr),                                     \
                                                 ((char*[]){__VA_ARGS__}),                     \
                                                 (sizeof((char*[]){__VA_ARGS__}) / sizeof(char*)))
#define cb_rebuild_self(argc, argv) _cb_rebuild(argc, argv, __FILE__, 0)
#define cb_rebuild_self_with(argc, argv, ...) _cb_rebuild(argc, argv, __FILE__, __VA_ARGS__, 0)
#define cb_run(Cmd, ...) _cb_run((Cmd), (struct cb_run_args) {     \
                                                   .async = false, \
                                                   .reset = true,  \
                                                   __VA_ARGS__     \
                                                })
#define cb_procs_push(Dynarr, Value) cb_dyn_push(Dynarr, Value)
static void cb_procs_wait(cb_procs *procs);
static void cb_proc_wait(cb_proc_handle handle);

#define cb_dyn_free_custom(Dynarr, Values) free((Dynarr)->Values)
#define cb_dyn_reserve_custom(Dynarr, HowMany, Values, Count, Capacity)     \
  do {                                                                      \
    if (!(Dynarr)->Capacity) {                                              \
      (Dynarr)->Capacity = CB_DYN_DEFAULT_CAPACITY;                         \
    }                                                                       \
    while ((HowMany) > (Dynarr)->Capacity) {                                \
      (Dynarr)->Capacity *= 2;                                              \
    }                                                                       \
    (Dynarr)->Values = realloc((Dynarr)->Values, (Dynarr)->Capacity *       \
                                             sizeof((Dynarr)->Values[0]));  \
    Assert((Dynarr)->Values);                                               \
  } while(0)
#define cb_dyn_push_custom(Dynarr, Node, Values, Count, Capacity) \
  do {                                                            \
    cb_dyn_reserve_custom((Dynarr), (Dynarr)->Count + 1,          \
                              Values, Count, Capacity);           \
    (Dynarr)->Values[(Dynarr)->Count++] = (Node);                 \
  } while(0)
#define cb_dyn_append_custom(Dynarr, Array, Size, Values, Count, Capacity) \
  do {                                                                     \
    cb_dyn_reserve((Dynarr), (Dynarr)->Count + (Size));                    \
    memcpy((Dynarr)->Values + (Dynarr)->Count, (Array),                    \
           (Size) * sizeof((Dynarr)->Values[0]));                          \
    (Dynarr)->Count += (Size);                                             \
  } while(0)

static bool _cb_need_rebuild(char *output_path, struct cb_path_list sources);
static void _cb_rebuild(int argc, char **argv, char *cb_src, ...);
static cb_proc_handle _cb_run(cb_cmd *cmd, struct cb_run_args args);

static void _cb_rebuild(int argc, char **argv, char *builder_src, ...) {
  Assert(argc >= 1);
  char *exe_name = argv[0];

  struct cb_path_list sources = {};
  cb_dyn_push(&sources, builder_src);

  va_list args;
  va_start(args, builder_src);
  for (;;) {
    char *path = va_arg(args, char*);
    if (!path) { break; }
    cb_dyn_push(&sources, path);
  }
  va_end(args);

  if (!_cb_need_rebuild(exe_name, sources)) {
    cb_dyn_free(&sources);
    return;
  }

  cb_cmd cmd = {};
  cb_cmd_append(&cmd, CB_CMD_REBUILD_SELF(exe_name, builder_src));
  cb_run(&cmd);

  cb_cmd_push(&cmd, exe_name);
  cb_cmd_append_dyn(&cmd, argv, argc);
  (void)cb_run(&cmd);
  exit(0);
}

static bool _cb_need_rebuild(char *output_path, struct cb_path_list sources) {
#if OS_WINDOWS
  FILETIME output_mtime_large = {};
  HANDLE output_handle = CreateFileA(output_path, GENERIC_READ, FILE_SHARE_READ,
                                     0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (output_handle == INVALID_HANDLE_VALUE ||
      !GetFileTime(output_handle, 0, 0, &output_mtime_large)) {
    CloseHandle(output_handle);
    return true;
  }
  CloseHandle(output_handle);

  ULARGE_INTEGER output_mtime = {};
  output_mtime.LowPart = output_mtime_large.dwLowDateTime;
  output_mtime.HighPart = output_mtime_large.dwHighDateTime;

  for (size_t i = 0; i < sources.count; ++i) {
    FILETIME source_mtime_large = {};
    HANDLE source_handle = CreateFileA(sources.values[i], GENERIC_READ, FILE_SHARE_READ,
                                       0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (source_handle == INVALID_HANDLE_VALUE ||
        !GetFileTime(source_handle, 0, 0, &source_mtime_large)) {
      CloseHandle(output_handle);
      return true;
    }
    CloseHandle(output_handle);

    ULARGE_INTEGER source_mtime = {};
    source_mtime.LowPart = source_mtime_large.dwLowDateTime;
    source_mtime.HighPart = source_mtime_large.dwHighDateTime;
    if (output_mtime.QuadPart > source_mtime.QuadPart) {
      return true;
    }
  }
  return false;
#else
  // NOTE(lb): on `fstat` failure assume needed rebuild
  struct stat output_stat = {};
  if (stat(output_path, &output_stat) < 0) { return true; }
  for (size_t i = 0; i < sources.count; ++i) {
    struct stat source_stat = {};
    if (stat(sources.values[i], &source_stat) < 0 ||
        output_stat.st_mtime < source_stat.st_mtime) { return true; }
  }
  return false;
#endif
}

static cb_proc_handle _cb_run(cb_cmd *cmd, struct cb_run_args args) {
  cb_proc_handle res = {};

#if OS_WINDOWS
  char cmdline[MAX_PATH] = {};
  size_t offset = 0;
  for (size_t i = 0; i < cmd->count; ++i) {
    strcat(cmdline, "\"");
    strcat(cmdline, cmd->values[i]);
    strcat(cmdline, "\"");
    if (i != cmd->count - 1) { strcat(cmdline, " "); }
  }

  STARTUPINFO si = {0};
  PROCESS_INFORMATION pi = {0};
  si.cb = sizeof(si);

  CreateProcess(0, cmdline, 0, 0, false, 0, 0, 0, &si, &pi);
  CloseHandle(pi.hThread);
  res = pi.hProcess;
#else
  res = fork();
  if (!res) {
    cb_cmd _cmd = {};
    cb_cmd_append_dyn(&_cmd, cmd->values, cmd->count);
    cb_cmd_push(&_cmd, 0);
    if (execvp(_cmd.values[0], _cmd.values) < 0) {
      fprintf(stderr, "couldn't start child process `%s`: %s\n",
              _cmd.values[0], strerror(errno));
      exit(-1);
    }
    // NOTE(lb): unreachable, execvp only returns on error.
  }
#endif

  if (args.reset) {
    cmd->count = 0;
  }
  if (!args.async) {
    cb_proc_wait(res);
    res = CB_PROC_HANDLE_INVALID;
  }
  return res;
}

static void cb_proc_wait(cb_proc_handle handle) {
#if OS_WINDOWS
  WaitForSingleObject(handle, INFINITE);
  CloseHandle(handle);
#else
  Assert(waitpid(handle, 0, 0) == handle);
#endif
}

static void cb_procs_wait(cb_procs *procs) {
  for (size_t i = 0; i < procs->count; ++i) {
    cb_proc_wait(procs->values[i]);
  }
  cb_dyn_free(procs);
}

#endif
