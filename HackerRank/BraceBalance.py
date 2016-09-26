# Complete the function below.


def  braces(values):
    response_array = []
    
    for bracket_string in values:
        failure = 0
        string_len = len(bracket_string)
        bracket_stack = []
        for i in range(string_len):
            if bracket_string[i] == "{":
                bracket_stack.append("{")
            if bracket_string[i] == "[":
                bracket_stack.append("[")
            if bracket_string[i] == "(":
                bracket_stack.append("(")
            if len(bracket_stack) == 0:
                failure = 1
                break
            if bracket_string[i] == "}":
                n = bracket_stack.pop()
                if n != "{":
                    failure = 1
                    break
            if bracket_string[i] == ")":
                n = bracket_stack.pop()
                if n != "(":
                    failure = 1
                    break
            if bracket_string[i] == "]":
                n = bracket_stack.pop()
                if n != "[":
                    failure = 1
                    break
        if len(bracket_stack) != 0:
            failure = 1
        if failure:
            response_array.append("NO")
        else:
            response_array.append("YES")
    return response_array
