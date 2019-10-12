# Windows Sudo

[中文版README](README.zh-CN.md)

A linux-like su/sudo on Windows. 

__This is a demo project. It may have security vulnerability. SO RUN AT YOUR OWN RISK.__

## 1. How to build?

```console
$ git clone https://github.com/DoubleLabyrinth/WindowsSudo.git
$ cd WindowsSudo
$ msbuild WindowsSudo.sln /p:Configuration=Release /p:Platform=x64  # or `x86` if you like
```

Then you will get 

```
su.exe
sudo.exe
WindowsSudo.Prompter.exe
WindowsSudo.Service.exe
```

at `bin/x64-Release` (or `bin/x86-Release`) directory.

## 2. How to use?

1. Move the four `exe` files to a directory which is 

   1. in `%PATH%` environment variable
   
   2.  is writable for __administrators ONLY__

   For example, you can place the four `exe` files to `C:\Windows\System32\`.

2. Run `cmd.exe` as administrator and install `WindowsSudo.Service.exe` as service.

   ```console
   $ cd C:\Windows\System32\
   $ WindowsSudo.Service.exe /install
   $ WindowsSudo.Service.exe /start
   ```

3. Enjoy `su` and `sudo` with yourself.

   __Example:__

   ```console
   $ sudo whoami /priv
   ```

## 3. Preview

![Preview.gif](doc/Preview.gif)
