# Nitrogen

A high level language built on [NVM](https://github.com/jonahisadev/nitrogen-vm)

In `tests/play.n` you can basically see the current features of the language

## To Do

* Revamp expression parsing.
	* Move to compiler so everything has context for variables
* Expression tree parser
	* Variables!!!
	* Add more places for epxressions, and fix current places
	* Add more operations
	* Keep track of registers for math
	* Status pointer for special cases
* Native Support (NVM needs work too)
* Arrays
* Structs
	* Access struct members
	* Allow functions to be created within structs