#!/bin/bash
assert() {
	expected="$1"
	input="$2"

	./acc "$input" > tmp.s
	cc -o tmp tmp.s
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$input => $expected expected, but got $actual"
		exit 1
	fi
}

assert 0 0
assert 42 42
assert 34 34
assert 30 "5+10+25-10"
assert 10 "240-235+5"
assert 30 "5 + 10 + 25  - 10"

echo OK
