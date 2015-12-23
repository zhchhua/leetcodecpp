ass Solution {
public:
bool isNumber(const char *s) {
enum InputType {
INVALID,
SPACE,
SIGN,
DIGIT,
DOT,
EXPONENT,
NUM_INPUTS
};
const int transitionTable[][NUM_INPUTS] = {
-1, 0, 3, 1, 2, -1,
-1, 8, -1, 1, 4, 5,
-1, -1, -1, 4, -1, -1,
-1, -1, -1, 1, 2, -1,
-1, 8, -1, 4, -1, 5,
-1, -1, 6, 7, -1, -1,
-1, -1, -1, 7, -1, -1,
-1, 8, -1, 7, -1, -1,
-1, 8, -1, -1, -1, -1,
};
int state = 0;
for (; *s != '\0'; ++s) {
InputType inputType = INVALID;
if (isspace(*s))
inputType = SPACE;
else if (*s == '+' || *s == '-')
inputType = SIGN;
else if (isdigit(*s))
inputType = DIGIT;
else if (*s == '.')
inputType = DOT;
else if (*s == 'e' || *s == 'E')
inputType = EXPONENT;
state = transitionTable[state][inputType];
if (state == -1) return false;
}
return state == 1 || state == 4 || state == 7 || state == 8;
}
};
