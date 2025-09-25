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
#include <stdint.h>
#include <stdio.h>

#if OS_WINDOWS
#  include <windows.h>
#  include <direct.h>
#  define win32_stdin stdin
#  undef stdin
#  define win32_stdout stdout
#  undef stdout
#  define win32_stderr stderr
#  undef stderr
#  define _cb_platform_mkdir(Path) _mkdir((Path));
#  define MAX_ENVVAR 32767
#  define CB_PROC_INVALID INVALID_HANDLE_VALUE
#  define CB_HANDLE_INVALID INVALID_HANDLE_VALUE

   typedef HANDLE CB_Handle;
   typedef HANDLE CB_ProcHandle;
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <sys/wait.h>
#  include <sys/stat.h>
#  define _cb_platform_mkdir(Path) mkdir((Path), S_IRWXU | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH));
#  define CB_PROC_INVALID -1
#  define CB_HANDLE_INVALID -1
   typedef int32_t CB_Handle;
   typedef pid_t CB_ProcHandle;
#endif

#define internal static
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

struct CB_PathList {
  char **values;
  size_t count;
  size_t capacity;
};
typedef struct CB_PathList CB_Cmd;

struct Cb_Cmd_RunArgs {
  bool async;
  bool reset;

  CB_Handle stdin;
  CB_Handle stdout;
  CB_Handle stderr;
};

typedef struct {
  int32_t status_code;
  CB_ProcHandle handle;
} CB_Process;

typedef struct {
  CB_Process *values;
  size_t count;
  size_t capacity;
} CB_ProcessList;

typedef uint8_t CB_AccessFlag;
enum {
  CB_AccessFlag_Read       = 1 << 0,
  CB_AccessFlag_Write      = 1 << 1,
  CB_AccessFlag_Append     = 1 << 2,
};

typedef uint8_t CB_LogLevel;
enum {
  CB_LogLevel_Info,
  CB_LogLevel_Warn,
  CB_LogLevel_Error,
};

#ifndef CB_DYN_DEFAULT_CAPACITY
#  define CB_DYN_DEFAULT_CAPACITY 8
#endif

#ifndef CB_RECOMPILE_OPTIONS
#  define CB_RECOMPILE_OPTIONS
#endif

#if OS_WINDOWS
#  define CB_CMD_REBUILD_SELF(Exe_name, Builder_src) "cl.exe", "/Fe:", (Exe_name),       \
                                                     (Builder_src), CB_RECOMPILE_OPTIONS
#else
#  define CB_CMD_REBUILD_SELF(Exe_name, Builder_src) "cc", "-o", (Exe_name), (Builder_src), \
                                                     CB_RECOMPILE_OPTIONS
#endif

#define cb_dyn_reserve(Dynarr, HowMany) cb_dyn_reserve_custom((Dynarr), (HowMany), values, count, capacity)
#define cb_dyn_free(Dynarr) cb_dyn_free_custom((Dynarr), values, count)
#define cb_dyn_push(Dynarr, Node) cb_dyn_push_custom((Dynarr), (Node), values, count, capacity)
#define cb_dyn_append(Dynarr, Array, Size) cb_dyn_append_custom((Dynarr), (Array), (Size), values, count, capacity)
#define cb_cmd_push(Dynarr, Value) cb_dyn_push(Dynarr, Value)
#define cb_cmd_append_dyn(Dynarr, Values, Count) cb_dyn_append((Dynarr), (Values), (Count));
#define cb_cmd_append(Dynarr, ...) cb_dyn_append((Dynarr),                                     \
                                                 ((char*[]){__VA_ARGS__}),                     \
                                                 (sizeof((char*[]){__VA_ARGS__}) / sizeof(char*)))
#define cb_println(Level, Fmt, ...) cb_print((Level), Fmt "\n", ##__VA_ARGS__)
#define cb_rebuild_self(argc, argv) _cb_rebuild(argc, argv, __FILE__, 0)
#define cb_rebuild_self_with(argc, argv, ...) _cb_rebuild(argc, argv, __FILE__, __VA_ARGS__, 0)
#define cb_is_outdated(OutputFile, ...) _cb_is_outdated((OutputFile), __VA_ARGS__, 0)
#define cb_cmd_run(Cmd, ...) _cb_cmd_run((Cmd), (struct Cb_Cmd_RunArgs) { \
                                           .async = false,                \
                                           .reset = true,                 \
                                           __VA_ARGS__                    \
                                        })
#define cb_proclist_push(Dynarr, Value) cb_dyn_push(Dynarr, Value)

#define cb_dyn_free_custom(Dynarr, Values, Count) \
  do {                                            \
    free((Dynarr)->Values);                       \
    (Dynarr)->Count = 0;                          \
  } while (0)
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

#define cb_handle_write(Handle, Content) _is_literal(Content) ? _cb_handle_write_lit(Handle, Content) : _cb_handle_write_dyn(Handle, Content)
#define _cb_handle_write_lit(Handle, Content) \
  _cb_handle_write((Handle), (Content), (sizeof((Content)) / sizeof(*(Content))))
#define _cb_handle_write_dyn(Handle, Content) \
  _cb_handle_write((Handle), (Content), strlen((Content)))
#define _is_literal(x) _is_literal_(x)
#define _is_literal_(x) _is_literal_f(#x, sizeof(#x) - 1)

static char* cb_format(const char *format, ...);
static void cb_print(CB_LogLevel level, const char *fmt, ...);
static char* cb_getenv(char *varname);
static bool cb_setenv(char *varname, char *value);
static void cb_process_wait(CB_Process *handle);
static void cb_proclist_wait(CB_ProcessList *procs);
static CB_Handle cb_handle_open(char *path, CB_AccessFlag permission);
static void cb_handle_close(CB_Handle fd);
static char* cb_handle_read(CB_Handle fd);
static bool cb_dir_create(char *path);
static void cb_dir_delete(char *path);
static void cb_file_delete(char *path);
static bool cb_file_rename(char *path, char *to);
static bool cb_file_exists(char *path);

internal void _cb_handle_write(CB_Handle fd, char *buffer, size_t buffsize);
internal char* _cb_format(const char *format, va_list args);
internal bool _cb_need_rebuild(char *output_path, struct CB_PathList sources);
internal void _cb_rebuild(int argc, char **argv, char *cb_src, ...);
internal bool _cb_is_outdated(char *output, ...);
internal CB_Process _cb_cmd_run(CB_Cmd *cmd, struct Cb_Cmd_RunArgs args);
internal size_t _last_occurance_of(char *string, char ch);
internal bool _is_literal_f(char *str, size_t l);


// ==============================================================================
// Implementation
static char* cb_format(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *res = _cb_format(format, args);
  va_end(args);
  return res;
}

static void cb_print(CB_LogLevel level, const char *fmt, ...) {
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
  va_list args;
  switch (level) {
    case CB_LogLevel_Info: {
      printf(ANSI_COLOR_CYAN "[INFO] ");
    } break;
    case CB_LogLevel_Warn: {
      printf(ANSI_COLOR_YELLOW "[WARNING] ");
    } break;
    case CB_LogLevel_Error: {
      printf(ANSI_COLOR_RED "[ERROR] ");
    } break;
    default: printf(ANSI_COLOR_RESET); goto print_str;
  }
  printf(ANSI_COLOR_RESET);

 print_str: ;
  va_start(args, fmt);
  printf("%s", _cb_format(fmt, args));
  va_end(args);
#undef ANSI_COLOR_RED
#undef ANSI_COLOR_GREEN
#undef ANSI_COLOR_YELLOW
#undef ANSI_COLOR_BLUE
#undef ANSI_COLOR_MAGENTA
#undef ANSI_COLOR_CYAN
#undef ANSI_COLOR_RESET
}

static char* cb_getenv(char *varname) {
#if OS_WINDOWS
  char *res = malloc(MAX_ENVVAR);
  if (!GetEnvironmentVariableA(varname, res, MAX_ENVVAR)) {
    free(res);
    res = 0;
  }
  return res;
#else
  return getenv(varname);
#endif
}

static bool cb_setenv(char *varname, char *value) {
#if OS_WINDOWS
  return SetEnvironmentVariableA(varname, value);
#else
  return !setenv(varname, value, true);
#endif
}

static void cb_process_wait(CB_Process *proc) {
  if (proc->handle == CB_PROC_INVALID) {
    cb_println(CB_LogLevel_Warn, "Waiting on invalid process handle");
    return;
  }

#if OS_WINDOWS
  WaitForSingleObject(proc->handle, INFINITE);
  GetExitCodeProcess(proc->handle, &proc->status_code);
  CloseHandle(proc->handle);
#else
  int32_t status = 0;
  Assert(waitpid(proc->handle, &status, 0) == proc->handle);
  if (WIFEXITED(status)) {
    proc->status_code = WEXITSTATUS(status);
  } else if (WIFSIGNALED(status)) {
    proc->status_code = WTERMSIG(status);
  } else {
    proc->status_code = 0;
  }
#endif
}

static void cb_proclist_wait(CB_ProcessList *procs) {
  for (size_t i = 0; i < procs->count; ++i) {
    cb_process_wait(&procs->values[i]);
  }
  cb_dyn_free(procs);
}

static CB_Handle cb_handle_open(char *path, CB_AccessFlag permission) {
#if OS_WINDOWS
  SECURITY_ATTRIBUTES security_attributes = {sizeof(SECURITY_ATTRIBUTES), 0, 0};
  DWORD access_flags = 0;
  DWORD share_mode = 0;
  DWORD creation_disposition = OPEN_EXISTING;

  if(permission & CB_AccessFlag_Read) { access_flags |= GENERIC_READ; }
  if(permission & CB_AccessFlag_Write) {
    access_flags |= GENERIC_WRITE;
    creation_disposition = CREATE_ALWAYS;
  }
  if(permission & CB_AccessFlag_Append) {
    access_flags |= FILE_APPEND_DATA;
    creation_disposition = OPEN_ALWAYS;
  }
  return CreateFileA(path, access_flags,
                     share_mode, &security_attributes,
                     creation_disposition,
                     FILE_ATTRIBUTE_NORMAL, 0);
#else
  int32_t flags = O_CREAT;
  if ((permission & CB_AccessFlag_Read) &&
      ((permission & CB_AccessFlag_Write) || (permission & CB_AccessFlag_Append))) {
    flags |= O_RDWR;
  } else if (permission & CB_AccessFlag_Read) {
    flags |= O_RDONLY;
  } else if (permission & CB_AccessFlag_Write) {
    flags |= O_WRONLY | O_TRUNC;
  }
  if (permission & CB_AccessFlag_Append) { flags |= O_APPEND; }
  return open(path, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
}

static void cb_handle_close(CB_Handle fd) {
  if (fd == CB_HANDLE_INVALID) {
    cb_println(CB_LogLevel_Warn, "Closing invalid handle");
    return;
  }

#if OS_WINDOWS
  FlushFileBuffers(fd);
  CloseHandle(fd);
#else
  fsync(fd);
  close(fd);
#endif
}

static char* cb_handle_read(CB_Handle fd) {
  if (fd == CB_HANDLE_INVALID) {
    cb_println(CB_LogLevel_Warn, "Reading from invalid handle");
    return 0;
  }

#if OS_WINDOWS
  LARGE_INTEGER file_size = {0};
  GetFileSizeEx(fd, &file_size);

  uint64_t to_read = file_size.QuadPart;
  uint64_t total_read = 0;
  uint8_t *ptr = malloc(to_read + 1);

  for(;total_read < to_read;) {
    uint64_t amount64 = to_read - total_read;
    uint32_t amount32 = amount64 > 0xFFFFFFFF ? 0xFFFFFFFF : (uint32_t)amount64;
    OVERLAPPED overlapped = {0};
    DWORD bytes_read = 0;
    (void)ReadFile(fd, ptr + total_read, amount32, &bytes_read, &overlapped);
    total_read += bytes_read;
    if(bytes_read != amount32) { break; }
  }

  ptr[to_read] = 0;
  if(total_read != to_read) {
    free(ptr);
    ptr = 0;
  }
  return ptr;
#else
  struct stat file_stat;
  if (!fstat(fd, &file_stat)) {
    char *res = malloc(file_stat.st_size);
    if(pread(fd, res, file_stat.st_size, 0) >= 0) {
      return res;
    }
  }
  return 0;
#endif
}

static bool cb_dir_create(char *path) {
  int32_t mkdir_res = _cb_platform_mkdir(path);
  if (mkdir_res < 0 && errno == ENOENT) {
    size_t parent_end = _last_occurance_of(path, '/');
    if (!parent_end) { return false; }
    char *parent = malloc(parent_end + 1);
    memcpy(parent, path, parent_end);
    parent[parent_end] = 0;
    cb_dir_create(parent);
    free(parent);
    _cb_platform_mkdir(path);
  }

  return !mkdir_res;
}

static void cb_dir_delete(char *path) {
#if OS_WINDOWS
  _rmdir(path);
#else
  rmdir(path);
#endif
}

static void cb_file_delete(char *path) {
#if OS_WINDOWS
  _unlink(path);
#else
  unlink(path);
#endif
}

static bool cb_file_rename(char *path, char *to) {
#if OS_WINDOWS
  return MoveFileEx(path, to,
                    MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
#else
  return !rename(path, to);
#endif
}

static bool cb_file_exists(char *path) {
  CB_Handle file = cb_handle_open(path, CB_AccessFlag_Read);
  if (file == CB_HANDLE_INVALID) {
    return false;
  }
  cb_handle_close(file);
  return true;
}


internal bool _cb_is_outdated(char *output, ...) {
  struct CB_PathList sources = {};
  va_list args;
  va_start(args, output);
  for (;;) {
    char *path = va_arg(args, char*);
    if (!path) { break; }
    cb_dyn_push(&sources, path);
  }
  va_end(args);
  return _cb_need_rebuild(output, sources);
}

internal void _cb_handle_write(CB_Handle fd, char *buffer, size_t buffsize) {
  if (fd == CB_HANDLE_INVALID) {
    cb_println(CB_LogLevel_Warn, "Writing to invalid handle");
    return;
  }

  while (!buffer[buffsize - 1]) { buffsize -= 1; }
#if OS_WINDOWS
  uint64_t to_write = buffsize;
  uint64_t total_write = 0;
  char *ptr = buffer;
  for(;total_write < to_write;) {
    uint64_t amount64 = to_write - total_write;;
    uint32_t amount32 = amount64 > 0xFFFFFFFF ? 0xFFFFFFFF : (uint32_t)amount64;
    DWORD bytes_written = 0;
    WriteFile(fd, ptr + total_write, amount32, &bytes_written, 0);
    total_write += bytes_written;
    if(bytes_written != amount32) { break; }
  }
#else
  write(fd, buffer, buffsize);
#endif
}

internal char* _cb_format(const char *format, va_list args) {
  va_list args2;
  va_copy(args2, args);
  uint32_t needed_bytes = vsnprintf(0, 0, format, args2) + 1;
  va_end(args2);

  char *res = malloc(needed_bytes);
  (void)vsnprintf(res, needed_bytes, format, args);
  return res;
}

internal CB_Process _cb_cmd_run(CB_Cmd *cmd, struct Cb_Cmd_RunArgs args) {
  CB_Process res = {};

#if OS_WINDOWS
  char cmdline[32767] = {};
  size_t offset = 0;
  for (size_t i = 0; i < cmd->count; ++i) {
    strcat(cmdline, strstr(cmd->values[i], " ") || strstr(cmd->values[i], "\\")
                    ? cb_format("\"%s\"", cmd->values[i])
                    : cmd->values[i]);
    if (i != cmd->count - 1) { strcat(cmdline, " "); }
  }

  STARTUPINFO si = {0};
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput  = args.stdin  ? args.stdin  : GetStdHandle(STD_INPUT_HANDLE);
  si.hStdOutput = args.stdout ? args.stdout : GetStdHandle(STD_OUTPUT_HANDLE);
  si.hStdError  = args.stderr ? args.stderr : GetStdHandle(STD_ERROR_HANDLE);

  PROCESS_INFORMATION pi = {};
  if (!CreateProcessA(0, cmdline, 0, 0, TRUE, 0, 0, 0, &si, &pi)) {
    LPVOID lpMsgBuf;
    DWORD error = GetLastError();
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  0, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR)&lpMsgBuf, 0, 0);
    cb_println(CB_LogLevel_Error, "Child process `%s` creation failed with error %u: %s",
               cmd->values[0], error, lpMsgBuf);
    exit(-1);
  }

  CloseHandle(pi.hThread);
  res.handle = pi.hProcess;
#else
  res.handle = fork();
  if (res.handle < 0) {
    cb_println(CB_LogLevel_Error, "Child process `%s` creation failed with error %d: %s\n",
               cmd->values[0], errno, strerror(errno));
    exit(-1);
  } else if (!res.handle) {
    if (args.stdout) { dup2(args.stdout, STDOUT_FILENO); }
    if (args.stderr) { dup2(args.stderr, STDERR_FILENO); }
    if (args.stdin)  {
      lseek(args.stdin, 0, SEEK_SET);
      dup2(args.stdin, STDIN_FILENO);
    }

    CB_Cmd _cmd = {};
    cb_cmd_append_dyn(&_cmd, cmd->values, cmd->count);
    cb_cmd_push(&_cmd, 0);
    if (execvp(_cmd.values[0], _cmd.values) < 0) {
      cb_println(CB_LogLevel_Error, "Child process `%s` creation failed with error %d: %s\n",
                 cmd->values[0], errno, strerror(errno));
      exit(-1);
    }
    // NOTE(lb): unreachable, execvp only returns on error.
  }
#endif

  if (args.reset) {
    cmd->count = 0;
  }
  if (!args.async) {
    cb_process_wait(&res);
    res.handle = CB_PROC_INVALID;
  }
  return res;
}

internal void _cb_rebuild(int argc, char **argv, char *builder_src, ...) {
  Assert(argc >= 1);
  char *exe_name = argv[0];

  struct CB_PathList sources = {};
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
  cb_println(CB_LogLevel_Info, "rebuilding %s", exe_name);

#if OS_WINDOWS
  char *exe_name_old = cb_format("%s.old", exe_name);
  if (!cb_file_rename(exe_name, exe_name_old)) {
    cb_println(CB_LogLevel_Info, "File rename failed: %s -> %s",
               exe_name, exe_name_old);
    exit(-1);
  }
#endif

  CB_Cmd cmd = {};
  cb_cmd_append(&cmd, CB_CMD_REBUILD_SELF(exe_name, builder_src));
  cb_print(CB_LogLevel_Info, "running: `");
  for (int32_t i = 0; i < cmd.count; ++i) {
    printf("%s ", cmd.values[i]);
  }
  printf("\b`\n");
  CB_Process recompiler = cb_cmd_run(&cmd);
  if (recompiler.status_code) {
#if OS_WINDOWS
    cb_file_rename(exe_name_old, exe_name);
#endif
    exit(-1);
  }

  cb_cmd_push(&cmd, exe_name);
  cb_cmd_append_dyn(&cmd, argv, argc);
  (void)cb_cmd_run(&cmd);
  exit(0);
}

internal bool _cb_need_rebuild(char *output_path, struct CB_PathList sources) {
#if OS_WINDOWS
  FILETIME output_mtime_large = {};
  HANDLE output_handle = CreateFileA(output_path, GENERIC_READ,
                                     FILE_SHARE_READ, 0, OPEN_EXISTING,
                                     FILE_ATTRIBUTE_NORMAL, 0);
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
    HANDLE source_handle = CreateFileA(sources.values[i], GENERIC_READ,
                                       FILE_SHARE_READ, 0, OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL, 0);
    if (source_handle == INVALID_HANDLE_VALUE) { return true; }
    if (!GetFileTime(source_handle, 0, 0, &source_mtime_large)) {
      CloseHandle(output_handle);
      return true;
    }
    CloseHandle(output_handle);

    ULARGE_INTEGER source_mtime = {};
    source_mtime.LowPart = source_mtime_large.dwLowDateTime;
    source_mtime.HighPart = source_mtime_large.dwHighDateTime;
    if (output_mtime.QuadPart < source_mtime.QuadPart) {
      return true;
    }
  }
  return false;
#else
  struct stat output_stat = {};
  if (stat(output_path, &output_stat) < 0) { return true; }
  for (size_t i = 0; i < sources.count; ++i) {
    struct stat source_stat = {};
    if (stat(sources.values[i], &source_stat) < 0) {
    rebuild_failure:
      cb_println(CB_LogLevel_Error,
                 "`%s` modification time unreadable: %s",
                 sources.values[i], strerror(errno));
      exit(-1);
    }
    if (output_stat.st_mtime < source_stat.st_mtime) { return true; }
  }
  return false;
#endif
}

internal size_t _last_occurance_of(char *string, char ch) {
  char *res = string;
  for (char *curr = string; curr && *curr; ++curr) {
    if (*curr == ch) { res = curr; }
  }
  return res - string;
}

internal bool _is_literal_f(char *str, size_t l) {
  const char *e = str + l;
  if (str[0] == 'L') str++;
  if (str[0] != '"') return false;
  for (; str != e; str = strchr(str + 1, '"')) {
    if (!str) { return false; }
    for (str++;
         *str == '\f' || *str == '\n' || *str == '\r' ||
         *str == '\t' || *str == '\v';
         ++str);
    if (*str != '"') { return false; }
  }
  return true;
}

#endif
