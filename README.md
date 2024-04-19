## About the Program
dgrep is a program that creates a list of all possible strings that match a regular expression.  This project is meant mainly as a tool for teaching/exploring regular expresssions, so it will never produce control characters.  Additionally, as of now, only characters from the original ASCII-7 set are supported.  Lastly, dgrep uses regular expresssions that are modelled off of [GNU Extended Regular Expressions](https://www.gnu.org/software/grep/manual/html_node/Regular-Expressions.html).  See the of this README on [Regular Expressions](#regular-expressions) for further discussion of supported regular expression syntax.

## Usage
The standard usage of dgrep is:
```
dgrep [options] <regex>
```
Many regular expressions have an infinite number of matching strings (yikes!). To handle this, dgrep provides two options for limiting matches to the user, discussed below.

### Length Cap
All regular expressions are limited to a length of 256 characters by default.  However, you can adjust this using the -l flag like so
```
dgrep -l <length> <regex>
```
The length provided must not be negative.  Increasing the length can lead to rapid increases in program execution due to the large number of matching strings that have to be computed.  In general, it is advised to maintain a small length.

### Alphabet
dgrep also supports changing the set of characters it considers to be valid.  Thus, if you were working with digits, you could restrict the alphabet down to just digits, allowing you to easily represent all digits with the wildcard '.' in your regular expression.  The alphabet can be set using the -a flag
```
dgrep -a <bracket expression> <regex>
```
Where bracket expression is uses the syntax surrounded with '[]' inherited from [GNU Regular Expressions](https://www.gnu.org/software/grep/manual/html_node/Regular-Expressions.html).  Some examples are
```
dgrep -a [[:digit:]] <regex>
dgrep -a [0-9] <regex>
```
In addition to named classes and ranges, the bracket expression can use the negation '^' symbol, which will simply negate the bracket expression from the standard alphabet (all ASCII-7 non-control characters).  Thus, the following example will yield an alphabet that contains all characters *except* digits.
```
dgrep -a [^0-9] <regex>
```
The alphabet also restricts the characters you can put in your regular expression.  For instance, if the alphabet is restricted to just digits, you cannot include an 'a' in your regular expression.  It just wouldn't make sense!  Finally, note that characters that are not included in the standard alphabet can be inserted into a custom alphabet.  Thus, since [tab] is not included in the standard alphabet, it cannot be added to the alphabet.

### Using Options
Options must be placed before the regular expression
Traditionally, options are used like so
```
dgrep -l 4 <regex>
```
However, the space following the option flag is not required.  As such, the following are valid
```
dgrep -l4 <regex>
dgrep -a[0-9] <regex>
```
Furthermore, if your regular expression begins with a '-', you can use the -r flag to prevent it from being parsed as another option.  This can be done with or without a space.
```
dgrep -r <regex>
dgrep -r<regex>
```

## Regular Expressions
As mentioned before, dgrep endeavors to support all of the [GNU Extended Regular Expression](https://www.gnu.org/software/grep/manual/html_node/Regular-Expressions.html) syntax.  However, there are some notable exceptions.

### Bracket Expressions
The named class "[:cntrl:]" is not recognized because control characters are not supported by dgrep so the class is empty.  The collating symbols '[.' and '.]' are not supported.  The equivalence symbols '[=' and '=]' are also not supported.  Finally, the range character '-' will always use the C locale.  Further reference on the ordering of characters in the C locale may be found in the [ASCII table](https://en.cppreference.com/w/cpp/language/ascii).

### Anchoring
When used outside of a bracket expression, the anchoring characters '$' and '^' are always treated as literals.  Anchoring does not make sense in the context of this program.  However, these characters can be escaped without error for compatibility reasons.  If you intend on using a regular expression with dgrep and grep, it is highly recommended that you escape '$' and '^'.

### Special Characters
The separator symbols '\b', '\B', '\\<', and '\\>' are not currently supported.  The space symbols '\s' and '\S' are supported, howerver the only character that is considered a space is ' ' since all the other whitespace characters are considered to be control characters and thus unsupported.
