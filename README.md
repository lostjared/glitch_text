# glitch_text

Three C++ command-line tools for text effects:

- `case` continuously shuffles words and alternates uppercase and lowercase characters.
- `scramble` shuffles the interior letters of each word while preserving its first and last letters.
- `rev-text` reverses the entire input, including spaces, punctuation, and newlines.

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
extensions, and defines three executable targets:

| Target / executable | Source | Purpose |
| --- | --- | --- |
| `case` | `case.cpp` | Continuously shuffle words and alternate character case. |
| `scramble` | `scramble.cpp` | Shuffle each word's interior letters once. |
| `rev-text` | `rev.cpp` | Reverse the entire input once. |

To build just one program, add `--target case`, `--target scramble`, or
`--target rev-text` to the build command.

The examples below use a single-configuration
build, which places the executables in `build/`. With a multi-configuration
generator such as Visual Studio, use `build/Release/case.exe`,
`build/Release/scramble.exe`, and `build/Release/rev-text.exe` instead.
`CMAKE_BUILD_TYPE` selects Release for
single-configuration generators; `--config Release` selects it for
multi-configuration generators.

To choose a compiler explicitly, use a fresh build directory, for example:

```sh
cmake -S . -B build-clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
cmake --build build-clang --config Release
```

## Usage

All three tools read standard input until end-of-file before producing output. Use a
finite input source, such as a text file or a piped phrase.

### Alternating case and shuffled words

```text
case --upper DELAY
case --lower DELAY
```

| Argument | Meaning |
| --- | --- |
| `--upper` | Start the first shuffled pass with an uppercase character. |
| `--lower` | Start the first shuffled pass with a lowercase character. |
| `DELAY` | Nonnegative integer delay in microseconds after each output chunk of up to 16 bytes. Use `0` for no intentional delay. |

In a Unix-like shell:

```sh
printf '%s\n' 'Hello world from glitch text' | ./build/case --upper 50000
./build/case --lower 100000 < input.txt
```

In PowerShell (Visual Studio build):

```powershell
'Hello world from glitch text' | .\build\Release\case.exe --upper 50000
Get-Content -Raw input.txt | .\build\Release\case.exe --lower 100000
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

### Combining scrambling and alternating case

With `scramble` and `case` on your `PATH`, pipe a file through both tools:

```sh
cat file.txt | scramble | env case --upper 1000
```

`env case` invokes the executable because `case` is a shell keyword.

`scramble` shuffles each word's interior letters once, then `case` continuously
shuffles the resulting words and alternates their case, starting with uppercase.
The delay is 1,000 microseconds (1 millisecond) after each output chunk of up to
16 bytes. Press **Ctrl+C** to stop.

To use the executables directly from a single-configuration build:

```sh
cat file.txt | ./build/scramble | ./build/case --upper 1000
```

## Input handling

- For `case` and `scramble`, words are runs of alphabetic characters. Numbers, punctuation, and whitespace
  act as separators and are omitted from the output.
- For `case` and `scramble`, input containing no alphabetic words exits successfully without output.
- `case` and `scramble` process bytes using C/C++ character classification and are intended
  for ASCII text rather than Unicode-aware text transformation.
- `rev-text` reverses every input byte, without filtering or splitting words.
- All three tools buffer all input before producing output. Empty input produces
  no output.

## Clean

```sh
cmake --build build --target clean
```

This removes compiled build outputs while keeping the CMake configuration.
The project currently defines no CMake install target.

## License

[BSD 2-Clause](LICENSE), copyright (c) 2026 Jared Bruni.
