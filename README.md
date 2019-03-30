# Scallion

Scallion is an experimental program language currently being maintained and developed by Alex Bruns. It's main feature is syntactical richness. Unlike many languages which prioritize readability over information density, Scallion chooses to maximize info-per-character via some unique syntax structures. 

## Getting Started

Run the following commands in a bash terminal to try compiling in program:
- `git clone https://github.com/AHBruns/Scallion`
- `cd Scallion`
- `./cscallion src/programs/prog1.sc --all`

## General Use

The compiler is `./cscallion`. It can be re-built from source via `gcc -o cscallion src/main.c`. When called it takes one required arg, the relative path of the file you want to compile. Additionally, cscallion can take 3 different flags:
- `--lex` : print out the lexed program representation used during parsing 
- `--ast` : print out the abstract-syntax-tree (ast) formed by parsing
- `--all` : print out all intermediate program representations created during compilation

### Prerequisites

- GCC

## Testing

##### TODO

## Contributing

Shoot me an email at, Alex.H.Bruns@gmail.com, or connect on twitter, @AlexHBruns.

##### TODO

## Versioning

##### TODO

## Authors

* **Alex Bruns** - *Sole Developer* - [LinkedIn](https://www.linkedin.com/in/alexbruns/) | [Twitter](https://twitter.com/AlexHBruns) | [Company](https://topl.co/)

## License

##### TODO

## Acknowledgments

* I'd like to say thank you to Nick Hopper, my university professor. Nick, without your course, Advanced Programming Principles, I wouldn't have known where to begin with this project.
* Additionally, I owe a big thank you to all the people working on CPython. I tested all sorts of weird operator orderings in your repl.
