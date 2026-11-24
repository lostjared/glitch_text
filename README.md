# glitch_text

A C++ command-line utility that turns text into a continuous stream of shuffled
words with alternating uppercase and lowercase characters. Use it to create a
glitch-style text effect in your terminal.

## Build

Requirements:

- A C++ compiler that supports the Makefile's `-std=c++23` flag and C++ ranges.
- GNU Make.

```sh
make all
```

This builds the `twist-app` executable. To choose a compiler explicitly:

```sh
make all CXX=clang++
```

Alternatively, compile directly:

```sh
g++ -std=c++23 -O3 case.cpp -o twist-app
```

## Usage

```text
twist-app --upper DELAY
twist-app --lower DELAY
```

The program reads text from standard input until end-of-file, then repeatedly
shuffles and prints its words. Press **Ctrl+C** to stop it.

| Argument | Meaning |
| --- | --- |
| `--upper` | Start the first shuffled pass with an uppercase character. |
| `--lower` | Start the first shuffled pass with a lowercase character. |
| `DELAY` | Nonnegative integer delay in microseconds after each output chunk of up to 16 bytes. Use `0` for no intentional delay. |

Pipe a short phrase into the program:

```sh
printf '%s\n' 'Hello world from glitch text' | ./twist-app --upper 50000
```

Read from a text file:

```sh
./twist-app --lower 100000 < input.txt
```

You can also run `./twist-app --upper 50000`, type or paste text, and send
end-of-file (**Ctrl+D** on Unix-like terminals) to start the effect.

## Output behavior

- Words are runs of alphabetic characters. Numbers, punctuation, and whitespace
  act as separators and are omitted from the output.
- Each pass randomly shuffles the words and joins them with spaces, including a
  trailing space.
- Letter case alternates at every character position, including spaces. For
  example, a pass ordered as `hello world` with `--upper` produces `HeLlO WoRlD `.
- The starting case flips after every pass.
- Output continues without added newlines, so the terminal wraps the stream.
- Input containing no alphabetic words exits successfully without output.

The tool processes bytes using C/C++ character classification; it is intended
for ASCII text rather than Unicode-aware text transformation. It buffers all
input before producing output, so use a finite input source.

## Install and clean

```sh
make install
```

Copies `twist-app` to `/usr/local/bin`. Writing to that directory may require
administrator privileges.

```sh
make clean
```

Removes the executable and object files.

## License

[BSD 2-Clause](LICENSE), copyright (c) 2026 Jared Bruni.
