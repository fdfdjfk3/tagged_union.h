# tagged_union.h

I wanted to see if it was possible to implement some kind of 'safe' tagged union
data type in pure C, and here's the result. It's... uh... far from perfect,
but it's close enough lol. This header relies on some boost preprocessor
headers because I don't wanna end up in a mental institution trying to
hand-roll my own macro trickery :P.

Included macros:

```c
tagged_union(name, pairs)
/// Macro that expands to a tagged union declaration; which is 
/// a struct with an enum field containing one value for each field in 
/// the union, and the union itself, with the fields named after each
/// member of the enum.
```
```c
tagged_union_from_enum(name, enumtype, pairs)
/// Like tagged_union, but it uses a given enum type as the backing enum.
```
```c
TUnionGet(t_union, variant, type)
/// Gets the value in the tagged union. If it's an inactive field, an error is 
/// issued and the program exits.
```
```c
TUnionLit(variant, literal)
/// Expands to a compound literal of a tagged union. Use this instead of 
/// manually writing out compound literals to avoid silly mistakes.
/// literals to avoid silly mistakes.
```
```c
TUnionActive(t_union)
/// Expands to the active field enum
```
```c
TUnionSet(t_union, variant, value)
/// Modifies the current variant's value. If the variant is different
/// than the one stored, it will be an error.
```

## Example usage:

```c
#include "tagged_union.h"
#include <stdio.h>

typedef tagged_union(test, (
	(THING1, int),
	(THING2, struct { int bla_bla; }),
	(THING3, float),
	(THING4, struct test *),
	(THING5, wchar_t)
)) test;

int main(void) {
	test a = TUnionLit(THING3, 5.5);
	printf("%f\n", TUnionGet(a, THING3, float));
}
```

## Closure

ok bai
