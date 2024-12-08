//lab group zero code
//known issues: can't parse negative sign when used in front of parenthesis
//issue parsing negatives: -14+16=16
//parenthesis error when paras are at the beggining of the equation (3-4)=0 and dealing with negatives
// para error when dealing with paras in paras 2**(2*(1+2))=1
//no decimal support

#include <iostream>
using namespace std;

const int bufferSize = 30;
bool divideByZero = false;

//this function will get the input
//input: char []
//output: char []
void getInput(char input[]) {

    //clear the buffer
    for (int count = 0; count < bufferSize; count++) {
        input[count] = ' ';
    }

    //recieve input
    cout << "Enter the expression (or 'END' to quit): ";
    cin.getline(input, bufferSize);

    // check buffer overflow
    if (cin.fail()) {
        cin.clear(); // clear the buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear the whole stream
        cout << "Your input Exceeded the max length of " << bufferSize << " characters.\n";
        getInput(input);
    }
}

//remove all blank spaces from an array
//input char[]
//output char[]
void removeBlank(char input[]) {
    int blanksRemoved = 0;

    for (int i = 0; i < bufferSize; ++i) {
        input[i - blanksRemoved] = input[i];
        if (input[i] == 0 || input[i] == '\0' || input[i] == ' ' || input[i] == 32) {
            blanksRemoved++;
        }
    }
    for (int i = bufferSize - blanksRemoved; i < bufferSize; ++i) {
        input[i] = 0;

    }
    // Null-terminate the resulting string
    //input[bufferSize- blanksRemoved-1] = '\0';
}


//remove all blank spaces from an 2d array
//input char[][]
//output char[][]
void removeBlankSquare(char input[][bufferSize]) {
    int blanksRemoved = 0;

    for (int i = 0; i < bufferSize; ++i) {
        for (int j = 0; j < bufferSize; ++j) {
            input[i - blanksRemoved][j] = input[i][j];
        }
        if (input[i][0] == 0 || input[i][0] == '\0' || input[i][0] == ' ' || input[i][0] == 32) {
            blanksRemoved++;
        }
    }
    for (int i = bufferSize - blanksRemoved; i < bufferSize; ++i) {
        for (int j = 0; j < bufferSize; ++j) {

            input[i][j] = 0;
        }
    }
}



//convert from character[] to intergers
//input char []
//output int
int charToInt(char input[]) {
    int output = 0;
    int orderOfMagnitude = 1;
    int temp;
    bool negativeFound = false;

    //preform computations left of the decimal
    for (int count = bufferSize - 1; count >= 0; count--) {
        //check for negative sign
        if (input[count] == '-') {
            negativeFound = true;
        }
        //check for numbers
        else if (input[count] >= 48 && input[count] <= 57) {
            //get value
            temp = abs(input[count] - 48);
            //set order of magnintude
            output += temp * orderOfMagnitude;
            //increment oom
            orderOfMagnitude *= 10;
        }
    }

    if (negativeFound) {
        output *= -1;
    }

    return output;
}

//convert from int to char[]
//input int
//output char[]
void  intToChar(int input, char output[]) {
    int temp;
    bool negativeFound = false;

    //check for negative value
    if (input < 0) {
        negativeFound = true;
    }
    input = abs(input);

    //preform computations left of the decimal
    for (int count = 0; count < bufferSize; count++) {
        //add negative sign
        if (input == 0 && negativeFound) {
            output[count] = '-';
            negativeFound = false;
        }
        else {
            temp = input % 10;
            output[count] = temp + 48;
            input /= 10;
        }
    }

    //reverse string
    for (int count = 0; count < bufferSize / 2 - 1; count++) {
        char temp = output[count];
        output[count] = output[bufferSize - 1 - count];
        output[bufferSize - 1 - count] = temp;
    }

    bool valueFound = false;
    while (output[0] == '0') {
        for (int count = 0; count < bufferSize - 1; count++) {
            output[count] = output[count + 1];
        }
        output[bufferSize - 1] = 0;
    }

    if (true);

}


//determine whether or not the input is valid
//input char[]
//output bool
bool checkInputAndTokenize(char input[], char tokens[][bufferSize]) {

    //tokenize using a 2 stack push down automota (equivilient to turning machine)
    int state = 0; //state of the pda
    int paraStack = 0; //stack 1 used to match parenthesis
    int repetitionStack = 0;

    //iterate through the whole input
    for (int count = 0; count < bufferSize; count++) {
        //define each state
        //q0    ( was the last char/start state
        if (state == 0) {
            //check each input symbol
            //check for (
            if (input[count] == '(') {
                state = 0;
                paraStack++;
                tokens[count][0] = input[count];
            }
            //check for numbers
            else if (input[count] >= 48 && input[count] <= 57) {
                state = 1;
                tokens[count][0] = input[count];
            }
            //check for -
            else if (input[count] == '-') {
                state = 2;
                tokens[count][0] = input[count];
            }
            //check for missing operand
            else if (input[count] == '*' || input[count] == '/' || input[count] == '+' || input[count] == '%') {
                state = 6;
            }
            //check for missing operator
            else if (input[count] == ')') {
                state = 7;
            }
            //check for blank space found at end of file
            else if (input[count] == 0 || input[count] == '\0' || input[count] == ' ' || input[count] == 32) {

            }
            //other symbol
            else {
                state = 8;
            }
        }
        //q1 #
        else if (state == 1) {
            //check each input symbol
            //check for (
            if (input[count] == '(') {
                state = 0;
                paraStack++;
                repetitionStack = 0;
                tokens[count][0] = input[count];
            }
            //check for numbers
            else if (input[count] >= 48 && input[count] <= 57) {
                state = 1;
                repetitionStack++;
                tokens[count- repetitionStack][repetitionStack] = input[count];
            }
            //check for non negative operator
            else if (input[count] == '*' || input[count] == '/' || input[count] == '+' || input[count] == '-' || input[count] == '%') {
                repetitionStack = 0;
                tokens[count][0] = input[count];
                state = 3;
            }
            //check for )
            else if (input[count] == ')') {
                repetitionStack = 0;
                paraStack--;
                tokens[count][0] = input[count];
                state = 4;
            }
            //check for missing operator
            else if (input[count] == '(') {
                state = 7;
            }
            //check for blank space found at end of file
            else if (input[count] == 0 || input[count] == '\0' || input[count] == ' ' || input[count] == 32) {

            }
            //other symbol
            else {
                state = 8;
            }
        }
        //q2 - (negative)
        else if (state == 2) {
            //check each input symbol
            //check for (
            if (input[count] == '(') {
                state = 0;
                paraStack++;
                tokens[count][0] = input[count];
            }
            //check for numbers
            else if (input[count] >= 48 && input[count] <= 57) {
                state = 1;
                repetitionStack++;
                tokens[count - repetitionStack][repetitionStack] = input[count];
            }
            //check for missing operand
            else if (input[count] == ')' || input[count] == '-' || input[count] == '*' || input[count] == '/' || input[count] == '+' || input[count] == '%') {
                state = 6;
            }
            //check for blank space found at end of file
            else if (input[count] == 0 || input[count] == '\0' || input[count] == ' ' || input[count] == 32) {

            }
            //other symbol
            else {
                state = 8;
            }
        }
        //q3 +, /, * -(minus)
        else if (state == 3) {
            //check each input symbol
            //check for (
            if (input[count] == '(') {
                state = 0;
                paraStack++;
                tokens[count][0] = input[count];
            }
            //check for numbers
            else if (input[count] >= 48 && input[count] <= 57) {
                tokens[count][0] = input[count];
                state = 1;
            }
            //check for - (negative)
            else if (input[count] == '-' && input[count - 1] != '-') {
                tokens[count][0] = input[count];
                state = 2;
            }
            //check for ** exponenet
            else if (input[count] == '*' && input[count-1] == '*') {
                tokens[count-1][1] = input[count];
                state = 5;
            }
            //check for missing operand
            else if (input[count] == ')' || input[count] == '-' || input[count] == '*' || input[count] == '/' || input[count] == '+' || input[count] == '%') {
                state = 6;
            }
            //check for blank space found at end of file
            else if (input[count] == 0 || input[count] == '\0' || input[count] == ' ' || input[count] == 32) {

            }
            //other symbol
            else {
                state = 8;
            }
        }
        //q4 )
        else if (state == 4) {
            //check each input symbol
            //check for operator
            if (input[count] == '*' || input[count] == '/' || input[count] == '+' || input[count] == '-' || input[count] == '%') {
                tokens[count][0] = input[count];
                state = 3;
            }
            //check for )
            else if (input[count] == ')') {
                paraStack--;
                tokens[count][0] = input[count];
                state = 4;
            }
            //check for missing operator
            else if ((input[count] >= 48 && input[count] <= 57) || input[count] == '(') {
                state = 7;
            }
            //check for blank space found at end of file
            else if (input[count] == 0 || input[count] == '\0' || input[count] == ' ' || input[count] == 32) {

            }
            //other symbol
            else {
                state = 8;
            }
        }
        //q5 ** exponent
        else if (state == 5) {
            //check each input symbol
            //check for (
            if (input[count] == '(') {
                state = 0;
                paraStack++;
                tokens[count][0] = input[count];
            }
            //check for numbers
            else if (input[count] >= 48 && input[count] <= 57) {
                tokens[count][0] = input[count];
                state = 1;
            }
            //check for -
            else if (input[count] == '-') {
                tokens[count][0] = input[count];
                state = 2;
            }
            //check for missing operand
            else if (input[count] == ')' || input[count] == '*' || input[count] == '/' || input[count] == '+' || input[count] == '%') {
                state = 6;
            }
            //check for blank space found at end of file
            else if (input[count] == 0 || input[count] == '\0' || input[count] == ' ' || input[count] == 32) {

            }
            //other symbol
            else {
                state = 8;
            }
        }
        //q6 missing Operand
        else if (state == 6) {
            cout << "\n ERROR: Missing Operand\n";
            return false;
        }
        //q7 missing operantor
        else if (state == 7) {
            cout << "\n ERROR: Missing Operator\n";
            return false;
        }
        //q8 invalid
        else if (state == 8) {
            cout << "\n ERROR: Invalid Character '" << input[count-1] << "' \n";
            cout << "\n Input: " << input << "\n";


            return false;
        }
        //unknown error
        else {
            cout << "\nERROR: You shouldn't be here\n";
            return false;

        }
        //check for missmatched parenthesis
        if (paraStack < 0) {
            cout << "\nERROR: Missmatched Parenthesis\n";
            return false;
        }


    }

    //check that the stack is empty and it is in an accepting state.
    if (paraStack != 0) {
        cout << "\nERROR: Missmatched Parenthesis\n";
        return false;
    }
    if (state != 1 && state != 4) {
        cout << "\n ERROR: Missing Operand\n";
        return false;
    }
    return true;

}


int divide(int leftA, int rightA) {

    if (rightA == 0) {
        divideByZero = true;
        cout << "\nERROR: Divide by Zero\n";
        return 0;
    }
    int result = leftA / rightA;
    return result;
}

int add(int left, int right) {
    return left + right;
}

int subtract(int left, int right) {
    return left - right;
}

int multiply(int left, int right) {
    return left * right;
}

int modulo(int left, int right) {
    return left % right;
}

int power(int left, int right) {
    //Base cases.
    if (left == 0) return 0;
    if (right == 0) return 1;
    int result = 1;
    for (int i = 0; i < right; i++) {
        result *= left;
    }
    return result;
}

int solve(char input[][bufferSize]);

void paraSearch(char input[][bufferSize]) {
    while (true) {
        // Find the first opening parenthesis '('
        int leftmost = -1;
        for (int i = 0; i < bufferSize; ++i) {
            if (input[i][0] == '(') {
                leftmost = i;
                break;
            }
        }

        if (leftmost == -1) {
            break;
        }

        // Find the corresponding closing parenthesis ')'
        int rightmost = -1;
        for (int i = leftmost + 1; i < bufferSize; ++i) {
            if (input[i][0] == ')') {
                rightmost = i;
                break;
            }
        }

        // Extract the subexpression between parentheses
        char subexpression[bufferSize][bufferSize] = {};
        int subSize = 0;

        for (int i = leftmost + 1; i < rightmost; ++i) {
            for (int j = 0; j < bufferSize; ++j) {
                subexpression[subSize][j] = input[i][j];
            }
            ++subSize;
        }

        // Solve the subexpression
        int result = solve(subexpression);

        // Replace the parenthesized subexpression with the result
        char resultStr[bufferSize];
        int temp = result;
        int idx = 0;
        if (temp < 0) {
            resultStr[idx++] = '-';
            temp = -temp;
        }
        char buffer[bufferSize] = {};
        int bufferIdx = 0;
        do {
            buffer[bufferIdx++] = '0' + (temp % 10);
            temp /= 10;
        } while (temp > 0);
        while (bufferIdx > 0) {
            resultStr[idx++] = buffer[--bufferIdx];
        }
        resultStr[idx] = '\0';
        for (int j = 0; j < bufferSize; ++j) {
            input[leftmost][j] = resultStr[j];
        }

        // Mark the remaining tokens as empty
        for (int i = leftmost + 1; i <= rightmost; ++i) {
            input[i][0] = '\0';
        }

        // Compact the array to remove blanks
        int writeIndex = 0;
        for (int i = 0; i < bufferSize; ++i) {
            if (input[i][0] != '\0') {
                if (writeIndex != i) {
                    for (int j = 0; j < bufferSize; ++j) {
                        input[writeIndex][j] = input[i][j];
                        input[i][j] = '\0';
                    }
                }
                ++writeIndex;
            }
        }
    }
}


int solve(char input[][bufferSize]) {
    paraSearch(input);
    // Handle exponentiation (** operator)
    for (int i = 0; i < bufferSize && input[i][0] != '\0'; ++i) {
        if (input[i][0] == '*' && input[i][1] == '*' && input[i][2] == '\0') {
            int base = charToInt(input[i - 1]);
            int exp = charToInt(input[i + 1]);
            int result = power(base, exp);

            char resultStr[bufferSize];
            intToChar(result, resultStr);
            for (int j = 0; j < bufferSize; ++j) {
                input[i - 1][j] = resultStr[j];
            }

            for (int k = i; k < bufferSize - 2; ++k) {
                for (int j = 0; j < bufferSize; ++j) {
                    input[k][j] = input[k + 2][j];
                }
            }

            i -= 1;
        }
    }

    // Handle multiplication, division, and modulo
    for (int i = 0; i < bufferSize && input[i][0] != '\0'; ++i) {
        if (input[i][0] == '*' || input[i][0] == '/' || input[i][0] == '%') {
            int lhs = charToInt(input[i - 1]);
            int rhs = charToInt(input[i + 1]);
            int result = 0;

            if (input[i][0] == '*') {
                result = multiply(lhs, rhs);
            }
            else if (input[i][0] == '/') {
                result = divide(lhs, rhs);
            }
            else if (input[i][0] == '%') {
                result = modulo(lhs, rhs);
            }

            char resultStr[bufferSize];
            intToChar(result, resultStr);
            for (int j = 0; j < bufferSize; ++j) {
                input[i - 1][j] = resultStr[j];
            }

            for (int k = i; k < bufferSize - 2; ++k) {
                for (int j = 0; j < bufferSize; ++j) {
                    input[k][j] = input[k + 2][j];
                }
            }

            i -= 1;
        }
    }

    // Handle addition and subtraction
    for (int i = 0; i < bufferSize && input[i][0] != '\0'; ++i) {
        if (input[i][0] == '+' || input[i][0] == '-') {
            int lhs = charToInt(input[i - 1]);
            int rhs = charToInt(input[i + 1]);
            int result = 0;

            if (input[i][0] == '+') {
                result = add(lhs, rhs);
            }
            else if (input[i][0] == '-') {
                result = subtract(lhs, rhs);
            }

            char resultStr[bufferSize];
            intToChar(result, resultStr);
            for (int j = 0; j < bufferSize; ++j) {
                input[i - 1][j] = resultStr[j];
            }

            for (int k = i; k < bufferSize - 2; ++k) {
                for (int j = 0; j < bufferSize; ++j) {
                    input[k][j] = input[k + 2][j];
                }
            }

            i -= 1;
        }
    }

    return charToInt(input[0]);
}



int main(){

    char input[bufferSize] = { ' ' } ;
    bool continueProgram = true;



    
    while (true) {
        char tokens[bufferSize][bufferSize] = { ' ' };
        bool valid = false;
        while (!valid) {
            getInput(input);
            removeBlank(input);

            //check if user wishes to exit the program
            if (input[0] == 'E' && input[1] == 'N' && input[2] == 'D') {
                continueProgram = false;
                break;
            }

            valid = checkInputAndTokenize(input, tokens);
            //valid = true;
            removeBlankSquare(tokens);

        }
        //exit if user requests
        if (continueProgram == false) {
            break;
        }

        int result = solve(tokens);

        if (divideByZero == false) {
            cout << input << "=" << result << endl << endl;
        }
        else {
            divideByZero = false;
        }
        if (true);

    }

}

