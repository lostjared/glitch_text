# glitch_text

Four C++ command-line tools for text effects:

- `twist-app` continuously shuffles words and alternates uppercase and lowercase characters.
- `scramble` shuffles the interior letters of each word while preserving its first and last letters.
- `rev-text` reverses the entire input, including spaces, punctuation, and newlines.
- `switch-case` alternates uppercase and lowercase characters in one pass, preserving input order and whitespace.

## Build

Requirements:

- CMake 3.13 or newer.
- A C++20 compiler and standard library with support for C++ ranges.
- A build tool supported by your CMake generator, such as Make, Ninja, or
  Visual Studio's MSBuild.

Run these commands from the repository root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

`CMakeLists.txt` sets C++20 as required, disables compiler-specific language
extensions, and defines four executable targets:

| Target / executable | Source | Purpose |
| --- | --- | --- |
| `twist-app` | `twist-app.cpp` | Continuously shuffle words and alternate character case. |
| `scramble` | `scramble.cpp` | Shuffle each word's interior letters once. |
| `rev-text` | `rev.cpp` | Reverse the entire input once. |
| `switch-case` | `switch_case.cpp` | Alternate character case once without shuffling. |

To build just one program, add `--target twist-app`, `--target scramble`,
`--target rev-text`, or `--target switch-case` to the build command.

The examples below use a single-configuration
build, which places the executables in `build/`. With a multi-configuration
generator such as Visual Studio, use `build/Release/twist-app.exe`,
`build/Release/scramble.exe`, `build/Release/rev-text.exe`, and
`build/Release/switch-case.exe` instead.
`CMAKE_BUILD_TYPE` selects Release for
single-configuration generators; `--config Release` selects it for
multi-configuration generators.

To choose a compiler explicitly, use a fresh build directory, for example:

```sh
cmake -S . -B build-clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build build-clang --config Release
```

## Install and uninstall

Choose an install prefix when configuring, then build and install:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build --config Release
cmake --build build --config Release --target install
```

This installs `twist-app`, `scramble`, `rev-text`, and `switch-case` into the prefix's `bin`
directory by default. Add that directory to your `PATH` to run the tools from
anywhere. Set `CMAKE_INSTALL_BINDIR` at configure time to change the binary
directory. On Windows, choose a writable prefix such as `C:/Tools/glitch_text`.
System-wide prefixes may require elevated permissions for installation and
uninstallation.

Uninstall using the same build directory:

```sh
cmake --build build --config Release --target uninstall
```

The uninstall target removes files recorded in `build/install_manifest.txt`,
leaves directories and unrelated files in place, and skips files already
removed. Keep the build directory and manifest until you uninstall. The manifest
records the most recent installation from that build directory. Both targets
support CMake's `DESTDIR` staging on Unix; use the same `DESTDIR` when uninstalling
a staged installation.

## Usage

All four tools read standard input until end-of-file before producing output. Use a
finite input source, such as a text file or a piped phrase.

### Alternating case and shuffled words

```text
twist-app --upper DELAY
twist-app --lower DELAY
```

| Argument | Meaning |
| --- | --- |
| `--upper` | Start the first shuffled pass with an uppercase character. |
| `--lower` | Start the first shuffled pass with a lowercase character. |
| `DELAY` | Nonnegative integer delay in microseconds after each output chunk of up to 16 bytes. Use `0` for no intentional delay. |

In a Unix-like shell:

```sh
printf '%s\n' 'Hello world from glitch text' | ./build/twist-app --upper 50000
./build/twist-app --lower 100000 < input.txt
```

In PowerShell (Visual Studio build):

```powershell
'Hello world from glitch text' | .\build\Release\twist-app.exe --upper 50000
Get-Content -Raw input.txt | .\build\Release\twist-app.exe --lower 100000
```

The program repeatedly shuffles the words and prints them with alternating
letter case. Press **Ctrl+C** to stop it.

- Each pass joins the shuffled words with spaces, including a trailing space.
- Case alternates at every character position, including spaces. For example,
  a pass ordered as `hello world` with `--upper` produces `HeLlO WoRlD `.
- The starting case flips after every pass.
- Output continues without added newlines, so the terminal wraps the stream.

### Scrambled letters

`scramble` takes no arguments and prints one pass, preserving word order:

```sh
printf '%s\n' 'Hello world from glitch text' | ./build/scramble
./build/scramble < input.txt
```

In PowerShell (Visual Studio build):

```powershell
'Hello world from glitch text' | .\build\Release\scramble.exe
Get-Content -Raw input.txt | .\build\Release\scramble.exe
```

Words shorter than four letters are unchanged. For longer words, only the
interior letters are shuffled; word length and the first and last letters are
preserved. A shuffle can leave a word unchanged. Output includes a space after
each word and no added newline.

### Reversed text (`rev-text`)

The program in `rev.cpp` is built as `rev-text`. It takes no arguments and reads
text from standard input, rather than accepting a filename as an argument:

```sh
cmake --build build --config Release --target rev-text
printf '%s' 'Hello world!' | ./build/rev-text
# Output: !dlrow olleH
./build/rev-text < input.txt > reversed.txt
```

In PowerShell (Visual Studio build):

```powershell
'Hello world!' | .\build\Release\rev-text.exe
Get-Content -Raw input.txt | .\build\Release\rev-text.exe
```

`rev-text` reverses the entire input once, then exits. It preserves all input
bytes, including numbers, punctuation, and whitespace, but reverses their order.
For multiple lines, this reverses both the order of the lines and the contents
of each line. A trailing input newline moves to the beginning of the output;
no newline is added. The Unix example uses `printf '%s'` to avoid a trailing
input newline; PowerShell's string pipeline adds a line ending.

The reversal operates on bytes, so use ASCII text: UTF-8 characters can be
broken by reversing their bytes. The executable name `rev-text` also distinguishes
it from the Unix `rev` utility, which reverses each line separately.

### Alternating case once (`switch-case`)

`switch-case` takes exactly one argument: `--upper` to start with uppercase or
`--lower` to start with lowercase. It reads standard input, alternates character
case once, then exits:

```sh
cmake --build build --config Release --target switch-case
printf '%s' 'Hello world!' | ./build/switch-case --upper
# Output: HeLlO WoRlD!
printf '%s' 'Hello world!' | ./build/switch-case --lower
# Output: hElLo wOrLd!
./build/switch-case --upper < input.txt > alternating.txt
```

In PowerShell (Visual Studio build):

```powershell
'Hello world!' | .\build\Release\switch-case.exe --upper
Get-Content -Raw input.txt | .\build\Release\switch-case.exe --lower
```

The program preserves character order, spaces, punctuation, numbers, and newlines.
Every byte position advances the case alternation, including nonletters; the
pattern continues across line breaks. It adds no newline. Missing, extra, or
unrecognized arguments print usage information to standard error and exit with
a failure status.

### Combining scrambling and alternating case

With `scramble` and `twist-app` on your `PATH`, pipe a file through both tools:

```sh
cat file.txt | scramble | twist-app --upper 1000
```

`twist-app` avoids conflicting with the shell keyword `case`.

`scramble` shuffles each word's interior letters once, then `twist-app` continuously
shuffles the resulting words and alternates their case, starting with uppercase.
The delay is 1,000 microseconds (1 millisecond) after each output chunk of up to
16 bytes. Press **Ctrl+C** to stop.

To use the executables directly from a single-configuration build:

```sh
cat file.txt | ./build/scramble | ./build/twist-app --upper 1000
```

## Input handling

- For `twist-app` and `scramble`, words are runs of alphabetic characters. Numbers, punctuation, and whitespace
  act as separators and are omitted from the output.
- For `twist-app` and `scramble`, input containing no alphabetic words exits successfully without output.
- `twist-app` and `scramble` process bytes using C/C++ character classification and are intended
  for ASCII text rather than Unicode-aware text transformation.
- `rev-text` reverses every input byte, without filtering or splitting words.
- `switch-case` changes case without filtering or splitting words. It uses byte-based
  C/C++ case conversion and is intended for ASCII text rather than Unicode-aware
  text transformation.
- All four tools buffer all input before producing output. Empty input produces
  no output.

## Clean

```sh
cmake --build build --target clean
```

This removes compiled build outputs while keeping the CMake configuration.

## License

[BSD 2-Clause](LICENSE), copyright (c) 2026 Jared Bruni.
