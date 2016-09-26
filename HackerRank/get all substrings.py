# Complete the function below.
all_strings = []

def buildSubsequences(s):
    isorisntthere(s, 0)
    all_strings.sort()
    return all_strings

def isorisntthere(s, index):
    if index == len(s):
        addstring = s.replace(" ", "")
        if addstring != "":
            all_strings.append(addstring)
        return
    temp = list(s)
    isorisntthere("".join(temp), index+1)
    temp[index] = " "
    isorisntthere("".join(temp), index+1)